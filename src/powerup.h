#ifndef __POWERUP_H__
#define	__POWERUP_H__


/* How many percent accuracy before powerups are even enabled */
/* This is actually the miss ratio */
#define	POWERUP_THRESHOLD	300
#define	POWERUP_LIKELY_SPAWN	10

#define	POWERUP_BASE		21
#define	POWERUP_COUNT		5


#ifdef REQUIRE_POWERUP_DATA
static const int powerup_threshold[] = {
	300,			/* More rapid fire */
	250,			/* Filler powerup */
	200,			/* Shifter */
	150,			/* Stair bomb */
	100,			/* Nuke */
};
#endif


enum powerup_num {
	POWERUP_NUM_GATLINGG,
	POWERUP_NUM_FILLER,
	POWERUP_NUM_SHIFTER,
	POWERUP_NUM_STAIRBOMB,
	POWERUP_NUM_NUKE,
};


void powerup_spawn();
void powerup_add(int powerup);
void powerup_nuke_do();
void powerup_filler_do();
void powerup_stairbomb_do();
void powerup_activate();


#endif
