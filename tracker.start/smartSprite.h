#ifndef SMARTSPRITE_H
#define SMARTSPRITE_H
#include <iostream>
#include <vector>
#include "drawable.h"
#include "ioManager.h"
#include "frameFactory.h"

class Sprite;
class ExplodingSprite;
class CollisionStrategy;

class SmartSprite : public Drawable {
public:
  SmartSprite(const std::string&, const Vector2f& pos, const float v, const std::vector<Sprite*> b, int wW, int wH, 
  int fW, int fH);
  void goUp();
  void goDown();
  virtual const Frame* getFrame() const {return frame; }
  virtual void explode();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual bool collidedWith(const Drawable*) const;

private:
  const Frame * frame;
  enum MODE {NORMAL, EVADE};
  IOManager& io;
  const float enemyVelocityX;
  std::vector<Sprite*> bullets;
  MODE currentMode;
  SmartSprite(const SmartSprite &);
  SmartSprite& operator=(const SmartSprite &rhs);
  ExplodingSprite *explosion;
  std::vector<CollisionStrategy*> strategies;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;
};
#endif
