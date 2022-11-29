#include "TFile.h"
#include "TH1.h"
#include "TString.h"
#include "TF1.h"
#include "TH2F.h"
#include "TCanvas.h"

#include <fstream>

void data()
{
    TFile *file = new TFile("particles.root");
    TH1 *htot[11];
    TString s[11] = {"types", "Angles", "pav", "Impulse",
                     "energy", "inv mass0", "inv mass1", "inv mass2",
                     "inv mass3", "inv mass4", "inv mass decay"};
    TString XLabels[7] = {"pi+", "pi-", "k+", "k-", "p+", "p-", "k*"};

TString names[7] = {"pi+","pi-","k+","k-","p+","p-","k*"};
    for (int i = 0; i < 11; i++)
    {
        htot[i] = (TH1 *)file->Get(s[i]);
    }

    for (int i = 1; i <= 7; i++)
    {
        htot[0]->GetXaxis()->SetBinLabel(i, XLabels[i - 1]);
    }

    htot[0]->SetStats(kFALSE);
    htot[0]->SetLabelSize(0.07);

    TF1 *f1 = new TF1{"f1", "[0]", 0, M_PI};
    TF1 *f2 = new TF1{"f2", "[0]", 0, 2 * M_PI};

    TH1D *AngleX = ((TH2F *)file->Get(s[1]))->ProjectionX("AngleX", 0, 100);
    AngleX->SetTitle("Azimuthal Angle Distribution");
    TH1D *AngleY = ((TH2F *)file->Get(s[1]))->ProjectionY("AngleY", 0, 100);
    AngleY->SetTitle("Polar Angle Distribution");

    AngleX->Fit("f2", "Q0");
    AngleY->Fit("f1", "Q0");

    TF1 *f3 = new TF1("f3", "[0]*e^(-[1]*x)", 0, 7);
    htot[2]->Fit("f3", "Q0");

    TH1F *SumCharges = new TH1F(*(TH1F *)file->Get(s[6]));
    TH1F *SumParticles = new TH1F(*(TH1F *)file->Get(s[8]));

    SumCharges->Add(htot[7], htot[6], 1, -1);
    SumCharges->SetTitle("Opposite charges minus Same charges");

    SumParticles->Add(htot[9], htot[8], 1, -1);
    SumParticles->SetTitle("p+/k- and p-/k+ minus p+/k+ and p-/k-");

    TF1 *f4 = new TF1("f4", "gaus", 0, 7);
    TF1 *f5 = new TF1("f5", "gaus", 0, 7);
    SumCharges->Fit("f4", "Q0");
    SumParticles->Fit("f5", "Q0");

    SumParticles->GetXaxis()->SetRangeUser(0.6, 1.2);

    TF1 *f6 = new TF1("f6", "gaus", 0.6, 1.2);
    TF1 *ftot[6] = {f2, f1, f3, f6, f4, f5};
    TH1 *HDraw[7] = {htot[0], AngleX, AngleY, htot[2],
                     htot[10], SumCharges, SumParticles};
    htot[10]->Fit("f6", "Q0");

    std::ofstream txt("./HistoData.txt", std::ofstream::out);
    if (txt.is_open())
    {
        txt << "=============================================" << '\n';
        txt << "|| ANALYZING GENERATED PARTICLE HISTOGRAMS ||" << '\n';
        txt << "=============================================" << '\n';
        txt << '\n';
        txt << '\n';
        txt << "->HISTOS ENTRIES: " << '\n';
        for (int i = 0; i < 11; i++)
        {
            txt << htot[i]->GetTitle() << " has " << htot[i]->GetEntries() << " entries. " << '\n';
        }
        txt << '\n';
        txt << '\n';
        txt << "->NUMBER OF OCCURRENCIES FOR EACH PARTICLE:" << '\n';
        
        for (int i = 0; i < 7; i++)
        {
            txt <<names[i] << " was generated " <<htot[0]->GetBinContent(i+1) <<" times"<<'\n';
        }        
    }

TCanvas *c1 = new TCanvas("c1", "MyCanvas1", 200, 10, 500, 800);
TCanvas *c2 = new TCanvas("c2", "MyCanvas2", 200, 10, 650, 800);
c1->Print("ParticlesHistos.pdf[");
c2->Print("InvMass.pdf[");
c2->Divide(1,3);
c2->Divide(1,4);
c1->cd();
for(int j = 0;j<4;++j){
c1->cd(j+1);
}
}

