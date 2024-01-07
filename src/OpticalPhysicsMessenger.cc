#include "DsPhysConsOptical.h"
#include "OpticalPhysicsMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithABool.hh"

OpticalPhysicsMessenger::OpticalPhysicsMessenger(DsPhysConsOptical* fOPphys) : OPphys(fOPphys)
{
    physDirectory = new G4UIdirectory("/neu/phys/");
    physDirectory -> SetGuidance("Optical physics configuration.");

    scintSimpleCmd = new G4UIcmdWithABool("/neu/phys/scintSimple", this);
    scintSimpleCmd -> SetGuidance("If use the simple scintillation process.");
    scintSimpleCmd -> SetParameterName("enableScintSimple", true);
    scintSimpleCmd -> SetDefaultValue(true);

    absReemitCmd = new G4UIcmdWithABool("/neu/phys/absReemit", this);
    absReemitCmd -> SetGuidance("If use the absorption-reemision process.");
    absReemitCmd -> SetParameterName("enableAbsReemit", true);
    absReemitCmd -> SetDefaultValue(true);

}

OpticalPhysicsMessenger::~OpticalPhysicsMessenger()
{
    delete absReemitCmd;
    delete scintSimpleCmd;
    delete physDirectory;
}


void OpticalPhysicsMessenger::SetNewValue
(G4UIcommand* cmd, G4String newValues) {
    if (cmd == scintSimpleCmd) 
        OPphys -> setUseScintSimple(scintSimpleCmd->GetNewBoolValue(newValues));
    else if (cmd == absReemitCmd) 
        OPphys -> setUseAbsReemit(absReemitCmd->GetNewBoolValue(newValues));
    else
        G4cout << "Error: Unknown Command !!! " << G4endl;
}
