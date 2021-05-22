#include "SnakeEngine.hpp"
#include "genetic-cnn/GenCNN.hpp"
#include "genetic-cnn/Matrix.hpp"
#include "genetic-cnn/NeuralNetwork.hpp"
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <iostream>

int main(int argc, char *argv[]) {
  NeuralNetwork<double> test({4, 9, 8}, -1.0, 1.0);
  std::cout << test << std::endl;
  auto feed = test.feedForward({3, 2, 1, 3});
  GenCNN<double> ai({3, 2, 3, 5}, 10);
  SnakeEngine snakeEngine{};
  snakeEngine.setFramesLimit(60u);
  while (snakeEngine.isRunning()) {
	if (snakeEngine.isGameOver()) {
	  snakeEngine.respawnSnake();
	  ai.endNetworkRun(Random::get<double>(-100.0, 100.0),
	      				0.3, -10.0, 10.0);
	}
	snakeEngine.listenKeyInput();
	snakeEngine.endFrame();
  }
  return 0;
}
