#include "twoWayScaledSprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include <iostream>
#include <cmath>
#include "SDL/SDL_rotozoom.h"
#include "gamedata.h"
#include "explodingSprite.h"
#include "collisionStrategy.h"

void TwoWayScaledSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	Vector2f direction = getVelocity();
	if (timeSinceLastFrame > frameInterval && direction[0] >= 0) {
                currentFrame = (currentFrame+1) % (numberOfFrames/2);
		timeSinceLastFrame = 0;
	}
	else if(timeSinceLastFrame > frameInterval && direction[0] < 0) {
                currentFrame = (currentFrame-1) % (numberOfFrames/2) + (numberOfFrames/2);
                timeSinceLastFrame = 0;
        }
}

TwoWayScaledSprite::TwoWayScaledSprite(const std::string& name, 
                           SDL_Surface* surface) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f((rand()%2?1:-1)*getRandom(
                    Gamedata::getInstance().getXmlInt(name+"/cushion"),
                    Gamedata::getInstance().getXmlInt(name+"/speedX")),
                    (rand()%2?1:-1)*getRandom(
                    Gamedata::getInstance().getXmlInt(name+"/cushion"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
                   )
  ), 
  scale(getRandFloat(Gamedata::getInstance().getXmlFloat(name+"/scale/min"),
                     Gamedata::getInstance().getXmlFloat(name+"/scale/max"))
  ),
  scaledSurface( rotozoomSurface(surface, 0, scale, SMOOTHING_ON) ),
  frames( FrameFactory::getInstance().getFrames(name) ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  explosion(NULL),
  strategies()
  {
    strategies.push_back(new RectangularCollisionStrategy);
    strategies.push_back(new PerPixelCollisionStrategy); 
  }


TwoWayScaledSprite::TwoWayScaledSprite(const TwoWayScaledSprite& s) :
  Drawable(s), 
  scale(s.scale),
  scaledSurface(s.scaledSurface),
  frames(s.frames),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  explosion(NULL),
  strategies()
  {
    strategies.push_back(new RectangularCollisionStrategy);
    strategies.push_back(new PerPixelCollisionStrategy); 
  }

void TwoWayScaledSprite::draw() const {
  if(explosion)
  {
    explosion->draw();
    return;
  } 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
}

void TwoWayScaledSprite::update(Uint32 ticks) { 
  if(explosion)
  {
    explosion->update(ticks);
    if(explosion->chunkCount() == 0)
    {
      delete explosion;
      explosion = NULL;
    }
    return;
  }

  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }  

}

bool TwoWayScaledSprite:: collidedWith(const Drawable* d) const
{
  if(strategies[0]->execute(*this, *d))
  {
    return strategies[1]->execute(*this, *d);
  }
  return false;
}

void TwoWayScaledSprite::explode()
{
  if(explosion)
  {
    explosion->draw();
    return;
  }
  Sprite sprite(getName(), getPosition(), getVelocity(), frames[currentFrame]);
  explosion = new ExplodingSprite(sprite);
}
