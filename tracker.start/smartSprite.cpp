#include <cmath>
#include "viewport.h"
#include "sprite.h"
#include "explodingSprite.h"
#include "smartSprite.h"
#include "collisionStrategy.h"

SmartSprite::SmartSprite(const std::string& name, 
             const Vector2f& pos, const float v, const std::vector<Sprite*> b) :
  Sprite(name, pos,
    Vector2f(Gamedata::getInstance().getXmlInt(name+"speedX"),
             Gamedata::getInstance().getXmlInt(name+"speedY"))
  ),
  frame(FrameFactory::getInstance().getFrame(name) ),
  io(IOManager::getInstance()),
  enemyVelocityX(v),
  bullets(b),
  currentMode(NORMAL),
  strategies()
{
  strategies.push_back(new RectangularCollisionStrategy);
  strategies.push_back(newPerPixelCollisionStrategy); 
}

void SmartSprite::draw() const { 
  if(explosion)
  {
    explosion->draw();
    return;
  }
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y); 
}

void SmartSprite::update(Uint32 ticks) { 
  if(explosion)
  {
    explosion->update(ticks);
    //check position, delete if offscreen
    if(explosion->chunkCount() == 0)
    {
      delete explosion;
      explosion = NULL;
    }
    return;
  }
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

void SmartSprite::explode()
{
  if(explosion)
  {
    explosion->draw();
    return;
  }
  explosion = new ExplodingSprite(*this);
}

bool SmartSprite::collidedWith(const Drawable* d) const
{
  std::cout << "collided!\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << std::endl;
  if(strategies[0]->execute(*this, *d))
  {
    return strategies[1]->execute(*this, *d);
  }
  return false;
}

void SmartSprite::goUp() { velocityY( -fabs(velocityY()) ); }
void SmartSprite::goDown() { velocityY( fabs(velocityY()) ); }
void SmartSprite::update(Uint32 ticks)
{
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

  bool laserLine = false;
  for(unsigned int i = 0; i < bullets.size(); i++)
  {
    if((bullets[i]->Y() < (Y()+20)) && (bullets[i]->Y() > (Y()-20)))
    {
      laserLine = true;
      break;
    }
  }
  if( laserLine )
  {
    currentMode = EVADE;
    int direction = 0;
    for(unsigned int i = 0; i < bullets.size(); i++)
    {
      direction += (bullets[i]->Y() - Y());
    }
    if(direction > 0) { goUp(); }
    else { goDown(); }
  }
  else
  {
    currentMode = NORMAL;
  }
  velocityX(enemyVelocityX);
  if( Y() < 0) {
    velocityY( abs(velocityY() ) );
  }
  if(Y() > worldHeight-frameHeight) {
    velocityY( -abs(velocityY() ) );
  }
}
