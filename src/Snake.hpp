#pragma once
#include "Field.hpp"
#include "SimpleRect.h"
#include <array>
#include <iostream>
#include <stdexcept>
using uint = unsigned int;

enum class Direction { WEST,
					   EAST,
					   NORTH,
					   SOUTH };

class Snake {
public:
  struct SnakeSegment {
	sf::Vector2u position;
	ly::SimpleRect drawable;
  };

public:
  Snake() = default;
  void move(const Field &field);
  void spawn(sf::Vector2u headPosition, uint initLength,
			 const Field &field);
  void updateBounds(const Field &field);
  void grow(const Field &field);
  void draw(sf::RenderWindow &window);
  void setColor(const sf::Color &newColor) noexcept;
  sf::Color getColor() const;
  sf::Vector2u getHeadPosition() const noexcept { return m_snakeBody[0].position; }
  sf::Vector2u getSegmentPositionAt(uint index) const
	  noexcept { return m_snakeBody[index].position; }
  constexpr Direction getDirection() const noexcept { return m_currentDirection; }
  void setDirection(Direction newDir) noexcept;
  bool isHeadInSnakebody();
  uint length();

private:
  Direction m_currentDirection{Direction::EAST};
  sf::Vector2u m_lastTailPosition;
  std::vector<SnakeSegment> m_snakeBody;
};
