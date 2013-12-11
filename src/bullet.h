#ifndef __BULLET_H__
#define	__BULLET_H__

#ifdef _BULLET_DATA
static const unsigned char bullet_color[] = {
	255, 255, 0, 255
};
#endif

enum bullet_mode {
	BULLET_MODE_IDLE,
	BULLET_MODE_FLYING,
	BULLET_MODE_WAITING,
};


struct bullet_entry {
	int			angle;

	/* Set these two on spawn */
	int			velocity;
	int			rest_movement;
	/* Pixel * 1000 */
	int			x;
	int			y;
	enum bullet_mode	mode;
	DARNIT_PARTICLE		*tail;
	DARNIT_PARTICLE		*impact;
};


struct bullet {
	struct bullet_entry	*bullet;
	int			bullets;
	DARNIT_TILE		*tile;
	DARNIT_TILESHEET	*ts;
};


#endif
