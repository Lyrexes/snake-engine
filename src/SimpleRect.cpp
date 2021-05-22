#include "SimpleRect.h"

namespace ly {
SimpleRect::SimpleRect() {
  constexpr auto positionX = 0.f;
  constexpr auto positionY = 0.f;
  constexpr auto sizeX = 100.f;
  constexpr auto sizeY = 100.f;
  updateBounds({positionX, positionY}, {sizeX, sizeY});
}

SimpleRect::SimpleRect(const sf::Vector2f &position, const sf::Vector2f &size, const sf::Color &color) {
  updateBounds(position, size);
  setColor(color);
}

void SimpleRect::setPosition(const sf::Vector2f &position) {
  updateBounds(position, m_vertices[3].position - m_vertices[0].position + m_vertices[1].position - m_vertices[0].position);
}

void SimpleRect::setColor(const sf::Color &color) {
  for (auto &vertex : m_vertices)
	vertex.color = color;
}

sf::Vector2f SimpleRect::getSize() const noexcept {
  return {m_vertices[3].position - m_vertices[0].position
		  + m_vertices[1].position - m_vertices[0].position};
}

sf::FloatRect SimpleRect::getGlobalBounds() const noexcept {
  float left = m_vertices[0].position.x;
  float top = m_vertices[0].position.y;
  float right = m_vertices[0].position.x;
  float bottom = m_vertices[0].position.y;

  for (std::size_t i = 1; i < m_vertices.size(); ++i) {
	sf::Vector2f position = m_vertices[i].position;

	// Update left and right
	if (position.x < left)
	  left = position.x;
	else if (position.x > right)
	  right = position.x;

	// Update top and bottom
	if (position.y < top)
	  top = position.y;
	else if (position.y > bottom)
	  bottom = position.y;
  }
  return sf::FloatRect(left, top, right - left, bottom - top);
}

void SimpleRect::draw(sf::RenderWindow &window) {
  window.draw(&m_vertices.front(), m_vertices.size(), sf::PrimitiveType::Quads);
}

void SimpleRect::updateBounds(const sf::Vector2f &position, const sf::Vector2f &size) {
  m_vertices[0].position = position;
  m_vertices[1].position = {position.x, position.y + size.y};
  m_vertices[2].position = {position.x + size.x, position.y + size.y};
  m_vertices[3].position = {position.x + size.x, position.y};
}
}// namespace ly
