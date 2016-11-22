#include <iostream>
#include "player.h"

Player& Player::getInstance() 
{
   static Player player;
   return player;
}

Player::Player(): lives(0), playerSprite(new TwoWayMultiSprite("xwing")), 
strategies() 
{
  strategies.push_back(new PerPixelCollisionStrategy);
  strategies.push_back(new RectangularCollisionStrategy);
} 

void Player::moveRight()
{
  playerSprite->velocityX(150);
}

void Player::moveLeft()
{
  playerSprite->velocityX(-150);
}

void Player::moveUp()
{
  playerSprite->velocityY(-150);
}

void Player::moveDown()
{
  playerSprite->velocityY(150);
}

void Player::stopXVelocity()
{
  playerSprite->velocityX(0);
}

void Player::stopYVelocity()
{
  playerSprite->velocityY(0);
}

int Player:: getLives() const{
  return lives;
}

void Player:: setLives(int l) {
  lives = l;
}

