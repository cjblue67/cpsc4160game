#include <SDL.h>
#include <string>
#include <deque>

class Manager;

class Clock {
public:
  static Clock& getInstance();
  unsigned int getTicks() const;
  unsigned int getTotalTicks() const { return sumOfAllTicks; }

private:
  friend class Manager;
  static Clock& instance;
  
  
  bool started;
  bool paused;
  bool sloMo;
  const bool framesAreCapped;
  const int frameCap;

  unsigned int frames;

  std::deque<int> recFrame;
  const unsigned int maxFrames;

  unsigned int tickSum;
  unsigned int sumOfAllTicks;
  unsigned int timeAtStart;
  unsigned int timeAtPause;
  unsigned int currTicks;
  unsigned int prevTicks;
  unsigned int ticks;
  
  unsigned int getElapsedTicks();
  Clock& operator++();
  void toggleSloMo();

  bool isStarted() const { return started; }
  bool isPaused() const  { return paused;  }
  unsigned int getFrames() const  { return frames;  }
  unsigned int getSeconds() const { return getTicks()/1000;  }
  unsigned int capFrameRate() const;
  int getAverageFPS() const;
  int getFPS() const;

  void start();
  void pause();
  void unpause();
  void display() const;

  Clock();
  Clock(const Clock&);
  Clock&operator=(const Clock&);
};
