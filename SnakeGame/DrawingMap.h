#pragma once
#include <SFML\Graphics.hpp>
#include <queue>
#include <vector>
class DrawingMap
{
private:
    const int SHUFFLE_NUM = 3, APPL = 505, TAIL = 404;
    const float KK = 0.3;
    int backMap[100][100], bfsMap[100][100], size = 10, count = 0;
    sf::Color backW = sf::Color(180, 180, 180), backB = sf::Color(160, 160, 160), appleCol = sf::Color(255, 200, 0);
    void clearPixel(int x, int y);
    void snakePixel(int x, int y, int i);
    void moveSnake(int i, int dx, int dy);
    void killSnake(int i);
public:

    DrawingMap(uint16_t width, uint16_t height, const char* programName);
    ~DrawingMap();
    struct Snake
    {
        sf::String name;
        sf::Color color;
        int ai_mode = 0, size = 3, status = 0, adlen = 0;
        std::pair<int, int> dir, hp, from;
        char W = 'W', A = 'A', S = 'S', D = 'D';
        std::queue<std::pair<int, int> > path;
        Snake() {}
    };
    sf::RenderWindow* getWindow();
    sf::RenderWindow window;
    sf::Image frontMap;
    sf::Sprite mainGame;
    std::vector<Snake*> snakes;
    void setGame(size_t n, std::vector<Snake*> &snks);
    void forward();
    void addApple();
    void checkKey(char key);
    std::pair<int, int> snake_bfs(int sx, int sy, int p);

    sf::Sprite * drawGame(size_t x, size_t y, size_t w);
private:
    uint16_t currentWidth, currentHeight;
};