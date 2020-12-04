#include <stdio.h>
#include "TFile.h"
#include <iostream>
#include <vector>
#include "TChain.h"
#include "TTree.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "TROOT.h"
#include "TMath.h"
#include "TStyle.h"
#include "TColor.h"
#include "TLegend.h"
#include "TPad.h"
#include "TLine.h"
#include "TH1F.h"
#include "TPaveStats.h"
#include "TPad.h"
#include <TString.h>
#include <vector>


class Chebyshev {
public: 
   Chebyshev(int n, double xmin, double xmax) : 
      fA(xmin), fB(xmax),
      fT(std::vector<double>(n) )  {}

   double operator() (const double * xx, const double *p) { 
      double x = (2.*xx[0] - fA -fB)/(fB-fA);
      int order = fT.size(); 
      if (order == 1) return p[0]; 
      if (order == 2) return p[0] + x*p[1]; 
      // build the polynomials
      fT[0] = 1;
      fT[1] = x; 
      for (int i = 1; i< order; ++i) { 
         fT[i+1] =  2 *x * fT[i] - fT[i-1]; 
      }
      double sum = p[0]*fT[0]; 
      for (int i = 1; i<= order; ++i) { 
         sum += p[i] * fT[i]; 
      }
      return sum; 
   }

private: 
   double fA; 
   double fB; 
   std::vector<double> fT; // polynomial
   std::vector<double> fC; // coefficients
};

// Making a function to ease the make-up of lines
void format_line(TAttLine* line,int col,int sty){
line->SetLineWidth(5); line->SetLineColor(col);
line->SetLineStyle(sty);}

// Here we create a definition of a customised function, namely a crystalball (the “signal”) plus a parabolicfunction, the “background”


double the_crystalpar(double* vars, double* pars){
  double signal = pars[0]*ROOT::Math::crystalball_function(vars[0],pars[1],pars[2],pars[3],pars[4]);
  double background = pars[5];
  return  signal+background;
}
double the_cheb(double* vars, double* pars){
return pars[0]*ROOT::Math::crystalball_function(vars[0],pars[1],pars[2],pars[3],pars[4])+pars[5];}

double totalf(double* vars, double* pars){
  double signal = the_crystalpar(vars,pars);
  double background =   10.;
      return signal+background;
  }


void myfitter2(){
	TFile *file1 = new TFile("out.root","RECREATE");

	TChain *treeMC = new TChain("tpTree/fitter_tree");
	
	treeMC->Add("skimmedrunB_Glb.root");
	float mass,eta,pt;
	bool Mu7p5_Track2_Jpsi_TK;	
	///*****mass_ngrams*******////
	//TCanvas *c1 = new TCanvas("MC", "MC", 800, 800);

	TH1F* mass_n = new TH1F("mass_n","mass_n",100,2.5,3.5);
	TH1F* eta_n = new TH1F("eta","eta",100,-2.5,2.5);
	TH1F* pt_n = new TH1F("Pt","Pt",100,0.,100.);
	
	treeMC->SetBranchAddress("mass",&mass);
	treeMC->SetBranchAddress("eta",&eta);
	treeMC->SetBranchAddress("pt",&pt);
	
	Long64_t ne = treeMC->GetEntries();
	int np=0;
	for(int p=0; p<ne ;p++){
		//Looking at the HLT
		treeMC->GetEntry(p);
		//if (Mu7p5_Track2_Jpsi_TK==1){
		//	np++;
		mass_n->Fill(mass);
		eta_n->Fill(eta);
		pt_n->Fill(pt);
			//Looking for muons that could be tag or probe
			
				//Looking for tag

					//Looking for probe

		//}
		
		

	}

	//Make up for the canvas
	auto canvas_6_1=new TCanvas("canvas_6_1","canvas_6_1");
	gStyle->SetOptTitle(0); gStyle->SetOptStat(0);
	gStyle->SetOptFit(1111); gStyle->SetStatBorderSize(0);
	gStyle->SetStatX(.89); gStyle->SetStatY(.89);

	TF1 parabola("parabola","[0]",2.8,3.4);
	format_line(&parabola,kBlue,2);

	TF1 crystalball("crystalball","[0]*ROOT::Math::crystalball_function(x,[1],[2],[3],[4])",2.8,3.4);
	format_line(&crystalball,kRed,2);

	//Define and initialize an instance of TF1
	TF1 crystalpar("crystalpar",the_crystalpar,2.8,3.4,6);
	double a=1000.; double b=1.; double c=.03;
	double constant=15000; double alpha=100; double n=2; double sigma=0.030; double mu=3.09; 
	crystalpar.SetParameters(constant,alpha,n,sigma,mu,a);
	crystalpar.SetParNames("Constant","Alpha","N","Sigma","Mu","a");
	format_line(&crystalpar,kGreen,1);
	
	// perform fit ..
		auto fitResPtr = mass_n->Fit(&crystalpar,"SR");

	// ... and retrieve fit results
		fitResPtr->Print(); // print fit results
	

	// Set the values of the crystalball and parabola
	for (int ipar=0;ipar<5;ipar++){
		crystalball.SetParameter(ipar,crystalpar.GetParameter(ipar));
	}

	for (int ipar=0;ipar<6;ipar++){
		parabola.SetParameter(ipar,crystalpar.GetParameter(ipar+5));
	}
	
	//mass_n->GetYaxis()->SetRangeUser(0,250);
	mass_n->DrawClone();
	parabola.DrawClone("Same"); 
	crystalball.DrawClone("Same"); 
	crystalpar.DrawClone("Same");
	TLatex latex(0.5,7500,"#splitline{Signal Peak over}{background}");
	latex.DrawClone("Same");
	canvas_6_1->Draw();
	//canvas_6_1->Close();
	auto canvas_6_2=new TCanvas("canvas_6_2","canvas_6_2");
        eta_n->Draw();
	canvas_6_2->Draw();
	auto canvas_6_3=new TCanvas("canvas_6_3","canvas_6_3");
        pt_n->Draw();
	canvas_6_3->Draw();

/*

	double xmin = 2.7; double xmax = 3.0; 
	double n = 2; 
	Chebyshev * cheb = new Chebyshev(n,xmin,xmax);
	//TF1 * f1 = new TF1("f1",cheb,xmin,xmax,n+1,"Chebyshev");
	TF1 * f1 = new TF1("f1","pol4",2.8,2.97); f1->SetParameters(1,1,1,1,1);
	TF1 *fit = new TF1("fit","crystalball",2.97,3.2); fit->SetParameters(1,1,1,1,1); 
	TF1 * f2 = new TF1("f2","pol4",3.16,3.4); f2->SetParameters(1,1,1,1,1);
	//for (int i = 0; i <=4; ++i) fit->SetParameter(i,1);  
	mass_n->Draw();
	mass_n->Fit("fit","R");
	//mass_n->Fit("f1","R+");
	//mass_n->Fit("f2","R++");
	//f1->SetRange(xmin,xmax);
	fit->Draw("SAME");
	//f1->Draw("SAME");
	//f2->Draw("SAME");
	mass_n->Write();
	file1->Close();		

*/
}
void myfitter(){
  myfitter2();
}
