// ------------------------------------------------------------
//      GEANT 4 class header file
//
// Paolo Crivelli (ETHZ)
// Implemented in SNiPER by C.Jollet and A.Meregaglia (CENBG)
// ----------------------------------------------------------------

#ifndef G4Positronium_h
#define G4Positronium_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"


// ######################################################################
// ###                         POSITRONIUM                            ###
// ######################################################################

class G4Positronium : public G4ParticleDefinition
{
 private:
   static G4Positronium* theInstance;
   G4Positronium(){}
   ~G4Positronium(){}

 public:
   static G4Positronium* Definition(G4double lifetime);
   static G4Positronium* PositroniumDefinition(G4double lifetime);
   static G4Positronium* Positronium(G4double lifetime);
  static G4Positronium* Definition();
  static G4Positronium* PositroniumDefinition();
  static G4Positronium* Positronium();
};


#endif




