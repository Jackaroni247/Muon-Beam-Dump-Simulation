#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* det) : G4UImessenger(), fDetector(det) {
    fMaterialCmd = new G4UIcmdWithAString("/dump/setMaterial", this);
    fMaterialCmd->SetGuidance("Select Material of the Beam Dump.");
    fMaterialCmd->SetParameterName("choice", false);
    fMaterialCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

DetectorMessenger::~DetectorMessenger() {
    delete fMaterialCmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
    if (command == fMaterialCmd) {
        fDetector->SetDumpMaterial(newValue);
    }
}
