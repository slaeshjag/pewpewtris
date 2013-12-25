#ifndef __POWERUP_H__
#define	__POWERUP_H__


/* How many percent accuracy before powerups are even enabled */
/* This is actually the miss ratio */
#define	POWERUP_THRESHOLD	30
#define	POWERUP_LIKELY_SPAWN	50

void powerup_spawn();

#endif
