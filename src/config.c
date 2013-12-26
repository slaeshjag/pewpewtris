#include "pewpewtris.h"
#include <string.h>


void config_init() {
	DARNIT_FILE *f;
	char buff[512];

	ppt.config.music_vol = 40;
	ppt.config.sfx_vol = 31;

	if (!(f = d_file_open("pewpewtris.config", "rb"))) {
		config_save();
		return;
	}

	*buff = 0;
	d_file_getln(buff, 512, f);
	d_file_close(f);

	sscanf(buff, "%i %i", &ppt.config.music_vol, &ppt.config.sfx_vol);
	
	return;
}


void config_save() {
	DARNIT_FILE *f;
	char buff[512];

	f = d_file_open("pewpewtris.config", "wb");

	sprintf(buff, "%i %i\n", ppt.config.music_vol, ppt.config.sfx_vol);
	d_file_write(buff, strlen(buff), f);

	return;
}
