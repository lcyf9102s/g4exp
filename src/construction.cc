#include "construction.hh"


MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    // define materials
    G4NistManager *nist = G4NistManager::Instance();

    // World材质
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR"); // ROI(boundary) & material
    
    
    // 设置产生切伦科夫辐射的材料构成：气凝胶（aerogel）SiO2, H2O, C
    G4Material *SiO2 = new G4Material("SiO2", 2.201*g/cm3, 2);
    SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
    SiO2->AddElement(nist->FindOrBuildElement("O"), 2);

    G4Material *H2O = new G4Material("H2O", 1.000*g/cm3, 2);
    H2O->AddElement(nist->FindOrBuildElement("H"), 2);
    H2O->AddElement(nist->FindOrBuildElement("O"), 1);

    G4Element *C = nist->FindOrBuildElement("C");

    G4Material *Aerogel = new G4Material("Aerogel", 0.200*g/cm3, 3);
    Aerogel->AddMaterial(SiO2, 62.5*perCent);
    Aerogel->AddMaterial(H2O, 37.4*perCent);
    Aerogel->AddElement(C, 0.1*perCent);

    // 设置切伦科夫辐射相关参数，momentum动量、refractive index(?)反射系数（要显示可见光光子，必须要设定材料的反射系数）
    G4double energy[2] = {1.239841939*eV/0.9, 1.239841939*eV/0.2};
    G4double rindexAerogel[2] = {1.1, 1.1};
    G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable();
    mptAerogel->AddProperty("RINDEX", energy, rindexAerogel, 2);
    Aerogel->SetMaterialPropertiesTable(mptAerogel);

    // 设置world的反射系数，以显示光路
    G4double rindexWorld[2] = {1.0, 1.0};
    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
    mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);
    worldMat->SetMaterialPropertiesTable(mptWorld);

    // 几何设置
    G4Box *solidWorld = new G4Box("solidWorld", 0.5*m, 0.5*m, 0.5*m); // solid world
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld"); // logical world
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true); // physical world

    G4Box *solidRadiator = new G4Box("solidRadiator", 0.4*m, 0.4*m, 0.01*m);
    G4LogicalVolume *logicRadiator = new G4LogicalVolume(solidRadiator, Aerogel, "logicRadiator");
    G4VPhysicalVolume *physRadiator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.2*m), logicRadiator, "physRadiator", logicWorld, false, 0, true);


    return physWorld;
}
