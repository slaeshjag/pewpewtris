#ifndef __POWERUP_H__
#define	__POWERUP_H__


/* How many percent accuracy before powerups are even enabled */
/* This is actually the miss ratio */
#define	POWERUP_THRESHOLD	70
#define	POWERUP_LIKELY_SPAWN	10

void powerup_spawn();

#endif
