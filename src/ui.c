#include "pewpewtris.h"
#include <math.h>
#include <limits.h>
#define	abs(x)		((x) < 0 ? -(x) : (x))


void ui_init() {
	ppt.ui.score = d_text_surface_new(ppt.font, 64, 240, 560, 58);
	ppt.ui.level = d_text_surface_new(ppt.font, 64, 240, 560, 130);
	ppt.ui.accur = d_text_surface_new(ppt.font, 64, 240, 560, 202);
	ppt.ui.highscore = d_text_surface_new(ppt.font, 512, 700, 110, 120);
	ppt.ui.main_menu = d_menu_vertical_new("New  Game\nHighscore\n Options\n Credits\nQuit Game", 314, 120, ppt.font, 164, 23, 800);
	ppt.ui.highscore_str[0] = 0;
	ppt.ui.highscore_name = d_menu_textinput_new(216, 310, ppt.font, ppt.ui.highscore_str, 28, 360);
	d_menu_shade_color(ppt.ui.main_menu, 0, 0, 0, 255);

	ppt.ui.player = d_sprite_load("res/turret.spr", 0, DARNIT_PFORMAT_RGB5A1);
	ppt.ui.special_tile = d_render_tile_new(9, ppt.block);
	ppt.ui.dragonbox = d_sprite_load("res/dragonbox.spr", 0, DARNIT_PFORMAT_RGB5A1);

	ppt.ui.aim_line = d_render_line_new(1, 1);
	ppt.ui.block_hit = d_sound_streamed_load("sounds/block_hit.ogg", DARNIT_AUDIO_PRELOAD, DARNIT_AUDIO_MONO);
	ppt.ui.block_explode = d_sound_streamed_load("sounds/block_destroy.ogg", DARNIT_AUDIO_PRELOAD, DARNIT_AUDIO_MONO);
	ppt.ui.bullet_shoot = d_sound_streamed_load("sounds/bullet_shoot.ogg", DARNIT_AUDIO_PRELOAD, DARNIT_AUDIO_MONO);

	return;
}


void ui_init_playing() {
	int i;

	ppt.ui.angle = 0;
	ppt.ui.turret_x = TURRET_DEFAULT_X;
	ppt.ui.turret_y = TURRET_DEFAULT_Y;
	ppt.ui.score_n = 0;
	ppt.ui.redraw = 1;
	ppt.ui.game_over = 0;
	ppt.request_new = 1;
	ppt.ui.gatling_last = 0;
	ppt.ui.nuke_going = 0;
	ppt.bs_y = 0;
	ppt.ui.gatling_reload_time = 500;

	memset(ppt.tm->data, 0, sizeof(int) * ppt.tm->w * ppt.tm->h);
	memset(ppt.tile_lookup, 0xFF, sizeof(int) * ppt.tm->w * ppt.tm->h);
	bullet_kill_all();
	/* TODO: kill all bullets */
	d_tilemap_recalc(ppt.tm);
	d_bbox_clear(ppt.bbox);

	d_sprite_rotate(ppt.ui.player, 0);
	d_sprite_move(ppt.ui.player, ppt.ui.turret_x, ppt.ui.turret_y);

	for (i = 0; i < 9; i++) {
		d_render_tile_move(ppt.ui.special_tile, i, INT_MAX, INT_MAX);
		ppt.ui.special_tiles[i] = 0;
	}
}


void ui_init_highscore() {
	int i;
	char buff[10];
	d_text_surface_reset(ppt.ui.highscore);

	for (i = 0; i < HIGHSCORE_NUMBER; i++) {
		if (!ppt.highscore.highscore[i].score)
			break;
		d_text_surface_string_append(ppt.ui.highscore, ppt.highscore.highscore[i].name);
		d_text_surface_offset_next_set(ppt.ui.highscore, 400);
		sprintf(buff, "%.8i", ppt.highscore.highscore[i].score);
		d_text_surface_string_append(ppt.ui.highscore, buff);
		d_text_surface_offset_next_set(ppt.ui.highscore, 550);
		sprintf(buff, "%.2i\n", ppt.highscore.highscore[i].level);
		d_text_surface_string_append(ppt.ui.highscore, buff);
	}

	return;
}


void ui_init_credits() {
	d_text_surface_reset(ppt.ui.highscore);
	d_text_surface_string_append(ppt.ui.highscore, "****************** CODE ******************\n");
	d_text_surface_string_append(ppt.ui.highscore, "               Steven Arnow\n");
	d_text_surface_string_append(ppt.ui.highscore, "\n");
	d_text_surface_string_append(ppt.ui.highscore, "***************** SOUND ******************\n");
	d_text_surface_string_append(ppt.ui.highscore, "            Generated by SFXR\n");
	d_text_surface_string_append(ppt.ui.highscore, "\n");
	d_text_surface_string_append(ppt.ui.highscore, "           Powered by libDarnit\n");
	
	return;
}



void ui_init_mainmenu() {
	d_sprite_move(ppt.ui.dragonbox, 0, 464);
	d_menu_selection_wait(ppt.ui.main_menu);

	return;
}


void ui_draw_player() {
	d_sprite_draw(ppt.ui.player);
}


void ui_loop_playing() {
	int js0_x, js0_y, angle, js1_y, y, x;
	float a, b, t;
	DARNIT_KEYS k;

	if (ppt.ui.game_over)
		return;
	angle = ppt.ui.angle;
	y = ppt.ui.turret_y;
	d_joystick_get(&js0_x, &js0_y, NULL, &js1_y);
	if (!js0_x && !js0_y) {
		if (d_keys_get().left) {
			ppt.ui.angle -= (d_last_frame_time());
		} else if (d_keys_get().right) {
			ppt.ui.angle += (d_last_frame_time());
		}
	} else {
		a = abs(js0_x);
		b = js0_y;
		t = b / a;
		t = atanf(t);
		t *= (1800.0f / M_PI);
		ppt.ui.angle = t;
	}

	if (!js1_y) {
		if (d_keys_get().up || d_keys_get().l)
			ppt.ui.turret_y -= d_last_frame_time() / 2;
		if (d_keys_get().down || d_keys_get().r)
			ppt.ui.turret_y += d_last_frame_time() / 2;
	} else {
		js1_y *= 150;		/* 150 pix/sec max speed */
		js1_y /= 32768;
		ppt.ui.turret_y += js1_y;
	}

	if (ppt.ui.angle <= UI_MIN_ANGLE)
		ppt.ui.angle = UI_MIN_ANGLE;
	else if (ppt.ui.angle >= UI_MAX_ANGLE)
		ppt.ui.angle = UI_MAX_ANGLE;
	if (ppt.ui.turret_y < 0)
		ppt.ui.turret_y = 0;
	else if (ppt.ui.turret_y > 412)
		ppt.ui.turret_y = 408;
	
	if (ppt.ui.turret_y != y)
		d_sprite_move(ppt.ui.player, ppt.ui.turret_x, ppt.ui.turret_y);
	if (ppt.ui.angle != angle)
		d_sprite_rotate(ppt.ui.player, -ppt.ui.angle);
	

	x = ppt.ui.turret_x + 12;
	y = ppt.ui.turret_y + 12;
	d_render_line_move(ppt.ui.aim_line, 0, x, y, (d_util_sin((900 + ppt.ui.angle)) * 1000) / 32768 + x, (d_util_sin(ppt.ui.angle) * 1000) / 32768 + y);
	
	if (d_keys_get().x) {
		if (d_time_get() - ppt.ui.gatling_last < ppt.ui.gatling_reload_time)
			return;
		ppt.ui.gatling_last = d_time_get();
		bullet_fire(0, ppt.ui.angle, 500, -12, ppt.ui.turret_y + 12);
		d_sound_play(ppt.ui.bullet_shoot, 0, UI_SOUND_VOLUME, UI_SOUND_VOLUME, 0);
	}

	if (d_keys_get().y) {
		powerup_activate();
		k = d_keys_zero();
		k.y = 1;
		d_keys_set(k);
	}

	if (ppt.ui.nuke_going)
		powerup_nuke_do();

	return;
}


void ui_loop_menu() {
//	if (d_keys_get().start)
//		ppt.state.new = STATE_NUM_IN_GAME;
	switch (d_menu_loop(ppt.ui.main_menu)) {
		case 0: /* New game */
			ppt.state.new = STATE_NUM_IN_GAME;
			break;
		case 1:
			ppt.state.new = STATE_NUM_HIGHSCORE;
			break;
		case 2:	/* Options */
			d_menu_selection_wait(ppt.ui.main_menu);
		case 3:	/* Credits */
			ppt.state.new = STATE_NUM_CREDITS;
			break;
		case 4:	/* Quit game */
		case -2:
			d_quit();
			break;
		case -1:
		default:
			return;
	}


	/* TODO: Play sound on menu selection change */

	return;
}


void ui_loop_highscore() {
	d_text_surface_draw(ppt.ui.highscore);

	return;
}


void ui_loop_credits() {
	d_text_surface_draw(ppt.ui.highscore);

	return;
}


void ui_draw_stats() {
	char buff[64];
	int acc;

	if (ppt.ui.redraw) {
		d_text_surface_reset(ppt.ui.score);
		d_text_surface_string_append(ppt.ui.score, "Score:");
		sprintf(buff, "%.8i", ppt.ui.score_n);
		d_text_surface_offset_next_set(ppt.ui.score, 100);
		d_text_surface_string_append(ppt.ui.score, buff);
		
		d_text_surface_reset(ppt.ui.level);
		d_text_surface_string_append(ppt.ui.level, "Level:");
		d_text_surface_offset_next_set(ppt.ui.level, 100);
		sprintf(buff, "%i", ppt.level.level);
		d_text_surface_string_append(ppt.ui.level, buff);

		d_text_surface_reset(ppt.ui.accur);
		d_text_surface_string_append(ppt.ui.accur, "Accur:");
		d_text_surface_offset_next_set(ppt.ui.accur, 100);
		if (!ppt.level.bullet_total)
			sprintf(buff, "-");
		else {
			acc = ppt.level.bullet_miss * 100 / ppt.level.bullet_total;
			acc = 100 - acc;
			sprintf(buff, "%i%%", acc);
		}
		d_text_surface_string_append(ppt.ui.accur, buff);



	}

	d_text_surface_draw(ppt.ui.score);
	d_text_surface_draw(ppt.ui.level);
	d_text_surface_draw(ppt.ui.accur);
	
	return;
}
