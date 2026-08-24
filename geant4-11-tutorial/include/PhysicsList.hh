#ifndef PHYSICSLISTS_HH
#define PHYSICSLISTS_HH

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4DecayPhysics.hh" //includes different decay models
#include "HadronElasticPhysicsHP.hh"
#include "ElectromagneticPhysics.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4IonPhysicsXS.hh"
#include "G4NeutronTrackingCut.hh"

class PhysicsList : public G4VModularPhysicsList
{
public:
    PhysicsList();
    ~PhysicsList();
};

#endif