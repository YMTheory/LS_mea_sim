// ------------------------------------------------------------
//      GEANT 4 class header file
//
// Paolo Crivelli (ETHZ)
// Implemented in SNiPER by C.Jollet and A.Meregaglia (CENBG)
// ------------------------------------------------------------
#ifndef G4PositroniumDecayChannel3G_h
#define G4PositroniumDecayChannel3G_h 1

#include "G4ios.hh"
#include "globals.hh"
#include "G4VDecayChannel.hh"

class G4PositroniumDecayChannel3G :public G4VDecayChannel
{
  // Class Decription
  //  This class describes orthopositronium decay kinemtics.

  public:  // With Description
    //Constructors 
      G4PositroniumDecayChannel3G(const G4String& theParentName,
			 G4double        theBR);
    //  Destructor
      virtual ~G4PositroniumDecayChannel3G();

  public:  // With Description
  virtual G4DecayProducts *DecayIt(G4double);     
  
  private:
  G4double dSigma(G4double e1,G4double e2,G4double e3);  
  void   rot(G4double *x0, G4double phi, G4double theta, G4double chi);

};  


#endif

