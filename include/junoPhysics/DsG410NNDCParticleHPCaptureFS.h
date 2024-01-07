//------------------------------------------------------------------------
//                   Final state of neutron capture            
//                            
// Modified class from original DsG4GdNeutronHPCaptureFS class 
//-------------------------------------------------------------------------
// Author: Dan Dwyer, 2009/07/27
//-------------------------------------------------------------------------

#ifndef DsG410NNDCParticleHPCaptureFS_h
#define DsG410NNDCParticleHPCaptureFS_h 1

#include "globals.hh"
#include "G4HadProjectile.hh"
#include "G4HadFinalState.hh"
#include "G4ParticleHPFinalState.hh"
#include "G4ReactionProductVector.hh"
#include "G4ParticleHPNames.hh"
#include "G4ParticleHPPhotonDist.hh"
#include "G4Nucleus.hh"
#include "G4Fragment.hh"

// the emission gammas of neutron capture from NNDC tables.
#include "DsG4NNDCCaptureGammas.h"

///////////////////////////////////////////////////////////////////////////////

class DsG410NNDCParticleHPCaptureFS : public G4ParticleHPFinalState
{
public:
  
    DsG410NNDCParticleHPCaptureFS();
  
    ~DsG410NNDCParticleHPCaptureFS();
  
    void   UpdateNucleus( const G4Fragment* , G4double );
    //void Init (G4double A, G4double Z, G4String & dirName, G4String & aFSType);
    void Init (G4double A, G4double Z, G4int M, G4String & dirName, G4String & aFSType, G4ParticleDefinition* );
  
    G4HadFinalState * ApplyYourself(const G4HadProjectile & theTrack);
    G4ParticleHPFinalState * New() {
        DsG410NNDCParticleHPCaptureFS * theNew = new DsG410NNDCParticleHPCaptureFS;
        return theNew;
    }
    G4ParticleHPVector* GetXsec( ){ return &theCrossSection; }
  
private:

    G4Fragment * nucleus;

    G4DynamicParticle * theTwo ;
    G4ReactionProduct theTarget; 
    G4Nucleus aNucleus;
    G4ReactionProduct theNeutron;

    G4double targetMass;
  
    DsG4NNDCCaptureGammas       theFinalgammas;
    G4ParticleHPVector theCrossSection;
};
#endif
