// ------------------------------------------------------------
//      GEANT 4 class header file
//
// Paolo Crivelli (ETHZ
// Implemented in SNiPER by C.Jollet and A.Meregaglia (CENBG)
// ------------------------------------------------------------
#ifndef G4PositroniumDecayChannel2G_h
#define G4PositroniumDecayChannel2G_h 1

#include "G4ios.hh"
#include "globals.hh"
#include "G4VDecayChannel.hh"

class G4PositroniumDecayChannel2G :public G4VDecayChannel
{
  // Class Decription
  //  This class describes paraopositronium decay kinemtics.

  public:  // With Description
    //Constructors 
      G4PositroniumDecayChannel2G(const G4String& theParentName,
			 G4double        theBR);
    //  Destructor
      virtual ~G4PositroniumDecayChannel2G();

  public:  // With Description
  virtual G4DecayProducts *DecayIt(G4double);     
  
  private:


};  


#endif

