#ifndef HamamatsuR12860PMTManager_hh
#define HamamatsuR12860PMTManager_hh

#include "globals.hh"
#include "G4Tubs.hh"
#include "G4VSolid.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VSensitiveDetector.hh"
#include "G4OpticalSurface.hh"


class Hamamatsu_R12860_PMTSolid;

class HamamatsuR12860PMTManager
{
    public:
        G4LogicalVolume*        getLV();
        G4double                GetPMTRadius();
        G4double                GetPMTHeight();
        G4double                GetZEquator();
        G4ThreeVector           GetPosInPMT();

    public:
        HamamatsuR12860PMTManager( const G4String& plabel );
        ~HamamatsuR12860PMTManager();

    private:
        //void init();
        void init_material();
        void init_mirror_surface();
        void init_pmt();
        void init_variables();

        void ConstructPMT_UsingTorusStack();

        G4String GetName() { return m_label; }

    private:
        G4String m_label;

        Hamamatsu_R12860_PMTSolid* m_pmtsolid_maker;

        void helper_make_solid();
        // * pmt solid (a little big than body solid)
        //   * body solid
        //     + inner1
        //     + inner2
        G4VSolid* pmt_solid; 
        G4VSolid* body_solid;
        G4VSolid* inner_solid;
        G4VSolid* inner1_solid;
        G4VSolid* inner2_solid;
        G4Tubs* dynode_solid;
        G4double hh_dynode;
        G4double z_dynode;
        /* logical volumes */
        void helper_make_logical_volume();
        G4LogicalVolume* body_log;
        G4LogicalVolume* inner1_log;
        G4LogicalVolume* inner2_log;
        G4LogicalVolume* dynode_log;
        /* physical volumes */
        void helper_make_physical_volume();
        G4PVPlacement* body_phys;
        G4PVPlacement* inner1_phys;
        G4PVPlacement* inner2_phys;
        G4PVPlacement* dynode_phys;
        /* dynode volumes */
        void helper_make_dynode_volume();
        /* optical surface */
        void helper_make_optical_surface();
        /* visual attribute */
        void helper_vis_attr();

    private:
        G4LogicalVolume* m_logical_pmt;
    
        G4OpticalSurface* m_mirror_opsurf;
    
        G4OpticalSurface* Photocathode_opsurf;
        G4Material* GlassMat;
        G4Material* PMT_Vacuum;
        G4Material* DynodeMat;
        G4Material* MaskMat;
        G4VSensitiveDetector *m_detector;

    private:
        // Cover Related
        G4LogicalVolume* m_logical_cover;
    
        G4Material* m_cover_mat;
        bool m_simplify_csg ; 
        bool m_fast_cover;
        bool m_enable_optical_model;
        std::string m_cover_mat_str;
    private:
        G4double m_z_equator;
        G4double m_pmt_r;
        G4double m_pmt_h;
    
    public:
        void init();
        G4LogicalVolume* get_pmt_log()        { return m_logical_pmt;}
        G4LogicalVolume* get_body_log()       { return body_log; }
        G4LogicalVolume* get_inner1_log()     { return inner1_log; }
        G4LogicalVolume* get_inner2_log()     { return inner2_log; }
};

#endif


