#include "pewpewtris.h"


static const int block_cutoff[BLOCK_TYPES] = {
	0, 5, 10, 20
};

static const int block_start[BLOCK_TYPES] = {
	300, 5, -40, -50
};

static const int block_multiply[BLOCK_TYPES] = {
	2, 5, 7, 10
};


void level_init() {
	ppt.level.blocks = 0;
	ppt.level.level = 1;
	ppt.d_y = 0;
	ppt.level.bullet_total = ppt.level.bullet_miss = 0;
	level_update();
//	block_add_special_slot(23);
	
	return;
}


void level_update() {
	int i, l;

	l = ppt.level.blocks / (ppt.level.level + 10) + 1;
	if (l > ppt.level.level) ppt.level.level = l;
	ppt.level.block_dy = 140 + ppt.level.level * 13;

	for (i = 0; i < BLOCK_TYPES; i++) {
		if (ppt.level.level < block_cutoff[i]) {
			ppt.level.block_likely[i] = ppt.level.block_likely[i - 1];
			continue;
		}

		ppt.level.block_likely[i] = block_start[i] + block_multiply[i] * ppt.level.level;
		if (i > 0)
			ppt.level.block_likely[i] += ppt.level.block_likely[i - 1];
	}

	return;
}
