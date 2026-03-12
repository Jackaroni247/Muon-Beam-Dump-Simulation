#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1
#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"

class DetectorConstruction;

class DetectorMessenger : public G4UImessenger {
  public:
    DetectorMessenger(DetectorConstruction* det);
    virtual ~DetectorMessenger();
    virtual void SetNewValue(G4UIcommand* command, G4String newValue);

  private:
    DetectorConstruction* fDetector;
    G4UIcmdWithAString* fMaterialCmd;
};
#endif
