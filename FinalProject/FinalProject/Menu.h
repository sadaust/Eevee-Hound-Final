#pragma once
#include "ResDefs.h"
#include <vector>
// base menu class
// got to figure out how to update and draw
// otherwise, it's got all the bits it needs besides a cursor.




struct Button {
	TextStruct text;
	bool clickable, hovered, clicked;
};





class Screen {
private:
	std::vector <Button> buttons;
public:
	~Screen();
	Screen();
	void Init(); // not sure if we need this or if this will even be used
	void AddButt(Button a_butt);
	void AddButt(int a_index, frect a_rect, char* a_text, bool a_clickable);
	Button& GetButt(int a_index);
	void SetButt(int a_index, frect a_rect, char* a_text, bool a_clickable);
};


class Menu {
private:
	std::vector <Screen> screens;
public:
	~Menu();
	Menu();
	void Init();
	void Update();
	void Draw();
};