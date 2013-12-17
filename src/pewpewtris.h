#ifndef __PEWPEWTRIS_H__
#define	__PEWPEWTRIS_H__

#include <darnit/darnit.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "block.h"
#include "ai.h"
#include "bullet.h"
#include "ui.h"


struct {
	DARNIT_TILESHEET		*block;
	DARNIT_TILE			*tile;
	DARNIT_BBOX			*bbox;
	DARNIT_TILEMAP			*tm;
	DARNIT_MAP			*play_background;
	DARNIT_FONT			*font;
	int				tile_lookup[180];
	int				bs_x;
	int				bs_y;
	int				d_y;
	int				current_speed;
	int				request_new;
	struct block_struct		falling;
	struct bullet			bullet;
	struct ui			ui;
} ppt;


void check_topography_falling(int buf[4]);
void check_topography_tm(int *buf, int size, int x);


#endif
