#ifndef TRIGO_H
#define TRIGO_H

#include <SFML2/System.hpp>
#define PI 3.1415928653

namespace Trigo
{
    float ToRad(float);
    float ToDeg(float);
    sf::Vector2f Translate(const float rad, const float dist, const sf::Vector2f& delta = sf::Vector2f(0,0));
    sf::Vector2f TranslateDeg(const float deg, const float dist, const sf::Vector2f& p = sf::Vector2f(0,0));
}

#endif // TRIGO_H
