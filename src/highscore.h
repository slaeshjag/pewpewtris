#ifndef __HIGHSCORE_H__
#define	__HIGHSCORE_H__

#define	HIGHSCORE_NUMBER	10
#define	HIGHSCORE_FILE		"pewpewtris.highscore"

struct highscore_entry {
	char			name[32];
	unsigned int		score;
	unsigned int		level;
};


struct highscore_file {
	struct highscore_entry	highscore[HIGHSCORE_NUMBER];
};

void highscore_init();
void highscore_save();
void highscore_add(char name[32]);
int highscore_is_new();


#endif
