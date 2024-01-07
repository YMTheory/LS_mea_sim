#ifndef NeutrinoSteppingAction_h
#define NeutrinoSteppingAction_h 1

#include "G4UserSteppingAction.hh"

class NeutrinoSteppingAction : public G4UserSteppingAction{
    public:
        NeutrinoSteppingAction();
        virtual ~NeutrinoSteppingAction();

        virtual void UserSteppingAction(const G4Step*);
};

#endif
