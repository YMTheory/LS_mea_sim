#include "NeutrinoPMTSD.hh"
#include "NeutrinoDetectorHit.hh"
#include "NormalTrackInfo.hh"


#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4OpticalPhoton.hh"
#include "G4DataInterpolation.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4ProcessManager.hh"

using namespace CLHEP;

NeutrinoPMTSD::NeutrinoPMTSD( const G4String& name, 
                  const G4String& hitsCollectionName)
    : G4VSensitiveDetector(name),
    fHitsCollection(NULL)
{
    collectionName.insert(hitsCollectionName);

    analysis = NeutrinoAnalysis::Instance();
}

NeutrinoPMTSD::~NeutrinoPMTSD()
{;}


void NeutrinoPMTSD::Initialize(G4HCofThisEvent* hce)
{
    // Create hits collection
    fHitsCollection
        = new NeutrinoDetectorHitsCollection( SensitiveDetectorName, collectionName[0]);
    
    // Add this collection in hce
    G4int hcID 
        = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection( hcID, fHitsCollection );


    // Create hits
    //for( G4int i=0; i<fNofPmts+1; i++ )  {
    //    fHitsCollection->insert(new NeutrinoPMTHit());
    //}
}

G4bool NeutrinoPMTSD::ProcessHits( G4Step* aStep, G4TouchableHistory*)
{
    G4Track* track = aStep->GetTrack();
    //G4cout << "Processing hit of trackID " << track->GetTrackID()
    //       << " particle type " << track->GetDefinition()->GetParticleName()
    //       << G4endl;
    if (track->GetDefinition() != G4OpticalPhoton::Definition()) {
        return false;
    }
    G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint* postStepPoint = aStep->GetPostStepPoint();
    double edep = aStep->GetTotalEnergyDeposit();
    //G4cout << " --> with deposit energy " << edep << G4endl;
    if (edep <= 0.0) { return false; }

    G4bool isOnBoundary = ( postStepPoint->GetStepStatus() == fGeomBoundary);
    if (not isOnBoundary) {
        return false;
    }

    static G4ThreadLocal G4OpBoundaryProcess* boundary_proc=NULL;
    if (!boundary_proc) {
        G4ProcessManager* OpManager =
            G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
        if (OpManager) {
            G4int MAXofPostStepLoops =
                OpManager->GetPostStepProcessVector()->entries();
            G4ProcessVector* fPostStepDoItVector =
                OpManager->GetPostStepProcessVector(typeDoIt);
            for ( G4int i=0; i<MAXofPostStepLoops; i++) {
                G4VProcess* fCurrentProcess = (*fPostStepDoItVector)[i];
                G4OpBoundaryProcess* op =  dynamic_cast<G4OpBoundaryProcess*>(fCurrentProcess);
                if (op) {
                    boundary_proc = op;
                    break;
                }
            }
        }
     
    }

    if (!boundary_proc) {
        G4cout << "Can't locate OpBoundaryProcess." << G4endl;
        return false;
    }

   G4OpBoundaryProcessStatus theStatus = Undefined;
   theStatus = boundary_proc->GetStatus();

   if (theStatus != Detection) {
       return false;
   }

    const G4AffineTransform& trans = track->GetTouchable()->GetHistory()->GetTopTransform();
    const G4ThreeVector& global_pos = postStepPoint->GetPosition();
    G4ThreeVector local_pos = trans.TransformPoint(global_pos);

    //double qe       = 1.;
    //double qe_calc  = 1.; 
    //double eff_pde  = 0.3;  // Set an initial 30% PDE for this PMT
    //double eff_ce   = 1.;   // 0.872 in JUNO offline;
    //qe_calc = eff_pde / eff_ce;

    double ce = get_ce(local_pos);
    double de = ce;

    //double f_angle_response = 1.0;
    //// = final DE = QE * CE, but QE is already applied, so only CE is important.
    //// = DE: Detection Efficiency
    //double m_qescale = 1.0/0.8;
    //qe = qe_calc * m_qescale;

    //double de = qe * ce * f_angle_response;
    //G4cout << "The rough PDE is " << de << ", where CE = " << ce << G4endl;

    if (de > 1.0) {
        std::cout << "NeutrinoPMTSD: DE is larger than 1.0 !!!" << std::endl;
        return false;
    }

    bool de_cull = G4UniformRand() > de;

    if (de_cull) { 
        return false;
    }

    // ========================================================================
    // create the transient PMT Hit Object
    // ========================================================================
    double hittime = postStepPoint->GetGlobalTime();
    double wavelength = twopi*hbarc/edep;
    int m_num_scintillation     = 0;
    int m_num_rayleigh          = 0;
    int m_num_reflection        = 0;
    double wavelength_OP        = 0;
    //int m_flag_scintillation    = 0;
    //int m_flag_rayleigh         = 0;
    //int m_flag_reflection       = 0;
    int m_flag_cerenkov         = 0;
    int m_flag_OP               = 0;
    G4VUserTrackInformation* trkinfo = track->GetUserInformation();
    if(trkinfo) {
        NormalTrackInfo* normaltrk = dynamic_cast<NormalTrackInfo*>(trkinfo);
        if (normaltrk) {
            m_num_scintillation = normaltrk->getScintillationNumber();
            m_num_rayleigh      = normaltrk->getRayleighNumber();
            m_num_reflection    = normaltrk->getReflectionNumber();

            //G4bool flag_scint   = normaltrk->isReemission();
            G4bool flag_cer     = normaltrk->isFromCerenkov();
            //G4bool flag_ray     = normaltrk->isRayleigh();
            //G4bool flag_ref     = normaltrk->isReflection();
            //if (flag_scint) { m_flag_scintillation = 1;}
            if (flag_cer)   { m_flag_cerenkov = 1;}
            //if (flag_ray)   { m_flag_rayleigh = 1;}
            //if (flag_ref)   { m_flag_reflection = 1;}
            G4bool flag_OP      = normaltrk->isOriginalOP();
            if(flag_OP)         { m_flag_OP = 1;}

            double openergy = normaltrk->getOPenergy();
            wavelength_OP = twopi * hbarc / openergy;

            std::vector<float> one_ref_posx = normaltrk->getRefPosX();
            std::vector<float> one_ref_posy = normaltrk->getRefPosY();
            std::vector<float> one_ref_posz = normaltrk->getRefPosZ();
            //for (int iref=0; iref<one_ref_posx.size(); iref++) {
            //    G4cout << "Hit track reflection " << track->GetTrackID() << " " << iref << " " << one_ref_posx[iref] << " " << one_ref_posy[iref] << " " << one_ref_posz[iref] << G4endl;
            //}

        }
    }
    
    // store a G4Hit in SD:
    NeutrinoDetectorHit* hit = new NeutrinoDetectorHit();
    G4int trackId = track->GetTrackID();
    hit->SetTrackID(trackId);
    hit->SetTime(hittime);
    hit->SetEnergy(edep);
    hit->SetRefNum(m_num_reflection);
    hit->SetRayNum(m_num_rayleigh);
    hit->SetScintNum(m_num_scintillation);
    fHitsCollection->insert(hit);
    
    // save into root files
    analysis->analysePEwavelength_junoPMT(wavelength);
    analysis->analyseOPwavelength_junoPMT(wavelength_OP);
    analysis->analysePEtime_junoPMT(hittime);
    analysis->analysePERefNum_junoPMT(m_num_reflection);
    analysis->analysePERayNum_junoPMT(m_num_rayleigh);
    analysis->analysePEScintNum_junoPMT(m_num_scintillation);
    analysis->analyseIsOP(m_flag_OP);

    //analysis->analyseIsReflection(m_flag_reflection);
    //analysis->analyseIsRayleigh(m_flag_rayleigh);
    //analysis->analyseIsReemission(m_flag_scintillation);
    analysis->analyseIsCerenkov(m_flag_cerenkov);

    return true;
}

double NeutrinoPMTSD::get_ce(const G4ThreeVector local_pos)
{
    static double s_theta_hamamatsu[] = {
        0.*deg, 13.*deg, 28.*deg, 41.*deg, 55.*deg, 66.*deg,
        79.*deg, 85.*deg, 90.*deg,
    };
    static double s_ce_hamamatsu[] =    {
        0.911,    0.911,    0.9222,     0.9294,     0.9235,     0.93,
        0.9095, 0.6261, 0.2733, 
    };
    static G4DataInterpolation s_di(s_theta_hamamatsu, s_ce_hamamatsu, 9, 0., 0.);
    double theta = local_pos.theta();
    return s_di.CubicSplineInterpolation(theta);
}


void NeutrinoPMTSD::EndOfEvent(G4HCofThisEvent*)
{
    G4int nofHits = fHitsCollection->entries();
    analysis -> analyseTotalPENumber_junoPMT(nofHits);
}


