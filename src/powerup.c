#define	REQUIRE_POWERUP_DATA
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
			return;
	t = rand() % i;
	powerup_attempt_spawn(t);
	return;
}
