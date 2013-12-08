#ifndef __AI_H__
#define	__AI_H__

struct ai_best_pos {
	int			pos;
	int			score;
};


struct ai_best_pos ai_find_best_spot();
void ai_determine_best_move();

#endif
