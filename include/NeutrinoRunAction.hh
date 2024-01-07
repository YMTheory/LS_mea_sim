#ifndef NeutrinoRunAction_h
#define NeutrinoRunAction_h 1

#include "G4UserRunAction.hh"
#include "TFile.h"
#include "TTree.h"
#include "globals.hh"

#include <string>

class G4Run;
class G4Timer;
class NeutrinoRunMessenger;

class NeutrinoRunAction : public G4UserRunAction{
    public:
        NeutrinoRunAction();
        ~NeutrinoRunAction();

    public:
        void BeginOfRunAction(const G4Run*);
        void EndOfRunAction(const G4Run*);
        
    private:
        NeutrinoRunMessenger* runMessenger;
};

#endif
