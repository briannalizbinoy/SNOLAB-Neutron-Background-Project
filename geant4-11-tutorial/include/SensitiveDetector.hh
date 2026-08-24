#ifndef SENSITIVEDETECTOR_HH
#define SENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh" //records hits when a particle pases through detector

#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"


class SensitiveDetector : public G4VSensitiveDetector
{
public:
    SensitiveDetector(G4String);
    ~SensitiveDetector();
public:
    static G4int fLi6ReactionCount;
  

private:
    G4double fTotalEnergyDeposited;
    G4int fEventID;

    virtual void Initialize(G4HCofThisEvent *) override;
    virtual void EndOfEvent(G4HCofThisEvent *) override;

    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
};

#endif
