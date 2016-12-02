#include <iostream>
#include "player.h"

Player& Player::getInstance() 
{
   static Player player;
   return player;
}

Player::Player(): lives(0), playerSprite(new TwoWayMultiSprite("xwing")), 
strategies(), enemiesDestroyed(0) 
{
  strategies.push_back(new RectangularCollisionStrategy);
  strategies.push_back(new PerPixelCollisionStrategy);
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

float Player::getXVelocity() const
{
  return playerSprite->velocityX();
}

float Player::getYVelocity() const
{
  return playerSprite->velocityY();
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

