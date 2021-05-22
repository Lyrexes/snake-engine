#pragma once
#include "Cell.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

using uint = unsigned int;

class Field {
public:
  Field(sf::Vector2f size, sf::Vector2f position,
		sf::Vector2u cellCount);
  sf::Vector2f getCellSize() const;
  sf::Vector2f getCellPositionAt(uint row,
								 uint column) const;
  sf::Vector2f getPosition() const noexcept { return m_position; }
  sf::Vector2f getSize() const noexcept { return m_size; }
  sf::Vector2u getCellCount() const noexcept;
  void setPostion(const sf::Vector2f &newPosition);
  void setSize(const sf::Vector2f &newSize);
  void setCellCount(const sf::Vector2u &newCellCount);

private:
  void updateBounds();

private:
  uint m_cellCountX;
  uint m_cellCountY;
  sf::Vector2f m_size;
  sf::Vector2f m_position;
  std::vector<std::vector<Cell>> m_cells;
};
