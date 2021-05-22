#pragma once
#include<SFML/Graphics.hpp>
#include<random>
#include"Field.hpp"
#include"SimpleRect.h"

class Apple {
public:
  Apple() = default;
  void draw(sf::RenderWindow &window);
  void updateBounds(const Field &field);
  void spawn(const Field &field);
  sf::Vector2u getPosition() const noexcept { return m_position; }
  sf::Color getColor() const noexcept;
  void setColor(const sf::Color &color);

private:
  ly::SimpleRect m_drawable;
  sf::Vector2u m_position;
};
