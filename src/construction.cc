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
    
    
    // 设置产生切伦科夫辐射的介质构成：气凝胶（aerogel）SiO2, H2O, C
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

    // 设置切伦科夫辐射相关参数，momentum动量、refractive index(?)折射率（要显示可见光光子，必须要设定材料的反射系数）
    G4double energy[2] = {1.239841939*eV/0.9, 1.239841939*eV/0.2};
    G4double rindexAerogel[2] = {1.1, 1.1};
    G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable();
    mptAerogel->AddProperty("RINDEX", energy, rindexAerogel, 2);
    Aerogel->SetMaterialPropertiesTable(mptAerogel);

    // 设置world的折射率，以显示光路
    G4double rindexWorld[2] = {1.0, 1.0};
    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
    mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);
    worldMat->SetMaterialPropertiesTable(mptWorld);

    // 辐射介质和world几何设置
    G4Box *solidWorld = new G4Box("solidWorld", 0.5*m, 0.5*m, 0.5*m); // solid world，一个0.5m*0.5m*0.5m的立方体
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld"); // logical world
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true); // physical world G4ThreeVector是中心点坐标

    G4Box *solidRadiator = new G4Box("solidRadiator", 0.4*m, 0.4*m, 0.01*m); // solid radiator
    G4LogicalVolume *logicRadiator = new G4LogicalVolume(solidRadiator, Aerogel, "logicRadiator"); // logical  radiator
    G4VPhysicalVolume *physRadiator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.2*m), logicRadiator, "physRadiator", logicWorld, false, 0, true); //  physical radiator

    // 探测器构建
    // 先定义solid空间，由于之后定义的灵敏体积要外部访问探测器的logical空间，因此需要在头文件中MyDetectorConstruction类中定义
    // 探测器的logical空间
    G4Box *solidDetector = new G4Box("solidDetector", 0.005*m, 0.005*m, 0.01*m); // solid detector
    logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");
    // 探测器阵列构建，使用for循环，构建一个100x100的探测器阵列
    for(G4int i = 0; i < 100; i++)
    {
        for(G4int j = 0; j < 100; j++)
        {
            G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(-0.5*m+(i+0.5)*m/100, -0.5*m+(j+0.5)*m/100, 0.49*m), logicDetector, "physDetector", logicWorld, false,  j+i*100, true);
        }
    }

    return physWorld;
}
