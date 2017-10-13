//
// Original Author:  Eric Appelt
//         Created:  Tue Nov 26, 2013
//
//

#include <memory>
#include <algorithm>
#include <cmath>
#include <TString.h>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDGetToken.h"

#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TF1.h>
#include <TTree.h>
#include "TVector3.h"
#include "TMath.h"
#include "TRandom3.h"


#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include <DataFormats/VertexReco/interface/Vertex.h>
#include <DataFormats/VertexReco/interface/VertexFwd.h>
#include <DataFormats/TrackReco/interface/Track.h>
#include <DataFormats/TrackReco/interface/TrackFwd.h>
//#include "DataFormats/RecoCandidate/interface/TrackAssociation.h"

// Particle Flow
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"
// RecoJets
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

//#include "TrackingCode/HIRun2015Ana/interface/HITrackCorrectionTreeHelper.h"

class BigFlowAnalyzer : public edm::EDAnalyzer {
   public:
      explicit BigFlowAnalyzer(const edm::ParameterSet&);
      ~BigFlowAnalyzer();

      static bool vtxSort( const reco::Vertex &  a, const reco::Vertex & b );

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      void initHistos(const edm::Service<TFileService> & fs);
      bool passesTrackCuts(const reco::Track & track, const reco::VertexCollection&);
      bool caloMatched(const reco::Track & track, const edm::Event& iEvent, unsigned it );
      double deltaEta(const TVector3 a, const TVector3 b);
      double deltaPhi(const TVector3 a, const TVector3 b);
      double getEff(const TVector3 a, int occ);
      double testFunction(const TVector3 a, const TVector3 b);
	
      // ----------member data ---------------------------


      TVector3 particle;
      TVector3 trigger; 
      TVector3 associated;
//      std::vector<TVector3> trigger;//(1,particle);

//      std::vector<TVector3> associated;//(1,particle);
      std::vector<std::vector<TVector3> > event;//(2);
      std::vector<std::vector<TVector3> > event1;//(2,trigger);
      std::vector<std::vector<std::vector<TVector3> >  > group_of_events;//(1, event);
      std::vector<std::vector<std::vector<TVector3> >  > eventsBg;
      std::vector<std::vector<std::vector<std::vector<TVector3> >  > > fullSet;//(600);


      std::map<std::string,TH3F*> trkCorr3D_;
      TH1F * vtxZ_;
      TH1D *histo_trigger_all;
      TH2D *histo_mult_;

      std::vector<TH2F*> eff2D;
      std::vector<TH2D*> signal1;
      std::vector<TH2D*> background1;

      std::vector<std::vector<TH2D*> > signal;
      std::vector<std::vector<TH2D*> > background;

       double effweight_trig = 1.0, effweight_ass=1.0;
       double fake_trig = 0.0, fake_ass=0.0;
       int ptint1, ptint2;


//      TFile *table;

      int centeffbins = 5; int centeffmax[5] = {5, 10, 30, 50, 100}; int centeffmin[5] = {0, 5,  10, 30, 50 }; 
///////

//      HITrackCorrectionTreeHelper treeHelper_;

      edm::EDGetTokenT<reco::VertexCollection> vertexSrc_;
      edm::EDGetTokenT<edm::View<reco::Track> > trackSrc_;

      edm::InputTag pfCandSrc_;

      std::vector<double> ptBins_;
      std::vector<double> etaBins_;
      std::vector<double> occBins_;

      bool doCaloMatched_;
      double reso_;
      
      std::vector<int> algoParameters_;

      bool applyVertexZCut_;
      double vertexZMax_, vertexZMin_;
      bool applyTrackCuts_;
      std::string qualityString_;
      std::string effTable_;
      double dxyErrMax_;
      double dzErrMax_;
      double ptErrMax_;
      int    nhitsMin_;
      double chi2nMax_;      
      double chi2nMax_pixel_;
      double dzErrMax_pixel_;
//      bool do2pc_;
//      double 


      bool useCentrality_;
      std::vector<double> ptTrigMax_; std::vector<double> ptTrigMin_;
      std::vector<double> ptAssMax_;  std::vector<double> ptAssMin_;
      int ptTbins_, ptNbins_, centMax_, centMin_, centEffBins_;
      std::vector<int> centEffMax_; std::vector<int> centEffMin_;
      bool doSymmetrisation_, doBackground_, doEffCorr_;

      edm::EDGetTokenT<int> centralitySrc_;

};

BigFlowAnalyzer::BigFlowAnalyzer(const edm::ParameterSet& iConfig):
//treeHelper_(),
vertexSrc_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertexSrc"))),
trackSrc_(consumes<edm::View<reco::Track> >(iConfig.getParameter<edm::InputTag>("trackSrc"))),

ptBins_(iConfig.getParameter<std::vector<double> >("ptBins")),
etaBins_(iConfig.getParameter<std::vector<double> >("etaBins")),
occBins_(iConfig.getParameter<std::vector<double> >("occBins")),
doCaloMatched_(iConfig.getParameter<bool>("doCaloMatched")),
reso_(iConfig.getParameter<double>("reso")),
algoParameters_(iConfig.getParameter<std::vector<int> >("algoParameters")),
applyVertexZCut_(iConfig.getParameter<bool>("applyVertexZCut")),
vertexZMax_(iConfig.getParameter<double>("vertexZMax")),
vertexZMin_(iConfig.getParameter<double>("vertexZMin")),
applyTrackCuts_(iConfig.getParameter<bool>("applyTrackCuts")),
qualityString_(iConfig.getParameter<std::string>("qualityString")),
effTable_(iConfig.getParameter<std::string>("effTable")),
dxyErrMax_(iConfig.getParameter<double>("dxyErrMax")),
dzErrMax_(iConfig.getParameter<double>("dzErrMax")),
ptErrMax_(iConfig.getParameter<double>("ptErrMax")),
nhitsMin_(iConfig.getParameter<int>("nhitsMin")),
chi2nMax_(iConfig.getParameter<double>("chi2nMax")),
chi2nMax_pixel_(iConfig.getParameter<double>("chi2nMax_pixel")),
dzErrMax_pixel_(iConfig.getParameter<double>("dzErrMax_pixel")),
useCentrality_(iConfig.getParameter<bool>("useCentrality")),
ptTrigMax_(iConfig.getParameter<std::vector<double> >("pt_trigmax")),
ptTrigMin_(iConfig.getParameter<std::vector<double> >("pt_trigmin")),
ptAssMax_(iConfig.getParameter<std::vector<double> >("pt_assmax")),
ptAssMin_(iConfig.getParameter<std::vector<double> >("pt_assmin")),
ptTbins_(iConfig.getParameter<int>("pt_tbins")),
ptNbins_(iConfig.getParameter<int>("pt_nbins")),
centMax_(iConfig.getParameter<int>("centmax")),
centMin_(iConfig.getParameter<int>("centmin")),
centEffBins_(iConfig.getParameter<int>("centeffbins")),
centEffMax_(iConfig.getParameter<std::vector<int> >("centeffmax")),
centEffMin_(iConfig.getParameter<std::vector<int> >("centeffmin")),
doSymmetrisation_(iConfig.getParameter<bool>("do_symmetrisation")),
doBackground_(iConfig.getParameter<bool>("do_background")),
doEffCorr_(iConfig.getParameter<bool>("doEffCorr")),
centralitySrc_(consumes<int>(iConfig.getParameter<edm::InputTag>("centralitySrc")))
{

   pfCandSrc_ = iConfig.getUntrackedParameter<edm::InputTag>("pfCandSrc");

   edm::Service<TFileService> fs;
   initHistos(fs);
   event.resize(ptTbins_+ptNbins_);
   fullSet.resize(600); 

 
}

BigFlowAnalyzer::~BigFlowAnalyzer()
{
}

void
BigFlowAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   using namespace edm;
   using namespace reco;
   using namespace std;

   edm::Handle<reco::PFCandidateCollection> pfCandidates;
   iEvent.getByLabel(pfCandSrc_, pfCandidates);
   if( !pfCandidates.isValid() ) std::cout<< "false callo matching"<<std::endl ;   
//  else std::cout<<"true callo matching"<<std::endl;



   // obtain reconstructed tracks
   Handle<edm::View<reco::Track> > tcol;
   iEvent.getByToken(trackSrc_, tcol);

   // obtain primary vertices
   Handle<reco::VertexCollection> vertex;
   iEvent.getByToken(vertexSrc_, vertex);
  
   // sort the vertcies by number of tracks in descending order
   VertexCollection vsorted = *vertex;
//   const VertexCollection *recoV = vertex.product();
  
   std::sort( vsorted.begin(), vsorted.end(), BigFlowAnalyzer::vtxSort );

   // determine centrality if set
   // note if there is no centrality information multiplicity 
   // will be used in place of the centrality
   int multiplicity =0;
   int cbin = multiplicity;
   int occ = multiplicity;
   if( useCentrality_ )
   {
     edm::Handle<int> centralityBin;
     iEvent.getByToken(centralitySrc_, centralityBin);
     cbin = *centralityBin;
     occ = cbin;
   }
   if ( occ < centMin_*2 || occ >= centMax_*2 ) return;

   // skip events with no PV, this should not happen
   if( vsorted.size() == 0) return;
   if( (int) vsorted.size() > 100 ) return;
   if( vsorted[0].tracksSize() < 1) return;

   // skip events failing vertex cut
   if( applyVertexZCut_)
   {
     if( fabs(vsorted[0].z()) > vertexZMax_ ) return;
     if( fabs(vsorted[0].z()) < vertexZMin_ ) return;
   }


   vtxZ_->Fill(vsorted[0].z());

   // determine event multipliticy
/*   for(edm::View<reco::Track>::size_type i=0; i<tcol->size(); ++i){
     edm::RefToBase<reco::Track> track(tcol, i);
     reco::Track* tr=const_cast<reco::Track*>(track.get());
//     if( !passesTrackCuts(*tr, vsorted) ) 
//       multiplicity++;
   }
*/




   int ntrack = 0;

   for (Int_t i=0; i<ptNbins_+ptTbins_; i++) event[i].resize(0);

   // ---------------------
   // loop through reco tracks to remember pT, eta and phi of particles of interest 
   // ---------------------

   for(edm::View<reco::Track>::size_type i=0; i<tcol->size(); ++i){ 
     edm::RefToBase<reco::Track> track(tcol, i);
     reco::Track* tr=const_cast<reco::Track*>(track.get());

     if ( tr->charge() == 0  ) continue;
     if ( fabs( tr->eta() ) > 2.4 ) continue; 

     // skip tracks that fail cuts, using vertex with most tracks as PV       
     if( ! passesTrackCuts(*tr, vsorted) ) continue;
     if( ! caloMatched(*tr, iEvent, i) ) continue;
     multiplicity++;
     double w =1.0;
   
     trkCorr3D_["hrec3D"]->Fill(tr->eta(), tr->pt(), occ, w);

     particle.SetPtEtaPhi(tr->pt(),tr->eta(),tr->phi());

     for (int i=0; i<ptNbins_; i++) 
       if ( tr->pt() > ptAssMin_[i] && tr->pt() < ptAssMax_[i] ) event[ptTbins_+i].push_back(particle);
    
     for (int i=0; i<ptTbins_; i++) 
       if ( tr->pt() > ptTrigMin_[i] && tr->pt() < ptTrigMax_[i] ) event[i].push_back(particle);

   }



   std::cout<<"multiplicity = "<<multiplicity<<std::endl;
   cout <<"broj triger cestica je "<<event[0].size()<<endl;
   histo_mult_->Fill(ntrack,occ);


   // ---------------------
   // Filling signal histograms
   // --------------------- 

//   int centBin;
   int Ntrig, Nass;
//   centBin = occ/5;
   double trigEff=1.0, assEff=1.0; double dEta, dPhi;

   for (int i_trigbin=0; i_trigbin<ptTbins_; i_trigbin++) {
     Ntrig = event[i_trigbin].size();     

//     for (int i_assbin=0; i_assbin<ptNbins_; i_assbin++) {
//       Nass = event[ptTbins_+i_assbin].size();
     for (int j=0; j<Ntrig; j++) {
       trigger = event[i_trigbin][j];
       if (doEffCorr_) trigEff=getEff(trigger,occ);
       histo_trigger_all->Fill(trigger.Pt(), (1.0)/trigEff);


       for (int i_assbin=0; i_assbin<ptNbins_; i_assbin++) {
         Nass = event[ptTbins_+i_assbin].size();
         cout<< j<<" "<<event[i_trigbin][j].Pt()<<endl;

//         trigger = event[i_trigbin][j];
         for (int i=0; i<Nass; i++) {
           associated = event[ptTbins_+i_assbin][i];
//           if (doEffCorr_) trigEff=getEff(trigger,occ);
           if (doEffCorr_) assEff=getEff(associated,occ);
           dEta=deltaEta(trigger,associated); dPhi=deltaPhi(trigger,associated);
           if (dEta != 0  && dPhi != 0) signal[i_trigbin][i_assbin]->Fill(dEta,dPhi,1.0/trigEff/assEff);
           if (dEta != 0  && dPhi != 0 && doSymmetrisation_){
              dEta=deltaEta(trigger,associated); dPhi=deltaPhi(associated,trigger);
              signal[i_trigbin][i_assbin]->Fill(dEta,dPhi,1.0/trigEff/assEff);       
              dEta=deltaEta(associated,trigger); dPhi=deltaPhi(associated,trigger);
              signal[i_trigbin][i_assbin]->Fill(dEta,dPhi,1.0/trigEff/assEff);
              dEta=deltaEta(associated,trigger); dPhi=deltaPhi(trigger,associated);
              signal[i_trigbin][i_assbin]->Fill(dEta,dPhi,1.0/trigEff/assEff);
           } 


         }
       }
     }
   }
   int centBin, vtxBin;
   vtxBin = (vsorted[0].z() +15)/2;
   centBin = occ/5;
   fullSet[15*centBin+vtxBin].push_back(event);
}

bool
BigFlowAnalyzer::passesTrackCuts(const reco::Track & track, const reco::VertexCollection & vertex)
{
   if ( ! applyTrackCuts_ ) return true;

   math::XYZPoint vtxPoint(0.0,0.0,0.0);
   double vzErr =0.0, vxErr=0.0, vyErr=0.0;
   int primaryvtx = 0;
   vtxPoint=vertex[primaryvtx].position();
   vzErr=vertex[primaryvtx].zError();
   vxErr=vertex[primaryvtx].xError();
   vyErr=vertex[primaryvtx].yError();

   double dxy=0.0, dz=0.0, dxysigma=0.0, dzsigma=0.0;
   dxy = track.dxy(vtxPoint);
   dz = track.dz(vtxPoint);
   dxysigma = sqrt(track.d0Error()*track.d0Error()+vxErr*vyErr);
   dzsigma = sqrt(track.dzError()*track.dzError()+vzErr*vzErr);
   
   double chi2n = track.normalizedChi2();
   double nlayers = track.hitPattern().trackerLayersWithMeasurement();
   chi2n = chi2n/nlayers;
   int nhits = track.numberOfValidHits();
   int algo  = track.originalAlgo();  

   if(fabs(track.eta()) > 2.4) return false;
   if(track.quality(reco::TrackBase::qualityByName(qualityString_)) != 1)
       return false;
   if (nhits < 7 && chi2n > chi2nMax_pixel_ ) return false;
   if (nhits < 7 && fabs(dz/dzsigma) > dzErrMax_pixel_ ) return false;
//   if(fabs(track.ptError()) / track.pt() > ptErrMax_) return false;
   if ( track.pt()<2.4 && ( nhits == 3 || nhits == 4 || nhits == 5 || nhits == 6) ) return true;
   if(fabs(dxy/dxysigma) > dxyErrMax_) return false;
   if(fabs(dz/dzsigma) > dzErrMax_) return false;
//   if(chi2n > 12 ) return false;
   if ( track.pt()<2.4 && ( nhits == 3 || nhits == 4 || nhits == 5 || nhits == 6) ) return true;
   if(fabs(track.ptError()) / track.pt() > ptErrMax_) return false;

   if(nhits < nhitsMin_ ) return false;
   int count = 0;
   for(unsigned i = 0; i < algoParameters_.size(); i++){
      if( algo == algoParameters_[i] ) count++;
   }
   if(track.pt()>2.4 && count == 0 ) return false;
   if(chi2n > chi2nMax_ ) return false;  

   return true;
}

bool 
BigFlowAnalyzer::caloMatched( const reco::Track & track, const edm::Event& iEvent, unsigned it )
{
  if( ! doCaloMatched_ ) return true;

  // obtain pf candidates
  edm::Handle<reco::PFCandidateCollection> pfCandidates;
  iEvent.getByLabel(pfCandSrc_, pfCandidates);
  if( !pfCandidates.isValid() ) return false;

  double ecalEnergy = 0.;
  double hcalEnergy = 0.;

  for( unsigned ic = 0; ic < pfCandidates->size(); ic++ ) {//calo matching loops

      const reco::PFCandidate& cand = (*pfCandidates)[ic];

      int type = cand.particleId();

      // only charged hadrons and leptons can be asscociated with a track
      if(!(type == reco::PFCandidate::h ||     //type1
      type == reco::PFCandidate::e ||     //type2
      type == reco::PFCandidate::mu      //type3
      )) continue;

      reco::TrackRef trackRef = cand.trackRef();
      if( it == trackRef.key() ) {
        // cand_index = ic;
        ecalEnergy = cand.ecalEnergy();
        hcalEnergy = cand.hcalEnergy();              
        break;
      } 
  }

  if( track.pt() < 20 || ( (hcalEnergy+ecalEnergy)/( track.pt()*TMath::CosH(track.eta() ) ) > reso_ && (hcalEnergy+ecalEnergy)/(TMath::CosH(track.eta())) > (track.pt() - 80.0) )  ) return true;
  else {
    return false;
  }
}


void
BigFlowAnalyzer::initHistos(const edm::Service<TFileService> & fs)
{




  std::vector<std::string> hNames3D = { "hrec3D" };

  for( auto name : hNames3D )
  {
     trkCorr3D_[name] = fs->make<TH3F>(name.c_str(),";#eta;p_{T};occ",
                           etaBins_.size()-1, &etaBins_[0],
                           ptBins_.size()-1, &ptBins_[0],
                           occBins_.size()-1, &occBins_[0]);
  }


  vtxZ_ = fs->make<TH1F>("vtxZ","Vertex z position",100,-30,30);


  histo_mult_ = fs->make<TH2D>("mult_1" , "mult_1" , 200 , 0 , 10000, occBins_.size()-1, &occBins_[0] );

//Efficiency tables
  eff2D.resize(centeffbins);
  TFile *table;

// /afs/cern.ch/work/m/mstojano/private/CENT_TEST/CMSSW_7_5_8_patch5/src/12010/HIN12010/crab/
  table = new TFile(effTable_.data());
  for (int i=0; i<centeffbins;i++) eff2D[i] = (TH2F*)table->Get(Form("Eff_%d_%d",centeffmin[i],centeffmax[i]));

// two-particle correlations histograms
  signal.resize(ptTbins_);
  background.resize(ptTbins_);
  for(int i=0; i<ptTbins_; i++) {
    signal[i].resize(ptNbins_);
    background[i].resize(ptNbins_);
  }      

  histo_trigger_all = fs->make<TH1D>(Form("Trigger_all") , "trigger_all" , 1000 , 0 , 100 );

  for (Int_t j=0; j<ptTbins_; j++ ) {
    for (Int_t i=0; i<ptNbins_; i++) {
    signal[j][i] = fs->make<TH2D>(Form("signal_trig_%d_%d_ass_%d_%d_cent_%d_%d",int(ptTrigMin_[j]*10), int(ptTrigMax_[j]*10), int(ptAssMin_[i]*10), int(ptAssMax_[i]*10), centMin_, centMax_) , ";#Delta#eta;#Delta#phi" , 33, -4.8 - 4.8/32.0, 4.8 + 4.8/32.0,  31, -0.5*TMath::Pi()+TMath::Pi()/32, 1.5*TMath::Pi()-TMath::Pi()/32 );
    background[j][i] = fs->make<TH2D>(Form("beckground_trig_%d_%d_ass_%d_%d_cent_%d_%d",int(ptTrigMin_[j]*10), int(ptTrigMax_[j]*10), int(ptAssMin_[i]*10), int(ptAssMax_[i]*10), centMin_, centMax_) , ";#Delta#eta;#Delta#phi" , 33, -4.8 - 4.8/32.0, 4.8 + 4.8/32.0,  31, -0.5*TMath::Pi()+TMath::Pi()/32, 1.5*TMath::Pi()-TMath::Pi()/32 );


    }
  }


}

bool
BigFlowAnalyzer::vtxSort( const reco::Vertex &  a, const reco::Vertex & b )
{
  if( a.tracksSize() != b.tracksSize() )
    return  a.tracksSize() > b.tracksSize() ? true : false ;
  else
    return  a.chi2() < b.chi2() ? true : false ;  
}

double
BigFlowAnalyzer::deltaEta(const TVector3 a, const TVector3 b)
{

return a.Eta() - b.Eta();

}


double
BigFlowAnalyzer::deltaPhi(const TVector3 a, const TVector3 b)
{

  double Dphi;

  Dphi = a.Phi() - b.Phi();
  while (Dphi < -0.5*TMath::Pi()) Dphi = Dphi + 2.0*TMath::Pi();
  while (Dphi > 1.5 *TMath::Pi()) Dphi = Dphi - 2.0*TMath::Pi();
  return Dphi;
}

double
BigFlowAnalyzer::getEff(const TVector3 a, int occ)
{

     double eff = 1.0;

     for (int i_cf=0; i_cf<centEffBins_;i_cf++) if(  occ>= centEffMin_[i_cf]*2 && occ<centEffMax_[i_cf]*2 )  {

           eff = eff2D[i_cf]->GetBinContent(
             eff2D[i_cf]->GetXaxis()->FindBin(a.Eta() ),
             eff2D[i_cf]->GetYaxis()->FindBin(a.Pt() )
           );
//           std::cout<<i_cf<<" "<<tr->eta()<<" "<<tr->pt()<<" "<<eff<<std::endl;
     }
     return eff;

}

double
BigFlowAnalyzer::testFunction(const TVector3 a, const TVector3 b)
{

return a.Eta() + b.Eta();

}
void
BigFlowAnalyzer::beginJob()
{
}

void
BigFlowAnalyzer::endJob()
{

   if (!doBackground_) return;

   int Ntrig, Nass;
   int Nbgev, Nev;// j_bgev;
   double trigEff=1.0, assEff=1.0; double dEta, dPhi;
   int occ, jev;

   for (int i_evtClass = 0; i_evtClass < 600; i_evtClass++) {
     Nev = fullSet[i_evtClass].size();
     Nbgev = 10;
     occ = i_evtClass/15 + 1;
     if ( Nev<11 ) Nbgev = Nev-1;

     for (int i_ev=0; i_ev<Nev; i_ev++) {
       event=fullSet[i_evtClass][i_ev];
       eventsBg=fullSet[i_evtClass];
       eventsBg.erase(eventsBg.begin() + i_ev);
      
       for (int i_bgev=0; i_bgev<Nbgev; i_bgev++)  {

	 jev=gRandom->Rndm()*eventsBg.size();         
         for (int i_trigbin=0; i_trigbin<ptTbins_; i_trigbin++) {
           Ntrig = event[i_trigbin].size();

           for (int i_assbin=0; i_assbin<ptNbins_; i_assbin++) {
             Nass = eventsBg[jev][ptTbins_+i_assbin].size();
             for (int j=0; j<Ntrig; j++) {
               trigger = event[i_trigbin][j];
               for (int i=0; i<Nass; i++) {
                 associated = eventsBg[jev][ptTbins_+i_assbin][i];
                 if (doEffCorr_) trigEff=getEff(trigger,occ);
                 if (doEffCorr_) assEff=getEff(associated,occ);
                 dEta=deltaEta(trigger,associated); dPhi=deltaPhi(trigger,associated);
                 if (dEta != 0  && dPhi != 0) background[i_trigbin][i_assbin]->Fill(dEta,dPhi,1.0/trigEff/assEff);
                 if (dEta != 0  && dPhi != 0 && doSymmetrisation_){
                   dEta=deltaEta(trigger,associated); dPhi=deltaPhi(associated,trigger);
                   background[i_trigbin][i_assbin]->Fill(dEta,dPhi,1.0/trigEff/assEff);
                   dEta=deltaEta(associated,trigger); dPhi=deltaPhi(associated,trigger);
                   background[i_trigbin][i_assbin]->Fill(dEta,dPhi,1.0/trigEff/assEff);
                   dEta=deltaEta(associated,trigger); dPhi=deltaPhi(trigger,associated);
                   background[i_trigbin][i_assbin]->Fill(dEta,dPhi,1.0/trigEff/assEff);
                 }
               }
             }
           }

         }

	 eventsBg.erase (eventsBg.begin()+jev); 
       }
     }


   }

}

DEFINE_FWK_MODULE(BigFlowAnalyzer);
