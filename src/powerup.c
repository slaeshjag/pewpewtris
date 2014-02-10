#define	REQUIRE_POWERUP_DATA
#include "pewpewtris.h"
#include <string.h>


/* This only *attempts* to spawn a powerup */
void powerup_attempt_spawn(int block) {
	int i;

	i = rand() % (10 * 12);	/* Only spawn in the lower 12 rows */
	i += 10 * 6;

	if (!ppt.tm->data[i])
		return;
	ppt.tm->data[i] = block;
	d_tilemap_recalc(ppt.tm);

	return;
}


void powerup_spawn() {
	int miss_ratio, t, i;

	if (ppt.level.level <3)
		return;
	if (ppt.ui.nuke_going)
		return;
	
	miss_ratio = ppt.level.bullet_miss * 1000 / ppt.level.bullet_total;
	if (miss_ratio > POWERUP_THRESHOLD)
		return;
	if (rand() % POWERUP_LIKELY_SPAWN)
		return;

	for (i = 0; i < POWERUP_COUNT; i++)
		if (powerup_threshold[i] < miss_ratio)
			break;
	t = rand() % (i * 128);
	t /= 128;
	powerup_attempt_spawn(t + POWERUP_BASE);
	return;
}


void powerup_add(int powerup) {
	switch (powerup) {
		case POWERUP_NUM_GATLINGG:
			ppt.ui.gatling_reload_time -= 40;
			if (ppt.ui.gatling_reload_time < 40)
				ppt.ui.gatling_reload_time = 40;
			break;
		default:
			block_add_special_slot(powerup + 21);
			break;
	}

	return;
}


void powerup_shifty_do() {
	int i, blocks, id;
	unsigned block[180];

		
	d_sound_play(ppt.ui.shift_sound, 0, UI_SOUND_VOLUME, UI_SOUND_VOLUME, 0);
	blocks = d_bbox_test(ppt.bbox, 0, 0, 1000, 1000, block, 180);
	for (i = 0; i < blocks; i++) {
		if (ppt.tile_lookup[block[i]] < 0)
			continue;
		id = ppt.tile_lookup[block[i]];
		if (ppt.tile_lookup[block[i]] >= 140) {
			block_destroy(block[i]);
			continue;
		}
		
		ppt.tile_lookup[block[i]] += 40;
		d_bbox_move(ppt.bbox, block[i], (id % 10) * 24, (id / 10 + 4) * 24);
	}

	memmove(&ppt.tm->data[40], ppt.tm->data, 140 * 4);
	memset(ppt.tm->data, 0, 40 * 4);
	d_tilemap_recalc(ppt.tm);
	return;
}


void powerup_stairbomb_do() {
	int i, blocks;
	unsigned block[180];

	blocks = d_bbox_test(ppt.bbox, 0, 0, 1000, 1000, block, 180);
	for (i = 0; i < blocks; i += 3) {
		d_sound_play(ppt.ui.block_explode, 0, UI_SOUND_VOLUME, UI_SOUND_VOLUME, 0);
		block_destroy(block[i]);
	}

	return;
}


void powerup_nuke_do() {
	int i, blocks;
	unsigned int block[2];

	blocks = d_bbox_test(ppt.bbox, 0, 0, 1000, 1000, block, 2);

	for (i = 0; i < blocks; i++) {
		d_sound_play(ppt.ui.block_explode, 0, UI_SOUND_VOLUME, UI_SOUND_VOLUME, 0);
		block_destroy(block[i]);
	}

	if (blocks < 2)
		ppt.ui.nuke_going = 0;

	return;
}


void powerup_filler_do() {
	int i, j, a, b, x, y;
	unsigned int c;

	for (i = 0; i < 18; i++) {
		a = b = c = 0;
		for (j = 0; j < 10; j++) {
			if (!ppt.tm->data[i * 10 + j]) {
				if (!b)
					a = 1, c++;
				else
					a++;
			} else if (a)
				b = 1;
		}

		if (a == 1 && c <3)
			for (j = 0; j < 10; j++)
				if (!ppt.tm->data[i * 10 + j]) {
					x = j * 24;
					y = i * 24;
					if (d_bbox_test(ppt.bbox,x, y, 24, 24, &c, 1))
						continue;
					c = d_bbox_add(ppt.bbox, x, y, 24, 24);
					/* Set filler */
					ppt.tm->data[i * 10 + j] = 5;
					ppt.tile_lookup[c] = i * 10 + j;
				}
	}

	d_tilemap_recalc(ppt.tm);
	return;
}


void powerup_activate() {
	int t;

	t = block_pop_special_slot() - 21;
	if (t == POWERUP_NUM_NUKE) {	/* Nuke */
		if (ppt.ui.nuke_going)
			return;
		ppt.ui.nuke_going = 1;
	} else if (t == POWERUP_NUM_FILLER)
		powerup_filler_do();
	else if (t == POWERUP_NUM_STAIRBOMB)
		powerup_stairbomb_do();
	else if (t == POWERUP_NUM_SHIFTER)
		powerup_shifty_do();
	return;
}
