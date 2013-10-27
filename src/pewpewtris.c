#include "pewpewtris.h"


void get_new_block() {
	struct block_struct bs;
	int i, j;

	bs = block_new();
	for (i = j = 0; i < 16; i++) {
		if (bs.blocks[i]) {
			d_render_tile_set(ppt.tile, j, 1);
			d_render_tile_move(ppt.tile, j, (i % 4) * 24, (i / 4) * 24);
			bs.box_id[j++] = d_bbox_add(ppt.bbox, (i % 4) * 24, (i / 4) * 24, 24, 24);
		}
	}
	
	ppt.falling = bs;

	return;
}


void check_topography_falling(int buf[4]) {
	int i, j;

	for (i = j = 0; i < 16; i++)
		if (ppt.falling.blocks[i])
			buf[i & 3] = i / 4;
	return;
}


void check_topography_tm(int buf[4], int x) {
	int i, j, k;

	buf[0] = buf[1] = buf[2] = buf[3] = -1;

	for (i = 0; i < 4; i++) {
		if (i + x >= 10) {
			fprintf(stderr, "out of range\n");
			buf[j++] = 1;
		} 
		
		for (j = 0; j < 18; j++)
			if (ppt.tm->data[10 * j + x + i]) {
				buf[i] = j;
			}
		if (buf[i] == -1)
			buf[i] = 17;
	}

	return;
}


void move_block() {
	int top[4], block[4], i, j;
	ppt.bs_y += 4;

	for (i = j = 0; i < 16; i++)
		if (ppt.falling.blocks[i])
			d_render_tile_move(ppt.tile, j, (i % 4) * 24, (i / 4) * 24);

	if (ppt.bs_y % 24 == 0) {
		check_topography_falling(block);
		check_topography_tm(top, ppt.bs_x / 24);
		for (i = 0; i < 4; i++)
			if (ppt.bs_y / 24 + 1 + block[i] + i >= top[i]) {
				fprintf(stderr, "At bottom! %i %i\n", i, top[i]);
				return;
			}
	}

	return;
}


int main(int argc, char **argv) {
	int n;	
	d_init("pewpewtris", "pewpewtris", NULL);

	ppt.block = d_render_tilesheet_load("res/block.png", 24, 24, DARNIT_PFORMAT_RGB5A1);
	ppt.tile = d_render_tile_new(10 * 18, ppt.block);
	ppt.bbox = d_bbox_new(180);
	ppt.tm = d_tilemap_new(0xFFF, ppt.block, 0xFFF, 10, 18);

	n = d_time_get() / 1000;
	for (;;) {
		if (d_time_get() / 1000 - n >= 1) {
			fprintf(stderr, "New block!\n");
			get_new_block();
			n = d_time_get() / 1000;
		}
		move_block();
		
		d_render_begin();
		d_render_tile_draw(ppt.tile, 4);
		d_tilemap_draw(ppt.tm);
		d_render_end();
		d_loop();
	}

	return 0;
}
