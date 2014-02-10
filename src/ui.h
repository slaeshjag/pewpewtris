#ifndef __UI_H__
#define	__UI_H__

#include <darnit/darnit.h>

#define	UI_MIN_ANGLE		-850
#define	UI_MAX_ANGLE		850

#define	TURRET_DEFAULT_X	264
#define	TURRET_DEFAULT_Y	204

#define	UI_GATLING_RELOAD	30
#define	UI_GAME_OVER_DELAY	2000
#define	UI_SOUND_VOLUME		ppt.config.sfx_vol


struct ui {
	int			angle;
	int			score_n;
	int			redraw;
	int			offset_x;
	int			offset_y;
	unsigned int		gatling_last;
	unsigned int		game_over;
	int			wait_for_name;

	int			turret_x;
	int			turret_y;

	int			gatling_reload_time;
	int			nuke_going;

	DARNIT_TEXT_SURFACE	*score;
	DARNIT_TEXT_SURFACE	*level;
	DARNIT_TEXT_SURFACE	*accur;
	DARNIT_TEXT_SURFACE	*highscore;
	DARNIT_TEXT_SURFACE	*pause_text;

	DARNIT_MENU		*main_menu;
	DARNIT_MENU		*highscore_name;
	DARNIT_MENU		*options_menu;
	char			highscore_str[32];

	DARNIT_MAP		*play_background;
	DARNIT_MAP		*menu_background;
	DARNIT_MAP		*highscore_background;

	DARNIT_LINE		*aim_line;
	DARNIT_TILE		*pause_bg;

	DARNIT_SPRITE		*player;
	DARNIT_SPRITE		*dragonbox;

	DARNIT_SOUND		*block_hit;
	DARNIT_SOUND		*block_explode;
	DARNIT_SOUND		*bullet_shoot;
	DARNIT_SOUND		*powerup_sound;
	DARNIT_SOUND		*shift_sound;

	DARNIT_TILE		*special_tile;
	int			special_tiles[9];

	int			mouse_x;
	int			mouse_y;
};


void ui_init();
void ui_init_playing();
void ui_init_highscore();
void ui_settings_create();
void ui_init_credits();
void ui_init_mainmenu();
void ui_loop_menu();
void ui_loop_settings();
void ui_loop_playing();
void ui_loop_highscore();
void ui_loop_credits();
void ui_draw_stats();
void ui_draw_player();

#endif
