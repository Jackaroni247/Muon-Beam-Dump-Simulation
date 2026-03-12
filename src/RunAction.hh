#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"

class G4Run;

class RunAction : public G4UserRunAction {
  public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    void AddMuonBefore() { fMuonsBeforeDump += 1; }
    void AddMuonAfter()  { fMuonsAfterDump += 1; }
    void AddExitEnergy(G4double energy) { fTotalExitEnergy += energy; }

  private:
    G4Accumulable<G4int> fMuonsBeforeDump;
    G4Accumulable<G4int> fMuonsAfterDump;
    G4Accumulable<G4double> fTotalExitEnergy;
};
#endif
