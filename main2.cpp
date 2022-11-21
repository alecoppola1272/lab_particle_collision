#include "ParticleType.h"
#include "ResonanceType.h"
#include "Particle.h"
#include <iostream>
#include <cmath>
#include "TMath.h"
#include "TRandom.h"
#include "TH1F.h"
#include "TFile.h"
#include <vector>

R__LOAD_LIBRARY(ParticleType_cpp.so);
R__LOAD_LIBRARY(ResonanceType_cpp.so);
R__LOAD_LIBRARY(Particle_cpp.so);

void macro2()
{
    /*
  char* ppi = new char('p');   // positive pion
  char* npi = new char('n');   // negative pion
  char* pk = new char('k');   // positive kaon
  char* nk = new char('q');   // negative kaon
  char* Pp = new char('+');   // positive proton
  char* Pn = new char('-');   // negative proton
  char* k = new char('d');  // resonance
*/
    Particle::AddParticleType("a", 0.13957, 1, 0);    // index 0
    Particle::AddParticleType("b", 0.13957, -1, 0);   // index 1
    Particle::AddParticleType("c", 0.49367, 1, 0);     // index 2
    Particle::AddParticleType("d", 0.49367, -1, 0);    // index 3
    Particle::AddParticleType("e", 0.93827, 1, 0);     // index 4
    Particle::AddParticleType("f", 0.93827, -1, 0);    // index 5
    Particle::AddParticleType("g", 0.89166, 0, 0.050); // index 6

    gRandom->SetSeed();
    std::vector<Particle> EventParticles;
    std::vector<Particle> ResonanceEvents;
   TH1F* invmass5 =
    new TH1F("inv mass5", "Invariant Mass Distribution (decay)", 1000, 0, 2);

    for (int i = 0; i < 10; i++) // 10E5
    {
        for (int j = 1; j <= 100; ++j)
        {

            Particle p;
            double phi = gRandom->Uniform(0, 2 * M_PI);
            double theta = gRandom->Uniform(0, M_PI);
            double impulse = gRandom->Exp(1.);
            double px = impulse * sin(theta) * cos(phi);
            double py = impulse * sin(theta) * sin(phi);
            double pz = impulse * cos(theta);
            double x = gRandom->Rndm();
            double y = gRandom->Rndm();

            if (x < 0.4){
                p = Particle{"a", px, py, pz};
            } else {
                if (x < 0.8) {
                    p = Particle{"b", px, py, pz};
                } else {
                    if(x<0.85){
                        p = Particle{"c",px,py,pz};
                    } else {
                        if(x<0.9) {
                            p = Particle{"d",px,py,pz};
                        } else {
                            if (x<0.945){
                                p = Particle{"e",px,py,pz};
                            } else {
                                if (x<0.99){
                                    p = Particle{"f",px,py,pz};
                                } else {
                                    p = Particle {"g",px,py,pz};
                                    if(y <0.5) {
                                        Particle d1{"a"};
                                        Particle d2{"d"};
                                        p.Decay2body(d1,d2); 
                                        ResonanceEvents.push_back(d1);
                                        ResonanceEvents.push_back(d2);
                                        // invmass5->Fill(d1.InvMass(d2));
                                    } else {
                                        Particle d1{"b"};
                                        Particle d2{"c"};
                                        p.Decay2body(d1,d2);
                                        ResonanceEvents.push_back(d1);
                                        ResonanceEvents.push_back(d2);
                                        //invmass5->Fill(d1.InvMass(d2));
                                    }
                                }
                            }
                        }
                    }
                }
            }

        }
    }
}