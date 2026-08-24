#include "PhysicsList.hh"

PhysicsList::PhysicsList()
{
    //EM Physics
    RegisterPhysics(new G4EmStandardPhysics());

    // // Radioactive decay physics
    // RegisterPhysics(new G4RadioactiveDecayPhysics());

    // // Decay physics
    // RegisterPhysics(new G4DecayPhysics());

    // //Hadronic and nuclear processes
    // RegisterPhysics(new G4HadronElasticPhysicsHP(1));
    // RegisterPhysics(new G4HadronPhysicsQGSP_BIC_HP());
    // RegisterPhysics(new G4StoppingPhysics());
    // RegisterPhysics(new G4IonPhysics());
    // RegisterPhysics(new G4NeutronTrackingCut());

    //Some physics lists require a verbose level
  //We don't require the associated verbose information
  //So we set the verbose level to 0
  G4int verb = 1;
  SetVerboseLevel(verb);
  //Some also require strings but we'll set that as we go
 
  // Hadron Elastic scattering
  RegisterPhysics(new HadronElasticPhysicsHP(verb));

  //RegisterPhysics(new G4HadronElasticPhysicsXS(verb));
 
  // Hadron Inelastic Physics
  RegisterPhysics( new G4HadronPhysicsQGSP_BIC_HP());
  //RegisterPhysics(new G4HadronInelasticQBBC(verb));
 
  // Ion Physics
  //RegisterPhysics( new G4IonPhysics());
  RegisterPhysics(new G4IonPhysicsXS(verb));
 
  // stopping Particles
  RegisterPhysics( new G4StoppingPhysics());
 
  // Gamma-Nuclear Physics
  //RegisterPhysics( new GammaNuclearPhysics("Gamma"));
 
  // EM physics
  RegisterPhysics(new ElectromagneticPhysics());
 
  // //Photodisintegration for Dueterium
  // RegisterPhysics(new GammaNuclearPhysicsLEND("gamma"));
 
  // Decay
  RegisterPhysics(new G4DecayPhysics());
 
  // Radioactive decay
  RegisterPhysics(new G4RadioactiveDecayPhysics());
  //G4RadioactiveDecayPhysics* decayProcess = new G4RadioactiveDecayPhysics();
  //decayProcess->SetThreshold(1 * MeV); // Set threshold for alpha detection
  //RegisterProcess(decayProcess);
 
  //EM extra list (Kishan)
 // RegisterPhysics(new G4EmExtraPhysics());

}


PhysicsList::~PhysicsList()
{
}