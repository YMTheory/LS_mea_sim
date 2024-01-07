#include "NeutrinoAnalysis.hh"
#include "NeutrinoPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GeneralParticleSource.hh"

NeutrinoPrimaryGeneratorAction::NeutrinoPrimaryGeneratorAction()
{
    generator = new G4GeneralParticleSource();
}

NeutrinoPrimaryGeneratorAction::~NeutrinoPrimaryGeneratorAction()
{
    delete generator;
}


void NeutrinoPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    generator->GeneratePrimaryVertex(anEvent);
}
