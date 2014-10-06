#pragma once
#include "Menu.h"








Screen::~Screen() {

}


Screen::Screen() {

}


void Screen::Init() {
	// not sure if we need this or if this will even be used
}


void Screen::AddButt(Button a_butt) {
	buttons.push_back(a_butt);
}


void AddButt(int a_index, RECT a_rect, char* a_text, bool a_clickable) {
	Button tempbutt;
	tempbutt.text.rec = a_rect;
	tempbutt.text.text = a_text;
	tempbutt.clickable = a_clickable;
}


Button& Screen::GetButt(int a_index) {
	return buttons[a_index];
}


void Screen::SetButt(int a_index, RECT a_rect, char* a_text, bool a_clickable) {
	buttons[a_index].text.rec = a_rect;
	buttons[a_index].text.text = a_text;
	buttons[a_index].clickable = a_clickable;
}


Menu::~Menu() {

}


Menu::Menu() {

}


void Menu::Init() {
	Screen tempscreen;
	Button tempbutt;
	RECT temprect;
	temprect.bottom = 40;
	temprect.top = 20;
	temprect.left = 50;
	temprect.right = 100;
	tempscreen.AddButt(tempbutt);
	tempscreen.SetButt(0, temprect, "test text", true);
	temprect.bottom = 140;
	temprect.top = 120;
	temprect.left = 150;
	temprect.right = 400;
	tempscreen.AddButt(tempbutt);
	tempscreen.SetButt(0, temprect, "test text 2", true);
	
}


void Menu::Update() {

}


void Menu::Draw() {

}

