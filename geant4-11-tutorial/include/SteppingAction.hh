#ifndef STEPPINGACTION_HH
#define STEPPINGACTION_HH

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include <unordered_set>

class RunAction;

class SteppingAction : public G4UserSteppingAction {
public:
    SteppingAction(RunAction* runAction);
    virtual ~SteppingAction() = default;

    virtual void UserSteppingAction(const G4Step* step);

private:
    RunAction* fRunAction;
    // Flags for current event
    G4bool fNeutronLostEnergy = false;
    G4bool fAlphaProduced     = false;

    G4int fCurrentEventID = -1;
    std::unordered_set<int> countedNeutrons;   // trackIDs already counted


    void ResetFlagsIfNewEvent(const G4Step* step);
};

#endif

