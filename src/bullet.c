#define	_BULLET_DATA
#include "pewpewtris.h"
#include <limits.h>


void bullet_init_tail(DARNIT_PARTICLE *p) {
	d_particle_color_target(p, 0, 0, 0, 0);
	d_particle_emitter_velocity(p, 0, 256);
	d_particle_emitter_gravity(p, 0, 200);
	d_particle_life(p, 200);
	d_particle_point_size(p, 3);
	d_particle_mode(p, DARNIT_PARTICLE_MODE_SHOWER);

	return;
}


void bullet_init_impact(DARNIT_PARTICLE *p) {
	d_particle_color_target(p, 255, 0, 0, 0);
	d_particle_emitter_velocity(p, 0, 64);
	d_particle_emitter_gravity(p, 0, 200);
	d_particle_life(p, 600);
	d_particle_point_size(p, 3);
	d_particle_mode(p, DARNIT_PARTICLE_MODE_PULSAR);

	return;
}


void bullet_init(int max_bullets) {
	int i;

	ppt.bullet.bullet = malloc(sizeof(*ppt.bullet.bullet) * max_bullets);
	ppt.bullet.bullets = max_bullets;
	ppt.bullet.ts = d_render_tilesheet_load("res/bullet.png", 12, 12, DARNIT_PFORMAT_RGB5A1);
	ppt.bullet.tile = d_render_tile_new(max_bullets, ppt.bullet.ts);

	for (i = 0; i < max_bullets; i++) {
		d_render_tile_move(ppt.bullet.tile, i, INT_MAX, INT_MAX);
		ppt.bullet.bullet[i].mode = BULLET_MODE_WAITING;
		ppt.bullet.bullet[i].tail = d_particle_new(40, DARNIT_PARTICLE_TYPE_POINT);
		ppt.bullet.bullet[i].impact = d_particle_new(40, DARNIT_PARTICLE_TYPE_POINT);
		bullet_init_tail(ppt.bullet.bullet[i].tail);
		bullet_init_impact(ppt.bullet.bullet[i].impact);
	}
	
	return;
}


void bullet_draw() {
	int i;

	for (i = 0; i < ppt.bullet.bullets; i++)
		switch (ppt.bullet.bullet[i].mode) {
			case BULLET_MODE_IDLE:
				continue;
			case BULLET_MODE_WAITING:
				d_particle_draw(ppt.bullet.bullet[i].tail);
				d_particle_draw(ppt.bullet.bullet[i].impact);
				if (!d_particle_used(ppt.bullet.bullet[i].impact)) {
					ppt.bullet.bullet[i].mode = BULLET_MODE_IDLE;
					d_render_tile_move(ppt.bullet.tile, i, INT_MAX, INT_MAX);
					continue;
				}
				break;
			default:
				d_particle_draw(ppt.bullet.bullet[i].tail);
				break;
		}
	
	d_render_tile_draw(ppt.bullet.tile, ~0);

	return;
}


void bullet_kill_all() {
	int i;

	for (i = 0; i < ppt.bullet.bullets; i++) {
		d_particle_clear_all(ppt.bullet.bullet[i].impact);
		d_particle_clear_all(ppt.bullet.bullet[i].tail);
		d_particle_mode(ppt.bullet.bullet[i].tail, DARNIT_PARTICLE_MODE_OFF);
		ppt.bullet.bullet[i].mode = BULLET_MODE_IDLE;
		ppt.bullet.bullet[i].rest_movement = 0;
		d_render_tile_set(ppt.bullet.tile, i, 1);
		d_render_tile_move(ppt.bullet.tile, i, INT_MAX, INT_MAX);
	}

	return;
}


void bullet_eczplode(int i, int x, int y) {
	d_particle_emitter_move(ppt.bullet.bullet[i].impact, x, y);
	d_particle_pulse(ppt.bullet.bullet[i].impact);
	d_particle_mode(ppt.bullet.bullet[i].tail, DARNIT_PARTICLE_MODE_OFF);
	d_render_tile_set(ppt.bullet.tile, i, 1);
	d_render_tile_move(ppt.bullet.tile, i, INT_MAX, INT_MAX);
	ppt.bullet.bullet[i].mode = BULLET_MODE_WAITING;
	ppt.bullet.bullet[i].rest_movement = 0;
	
	return;
}


void bullet_move() {
	int i, xv, yv, x, y;
	unsigned int hit;

	if (ppt.paused)
		return;

	for (i = 0; i < ppt.bullet.bullets; i++) {
		if (ppt.bullet.bullet[i].mode != BULLET_MODE_FLYING)
			continue;
		ppt.bullet.bullet[i].rest_movement += d_last_frame_time() * ppt.bullet.bullet[i].velocity;
		while (ppt.bullet.bullet[i].rest_movement >= 1000) {
			xv = d_util_sin(ppt.bullet.bullet[i].angle + 900);
			yv = d_util_sin(ppt.bullet.bullet[i].angle);
			xv *= 1000;
			yv *= 1000;
			xv /= 65536;
			yv /= 65536;
			ppt.bullet.bullet[i].x += xv;
			ppt.bullet.bullet[i].y += yv;

			x = (ppt.bullet.bullet[i].x / 1000) - 2;
			y = (ppt.bullet.bullet[i].y / 1000) - 2;
			d_particle_emitter_move(ppt.bullet.bullet[i].tail, x + 2, y + 2);
			hit = 0;
			if (x > 236 || y < 0 || y >= 428) {
				ppt.level.bullet_miss++;
				bullet_eczplode(i, x, y);
				continue;
			} else if (d_bbox_test(ppt.bbox, x, y, 5, 5, &hit, 1) > 0) {
				ppt.bullet.bullet[i].hp -= block_impact(hit, ppt.bullet.bullet[i].hp);
				if (!ppt.bullet.bullet[i].hp)
					bullet_eczplode(i, x, y);
				/* TODO: Tell the block about the bad news */

				continue;
			}
			
			ppt.bullet.bullet[i].rest_movement -= 1000;
		}
		
		x = (ppt.bullet.bullet[i].x / 1000);
		y = (ppt.bullet.bullet[i].y / 1000);
		ppt.bullet.bullet[i].age += d_last_frame_time();
		if (ppt.bullet.bullet[i].age >= 2000) {
			d_particle_mode(ppt.bullet.bullet[i].tail, DARNIT_PARTICLE_MODE_OFF);
			ppt.bullet.bullet[i].mode = BULLET_MODE_WAITING;
		}

		if (ppt.bullet.bullet[i].mode == BULLET_MODE_FLYING)
			d_render_tile_move(ppt.bullet.tile, i, x - 5, y - 5);

	}

	return;
}


void bullet_fire(int type, int angle, int velocity, int x, int y) {
	int i;
	unsigned int r, g, b, a;

	for (i = 0; i < ppt.bullet.bullets; i++)
		if (ppt.bullet.bullet[i].mode == BULLET_MODE_IDLE)
			break;
	if (ppt.bullet.bullets == i)
		return;
	d_render_tile_set(ppt.bullet.tile, i, type);
	ppt.bullet.bullet[i].mode = BULLET_MODE_FLYING;
	ppt.bullet.bullet[i].angle = angle;
	ppt.bullet.bullet[i].velocity = velocity;
	ppt.bullet.bullet[i].x = x * 1000;
	ppt.bullet.bullet[i].y = y * 1000;
	ppt.bullet.bullet[i].rest_movement = 0;
	ppt.bullet.bullet[i].age = 0;
	ppt.bullet.bullet[i].hp = ppt.level.bullet_hp;

	d_particle_mode(ppt.bullet.bullet[i].tail, DARNIT_PARTICLE_MODE_SHOWER);
	d_particle_emitter_angle(ppt.bullet.bullet[i].impact, angle - 20 + 3600, angle + 20);
	d_particle_emitter_move(ppt.bullet.bullet[i].tail, x, y);
	angle += 1800;
	d_particle_emitter_angle(ppt.bullet.bullet[i].tail, angle - 200, angle + 200);
	r = bullet_color[(type << 2)];
	g = bullet_color[(type << 2) + 1];
	b = bullet_color[(type << 2) + 2];
	a = bullet_color[(type << 2) + 3];
	d_particle_color_start(ppt.bullet.bullet[i].tail, r, g, b, a);
	d_particle_color_start(ppt.bullet.bullet[i].impact, r, g, b, a);
	ppt.level.bullet_total++;

	return;
}
