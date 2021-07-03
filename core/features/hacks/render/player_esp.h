#pragma once

class c_player_esp {

	void draw_2d_box(entity_t* e, const Box& b);
	void draw_name(entity_t* e, const Box& b);
	void draw_skeleton(player_t* e, color color, matrix_t matrix[MAXSTUDIOBONES]);
	

public:

	void on_draw();
};

extern c_player_esp g_player_esp;