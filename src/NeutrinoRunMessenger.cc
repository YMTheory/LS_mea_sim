#include "NeutrinoRunMessenger.hh"
#include "NeutrinoRunAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

NeutrinoRunMessenger::NeutrinoRunMessenger(NeutrinoRunAction* aRunAction)
    : fRunAction(aRunAction)
{
}

NeutrinoRunMessenger::~NeutrinoRunMessenger()
{
}

void NeutrinoRunMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
}
