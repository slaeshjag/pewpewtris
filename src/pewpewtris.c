#include "pewpewtris.h"
#include <limits.h>



void block_falling_reconsider() {
	struct block_struct bs;
	int i, j;

	ppt.bs_x = ppt.bs_y = 0;
	ai_determine_best_move();
	bs = ppt.falling;
	
	for (i = j = 0; i < 16 && j < 4; i++) {
		if (ppt.falling.box_id[j] == -1) {
			d_render_tile_move(ppt.tile, j, INT_MAX, INT_MAX);
			j++;
			i--;
			continue;
		}

		if (bs.blocks[i])
			d_render_tile_move(ppt.tile, j, (i % 4) * 24, (i / 4) * 24);
	}

	return;
}


void get_new_block() {
	struct block_struct bs;
	int i, j;

	bs = block_new();
	ppt.falling = bs;
	ppt.bs_x = ppt.bs_y = 0;
	ai_determine_best_move();
	bs = ppt.falling;
	for (i = j = 0; i < 16; i++) {
		if (bs.blocks[i]) {
			d_render_tile_set(ppt.tile, j, bs.blocks[i]);
			d_render_tile_move(ppt.tile, j, (i % 4) * 24, (i / 4) * 24);
			ppt.falling.box_id[j++] = d_bbox_add(ppt.bbox, (i % 4) * 24, (i / 4) * 24, 24, 24);
		}
	}
	

	return;
}


void check_topography_falling(int buf[4]) {
	int i, j;

	for (i = 0; i < 4; i++)
		buf[i] = -1;
	
	for (i = j = 0; i < 16; i++)
		if (ppt.falling.blocks[i])
			buf[i & 3] = i / 4;
	return;
}


void check_topography_tm(int *buf, int size, int x) {
	int i, j;

	for (i = 0; i < size; i++)
		buf[i] = -1;

	for (i = 0; i < size; i++) {
		if (i + x >= 10) {
			buf[i] = 1;
			continue;
		}
		
		for (j = 17; j >= 0; j--)
			if (ppt.tm->data[10 * j + x + i])
				buf[i] = j;
		if (buf[i] == -1)
			buf[i] = 18;
	}

	return;
}


void move_block() {
	int top[4], block[4], i, j, k, ind;
	ppt.bs_y += 1;

	for (i = j = 0; i < 16; i++)
		if (ppt.falling.blocks[i])
			d_bbox_move(ppt.bbox, ppt.falling.box_id[j++], ppt.bs_x, ppt.bs_y);

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

				for (j = k = 0; j < 16; j++)
					if (ppt.falling.blocks[j]) {
						ind = (ppt.bs_y / 24 * 10 + (j / 4 * 10) + (j & 3) + ppt.bs_x / 24);
						ppt.tm->data[ind] = ppt.falling.blocks[j];
						if (ppt.falling.box_id[k] >= 0)
							ppt.tile_lookup[ppt.falling.box_id[k]] = ind;
						k++;
					}
				d_tilemap_recalc(ppt.tm);
				get_new_block();
				return;
			}
		}
		ppt.falling.first_check = 0;
	}

	return;
}


int main(int argc, char **argv) {
	int offset_x, offset_y, i;
	d_init("pewpewtris", "pewpewtris", NULL);
	d_fs_mount_self();
	d_fs_mount("music.ldi");
	d_fs_mount("sounds.ldi");

	for (i = 0; i < 180; ppt.tile_lookup[i++] = -1);
	ppt.block = d_render_tilesheet_load("res/block.png", 24, 24, DARNIT_PFORMAT_RGB5A1);
	ppt.tile = d_render_tile_new(10 * 18, ppt.block);
	ppt.bbox = d_bbox_new(180);
	ppt.tm = d_tilemap_new(0xFFF, ppt.block, 0xFFF, 10, 18);
	offset_x = 288;
	offset_y = 0;
	d_tilemap_camera_move(ppt.tm, -offset_x, -offset_y);
	ppt.play_background = d_map_load("res/playfield_background.ldmz");
	get_new_block();
	bullet_init(30);

	for (;;) {
		move_block();
		bullet_move();

		if (d_keys_get().a) {
			d_keys_set(d_keys_get());
			bullet_fire(0, 0, 500, 0, 120);
		}
		
		d_render_begin();
		d_render_offset(-ppt.bs_x - offset_x, -ppt.bs_y - offset_y);
		d_render_tile_draw(ppt.tile, 4);
		d_render_offset(0, 0);
		d_tilemap_draw(ppt.tm);
		d_tilemap_draw(ppt.play_background->layer[0].tilemap);
		d_render_offset(-offset_x, -offset_y);
		bullet_draw();
		d_render_end();
		d_loop();
	}

	return 0;
}
