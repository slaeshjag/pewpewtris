#ifndef __CONFIG_H__
#define	__CONFIG_H__


struct config {
	int			music_vol;
	int			sfx_vol;
};


void config_init();
void config_save();

#endif
