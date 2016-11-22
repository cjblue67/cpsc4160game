#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H
#include <string>
#include <vector>
#include "drawable.h"

class ExplodingSprite;

class TwoWayMultiSprite : public Drawable {
public:
  TwoWayMultiSprite(const std::string&);
  TwoWayMultiSprite(const TwoWayMultiSprite&);
  virtual ~TwoWayMultiSprite() { } 

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual void explode();
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }

protected:
  const std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;
  ExplodingSprite* explosion;
  TwoWayMultiSprite& operator=(const TwoWayMultiSprite&);
  void advanceFrame(Uint32 ticks);

private:
  
};
#endif
