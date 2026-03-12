#include "RunAction.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4AnalysisManager.hh"

RunAction::RunAction() : G4UserRunAction(),
  fMuonsBeforeDump(0), fMuonsAfterDump(0), fTotalExitEnergy(0.) {
    
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Register(fMuonsBeforeDump);
    accumulableManager->Register(fMuonsAfterDump);
    accumulableManager->Register(fTotalExitEnergy);
    
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetDefaultFileType("csv");

    analysisManager->CreateNtuple("EnergyDecay", "Muon Energy Profile");
    analysisManager->CreateNtupleIColumn("EventID");
    analysisManager->CreateNtupleDColumn("Z_Position");
    analysisManager->CreateNtupleDColumn("KineticEnergy");
    analysisManager->FinishNtuple();
}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run*) {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile("EnergyProfile.csv");
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Reset();
}

void RunAction::EndOfRunAction(const G4Run* run) {
    G4int nofEvents = run->GetNumberOfEvent();
    if (nofEvents == 0) return;

    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Merge();

    if (IsMaster()) {
        G4String materialName = "Unknown";
        G4LogicalVolume* dumpLV = G4LogicalVolumeStore::GetInstance()->GetVolume("BeamDump"); 
        if (dumpLV) materialName = dumpLV->GetMaterial()->GetName();

        G4int before = fMuonsBeforeDump.GetValue();
        G4int after = fMuonsAfterDump.GetValue();
        G4double energy = fTotalExitEnergy.GetValue();

        G4cout << "\n--------------------End of Run-----------------------" << G4endl;
        G4cout << " Target Material:            " << materialName << G4endl;
        G4cout << " Number of Events simulated: " << nofEvents << G4endl;
        G4cout << " Muons entering the dump:    " << before << G4endl;
        G4cout << " Muons exiting the dump:     " << after << G4endl;
        
        if (before > 0) {
            G4double transmission = (G4double)after / before * 100.0;
            G4cout << " Transmission rate:          " << transmission << "%" << G4endl;
        }

        if (after > 0) {
            G4double avgExitEnergy = energy / after;
            G4double avgEnergyLost = 3 * GeV - avgExitEnergy; 
            G4cout << " Avg Exit Energy:            " << G4BestUnit(avgExitEnergy, "Energy") << G4endl;
            G4cout << " Avg Energy Lost in Dump:    " << G4BestUnit(avgEnergyLost, "Energy") << G4endl;
        }
        G4cout << "-----------------------------------------------------" << G4endl;
        auto analysisManager = G4AnalysisManager::Instance();
        analysisManager->Write();
        analysisManager->CloseFile();
    }
}
