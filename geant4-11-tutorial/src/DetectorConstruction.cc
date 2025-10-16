#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4Isotope.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"


DetectorConstruction::DetectorConstruction()
{
}

DetectorConstruction::~DetectorConstruction()
{
}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
    G4bool checkOverlaps = true;

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");



    G4double xWorld = 1. *m;
    G4double yWorld = 1. *m;
    G4double zWorld = 1. *m;

    G4Box *solidWorld = new G4Box("solidWorld", 0.5 * xWorld, 0.5 * yWorld, 0.5 * zWorld);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(
    0,                            // no rotation
    G4ThreeVector(0., 0., 0.),    // at origin
    logicWorld,                   // logical volume
    "physWorld",                  // name
    0,                            // mother volume (none)
    false,                        // no boolean operation
    0,                            // copy number
    checkOverlaps                 // check overlaps
);

    // Define He-4 gas
    G4double atomicMass = 4.002602 * g / mole;
    G4Isotope *he4Iso = new G4Isotope("He4Iso", 2, 4, atomicMass);
    G4Element *He4 = new G4Element("Helium4", "He4", 1);
    He4->AddIsotope(he4Iso, 100.0 *perCent);

    G4double density = 0.0321 *g/cm3;
    G4double temperature = 293. *kelvin;
    G4double pressure = 180. *bar; 
    
    G4Material *He4Gas = new G4Material("He4Gas", density, 1, kStateGas, temperature, pressure);
    He4Gas->AddElement(He4, 1);

    G4double innerRadius = 22. *mm;
    G4double outerRadius = 100. *mm; // need to double check
    G4double Length = 670.0 *mm; //G4Tubs uses half-length
    
    G4Tubs *solidDetector = new G4Tubs("He4Detector", innerRadius, outerRadius, 0.5 *Length, 0. *deg, 360. *deg);
    logicDetector = new G4LogicalVolume(solidDetector, He4Gas, "logicDetector");
    G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(0., 0., 10.5 * cm), logicDetector, "physDetector", logicWorld, false, checkOverlaps);

    G4VisAttributes *detVisAtt = new G4VisAttributes(G4Color(1.0, 1.0, 0.0, 0.5));
    detVisAtt->SetForceSolid(true);
    logicDetector->SetVisAttributes(detVisAtt);


    return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
    SensitiveDetector *sensDet = new SensitiveDetector("SensitiveDetector");
    logicDetector->SetSensitiveDetector(sensDet);
    G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
}