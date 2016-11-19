#ifndef PLAYER_H
#define PLAYER_H
#include <SDL.h>
#include <iostream>
#include "drawable.h"
#include "2wayMultiSprite.h"
#include "gamedata.h"

class Player {
public:
  static Player& getInstance();
  void moveDown();
  void moveLeft();
  void moveRight();
  void moveUp();
  void stopXVelocity();
  void stopYVelocity();
  ~Player() { }
  Drawable* getPlaySprite() const { return playerSprite; } 
  int getLives() const;
  void setLives(int l);
  
private:
  Player();
  int lives;
  Player(const Player&);
  Player& operator=(const Player&);
  Drawable* playerSprite;
};
#endif 
