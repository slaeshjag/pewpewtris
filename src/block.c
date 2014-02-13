#define	REQUIRE_DATA
#include "pewpewtris.h"
#include <limits.h>


void block_set_special(int i) {
	int t;

	t = ppt.ui.special_tiles[i];
	d_render_tile_set(ppt.ui.special_tile, i, t);
	if (t)
		d_render_tile_move(ppt.ui.special_tile, i, 24 + i * 24, 408);
	else
		d_render_tile_move(ppt.ui.special_tile, i, INT_MAX, INT_MAX);

	return;
}


void block_update_specials() {
	int i;

	for (i = 0; i < 9; i++)
		block_set_special(i);
	return;
}


void block_add_special_slot(int t) {
	int i;

	for (i = 0; i < 9; i++)
		if (!ppt.ui.special_tiles[i])
			break;
	if (i == 9)
		return;
	ppt.ui.special_tiles[i] = t;
	block_set_special(i);

	return;
}
	

int block_pop_special_slot() {
	int i, ret;

	ret = ppt.ui.special_tiles[0];
	for (i = 1; i < 9; i++)
		ppt.ui.special_tiles[i - 1] = ppt.ui.special_tiles[i];
	block_update_specials();

	return ret;
}


int block_get_type() {
	unsigned int i, b;

	b = rand() % ppt.level.block_likely[BLOCK_TYPES - 1];
	for (i = 0; i < BLOCK_TYPES; i++)
		if (b < ppt.level.block_likely[i])
			return i + 1;
	return BLOCK_TYPE_SOLID;
}


void block_check_line() {
	int i, j;

	for (i = 0; i < 18; i++) {
		for (j = 0; j < 10; j++)
			if (!ppt.tm->data[i * 10 + j] || ppt.tm->data[i * 10 + j] == BLOCK_TYPE_FILLER)
				break;
		if (ppt.tm->data[i * 10] == BLOCK_TYPE_SOLID)
			continue;
		if (j < 10)
			continue;
		for (j = 0; j < 10; j++)
			ppt.tm->data[i * 10 + j] = BLOCK_TYPE_SOLID;
	}

	return;
}


void block_destroy(int index) {
	int i, j, k, f, x, y, r, g, b, t;

	i = ppt.tile_lookup[index];
	if (i >= 0) {
		x = ppt.tile_lookup[index] % 10;
		y = ppt.tile_lookup[index] / 10;
		t = ppt.tm->data[i];
		block_color(ppt.tm->data[i] & 0xFF, &r, &g, &b);
		block_particle_trig(x * 24 + 12, y * 24 + 12, r, g, b);
		if (ppt.tm->data[ppt.tile_lookup[index]] >= POWERUP_BASE)
			powerup_add(ppt.tm->data[i] - POWERUP_BASE);
		
		ppt.tile_lookup[index] = -1;
		ppt.tm->data[i] = 0;
		d_tilemap_recalc(ppt.tm);
	} else {
		for (i = f = 0, j = -1; i < 4; i++)
			if (ppt.falling.box_id[i] == index) {
				for (k = b = 0; b != i; k++)
					if (ppt.falling.blocks[k])
						b++;
				t = ppt.falling.blocks[k - 1] & 0xFF;
				block_color(ppt.falling.blocks[k - 1] & 0xFF, &r, &g, &b);
				block_particle_trig(ppt.falling.x[i] + 12, ppt.falling.y[i] + 12, r, g, b);
				ppt.falling.box_id[i] = -1, j = i;
				d_render_tile_move(ppt.tile, j, INT_MAX, INT_MAX);
				break;
			} else if (ppt.falling.box_id[i] == -1)
				f++;
		j -= f;
		for (i = 0, k = -1; k < j; i++)
			if (ppt.falling.blocks[i])
				k++;
		i--;
		if (i >= 0)
			ppt.falling.blocks[i] = 0;
		for (i = k = 0; i < 16; i++)
			if (ppt.falling.blocks[i])
				k++;
		if (!k)
			ppt.request_new = 1;
	}
	
	d_bbox_delete(ppt.bbox, index);

	/* Add points */
	ppt.level.blocks++;
	if (t != BLOCK_TYPE_FILLER)
		ppt.ui.score_n += 150 + 50 * ppt.level.level;
	ppt.ui.redraw = 1;
	powerup_spawn();
	level_update();

	return;
}


int block_impact(int index, int damage) {
	int i, j, k, l, f, hp_used = 0;

	i = ppt.tile_lookup[index];

	if (i >= 0) {
		switch (ppt.tm->data[i]) {
			case BLOCK_TYPE_SOLID:
				ppt.level.bullet_miss++;
				return damage;
			case BLOCK_TYPE_GATLINGG:
			case BLOCK_TYPE_NUKE:
			case BLOCK_TYPE_PFILLER:
				block_destroy(index);
				d_sound_play(ppt.ui.powerup_sound, 0, UI_SOUND_VOLUME, UI_SOUND_VOLUME, 0);
				return 1;
			case BLOCK_TYPE_FILLER:
				block_destroy(index);
				return 0;
			default:
				if (ppt.tm->data[i] <= damage) {
					hp_used = ppt.tm->data[i];
					d_sound_play(ppt.ui.block_explode, 0, UI_SOUND_VOLUME, UI_SOUND_VOLUME, 0);
					block_destroy(index);
				} else {
					d_sound_play(ppt.ui.block_hit, 0, UI_SOUND_VOLUME, UI_SOUND_VOLUME, 0);
					ppt.tm->data[i] -= damage;
					hp_used = damage;
					d_tilemap_recalc(ppt.tm);
				}

				break;
		}
	} else {
		for (i = f = 0, j = -1; i < 4; i++)
			if (ppt.falling.box_id[i] == index) {
				j = i;
				break;
			} else if (ppt.falling.box_id[i] == -1)
				f++;
		l = j;
		j -= f;
		for (i = 0, k = -1; k < j; i++)
			if (ppt.falling.blocks[i])
				k++;
		i--;
		if (i >= 0) {
			if (ppt.falling.blocks[i] <= damage) {
				hp_used = ppt.falling.blocks[i];
				d_sound_play(ppt.ui.block_explode, 0, UI_SOUND_VOLUME, UI_SOUND_VOLUME, 0);
				block_destroy(index);
			} else {
				hp_used = damage;
				d_sound_play(ppt.ui.block_hit, 0, UI_SOUND_VOLUME, UI_SOUND_VOLUME, 0);
				ppt.falling.blocks[i] -= damage;
				d_render_tile_set(ppt.tile, l, ppt.falling.blocks[i]);
			}
		} else
			fprintf(stderr, "No block data found\n");
		
	}
	
	ppt.ui.score_n += 10 * ppt.level.level;
	ppt.ui.redraw = 1;

	return hp_used;
}


void block_move() {
	int top[4], block[4], i, j, k, ind;
	ppt.bs_y += 1;

	if (ppt.request_new && !ppt.ui.nuke_going)
		ppt.request_new = 0, block_get_new();

	if (ppt.bs_y % 24 == 0 || ppt.falling.first_check) {
		check_topography_falling(block);
		check_topography_tm(top, 4, ppt.bs_x / 24);
		for (i = 0; i < 4; i++) {
			if (ppt.bs_x / 24 + i < 0 || ppt.bs_x / 24 + i >= 10)
				continue;
			if (block[i] == -1)
				continue;
			if (ppt.bs_y / 24 + 1 + block[i] >= top[i]) {
				if (ppt.falling.first_check) {
					//ppt.state.new = STATE_NUM_NEW_HIGHSCORE;
					ppt.ui.wait_for_name = (highscore_is_new() ? 1 : 0);;
					ppt.ui.game_over = d_time_get();
					bullet_kill_all();
					return;
					/* Signal game over */
				}

				for (j = k = 0; j < 16; j++) {
					if (ppt.falling.box_id[k] == -1) {
						k++;
						j--;
						continue;
					}

					if (ppt.falling.blocks[j]) {
						ind = (ppt.bs_y / 24 * 10 + (j / 4 * 10) + (j & 3) + ppt.bs_x / 24);
						ppt.tm->data[ind] = ppt.falling.blocks[j];
						if (ppt.falling.box_id[k] >= 0)
							ppt.tile_lookup[ppt.falling.box_id[k]] = ind;
						k++;
					}
				}
				block_check_line();
				d_tilemap_recalc(ppt.tm);
				block_get_new();
				return;
			}
		}
		ppt.falling.first_check = 0;
	}
	
	for (i = j = k = 0; i < 16; i++) {
		if (ppt.falling.box_id[j] == -1) {
			j++, i--;
			continue;
		}

		if (ppt.falling.blocks[i]) {
			ppt.falling.x[j] = ppt.bs_x + (i % 4) * 24;
			ppt.falling.y[j] = ppt.bs_y + (i / 4 * 24);
			d_bbox_move(ppt.bbox, ppt.falling.box_id[j++], ppt.bs_x + (i % 4) * 24, ppt.bs_y + (i / 4 * 24));
		}
	}

	return;
}


void block_move_loop() {
	if (ppt.paused)
		return;
	ppt.d_y += d_last_frame_time() * ppt.level.block_dy;
	while (ppt.d_y >= 1000 && !ppt.ui.game_over) {
		ppt.d_y -= 1000;
		block_move();
	}

	return;
}


struct block_struct block_rotate(struct block_struct bs) {
	struct block_struct bs_new;
	int i;

	bs_new = bs;
	for (i = 0; i < 16; i++)
		bs_new.blocks[((i & 0x3) << 2) + (i >> 2)] = bs.blocks[i];
	
	return bs_new;
}


void block_get_new() {
	struct block_struct bs;
	int i, j;

	bs = block_new();
	ppt.falling = bs;
	ppt.bs_x = ppt.bs_y = 0;
	ai_determine_best_move();
	bs = ppt.falling;

	j = 16;

	for (i = j = 0; i < 16; i++) {
		if (bs.blocks[i]) {
			if (i < j)
				j = i;
			ppt.falling.blocks[i] = block_get_type();
			d_render_tile_set(ppt.tile, j, ppt.falling.blocks[i]);
			d_render_tile_move(ppt.tile, j, (i % 4) * 24, (i / 4) * 24);
			ppt.falling.box_id[j++] = d_bbox_add(ppt.bbox, (i % 4) * 24, (i / 4) * 24, 24, 24);
		}
	}

	ppt.bs_y = -(j / 4) * 24;

	return;
}


struct block_struct block_new() {
	struct block_struct bs;
	char block, *src;

	block = rand() % 7;

	switch (block) {
		case 0:
			src = block_0;
			break;
		case 1:
			src = block_1;
			break;
		case 2:
			src = block_2;
			break;
		case 3:
			src = block_3;
			break;
		case 4:
			src = block_4;
			break;
		case 5:
			src = block_5;
			break;
		case 6:
			src = block_6;
			break;
		default:
			src = block_0;
			break;
	}

	memcpy(bs.blocks, src, 16);
	bs.first_check = 1;
	/* TODO: Add block id */

	return bs;
}


void block_particle_init() {
	int i;

	for (i = 0; i < 180; i++) {
		ppt.b_particle.used[i] = 0;
		ppt.b_particle.p[i] = d_particle_new(30, DARNIT_PARTICLE_TYPE_POINT);
		d_particle_color_target(ppt.b_particle.p[i], 0, 0, 0, 0);
		d_particle_emitter_velocity(ppt.b_particle.p[i], 0, 64);
		d_particle_emitter_gravity(ppt.b_particle.p[i], 0, 200);
		d_particle_life(ppt.b_particle.p[i], 600);
		d_particle_point_size(ppt.b_particle.p[i], 3);
		d_particle_emitter_angle(ppt.b_particle.p[i], 0, 3599);
		d_particle_mode(ppt.b_particle.p[i], DARNIT_PARTICLE_MODE_PULSAR);
	}

	return;
}


void block_particle_reset() {
	int i;

	for (i = 0; i < 180; i++) {
		d_particle_clear_all(ppt.b_particle.p[i]);
		ppt.b_particle.used[i] = 0;
	}

	return;
}


void block_particle_trig(int x, int y, int r, int g, int b) {
	int i;

	for (i = 0; i < 180 && ppt.b_particle.used[i]; i++);
	if (i == 180)
		return;
	d_particle_emitter_move(ppt.b_particle.p[i], x, y);
	d_particle_color_start(ppt.b_particle.p[i], r, g, b, 255);
	d_particle_pulse(ppt.b_particle.p[i]);
	ppt.b_particle.used[i] = 1;

	return;
}

void block_particle_draw() {
	int i;

	for (i = 0; i < 180; i++) {
		if (!ppt.b_particle.used[i])
			continue;
		d_particle_draw(ppt.b_particle.p[i]);
		if (!d_particle_used(ppt.b_particle.p[i]))
			ppt.b_particle.used[i] = 0;
	}

	return;
}


void block_color(int id, int *r, int *g, int *b) {
	/* TODO: Real values */

	if (id >= sizeof(block_color_a) / 4) {
		*r = 255;
		*g = 255;
		*b = 255;
		return;
	}

	*r = block_color_a[id * 4];
	*g = block_color_a[id * 4 + 1];
	*b = block_color_a[id * 4 + 2];

	return;
}
