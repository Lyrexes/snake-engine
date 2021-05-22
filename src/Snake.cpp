#include "Snake.hpp"

void Snake::move(const Field &field) {
  sf::Vector2i add;
  sf::Vector2u tmp(m_snakeBody.front().position);
  switch (m_currentDirection) {
  case Direction::EAST:
	add = {1, 0};
	break;
  case Direction::WEST:
	add = {-1, 0};
	break;
  case Direction::NORTH:
	add = {0, -1};
	break;
  case Direction::SOUTH:
	add = {0, 1};
	break;
  }
  m_snakeBody.front().position.x += add.x;
  m_snakeBody.front().position.y += add.y;
  m_snakeBody.front().drawable.setPosition(
	  field.getCellPositionAt(
		  m_snakeBody.front().position.x,
		  m_snakeBody.front().position.y));
  for (std::size_t i = 1; i < m_snakeBody.size(); i++) {
	std::swap(tmp.x, m_snakeBody[i].position.x);
	std::swap(tmp.y, m_snakeBody[i].position.y);
	m_snakeBody[i].drawable.setPosition(
		field.getCellPositionAt(
			m_snakeBody[i].position.x,
			m_snakeBody[i].position.y));
  }
  m_lastTailPosition = tmp;
}

void Snake::spawn(sf::Vector2u headPosition, uint initLength,
				  const Field &field) {
  m_snakeBody.resize(initLength);
  if (initLength > headPosition.x)
	throw std::length_error("invalid init snake pos!");
  for (int i = 0; i < m_snakeBody.size(); i++) {
	m_snakeBody[i].position = {headPosition.x - i,
							   headPosition.y};
	m_snakeBody[i].drawable.setSize(
		field.getCellSize());
	m_snakeBody[i].drawable.setPosition(
		field.getCellPositionAt(
			headPosition.x - i,
			headPosition.y));
  }
  m_currentDirection = Direction::EAST;
  setColor(sf::Color::Green);
}

void Snake::updateBounds(const Field &field) {
  for (auto &rect : m_snakeBody) {
	rect.drawable.setSize(
		field.getCellSize());
	rect.drawable.setPosition(
		field.getCellPositionAt(rect.position.x,
								rect.position.y));
  }
}

void Snake::draw(sf::RenderWindow &window) {
  for (auto &el : m_snakeBody) {
	el.drawable.draw(window);
  }
}

void Snake::setDirection(Direction newDir) noexcept {
  m_currentDirection = newDir;
}

void Snake::setColor(const sf::Color &color) noexcept {
  for (auto &el : m_snakeBody)
	el.drawable.setColor(color);
}

sf::Color Snake::getColor() const {
  return m_snakeBody.front().drawable.getColor();
}

void Snake::grow(const Field &field) {
  uint negPos = static_cast<uint>(-1);
  if (m_lastTailPosition == sf::Vector2u(negPos, negPos))
	throw std::logic_error("move has to be called before grow!");
  m_snakeBody.resize(m_snakeBody.size() + 1);
  m_snakeBody.back().position = m_lastTailPosition;
  m_snakeBody.back().drawable.setSize(field.getCellSize());
  m_snakeBody.back().drawable.setColor(getColor());
  m_snakeBody.back().drawable.setPosition(
	  field.getCellPositionAt(
		  m_lastTailPosition.x,
		  m_lastTailPosition.y));
  m_lastTailPosition = {negPos, negPos};
}

bool Snake::isHeadInSnakebody() {
  auto headPosition = m_snakeBody[0].position;
  for (uint i = 1; i < m_snakeBody.size(); i++) {
	if (headPosition == m_snakeBody[i].position)
	  return true;
  }
  return false;
}

uint Snake::length() {
  return m_snakeBody.size();
}
