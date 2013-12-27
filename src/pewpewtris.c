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

		if (i + x < 0) {
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


static void init() {
	int i;

	d_init("pewpewtris", "pewpewtris", NULL);
	d_fs_mount_self();
	d_fs_mount("music.ldi");
	d_fs_mount("sounds.ldi");
	config_init();
	
	ppt.ui.offset_x = 288;
	ppt.ui.offset_y = 0;

	for (i = 0; i < 180; ppt.tile_lookup[i++] = -1);

	ppt.block = d_render_tilesheet_load("res/block.png", 24, 24, DARNIT_PFORMAT_RGB5A1);
	ppt.tile = d_render_tile_new(10 * 18, ppt.block);
	ppt.bbox = d_bbox_new(180);
	d_bbox_set_indexkey(ppt.bbox);
	ppt.tm = d_tilemap_new(0xFFF, ppt.block, 0xFFF, 10, 18);
	ppt.request_new = 0;
	ppt.font = d_font_load("res/font.ttf", 28, 256, 256);
	ui_init();
	bullet_init(30);
	highscore_init();
	state_init();
	music_init();
	
	ppt.ui.play_background = d_map_load("res/playfield_background.ldmz");
	ppt.ui.menu_background = d_map_load("res/mainmenu_background.ldmz");
	ppt.ui.highscore_background = d_map_load("res/highscore_background.ldmz");


	/* FIXME: Remove */
	ppt.fps = d_text_surface_new(ppt.font, 6, 1000, 0, 0);
}


int main(int argc, char **argv) {
	char fps[6];
	init();

	d_tilemap_camera_move(ppt.tm, -ppt.ui.offset_x, -ppt.ui.offset_y);

	for (;;) {
		d_render_begin();

		state_loop();

		/* FIXME: Remove */
		sprintf(fps, "%i", d_fps());
		d_text_surface_reset(ppt.fps);
		d_text_surface_string_append(ppt.fps, fps);
		d_render_blend_enable();
		d_text_surface_draw(ppt.fps);
		d_render_blend_disable();

		d_render_end();
		d_loop();
	}

	return 0;
}
