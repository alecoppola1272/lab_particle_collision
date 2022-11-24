/*
#include "ParticleType.h"
#include "ResonanceType.h"
#include "Particle.h"
#include <iostream>
#include <cmath>
#include "TMath.h"
#include "TRandom.h"
#include "TH1F.h"
#include "TFile.h"

R__LOAD_LIBRARY(ParticleType_cpp.so);
R__LOAD_LIBRARY(ResonanceType_cpp.so);
R__LOAD_LIBRARY(Particle_cpp.so);

void macro()
{

    Particle::AddParticleType("pi+", 0.13957, 1, 0);    // index 0
    Particle::AddParticleType("pi-", 0.13957, -1, 0);   // index 1
    Particle::AddParticleType("K+", 0.49367, 1, 0);     // index 2
    Particle::AddParticleType("K-", 0.49367, -1, 0);    // index 3
    Particle::AddParticleType("P+", 0.93827, 1, 0);     // index 4
    Particle::AddParticleType("P-", 0.93827, -1, 0);    // index 5
    Particle::AddParticleType("K*", 0.89166, 0, 0.050); // index 6

    gRandom->SetSeed();

    TH1F *hType = new TH1F("hType", "Particle Types Distribution", 7, 0, 120);

    TH1F *hPhi = new TH1F("hPhi", "Azimuth Angle Distribution", 100, 0., 2 * TMath::Pi());
    TH1F *hTheta = new TH1F("hTheta", "Polar Angle Distribution", 100, 0., TMath::Pi());

    TH1F *hImpulse = new TH1F("hImpulse", "Impulse Distribution", 100, 0, 1000);
    TH1F *hEnergy = new TH1F("hEnergy", "Energy Distribution", 100, 0, 1000);

    TH1F *hInvConc = new TH1F("hInvConc", "Invarian Mass Distribution Concordant Sign", 120, 0, 1000);
    hInvConc->Sumw2();
    TH1F *hInvDisc = new TH1F("hInvDisc", "Invarian Mass Distribution Discordant Sign", 120, 0, 1000);
    hInvDisc->Sumw2();
    TH1F *hInvConcPK = new TH1F("hInvConcPK", "Invarian Mass Distribution Concordant Sign Decay", 120, 0, 1000);
    hInvConcPK->Sumw2();
    TH1F *hInvDiscPK = new TH1F("hInvDiscPK", "Invarian Mass Distribution Discordant Sign Decay", 120, 0, 1000);
    hInvDiscPK->Sumw2();
    TH1F *hInvK = new TH1F("hInvK", "Invarian Mass Distribution K* Decay", 120, 0, 1000);

    double phi = 0;
    double theta = 0;
    double impulse = 0;
    double px = 0;
    double py = 0;
    double pz = 0;
    double massInv = 0;

    Particle EventParticles[130];

    for (int i = 0; i < 1E5; i++)
    {
        int k = 100;

        for (int j = 0; j < 100; j++)
        {
            Particle p;

            phi = gRandom->Rndm() * 2 * TMath::Pi();
            theta = gRandom->Rndm() * TMath::Pi();
            impulse = gRandom->Exp(1);

            px = impulse * sin(theta) * cos(phi);
            py = impulse * sin(theta) * sin(phi);
            pz = impulse * cos(theta);
            p.SetP(px, py, pz);

            double x = 0;
            x = gRandom->Rndm();

            if (x < 0.4)
            {
                p.SetIndex("pi+");
            }
            else if (x < 0.8)
            {
                p.SetIndex("pi-");
            }
            else if (x < 0.85)
            {
                p.SetIndex("K+");
            }
            else if (x < 0.9)
            {
                p.SetIndex("K-");
            }
            else if (x < 0.945)
            {
                p.SetIndex("P+");
            }
            else if (x < 0.99)
            {
                p.SetIndex("P-");
            }
//errore qui, commentando sia l'else sia da eventparticle[j] a henergy funziona.

            else
            {
                
                p.SetIndex("K*");
                
                double t = gRandom->Rndm();
                if (t < 0.5)
                {
                    EventParticles[k].SetIndex("pi+");
                    EventParticles[k + 1].SetIndex("K-");
                }
                else
                {
                    EventParticles[k].SetIndex("pi-");
                    EventParticles[k + 1].SetIndex("K+");
                }
                /*
                
                p.Decay2body(EventParticles[k], EventParticles[k + 1]);
                massInv = EventParticles[k].InvMass(EventParticles[k + 1]);
                hInvK->Fill(massInv);
                
                k += 2;
                
                */
               /*
            }
            hType->Fill(p.GetIndex());
            hTheta->Fill(theta);
            hPhi->Fill(phi);
            hImpulse->Fill(impulse);
/*
            double energy = p.GetEnergy();
            hEnergy->Fill(energy);
            EventParticles[j] = p;
                
*/
/*
        }
        

        for (int n = 0; n < k; n++)
        {
            for (int m = n+1; m < k; m++)
            {
                if(EventParticles[n].GetIndex() !=6 && EventParticles[m].GetIndex() != 6){
                if (EventParticles[n].GetCharge() * EventParticles[m].GetCharge() == 1)
                {

                    massInv = EventParticles[n].InvMass(EventParticles[m]);
                    hInvConc->Fill(massInv);
                }
                if (EventParticles[n].GetCharge() * EventParticles[m].GetCharge() == -1)
                {
                    massInv = EventParticles[n].InvMass(EventParticles[m]);
                    hInvDisc->Fill(massInv);
                }
                if ((EventParticles[n].GetIndex() == 0 && EventParticles[m].GetIndex() == 2) || (EventParticles[n].GetIndex() == 1 && EventParticles[m].GetIndex() == 3))
                {
                    massInv = EventParticles[n].InvMass(EventParticles[m]);
                    hInvConcPK->Fill(massInv);
                }
                if ((EventParticles[n].GetIndex() == 0 && EventParticles[m].GetIndex() == 3) || (EventParticles[n].GetIndex() == 1 && EventParticles[m].GetIndex() == 2))
                {
                    massInv = EventParticles[n].InvMass(EventParticles[m]);
                    hInvDiscPK->Fill(massInv);
                }
                else
                {
                    std::cout << "No Match" << '\n';
                }
                }
            }
        }
    }
        

    TFile *file = new TFile("particle.root", "RECREATE");
    file->Write();
    file->Close();
}
/*
