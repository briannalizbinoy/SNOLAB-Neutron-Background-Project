#include "SensitiveDetector.hh"
#include "G4Alpha.hh"
#include "G4Neutron.hh"
#include "G4Triton.hh"
#include "G4HadronicProcess.hh"
#include "G4Nucleus.hh"

// static std::ofstream particleOut("AllParticles.txt", std::ios::app);


SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
    fTotalEnergyDeposited = 0.;
    fEventID = -1;
}

SensitiveDetector::~SensitiveDetector()
{
}

void SensitiveDetector::Initialize(G4HCofThisEvent *)
{
    fTotalEnergyDeposited = 0.;
    fEventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
}

G4int SensitiveDetector::fLi6ReactionCount = 0;


G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{


    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4int trackID = aStep->GetTrack()->GetTrackID();
    G4int parentID = aStep->GetTrack()->GetParentID();
    G4double edep = aStep->GetTotalEnergyDeposit();
    // Print or store particle information for debugging
    G4String particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();
    G4int particleNumber = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();
    // --- Detect Li6(n, α)t reactions ---
    const G4VProcess* creator = aStep->GetTrack()->GetCreatorProcess();

    if (creator) {
        G4String procName = creator->GetProcessName();
        G4String pname    = aStep->GetTrack()->GetDefinition()->GetParticleName();

        // Check if this is an  triton created by neutron capture
        if (procName == "neutronInelastic" ) {
            if (pname == "triton") {

                // Count only once per reaction 
                static thread_local G4int lastEvent = -1;

                if (lastEvent != eventID) {
                    SensitiveDetector::fLi6ReactionCount++;
                    lastEvent = eventID;

                    // G4cout << "Li6(n,a)t reaction detected in event "
                    //     << eventID << " (Total = "
                    //     << SensitiveDetector::fLi6ReactionCount << ")"
                    //     << G4endl;
                }
            }
        }
    }









    
    G4ParticleDefinition* particle = aStep->GetTrack()->GetDefinition();
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();

    G4double fGlobalTime = preStepPoint->GetGlobalTime();
    G4ThreeVector posNeutron = preStepPoint->GetPosition();
    G4ThreeVector momNeutron = preStepPoint->GetMomentum();
    G4double ekin1 = preStepPoint->GetKineticEnergy();
    G4double fMomNeutronMag = momNeutron.mag();


    // G4cout << "EventID: " << eventID
    //    << "TrackID: " << aStep->GetTrack()->GetTrackID()
    //    << "ParticleName: " << particleName 
    //    << " Edep: " << aStep->GetTotalEnergyDeposit()/keV << " keV"

    //    << " in volume: " << aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()
    //    << "Process: " << aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()
    //    << G4endl;
    // G4ThreeVector position = aStep->GetTrack()->GetPosition();
    // Write particle information to file (not to terminal)

// G4String processName = creator ? creator->GetProcessName() : "primary";

// if (particleOut.tellp() == 0) {
//     particleOut << "EventID  TrackID  ParentID  Particle  Process  Ekin(MeV)  \n";
// }

// particleOut 
//     << eventID << "  "
//     << trackID << "  "
//     << parentID << "  "
//     << particleName << "  "
//     << processName << "  "
//     << ekin1 << "\n"; 
    // G4double fWlen = (1.239841939 *eV / fMomPhotonMag) * 1E+03;//energy of photon into wavelength converstion

    analysisManager->FillNtupleIColumn(0, 0, eventID);
    analysisManager->FillNtupleDColumn(0, 1, posNeutron[0]);
    analysisManager->FillNtupleDColumn(0, 2, posNeutron[1]);
    analysisManager->FillNtupleDColumn(0, 3, posNeutron[2]);
    analysisManager->FillNtupleDColumn(0, 4, fGlobalTime);
    // analysisManager->FillNtupleDColumn(0, 5, edep);
    // analysisManager->FillNtupleDColumn(0, 5, fWlen);
    analysisManager->AddNtupleRow(0);

    G4double fEnergyDeposited = aStep->GetTotalEnergyDeposit();

    if(fEnergyDeposited > 0)
    {
        fTotalEnergyDeposited += fEnergyDeposited;
    }


    return true;
}


void SensitiveDetector::EndOfEvent(G4HCofThisEvent *)
{
    if(fTotalEnergyDeposited > 0)
    {

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    analysisManager->FillH1(0, fTotalEnergyDeposited);
    
    G4cout << "Deposited energy: " << fTotalEnergyDeposited  << " MeV" << G4endl;
    }

    // --- Write to text file ---
    // static std::ofstream outFile("Edep_events.txt", std::ios::app);

    // if (outFile.tellp() == 0)
    // {
    //     outFile << "EventID  DepositedEnergy(MeV)\n";
    // }

    // outFile << fEventID << "  "
    //         << std::setprecision(12) << fTotalEnergyDeposited << "\n";

}


