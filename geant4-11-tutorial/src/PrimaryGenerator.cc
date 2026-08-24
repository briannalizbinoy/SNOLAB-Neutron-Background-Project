#include "PrimaryGenerator.hh"
#include "G4Alpha.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4Event.hh"

PrimaryGenerator::PrimaryGenerator()
{
    // fParticleGun = new G4ParticleGun(1);
    fParticleGPS = new G4GeneralParticleSource();
    
    // // Particle position
    // G4double x = 0. *m;
    // G4double y = 0. *m;
    // G4double z = 0.7 *m;

    // G4ThreeVector pos(x, y, z);

    // //Particle direction
    // G4double px = 0.;
    // G4double py = 0.;
    // G4double pz = -1.;

    // G4ThreeVector mom(px, py, pz);
    

    //Particle type
    // G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    // G4ParticleDefinition *particle = particleTable->FindParticle("neutron");
    // fParticleGun->SetParticlePosition(pos);
    // fParticleGun->SetParticleMomentumDirection(mom);
    // fParticleGun->SetParticleEnergy(1*MeV);
    // fParticleGun->SetParticleDefinition(particle);
}

PrimaryGenerator::~PrimaryGenerator()
{
    delete fParticleGPS;
    //delete fParticleGun;
}

void PrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    // //Particle type
    // G4int Z = 9;
    // G4int A = 18;

    // G4double charge = 0. * eplus;
    // G4double energy = 0. * keV;

    // G4ParticleDefinition *ion = G4IonTable::GetIonTable()->GetIon(Z, A, energy);
    // fParticleGun->SetParticleDefinition(ion);
    // fParticleGun->SetParticleCharge(charge);
    // fParticleGun->SetParticleEnergy(energy);


    //Create vertex
    //fParticleGun->GeneratePrimaryVertex(anEvent);
     fParticleGPS->GeneratePrimaryVertex(anEvent);
}

