#pragma once
#include "ScrollWindow.h"

sf::Sprite backGround;

ScrollWindow::ScrollWindow()
{

}

ScrollWindow::ScrollWindow(sf::FloatRect real, sf::FloatRect mind)
{
    // Set bounds //
    control = real;
    mindPos.x = mind.left, mindPos.y = mind.top;
    mindSize.x = mind.width, mindSize.y = mind.height;
    mainField.create(control.width, control.height); // Exc
                                                     // Set variables //
    speed = sf::Vector2f(0, 0);
}

ScrollWindow::~ScrollWindow()
{

}

void ScrollWindow::update()
{
    // Update coords //
    mindPos.x += speed.x;
    mindPos.y += speed.y;
    if (mindPos.x < 0) mindPos.x = 0;
    if (mindPos.y < 0) mindPos.y = 0;
    if (mindPos.x + control.width >= mindSize.x)
        mindPos.x = mindSize.x - control.width;
    if (mindPos.y + control.height >= mindSize.y)
        mindPos.y = mindSize.y - control.height;
    // Update speed //
    if (isPressed)
        speed.x *= deaccl.x, speed.y *= deaccl.y;
    else speed.x *= deaccl_f.x, speed.y *= deaccl_f.y;
}

void ScrollWindow::moveWindow(float dx, float dy)
{
    speed.x += dx * accl.x;
    speed.y += dy * accl.y;
}

void ScrollWindow::openDrawing()
{
    mainField.clear(sf::Color::Transparent);
    mainField.setSmooth(false);
}

bool ScrollWindow::checkPressed(float x, float y)
{
    if (!isPressed && control.contains(x, y))
    {
        isPressed = true;
        return true;
    }
    return false;
}

bool ScrollWindow::checkMove(float dx, float dy)
{
    if (isPressed)
    {
        moveWindow(dx, dy);
        return true;
    }
    return false;
}

void ScrollWindow::draw(PxButton * btn, int dx, int dy)
{
    if (btn->backGround)
    {
        btn->backGround->setPosition(btn->control.left - mindPos.x, btn->control.top - mindPos.y);
        mainField.draw(*(btn->backGround));
    }
    sf::FloatRect txtsz = btn->mainText.getLocalBounds();
    float tx = btn->control.left + btn->control.width * USS / 2 - mindPos.x;
    float ty = btn->control.top + btn->control.height * USS / 2 - mindPos.y;
    btn->mainText.setPosition(tx + dx, ty + dy);
    mainField.draw(btn->mainText);

}

void ScrollWindow::draw(sf::FloatRect rect, sf::Color c)
{
    sf::RectangleShape bck;
    bck.setFillColor(sf::Color(0, 0, 0, 30));
    bck.setPosition(rect.left - mindPos.x, rect.top - mindPos.y);
    bck.setSize(sf::Vector2f(rect.width * USS, rect.height * USS));
    mainField.draw(bck);
}

void ScrollWindow::draw(sf::Sprite * spr)
{
    sf::Vector2f buf = spr->getPosition();
    spr->setPosition(buf.x - mindPos.x, buf.y - mindPos.y);
    mainField.draw(*spr);
    spr->setPosition(buf);
}

void ScrollWindow::draw(TextBox * txb, int16_t delta_h)
{
    sf::FloatRect buf = txb->bounds;
    txb->resultBox.setPosition(buf.left - mindPos.x, buf.top - mindPos.y);
    mainField.draw(txb->resultBox);
    txb->resultBox.setPosition(buf.left, buf.top);
    if (txb->isCursorShowed)
    {
        sf::RectangleShape cursorShape(sf::Vector2f(2, txb->charH * 1.2));
        cursorShape.setPosition(txb->cursorPos.x - 1 - mindPos.x, txb->cursorPos.y + delta_h - mindPos.y);
        cursorShape.setFillColor(txb->resultBox.getFillColor());
        mainField.draw(cursorShape);
    }
}

void ScrollWindow::uniteDrawing(sf::RenderWindow * window)
{
    mainField.display();
    sf::Sprite realSprite(mainField.getTexture());
    realSprite.setPosition(control.left, control.top);
    window->draw(realSprite);
}