//OK
#include "ParticleType.h"
#include <iostream>

ParticleType::ParticleType(const char *fName, double fMass, int fCharge) : fName_(fName), fMass_(fMass), fCharge_(fCharge){}
const char *ParticleType::GetName() const { return fName_; }
double ParticleType::GetMass() const { return fMass_; }
int ParticleType::GetCharge() const { return fCharge_; }
double ParticleType::GetWidth() const {return 0;}
void ParticleType::Print() const
{

    std::cout << "Particle Name :" << GetName() << '\n';
    std::cout << "Particle Mass :" << GetMass() << '\n';
    std::cout << "Particle Charge :" << GetCharge() << '\n'
              << '\n';
}
