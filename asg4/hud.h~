#ifndef HUD_H
#define HUD_H
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>
#include <iomanip>
#include "ioManager.h"

class Hud {
private:
SDL_Surface* screen;
bool display;
int x1,x2,y1,y2;
Hud(const Hud&);

Hud& operator=(const Hud&);
public:
Hud(SDL_Surface* s):screen(s), display(true), x1(5), x2(210), y1(5),y2(210){};
void draw(int seconds, int FPS) const;
bool get_display() const {return display;};
void change_display() {display = display ^ 1;};
void set_false() {display = false;};
~Hud() {};
};
#endif
