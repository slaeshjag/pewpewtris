#define	REQUIRE_POWERUP_DATA
#include "pewpewtris.h"


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
	
	miss_ratio = ppt.level.bullet_miss * 1000 / ppt.level.bullet_total;
	if (miss_ratio > POWERUP_THRESHOLD)
		return;
	if (rand() % POWERUP_LIKELY_SPAWN)
		return;

	for (i = 0; i < POWERUP_COUNT; i++)
		if (powerup_threshold[i] < miss_ratio)
			break;
	t = rand() % i;
	powerup_attempt_spawn(t + POWERUP_BASE);
	return;
}


void powerup_add(int powerup) {
	switch (powerup) {
		case POWERUP_NUM_GATLINGG:
			ppt.ui.gatling_ammo += POWERUP_GATLING_AMMO_PACK;
			break;
		case POWERUP_NUM_NUKE:
			ppt.ui.nukes++;
			break;
		default:
			fprintf(stderr, "Unimplemented powerup %i\n", powerup);
			break;
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
