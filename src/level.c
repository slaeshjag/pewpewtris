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
	level_update();
	block_get_new();
	
	return;
}


void level_update() {
	int i;

	ppt.level.level = ppt.level.blocks / 10 + 1;
	ppt.level.block_dy = 50 + ppt.level.level * 10;

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
