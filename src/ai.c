#include "pewpewtris.h"

int ai_rate_spot(int index) {
	int top[10], block[4], score[4];
	int i, s, q, w, min, match, bonus;
	
	check_topography_falling(block);
	check_topography_tm(top, 10, 0);
	for (i = 0; i < 4; i++)
		if (block[i] >= 0)
			break;
	s = i;
	for (i = 0; i < 4; i++)
		if (block[i] >= 0)
			q = i;
	index -= s;
	if (index + q >= 10)
		return -1123131;

	min = 18;
	/* Check 4 spots on the main area */
	for (i = 0; i < 4; i++) {
		if (index + i < 0)
			continue;
		if (index + i >= 10)
			continue;
		if (!block[i])
			continue;
		score[i] = top[index + i] - block[i];
		if (score[i] < min)
			min = score[i];
	}


	/* Determine score match */
	for (i = match = 0; i < 4; i++)
		if (score[i] == min)
			match++;
	bonus = 0;
	if (match == (q - s + 1))
		bonus = min;
	else 
		bonus = -20;
	
//	match /= (q - s + 2);
	/* Use this as score */
	return match * min + bonus;
}


struct ai_best_pos ai_find_best_spot() {
	int score[10], block[4];
	int i, max_score, max_i, s;
	struct ai_best_pos bp;

	check_topography_falling(block);
	for (i = 0; i < 4; i++)
		if (block[i] >= 0)
			break;
	s = i;

	max_score = -4000;
	for (i = 0; i < 10; i++) {
		score[i] = ai_rate_spot(i);
		if (score[i] > max_score) {
			max_score = score[i];
			max_i = i - s;
		}
	}
	
	fprintf(stderr, "Index %i best (%i)\n", max_i, max_score);

	bp.pos = max_i;// - s;
	bp.score = max_score;

	return bp;
}


void ai_determine_best_move() {
	struct ai_best_pos p[4];
	struct block_struct bs;
	int i, max_i, max;

	bs = ppt.falling;
	max = -10000;
	i = 0;
	for (i = 0; i < 4; i++) {
		p[i] = ai_find_best_spot();
		ppt.falling = block_rotate(ppt.falling);
		if (max < p[i].score) {
			max = p[i].score;
			max_i = i;
		}
	}
	
	for (i = 0; i < max_i; i++)
		ppt.falling = block_rotate(ppt.falling);
	fprintf(stderr, "Dropping at pos %i\n", p[max_i].pos);
	ppt.bs_x = p[max_i].pos * 24;
}
