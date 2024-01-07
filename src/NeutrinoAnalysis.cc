#include <iostream>
#include <assert.h>

#include "NeutrinoAnalysis.hh"

#include "G4Gamma.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4Electron.hh"
#include "G4EmCalculator.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalPhoton.hh"
#include "G4LossTableManager.hh"
#include "G4PhysicalConstants.hh"
#include "CLHEP/Units/PhysicalConstants.h"

#include "TAxis.h"
#include "TGraph.h"

NeutrinoAnalysis* NeutrinoAnalysis::instance = NeutrinoAnalysis::Instance();

NeutrinoAnalysis::NeutrinoAnalysis(const std::string& name)
    : fileName("sim.root")
{
    analysisMessenger = new NeutrinoAnalysisMessenger();
}

NeutrinoAnalysis::~NeutrinoAnalysis()
{
}

void
NeutrinoAnalysis::BeginOfRunAction(const G4Run* aRun)
{
    m_file = new TFile(fileName, "recreate");
    
    m_evt_tree = new TTree("evt", "evt");
    m_evt_tree->Branch("evtID",   &m_evtID,   "evtID/I");
    m_evt_tree->Branch("initE",   &m_initE,   "initE/D");
    
    // m_step_tree = new TTree("step", "step");
    // m_step_tree->Branch("stepNum", &m_stepNum, "stepNum/I");
    // m_step_tree->Branch("E",       &m_E);
    // m_step_tree->Branch("T",       &m_T);
    // m_step_tree->Branch("dE",      &m_dE);
    // m_step_tree->Branch("dx",      &m_dx);
    // m_step_tree->Branch("dEdx",    &m_dEdx);

    m_op_tree = new TTree("optical", "optical");
    m_op_tree->Branch("totalOP",    &m_totalOP);
    m_op_tree->Branch("CerenOP",    &m_CerenOP);
    m_op_tree->Branch("ScintOP",    &m_ScintOP);
    // m_op_tree->Branch("CerenWL",    &m_CerenWL);
    // m_op_tree->Branch("ScintWL",    &m_ScintWL);


    //m_pe_tree = new TTree("photoelectron", "photoelectron");
    //m_pe_tree->Branch("totalPE", &m_totalPE);
    //m_pe_tree->Branch("PEenergy", &m_PEenergy);
    //m_pe_tree->Branch("PEtime", &m_PEtime);

    m_pe_juno_tree = new TTree("junoPE", "junoPE");
    m_pe_juno_tree->Branch("totalPE",       &m_totalPE_junoPMT);
    m_pe_juno_tree->Branch("PEwavelength",  &m_PEwavelength_junoPMT);
    m_pe_juno_tree->Branch("OPwavelength",  &m_OPwavelength_junoPMT);
    m_pe_juno_tree->Branch("PEtime",        &m_PEtime_junoPMT);
    m_pe_juno_tree->Branch("PERefNum",      &m_PERefNum_junoPMT);
    m_pe_juno_tree->Branch("PERayNum",      &m_PERayNum_junoPMT);
    m_pe_juno_tree->Branch("PEScintNum",    &m_PEScintNum_junoPMT);
    m_pe_juno_tree->Branch("OPFlag",        &m_OPFlag_junoPMT);
    //m_pe_juno_tree->Branch("PERefFlag",     &m_PERefFlag_junoPMT);
    //m_pe_juno_tree->Branch("PERayFlag",     &m_PERayFlag_junoPMT);
    //m_pe_juno_tree->Branch("PEScintFlag",   &m_PEScintFlag_junoPMT);
    m_pe_juno_tree->Branch("PECheFlag",     &m_PECheFlag_junoPMT);



    /*
    if(fPrimary){
        G4double stepSize = 0.1*MeV;
        G4double maxKinEnergy = 1024.0*MeV;
        
        G4Material* material = G4Material::GetMaterial("LS");
        G4ParticleDefinition* particle = fPrimary->GetParticleDefinition();

        G4EmCalculator emCalculator;

        TGraph *g_dEdx_E = new TGraph();
        TGraph *g_dEdx_v = new TGraph();
        for(int i=0;i<int(maxKinEnergy/stepSize);i++){
            G4double E = (i+1) * stepSize;
            G4double m = particle->GetPDGMass();
            G4double T = E/m;
            G4double v = c_light*std::sqrt(T*(T+2.))/(T+1.0);

            G4double dEdx = emCalculator.GetDEDX(E, particle, material);
            g_dEdx_E->AddPoint(E/MeV, dEdx/(MeV/mm));
            g_dEdx_v->AddPoint(v/c_light, dEdx/(MeV/mm));
        }

        g_dEdx_E->GetXaxis()->SetTitle("Kinetic Energy [MeV]");
        g_dEdx_E->GetYaxis()->SetTitle("dE/dx [MeV/mm]");
        g_dEdx_E->SetNameTitle("g_dEdx_E");
        g_dEdx_E->Write();

        g_dEdx_v->GetXaxis()->SetTitle("Beta");
        g_dEdx_v->GetYaxis()->SetTitle("dE/dx [MeV/mm]");
        g_dEdx_v->SetNameTitle("g_dEdx_v");
        g_dEdx_v->Write();
    }
    */
}

void
NeutrinoAnalysis::EndOfRunAction(const G4Run* aRun)
{
    m_evt_tree->Write();
    m_op_tree->Write();
    //m_pe_tree->Write();
    m_pe_juno_tree->Write();
    // m_step_tree->Write();

    m_file->Close();
}

void
NeutrinoAnalysis::BeginOfEventAction(const G4Event* anEvent)
{   
    G4cout<<"Begin of "<<anEvent->GetEventID()<<"th event..."<<G4endl;

    m_evtID = anEvent->GetEventID();
    m_initE = 0.;
    m_stepNum = 0;

    m_totalOP = 0;
    m_CerenOP = 0;
    m_ScintOP = 0;

    m_CerenWL.clear();
    m_ScintWL.clear();

    //m_totalPE = 0;
    //m_PEenergy.clear();
    //m_PEtime.clear();

    m_totalPE_junoPMT = 0;
    m_PEwavelength_junoPMT.clear();
    m_OPwavelength_junoPMT.clear();
    m_PEtime_junoPMT.clear();
    m_PERefNum_junoPMT.clear();
    m_PERayNum_junoPMT.clear();
    m_PEScintNum_junoPMT.clear();
    m_OPFlag_junoPMT.clear();
    //m_PERefFlag_junoPMT.clear();
    //m_PERayFlag_junoPMT.clear();
    m_PECheFlag_junoPMT.clear();
    //m_PEScintFlag_junoPMT.clear();

    m_dE.clear();
    m_dx.clear();
}

void
NeutrinoAnalysis::EndOfEventAction(const G4Event* anEvent)
{
    m_evt_tree->Fill();
    m_op_tree->Fill();
    //m_pe_tree->Fill();
    m_pe_juno_tree->Fill();
    // m_step_tree->Fill();
}

void
NeutrinoAnalysis::PreUserTrackingAction(const G4Track* aTrack)
{
    if(aTrack->GetParentID() == 0){
        m_initE = aTrack->GetVertexKineticEnergy();
    }

    //if(aTrack->GetParticleDefinition() == G4OpticalPhoton::Definition()){
    if(aTrack->GetParticleDefinition() == G4OpticalPhoton::Definition() and aTrack->GetParentID() == 1){
        // not exactly correct method, as there is possibility for generating secondary charged particles by the primary charged pariticle.
        m_totalOP++;
        if (aTrack->GetTrackID() == 1) {
            //G4cout << "This is an initial photon from the generator." << G4endl;
            assert(0);
        }
        else {
            if(aTrack->GetCreatorProcess()->GetProcessName() == "Scintillation" or aTrack->GetCreatorProcess()->GetProcessName() == "OpAbsReemit"){
            //if(aTrack->GetCreatorProcess()->GetProcessName() == "Scintillation"){
                m_ScintOP++;
                // m_ScintWL.push_back(aTrack->GetTotalEnergy()/eV);
            }else if(aTrack->GetCreatorProcess()->GetProcessName() == "Cerenkov"){
                m_CerenOP++;
                // m_CerenWL.push_back(aTrack->GetTotalEnergy()/eV);
            }else{
                G4cout<<"Unkown process ?"<<aTrack->GetCreatorProcess()->GetProcessName()<<G4endl;
                assert(0);
            }
        }
    }
}

void
NeutrinoAnalysis::PostUserTrackingAction(const G4Track* aTrack)
{
    if(aTrack->GetParentID() == 0){
        m_stepNum = aTrack->GetCurrentStepNumber();
    }
}

void
NeutrinoAnalysis::UserSteppingAction(const G4Step* aStep)
{
    //G4Track* aTrack = aStep->GetTrack();

    //if(aTrack->GetParticleDefinition() == G4OpticalPhoton::Definition())
    //{
    //    aTrack->SetTrackStatus(fStopAndKill);
    //}

    //G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
    //G4StepPoint* postStepPoint = aStep->GetPostStepPoint();

    ///*
    //if(aStep->GetTrack()->GetParentID() == 0){
    //    double E  = preStepPoint->GetKineticEnergy();
    //    double T  = postStepPoint->GetGlobalTime();
    //    double dE = aStep->GetTotalEnergyDeposit();
    //    double dx = aStep->GetStepLength();
    //    
    //    m_T     .push_back(T);
    //    m_E     .push_back(E);
    //    m_dE    .push_back(dE);
    //    m_dx    .push_back(dx);
    //    m_dEdx  .push_back(dE/dx);
    //}
    //*/
}

void 
NeutrinoAnalysis::analyseEventID(G4int evtid)
{}


void NeutrinoAnalysis::analyseTotalPENumber(G4int number)
{
    m_totalPE = number;
}

void NeutrinoAnalysis::analysePEenergy(G4double energy)
{
    m_PEenergy.push_back(energy);
}

void NeutrinoAnalysis::analysePEtime(G4double time)
{
    m_PEtime.push_back(time);
}



void 
NeutrinoAnalysis::analyseEventID_junoPMT(G4int evtid)
{}


void NeutrinoAnalysis::analyseTotalPENumber_junoPMT(G4int number)
{
    m_totalPE_junoPMT = number;
}

void NeutrinoAnalysis::analysePEwavelength_junoPMT(G4double energy)
{
    m_PEwavelength_junoPMT.push_back(energy);
}

void NeutrinoAnalysis::analyseOPwavelength_junoPMT(G4double energy)
{
    m_OPwavelength_junoPMT.push_back(energy);
}


void NeutrinoAnalysis::analysePEtime_junoPMT(G4double time)
{
    m_PEtime_junoPMT.push_back(time);
}

void NeutrinoAnalysis::analysePERefNum_junoPMT(G4int num) {
    m_PERefNum_junoPMT.push_back(num);
}

void NeutrinoAnalysis::analysePERayNum_junoPMT(G4int num) {
    m_PERayNum_junoPMT.push_back(num);
}

void NeutrinoAnalysis::analysePEScintNum_junoPMT(G4int num) {
    m_PEScintNum_junoPMT.push_back(num);
}

void NeutrinoAnalysis::analyseIsReemission(G4int val) {
    m_PEScintFlag_junoPMT.push_back(val);
}

void NeutrinoAnalysis::analyseIsCerenkov(G4int val) {
    m_PECheFlag_junoPMT.push_back(val);
}

void NeutrinoAnalysis::analyseIsRayleigh(G4int val) {
    m_PERayFlag_junoPMT.push_back(val);
}

void NeutrinoAnalysis::analyseIsReflection(G4int val) {
    m_PERefFlag_junoPMT.push_back(val);
}

void NeutrinoAnalysis::analyseIsOP(G4int val) {
    m_OPFlag_junoPMT.push_back(val);
}

