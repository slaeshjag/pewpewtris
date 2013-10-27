#define	REQUIRE_DATA
#include "pewpewtris.h"


struct block_struct block_rotate(struct block_struct bs) {
	struct block_struct bs_new;
	int i;

	memset(bs.blocks, 0, 16);
	for (i = 0; i < 16; i++) {
		/* TODO: Implement. Short on time */
	}
	
	return bs;
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
