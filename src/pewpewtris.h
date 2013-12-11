#ifndef __PEWPEWTRIS_H__
#define	__PEWPEWTRIS_H__

#include <darnit/darnit.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "block.h"
#include "ai.h"
#include "bullet.h"


struct {
	DARNIT_TILESHEET		*block;
	DARNIT_TILE			*tile;
	DARNIT_BBOX			*bbox;
	DARNIT_TILEMAP			*tm;
	DARNIT_MAP			*play_background;
	int				tile_lookup[180];
	int				bs_x;
	int				bs_y;
	struct block_struct		falling;
	struct bullet			bullet;
} ppt;


void check_topography_falling(int buf[4]);
void check_topography_tm(int *buf, int size, int x);


#endif
