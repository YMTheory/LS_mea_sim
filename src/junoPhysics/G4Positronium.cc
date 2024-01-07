// ----------------------------------------------------------------------
//      GEANT 4 class implementation file
//
// Paolo Crivelli (ETHZ)
// Implemented in SNiPER by C.Jollet and A.Meregaglia (CENBG)
// ----------------------------------------------------------------------

#include "G4Positronium.hh"
#include "G4ParticleTable.hh"

#include "G4PositroniumDecayChannel3G.hh"
#include "G4PositroniumDecayChannel2G.hh"
#include "G4DecayTable.hh"
#include "G4SystemOfUnits.hh"

// ######################################################################
// ###                         POSITRONIUM                            ###
// ######################################################################
G4Positronium* G4Positronium::theInstance = 0;

G4Positronium* G4Positronium::Definition()
{
  if (theInstance !=0) return theInstance;
  const G4String name = "positronium";
  // search in particle table]
  G4ParticleTable* pTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* anInstance = pTable->FindParticle(name);
  if (anInstance ==0)
  {
  // create particle
  //
  //    Arguments for constructor are as follows
  //               name             mass          width         charge
  //             2*spin           parity  C-conjugation
  //          2*Isospin       2*Isospin3       G-parity
  //               type    lepton number  baryon number   PDG encoding
  //             stable         lifetime    decay table
  //             shortlived      subType    anti_encoding
    /*
   anInstance = new G4ParticleDefinition(
                 name,  2*0.51099906*MeV,       0.0*MeV,    0, 
		    1,               0,             0,          
		    0,               0,             0,             
	     "atom",              0,             0,          0,
		false,            142.05*ns,          NULL,
                false,             "positronium"
              );
    */

    //lifetime in vacuum overwritten in G4PsitroniumFormation according to the material 
  anInstance = new G4ParticleDefinition(
                 name,  2*0.51099906*MeV,       0.0*MeV,    0, 
		    1,               0,             0,          
		    0,               0,             0,             
	     "atom",              0,             0,          0,
		 false,            142.05*ns,          NULL,
		 false,             "positronium"
              );

  //create Decay Table 
  G4DecayTable* table = new G4DecayTable();
 
  G4double tau3=142.05*ns;//vacuum lifetime, all in 3 gammas
  G4double prob3G=142.05*ns/tau3;
  G4double prob2G=1-prob3G;
  
  // create a decay channel
  G4VDecayChannel* mode = new G4PositroniumDecayChannel3G("positronium",prob3G);

  table->Insert(mode);
   G4VDecayChannel* mode1 = new G4PositroniumDecayChannel2G("positronium",prob2G);
 
  table->Insert(mode1);

  anInstance->SetDecayTable(table);
   
  }
  theInstance = reinterpret_cast<G4Positronium*>(anInstance);
  return theInstance;
}

G4Positronium*  G4Positronium::PositroniumDefinition()
{
  return Definition();
}

G4Positronium*  G4Positronium::Positronium()
{
  return Definition();
}


