// ------------------------------------------------------------
//      GEANT 4 class header file
//
// Paolo Crivelli (ETHZ)
// Implemented in SNiPER by C.Jollet and A.Meregaglia (CENBG)
// ------------------------------------------------------------

#include "G4ParticleDefinition.hh"
#include "G4DecayProducts.hh"
#include "G4VDecayChannel.hh"
#include "G4PositroniumDecayChannel2G.hh"
#include "Randomize.hh"
#include "G4LorentzVector.hh"
#include "G4LorentzRotation.hh"
#include "G4RotationMatrix.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"


G4PositroniumDecayChannel2G::G4PositroniumDecayChannel2G(const G4String& theParentName, 
				       G4double        theBR)
                   :G4VDecayChannel("Positronium Decay",1)
{
  // set names for daughter particles
  if (theParentName == "positronium") {
    SetBR(theBR);
    SetParent("positronium");
    SetNumberOfDaughters(2);
    SetDaughter(0, "gamma");
    SetDaughter(1, "gamma");
  } else {
#ifdef G4VERBOSE
    if (GetVerboseLevel()>0) {
      G4cout << "G4PositroniumDecayChannel2G:: constructor :";
      G4cout << " parent particle is : ";
      G4cout << theParentName << G4endl;
    }
#endif
  }
}

G4PositroniumDecayChannel2G::~G4PositroniumDecayChannel2G()
{
}

G4DecayProducts *G4PositroniumDecayChannel2G::DecayIt(G4double) 
{
 #ifdef G4VERBOSE
  if (GetVerboseLevel()>1) G4cout << "G4PositroniumDecayChannel2G::DecayIt ";
#endif

  // if (G4MT_parent == 0) FillParent();  
  if (G4MT_parent == 0) CheckAndFillParent();  
  if (G4MT_daughters == 0) CheckAndFillDaughters();
 
  // parent mass
  // G4double parentmass = parent->GetPDGMass();
  G4double parentmass = GetParentMass();

  //daughters'mass
  G4double daughtermass[2]; 
  G4double sumofdaughtermass = 0.0;
  for (G4int index=0; index<2; index++){
    //daughtermass[index] = daughters[index]->GetPDGMass();
    daughtermass[index] = GetDaughterMass(index);
    sumofdaughtermass += daughtermass[index];
  }

   //create parent G4DynamicParticle at rest
  G4ThreeVector dummy;
  G4DynamicParticle * parentparticle = new G4DynamicParticle( G4MT_parent, dummy, 0.0);
  //create G4Decayproducts
  G4DecayProducts *products = new G4DecayProducts(*parentparticle);
  delete parentparticle;

   
  
   //Generating Energy
  G4double cosTeta = 2.*G4UniformRand()-1. , sinTeta = sqrt(1.-cosTeta*cosTeta);
  G4double phi     = twopi * G4UniformRand();
  G4ThreeVector direction (sinTeta*cos(phi), sinTeta*sin(phi), cosTeta);

  //gamma 1
  G4DynamicParticle * daughterparticle = new G4DynamicParticle(G4MT_daughters[0],direction, parentmass/2);
  products->PushProducts(daughterparticle);
  
  // gamma 2

  G4DynamicParticle * daughterparticle1 = new G4DynamicParticle(G4MT_daughters[1],-direction, parentmass/2);
  products->PushProducts(daughterparticle1);

  G4cout<<" decay 2 gammas "<<parentmass/2<<G4endl;

 // output message
#ifdef G4VERBOSE
  if (GetVerboseLevel()>1) {
    G4cout << "G4PositroniumDecayChannel2G::DecayIt ";
    G4cout << "  create decay products in rest frame " <<G4endl;
    products->DumpInfo();
  }
#endif
  return products;
}




