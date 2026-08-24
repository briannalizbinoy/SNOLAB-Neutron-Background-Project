#include "RunAction.hh"
#include "G4AnalysisManager.hh"
#include "SensitiveDetector.hh"
#include "G4SDManager.hh"

std::atomic<int> RunAction::neutronEntryCount{0};

RunAction::RunAction()
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    // analysisManager->SetFileName("output");
    // analysisManager->SetDefaultFileType("csv");

    analysisManager->SetVerboseLevel(1);
    analysisManager->SetNtupleMerging(true); // safe for multithreaded runs
    
    analysisManager->CreateH1("Edep", "Energy deposit; Energy deposited (MeV); Counts", 1000, 0., 15 * MeV);
    analysisManager->SetH1XAxisTitle(0, "Deposited Energy (MeV)");
    analysisManager->SetH1YAxisTitle(0, "Counts");
    analysisManager->CreateNtuple("Alphas", "Alphas");
    analysisManager->CreateNtupleIColumn("iEvent");
    analysisManager->CreateNtupleDColumn("fX");
    analysisManager->CreateNtupleDColumn("fY");
    analysisManager->CreateNtupleDColumn("fZ");
    analysisManager->CreateNtupleDColumn("fGlobalTime");
    // analysisManager->CreateNtupleDColumn("Edep"); 
    // analysisManager->CreateNtupleDColumn("fWlen");
    analysisManager->FinishNtuple(0);
}

RunAction::~RunAction()
{
    delete G4AnalysisManager::Instance();
}

void RunAction::BeginOfRunAction(const G4Run *run)
{
    fHeElasticCount = 0;
    neutronEntryCount = 0;

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    G4VSensitiveDetector* vsd =
    G4SDManager::GetSDMpointer()->FindSensitiveDetector("BDS", false);
    SensitiveDetector* sd = dynamic_cast<SensitiveDetector*>(vsd);

    G4int runID = run->GetRunID();

    std::stringstream strRunID;
    strRunID << runID;

    analysisManager->OpenFile("output_2p5MeV_neutrons_He4" + strRunID.str() + ".root");


}



void RunAction::EndOfRunAction(const G4Run *run)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();


    analysisManager->Write();

    analysisManager->CloseFile();

    G4int runID = run->GetRunID();

        // ---------------------------------------------------------
    // Print total number of Li-6(n,α)t reactions
    // ---------------------------------------------------------
    G4cout << "\n=============================================\n";
    G4cout << " Run ID: " << runID << G4endl;
    G4cout << " Total Li6(n,alpha)t reactions detected = "
           << SensitiveDetector::fLi6ReactionCount << G4endl;
    G4cout << "=============================================\n\n";


    G4cout << "\n=============================================\n";
    G4cout << " Run ID: " << run->GetRunID() << G4endl;
    G4cout << " Total He-4(n,n)elastic events = "
           << fHeElasticCount << G4endl;
    G4cout << "=============================================\n\n";


    G4cout << " Total neutron entries into detector = "
       << neutronEntryCount << G4endl;
}

