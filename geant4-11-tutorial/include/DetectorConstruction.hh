#ifndef DETECTORCONSTRUCTION_HH
#define DETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include <vector>

#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4SDManager.hh"

#include "SensitiveDetector.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume *Construct();

private:
    G4LogicalVolume* logicShell;
    G4LogicalVolume* logicCap;
    G4LogicalVolume* logicHe4;
    G4LogicalVolume* logicLiF;
    G4LogicalVolume* logicCap1;
    G4LogicalVolume* logicCap2;
    G4LogicalVolume* logicShield;
    G4LogicalVolume* logicD2OShield;



    virtual void ConstructSDandField();
};

#endif



