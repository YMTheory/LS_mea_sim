#include "G4ios.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"

#include "NeutrinoAnalysis.hh"
#include "NeutrinoEventAction.hh"

NeutrinoEventAction::NeutrinoEventAction()
{
}

NeutrinoEventAction::~NeutrinoEventAction()
{
}

void NeutrinoEventAction::BeginOfEventAction(const G4Event* anEvent)
{
    NeutrinoAnalysis::Instance()->BeginOfEventAction(anEvent);
}

void NeutrinoEventAction::EndOfEventAction(const G4Event* anEvent)
{   
    NeutrinoAnalysis::Instance()->EndOfEventAction(anEvent);
}
