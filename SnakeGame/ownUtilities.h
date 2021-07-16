#pragma once
#include <SFML\Graphics.hpp>
class ownUtilities
{
public:
    // Constants //
    const sf::String fontFamily = "fonts/Bungee-Regular.ttf";
    const sf::String textFontFamily = "fonts/AGENCYB.ttf";
    const float USS = 2;
    // Fields //
    // Methods //
    sf::Sprite * createBackground(sf::FloatRect box, sf::String texturePath, bool scaled = true);
    ownUtilities();
    ~ownUtilities();
};

