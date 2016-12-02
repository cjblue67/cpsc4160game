#ifndef SPRITE__H
#define SPRITE__H
#include <string>
#include <vector>
#include "drawable.h"

class ExplodingSprite;
class CollisionStrategy;

class Sprite : public Drawable {
public:
  Sprite(const std::string&);
  Sprite(const std::string&, const Vector2f& pos, const Vector2f& vel);
  Sprite(const std::string&, 
         const Vector2f& pos, const Vector2f& vel, const Frame*);
  Sprite(const Sprite& s);
  virtual ~Sprite() { } 

  virtual const Frame* getFrame() const { return frame; }
  virtual void draw() const;
  virtual void explode();
  virtual bool collidedWith(const Drawable*) const;

  virtual void update(Uint32 ticks);
  virtual bool updateRemovable(Uint32 ticks);
  Sprite& operator=(const Sprite& rhs);
  
protected:
  const Frame * frame;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  int getDistance(const Sprite*) const;
  ExplodingSprite* explosion;
  std::vector<CollisionStrategy*> strategies;
};
#endif
