#include "hud.h"
#include "aaline.h"
#include "ioManager.h"
#include <iostream>
#include <string>
#include <iomanip>
void Hud::draw(int seconds, int FPS, int lives, int enemies) const {
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
    io.printMessageAt("F key to move right", 10, 190);
    if(enemies<5000) {
      io.printMessageAt("Spacebar to shoot", 10, 220);
      io.printMessageValueAt("Damage: ", lives, 10, 250);
      io.printMessageValueAt("Points: ", enemies, 10, 280);
      io.printMessageAt("Kill enemies to earn at least", 10, 310);
      io.printMessageAt("3000 points & save the galaxy!", 10, 340);
    }
    else {
    io.printMessageAt("Spacebar to shoot",10,220);
    io.printMessageAt("YOU WON!", 10, 250);
    io.printMessageValueAt("Final score: ", enemies - lives, 10, 250);
    }
}
