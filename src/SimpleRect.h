#pragma once
#include "SFML/Graphics.hpp"
#include <array>

namespace ly {
class SimpleRect {
public:
  SimpleRect();
  SimpleRect(const sf::Vector2f &position, const sf::Vector2f &size, const sf::Color &color = sf::Color());
  void setSize(float x, float y) { setSize({x, y}); }
  void setSize(const sf::Vector2f &size) { updateBounds(m_vertices.front().position, size); };
  void setPosition(float x, float y) { setPosition({x, y}); }
  void setPosition(const sf::Vector2f &position);
  void move(float x, float y) { move({x, y}); }
  void move(const sf::Vector2f &offset) { setPosition(m_vertices.front().position + offset); }
  void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha = 255) { setColor({r, g, b, alpha}); }
  void setColor(const sf::Color &color);
  sf::Vector2f getSize() const noexcept;
  sf::Vector2f getPosition() const noexcept { return m_vertices.front().position; };
  sf::FloatRect getGlobalBounds() const noexcept;
  sf::Color getColor() const noexcept { return m_vertices.front().color; };
  void draw(sf::RenderWindow &window);

private:
  void updateBounds(const sf::Vector2f &position, const sf::Vector2f &size);

private:
  std::array<sf::Vertex, 4> m_vertices;
};
}// namespace ly
