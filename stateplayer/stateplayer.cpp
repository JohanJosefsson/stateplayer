// stateplayer.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <cassert>
#include "StateManager.hpp"
#include "Player.hpp"




Player::Player()
:itsStateManager(this, states, NUMBER_OF_STATES, stopped)
{};


bool Player::runningHandler(/*Signal*/int sig)
{
	switch(sig) {

	case ON_ENTRY:
		std::cout << ("+++++ Lock Lid\n");
		return 1;
	case ON_EXIT:
		std::cout << ("----- Unlock Lid\n");
		return 1;

	case STOP:
		itsStateManager.set_next_state(stopped);
		return 1;
	}
	return 0;
}

bool Player::playingHandler(int sig)
{
	switch(sig) {
	case PAUSE:
		itsStateManager.set_next_state(paused);
		//NEW(smh, paused);
		return 1;
	default:
		return 0;
	}
}

bool Player::pausedHandler(int sig)
{
	switch(sig) {
	case PAUSE:
		itsStateManager.set_next_state(playing);
		return 1;
	default:
		return 0;
	}
}



bool Player::stoppedHandler(int sig)
{
	switch(sig) {
	case PLAY:
		itsStateManager.set_next_state(playing);
		return 1;
	default:
		return 0;
	}
}



void Player::play_button()
{
	itsStateManager.send(PLAY);
	itsStateManager.process();
}
void Player::stop_button()
{
	itsStateManager.send(STOP);
	itsStateManager.process();
}
void Player::pause_button()
{
	itsStateManager.send(PAUSE);
	itsStateManager.process();
}



StateManager<Player>::StateDesc Player::states[] = 
{
	{running, NO_STATE, NO_STATE, &Player::runningHandler, "running"},
    	{playing, running, NO_STATE, &Player::playingHandler, "playing"},
	    {paused, running, NO_STATE, &Player::pausedHandler, "paused"},
	{stopped, NO_STATE, NO_STATE, &Player::stoppedHandler, "stopped"},
};











class Switch
{
public:
	void turn_on()
	{
		itsStateManager.send(ON);
		itsStateManager.process();
	};
	void turn_off()
	{
		itsStateManager.send(OFF);
		itsStateManager.process();
	};
	void touch()
	{
		itsStateManager.send(TOUCH);
		itsStateManager.process();
	};
		enum State
	{
		top,
		  on,
		    untouched,
			touched,
		  off,



		NUMBER_OF_STATES,
		NO_STATE,
	};

	enum Signal
	{
		ON_ENTRY,
		ON_EXIT,
		ON_INIT,
		// user signals:
		ON,
		OFF,
		TOUCH,
	};



Switch()
:itsStateManager(this, states, NUMBER_OF_STATES, on)
{};

private:

	bool top_handler(int sig)
	{
		switch(sig)
		{
		case ON_ENTRY:
			std::cout << "enters top" << std::endl;
			return true;
		case ON_EXIT:
			std::cout << "exits top... IMPOSSIBLE" << std::endl;
			return true;
		case ON:
			std::cout << "top: tried to turn on" << std::endl;
			return true;
		case OFF:
			std::cout << "top: tried to turn off" << std::endl;
			return true;
		case TOUCH:
			std::cout << "top: touched" << std::endl;
			return true;
		default:
			return false;
		}
	};

	bool on_handler(int sig)
	{
		switch(sig)
		{
		case ON_ENTRY:
			std::cout << "is on" << std::endl;
			return true;
		case ON_EXIT:
			std::cout << "is off" << std::endl;
			return true;

		case ON_INIT:
			std::cout << "INIT on" << std::endl;
			break;

		case OFF:
			itsStateManager.set_next_state(off);
			return true;
		case TOUCH:
			std::cout << "on, touched" << std::endl;
			return true;
		}
		return false;
	};



	bool untouched_handler(int sig)
	{
		switch(sig)
		{
		case ON_ENTRY:
			std::cout << "enters untouched" << std::endl;
			return true;
		case ON_EXIT:
			std::cout << "leaves untouched" << std::endl;
			return true;

		case ON_INIT:
			std::cout << "INIT untouched" << std::endl;
			break;

		case TOUCH:
			itsStateManager.set_next_state(touched);
			return true;
		}
		return false;
	}



	
	bool touched_handler(int sig)
	{
		switch(sig)
		{
		case ON_ENTRY:
			std::cout << "is touched" << std::endl;
			return true;
		case ON_EXIT:
			std::cout << "leaves touched" << std::endl;
			return true;




		case ON_INIT:
			std::cout << "INIT untouched" << std::endl;
			break;

		}
		return false;
	}



	bool off_handler(int sig)
	{
		switch(sig)
		{
		case ON_ENTRY:
			std::cout << "enters off" << std::endl;
			return true;
		case ON_EXIT:
			std::cout << "exits off" << std::endl;
			return true;
		case ON:
			itsStateManager.set_next_state(on);
			return true;
		default:
			return false;
		}
	};

	StateManager<Switch> itsStateManager;
	static StateManager<Switch>::StateDesc states[];

};


StateManager<Switch>::StateDesc Switch::states[] = 
{
	{top, NO_STATE, NO_STATE, &Switch::top_handler, "top"},
		{on, top, untouched, &Switch::on_handler, "on"},
			{untouched, on, NO_STATE, &Switch::untouched_handler, "untouched"},
			{touched, on, NO_STATE, &Switch::touched_handler, "touched"},
		{off, top, NO_STATE, &Switch::off_handler, "off"},

};





int main()
{
	std::cout << "Hej" << std::endl;

	Player p;
	Player p2;

	Switch s;

	int in;

	while(std::cin >> in)
	{
		std::cout << "(" << in << ")" << std::endl;
		switch(in)
		{
		case 1:
			p.stop_button();
			break;
		case 2:
			p.play_button();
			break;
		case 3:
			p.pause_button();
			break;




		case 4:
			p2.stop_button();
			break;
		case 5:
			p2.play_button();
			break;
		case 6:
			p2.pause_button();
			break;

		case 7:
			s.turn_on();
			break;
		case 8:
			s.turn_off();
			break;
		case 9:
			s.touch();
			break;


		}
	}
	return 0;
}



