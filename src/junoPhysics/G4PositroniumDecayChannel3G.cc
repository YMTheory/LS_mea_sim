// ------------------------------------------------------------
//      GEANT 4 class header file
//
// Paolo Crivelli (ETHZ)
// Implemented in SNiPER by C.Jollet and A.Meregaglia (CENBG)
// ------------------------------------------------------------

#include "G4ParticleDefinition.hh"
#include "G4DecayProducts.hh"
#include "G4VDecayChannel.hh"
#include "G4PositroniumDecayChannel3G.hh"
#include "Randomize.hh"
#include "G4LorentzVector.hh"
#include "G4LorentzRotation.hh"
#include "G4RotationMatrix.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

G4PositroniumDecayChannel3G::G4PositroniumDecayChannel3G(const G4String& theParentName, 
				       G4double        theBR)
                   :G4VDecayChannel("Positronium Decay",1)
{
  // set names for daughter particles
  if (theParentName == "positronium") {
    SetBR(theBR);
    SetParent("positronium");
    SetNumberOfDaughters(3);
    SetDaughter(0, "gamma");
    SetDaughter(1, "gamma");
    SetDaughter(2, "gamma");
  } else {
#ifdef G4VERBOSE
    if (GetVerboseLevel()>0) {
      G4cout << "G4PositroniumDecayChannel:: constructor :";
      G4cout << " parent particle is not muon but ";
      G4cout << theParentName << G4endl;
    }
#endif
  }
}

G4PositroniumDecayChannel3G::~G4PositroniumDecayChannel3G()
{
}

G4DecayProducts *G4PositroniumDecayChannel3G::DecayIt(G4double) 
{

#ifdef G4VERBOSE
  if (GetVerboseLevel()>1) G4cout << "G4PositroniumDecayChannel3G::DecayIt ";
#endif

  if (G4MT_parent == 0) CheckAndFillParent();  
  if (G4MT_daughters == 0) CheckAndFillDaughters();
 
  // parent mass
  // G4double parentmass = parent->GetPDGMass();
  G4double parentmass = GetParentMass();

  //daughters'mass
  G4double daughtermass[3]; 
  G4double sumofdaughtermass = 0.0;
  for (G4int index=0; index<3; index++){
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

   
  
   //Generating Random Energy according to the Matrix element dSigma

  G4double e1,e2,e3;
  while(1){
    e1 =parentmass/2.*G4UniformRand();
    e2 =parentmass/2.*G4UniformRand();  
    if(e1+e2<parentmass/2.) continue;
    e3 = parentmass - e1 - e2;
    if(G4UniformRand()<dSigma(e1,e2,e3)) break;
  }
   
  //order by energy
  if(e1<e2){G4double em=e2;e2=e1;e1=em;}
  if(e1<e3){G4double em=e3;e3=e1;e1=em;}
  if(e2<e3){G4double em=e3;e3=e2;e2=em;}

  //find angles
  G4double ang1 = 0;
  G4double ang2 = twopi/2.0 - acos(parentmass/e1+parentmass/e2-0.5*pow(parentmass,2)/(e1*e2)-1);
  G4double ang3 = twopi/2.0 + asin(e2*sin(ang2)/e3);

  if(G4UniformRand()>0.50){ang2=twopi-ang2;ang3=twopi-ang3;}  

  //random rotation in xy plane
    G4double xyrot = twopi*G4UniformRand();
    ang1+=xyrot;  ang2+=xyrot;  ang3+=xyrot;

  G4double x1[4],x2[4],x3[4];
    x1[0] = 1; x1[1] = cos(ang1);  x1[2] = sin(ang1);   x1[3] = 0;
    x2[0] = 1; x2[1] = cos(ang2);  x2[2] = sin(ang2);   x2[3] = 0;
    x3[0] = 1; x3[1] = cos(ang3);  x3[2] = sin(ang3);   x3[3] = 0;
 
 //random rotation
    G4double a1 = twopi*G4UniformRand();
    G4double a2 = 2*G4UniformRand()-1.0;a2 = acos(a2);
    G4double a3 = twopi*G4UniformRand();
    rot(x1,a1,a2,a3);  rot(x2,a1,a2,a3);  rot(x3,a1,a2,a3);

  G4ThreeVector d1(x1[1],x1[2],x1[3]);
  G4ThreeVector d2(x2[1],x2[2],x2[3]);
  G4ThreeVector d3(x3[1],x3[2],x3[3]);

  //gamma 1

  G4DynamicParticle * daughterparticle = new G4DynamicParticle ( G4MT_daughters[0],   d1 * e1);
  products->PushProducts(daughterparticle);
  
  // gamma 2


  G4DynamicParticle * daughterparticle1 = new G4DynamicParticle ( G4MT_daughters[1],	 d2 * e2);
  products->PushProducts(daughterparticle1);

  //gamma 3
  

  G4DynamicParticle * daughterparticle2 = new G4DynamicParticle ( G4MT_daughters[2],  d3 * e3);
  products->PushProducts(daughterparticle2);


 // output message
#ifdef G4VERBOSE
  if (GetVerboseLevel()>1) {
    G4cout << "G4PositroniumDecayChannel3G::DecayIt ";
    G4cout << "  create decay products in rest frame " <<G4endl;
    products->DumpInfo();
  }
#endif
  return products;
}

G4double G4PositroniumDecayChannel3G::dSigma(G4double e1,G4double e2,G4double e3){
  //  Matrix element for decay of o_Ps -> 3 gamma
  //  Source: V.B.Berestetskii, E.M.Lifshitz, L.P.Pitaevskii 
  //         "Relativistic Quantum Theory", Volume 4, part 1, 
  //          Pergamon Press Ltd., New York, 1971. Page 312
  G4double Me = (GetParentMass())/2.;
 
  return (pow(Me,2)/2)*(pow((Me-e1)/(e2*e3),2) + pow((Me-e2)/(e1*e3),2) + pow((Me-e3)/(e1*e2),2));
}

void G4PositroniumDecayChannel3G::rot(G4double *x0, G4double phi, G4double theta, G4double chi){
  G4double a[3][3];

  a[0][0] = cos(chi)*cos(phi)-cos(theta)*sin(phi)*sin(chi);
  a[0][1] = cos(chi)*sin(phi)+cos(theta)*cos(phi)*sin(chi);
  a[0][2] = sin(chi)*sin(theta);
  
  a[1][0] = -sin(chi)*cos(phi) - cos(theta)*sin(phi)*cos(chi);
  a[1][1] = -sin(chi)*sin(phi) + cos(theta)*cos(phi)*cos(chi);
  a[1][2] = cos(chi)*sin(theta);

  a[2][0] = sin(theta)*sin(phi);
  a[2][1] = -sin(theta)*cos(phi);
  a[2][2] = cos(theta);
  
  //x0 is a four vector
  G4double x[3]; x[0]=x[1]=x[2]=0;
  for(int i=0;i<3;i++){
    x[0] += x0[i+1]*a[0][i];
    x[1] += x0[i+1]*a[1][i];
    x[2] += x0[i+1]*a[2][i];
  }

  x0[1]=x[0];
  x0[2]=x[1];
  x0[3]=x[2];

}
