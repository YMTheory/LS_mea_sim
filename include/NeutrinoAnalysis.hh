#ifndef NeutrinoAnalysis_h
#define NeutrinoAnalysis_h 1

#include <iostream>

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4Step.hh"

#include "TFile.h"
#include "TTree.h"

#include "G4GeneralParticleSource.hh"
#include "NeutrinoAnalysisMessenger.hh"

class NeutrinoAnalysis
{
    public:
        NeutrinoAnalysis(const std::string& name = "AnalysisManager");
        ~NeutrinoAnalysis();

        void BeginOfRunAction(const G4Run*);
        void EndOfRunAction(const G4Run*);

        void BeginOfEventAction(const G4Event*);
        void EndOfEventAction(const G4Event*);
    
        void PreUserTrackingAction(const G4Track*);
        void PostUserTrackingAction(const G4Track*);

        void UserSteppingAction(const G4Step* step);

        static NeutrinoAnalysis* Instance()
        {
            if(instance == 0)
            {
                instance = new NeutrinoAnalysis();
            }

            return instance;
        }

        void SetFileName(G4String name) { fileName = name; }
        void SetPrimaryGenerator(G4GeneralParticleSource* p) { fPrimary = p; }

        void analyseEventID(G4int evtid);
        void analyseTotalPENumber(G4int number);
        void analysePEenergy(G4double energy);
        void analysePEtime(G4double time);

        void analyseEventID_junoPMT(G4int evtid);
        void analyseTotalPENumber_junoPMT(G4int number);
        void analysePEwavelength_junoPMT(G4double energy);
        void analyseOPwavelength_junoPMT(G4double energy);
        void analysePEtime_junoPMT(G4double time);
        void analysePERefNum_junoPMT(G4int num);
        void analysePERayNum_junoPMT(G4int num);
        void analysePEScintNum_junoPMT(G4int num);
        void analyseIsReemission(G4int val);
        void analyseIsCerenkov(G4int val);
        void analyseIsRayleigh(G4int val);
        void analyseIsReflection(G4int val);
        void analyseIsOP(G4int val);


    private:
        G4String fileName;

        TFile *m_file;
        TTree *m_evt_tree;
        TTree *m_step_tree;
        TTree *m_op_tree;
        TTree *m_pe_tree;
        TTree *m_pe_juno_tree;

        int m_evtID;
        int m_stepNum;

        int m_totalOP;
        int m_CerenOP;
        int m_ScintOP;

        int m_totalPE;
        std::vector<double> m_PEenergy;
        std::vector<double> m_PEtime;
        int m_totalPE_junoPMT;
        std::vector<double> m_PEwavelength_junoPMT;
        std::vector<double> m_OPwavelength_junoPMT;
        std::vector<double> m_PEtime_junoPMT;
        std::vector<int> m_PERefNum_junoPMT;
        std::vector<int> m_PERayNum_junoPMT;
        std::vector<int> m_PEScintNum_junoPMT;
        std::vector<int> m_PERefFlag_junoPMT;
        std::vector<int> m_PERayFlag_junoPMT;
        std::vector<int> m_PEScintFlag_junoPMT;
        std::vector<int> m_PECheFlag_junoPMT;
        std::vector<int> m_OPFlag_junoPMT;

        std::vector<double> m_CerenWL;
        std::vector<double> m_ScintWL;
        
        double m_initE;
        
        std::vector<double> m_T;
        std::vector<double> m_E;
        std::vector<double> m_dE;
        std::vector<double> m_dx;
        std::vector<double> m_dEdx;

        G4GeneralParticleSource* fPrimary;
        NeutrinoAnalysisMessenger* analysisMessenger;

    private:
        static NeutrinoAnalysis* instance;
};

#endif
