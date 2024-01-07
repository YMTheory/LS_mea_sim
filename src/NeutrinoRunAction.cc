#include "NeutrinoAnalysis.hh"
#include "NeutrinoRunAction.hh"
#include "NeutrinoRunMessenger.hh"

#include "G4Run.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4Event.hh"

NeutrinoRunAction::NeutrinoRunAction()
{
    runMessenger = new NeutrinoRunMessenger(this);
}

NeutrinoRunAction::~NeutrinoRunAction()
{
    delete runMessenger;
}

void NeutrinoRunAction::BeginOfRunAction(const G4Run* aRun)
{
    NeutrinoAnalysis::Instance()->BeginOfRunAction(aRun);
}

void NeutrinoRunAction::EndOfRunAction(const G4Run* aRun)
{
    NeutrinoAnalysis::Instance()->EndOfRunAction(aRun);
}
