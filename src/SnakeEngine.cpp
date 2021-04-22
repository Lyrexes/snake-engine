#include "SnakeEngine.hpp"

SnakeEngine::SnakeEngine()
  : m_field({ c_fieldPositionX, c_fieldPositionY},
      {c_width, c_height},
      { c_cellCountWidth, c_cellCountHeight}),
  m_window(sf::VideoMode(c_width, c_height), c_title) {
  m_spawnPosition = sf::Vector2u(
      static_cast<uint>(m_field.getCellCount().x / 2),
      static_cast<uint>(m_field.getCellCount().y / 2)
      );
  m_apple.spawn(m_field);
  m_snake.spawn(m_spawnPosition, m_initSnakeLength, m_field);
}

void SnakeEngine::endFrame() {
  windowEvents();
  update();
  draw();
}

void SnakeEngine::windowEvents() {
  sf::Event event;
  while(m_window.pollEvent(event)) {
    if(event.type == sf::Event::Closed)
      m_window.close();
    if(event.type == sf::Event::Resized)
      resize(event.size.width, event.size.height);
  }
}

void SnakeEngine::update() {
  updateGameOver();
  if(m_apple.getPosition() == m_snake.getHeadPosition()) {
    m_snake.grow(m_field);
    do {
    m_apple.spawn(m_field);
    }while(isSnakeAt(m_apple.getPosition().x,
                     m_apple.getPosition().y));
  }
  if(!m_gameOver)
    m_snake.move(m_field);
}

void SnakeEngine::draw() {
  m_window.clear();
  m_apple.draw(m_window);
  m_snake.draw(m_window);
  m_window.display();
}

bool SnakeEngine::isRunning(){
  return m_window.isOpen();
}

void SnakeEngine::updateGameOver() {
  if(m_snake.isHeadInSnakebody()) {
    m_gameOver = true;
    return;
  }
  auto headPos = m_snake.getHeadPosition();
  switch(m_snake.getDirection()) {
    case Direction::EAST:
      if(headPos.x == m_field.getCellCount().x - 1)
        m_gameOver = true;
      break;

    case Direction::WEST:
      if(headPos.x == 0)
        m_gameOver = true;
      break;
    
    case Direction::SOUTH:
      if(headPos.y == m_field.getCellCount().y - 1)
        m_gameOver = true;
      break;

    case Direction::NORTH:
      if(headPos.y == 0)
        m_gameOver = true;
      break;
  }
  
}

void SnakeEngine::setFramesLimit(uint frames) noexcept {
  m_window.setFramerateLimit(frames);
}

void SnakeEngine::listenKeyInput() {
 if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
   m_snake.setDirection(Direction::NORTH);
 else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
   m_snake.setDirection(Direction::WEST);

  if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
   m_snake.setDirection(Direction::SOUTH);

  if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
   m_snake.setDirection(Direction::EAST);
}


void SnakeEngine::sendInput(Direction direction) {
  m_snake.setDirection(direction); 
}

void SnakeEngine::sendInput(char direction) {
  switch(direction) {
    case 'w':
      m_snake.setDirection(Direction::WEST);
      break;
    case 'e':
      m_snake.setDirection(Direction::EAST);
      break;
    case 's':
      m_snake.setDirection(Direction::SOUTH);
      break;
    case 'n':
      m_snake.setDirection(Direction::NORTH);
      break;
  }
}

sf::Color SnakeEngine::getAppleColor() {
  return m_apple.getColor();
}

void SnakeEngine::setCellCount(uint x, uint y) {
  m_field.setCellCount({x, y});
  m_snake.updateBounds(m_field);
  m_apple.updateBounds(m_field);
}

sf::Vector2u SnakeEngine::getCellCount() {
  return m_field.getCellCount();
}

sf::Color SnakeEngine::getSnakeColor() {
  return m_snake.getColor();
}

void SnakeEngine::setSnakeColor(const sf::Color& newColor) {
  m_snake.setColor(newColor);
}
void SnakeEngine::setAppleColor(const sf::Color& newColor) {
  m_apple.setColor(newColor);
}

void SnakeEngine::respawnSnake() {
  m_spawnPosition = sf::Vector2u(
      static_cast<uint>(m_field.getCellCount().x / 2),
      static_cast<uint>(m_field.getCellCount().y / 2)
      );
  m_apple.spawn(m_field);
  m_snake.spawn(m_spawnPosition, m_initSnakeLength, m_field);
  m_gameOver = false;
}

void SnakeEngine::resize(uint newWidth, uint  newHeight) {
  m_field.setSize({static_cast<float>(newWidth),
                   static_cast<float>(newHeight)});
  m_snake.updateBounds(m_field);
  m_apple.updateBounds(m_field);
}

bool SnakeEngine::isSnakeAt(uint x, uint y) {
  if(!isPositionInBounds(x, y))
    throw std::length_error("index out of bounds!");
  for(uint i = 0; i < m_snake.length(); i++) {
    if(m_snake.getSegmentPositionAt(i) == sf::Vector2u(x, y))
      return true;
  }
  return false;
}

bool SnakeEngine::isAppleAt(uint x, uint y) {
  if(!isPositionInBounds(x, y))
    throw std::length_error("index out of bounds!");
  return m_apple.getPosition() == sf::Vector2u(x, y);
}

sf::Vector2u SnakeEngine::getSnakePositionAt(uint index) {
  if(index < m_snake.length())
    throw std::length_error("snake index out of bounds!");
  return m_snake.getSegmentPositionAt(index);
}

sf::Vector2u SnakeEngine::getApplePosition() {
  return m_apple.getPosition();
}

bool SnakeEngine::isPositionInBounds(uint x, uint y) {
  return x < m_field.getCellCount().x &&
         y < m_field.getCellCount().y;
}
