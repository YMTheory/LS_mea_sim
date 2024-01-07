#ifndef OpticalPhysicsMessenger_h
#define OpticalPhysicsMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class DsPhysConsOptical;
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithABool;

class OpticalPhysicsMessenger : public G4UImessenger {

    public:
        OpticalPhysicsMessenger(DsPhysConsOptical* fOPphys);
        ~OpticalPhysicsMessenger();

        void SetNewValue(G4UIcommand* cmd, G4String newValues);

    private:
        DsPhysConsOptical* OPphys;

    private:
        G4UIdirectory*      physDirectory;
        G4UIcmdWithABool*   scintSimpleCmd;
        G4UIcmdWithABool*   absReemitCmd;

};

#endif
