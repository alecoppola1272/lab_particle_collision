#include "Particle.h"
#include "ParticleType.h"
#include "ResonanceType.h"
#include <iostream>
// check default width and -fpermessive

int main()
{

    std::cout << ">>> TEST PARTICLE PROGRAMM <<<" << '\n'
              << '\n';
    ParticleType n_particle{"particle", 5, -1};
    ParticleType const const_particle{"const_particle", 2, 0};
    ResonanceType res_particle{"res_particle", 4, 1, 10};

    std::cout << "->TEST PARTICLETYPE, RESONANCETYPE AND CONST PARTICLE" << '\n'
              << '\n';
    // check ParticleType and ResonanceType and number of particle
    ParticleType *p[2];
    p[0] = new ParticleType("n_article", 5, -1);
    p[1] = new ResonanceType("res_particle", 4, 1, 10);

    for (int i = 0; i < 2; i++)
    {
        if (i == 0)
        {
            std::cout << "-PARTICLE : (particle,5,-1)" << '\n';
        }
        else
        {
            std::cout << "-RESONANCE PARTICLE : (res_particle,4,1,10)" << '\n';
        }
        p[i]->Print();
    }
    std::cout << "-CONST PARTICLE : (const_particle, 2 , 0)" << '\n';
    const_particle.Print();
    // check Particle
    std::cout << "->TEST PARTICLE" << '\n'
              << '\n';

    Particle::AddParticleType("P1", 4, 1, 0);
    Particle::AddParticleType("P2", 6, 1, 2);
    Particle::AddParticleType("P3", 8, -1, 0);
    Particle::AddParticleType("P4", 8, 0, 2);
    Particle::PrintTypes();
}
