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


void Screen::AddButt(int a_index, frect a_rect, char* a_text, D3DCOLOR a_color, bool a_clickable) {
	Button tempbutt;
	tempbutt.text.rec = a_rect;
	tempbutt.text.text = a_text;
	tempbutt.text.textColor = a_color;
	tempbutt.clickable = a_clickable;
}


int Screen::NumButt() {
	return buttons.size();
}


Button& Screen::GetButt(int a_index) {
	return buttons[a_index];
}


void Screen::SetButt(int a_index, frect a_rect, char* a_text, D3DCOLOR a_color, bool a_clickable) {
	buttons[a_index].text.rec = a_rect;
	buttons[a_index].text.text = a_text;
	buttons[a_index].clickable = a_clickable;
	buttons[a_index].text.textColor = a_color;
}


Menu::~Menu() {

}


Menu::Menu() {

}


void Menu::Init() {
	curScreen = 0;
	Screen tempscreen;
	Button tempbutt;
	frect temprect;
	temprect.bottom = 40;
	temprect.top = 20;
	temprect.left = 50;
	temprect.right = 100;
	tempscreen.AddButt(tempbutt);
	tempscreen.SetButt(0, temprect, "test text", D3DCOLOR(0xffffffff), true);
	temprect.bottom = 140;
	temprect.top = 120;
	temprect.left = 150;
	temprect.right = 400;
	tempscreen.AddButt(tempbutt);
	tempscreen.SetButt(1, temprect, "test text 2", D3DCOLOR(0xffffffff), true);
	screens.push_back(tempscreen);
}


void Menu::Update() {

}


void Menu::Draw(DXFrame& DXVid) {
	RenInfo tempRen;
	tempRen.locCamNum = 1;
	tempRen.type = text;
	for(int i = 0; i < screens[curScreen].NumButt(); ++i) {
		tempRen.asset = &screens[curScreen].GetButt(i).text;
		DXVid.addRen(tempRen);
	}

}

