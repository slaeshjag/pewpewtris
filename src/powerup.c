#include "pewpewtris.h"


/* This only *attempts* to spawn a powerup */
void powerup_attempt_spawn(int block) {
	int i;

	fprintf(stderr, "Attempt to spawn %i\n", block);
	i = rand() % (10 * 12);	/* Only spawn in the lower 12 rows */
	i += 10 * 6;

	if (!ppt.tm->data[i]) {
		fprintf(stderr, "Nothing to replace here\n");
		return;
	}
	ppt.tm->data[i] = block;
	d_tilemap_recalc(ppt.tm);

	return;
}


void powerup_spawn() {
	int miss_ratio, likely_gat, likely_nuke, q;

	if (ppt.level.level <3)
		return;
	
	miss_ratio = ppt.level.bullet_miss * 100 / ppt.level.bullet_total;
	if (miss_ratio > POWERUP_THRESHOLD)
		return;
	if (rand() % POWERUP_LIKELY_SPAWN)
		return;

	likely_gat = miss_ratio;
	likely_nuke = POWERUP_THRESHOLD - miss_ratio;

	q = rand() % (likely_gat + likely_nuke);

	if (q < likely_gat)
		powerup_attempt_spawn(BLOCK_TYPE_GATLINGG);
	else
		powerup_attempt_spawn(BLOCK_TYPE_NUKE);
	return;
}
