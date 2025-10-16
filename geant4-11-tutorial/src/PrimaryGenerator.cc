#include "PrimaryGenerator.hh"

PrimaryGenerator::PrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);
    
    // Particle position
    G4double x = 0. *cm;
    G4double y = 0. *cm;
    G4double z = -1. *cm;

    G4ThreeVector pos(x, y, z);

    //Particle direction
    G4double px = 0.;
    G4double py = 0.;
    G4double pz = 0.;

    G4ThreeVector mom(px, py, pz);

    // //Particle type
    // G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    // G4ParticleDefinition *particle = particleTable->FindParticle("gamma");

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    // fParticleGun->SetParticleEnergy(1. *MeV);
    // fParticleGun->SetParticleDefinition(particle);
}

PrimaryGenerator::~PrimaryGenerator()
{
    delete fParticleGun;
}

void PrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    //Particle type
    G4int Z = 9;
    G4int A = 18;

    G4double charge = 0. * eplus;
    G4double energy = 0. * keV;

    G4ParticleDefinition *ion = G4IonTable::GetIonTable()->GetIon(Z, A, energy);
    fParticleGun->SetParticleDefinition(ion);
    fParticleGun->SetParticleCharge(charge);
    fParticleGun->SetParticleEnergy(energy);


    //Create vertex
    fParticleGun->GeneratePrimaryVertex(anEvent);
}