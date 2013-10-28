#include "pewpewtris.h"


void ai_find_best_spot() {
	int top[10], block[4];
	int i, j, s, q, a, b, c, d, e, m, r;

	check_topography_falling(block);
	check_topography_tm(top, 10, 0);
	for (i = 0; i < 4; i++)
		if (block[i])
			break;
	s = i;
	fprintf(stderr, "Detected s=%i\n", s);
	for (i = 0; i < 4; i++)
		if (block[i])
			q = i;
	c = d = e = 0;
	for (i = -s; i < 13 - q; i++) {
		a = b = 0;
		m = 18;
		for (j = 0; j < 4; j++) {
			if (i + j < 0 || i + j >= 10)
				continue;
			m = (m < top[i + j]) ? m : top[i + j];
		}

		fprintf(stderr, "m=%i\n", m);
		r = 0;
		for (j = 3; j >= 0; j--)
			if (block[j])
				r = (r < block[j]) ? block[j] : r;
		m -= (r);
		fprintf(stderr, "New n=%i\n", m);
		for (j = 0; j < 4; j++) {
			if (i + j < 0)
				continue;
			if (i + j >= 10)
				continue;
			if (!block[j])
				continue;
			if (block[j] + m == top[i + j]) {
				fprintf(stderr, "a++ @ %i %i (%i + %i == %i)\n", i, j, block[j], m, top[i + j]);
				a++;
			} else if (block[j] + m == top[i + j] - 1) {
				fprintf(stderr, "b++ @ %i %i (%i + %i == %i)\n", i, j, block[j], m, top[i + j] - 1);
				b++;
			}
		}

		if (a > c || a == c && b > d) {
			fprintf(stderr, "Better!\n");
			c = a, d = b, e = i;
		}
	}

	fprintf(stderr, "Match: %i %i @ %i\n", c, d, e);
	ppt.bs_x = e * 24;

}
