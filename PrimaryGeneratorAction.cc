#include "PrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4MuonMinus.hh"
#include "G4SystemOfUnits.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction() {
    fParticleGun = new G4ParticleGun(1);
    
    // Set particle to Muon Minus
    G4ParticleDefinition* particle = G4MuonMinus::MuonMinusDefinition();
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.)); // Shoot +Z
    fParticleGun->SetParticleEnergy(3.*GeV);
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -10.*m)); // Start before dump
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
