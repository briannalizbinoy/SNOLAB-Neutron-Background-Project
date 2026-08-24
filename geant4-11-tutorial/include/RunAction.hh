#ifndef RUNACTION_HH
#define RUNACTION_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

    

class RunAction : public G4UserRunAction {
public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
    // Counter for He-4 elastic events (from SteppingAction)
    void CountHe4Elastic() { fHeElasticCount++; }
    void CountNeutronEntry() { neutronEntryCount++; }
    static std::atomic<int> neutronEntryCount;

private:
    G4int fHeElasticCount = 0;
};

#endif


// class RunAction : public G4UserRunAction
// {
// public:
//     RunAction();
//     ~RunAction();

//     virtual void BeginOfRunAction(const G4Run *);
//     virtual void EndOfRunAction(const G4Run *);
// };
