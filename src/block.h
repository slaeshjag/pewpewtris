#ifndef __BLOCK_H__
#define	__BLOCK_H__

#define	BLOCK_TYPES		4

enum block_type {
	BLOCK_TYPE_NONE		= 0,
	BLOCK_TYPE_1		= 1,
	BLOCK_TYPE_2		= 2,
	BLOCK_TYPE_3		= 3,
	BLOCK_TYPE_4		= 4,
	BLOCK_TYPE_FILLER	= 5,
	BLOCK_TYPE_SOLID	= 20,
	BLOCK_TYPE_GATLINGG	= 21,
	BLOCK_TYPE_PFILLER	= 22,
	BLOCK_TYPE_NUKE		= 23,
};

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

static unsigned char block_color_a[] = {
	0, 0, 0, 0,
	0, 0, 255, 255,
	255, 0, 255, 255,
	255, 0, 0, 255,
	219, 219, 0, 255,
	0, 0, 0, 255
};



#endif


struct block_struct {
	char		blocks[16];
	int		box_id[4];
	int		x[4];
	int		y[4];
	int		first_check;
};

struct block_particle {
	DARNIT_PARTICLE	*p[180];
	int		used[180];
};

struct block_struct block_new();
struct block_struct block_rotate(struct block_struct bs);
void block_check_line();
void block_move();
void block_move_loop();
void block_get_new();
void block_destroy(int index);
int block_impact(int index, int damage);

void block_add_special_slot(int i);
int block_pop_special_slot();

void block_particle_init();
void block_particle_reset();
void block_color(int id, int *r, int *g, int *b);
void block_particle_trig(int x, int y, int r, int g, int b);
void block_particle_draw();

#endif
