#ifndef NeutrinoPrimaryGeneratorAction_h
#define NeutrinoPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

class G4GeneralParticleSource;
class G4Event;

class NeutrinoPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction{
    public:
        NeutrinoPrimaryGeneratorAction();
        ~NeutrinoPrimaryGeneratorAction();

    public:
        void GeneratePrimaries(G4Event* anEvent);

    private:
        G4GeneralParticleSource* generator;

};

#endif
