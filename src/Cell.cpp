#include "Cell.hpp"

Cell::Cell() {}

Cell::Cell(float x, float y, float width, float height)
  : 
    m_position(x, y),
    m_size(x, y)
  {}

  Cell::Cell(const sf::Vector2f& position, const sf::Vector2f& size) :
    m_position(position),
    m_size(size)
{}

void Cell::setPostion(float x, float y) noexcept {
  m_position = {x ,y};
}

void Cell::setSize(float width, float height) noexcept {
  m_size = {width, height};
}

void Cell::setPostion(const sf::Vector2f& position) noexcept {
  m_position = position;
}

void Cell::setSize(const sf::Vector2f& size) noexcept {
  m_size = size;
}
