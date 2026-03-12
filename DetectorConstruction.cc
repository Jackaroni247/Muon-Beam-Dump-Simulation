#include "DetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "DetectorMessenger.hh"

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction(), logicDump(nullptr) {
    fMessenger = new DetectorMessenger(this); 
}

DetectorConstruction::~DetectorConstruction() {
    delete fMessenger;
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    G4NistManager* nist = G4NistManager::Instance();
    
    G4Element* elFe = nist->FindOrBuildElement("Fe"); // Iron
    G4Element* elMn = nist->FindOrBuildElement("Mn"); // Manganese
    G4Element* elC = nist->FindOrBuildElement("C"); // Carbon
    G4Element* elSi = nist->FindOrBuildElement("Si"); // Silicon
    G4Element* elAl = nist->FindOrBuildElement("Al"); // Aluminium
    G4Element* elW = nist->FindOrBuildElement("W"); // Tungsten
    G4Element* elCu = nist->FindOrBuildElement("Cu"); // Copper
    G4Element* elNi = nist->FindOrBuildElement("Ni"); // Nickel

    G4double AHSSDensity = 7.87 * g/cm3;
    G4Material* customSteel = new G4Material("AHSS_Steel", AHSSDensity, 5);

    customSteel->AddElement(elFe, 0.97085);
    customSteel->AddElement(elMn, 0.0175);
    customSteel->AddElement(elC, 0.00105);
    customSteel->AddElement(elSi, 0.01);
    customSteel->AddElement(elAl, 0.0006);
    
    G4double CTADensity = 13.25 * g/cm3;
    G4Material* copperTungstenAlloy = new G4Material("CTA", CTADensity, 2);
    
    copperTungstenAlloy->AddElement(elW,0.8);
    copperTungstenAlloy->AddElement(elCu,0.2);
    
    G4double TNIDensity = 17.51 * g/cm3;
    G4Material* tungstennickelironAlloy = new G4Material("TNI", TNIDensity, 3);
    
    tungstennickelironAlloy->AddElement(elFe,0.02);
    tungstennickelironAlloy->AddElement(elNi,0.06);
    tungstennickelironAlloy->AddElement(elW,0.92);

    
    G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");
    G4Material* dump_mat = nist->FindOrBuildMaterial("G4_Pb");
    
    // World Volume
    G4Box* solidWorld = new G4Box("World", 10*m, 10*m, 20*m);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, true);

    // Beam Dump Volume
    G4Box* solidDump = new G4Box("BeamDump", 10*m, 10*m, 1*m);
    logicDump = new G4LogicalVolume(solidDump, dump_mat, "BeamDump");
    
    //Beam Placenebt
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicDump, "BeamDump", logicWorld, false, 0, true);

    return physWorld;
}

void DetectorConstruction::SetDumpMaterial(G4String materialName) {
    G4NistManager* nist = G4NistManager::Instance();
    G4Material* newMaterial = nist->FindOrBuildMaterial(materialName);
    if (newMaterial && logicDump) {
        logicDump->SetMaterial(newMaterial);
        G4RunManager::GetRunManager()->PhysicsHasBeenModified();
    }
}
