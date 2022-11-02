//OK
#include "ResonanceType.h"
#include <iostream>

ResonanceType::ResonanceType(const char *fName, double fMass, int fCharge, double fWidth) : ParticleType(fName, fMass, fCharge), fWidth_(fWidth){}
double ResonanceType::GetWidth() const { return fWidth_; }
void ResonanceType::Print() const
{

    std::cout << "Particle Name :" << GetName() << '\n';
    std::cout << "Particle Mass :" << GetMass() << '\n';
    std::cout << "Particle Charge :" << GetCharge() << '\n';
    std::cout << "Particle Width :" << GetWidth() << '\n'
              << '\n';
}