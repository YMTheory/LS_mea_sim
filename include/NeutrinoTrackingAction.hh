#ifndef NeutrinoTrackingAction_H
#define NeutrinoTrackingAction_H 1

#include "G4UserTrackingAction.hh"

class G4Track;

class NeutrinoTrackingAction : public G4UserTrackingAction{
    public:
        NeutrinoTrackingAction();
        ~NeutrinoTrackingAction();

    public:
        void PreUserTrackingAction(const G4Track*);
        void PostUserTrackingAction(const G4Track*);
};

#endif
