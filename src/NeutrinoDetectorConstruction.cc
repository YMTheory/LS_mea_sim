#include "NeutrinoDetectorConstruction.hh"
//#include "NeutrinoDetectorSD.hh"
#include "NeutrinoPMTSD.hh"

#include "HamamatsuR12860PMTManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "OpticalProperty.icc"
#include "G4LogicalBorderSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "CLHEP/Units/PhysicalConstants.h"
#include "G4SDManager.hh"
#include "G4PhysicalVolumeStore.hh"


NeutrinoDetectorConstruction::NeutrinoDetectorConstruction() :
    G4VUserDetectorConstruction(),
    solidWorld(NULL), logicWorld(NULL), physWorld(NULL),
    solidDetector(NULL), logicDetector(NULL), physDetector(NULL),
    solidTube(NULL), logicTube(NULL), physTube(NULL),
    solidLS(NULL), logicLS(NULL), physLS(NULL),
    pmt_manager(NULL),
    m_Alsurface_finish("polished"),
    coeff_abslen(2.862), coeff_rayleigh(0.643), coeff_efficiency(1.0),
    m_optical_model("old"),
    m_c0_PPO(2.5), m_c0_bisMSB(1),
    m_c_PPO(2.5), m_c_bisMSB(1),
    fCheckOverlaps(false)
{
    pmt_manager = new HamamatsuR12860PMTManager("HamamatsuPMT"); 

    theMessenger = new NeutrinoDetectorConstructionMessenger(this);
}

NeutrinoDetectorConstruction::~NeutrinoDetectorConstruction()
{
    if (pmt_manager) {
        delete pmt_manager;
    }
}

G4VPhysicalVolume* NeutrinoDetectorConstruction::Construct()
{
    // initialize materials
    ConstructMaterials();

    // world volume
    ConstructWorld();

    // detector
    ConstructDetector();

    // print info
    GetAllGeometryInfo();

    // return physical world
    return physWorld;
}

void NeutrinoDetectorConstruction::ConstructWorld()
{
    G4double world_x = 20.*m, world_y = 20.*m, world_z = 20.*m;
    
    solidWorld = new G4Box( "solidWorld", 
                            0.5*world_x, 
                            0.5*world_y, 
                            0.5*world_z );
    
    logicWorld = new G4LogicalVolume( solidWorld,
                                      air,
                                      "logicWorld" );
    
    physWorld  = new G4PVPlacement( 0,
                                    G4ThreeVector(),
                                    logicWorld,
                                    "physWorld",
                                    0,
                                    false,
                                    0,
                                    fCheckOverlaps );
}

void NeutrinoDetectorConstruction::ConstructDetector()
{
    //// Air - glass
    //G4OpticalSurface* OpticalAirHam = new G4OpticalSurface("AirHamface");
    //OpticalAirHam -> SetModel(unified);
    //OpticalAirHam -> SetType(dielectric_dielectric);
    //OpticalAirHam -> SetFinish(ground);
    //G4MaterialPropertiesTable* AirHamMPT = new G4MaterialPropertiesTable();
    //OpticalAirHam -> SetMaterialPropertiesTable(AirHamMPT);

    //// Air - LS
    //G4OpticalSurface* OpticalAirLS = new G4OpticalSurface("AirLSface");
    //OpticalAirLS -> SetModel(unified);
    //OpticalAirLS -> SetType(dielectric_dielectric);
    //OpticalAirLS -> SetFinish(ground);
    //G4MaterialPropertiesTable* AirLSMPT = new G4MaterialPropertiesTable();
    //OpticalAirLS -> SetMaterialPropertiesTable(AirLSMPT);

    // Set photocathode optical surface:
    G4OpticalSurface* opticalCathodeSurface = new G4OpticalSurface("cathodeSurface");
    opticalCathodeSurface->SetType(dielectric_metal);
    opticalCathodeSurface->SetMaterialPropertiesTable(G4Material::GetMaterial("photocathode_Ham20inch")->GetMaterialPropertiesTable());
    

    // Set optical surface properties for Al as skin surface.
    G4OpticalSurface* opticalAlSurface = new G4OpticalSurface("AlSurface");
    opticalAlSurface->SetType(dielectric_metal); 
    opticalAlSurface->SetModel(unified); 

    if (m_Alsurface_finish == "polished") {
        G4cout << "The inner surface of Al tube (inner radius = "<< m_inner_tube_radius << ") is polished finish with flat reflectivity " << m_Al_ref << G4endl;
        opticalAlSurface->SetFinish(polished); 
        opticalAlSurface->SetPolish(0.9); 
        G4double photonEnergy[2] = {1.55*eV, 15.5*eV};
        G4double reflectivity[2] = {m_Al_ref, m_Al_ref};
        G4int numEntries = 2;
        G4MaterialPropertiesTable* materialPropertiesTable = new G4MaterialPropertiesTable();
        materialPropertiesTable->AddProperty("REFLECTIVITY", photonEnergy, reflectivity, numEntries);
        opticalAlSurface->SetMaterialPropertiesTable(materialPropertiesTable);
    }

    else if (m_Alsurface_finish == "ground") {
        G4cout << "The inner surface of Al tube (inner radius = "<< m_inner_tube_radius << ") is ground finish with flat reflectivity " << m_Al_ref << G4endl;
        opticalAlSurface->SetFinish(ground);
        G4double sigma_alpha = 6.*deg;
        opticalAlSurface->SetSigmaAlpha(sigma_alpha);
        const G4int numEntries = 2;
        G4double photonEnergy[numEntries] = {1.55*eV, 15.5*eV};
        G4double reflectivity[numEntries] = {m_Al_ref, m_Al_ref};
        G4double specularSpike[numEntries] = {0., 0.};
        G4double specularLobe[numEntries] = {0., 0.};
        G4double backScattering[numEntries] = {0., 0.};
        G4MaterialPropertiesTable* materialPropertiesTable = new G4MaterialPropertiesTable();
        materialPropertiesTable->AddProperty("REFLECTIVITY", photonEnergy, reflectivity, numEntries);
        materialPropertiesTable->AddProperty("SPECULARSPIKECONSTANT", photonEnergy, specularSpike, numEntries);
        materialPropertiesTable->AddProperty("SPECULARLOBECONSTANT", photonEnergy, specularLobe, numEntries);
        materialPropertiesTable->AddProperty("BACKSCATTERCONSTANT", photonEnergy, backScattering, numEntries);
        opticalAlSurface->SetMaterialPropertiesTable(materialPropertiesTable);
    }

    else {
        G4cout << "No such finish type -> " << m_Alsurface_finish << G4endl;
        assert(0);
    }

    G4double tube_length = 17.*m;
    G4double tube_inner_radius = m_inner_tube_radius;
    G4double tube_outer_radius = tube_inner_radius + 0.5*cm;
    G4double pi = 3.141592653;
    solidTube = new G4Tubs("solidTube",
                            tube_inner_radius,
                            tube_outer_radius,
                            tube_length/2.,
                            0.*deg,
                            360.*deg);

    logicTube = new G4LogicalVolume(solidTube,
                                    Aluminium,
                                    "logicTube");

    physTube = new G4PVPlacement(0,
                                 G4ThreeVector(),
                                 logicTube,
                                 "physTube",
                                 logicWorld,
                                 false,
                                 0,
                                 fCheckOverlaps);

    G4LogicalSkinSurface* skinSurface = new G4LogicalSkinSurface("surfaceName", logicTube, opticalAlSurface);

    solidLS = new G4Tubs(   "solidLS",
                            0.*cm,
                            tube_inner_radius,
                            tube_length/2.,
                            0.*deg,
                            360.*deg);

    //solidLS = new G4Box("solidLS", 2.*cm, 2.*cm, 2.*cm);
    logicLS = new G4LogicalVolume(  solidLS,
                                    LS,
                                    "logicLS");

    physLS = new G4PVPlacement(  0,
                                 G4ThreeVector(),
                                 logicLS,
                                 "physLS",
                                 logicWorld,
                                 false,
                                 0,
                                 fCheckOverlaps);


    // PMT construction:
    pmt_manager -> init();
    logicPMT    = pmt_manager->getLV();
    
    ///G4double pmt_height = 10.*cm;
    ///G4double pmt_outer_radius = 25.*cm;
    ///solidPMT = new G4Tubs(  "solidPMT",
    ///                        0.*cm,
    ///                        pmt_outer_radius,
    ///                        pmt_height/2.,
    ///                        0,
    ///                        2*pi
    ///                     );
    ///logicPMT = new G4LogicalVolume( solidPMT,
    ///                                //Photocathode_mat_Ham20inch,
    ///                                //air,
    ///                                vacuum,
    ///                                "logicPMT"
    ///                              );
    ///G4LogicalSkinSurface* cathodeSkinSurface = new G4LogicalSkinSurface("cathodeSurfaceName", logicPMT, opticalCathodeSurface);
    

    G4RotationMatrix* rot = new G4RotationMatrix();
    rot->rotateX(180.*deg);
    physPMT1    = new G4PVPlacement(rot,
                                    G4ThreeVector(0, 0, 8700.*mm),
                                    logicPMT,
                                    "physPMT1",
                                    logicWorld,
                                    false,
                                    0,
                                    fCheckOverlaps
                                    );
    //physPMT2    = new G4PVPlacement(0,
    //                                G4ThreeVector(0, 0, -9.*m),
    //                                logicPMT,
    //                                "physPMT1",
    //                                logicWorld,
    //                                false,
    //                                0,
    //                                fCheckOverlaps
    //                                );


    //G4LogicalBorderSurface* optical_airham1 = new G4LogicalBorderSurface("optical_airham1", physPMT1, physWorld, OpticalAirHam);
    //G4LogicalBorderSurface* optical_airham2 = new G4LogicalBorderSurface("optical_airham2", physPMT2, physWorld, OpticalAirHam);
    //G4LogicalBorderSurface* optical_airls  = new G4LogicalBorderSurface("optical_airls",  physLS,   physWorld, OpticalAirLS);

}

void NeutrinoDetectorConstruction::ConstructMaterials()
{
    ModifyOpticalProperty();

    G4cout << ">>> *************************** <<< \n"
           << " PPO c0 [g/L]     bisMSB c0 [mg/L] \n"
           << "   " << m_c0_PPO << "               " << m_c0_bisMSB << "\n"
           << " PPO c  [g/L]     bisMSB c  [mg/L] \n"
           << "   " << m_c_PPO << "               " << m_c_bisMSB << "\n";

    // elements
    G4Element* H  = new G4Element("Hydrogen", "H" , 1., 1.01*g/mole);
    G4Element* O  = new G4Element("Oxygen", "O", 8., 16.00*g/mole); 
    G4Element* B  = new G4Element("Boron",  "B", 5, 10.811*g/mole);
    G4Element* Na = new G4Element("Sodium", "Na", 11., 22.98977*g/mole);
    G4Element* Si = new G4Element("Silicon", "Si", 14., 28.09*g/mole);
    G4Element* Ar = new G4Element("Argon", "Ar", 18, 39.948*g/mole);
    G4Element* C  = new G4Element("Carbon", "C" , 6., 12.01*g/mole); 
    G4Element* N  = new G4Element("Nitrogen", "N", 7., 14.01*g/mole);
    G4Element* K  = new G4Element("Potassium", "K", 19., 39.0983*g/mole);
    G4Element* Fe = new G4Element("Iron", "Fe", 26., 55.845*g/mole);
    G4Element* Mn = new G4Element("Mn", "Mn", 25., 54.9381*g/mole);
    G4Element* P  = new G4Element("Phosphorus", "P", 15, 30.9738*g/mole);
    G4Element* S  = new G4Element("Sulfur", "S", 16., 32.066*g/mole);
    G4Element* Cr = new G4Element("Cr", "Cr", 24, 51.9961*g/mole);
    G4Element* Ni = new G4Element("Ni", "Ni", 28, 58.6934*g/mole);
    G4Element* Al = new G4Element("Aluminium", "Al", 13., 26.89*g/mole);
    
    G4Element* TS_C_of_Graphite = new G4Element("TS_C_of_Graphite", "C_GRAPHITE" , 6., 12.01*g/mole);
    G4Element* TS_H_of_Water = new G4Element("TS_H_of_Water", "H_WATER" , 1., 1.01*g/mole);

    // materials
    // water
    G4double density = 1.000*g/cm3;
    water = new G4Material("Water", density, 2);
    water->AddElement(H, 2);
    water->AddElement(O, 1);
    G4MaterialPropertiesTable* WaterMPT = new G4MaterialPropertiesTable();
    WaterMPT->AddProperty("RINDEX", fPP_Water_RIN, fWaterRINDEX,36);
    double water_abslen_scale_factor = 90.*m/(2651.815*cm);
    for (int j = 0; j < 316; ++j) {
        fWaterABSORPTION[j] *= water_abslen_scale_factor;
    }
    WaterMPT->AddProperty("ABSLENGTH", fPP_Water_ABS,fWaterABSORPTION, 316);
    water->SetMaterialPropertiesTable(WaterMPT);

    // pyrex
    density = 2.23*g/cm3;
    G4Material* SiO2 = new G4Material("SiO2", density, 2);
    SiO2->AddElement(Si, 1);
    SiO2->AddElement(O , 2);

    density = 2.23*g/cm3;
    G4Material* B2O2 = new G4Material("B2O2", density, 2);
    B2O2->AddElement(B,  2);
    B2O2->AddElement(O,  2);

    density = 2.23*g/cm3;
    G4Material* Na2O = new G4Material("Na2O", density, 2);
    Na2O->AddElement(Na, 2);
    Na2O->AddElement(O,  1);
    
    density = 2.23*g/cm3;
    pyrex = new G4Material("Pyrex", density, 3);
    pyrex->AddMaterial(SiO2, .80);
    pyrex->AddMaterial(B2O2, .13);
    pyrex->AddMaterial(Na2O, .07);
    G4MaterialPropertiesTable* PyrexMPT = new G4MaterialPropertiesTable();
    PyrexMPT->AddProperty("RINDEX", fPP_Pyrex, fPyrexRINDEX, 6);
    PyrexMPT->AddProperty("ABSLENGTH", fPP_PyrexABS, fPyrexABSORPTION, 9);
    pyrex->SetMaterialPropertiesTable(PyrexMPT);

    // steel
    density = 8.1*g/cm3;
    steel = new G4Material("Steel", density, 8);
    steel->AddElement(Fe, 0.70845);
    steel->AddElement(C, 0.0008);
    steel->AddElement(Mn, 0.02);
    steel->AddElement(P, 0.00045);
    steel->AddElement(S, 0.00030);
    steel->AddElement(Si, 0.01);
    steel->AddElement(Cr, 0.18);
    steel->AddElement(Ni, 0.08);
    G4MaterialPropertiesTable* SteelMPT = new G4MaterialPropertiesTable();
    SteelMPT->AddProperty("ABSLENGTH", fPP_SteelTank, fSteelTankABSORPTION, 4);
    steel->SetMaterialPropertiesTable(SteelMPT);

    // Aluminium
    density = 2.7*g/cm3;
    Aluminium = new G4Material("Aluminium", density, 1);
    Aluminium->AddElement(Al, 1);

    // air
    density = 1.205e-3*g/cm3;
    air = new G4Material("Air", density, 4);
    air->AddElement(N, 0.7550);
    air->AddElement(O, 0.2321);
    air->AddElement(Ar, 0.0128);
    air->AddElement(C, 0.0001);
    G4double airPP[2] =
    {
        1.55*eV, 6.20*eV
    };
    G4double airRINDEX[2] =
    {
        1.000273, 1.000273
    };
    G4double airABSLENGTH[2] =
    {
        10000*m, 10000*m
    };
    G4MaterialPropertiesTable* airMPT = new G4MaterialPropertiesTable();
    airMPT->AddProperty("RINDEX",    airPP, airRINDEX,     2);
    airMPT->AddProperty("ABSLENGTH", airPP, airABSLENGTH,  2);
    air->SetMaterialPropertiesTable(airMPT);

    // vacuum
    density     =  1e-3 * CLHEP::kGasThreshold;
    G4double temperature = CLHEP::STP_Temperature;
    G4double pressure    = CLHEP::STP_Pressure * density / (1.29e-3*g/cm3);
    vacuum = new G4Material("Vacuum", density, 1, kStateGas,temperature,pressure);
    vacuum->AddMaterial(air, 1.);
    G4MaterialPropertiesTable* VacuumMPT = new G4MaterialPropertiesTable();
    G4double VacuumPP[2] = 
    {
        1.55*eV, 15.50*eV
    };
    G4double VacuumRINDEX[2] = 
    {
        1.000, 1.000
    };
    VacuumMPT->AddProperty("RINDEX", VacuumPP, VacuumRINDEX, 2);
    vacuum->SetMaterialPropertiesTable(VacuumMPT);

    // hamamatsu pmt photocathode
    density = 5. *g/cm3; // true??
    Photocathode_mat_Ham20inch = new G4Material("photocathode_Ham20inch",density,1);
    Photocathode_mat_Ham20inch->AddElement(K, 1);
    G4MaterialPropertiesTable* PhotocathodeMPT_Ham20inch = new G4MaterialPropertiesTable();
    PhotocathodeMPT_Ham20inch->AddProperty("RINDEX", fPP_PhC_NEW, fPhCRINDEX_NEW, 25);
    PhotocathodeMPT_Ham20inch->AddProperty("KINDEX", fPP_PhC_NEW, fPhCKINDEX_NEW, 25);
    PhotocathodeMPT_Ham20inch->AddProperty("REFLECTIVITY", fPP_PhC, fPhCREFLECTIVITY, 4);
    PhotocathodeMPT_Ham20inch->AddProperty("EFFICIENCY", fPP_PhCQE_Dynode20inch, fPhCEFFICIENCY_Dynode20inch, 43);
    //PhotocathodeMPT_Ham20inch->AddProperty("THICKNESS", fPosZ, fTHICKNESS, 2);
    Photocathode_mat_Ham20inch->SetMaterialPropertiesTable(PhotocathodeMPT_Ham20inch);

    // mcp-pmt photocathode
    density = 5. *g/cm3; // true??
    Photocathode_mat_MCP20inch = new G4Material("photocathode_MCP20inch",density,1);
    Photocathode_mat_MCP20inch->AddElement(K, 1);
    G4MaterialPropertiesTable* PhotocathodeMPT_MCP20inch = new G4MaterialPropertiesTable();
    PhotocathodeMPT_MCP20inch->AddProperty("RINDEX", fPP_PhC_NEW, fPhCRINDEX_NEW, 25);
    PhotocathodeMPT_MCP20inch->AddProperty("KINDEX", fPP_PhC_NEW, fPhCKINDEX_NEW, 25);
    PhotocathodeMPT_MCP20inch->AddProperty("REFLECTIVITY", fPP_PhC, fPhCREFLECTIVITY, 4);
    PhotocathodeMPT_MCP20inch->AddProperty("EFFICIENCY", fPP_PhCQE_AverageMCP20inch, fPhCEFFICIENCY_AverageMCP20inch, 43);
    PhotocathodeMPT_MCP20inch->AddProperty("THICKNESS", fPosZ, fTHICKNESS, 2);
    Photocathode_mat_MCP20inch->SetMaterialPropertiesTable(PhotocathodeMPT_MCP20inch);

    // liquid scintillator
    density = 0.859*g/cm3;
    LS = new G4Material("LS", density, 5);
    LS->AddElement(TS_C_of_Graphite,  0.87924);
    LS->AddElement(TS_H_of_Water,  0.1201);
    LS->AddElement(O,  0.00034);
    LS->AddElement(N,  0.00027);
    LS->AddElement(S,  0.00005);

    G4MaterialPropertiesTable* LSMPT = new G4MaterialPropertiesTable();
    if (m_optical_model == "old") {
        G4cout << "--> Set LS absorption length in old optical model !!!" << G4endl;
        LSMPT->AddProperty("ABSLENGTH", fPP_ABSLENGTH_v1, LS_ABSLENGTH_v1, 497);
    }
    else if (m_optical_model == "new") {
        G4cout << "--> Set LAB absorption length in new optical model !!!" << G4endl;
        LSMPT->AddProperty("ABSLENGTH", fPP_ABSLENGTH_v2, LS_ABSLENGTH_v2, 428);
    }
    LSMPT->AddProperty("RINDEX", fPP_RINDEX, LS_RINDEX, 18);
    LSMPT->AddProperty("FASTCOMPONENT", fPP_FASTCOMPONENT, LS_FASTCOMPONENT, 275);
    LSMPT->AddProperty("SLOWCOMPONENT", fPP_SLOWCOMPONENT, LS_SLOWCOMPONENT, 275);
    LSMPT->AddProperty("REEMISSIONPROB", fPP_REEMISSIONPROB, LS_REEMISSIONPROB, 28);
    LSMPT->AddProperty("RAYLEIGH", fPP_RAYLEIGH, LS_RAYLEIGH, 11);
    LSMPT->AddProperty("OpticalCONSTANT", fPP_OpticalCONSTANT, LS_OpticalCONSTANT, 1);
    LSMPT->AddProperty("GammaCONSTANT", fPP_GammaCONSTANT, LS_GammaCONSTANT, 4);
    LSMPT->AddProperty("AlphaCONSTANT", fPP_AlphaCONSTANT, LS_AlphaCONSTANT, 4);
    LSMPT->AddProperty("NeutronCONSTANT", fPP_NeutronCONSTANT, LS_NeutronCONSTANT, 4);
    LSMPT->AddProperty("PPOABSLENGTH", fPP_PPOABSLENGTH, LS_PPOABSLENGTH, 770);
    LSMPT->AddProperty("PPOREEMISSIONPROB", fPP_PPOREEMISSIONPROB, LS_PPOREEMISSIONPROB, 15);
    LSMPT->AddProperty("PPOCOMPONENT", fPP_PPOCOMPONENT, LS_PPOCOMPONENT, 200);
    LSMPT->AddProperty("PPOTIMECONSTANT", fPP_PPOTIMECONSTANT, LS_PPOTIMECONSTANT, 2);
    LSMPT->AddProperty("bisMSBABSLENGTH", fPP_bisMSBABSLENGTH, LS_bisMSBABSLENGTH, 375);
    LSMPT->AddProperty("bisMSBREEMISSIONPROB", fPP_bisMSBREEMISSIONPROB, LS_bisMSBREEMISSIONPROB, 23);
    LSMPT->AddProperty("bisMSBCOMPONENT", fPP_bisMSBCOMPONENT, LS_bisMSBCOMPONENT, 275);
    LSMPT->AddProperty("bisMSBTIMECONSTANT", fPP_bisMSBTIMECONSTANT, LS_bisMSBTIMECONSTANT, 2);

    float ScintillationYield = 11522./MeV;
    if (m_optical_model == "new") { ScintillationYield = 11158; }
    float ResolutionScale = 1.0;
    LSMPT->AddConstProperty("SCINTILLATIONYIELD", ScintillationYield);
    LSMPT->AddConstProperty("RESOLUTIONSCALE", ResolutionScale);

    LS->SetMaterialPropertiesTable(LSMPT);

    // black construction
    G4int compNum = 0;
    G4int nEntries = 2;
    G4String name;
    G4double photonEnergy[2] = {1.55*eV, 15.5*eV};
    G4double rindex[2] = {1.0, 1.0};
    G4double RayLength[2] = {10000*m, 10000*m};
    G4double Abs[2] = {1.0, 1.0};
    black = new G4Material(name="black", density=2.0*g/cm3, compNum=1);
    G4MaterialPropertiesTable* black_mpt = new G4MaterialPropertiesTable();
    black_mpt -> AddProperty("RINDEX", photonEnergy, rindex, nEntries);
    black_mpt -> AddProperty("RAYLEIGH", photonEnergy, RayLength, nEntries);
    black_mpt -> AddProperty("theAbsorption", photonEnergy, Abs, nEntries);
    black -> SetMaterialPropertiesTable(black_mpt);
}



void NeutrinoDetectorConstruction::ConstructSDandField()
{
    G4cout << " -----> Construct Sensitive Detector \n" << G4endl;
    
    // simple sensitive Detector
    //auto detectorSD = new NeutrinoDetectorSD("detectorSD", "PmtHitsCollection");
    //G4SDManager::GetSDMpointer()->AddNewDetector(detectorSD);
    ////SetSensitiveDetector("logicPMT", detectorSD);
    //SetSensitiveDetector("HamamatsuPMT_body_log", detectorSD);
    //SetSensitiveDetector("HamamatsuPMT_inner1_log", detectorSD);

    // juno PMT like SD:
    auto detectorSD = new NeutrinoPMTSD("detectorSD", "PmtHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(detectorSD);
    SetSensitiveDetector("HamamatsuPMT_inner1_log", detectorSD);
    SetSensitiveDetector("HamamatsuPMT_body_log", detectorSD);
}


void NeutrinoDetectorConstruction::GetAllGeometryInfo() {
    G4PhysicalVolumeStore* volumeStore = G4PhysicalVolumeStore::GetInstance();
    G4int numVolumes = volumeStore->size();

    for (G4int i = 0; i < numVolumes; ++i) {
        G4VPhysicalVolume* volume = (*volumeStore)[i];
        G4double phys_x = volume->GetTranslation().getX();
        G4double phys_y = volume->GetTranslation().getY();
        G4double phys_z = volume->GetTranslation().getZ();
        G4LogicalVolume* logicalVolume = volume->GetLogicalVolume();

        G4String volumeName = logicalVolume->GetName();
        G4String materialName = logicalVolume->GetMaterial()->GetName();

        G4cout << "Volume Name: " << volumeName << G4endl;
        G4cout << "Material Name: " << materialName << G4endl;
        G4cout << "[ " << phys_x << ", " << phys_y << ", " << phys_z << " ]" << G4endl;
        G4cout << G4endl;
    }
}

void NeutrinoDetectorConstruction::ModifyOpticalProperty()
{
    for (int i=0; i<497; i++) {
        //GdLSABSLength[i] *= coeff_abslen;
        LS_ABSLENGTH_v1[i] *= coeff_abslen;
    }

    for (int i=0; i<11; i++) {
        GdLSRayLength[i] *= coeff_rayleigh;
    }

    for (int i=0; i<43; i++) {
        fPhCEFFICIENCY_Dynode20inch[i] *= coeff_efficiency;
    }

    for (int i=0; i<200; i++) {
        LS_PPOABSLENGTH[i] *=  1/0.0187;
    }

    for (int i=0; i<770; i++) {
        double scale = m_c0_PPO / m_c_PPO;
        LS_PPOABSLENGTH[i] *= scale;
    }

    for (int i=0; i<375; i++) {
        double scale = m_c0_bisMSB / m_c_bisMSB;
        LS_bisMSBABSLENGTH[i] *= scale;
    }
}

