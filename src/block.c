#define	REQUIRE_DATA
#include "pewpewtris.h"

void block_destroy(int index) {
	int i, j, k;

	i = ppt.tile_lookup[index];
	if (i >= 0) {
//		return;
		ppt.tm->data[i] = 0;
		ppt.tile_lookup[index] = -1;
	} else {
		for (i = 0, j = -1; i < 4; i++)
			if (ppt.falling.box_id[0] == index)
				ppt.falling.box_id[i] = -1, j = i;
		for (i = k = 0; k < j; i++)
			if (ppt.falling.blocks[i])
				k++;
		i--;
		if (i >= 0)
			ppt.falling.blocks[i] = 0;
	}
	
	d_bbox_delete(ppt.bbox, index);
	
}


void block_impact(int index, int damage) {
	int i, j, k;

	i = ppt.tile_lookup[index];

	if (i <= 0) {
		if (ppt.tm->data[i] <= damage)
			block_destroy(index);
		else
			ppt.tm->data[i] -= damage;
	} else {
		for (i = 0, j = -1; i < 4; i++)
			if (ppt.falling.box_id[0] == index)
				ppt.falling.box_id[i] = -1, j = i;
		for (i = k = 0; k < j; i++)
			if (ppt.falling.blocks[i])
				k++;
		i--;
		if (i >= 0) {
			if (ppt.tm->data[i] <= damage)
				block_destroy(index);
			else
				ppt.tm->data[i] -= damage;
		}
		
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
	}

	memcpy(bs.blocks, src, 16);
	bs.first_check = 1;
	/* TODO: Add block id */

	return bs;
}
