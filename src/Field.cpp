#include "Field.hpp"

Field::Field(sf::Vector2f position, sf::Vector2f size,
			 sf::Vector2u cellCount) : m_cellCountX(cellCount.x), m_cellCountY(cellCount.y),
									   m_size(size), m_position(position) {
  m_cells.resize(m_cellCountX);
  for (auto &el : m_cells) {
	el.resize(m_cellCountY);
  }
  float cellWidth = static_cast<float>(size.x)
	  / static_cast<float>(m_cellCountX);
  float cellHeight = static_cast<float>(size.y)
	  / static_cast<float>(m_cellCountY);
  for (uint row = 0; row < m_cellCountX; row++) {
	for (uint column = 0; column < m_cellCountY; column++) {

	  m_cells[row][column].setSize(cellWidth, cellHeight);
	  m_cells[row][column].setPostion(position.x + cellWidth * static_cast<float>(row),
									  position.y + cellHeight * static_cast<float>(column));
	}
  }
}

void Field::updateBounds() {
  m_cells.resize(m_cellCountX);
  for (auto &el : m_cells) {
	el.resize(m_cellCountY);
  }

  float cellWidth = static_cast<float>(m_size.x)
	  / static_cast<float>(m_cellCountX);
  float cellHeight = static_cast<float>(m_size.y)
	  / static_cast<float>(m_cellCountY);

  for (uint row = 0; row < m_cellCountX; row++) {
	for (uint column = 0; column < m_cellCountY; column++) {
	  m_cells[row][column].setSize(cellWidth, cellHeight);
	  m_cells[row][column].setPostion(m_position.x + cellWidth * static_cast<float>(row),
									  m_position.y + cellHeight * static_cast<float>(column));
	}
  }
}

sf::Vector2f Field::getCellSize() const {
  return m_cells[0][0].getSize();
}

sf::Vector2f Field::getCellPositionAt(uint row, uint column) const {
  return m_cells[row][column].getPosition();
}

void Field::setPostion(const sf::Vector2f &newPosition) {
  m_position = newPosition;
  updateBounds();
}

void Field::setSize(const sf::Vector2f &newSize) {
  m_size = newSize;
  updateBounds();
}

void Field::setCellCount(const sf::Vector2u &newCellCount) {
  m_cellCountX = newCellCount.x;
  m_cellCountY = newCellCount.y;
  updateBounds();
}

sf::Vector2u Field::getCellCount() const noexcept {
  return sf::Vector2u(m_cellCountX, m_cellCountY);
}
