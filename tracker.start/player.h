#ifndef PLAYER_H
#define PLAYER_H
#include <SDL.h>
#include <iostream>
#include "drawable.h"
#include <vector>
#include "collisionStrategy.h"
#include "2wayMultiSprite.h"
#include "gamedata.h"

class Player {
public:
  static Player& getInstance();
  bool collidedWith(const Drawable* d) const 
  {
    if(strategies[0]->execute(*playerSprite, *d))
    {
      return strategies[1]->execute(*playerSprite, *d);
    }
    return false;
  }
  void moveDown();
  void moveLeft();
  void moveRight();
  void moveUp();
  float getXVelocity() const;
  float getYVelocity() const;
  void stopXVelocity();
  void stopYVelocity();
  ~Player() { }
  Drawable* getPlaySprite() const { return playerSprite; } 
  int getLives() const;
  void setLives(int l);
  int getEnemiesDestroyed() const {return enemiesDestroyed;}
  void setEnemiesDestroyed(int e) {enemiesDestroyed = e;}
private:
  Player();
  int lives;
  Player(const Player&);
  Player& operator=(const Player&);
  Drawable* playerSprite;
  std::vector<CollisionStrategy*> strategies;
  int enemiesDestroyed;
};
#endif 
