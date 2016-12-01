#ifndef TWOWAYSCALEDSPRITE__H
#define TWOWAYSCALEDSPRITE__H
#include <string>
#include <vector>
#include <iostream>
#include "drawable.h"
class ExplodingSprite;
class CollisionStrategy;

class TwoWayScaledSprite : public Drawable{
public:
  TwoWayScaledSprite(const std::string& name, 
                           SDL_Surface* surface);
  TwoWayScaledSprite(const TwoWayScaledSprite&);
  virtual ~TwoWayScaledSprite() { } 

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }

  bool operator<(const TwoWayScaledSprite& rhs) const {
    return scale < rhs.scale;
  }

  unsigned getPixel(Uint32, Uint32) const;
  float getScale() const { return scale; }

  Vector2f getCenter() const { 
    return Vector2f( X()+getFrame()->getWidth()/2, Y()+getFrame()->getHeight()/2 );
  }
  double scale;
  virtual bool collidedWith(const Drawable* d) const;
  virtual void explode();
  
private:
  
  SDL_Surface* scaledSurface;
  const std::vector<Frame *> frames;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  void advanceFrame(Uint32 ticks);
  ExplodingSprite* explosion;
  std::vector<CollisionStrategy*> strategies;
  
  TwoWayScaledSprite& operator=(const TwoWayScaledSprite&);
};
#endif
