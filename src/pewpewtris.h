#ifndef __PEWPEWTRIS_H__
#define	__PEWPEWTRIS_H__

#include <darnit/darnit.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "block.h"


struct {
	DARNIT_TILESHEET		*block;
	DARNIT_TILE			*tile;
	DARNIT_BBOX			*bbox;
	DARNIT_TILEMAP			*tm;
	int				bs_x;
	int				bs_y;
	struct block_struct		falling;
} ppt;



#endif
