#ifndef __UI_H__
#define	__UI_H__

#include <darnit/darnit.h>

#define	UI_MIN_ANGLE		-850
#define	UI_MAX_ANGLE		850


struct ui {
	int			angle;
	int			score_n;
	int			redraw;
	int			offset_x;
	int			offset_y;
	int			game_over;
	DARNIT_TEXT_SURFACE	*score;
	DARNIT_TEXT_SURFACE	*level;
	DARNIT_TEXT_SURFACE	*highscore;

	DARNIT_MAP		*play_background;
	DARNIT_MAP		*menu_background;
	DARNIT_MAP		*highscore_background;
};


void ui_init();
void ui_init_playing();
void ui_init_highscore();
void ui_loop_menu();
void ui_loop_playing();
void ui_loop_highscore();
void ui_draw_stats();

#endif
