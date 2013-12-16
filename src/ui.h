#ifndef __UI_H__
#define	__UI_H__

#define	UI_MIN_ANGLE		-800
#define	UI_MAX_ANGLE		800


struct ui {
	int			angle;
};

void ui_init_playing();
void ui_loop_playing();

#endif
