#define	REQUIRE_DATA
#include "pewpewtris.h"
#include "limits.h"


void block_check_line() {
	int i, j;

	for (i = 0; i < 18; i++) {
		for (j = 0; j < 10; j++)
			if (!ppt.tm->data[i * 10 + j])
				break;
		if (ppt.tm->data[i * 10] == BLOCK_HP_SOLID)
			continue;
		if (j < 10)
			continue;
		for (j = 0; j < 10; j++)
			ppt.tm->data[i * 10 + j] = BLOCK_HP_SOLID;
	}

	return;
}


void block_destroy(int index) {
	int i, j, k, f;

	i = ppt.tile_lookup[index];
	if (i >= 0) {
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
	int i, j, k, l, f;

	i = ppt.tile_lookup[index];

	if (i >= 0) {
		if (ppt.tm->data[i] == BLOCK_HP_SOLID);
		else if (ppt.tm->data[i] <= damage)
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
		l = j;
		j -= f;
		for (i = 0, k = -1; k < j; i++)
			if (ppt.falling.blocks[i])
				k++;
		i--;
		if (i >= 0) {
			if (ppt.tm->data[i] == BLOCK_HP_SOLID);
			else if (ppt.falling.blocks[i] <= damage)
				block_destroy(index);
			else {
				ppt.falling.blocks[i] -= damage;
				d_render_tile_set(ppt.tile, l, ppt.falling.blocks[i]);
			}
		} else
			fprintf(stderr, "No block data found\n");
		
	}

	return;
}


void block_move() {
	int top[4], block[4], i, j, k, ind;
	ppt.bs_y += 1;

	if (ppt.request_new)
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
					d_quit();
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
	
	for (i = j = 0; i < 16; i++) {
		if (ppt.falling.box_id[j] == -1) {
			j++, i--;
			continue;
		}

		if (ppt.falling.blocks[i])
			d_bbox_move(ppt.bbox, ppt.falling.box_id[j++], ppt.bs_x + (i % 4) * 24, ppt.bs_y + (i / 4 * 24));
	}

	return;
}


void block_move_loop() {
	ppt.d_y += d_last_frame_time() * ppt.current_speed;
	while (ppt.d_y >= 1000) {
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
	for (i = j = 0; i < 16; i++) {
		if (bs.blocks[i]) {
			ppt.falling.blocks[i] += (rand() % 3);
			d_render_tile_set(ppt.tile, j, ppt.falling.blocks[i]);
			d_render_tile_move(ppt.tile, j, (i % 4) * 24, (i / 4) * 24);
			ppt.falling.box_id[j++] = d_bbox_add(ppt.bbox, (i % 4) * 24, (i / 4) * 24, 24, 24);
		}
	}
	

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
	}

	memcpy(bs.blocks, src, 16);
	bs.first_check = 1;
	/* TODO: Add block id */

	return bs;
}
