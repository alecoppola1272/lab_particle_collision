#include "Particle.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

int Particle::fNParticleType = 0;
ParticleType *Particle::fParticleType[Particle::fMaxNumParticleType];
Particle::Particle()
{
    fPx_ = 0;
    fPy_ = 0;
    fPz_ = 0;
    fIndex = 0;
}
Particle::Particle(const char *ParticleName, double fPx, double fPy, double fPz)
{
    fPx_ = fPx;
    fPy_ = fPy;
    fPz_ = fPz;
    int index = FindParticle(ParticleName);
    if (index != -1)
    {
        fIndex = index;
    }
    else
    {
        std::cout << "ParticleType  < " << ParticleName << " > is not present  " << '\n';
        fIndex = -1;
    }
}

double Particle::GetPx() const
{
    return fPx_;
}
double Particle::GetPy() const
{
    return fPy_;
}
double Particle::GetPz() const
{
    return fPz_;
}

int Particle::FindParticle(const char *ParticleName)
{
    int index=0;
    for (int i = 0; i < fNParticleType; i++)
    {

        if (ParticleName == fParticleType[i]->GetName())
        {
            index=i;
            break;
        }
        else
        {
            std::cout << "No matches for " << ParticleName << " founded" << '\n';
            index= -1;
            break;
        }
    }
    return index;
}

void Particle::AddParticleType(const char *ParticleName, double ParticleMass, int ParticleCharge, double ParticleWidth)
{
    if ((fNParticleType < fMaxNumParticleType))
    {
        int index = FindParticle(ParticleName);
        // std::cout << "VERIFING index : " << index << '\n';

        if (index != -1)
        {

            std::cout << "Particle already present" << '\n';
        }
        if (ParticleWidth == 0)
        {
            fParticleType[fNParticleType] = new ParticleType(ParticleName, ParticleMass, ParticleCharge);
        }
        else
        {
            fParticleType[fNParticleType] = new ResonanceType(ParticleName, ParticleMass, ParticleCharge, ParticleWidth);
        }
        fNParticleType++;
    }
    else
    {

        std::cout << "Maximum number of particle reached" << '\n';
    }
}
// check SetIndex with name
void Particle::SetIndex(const char *ParticleName)
{
    int index = FindParticle(ParticleName);
    if (index != -1)
    {
        std::cout << "Particle already present" << '\n';
    }
    else
    {
        fIndex = index;
    }
}
// check if
void Particle::SetIndex(int index)
{
    if (fParticleType[index] != nullptr)
    {
        std::cout << "Particle already present" << '\n';
    }
    else
    {
        fIndex = index;
    }
}

int Particle::GetIndex()
{
    return fIndex;
}

int Particle::GetCharge() const
{
    return fParticleType[fIndex]->GetCharge();
}

void Particle::PrintTypes()
{
    for (int i = 0; i < fNParticleType; ++i)
    {
        fParticleType[i]->Print();
    }
}

void Particle::Print()
{
    std::cout << "Particle Type Index " << fIndex << '\n';
    std::cout << "Particle Name " << fParticleType[fIndex]->GetName() << '\n';
    double Px = GetPx();
    std::cout << "Particle Px component " << Px << '\n';
    double Py = GetPy();
    std::cout << "Particle Px component " << Py << '\n';
    double Pz = GetPz();
    std::cout << "Particle Px component " << Pz << '\n';
}

double Particle::GetMass() const
{
    return fParticleType[fIndex]->GetMass();
}

double Particle::GetEnergy() const
{
    double mass = GetMass();
    double px = GetPx();
    double py = GetPy();
    double pz = GetPz();
    double p = sqrt(px * px + py * py + pz * pz);
    double energy = sqrt(mass * mass + p * p);
    return energy;
}

void Particle::SetP(double Px, double Py, double Pz)
{
    fPx_ = Px;
    fPy_ = Py;
    fPz_ = Pz;
}

double Particle::InvMass(Particle &particle)
{
    double energy = particle.GetEnergy() + GetEnergy();
    double px = (particle.GetPx() + GetPx());
    double py = (particle.GetPy() + GetPy());
    double pz = (particle.GetPz() + GetPz());

    double massInv = sqrt((energy * energy) - (px * px + py * py + pz * pz));
    return massInv;
}

int Particle::Decay2body(Particle &dau1, Particle &dau2) const
{
    if (GetMass() == 0.0)
    {
        printf("Decayment cannot be preformed if mass is zero\n");
        return 1;
    }

    double massMot = GetMass();
    double massDau1 = dau1.GetMass();
    double massDau2 = dau2.GetMass();

    if (fIndex > -1)
    { // add width effect

        // gaussian random numbers

        float x1, x2, w, y1;

        double invnum = 1. / RAND_MAX;
        do
        {
            x1 = 2.0 * rand() * invnum - 1.0;
            x2 = 2.0 * rand() * invnum - 1.0;
            w = x1 * x1 + x2 * x2;
        } while (w >= 1.0);

        w = sqrt((-2.0 * log(w)) / w);
        y1 = x1 * w;
        // y2 = x2 * w;

        massMot += fParticleType[fIndex]->GetWidth() * y1;
    }

    if (massMot < massDau1 + massDau2)
    {
        printf("Decayment cannot be preformed because mass is too low in this channel\n");
        return 2;
    }

    double pout = sqrt((massMot * massMot - (massDau1 + massDau2) * (massDau1 + massDau2)) * (massMot * massMot - (massDau1 - massDau2) * (massDau1 - massDau2))) / massMot * 0.5;

    double norm = 2 * M_PI / RAND_MAX;

    double phi = rand() * norm;
    double theta = rand() * norm * 0.5 - M_PI / 2.;
    dau1.SetP(pout * sin(theta) * cos(phi), pout * sin(theta) * sin(phi), pout * cos(theta));
    dau2.SetP(-pout * sin(theta) * cos(phi), -pout * sin(theta) * sin(phi), -pout * cos(theta));

    double energy = sqrt(fPx_ * fPx_ + fPy_ * fPy_ + fPz_ * fPz_ + massMot * massMot);

    double bx = fPx_ / energy;
    double by = fPy_ / energy;
    double bz = fPz_ / energy;

    dau1.Boost(bx, by, bz);
    dau2.Boost(bx, by, bz);

    return 0;
}
void Particle::Boost(double bx, double by, double bz)
{

    double energy = GetEnergy();

    // Boost this Lorentz vector
    double b2 = bx * bx + by * by + bz * bz;
    double gamma = 1.0 / sqrt(1.0 - b2);
    double bp = bx * fPx_ + by * fPy_ + bz * fPz_;
    double gamma2 = b2 > 0 ? (gamma - 1.0) / b2 : 0.0;

    fPx_ += gamma2 * bp * bx + gamma * bx * energy;
    fPy_ += gamma2 * bp * by + gamma * by * energy;
    fPz_ += gamma2 * bp * bz + gamma * bz * energy;
}
