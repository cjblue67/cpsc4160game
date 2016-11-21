#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <vector>
#include "scaledSprite.h"
#include "2wayMultiSprite.h"
#include "multisprite.h"
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"
#include "twoWayScaledSprite.h"

static std::queue<unsigned int> frameTimes;

class ScaledSpriteCompare {
public:
  bool operator()(const TwoWayScaledSprite* lhs, const TwoWayScaledSprite* rhs)
  {
    return lhs->getScale() < rhs->getScale();
  }
};

Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }
  for(unsigned i = 0; i < bullets.size(); ++i) {
  	delete bullets[i];
  }
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  player( Player::getInstance()),
  screen( io.getScreen() ),
  stars("stars", Gamedata::getInstance().getXmlInt("stars/factor") ),
  //planets("planets", Gamedata::getInstance().getXmlInt("planets/factor") ),
  deathStar("deathStar", Gamedata::getInstance().getXmlInt("deathStar/factor") ),
  viewport( Viewport::getInstance() ),
  sprites(),
  bullets(),
  currentSprite(0),

  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ) ,
  hud(screen),
  enemiesDestroyed(0)
{
  player.setLives(Gamedata::getInstance().getXmlInt("playerLives"));
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  SDL_Surface * const tieSurface = io.loadAndSet(Gamedata::getInstance().getXmlStr("tiefighter/file"),
                Gamedata::getInstance().getXmlBool("tiefighter/transparency"));
  sprites.push_back( player.getPlaySprite() );
  sprites.push_back( new MultiSprite("wreckage") );
  std::vector<TwoWayScaledSprite*> ties; 
  for(int i = 0; i < Gamedata::getInstance().getXmlInt("numberOfShips"); i++)
  {
    ties.push_back(new TwoWayScaledSprite("tiefighter", tieSurface) );
  }
  sort(ties.begin(), ties.end(), ScaledSpriteCompare());
  for(int i = 0; i < Gamedata::getInstance().getXmlInt("numberOfShips"); i++)
  {
    sprites.push_back(ties[i]);
  }
  bullets.reserve(10);
  viewport.setObjectToTrack(sprites[currentSprite]);
}

void Manager::draw() const {
  stars.draw();
  //planets.draw();
  deathStar.draw();
  for (unsigned i = 0; i < sprites.size(); ++i) {
    sprites[i]->draw();
  }
  for (unsigned i = 0; i < bullets.size(); ++i) {
    bullets[i]->draw();
  }
  
  if(hud.get_display()) {
  	hud.draw(clock.getSeconds(), clock.getAverageFPS(), player.getLives(), 0);
  }
  
  io.printMessageAt(title, 10, 450);
  viewport.draw();

  SDL_Flip(screen);
}

void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::switchSprite() {
  currentSprite = (currentSprite+1) % sprites.size();
  viewport.setObjectToTrack(sprites[currentSprite]);
}

void Manager::update() {
  ++(clock);
  Uint32 ticks = clock.getElapsedTicks();

  static unsigned int lastSeconds = clock.getSeconds();
  if ( clock.getSeconds() - lastSeconds == 5 ) {
    lastSeconds = clock.getSeconds();
    //switchSprite();
  }
  if(clock.getSeconds() == 2) {
  	hud.set_false();
  }
  
  for (unsigned int i = 0; i < sprites.size(); ++i) {
    sprites[i]->update(ticks);
  }
  for (unsigned int i = 0; i < bullets.size(); ++i) {
    bool deleteBullet = bullets[i]->updateRemovable(ticks);
    if(deleteBullet) {
      delete bullets[i];
      bullets.erase(bullets.begin() + i);
    }
  }
  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  stars.update();
  //planets.update();
  deathStar.update();
  viewport.update(); // always update viewport last
  if(frameTimes.size() >=(unsigned) Gamedata::getInstance().getXmlInt("framesFPS")) { 
    frameTimes.pop(); 
  }
  frameTimes.push(clock.getTicks());
}

void Manager::reset() {
  enemiesDestroyed = 0;
  player.setLives(Gamedata::getInstance().getXmlInt("playerLives"));
}

void Manager::play() {
  SDL_Event event;
  bool done = false;

  while ( not done ) {
    while ( SDL_PollEvent(&event) ) {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          break;
        }
        if ( keystate[SDLK_s] && keystate[SDLK_w]) {
          player.stopYVelocity();
        }
        else if ( keystate[SDLK_w] ) {
          player.moveUp();
        }
        else if ( keystate[SDLK_s] ) {
          player.moveDown();
        }
        if ( keystate[SDLK_a] && keystate[SDLK_d]) {
          player.stopXVelocity();
        }
        else if ( keystate[SDLK_d] ) {
          player.moveRight();
        }
        else if ( keystate[SDLK_a] ) {
          player.moveLeft();
        }
        if ( keystate[SDLK_p] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if( keystate[SDLK_F1] ) {
        	hud.change_display();
        }
        if ( keystate[SDLK_r] ) {
          reset();
        }
        if( keystate[SDLK_SPACE] ) {
          // only allow 10 bullets out at once
          // if the ship is going right
          if(bullets.size() < 10) { 
            if(player.getPlaySprite()->velocityX() >= 0) {
              bullets.push_back( new Sprite(string("laser"), Vector2f(player.getPlaySprite()->X(), player.getPlaySprite()->Y()), Vector2f(200.0,0.0) ) );
            }
            else {
              bullets.push_back( new Sprite(string("laser"), Vector2f(player.getPlaySprite()->X(), player.getPlaySprite()->Y()), Vector2f(-200.0,0.0) ) );
            }
          }
        }
        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
      }
      if(event.type == SDL_KEYUP)
      {
        if( !keystate[SDLK_a] && !keystate[SDLK_d])
        {
          player.stopXVelocity();
        }
        if( !keystate[SDLK_w] && !keystate[SDLK_s])
        {
          player.stopYVelocity();
        }

      }
    }
    draw();
    update();
  }
}
