#include "pewpewtris.h"


void powerup_spawn() {
	int miss_ratio, likely_gat, likely_nuke, q;

	if (ppt.level.level <3)
		return;
	
	miss_ratio = ppt.level.bullet_miss * 100 / ppt.level.bullet_total;
	if (miss_ratio < POWERUP_THRESHOLD)
		return;
	if (rand() % POWERUP_LIKELY_SPAWN)
		return;

	likely_gat = miss_ratio;
	likely_nuke = POWERUP_THRESHOLD - miss_ratio;

	q = rand() % (likely_gat + likely_nuke);

	if (q < likely_gat)
		fprintf(stderr, "TODO: Attempt spawn gatling gun powerup\n");
	else
		fprintf(stderr, "TODO: Attempt spawn nuke powerup\n");
	return;
}
