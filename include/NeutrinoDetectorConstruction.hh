#ifndef NeutrinoDetectorConstruction_h
#define NeutrinoDetectorConstruction_h 1

#include "globals.hh"
#include "G4VSolid.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VUserDetectorConstruction.hh"
#include "NeutrinoDetectorConstructionMessenger.hh"

class HamamatsuR12860PMTManager;

class NeutrinoDetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        NeutrinoDetectorConstruction();
        ~NeutrinoDetectorConstruction();
        
        // construct materials
        void ConstructMaterials();
        
        // construct the detector
        G4VPhysicalVolume* Construct();

        // construct SD
        virtual void ConstructSDandField();

        // construct world
        void ConstructWorld();

        // construct detector
        void ConstructDetector();

        void GetAllGeometryInfo();

    private:
        // materials
        G4Material* LS;
        G4Material* water;
        G4Material* air;
        G4Material* pyrex;
        G4Material* steel;
        G4Material* Aluminium;
        G4Material* vacuum;
        G4Material* Photocathode_mat_Ham20inch;
        G4Material* Photocathode_mat_MCP20inch;

        G4Material* black;
        
        // world
        G4VSolid*           solidWorld;
        G4LogicalVolume*    logicWorld;
        G4VPhysicalVolume*  physWorld;

        G4VSolid*           solidDetector;
        G4LogicalVolume*    logicDetector;
        G4VPhysicalVolume*  physDetector;

        // add in our case
        // LS-tub
        G4VSolid*           solidTube;
        G4LogicalVolume*    logicTube;
        G4VPhysicalVolume*  physTube;

        // LS
        G4VSolid*           solidLS;
        G4LogicalVolume*    logicLS;
        G4VPhysicalVolume*  physLS;

        // PMT-1
        G4VSolid*           solidPMT;
        G4LogicalVolume*    logicPMT;
        G4VPhysicalVolume*  physPMT1;

        // PMT-2
        G4VPhysicalVolume*  physPMT2;

        G4bool fCheckOverlaps;

        HamamatsuR12860PMTManager* pmt_manager;
    
    private:
        void ModifyOpticalProperty();
        double coeff_abslen;
        double coeff_efficiency;
        double coeff_rayleigh;

    private:
        NeutrinoDetectorConstructionMessenger* theMessenger;

        // configurations of inner tube surface:
        G4double m_Al_ref;
        G4String m_Alsurface_finish;
        G4double m_inner_tube_radius;
        G4String m_optical_model;

        // configuration of LS concentration
        G4double m_c0_PPO;
        G4double m_c_PPO;
        G4double m_c0_bisMSB;
        G4double m_c_bisMSB;

    public:
        void SetAlReflection(G4double val)      { m_Al_ref = val;}
        void SetTubeInnerRadius(G4double val)   { m_inner_tube_radius = val; }
        void SetAlSurfaceFinish(G4String val)   { m_Alsurface_finish=val;}
        void SetOpticalModel(G4String val)      { m_optical_model = val; }

        void SetPPOConcentration(G4double c)    { m_c_PPO = c; }
        void SetbisMSBConcentration(G4double c) { m_c_bisMSB = c;}

};  

#endif
