#define	REQUIRE_DATA
#include "pewpewtris.h"
#include "limits.h"

void block_destroy(int index) {
	int i, j, k, f;

	i = ppt.tile_lookup[index];
	if (i >= 0) {
		fprintf(stderr, "Destroying settled block %i\n", index);
//		return;
		ppt.tm->data[i] = 0;
		ppt.tile_lookup[index] = -1;
		d_tilemap_recalc(ppt.tm);
	} else {
		for (i = f = 0, j = -1; i < 4; i++)
			if (ppt.falling.box_id[i] == index) {
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
	
}


void block_impact(int index, int damage) {
	int i, j, k, f;

	i = ppt.tile_lookup[index];

	if (i >= 0) {
		if (ppt.tm->data[i] <= damage)
			block_destroy(index);
		else {
			ppt.tm->data[i] -= damage;
			d_tilemap_recalc(ppt.tm);
		}
	} else {
		for (i = f = 0, j = -1; i < 4; i++)
			if (ppt.falling.box_id[i] == index) {
				j = i;
				break;
			} else if (ppt.falling.box_id[i] == -1)
				f++;
		j -= f;
		for (i = 0, k = -1; k < j; i++)
			if (ppt.falling.blocks[i])
				k++;
		i--;
		if (i >= 0) {
			if (ppt.falling.blocks[i] <= damage)
				block_destroy(index);
			else {
				ppt.falling.blocks[i] -= damage;
				d_render_tile_set(ppt.tile, j, ppt.falling.blocks[i]);
			}
		} else
			fprintf(stderr, "No block data found\n");
		
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
