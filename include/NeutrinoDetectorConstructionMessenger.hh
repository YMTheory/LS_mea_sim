#ifndef NeutrinoDetectorConstructionMessenger_h
#define NeutrinoDetectorConstructionMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class NeutrinoDetectorConstruction;
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;

class NeutrinoDetectorConstructionMessenger : public G4UImessenger {
    public:
        NeutrinoDetectorConstructionMessenger(NeutrinoDetectorConstruction* fLSDet);
        ~NeutrinoDetectorConstructionMessenger();

        void SetNewValue(G4UIcommand* cmd, G4String newValues);

    private:
        NeutrinoDetectorConstruction* LSDet;

    private:
        G4UIdirectory*              neuDirectory;
        G4UIcmdWithADouble*         alreflectionCmd;
        G4UIcmdWithAString*         alSurfaceFinishCmd;
        G4UIcmdWithADoubleAndUnit*  innertubeRCmd;
        G4UIcmdWithAString*         opticalModelCmd;

        G4UIcmdWithADouble*         ppoConcentrationCmd;
        G4UIcmdWithADouble*         bisMSBConcentrationCmd;
};

#endif
