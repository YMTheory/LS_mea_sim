#include "NeutrinoDetectorConstruction.hh"
#include "NeutrinoDetectorConstructionMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

NeutrinoDetectorConstructionMessenger::NeutrinoDetectorConstructionMessenger(NeutrinoDetectorConstruction* fLSDet) : LSDet(fLSDet)
{
    neuDirectory = new G4UIdirectory("/neu/det/");
    neuDirectory -> SetGuidance("Detectro configuration.");

    alreflectionCmd = new G4UIcmdWithADouble("/neu/det/alref", this);
    alreflectionCmd -> SetGuidance("Set reflection of Al.");
    alreflectionCmd -> SetParameterName("alreflection", true);
    alreflectionCmd -> SetDefaultValue(0.5);

    alSurfaceFinishCmd = new G4UIcmdWithAString("/neu/det/alfinish", this);
    alSurfaceFinishCmd -> SetGuidance("Set Al surface finish type.");
    alSurfaceFinishCmd -> SetParameterName("alSurfaceFinish", true);
    alSurfaceFinishCmd -> SetDefaultValue("polished");

    innertubeRCmd = new G4UIcmdWithADoubleAndUnit("/neu/det/tubeinr", this);
    innertubeRCmd -> SetGuidance("Inner radius of the LS tube.");
    innertubeRCmd -> SetParameterName("innertubeR", true);
    innertubeRCmd -> SetDefaultValue(5);
    innertubeRCmd -> SetDefaultUnit("cm");
    innertubeRCmd -> SetUnitCandidates("mm cm m");

    opticalModelCmd = new G4UIcmdWithAString("/neu/det/opticalModel", this);
    opticalModelCmd -> SetGuidance("Which optical model to use (absorption length setting).");
    opticalModelCmd -> SetParameterName("opticalModel", true);
    opticalModelCmd -> SetDefaultValue("old");

    ppoConcentrationCmd = new G4UIcmdWithADouble("/neu/det/ppo", this);
    ppoConcentrationCmd -> SetGuidance("Set PPO concentration with unit g/L.");
    ppoConcentrationCmd -> SetParameterName("PPOConcentration", true);
    ppoConcentrationCmd -> SetDefaultValue(2.5);


    bisMSBConcentrationCmd = new G4UIcmdWithADouble("/neu/det/bisMSB", this);
    bisMSBConcentrationCmd -> SetGuidance("Set bisMSB concentration with unit mg/L.");
    bisMSBConcentrationCmd -> SetParameterName("bisMSBConcentration", true);
    bisMSBConcentrationCmd -> SetDefaultValue(1);
}

NeutrinoDetectorConstructionMessenger::~NeutrinoDetectorConstructionMessenger()
{
    delete alreflectionCmd;
    delete innertubeRCmd;
    delete alSurfaceFinishCmd;
    delete opticalModelCmd;
    delete ppoConcentrationCmd;
    delete bisMSBConcentrationCmd;
    delete neuDirectory;
}

void NeutrinoDetectorConstructionMessenger::SetNewValue(G4UIcommand* cmd, G4String newValues) 
{
    if (cmd == alreflectionCmd)
        LSDet -> SetAlReflection(alreflectionCmd->GetNewDoubleValue(newValues));
    else if (cmd == alSurfaceFinishCmd)
        LSDet -> SetAlSurfaceFinish(newValues);
    else if (cmd == innertubeRCmd)
        LSDet -> SetTubeInnerRadius(innertubeRCmd->GetNewDoubleValue(newValues));
    else if (cmd == opticalModelCmd)
        LSDet -> SetOpticalModel(newValues);
    else if (cmd == ppoConcentrationCmd)
        LSDet -> SetPPOConcentration(ppoConcentrationCmd->GetNewDoubleValue(newValues));
    else if ( cmd == bisMSBConcentrationCmd) 
        LSDet -> SetbisMSBConcentration(bisMSBConcentrationCmd->GetNewDoubleValue(newValues));
    else
        G4cout << "Error: Unknown Command !" << G4endl;
}

