#include "ParticleType.h"
#include "ResonanceType.h"
#include "Particle.h"
#include <cmath>
#include "TMath.h"
#include "TRandom.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include <vector>

R__LOAD_LIBRARY(ParticleType_cpp.so);
R__LOAD_LIBRARY(ResonanceType_cpp.so);
R__LOAD_LIBRARY(Particle_cpp.so);

void macro2()
{

    Particle::AddParticleType("pi+", 0.13957, 1, 0);    // index 0 , positive pion
    Particle::AddParticleType("pi-", 0.13957, -1, 0);   // index 1
    Particle::AddParticleType("k+", 0.49367, 1, 0);     // index 2 , positive kaon
    Particle::AddParticleType("k-", 0.49367, -1, 0);    // index 3
    Particle::AddParticleType("p+", 0.93827, 1, 0);     // index 4 , positive proton
    Particle::AddParticleType("p-", 0.93827, -1, 0);    // index 5
    Particle::AddParticleType("k*", 0.89166, 0, 0.050); // index 6 , resonance

    gRandom->SetSeed();
    std::vector<Particle> EventParticles;
    std::vector<Particle> ResonanceEvents;
    TH1F *invmass5 =
        new TH1F("inv mass5", "Invariant Mass Distribution (decay)", 300, 0, 2);
    TH1F *types =
        new TH1F("types", "Abundancies of generated Particles", 7, 0, 7);
    TH2F *angles =
        new TH2F("Angles", "Distribution of azimutal and polar angles", 100, 0, 2 * M_PI, 100, 0, M_PI);
    TH1F *pav =
        new TH1F("pav", "Average impulse Distribution", 1000, 0, 7);
    TH1F *Impulse =
        new TH1F("Impulse", "Distribution of trasverse impulse", 1000, 0, 5);
    TH1F *energy = new TH1F("energy", "Distribution of energy", 1000, 0, 7);
    TH1F *invmass0 =
        new TH1F("inv mass0", "Invariant Mass Distribution", 1000, 0, 7);
    TH1F *invmass1 =
        new TH1F("inv mass1", "Invariant Mass Distribution (opposite charges)",
                 100, 0.75, 1.05);
    TH1F *invmass2 =
        new TH1F("inv mass2", "Invariant Mass Distribution (same charges)", 100,
                 0.75, 1.05);
    TH1F *invmass3 = new TH1F(
        "inv mass3", "Invariant Mass Distribution (p+/k- or p-/k+)", 1000, 0, 7.);
    TH1F *invmass4 = new TH1F(
        "inv mass4", "Invariant Mass Distribution (p+/k+ or p-/k-)", 1000, 0, 7.);

    double average_p = gRandom->Exp(1.);

    for (int i = 0; i < 1E5; i++) // 10E5
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

            if (x < 0.4)
            {
                p = Particle{"pi+", px, py, pz};
            }
            else
            {
                if (x < 0.8)
                {
                    p = Particle{"pi-", px, py, pz};
                }
                else
                {
                    if (x < 0.85)
                    {
                        p = Particle{"k+", px, py, pz};
                    }
                    else
                    {
                        if (x < 0.9)
                        {
                            p = Particle{"k-", px, py, pz};
                        }
                        else
                        {
                            if (x < 0.945)
                            {
                                p = Particle{"p+", px, py, pz};
                            }
                            else
                            {
                                if (x < 0.99)
                                {
                                    p = Particle{"p-", px, py, pz};
                                }
                                else
                                {
                                    p = Particle{"k*", px, py, pz};

                                    if (y < 0.5)
                                    {
                                        Particle d1{"pi+"};
                                        Particle d2{"k-"};
                                        p.Decay2body(d1, d2);
                                        ResonanceEvents.push_back(d1);
                                        ResonanceEvents.push_back(d2);
                                        invmass5->Fill(d1.InvMass(d2));
                                    }
                                    else
                                    {
                                        Particle d1{"pi-"};
                                        Particle d2{"k+"};
                                        p.Decay2body(d1, d2);
                                        ResonanceEvents.push_back(d1);
                                        ResonanceEvents.push_back(d2);
                                        invmass5->Fill(d1.InvMass(d2));
                                    }
                                }
                            }
                        }
                    }
                }
            }
            EventParticles.push_back(p);
            types->Fill(p.GetIndex()); // tipi di particelle generate
            angles->Fill(phi, theta);
            pav->Fill(average_p);
            Impulse->Fill(sqrt(pow(px, 2) + pow(py, 2)));
            energy->Fill(p.GetEnergy());
        }

        for (int m = 0; m < ResonanceEvents.size(); ++m)
        {
            EventParticles.push_back(ResonanceEvents[m]);
        }
        for (int i = 0; i < EventParticles.size() - 1; i++)
        {
        }
    }
    invmass5->Draw();
    Impulse->Draw();
    types->Draw();
}
