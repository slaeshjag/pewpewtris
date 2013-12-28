#ifndef __POWERUP_H__
#define	__POWERUP_H__


/* How many percent accuracy before powerups are even enabled */
/* This is actually the miss ratio */
#define	POWERUP_THRESHOLD	700
#define	POWERUP_LIKELY_SPAWN	10

#define	POWERUP_BASE		21
#define	POWERUP_COUNT		2


#ifdef REQUIRE_POWERUP_DATA
static const int powerup_threshold[] = {
	700,			/* Gatling gun */
	100,			/* Nuke */
};
#endif


void powerup_spawn();

#endif
