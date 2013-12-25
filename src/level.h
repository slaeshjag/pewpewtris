#ifndef __LEVEL_H__
#define	__LEVEL_H__

struct level {
	int			blocks;
	int			block_dy;
	int			level;

	int			bullet_miss;
	int			bullet_total;
	
	unsigned int		block_likely[BLOCK_TYPES + 1];
};


void level_init();
void level_update();

#endif
