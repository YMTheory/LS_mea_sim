//------------------------------------------------------------------------
//                   Final state of neutron captured by Gadolinium            
//                            
// Modified class from original G4NeutronHPCaptureFS class to deexcite and
// add correctly the secondary to the hadronic final state
//-------------------------------------------------------------------------
// Author: Liang Zhan, 2006/01/27
// Modifed: bv@bnl.gov 2008/4/16 for DetSim
//-------------------------------------------------------------------------

#ifndef DsG410GdParticleHPCaptureFS_h
#define DsG410GdParticleHPCaptureFS_h 1

#include "globals.hh"
#include "G4HadProjectile.hh"
#include "G4HadFinalState.hh"
#include "G4ParticleHPFinalState.hh"
#include "G4ReactionProductVector.hh"
#include "G4ParticleHPNames.hh"
#include "G4ParticleHPPhotonDist.hh"
#include "G4Nucleus.hh"
#include "G4Fragment.hh"

// the emission gammas of neutron captured by Gd.
#include "DsG4GdCaptureGammas.h"

///////////////////////////////////////////////////////////////////////////////

//class DsG410GdParticleHPCaptureFS : public G4NeutronHPFinalState
class DsG410GdParticleHPCaptureFS : public G4ParticleHPFinalState 
{
public:
  
    DsG410GdParticleHPCaptureFS() {
      hasXsec = false;
      targetMass = 0;
    }
  
    ~DsG410GdParticleHPCaptureFS() {
    }
  
    void   UpdateNucleus( const G4Fragment* , G4double );
    //void Init (G4double A, G4double Z, G4String & dirName, G4String & aFSType);
    void Init (G4double A, G4double Z, G4int M, G4String & dirName, G4String & aFSType, G4ParticleDefinition*);
  
    G4HadFinalState * ApplyYourself(const G4HadProjectile & theTrack);
    G4ParticleHPFinalState * New() {
        DsG410GdParticleHPCaptureFS * theNew = new DsG410GdParticleHPCaptureFS();
        return theNew;
    }
  
private:

    G4Fragment * nucleus;

    G4DynamicParticle * theTwo ;
    G4ReactionProduct theTarget; 
    G4Nucleus aNucleus;
    G4ReactionProduct theNeutron;

    G4double targetMass;
  
    G4ParticleHPPhotonDist theFinalStatePhotons;
    DsG4GdCaptureGammas       theFinalgammas;
    G4ParticleHPNames theNames;
  
    G4double theCurrentA;
    G4double theCurrentZ;
};
#endif
