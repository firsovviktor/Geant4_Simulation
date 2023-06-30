#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    G4NistManager *nist = G4NistManager::Instance();

    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
    G4Material *BGO = nist->FindOrBuildMaterial("G4_BGO");
    G4Material *Al = nist->FindOrBuildMaterial("G4_Al");

    //rindexBGO from Williams et al. 1996: n 0.305–1.0 µm
    G4double energy[8] = {1.2398*eV, 1.5005*eV, 2.0069*eV, 2.4817*eV, 2.9790*eV, 3.5053*eV, 3.9739*eV, 4.0651*eV};
    G4double abslBGO[8] = {5.2*cm, 5.2*cm, 5.2*cm, 5.2*cm, 5.2*cm, 5.2*cm, 5.2*cm, 5.2*cm};
    G4double rindexBGO[8] = {2.0560, 2.06849, 2.1009, 2.1428, 2.2016, 2.2857, 2.3870, 2.4105};
    G4double rindexWorld[8] = {1., 1., 1., 1., 1., 1., 1., 1.};      

    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
    mptWorld->AddProperty("RINDEX", energy, rindexWorld, 8);
    worldMat->SetMaterialPropertiesTable(mptWorld);    
    
    G4MaterialPropertiesTable *mptBGO = new G4MaterialPropertiesTable();
    mptBGO->AddProperty("RINDEX", energy, rindexBGO, 8);
    mptBGO->AddProperty("ABSLENGTH", energy, abslBGO, 8); 
    mptBGO->AddConstProperty("SCINTILLATIONYIELD", 8./keV);
    mptBGO->AddConstProperty("RESOLUTIONSCALE", 1.0);
    mptBGO->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 300.*ns);
    BGO->SetMaterialPropertiesTable(mptBGO);
    
    G4Box *solidWorld = new G4Box("solidWorld", 0.25*m, 0.25*m, 0.25*m);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);   

    G4Tubs *solidScintillator = new G4Tubs("solidScintillator", 0.0*m, 0.036*m, 0.011*m, 0., 2*CLHEP::pi);
    G4LogicalVolume *logicScintillator = new G4LogicalVolume(solidScintillator, BGO, "logicScintillator");
    G4VPhysicalVolume *physScintillator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicScintillator, "physScintillator", logicWorld, false, 0, true);

    
    std::vector<G4double> pp = {2.038*eV, 4.144*eV};
    //std::vector<G4double> specularlobe = {0.3, 0.3};
    //std::vector<G4double> specularspike = {0.2, 0.2};
    //std::vector<G4double> backscatter = {0.1, 0.1};
    std::vector<G4double> rindex = {1.35, 1.40};
    std::vector<G4double> reflectivity = {0.95, 0.95};
    //std::vector<G4double> efficiency = {0.8, 0.1};

    G4MaterialPropertiesTable* SMPT = new G4MaterialPropertiesTable();

    SMPT->AddProperty("RINDEX", pp, rindex);
    //SMPT->AddProperty("SPECULARLOBECONSTANT", pp, specularlobe);
    //SMPT->AddProperty("SPECULARSPIKECONSTANT", pp, specularspike);
    //SMPT->AddProperty("BACKSCATTERCONSTANT", pp, backscatter);
    SMPT->AddProperty("REFLECTIVITY", pp, reflectivity);
    //SMPT->AddProperty("EFFICIENCY", pp, efficiency);


/*
    //debug mirror
    G4Box *solidDebug = new G4Box("solidDebug", 0.10*m, 0.10*m, 0.005*m);
    G4LogicalVolume *logicDebug = new G4LogicalVolume(solidDebug, Al, "logicDebug");
    G4VPhysicalVolume *physDebug = new G4PVPlacement(0, G4ThreeVector(0., 0., -0.1*m), logicDebug, "physScintillator", logicWorld, false, 0, true);

    G4OpticalSurface* OpSurface = new G4OpticalSurface("name");

    G4LogicalSkinSurface* Surface = new
    G4LogicalSkinSurface("name",logicDebug,OpSurface);

    OpSurface->SetType(dielectric_dielectric);
    OpSurface->SetModel(unified);
    OpSurface->SetFinish(polishedbackpainted);
    OpSurface->SetMaterialPropertiesTable(SMPT);
    //debug mirror
*/


       
    //Setting up side mirror
    G4Tubs *solidMirror1 = new G4Tubs("solid", 0.0355*m, 0.036*m, 0.011*m, 0., 2*CLHEP::pi);
    G4LogicalVolume *logicMirror1 = new G4LogicalVolume(solidMirror1, Al, "logicMirror1");
    G4VPhysicalVolume *physMirror1 = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicMirror1, "physMirror1", logicScintillator, false, 0, true); 

    G4OpticalSurface* opMirror1Surface = new G4OpticalSurface("Mirror1Surface");

    G4LogicalSkinSurface* SurfaceMirror1 = new 
    G4LogicalSkinSurface("SurfaceMirror1", logicMirror1, opMirror1Surface);

    opMirror1Surface->SetType(dielectric_dielectric);
    opMirror1Surface->SetFinish(polishedbackpainted);
    opMirror1Surface->SetModel(unified);
    opMirror1Surface->SetMaterialPropertiesTable(SMPT);


    //Setting up base mirror
    G4Tubs *solidMirror2 = new G4Tubs("solid", 0.0*m, 0.0355*m, 0.00025*m, 0., 2*CLHEP::pi);
    G4LogicalVolume *logicMirror2 = new G4LogicalVolume(solidMirror2, Al, "logicMirror2");
    G4VPhysicalVolume *physMirror2 = new G4PVPlacement(0, G4ThreeVector(0., 0., -0.011*m+0.00025*m), logicMirror2, "physMirror2", logicScintillator, false, 0, true); 

    G4OpticalSurface* opMirror2Surface = new G4OpticalSurface("Mirror2Surface");

    G4LogicalSkinSurface* SurfaceMirror2 = new 
    G4LogicalSkinSurface("SurfaceMirror2", logicMirror2, opMirror2Surface);

    opMirror2Surface->SetType(dielectric_dielectric);
    opMirror2Surface->SetFinish(polishedbackpainted);
    opMirror2Surface->SetModel(unified);
    opMirror2Surface->SetMaterialPropertiesTable(SMPT);
    
    //PMT construction

    G4Tubs *solidDetector = new G4Tubs("SolidDetector", 0.0*m, 0.035*m, 0.05*m, 0., 2*CLHEP::pi);
    logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");
    G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(0., 0., (0.011+0.05+0.001)*m), logicDetector, "physDetector", logicWorld, false, 0, true);


    return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *sensDet = new 
    MySensitiveDetector("SensitiveDetector");

    logicDetector->SetSensitiveDetector(sensDet);
}
