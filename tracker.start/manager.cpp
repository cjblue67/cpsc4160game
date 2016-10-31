#include <iostream>
#include <string>
#include <iomanip>
#include "2wayMultiSprite.h"
#include "multisprite.h"
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"

static std::queue<unsigned int> frameTimes;

Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  stars("stars", Gamedata::getInstance().getXmlInt("stars/factor") ),
  planets("planets", Gamedata::getInstance().getXmlInt("planets/factor") ),
  deathStar("deathStar", Gamedata::getInstance().getXmlInt("deathStar/factor") ),
  viewport( Viewport::getInstance() ),
  sprites(),
  currentSprite(0),

  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") )
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  sprites.push_back( new TwoWayMultiSprite("xwing") );
  sprites.push_back( new MultiSprite("wreckage") );
  sprites.push_back( new Sprite("tiefighter") );
  viewport.setObjectToTrack(sprites[currentSprite]);
}

void Manager::draw() const {
  stars.draw();
  planets.draw();
  deathStar.draw();
  for (unsigned i = 0; i < sprites.size(); ++i) {
    sprites[i]->draw();
  }

  io.printMessageValueAt("Seconds: ", clock.getSeconds(), 10, 20);
  io.printMessageAt("Press T to switch sprites", 10, 45);
  io.printMessageValueAt("FPS: ", clock.fps(frameTimes, Gamedata::getInstance().getXmlInt("framesFPS")), 10, 70);
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

  for (unsigned int i = 0; i < sprites.size(); ++i) {
    sprites[i]->update(ticks);
  }
  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  stars.update();
  planets.update();
  deathStar.update();
  viewport.update(); // always update viewport last
  if(frameTimes.size() >=(unsigned) Gamedata::getInstance().getXmlInt("framesFPS")) { 
    frameTimes.pop(); 
  }
  frameTimes.push(clock.getTicks());
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
        if ( keystate[SDLK_s] ) {
          player.moveDown();
        }
        if ( keystate[SDLK_d] ) {
          player.moveRight();
        }
        if ( keystate[SDLK_w] ) {
          player.moveUp();
        }
        if ( keystate[SDLK_a] ) {
          player.moveLeft();
        }
        if ( keystate[SDLK_p] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
      }
    }
    draw();
    update();
  }
}
