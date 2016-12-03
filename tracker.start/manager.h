#include <vector>
#include <SDL.h>
#include <queue>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "hud.h"
#include "sound.h"
#include "sprite.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void switchSprite();

private:
  const bool env;
  const IOManager& io;
  Clock& clock;
  Player& player;
  SDLSound& sound;

  SDL_Surface * const screen;
  World stars;
  //World planets;
  World deathStar;
  Viewport& viewport;

  std::vector<Drawable*> sprites;
  std::vector<Sprite*> bullets;
  std::vector<bool> friends;
  int currentSprite;

  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;
  Hud hud;
  

  void draw() const;
  void update();

  bool checkForCollisions() const;
  Drawable* shot() const;

  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
  bool god;
  void reset();
};
  
