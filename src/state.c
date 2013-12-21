#include "pewpewtris.h"

void state_init() {
	ppt.state.new = STATE_NUM_DUMMY;
	ppt.state.new = STATE_NUM_MAIN_MENU;

	return;
}


void state_loop() {
	if (ppt.state.new != ppt.state.old) {
		d_render_fade_in(300, 0, 0, 0);
		if (d_render_fade_status() == 2) {
			switch (ppt.state.old) {
				default:
					break;
			}
	
			switch (ppt.state.new) {
				case STATE_NUM_IN_GAME:
					ui_init_playing();
					level_init();
					break;
				case STATE_NUM_NEW_HIGHSCORE:
					if (!highscore_is_new())
						ppt.state.new = STATE_NUM_HIGHSCORE;
					else {
						/* Just temporary */
						ppt.state.new = STATE_NUM_HIGHSCORE;
						highscore_add("Arne");
					}
				case STATE_NUM_HIGHSCORE:
					ui_init_highscore();
					break;
				default:
					break;
			}

			ppt.state.old = ppt.state.new;
			d_render_fade_out(300);
		}
	}

	switch (ppt.state.old) {
		case STATE_NUM_MAIN_MENU:
			ui_loop_menu();

			d_tilemap_draw(ppt.ui.menu_background->layer[0].tilemap);
			break;
		case STATE_NUM_IN_GAME:
			block_move_loop();
			d_render_offset(-ppt.ui.offset_x, -ppt.ui.offset_y);
			bullet_move();

			ui_loop_playing();

			d_render_offset(-ppt.bs_x - ppt.ui.offset_x, -ppt.bs_y - ppt.ui.offset_y);
			d_render_tile_draw(ppt.tile, 4);
			d_render_offset(0, 0);
			d_tilemap_draw(ppt.tm);
			d_tilemap_draw(ppt.ui.play_background->layer[0].tilemap);
			d_render_offset(-ppt.ui.offset_x, -ppt.ui.offset_y);
			d_render_blend_enable();
			bullet_draw();
			d_render_offset(0, 0);
			ui_draw_stats();
			d_render_blend_disable();

			break;
		case STATE_NUM_HIGHSCORE:
			d_tilemap_draw(ppt.ui.highscore_background->layer[0].tilemap);
			d_render_blend_enable();
			ui_loop_highscore();
			d_render_blend_disable();

			if (d_keys_get().start) {
				d_keys_set(d_keys_get());
				ppt.state.new = STATE_NUM_MAIN_MENU;
			}
			break;
		default:
			break;
	}

	return;
}
