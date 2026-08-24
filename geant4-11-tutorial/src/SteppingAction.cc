#include "SteppingAction.hh"
#include "RunAction.hh"

#include "G4SystemOfUnits.hh"
#include "G4Neutron.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include <unordered_map>
#include <unordered_set>


//detector region
bool IsInDetector(G4VPhysicalVolume* vol)
{
    auto name = vol->GetName();
    return ( name == "physShell"|| name == "physCap1" || name == "physCap2");
}


SteppingAction::SteppingAction(RunAction* runAction)
: fRunAction(runAction)
{}

void SteppingAction::ResetFlagsIfNewEvent(const G4Step* step)
{
    G4int eventID =
        G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();

    if (eventID != fCurrentEventID) {
        fCurrentEventID = eventID;
        fNeutronLostEnergy = false;
        fAlphaProduced = false;
    }
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    ResetFlagsIfNewEvent(step);

    G4Track* track = step->GetTrack();
    G4String name = track->GetParticleDefinition()->GetParticleName();
    // G4VPhysicalVolume* preVol = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();

    // Count neutrons that ENTER the detector (only once)
    if (name == "neutron")
{
    auto preVol  = step->GetPreStepPoint()->GetPhysicalVolume();
    auto postVol = step->GetPostStepPoint()->GetPhysicalVolume();
    // G4cout << "isindetector" << IsInDetector << G4endl;
    if (!preVol || !postVol) return;

    // neutron enters detector region for first time

    if (IsInDetector(postVol))
    {
        int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
        // G4cout << "odo" << (countedNeutrons.find(eventID) == countedNeutrons.end()) << G4endl;


        if (countedNeutrons.find(eventID) == countedNeutrons.end())
        {
            countedNeutrons.insert(eventID);

            fRunAction->CountNeutronEntry();

            // G4cout << "NEUTRON ENTERED DETECTOR | TrackID="
            //        << eventID  << " :" << postName << G4endl;
        }
    }
}
    // 1. Detect neutron losing energy
    if (name == "neutron") {
        G4double preE  = step->GetPreStepPoint()->GetKineticEnergy();
        G4double postE = step->GetPostStepPoint()->GetKineticEnergy();

        if (postE < preE) {
            fNeutronLostEnergy = true;
        }
    }

    // 2. Detect alpha produced by hadElastic
    if (name == "alpha") {
        const G4VProcess* creator = track->GetCreatorProcess();

        if (creator && creator->GetProcessName() == "hadElastic") {
            fAlphaProduced = true;
        }
    }


    // 3. If both conditions are satisfied count event once
    if (fNeutronLostEnergy && fAlphaProduced) {
        fRunAction->CountHe4Elastic();

        // prevent double counting
        fNeutronLostEnergy = false;
        fAlphaProduced = false;
    }
    


}

