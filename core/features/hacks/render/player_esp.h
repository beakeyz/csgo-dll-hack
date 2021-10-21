#pragma once

class c_skeleton_esp : public singleton<c_skeleton_esp> {

public:
	//void draw_2d_box(entity_t* e, const Box& b);
	//void draw_name(entity_t* e, const Box& b);
	void draw_skeleton(player_t* e, color color, matrix_t matrix[MAXSTUDIOBONES]);
	

public:

	bool teammates = false;
	bool enemies = false;
	
	std::unordered_map<int, std::string> color_map = { {0, "Red"}, {1, "Orange"}, {2, "Yellow"}, {3, "Green"}, {4, "Cyan"}, {5, "Blue"}, {6, "Purple"}, {7, "Pink"},  {8, "White"}, {9, "Black"}};

	std::string enemy_color;
	std::string team_color;

	color get_color(std::unordered_map<int, std::string> p_color_map, std::string c);
	void on_draw();
};