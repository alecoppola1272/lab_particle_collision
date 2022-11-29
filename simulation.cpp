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

void macro()
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
    TH1F *htypes = new TH1F("types", "Abundancies of generated Particles", 7, 0, 7);
    TH2F *hangles = new TH2F("Angles", "Distribution of azimutal and polar angles", 100, 0, 2 * M_PI, 100, 0, M_PI);
    TH1F *hpav = new TH1F("pav", "Average impulse Distribution", 1000, 0, 7);
    TH1F *hImpulse = new TH1F("Impulse", "Trasverse impulse Distribution", 1000, 0, 5);
    TH1F *henergy = new TH1F("energy", "Energy Distribution", 1000, 0, 7);
    TH1F *hinvmass0 = new TH1F("inv mass0", "Invariant Mass Distribution", 1000, 0, 7);
    TH1F *hinvmass1 = new TH1F("inv mass1", "Invariant Mass Distribution (opposite charges)", 100, 0.75, 1.05);
    TH1F *hinvmass2 = new TH1F("inv mass2", "Invariant Mass Distribution (same charges)", 100, 0.75, 1.05);
    TH1F *hinvmass3 = new TH1F("inv mass3", "Invariant Mass Distribution (p+/k- or p-/k+)", 1000, 0, 7.);
    TH1F *hinvmass4 = new TH1F("inv mass4", "Invariant Mass Distribution (p+/k+ or p-/k-)", 1000, 0, 7.);
    TH1F *hinvmass_decay = new TH1F("inv mass decay", "Invariant Mass Distribution (decay)", 300, 0, 2);

    for (int i = 0; i < 10; i++)
    {
        for (int j = 1; j <= 1000; ++j)
        {

            Particle p;
            double average_p = gRandom->Exp(1.);
            double phi = gRandom->Uniform(0, 2 * M_PI);
            double theta = gRandom->Uniform(0, M_PI);
            double impulse = gRandom->Exp(1.);
            double px = impulse * sin(theta) * cos(phi);
            double py = impulse * sin(theta) * sin(phi);
            double pz = impulse * cos(theta);
            double x = gRandom->Rndm();
            double y = gRandom->Rndm();
            // try with setIndex
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
                                        hinvmass_decay->Fill(d1.InvMass(d2));
                                    }
                                    else
                                    {
                                        Particle d1{"pi-"};
                                        Particle d2{"k+"};
                                        p.Decay2body(d1, d2);
                                        ResonanceEvents.push_back(d1);
                                        ResonanceEvents.push_back(d2);
                                        hinvmass_decay->Fill(d1.InvMass(d2));
                                    }
                                }
                            }
                        }
                    }
                }
            }
            EventParticles.push_back(p);
            htypes->Fill(p.GetIndex());
            hangles->Fill(phi, theta);
            hpav->Fill(average_p);
            hImpulse->Fill(sqrt(pow(px, 2) + pow(py, 2)));
            henergy->Fill(p.GetEnergy());
        }

        int res_size = ResonanceEvents.size();

        for (int m = 0; m < res_size; ++m)
        {
            EventParticles.push_back(ResonanceEvents[m]);
        }
        int size = EventParticles.size();

        for (int j = 0; j < size - 1; j++)
        {

            if (EventParticles[j].GetIndex() == 6)
            {
                continue;
            }
            for (int k = j + 1; k < size; k++)
            {

                if (EventParticles[k].GetIndex() == 6 || j == k)
                {
                    continue;
                }

                double mass = EventParticles[j].InvMass(EventParticles[k]);
                hinvmass0->Fill(mass);
                if (EventParticles[j].GetCharge() * EventParticles[k].GetCharge() ==
                    -1)
                {
                    hinvmass1->Fill(mass);
                }
                if (EventParticles[j].GetCharge() * EventParticles[k].GetCharge() ==
                    1)
                {
                    hinvmass2->Fill(mass);
                }
                if ((EventParticles[j].GetIndex() == 0 &&
                     EventParticles[k].GetIndex() == 3) ||
                    (EventParticles[j].GetIndex() == 1 &&
                     EventParticles[k].GetIndex() == 2))
                {
                    hinvmass3->Fill(mass);
                }
                if ((EventParticles[j].GetIndex() == 0 &&
                     EventParticles[k].GetIndex() == 2) ||
                    (EventParticles[j].GetIndex() == 1 &&
                     EventParticles[k].GetIndex() == 3))
                {
                    hinvmass4->Fill(mass);
                }
            }
        }
        EventParticles.clear(); // clear vectors
        ResonanceEvents.clear();
    }
    // hinvmass_decay->Fit("gaus");
    TFile *file = new TFile("particles.root", "RECREATE");
    htypes->Write();
    hpav->Write();
    hangles->Write();
    henergy->Write();
    hImpulse->Write();
    hinvmass0->Write();
    hinvmass1->Write();
    hinvmass2->Write();
    hinvmass3->Write();
    hinvmass4->Write();
    hinvmass_decay->Write();
    file->Close();
}
