#ifndef NeutrinoAnalysisMessenger_h
#define NeutrinoAnalysisMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithAString;

class NeutrinoAnalysisMessenger : public G4UImessenger
{
    public:
        NeutrinoAnalysisMessenger();
        ~NeutrinoAnalysisMessenger();

        void SetNewValue(G4UIcommand*, G4String);

    private:
        G4UIdirectory* UIdir;
        G4UIcmdWithAString* setFileNameCmd;
};

#endif
