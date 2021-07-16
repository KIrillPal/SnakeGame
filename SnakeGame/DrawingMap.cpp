#include "DrawingMap.h"
#include <queue>
#include <cstring>
#include <algorithm>
sf::RenderWindow window;

void DrawingMap::clearPixel(int x, int y)
{
    backMap[y][x] = 0;
    frontMap.setPixel(x, y, (x + y) % 2 ? backB : backW);
}

void DrawingMap::snakePixel(int x, int y, int i)
{
    backMap[y][x] = i + 1;
    frontMap.setPixel(x, y, snakes[i]->color);
}

void DrawingMap::moveSnake(int i, int dx, int dy)
{
    int x = snakes[i]->hp.first + dx, y = snakes[i]->hp.second + dy;
    snakes[i]->from = snakes[i]->dir;
    if (x < 0 || x >= size || y < 0 || y >= size)
        killSnake(i);
    else if (backMap[y][x] == APPL || backMap[y][x] == 0)
    {
        if (snakes[i]->adlen == 0)
        {
            auto tail = snakes[i]->path.front();
            clearPixel(tail.first, tail.second);
            snakes[i]->path.pop();
        }
        else --snakes[i]->adlen;
        if (backMap[y][x] == APPL)
        {
            ++snakes[i]->adlen;
            addApple();
        }
        snakePixel(x, y, i);
        snakes[i]->hp = { x, y };
        snakes[i]->path.push({x, y});
    }
    else killSnake(i);
}

void DrawingMap::killSnake(int i)
{
    snakes[i]->status = -1;
    snakes[i]->color.r *= KK, snakes[i]->color.g *= KK;
    snakes[i]->color.b *= KK;
    while (snakes[i]->path.size())
    {
        auto tail = snakes[i]->path.front();
        backMap[tail.second][tail.first] = 11 + i;
        frontMap.setPixel(tail.first, tail.second, snakes[i]->color);
        snakes[i]->path.pop();
    }
}

DrawingMap::DrawingMap(uint16_t width, uint16_t height, const char* programName)
{
    window.create(sf::VideoMode(width, height), programName);
    currentWidth = width;
    currentHeight = height;
    memset(backMap, 0, sizeof(backMap));
}

sf::RenderWindow* DrawingMap::getWindow()
{
    return &window;
}

void DrawingMap::setGame(size_t n, std::vector<Snake*>& snks)
{
    snakes = snks;
    size = n, count = snakes.size();
    memset(backMap, 0, sizeof(backMap));
    frontMap.create(n, n);
    for (int y = 0; y < n; ++y)
        for (int x = 0; x < n; ++x)
            clearPixel(x, y);
    for (int i = 0; i < count; ++i)
    {
        while (snakes[i]->path.size())
            snakes[i]->path.pop();
        int x_head = std::min((int)n / 3 - 1, 7);
        int y = (i + 1) * n / (count + 1);
        snakes[i]->hp = { x_head, y };
        for (int k = -2; k <= 0; ++k)
        {
            snakes[i]->path.push({ x_head + k, y});
            snakePixel(x_head + k, y, i);
        }
    }
}

void DrawingMap::forward()
{
    for (int i = 0; i <count * SHUFFLE_NUM; ++i)
        std::swap(snakes[rand() % count], snakes[rand() % count]);
    for (int i = 0; i < count; ++i)
    {
        if (snakes[i]->status == -1) continue;
        if (snakes[i]->ai_mode == 0)
            moveSnake(i, snakes[i]->dir.first, snakes[i]->dir.second);
        else
        {
            int x = snakes[i]->hp.first, y = snakes[i]->hp.second;
            auto res = snake_bfs(x, y, APPL);
            if (res.first == -100)
            {
                auto tail = snakes[i]->path.front();
                backMap[tail.second][tail.first] = TAIL;
                res = snake_bfs(x, y, TAIL);
                if (res.first == -100)
                {
                    backMap[tail.second][tail.first] = i + 1;
                    if (x > 0 && backMap[y][x - 1] == 0) moveSnake(i, -1, 0);
                    else if (y > 0 && backMap[y - 1][x] == 0) moveSnake(i, 0, -1);
                    else if (x + 1 < size && backMap[y][x + 1] == 0) moveSnake(i, 1, 0);
                    else if (y + 1 < size && backMap[y + 1][x] == 0) moveSnake(i, 0, 1);
                    else killSnake(i);
                }
                else moveSnake(i, res.first, res.second);
            }
            else moveSnake(i, res.first, res.second);
        }
    }
}

void DrawingMap::addApple()
{
    int x, y;
    do x = rand() % size, y = rand() % size;
    while (backMap[y][x] != 0);
    backMap[y][x] = APPL;
    frontMap.setPixel(x, y, appleCol);
}

void DrawingMap::checkKey(char key)
{
    for (int i = 0; i < snakes.size(); ++i)
    {
        if (snakes[i]->ai_mode) continue;
        if (snakes[i]->W == key + 'A' && snakes[i]->from.second != 1)
            snakes[i]->dir = { 0, -1 };
        else if (snakes[i]->A == key + 'A' && snakes[i]->from.first != 1)
            snakes[i]->dir = { -1, 0 };
        else if (snakes[i]->S == key + 'A' && snakes[i]->from.second != -1)
            snakes[i]->dir = { 0, 1 };
        else if (snakes[i]->D == key + 'A' && snakes[i]->from.first != -1)
            snakes[i]->dir = { 1, 0 };
    }
}

std::pair<int, int> DrawingMap::snake_bfs(int sx, int sy, int p)
{
    memset(bfsMap, 0, sizeof(bfsMap));
    std::queue<std::pair<std::pair<int, int>, int> > points;
    points.push({ { sx, sy }, 1 });
    std::pair<int, int> res = { -100, -100 };
    int nk = 0;
    while (points.size())
    {
        int x = points.front().first.first;
        int y = points.front().first.second;
        int k = points.front().second;
        points.pop();
        if (bfsMap[y][x]) continue;
        bfsMap[y][x] = k;
        if (backMap[y][x] == p) { res = { x, y }, nk = k; break; }
        if (backMap[y][x] && (x != sx || y != sy)) { bfsMap[y][x] = -1; continue; }
        if (x > 0 && bfsMap[y][x - 1] == 0)
            points.push({ { x - 1, y }, k + 1 });
        if (y > 0 && bfsMap[y - 1][x] == 0)
            points.push({ { x, y - 1 }, k + 1 });
        if (x + 1 < size && bfsMap[y][x + 1] == 0)
            points.push({ { x + 1, y }, k + 1 });
        if (y + 1 < size && bfsMap[y + 1][x] == 0)
            points.push({ { x, y + 1 }, k + 1 });
    }
    if (res.first == -100) return res;
    std::pair<int, int> lst = { -1, -1 };
    int x = res.first, y = res.second;
    while (x != sx || y != sy)
    {
        lst = { x, y }, --nk;
        if (x > 0 && bfsMap[y][x - 1] == nk) --x;
        else if (y > 0 && bfsMap[y - 1][x] == nk) --y;
        else if (x + 1 < size && bfsMap[y][x + 1] == nk) ++x;
        else if (y + 1 < size && bfsMap[y + 1][x] == nk) ++y;
    }
    return { lst.first - x, lst.second - y };
}

sf::Sprite * DrawingMap::drawGame(size_t x, size_t y, size_t width)
{
    sf::Texture *back = new sf::Texture();
    back->loadFromImage(frontMap);
    back->setSmooth(false);
    mainGame = sf::Sprite(*back);
    mainGame.setPosition(x, y);
    mainGame.setScale(width * 1.f / size, width * 1.f / size);
    return &mainGame;
}

DrawingMap::~DrawingMap()
{
}