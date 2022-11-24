#ifndef PARTICLE_HPP
#define PARTICLE_HPP
#include "ParticleType.h"
#include "ResonanceType.h"

class Particle
{
public:
    Particle(const char *, double, double, double);
    Particle();
    Particle(char*);
    static void AddParticleType(const char *, double, int, double);
    void SetIndex(int );
    void SetIndex(const char *);
    static void PrintTypes();
    void Print();
    double GetPx() const;
    double GetPy() const;
    double GetPz() const;
    double GetMass() const;
    double GetEnergy() const;
    int GetCharge() const;
    int GetIndex(); //const?
    void SetP(double, double, double);
    double InvMass(Particle &);
    int Decay2body(Particle &dau1, Particle &dau2) const;

private:
    static const int fMaxNumParticleType = 10;
    static ParticleType *fParticleType[fMaxNumParticleType];
    static int fNParticleType;
    int fIndex;
    double fPx_ = 0;
    double fPy_ = 0;
    double fPz_ = 0;
    static int FindParticle(const char *ParticleName);
    void Boost(double bx, double by, double bz);
};

#endif
