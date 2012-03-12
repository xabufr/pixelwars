#include "trigo.h"

namespace Trigo
{

    float ToRad(float deg)
    {
        return deg*PI/180;
    }

    float ToDeg(float rad)
    {
        return rad*180/PI;
    }

    sf::Vector2f Translate(const float rad, const float dist, const sf::Vector2f& p)
    {
        return sf::Vector2f(p.x + dist* std::cos(rad), p.y - dist * std::sin(rad));
    }
    sf::Vector2f TranslateDeg(const float deg, const float dist, const sf::Vector2f& p)
    {
        return Translate(ToRad(deg), dist, p);
    }
}
