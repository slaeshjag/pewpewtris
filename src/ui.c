#include "pewpewtris.h"
#include <math.h>
#define	abs(x)		((x) < 0 ? -(x) : (x))


void ui_init() {
	ppt.ui.score = d_text_surface_new(ppt.font, 64, 240, 560, 58);
	ppt.ui.level = d_text_surface_new(ppt.font, 64, 240, 560, 130);

	return;
}


void ui_init_playing() {
	ppt.ui.angle = 0;
	ppt.ui.score_n = 0;
	ppt.ui.redraw = 1;
}


void ui_loop_playing() {
	int js0_x, js0_y;
	float a, b, t;
	DARNIT_KEYS k;

	d_joystick_get(&js0_x, &js0_y, NULL, NULL);
	if (!js0_x && !js0_y) {
		if (d_keys_get().up) {
			ppt.ui.angle -= (d_last_frame_time() / 3);
		} else if (d_keys_get().down) {
			ppt.ui.angle += (d_last_frame_time() / 3);
		}
	} else {
		a = abs(js0_x);
		b = js0_y;
		t = b / a;
		t = atanf(t);
		t *= (1800.0f / M_PI);
		ppt.ui.angle = t;
	}

	if (ppt.ui.angle <= UI_MIN_ANGLE)
		ppt.ui.angle = UI_MIN_ANGLE;
	else if (ppt.ui.angle >= UI_MAX_ANGLE)
		ppt.ui.angle = UI_MAX_ANGLE;
	
	if (d_keys_get().a) {
		k = d_keys_zero();
		k.a = 1;
		d_keys_set(k);
		bullet_fire(0, ppt.ui.angle, 500, 0, 216);
	}

	return;
}


void ui_draw_stats() {
	char buff[64];

	if (ppt.ui.redraw) {
		d_text_surface_reset(ppt.ui.score);
		d_text_surface_string_append(ppt.ui.score, "Score:");
		sprintf(buff, "%.8i", ppt.ui.score_n);
		d_text_surface_offset_next_set(ppt.ui.score, 100);
		d_text_surface_string_append(ppt.ui.score, buff);
		
		d_text_surface_reset(ppt.ui.level);
		d_text_surface_string_append(ppt.ui.level, "Level");
		d_text_surface_offset_next_set(ppt.ui.level, 100);
		sprintf(buff, "%i", ppt.level.level);
		d_text_surface_string_append(ppt.ui.level, buff);

	}

	d_text_surface_draw(ppt.ui.score);
	d_text_surface_draw(ppt.ui.level);
	
	return;
}
