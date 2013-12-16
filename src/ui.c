#include "pewpewtris.h"
#include <math.h>
#define	abs(x)		((x) < 0 ? -(x) : (x))


void ui_init_playing() {
	ppt.ui.angle = 0;
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
		bullet_fire(0, ppt.ui.angle, 500, 0, 120);
	}

	return;
}