#ifndef PLAYER_HPP_
#define  PLAYER_HPP_

#include "StateManager.hpp"

class Player
{

public:

	void play_button();
	void stop_button();
	void pause_button();

	enum State
	{
		running,
		playing,
		paused,
		stopped,
		NUMBER_OF_STATES,
		NO_STATE,
	};

	enum Signal
	{
		ON_ENTRY,
		ON_EXIT,
		ON_INIT,
		PLAY,
		STOP,
		PAUSE,
	};

private:
	StateManager<Player> itsStateManager;
public:
	Player();
private:
	bool runningHandler(int sig);
	bool playingHandler(int sig);
	bool pausedHandler(int sig);
	bool stoppedHandler(int sig);
	static StateManager<Player>::StateDesc states[];
};


#endif /*  PLAYER_HPP_ */