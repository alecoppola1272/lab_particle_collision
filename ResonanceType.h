//OK
#ifndef RESONACETYPE_HPP
#define RESONACETYPE_HPP
#include "ParticleType.h"

class ResonanceType : public ParticleType
{
public:
    ResonanceType(const char *, double, int, double);
    double GetWidth() const;
    void Print() const;

private:
    const double fWidth_;
};

#endif