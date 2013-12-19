#ifndef __UI_H__
#define	__UI_H__

#include <darnit/darnit.h>

#define	UI_MIN_ANGLE		-850
#define	UI_MAX_ANGLE		850


struct ui {
	int			angle;
	int			score_n;
	int			redraw;
	DARNIT_TEXT_SURFACE	*score;
	DARNIT_TEXT_SURFACE	*level;
};


void ui_init();
void ui_draw_stats();
void ui_init_playing();
void ui_loop_playing();

#endif
