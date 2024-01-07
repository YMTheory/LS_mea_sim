// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4PositroniumFormation
//
// Paolo Crivelli (ETHZ)
//
// Implemented in SNiPER by C.Jollet and A.Meregaglia (CENBG)
// -------------------------------------------------------------------
//

#ifndef G4PositroniumFormation_h
#define G4PositroniumFormation_h 1

#include "G4VEmProcess.hh"
#include "G4Positron.hh"
#include "G4Positronium.hh"
#include "G4VEmModel.hh"

class G4PositroniumFormation : public G4VEmProcess
{

public:

  G4PositroniumFormation(const G4String& name = "PsFormation");

  virtual ~G4PositroniumFormation();

  virtual G4bool IsApplicable(const G4ParticleDefinition& p);

  virtual G4VParticleChange* AtRestDoIt(
                             const G4Track& track,
                             const G4Step& stepData);

  G4double AtRestGetPhysicalInteractionLength(
                             const G4Track& track,
                             G4ForceCondition* condition
                            );

  // Print out of the class parameters
  virtual void PrintInfo();

  //Calculate the ops velocity and kinetic energy

  G4double GetOpsVelocity();
  G4double OpsVelocity(G4double T);
  G4double GetOpsEkin();


protected:

  virtual void InitialiseProcess(const G4ParticleDefinition*);

  std::vector<G4DynamicParticle*>* SecondariesPostStep(
                                   G4VEmModel*,
                             const G4MaterialCutsCouple*,
                             const G4DynamicParticle*);

private:

 // hide assignment operator
  G4PositroniumFormation & operator=(const G4PositroniumFormation &right);
  G4PositroniumFormation(const G4PositroniumFormation&);
  
  G4bool isInitialised;


  

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4bool G4PositroniumFormation::IsApplicable(const G4ParticleDefinition& p)
{
  return (&p == G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4double G4PositroniumFormation::AtRestGetPhysicalInteractionLength(
                              const G4Track&, G4ForceCondition* condition)
{
  *condition = NotForced;
  return 0.0;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

