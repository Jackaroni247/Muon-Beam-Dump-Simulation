#include "SteppingAction.hh"
#include "RunAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4MuonMinus.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4Event.hh"

SteppingAction::SteppingAction() : G4UserSteppingAction() {}
SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) {
    G4Track* track = step->GetTrack();
    if (track->GetDefinition() != G4MuonMinus::MuonMinusDefinition()) return;

    G4StepPoint* prePoint = step->GetPreStepPoint();
    G4StepPoint* postPoint = step->GetPostStepPoint();

    G4VPhysicalVolume* postVolume = postPoint->GetPhysicalVolume();
    if (!postVolume) return; 

    G4String preName = prePoint->GetPhysicalVolume()->GetName();
    G4String postName = postVolume->GetName();

    RunAction* runAction = (RunAction*) G4RunManager::GetRunManager()->GetUserRunAction();

    if (preName == "BeamDump") {
       G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    
        G4double zPos = track->GetPosition().z();
        G4double energy = track->GetKineticEnergy();
  
        auto analysisManager = G4AnalysisManager::Instance();
        analysisManager->FillNtupleIColumn(0, eventID);
        analysisManager->FillNtupleDColumn(1, zPos);
        analysisManager->FillNtupleDColumn(2, energy);
       analysisManager->AddNtupleRow(); 
    }

    if (preName == "World" && postName == "BeamDump") {
        runAction->AddMuonBefore();
    }
    
    if (preName == "BeamDump" && postName == "World") {
        if (track->GetMomentumDirection().z() > 0) {
            runAction->AddMuonAfter();
            runAction->AddExitEnergy(track->GetKineticEnergy());
        }
    }
}
