#include "Noise.h"
#include "PerlinNoise.h"
#include <algorithm>

Noise::Noise() :
    octaves(0),
    init(false)
{
}

Noise::Noise(int _octaves,
             const Vector3f &color1, 
             const Vector3f &color2,
             float freq,
             float amp) :
    octaves(_octaves),
    frequency(freq),
    amplitude(amp)
{
    color[0] = color1;
    color[1] = color2;
    init = true;
}

Vector3f
Noise::getColor(const Vector3f &pos)
{
    // IMPLEMENT: Fill in this function ONLY.
    // Interpolate between two colors by weighted average
  double perlin = perlinOctaveNoise(pos,octaves);
  float m = std::sin(frequency*pos.x()+amplitude*perlin)/2+0.5f;
  float m_clamped = std::min(1.0f,std::max(m,0.0f));

  return (float)m*color[0]+(1-(float)m)*color[1];
}

bool
Noise::isValid()
{
    return init;
}
