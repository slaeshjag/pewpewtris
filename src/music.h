#ifndef __MUSIC_H__
#define	__MUSIC_H__

struct music_entry {
	DARNIT_SOUND		*music;
	int			key;
};


struct music {
	struct music_entry	*entry;
	int			entries;
	int			cur;
};


void music_init();
void music_loop();

#endif
