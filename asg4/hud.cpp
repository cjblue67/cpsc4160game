#include "hud.h"
#include "aaline.h"
#include "ioManager.h"
#include <iostream>
#include <string>
#include <iomanip>
void Hud::draw(int seconds, int FPS) const {
	Draw_AALine(screen, x1,y1, x1,y2, 2.0, 0xFF);
	Draw_AALine(screen, x2,y1, x2,y2, 2.0, 0xFF);
	Draw_AALine(screen, x1,y1, x2,y1, 2.0, 0xFF);
	Draw_AALine(screen, x1,y2, x2,y2, 2.0, 0xFF);
	
	IOManager& io = IOManager::getInstance();
	io.printMessageValueAt("Seconds: ", seconds, 10, 20);
    io.printMessageValueAt("FPS: ", FPS, 10, 45);
    io.printMessageAt("F1 to toggle HUD", 10, 70);
    io.printMessageAt("A key to move left", 10, 100);
    io.printMessageAt("S key to move down", 10, 130);
    io.printMessageAt("W key to move up", 10, 160);
    io.printMessageAt("D key to move right", 10, 190);
}