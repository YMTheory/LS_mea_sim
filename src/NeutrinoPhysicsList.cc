//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "NeutrinoPhysicsList.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicsConstructor.hh"
// #include "DsPhysConsElectroNu.h"
// #include "DsPhysConsEM.h"
// #include "DsPhysConsGeneral.h"
// #include "DsPhysConsHadron.h"
// #include "DsPhysConsIon.h"
#include "DsPhysConsOptical.h"

#include "G4RegionStore.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTable.hh"

#include "G4ParticleTypes.hh"
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4Decay.hh"
#include "G4RadioactiveDecay.hh"
//#include "DsG4RadioactiveDecay.hh"
#include "G4ProcessManager.hh"
#include "G4ProductionCutsTable.hh"
#include "G4ParticlePropertyTable.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "DsG4EmLivermorePhysics.hh"
#include "DsWrapPhysConsIon.h"
#include "G4EmLivermorePhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "DsG4HadronPhysicsQGSP_BERT_HP.hh"
//#include "G4HadronPhysicsQGSP_BERT_HP.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4IonPhysicsPHP.hh"

#include "G4UnitsTable.hh"
#include "G4LossTableManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutrinoPhysicsList::NeutrinoPhysicsList() : G4VModularPhysicsList()
{
  cutForGamma     = 1.0*mm;
  cutForElectron  = 0.1*mm;
  cutForPositron  = 0.1*mm;
  

  SetVerboseLevel(1);

  /*
  //default physics
  electroNuList = new DsPhysConsElectroNu();
  // emPhysicsList = new DsPhysConsEM();
  emPhysicsList = 0;
  generalList = new DsPhysConsGeneral();
  hadronList = new DsPhysConsHadron();
  ionList = new DsPhysConsIon();
  */
  // opticalList = new DsPhysConsOptical();

  //emPhysicsList = new G4EmLivermorePhysics();
  // emPhysicsList = new DsG4EmLivermorePhysics();
  emPhysicsList = 0;
  
  emExtraPhysicsList = new G4EmExtraPhysics();
  decayList = new G4DecayPhysics();
  raddecayList = new G4RadioactiveDecayPhysics();

  hadronElasticList = new G4HadronElasticPhysicsHP();
  //hadronList = new G4HadronPhysicsQGSP_BERT_HP();
  hadronList = new DsG4HadronPhysicsQGSP_BERT_HP();
  stoppingList = new G4StoppingPhysics();
 // ionList = new G4IonPhysicsPHP();
  ionList = 0 ;

  opticalList = 0;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutrinoPhysicsList::~NeutrinoPhysicsList()
{
  /*
  delete electroNuList;
  delete emPhysicsList;
  delete generalList;
  delete hadronList;
  delete ionList;
  // delete opticalList;
  */

  delete emPhysicsList; // if it is a SNiPER tool, don't delete it.
  delete emExtraPhysicsList;
  delete decayList;
  delete raddecayList;
  delete hadronElasticList;
  delete hadronList;
  delete stoppingList;
  delete ionList;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutrinoPhysicsList::ConstructParticle()
{
    {   // All bosons
        G4BosonConstructor con;
        con.ConstructParticle();
    }
    {   // All leptons
        G4LeptonConstructor con;
        con.ConstructParticle();
    }
    {   // All mesons
        G4MesonConstructor con;
        con.ConstructParticle();
    }
    {   // All baryons
        G4BaryonConstructor con;
        con.ConstructParticle();
    }
    {   // All ions
        G4IonConstructor con;
        con.ConstructParticle();
    }

    raddecayList->ConstructParticle();

    {   // all short lived (resonaces and quarks)
        G4ShortLivedConstructor con;
        con.ConstructParticle();
    }

    // construct particle in em physics list
    emPhysicsList = new DsG4EmLivermorePhysics();
    emPhysicsList->ConstructParticle();

    // construct particle im ion physics list
    ionList = new DsWrapPhysConsIon("Ion");
    ionList->ConstructParticle();

    // construct optical physics list
    opticalList = new DsPhysConsOptical();
    opticalList->ConstructParticle();

    /// Special hook: change the PDG encoded value for optical photons to be unique and useful.
    G4ParticlePropertyTable* propTable = G4ParticlePropertyTable::GetParticlePropertyTable();
    assert(propTable);
    G4ParticlePropertyData* photonData = propTable->GetParticleProperty(G4OpticalPhoton::Definition());
    assert(photonData);
    photonData->SetPDGEncoding(20022);
    photonData->SetAntiPDGEncoding(20022);
    if(propTable->SetParticleProperty(*photonData))
      G4cout << "Set PDG code for opticalphoton to " << G4OpticalPhoton::Definition()->GetPDGEncoding() << G4endl;
    else
      G4cout << "Failed to reset PDG code on opticalphoton.. it's still set to "
                << G4OpticalPhoton::Definition()->GetPDGEncoding() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutrinoPhysicsList::ConstructProcess()
{
  // construct process

  AddTransportation();
  emPhysicsList->ConstructProcess();
  emExtraPhysicsList->ConstructProcess();
  decayList->ConstructProcess();
  raddecayList->ConstructProcess();
  hadronElasticList->ConstructProcess();
  hadronList->ConstructProcess();
  stoppingList->ConstructProcess();
  ionList->ConstructProcess();

  opticalList->ConstructProcess();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutrinoPhysicsList::SetCuts()
{
  this->SetCutsWithDefault();

  // SetCutValue(cutForGamma, "gamma");
  // SetCutValue(cutForElectron, "e-");
  // SetCutValue(cutForPositron, "e+");

  G4double lowlimit=250*eV;
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(lowlimit,100.*TeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutrinoPhysicsList::SetCutForGamma(G4double cut)
{
  cutForGamma = cut;
  SetParticleCuts(cutForGamma, G4Gamma::Gamma());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutrinoPhysicsList::SetCutForElectron(G4double cut)
{
  cutForElectron = cut;
  SetParticleCuts(cutForElectron, G4Electron::Electron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutrinoPhysicsList::SetCutForPositron(G4double cut)
{
  cutForPositron = cut;
  SetParticleCuts(cutForPositron, G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
