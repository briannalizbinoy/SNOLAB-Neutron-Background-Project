#include "PhysicsList.hh"

PhysicsList::PhysicsList()
{
    //EM Physics
    RegisterPhysics(new G4EmStandardPhysics());

    // Radioactive decay physics
    RegisterPhysics(new G4RadioactiveDecayPhysics());

    // Decay physics
    RegisterPhysics(new G4DecayPhysics());
}


PhysicsList::~PhysicsList()
{
}