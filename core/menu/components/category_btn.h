#pragma once
#include "./../../../dependencies/utilities/csgo.hpp"
#include "./../framework.hpp"
#include "./../variables.hpp"

#include "group_box.h"

class c_category_btn{

public:

	c_category_btn(int& tab, const int locale_tab, std::string category_name) : m_tab(tab) {
		this->m_tab = m_tab;
		this->m_locale_tab = locale_tab;
		this->m_name = category_name;

		this->m_boxes = {};
	}

	~c_category_btn() {
		this->m_boxes.clear();
	}

	//void draw_box(groupBox& box);

	void draw(std::string previous_name);

	void mouseClicked(int mouseX, int mouseY, int btn);

public:
	int& m_tab;
	int m_locale_tab;
	std::string m_name;


public:
	std::set<groupBox*> m_boxes = {};
};