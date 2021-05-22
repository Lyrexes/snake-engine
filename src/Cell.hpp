#pragma once
#include <SFML/Graphics.hpp>

class Cell {
public:
  Cell();
  Cell(float x, float y, float width, float height);
  Cell(const sf::Vector2f &position, const sf::Vector2f &size);
  sf::Vector2f getSize() const noexcept { return m_size; }
  sf::Vector2f getPosition() const noexcept { return m_position; }
  void setPostion(float x, float y) noexcept;
  void setSize(float width, float height) noexcept;
  void setPostion(const sf::Vector2f &position) noexcept;
  void setSize(const sf::Vector2f &size) noexcept;

private:
  sf::Vector2f m_position;
  sf::Vector2f m_size;
};
