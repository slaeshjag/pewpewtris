#ifndef __STATE_H__
#define	__STATE_H__

enum state_num {
	STATE_NUM_DUMMY,
	STATE_NUM_MAIN_MENU,
	STATE_NUM_IN_GAME,
	STATE_NUM_HIGHSCORE,
	STATE_NUM_NEW_HIGHSCORE,
};


struct state {
	enum state_num		new;
	enum state_num		old;
};

void state_init();
void state_loop();

#endif
