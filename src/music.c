#include "pewpewtris.h"


void music_init() {
	int entries, i, v;
	char buff[512];
	DARNIT_DIR_LIST *dir, *next;
	
	ppt.music.entries = 0;
	ppt.music.cur = 0;

	entries = 0;
	dir = d_file_list("music/", DARNIT_FILESYSTEM_TYPE_READ, &entries);
	if (!entries)
		return;
	ppt.music.entry = malloc(sizeof(*ppt.music.entry) * entries);
	ppt.music.entries = entries;
	next = dir;
	for (i = 0; next; next = next->next, i++) {
		sprintf(buff, "music/%s", next->fname);
		ppt.music.entry[i].music = d_sound_tracked_load(buff, DARNIT_AUDIO_STREAM, DARNIT_AUDIO_STEREO);
		fprintf(stderr, "Adding %s to playlist\n", buff);
	}

	d_file_list_free(dir);
	i = ppt.music.cur;
	v = ppt.config.music_vol;
	ppt.music.entry[i].key = d_sound_play(ppt.music.entry[i].music, 0, v, v, 0);
	
	return;
}


void music_loop() {
	int v;

	if (!ppt.music.entries)
		return;

	v = ppt.config.music_vol;
	if (d_sound_playback_status(ppt.music.entry[ppt.music.cur].key) < 0) {
		if (++ppt.music.cur >= ppt.music.entries)
			ppt.music.cur = 0;
		ppt.music.entry[ppt.music.cur].key = d_sound_play(ppt.music.entry[ppt.music.cur].music, 0, v, v, 0);
	}

	return;
}

