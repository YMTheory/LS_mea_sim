#include "NeutrinoAnalysis.hh"
#include "NeutrinoAnalysisMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

NeutrinoAnalysisMessenger::NeutrinoAnalysisMessenger()
{
    UIdir = new G4UIdirectory("/analysis/");
    UIdir->SetGuidance("Set parameters for analysis manager");

    setFileNameCmd = new G4UIcmdWithAString("/analysis/setFileName", this);
    setFileNameCmd->SetGuidance("Set output file name");
}

NeutrinoAnalysisMessenger::~NeutrinoAnalysisMessenger()
{
}

void NeutrinoAnalysisMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if(command == setFileNameCmd)
    {
        NeutrinoAnalysis::Instance()->SetFileName(newValue);
    }
}
