#include <cstdint>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "SnakeEngine.hpp"

int main(int argc, char* argv[]) {
  SnakeEngine snakeEngine;
  snakeEngine.setFramesLimit(10u);
  while(snakeEngine.isRunning()) {
    if(snakeEngine.isGameOver())
      snakeEngine.respawnSnake();
    snakeEngine.listenKeyInput();
    snakeEngine.endFrame();
  }
  return 0;
}
