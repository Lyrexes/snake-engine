#include "Apple.hpp"

void Apple::draw(sf::RenderWindow& window) {
  m_drawable.draw(window);
}

void Apple::updateBounds(const Field& field) {
  m_drawable.setPosition(
      field.getCellPositionAt(m_position.x, m_position.y));
  m_drawable.setSize(field.getCellSize());
}

void Apple::spawn(const Field& field) {
  std::random_device rd;  
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> randomX(0,
      field.getCellCount().x - 1);
  std::uniform_int_distribution<> randomY(0,
      field.getCellCount().y - 1);
  m_position = sf::Vector2u(randomX(gen), randomY(gen));
  setColor(sf::Color::Red);
  updateBounds(field);
}

sf::Color Apple::getColor() const noexcept {
  return m_drawable.getColor();
}

void Apple::setColor(const sf::Color& color) {
  m_drawable.setColor(color);
}
