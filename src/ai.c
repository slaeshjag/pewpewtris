#include "pewpewtris.h"


int ai_rate_spot(int index) {
	int top[10], block[4], score[4];
	int i, s, q, min, match;
	
	check_topography_falling(block);
	check_topography_tm(top, 10, 0);
	for (i = 0; i < 4; i++)
		if (block[i])
			break;
	s = i;
	for (i = 0; i < 4; i++)
		if (block[i])
			q = i;
	index -= s;
	if (index + q >= 10)
		return -1;

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

	/* Use this as score */
	return match * min;
}


void ai_find_best_spot() {
	int score[10], block[4];
	int i, max_score, max_i, s;

	max_score = -1;
	for (i = 0; i < 10; i++) {
		score[i] = ai_rate_spot(i);
		if (score[i] > max_score) {
			max_score = score[i];
			max_i = i;
		}
	}
	
	fprintf(stderr, "Index %i best (%i)\n", max_i, max_score);

	check_topography_falling(block);
	for (i = 0; i < 4; i++)
		if (block[i])
			break;
	s = i;
	
	ppt.bs_x = (max_i - s) * 24;

}
