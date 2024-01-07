// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:     G4PositroniumFormation
//
// Author:        Pol Crivelli based on of G4eplusAnnihilation code
// Implemented in SNiPER by C.Jollet and A.Meregaglia (CENBG)
//
// Creation date: 20.10.2006
//
// Modifications:
//

//
// -------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4PositroniumFormation.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4Gamma.hh"
#include "G4PhysicsVector.hh"
#include "G4PhysicsLogVector.hh"
#include "G4eeToTwoGammaModel.hh"
#include "G4EventManager.hh"
#include "G4DecayTable.hh"
#include "G4PositroniumDecayChannel3G.hh"
#include "G4PositroniumDecayChannel2G.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

G4PositroniumFormation::G4PositroniumFormation(const G4String& name)
  : G4VEmProcess(name), isInitialised(false)
{
 enableAtRestDoIt=true;
 SetProcessSubType(fAnnihilation);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4PositroniumFormation::~G4PositroniumFormation()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4PositroniumFormation::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialised) {
    isInitialised = true;
    //    SetVerboseLevel(3);
    SetBuildTableFlag(true);
    SetStartFromNullFlag(false);
    SetSecondaryParticle(G4Positronium::Positronium());
    
    G4double emin = 0.001*keV;
    G4double emax = 1.*TeV;
    SetLambdaBinning(120);
    SetMinKinEnergy(emin);
    SetMaxKinEnergy(emax);
    G4VEmModel* em = new G4eeToTwoGammaModel();
    em->SetLowEnergyLimit(emin);
    em->SetHighEnergyLimit(emax);
    AddEmModel(1, em);


  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4PositroniumFormation::PrintInfo()
{
  G4cout << "      Formation of positronium with annihilation probability included"
         << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4VParticleChange* G4PositroniumFormation::AtRestDoIt(const G4Track& aTrack,
                                                     const G4Step& )
//
{
  //G4cout << "G4PositroniumFormation::AtRestDoIt" << G4endl;

  //select scintillator for Ps creation, in other volumes do annihilation only
  G4String volname = aTrack.GetStep()->GetPreStepPoint()->GetPhysicalVolume()->GetName();

    
  G4double prob;//probability of doing oPs
    prob=0;

  //select plexiglass for PS creation with the correct values
  G4String matname = aTrack.GetStep()->GetPreStepPoint()->GetMaterial()->GetName();
  if(matname=="LS")
    prob=0.545;//from https://arxiv.org/pdf/1011.5736.pdf
  //prob=1.;
 
  if(G4UniformRand()>prob)
    {
      //annihilation
        fParticleChange.InitializeForPostStep(aTrack);
 
	fParticleChange.SetNumberOfSecondaries(2);
 
	G4double cosTeta = 2.*G4UniformRand()-1. , sinTeta = sqrt(1.-cosTeta*cosTeta);
	G4double phi     = twopi * G4UniformRand();
	G4ThreeVector direction (sinTeta*cos(phi), sinTeta*sin(phi), cosTeta);
	fParticleChange.AddSecondary( new G4DynamicParticle (G4Gamma::Gamma(),
							     direction, electron_mass_c2) );
	fParticleChange.AddSecondary( new G4DynamicParticle (G4Gamma::Gamma(),
							     -direction, electron_mass_c2) );
  }
  else
    {
      // Positronium
      fParticleChange.InitializeForPostStep(aTrack);
      
      fParticleChange.SetNumberOfSecondaries(1);
      
      G4double cosTeta = 2.*G4UniformRand()-1. , sinTeta = sqrt(1.-cosTeta*cosTeta);
      G4double phi     = twopi * G4UniformRand();
      if(cosTeta<0.){
	cosTeta*=-1.;
      }
      G4ThreeVector direction (sinTeta*cos(phi), sinTeta*sin(phi), cosTeta);
      
      
      G4ParticleDefinition *partpos=G4Positronium::Positronium();
 
      if(matname=="LS")
	{
	  partpos->SetPDGLifeTime(3.08*ns);//from https://arxiv.org/pdf/1011.5736.pdf
	  G4DecayTable* table = new G4DecayTable();
	  G4double tau3=142.05*ns;//vacuum lifetime, all in 3 gammas
	  G4double prob3G=3.08*ns/tau3;// from https://arxiv.org/pdf/1011.5736.pdf
	  G4double prob2G=1-prob3G;
	  G4VDecayChannel* mode = new G4PositroniumDecayChannel3G("positronium",prob3G);
	  table->Insert(mode);
	  G4VDecayChannel* mode1 = new G4PositroniumDecayChannel2G("positronium",prob2G);
	  table->Insert(mode1);
	  partpos->SetDecayTable(table);
	}

 
	fParticleChange.AddSecondary( new G4DynamicParticle (partpos,direction,0));
	//	fParticleChange.AddSecondary( new G4DynamicParticle (G4Positronium::Positronium(),direction,0));
    

    }



      // Kill the incident positron
      
      fParticleChange.ProposeTrackStatus(fStopAndKill);
      return &fParticleChange;
    
}


