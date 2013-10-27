#ifndef __BLOCK_H__
#define	__BLOCK_H__

#ifdef REQUIRE_DATA

int block_0[] = {
	0, 0, 1, 0,
	0, 0, 1, 0,
	0, 0, 1, 0,
	0, 0, 1, 0
};

int block_1[] = {
	0, 0, 0, 0,
	1, 1, 0, 0,
	0, 1, 1, 0,
	0, 0, 0, 0
};


int block_2[] = {
	0, 0, 0, 0,
	0, 0, 1, 1,
	0, 1, 1, 0,
	0, 0, 0, 0
};


int block_3[] = {
	0, 0, 0, 0,
	0, 1, 1, 0,
	0, 1, 1, 0,
	0, 0, 0, 0
};

int block_4[] = {
	0, 0, 0, 0,
	0, 1, 0, 0,
	1, 1, 1, 0,
	0, 0, 0, 0
};


int block_5[] = {
	0, 0, 0, 0,
	0, 0, 0, 1,
	0, 1, 1, 1,
	0, 0, 0, 0
};

int block_6[] = {
	0, 0, 0, 0,
	1, 0, 0, 0,
	1, 1, 1, 0,
	0, 0, 0, 0
};

#endif

struct block_struct {
	char		blocks[16];
	int		box_id[4];
};

struct block_struct block_new();

#endif
