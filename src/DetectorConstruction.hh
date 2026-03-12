#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1
#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4String.hh"

class DetectorMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction {
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    virtual G4VPhysicalVolume* Construct();
    void SetDumpMaterial(G4String materialName);

  private:
    G4LogicalVolume* logicDump;
    DetectorMessenger* fMessenger;
};
#endif
