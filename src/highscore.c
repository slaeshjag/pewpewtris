#include "pewpewtris.h"
#include <string.h>


void highscore_swap_endian() {
	int i;

	for (i = 0; i < HIGHSCORE_NUMBER; i++) {
		ppt.highscore.highscore[i].score = d_util_htonl(ppt.highscore.highscore[i].score);
		ppt.highscore.highscore[i].level = d_util_htonl(ppt.highscore.highscore[i].level);
	}

	return;
}


void highscore_init() {
	DARNIT_FILE *f;

	memset(&ppt.highscore, 0, sizeof(ppt.highscore));

	if (!(f = d_file_open(HIGHSCORE_FILE, "rb")))
		return;
	d_file_read_compressed(f, &ppt.highscore, sizeof(ppt.highscore));
	highscore_swap_endian();
	
	d_file_close(f);

	return;
}


void highscore_save() {
	DARNIT_FILE *f;

	highscore_swap_endian();
	f = d_file_open(HIGHSCORE_FILE, "w");
	d_file_write_compressed(f, &ppt.highscore, sizeof(ppt.highscore));
	d_file_close(f);
	highscore_swap_endian();

	return;
}


void highscore_add(char name[32]) {
	int i;

	for (i = 0; i < HIGHSCORE_NUMBER; i++)
		if (ppt.highscore.highscore[i].score < ppt.ui.score_n) {	/* Add score */
			memmove(&ppt.highscore.highscore[i + 1], &ppt.highscore.highscore[i], sizeof(ppt.highscore.highscore[i]) * (HIGHSCORE_NUMBER - i - 1));
			strcpy(ppt.highscore.highscore[i].name, name);
			ppt.highscore.highscore[i].score = ppt.ui.score_n;
			ppt.highscore.highscore[i].level = ppt.level.level;
			highscore_save();
			break;
		}
	return;
}


int highscore_is_new() {
	int i;

	for (i = 0; i < HIGHSCORE_NUMBER; i++)
		if (ppt.highscore.highscore[i].score < ppt.ui.score_n)
			return 1;
	return 0;
}
