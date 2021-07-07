#pragma once

class c_player_esp {

	void draw_2d_box(entity_t* e, const Box& b);
	void draw_name(entity_t* e, const Box& b);
	void draw_skeleton(player_t* e, color color, matrix_t matrix[MAXSTUDIOBONES]);
	

public:

	bool is_enabled = false;
	color teammate_color = {
		0,
		255,
		0,
		255
	};
	color enemy_color = {
		255,
		0,
		0,
		255
	};
	void on_draw();
};

extern c_player_esp g_player_esp;