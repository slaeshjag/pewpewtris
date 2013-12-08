#ifndef __BLOCK_H__
#define	__BLOCK_H__

#ifdef REQUIRE_DATA

char block_0[] = {
	0, 0, 1, 0,
	0, 0, 1, 0,
	0, 0, 1, 0,
	0, 0, 1, 0
};

char block_1[] = {
	0, 0, 0, 0,
	1, 1, 0, 0,
	0, 1, 1, 0,
	0, 0, 0, 0
};


char block_2[] = {
	0, 0, 0, 0,
	0, 0, 1, 1,
	0, 1, 1, 0,
	0, 0, 0, 0
};


char block_3[] = {
	0, 0, 0, 0,
	0, 1, 1, 0,
	0, 1, 1, 0,
	0, 0, 0, 0
};

char block_4[] = {
	0, 0, 0, 0,
	0, 1, 0, 0,
	1, 1, 1, 0,
	0, 0, 0, 0
};


char block_5[] = {
	0, 0, 0, 0,
	0, 0, 0, 1,
	0, 1, 1, 1,
	0, 0, 0, 0
};

char block_6[] = {
	0, 0, 0, 0,
	1, 0, 0, 0,
	1, 1, 1, 0,
	0, 0, 0, 0
};

#endif

struct block_struct {
	char		blocks[16];
	int		box_id[4];
	int		first_check;
};

struct block_struct block_new();
struct block_struct block_rotate(struct block_struct bs);

#endif
