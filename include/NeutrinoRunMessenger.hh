#ifndef NeutrinoRunMessenger_h
#define NeutrinoRunMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class NeutrinoRunAction;
class G4UIdirectory;
class G4UIcmdWithAString;

class NeutrinoRunMessenger : public G4UImessenger
{
    public:
        NeutrinoRunMessenger(NeutrinoRunAction*);
        ~NeutrinoRunMessenger();

        void SetNewValue(G4UIcommand*, G4String);

    private:
        NeutrinoRunAction* fRunAction;
        G4UIdirectory* fFileDir;

        //change output filename
        G4UIcmdWithAString* fFileCmd;
};

#endif
