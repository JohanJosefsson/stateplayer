#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include <list>


template <class Callee>
class StateManager
{
public:
	struct StateDesc
	{
		typedef bool (Callee::*handler_t)(int sig);
		int state;
		int super_state;
		int initial_substate; // Not mandatory
		handler_t handler;
		const char* name;
	};

private:
	StateDesc * states;
	int num;
	int newTargetState;
	int curstate;
	int initial_state;
	Callee * client;
	bool is_inited;
	void init()
	{
		assert(!is_inited);

		print_topology();

		// Perform the init action
		(*client.*states[initial_state].handler)(2 /*ON_INIT*/);

		// Perform entry actions
		do_init(initial_state);
		
		curstate = initial_state;

		init_substate(curstate);
		//
	}

	void do_init(int state)
	{
		//should another state be called first?
		if(states[state].super_state < num)
		{
			do_init(states[state].super_state);
		}
		// send entry signal to the state handler

		(*client.*states[state].handler)(0 /*ON_ENTRY*/);
	}


	int isSuperStateOf(int possibleSuperState, int subState) const
	{
		int i = subState;
		do{
			i = (states+i)->super_state;
			if(i == possibleSuperState) return 1;
		}while(i < num);
		return 0;
	}

	void handleSignal(int sig)
	{

		assert(sig > 2);
		int stateFuncIdToCall;
		int stateFuncRet;

//		std::cout << "***\nHandle signal:  "<< sig << std::endl;
//		std::cout << "state before processing = " << states[curstate].name << std::endl;
		newTargetState = -1;

		// call the state function
		stateFuncIdToCall = curstate;
		do {
			assert_state();
			stateFuncRet = (*client.*states[stateFuncIdToCall].handler)(sig);
			stateFuncIdToCall = (states+stateFuncIdToCall)->super_state;
		} while (!stateFuncRet && stateFuncIdToCall < num/*NO_STATE*/);

		if(newTargetState != -1)
		{
			int s, s2, tgt;
			tgt = newTargetState;
			assert_state();
			s = curstate;
			//exit all hierarchical states
			while(!isSuperStateOf(s, tgt)) {
				// call state handler with exit signal
//				std::cout << "EXIT action of state: " << states[s].name << " " << s << std::endl;
				//assert_state(pTheStateMachine,s);
				(*client.*states[s].handler)(1);
				s = (states+s)->super_state;
			};
			// enter all hierarcical states

			while(s != tgt) {
				s2 = tgt;
				while((states+s2)->super_state != s) {
					s2 = (states+s2)->super_state;
				} // Find the right state under s
//				std::cout << "ENTRY action of state: " << states[s2].name << " " << s2 << std::endl;
				(*client.*states[s2].handler)(0);
				s = s2;
			}
			// We are now in the target state

			int s3 = tgt;
			init_substate(s3);
			curstate = s3;

		}

//		std::cout << "state after processing = "<< (states+(curstate))->name << std::endl;
	}

	void init_substate(int& initstate)
	{
					while(states[initstate].initial_substate < num)
			{
				initstate = states[initstate].initial_substate;
				(*client.*states[initstate].handler)(2 /*ON_INIT*/);
				(*client.*states[initstate].handler)(0);

			}
	}

	void print()const
	{
//		std::cout << "state = " << ((states)+(curstate))->name << std::endl;
	}

	void print_topology()const
	{
		for(int i = 0; i < num; i++)
		{
			int level = 0;
			int s = states[i].super_state;
			while(s < num)
			{
				level++;
				s = states[s].super_state;
			}



			int j = 0;
			while(j < level)
			{
				std::cout << " ";
				j++;
			}
			std::cout << states[i].name  << std::endl;

		}
	}



	void assert_state() const
	{
		assert(initial_state < num);
		assert(newTargetState < num);
		assert(curstate < num);
		assert(newTargetState < num);
		assert(num > 0 && num < 100);
		for(int i = 0; i < num; i++)
		{
			assert(states[i].state == i);
		}
	}


	std::list<int> l;

public:

	StateManager(Callee * cli, StateDesc * pStates, int num_of_states, int initial)
	{
		states = pStates;
		num = num_of_states;
		initial_state = initial;
		this->client = cli;
		is_inited = false;
	}

	void send(int sig)
	{
		l.push_back(sig);
	}

	void send_prio(int sig)
	{
		//assert(0); // not impl.
		l.push_front(sig);
	}

	void send_synchd(int sig)
	{
		assert(is_inited);
		handleSignal(sig);
	}

	void process()
	{
		if(!is_inited)
		{
			init();
			is_inited = true;
		}
		//	handleSignal(cursig);
		while(!l.empty())
		{
			handleSignal(l.front());
			l.pop_front();
		}
	}

	void set_next_state(int next)
	{
		newTargetState = next;
	}

};

#endif /* STATE_MANAGER_HPP */

