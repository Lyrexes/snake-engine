#pragma once
#include<SFML/Graphics.hpp>
#include<boost/python.hpp>
#include"Field.hpp"
#include"Snake.hpp"
#include"Apple.hpp"

class SnakeEngine {
  public:
    SnakeEngine();
    void endFrame();
    bool isRunning();
    void sendInput(Direction direction);
    void sendInput(char direction); 
    void listenKeyInput();
    void setSnakeColor(uint r, uint g, uint b, uint a=255);
    void setAppleColor(uint r, uint g, uint b, uint a=255);
    void setFramesLimit(uint framesPerSecond) noexcept;
    sf::Color getAppleColor();
    void setCellCount(uint x, uint y);
    sf::Vector2u getCellCount();
    sf::Color getSnakeColor();
    constexpr bool isGameOver() const noexcept
    { return m_gameOver;}
    void respawnSnake();
    bool isSnakeAt(uint x, uint y);
    bool isAppleAt(uint x, uint y);
    sf::Vector2u getSnakePositionAt(uint index);
    sf::Vector2u getApplePosition();

  private:
    void windowEvents();
    void resize(uint newWidth, uint  newHeight);
    void draw();
    void update();
    void updateGameOver();
    bool isPositionInBounds(uint x, uint y);

  private:
    bool m_gameOver = false;
    uint m_initSnakeLength = 5u;
    sf::Vector2u m_spawnPosition;
    Apple m_apple;
    Field m_field;
    Snake m_snake;
    sf::RenderWindow m_window;

  private:
    constexpr static const char* c_title    = "Snake Engine";
    constexpr static uint c_cellCountWidth  = 30;
    constexpr static uint c_cellCountHeight = 30;
    constexpr static uint c_width           = 800;
    constexpr static uint c_height          = 800;
    constexpr static uint c_fieldPositionX  = 0;
    constexpr static uint c_fieldPositionY  = 0;
    constexpr static float c_delay          = 0.f;
};


