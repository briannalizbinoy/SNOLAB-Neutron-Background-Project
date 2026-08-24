#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4Isotope.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4NistManager.hh"

G4NistManager* nist = G4NistManager::Instance();



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

    // World

    // G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
    // Vacuum
    auto vacuum = new G4Material("Galactic", 1., 1.01 * g / mole, universe_mean_density,
        kStateGas, 2.73 * kelvin, 3.e-18 * pascal);
 
    G4Material *worldMat = vacuum;
    
 

    G4double xWorld = 15 *m;
    G4double yWorld = 15 *m;
    G4double zWorld = 15 *m;

    G4Box *solidWorld = new G4Box("solidWorld",1  * xWorld, 1 * yWorld, 1 * zWorld);
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


    // MATERIALS

    //stainless steel
    G4Material *shellMat = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    //LiF
    G4Material *LiF = nist->FindOrBuildMaterial("G4_LITHIUM_FLUORIDE");

    
    G4Isotope* isoD = new G4Isotope("DeuteriumIso", 1, 2, 2.0141018 * g/mole);
    G4Element* elD = new G4Element("Deuterium", "D", 1);
    elD->AddIsotope(isoD, 100.0 * perCent);

    
    G4Element* elO = nist->FindOrBuildElement("O");

    // UPW
    G4double d2oDensity = 1.107 * g/cm3;
    G4Material* d2oMat = nist->FindOrBuildMaterial("G4_WATER");


    // Polyethylene
    G4Material *polyethyleneMat = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

    // Norite???
    G4Material* wallMat = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

    //He4 gas
    G4double atomicMass = 4.002602 * g / mole;
    G4Isotope *he4Iso = new G4Isotope("He4Iso", 2, 4, atomicMass);
    G4Element *He4 = new G4Element("Helium4", "He4", 1);
    He4->AddIsotope(he4Iso, 100.0 *perCent);


    G4double density = 0.0321 *g/cm3;
    G4double temperature = 293. *kelvin;
    G4double pressure = 180. *bar; 

    G4Material *He4Gas = new G4Material("He4Gas", density, 1, kStateGas, temperature, pressure);
    He4Gas->AddElement(He4, 1);
    
    
    //DIMENSIONS
    //stainless steel 
    G4double shellOuter = 35.*mm;
    G4double endcapLen = 35. *mm;
    G4double sensLen = 600. *mm;
    G4double segLen = (sensLen / 3.0) ; // 200 mm
    // detector HalfLen
    G4double detectorTotalHalfLen = 0.5 * (sensLen + (2.0 * endcapLen)); // 335 mm

    // Air Gap around detector
    G4double gapThickness = 5.0 * mm;
    G4double innerHoleRadius = shellOuter + gapThickness;            // 40.0 mm
    G4double innerHoleHalfLen = detectorTotalHalfLen + gapThickness; // 340.0 mm

    // Polyethylene shielding 
    G4double shieldThickness = 2.0 * 2.54 * cm; // 50.8 mm
    G4double shieldHalfX = innerHoleRadius + shieldThickness;   // 90.8 mm
    G4double shieldHalfY = innerHoleRadius + shieldThickness;   // 90.8 mm
    G4double shieldHalfZ = innerHoleHalfLen + shieldThickness;  // 390.8 mm

    //UPW Outer Shielding 
    G4double d2oThickness = 12.0 * 2.54 * cm; // 304.8 mm
    G4double d2oHalfX = shieldHalfX + d2oThickness; // 395.6 mm
    G4double d2oHalfY = shieldHalfY + d2oThickness; // 395.6 mm
    G4double d2oHalfZ = shieldHalfZ + d2oThickness; // 695.6 mm

    //LiF
    G4double liFThickness = 0.5 *mm; //cant find what the actual thickness is, so assumed this
    G4double liFOuterRadius = 22. *mm;
    G4double liFInnerRadius = liFOuterRadius - liFThickness;

    //He4 gas
    G4double gasOuterRadius = liFInnerRadius - 0.01*mm;

    //stainless steel Subtractor 
    G4double steelSubtractorRadius = liFOuterRadius + 0.01*mm;

    
    // Solid sensitive stainless-steel 
    G4Tubs *solidsteel= new G4Tubs("solidsteel", 0., shellOuter, 0.5 *sensLen, 0. *deg, 360. *deg);

    //Subtractor 
    G4Tubs *subtractorsteel= new G4Tubs("subtractorsteel", 0., steelSubtractorRadius, 0.5 *sensLen, 0. *deg, 360. *deg);

    // Outershell
    G4VSolid* solidShell = new G4SubtractionSolid("solidShell", solidsteel, subtractorsteel);
    logicShell = new G4LogicalVolume(solidShell, shellMat , "logicShell");
    G4VPhysicalVolume *physShell = new G4PVPlacement(0, G4ThreeVector(0., 0., 0. * cm), logicShell, "physShell", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes *outerShellVis = new G4VisAttributes(G4Color(1., 0.0, 1., 0.5)); 
    outerShellVis->SetForceSolid(true);
    logicShell->SetVisAttributes(outerShellVis);
   
    // Endcap1
    G4Tubs *solidCap1= new G4Tubs("solidCap1", 0., shellOuter, 0.5 *endcapLen, 0. *deg, 360. *deg);
    logicCap1 = new G4LogicalVolume(solidCap1, shellMat , "logicCap1");
    G4VPhysicalVolume *physCap1 = new G4PVPlacement(0, G4ThreeVector(0., 0.,0.5*(sensLen+endcapLen)), logicCap1, "physCap1", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes *endCap1Vis = new G4VisAttributes(G4Color(1., 0.0, 1., 0.5)); 
    endCap1Vis->SetForceSolid(true);
    logicCap1->SetVisAttributes(endCap1Vis);

    //Endcap2
    G4Tubs *solidCap2= new G4Tubs("solidCap2", 0., shellOuter, 0.5 *endcapLen, 0. *deg, 360. *deg);
    logicCap2 = new G4LogicalVolume(solidCap2, shellMat , "logicCap2");
    G4VPhysicalVolume *physCap2 = new G4PVPlacement(0, G4ThreeVector(0., 0.,-0.5*(sensLen+endcapLen)), logicCap2, "physCap2", logicWorld, false, 0, checkOverlaps);   
    G4VisAttributes *endCap2Vis = new G4VisAttributes(G4Color(1., 0.0, 1., 0.5)); 
    endCap2Vis->SetForceSolid(true);
    logicCap2->SetVisAttributes(endCap2Vis); 

    //LiF test to see if simulation works
    // G4Tubs *solidLiF = new G4Tubs("solidLiF", 0., gasOuterRadius, 0.5 *sensLen, 0. *deg, 360.*deg);
    // logicLiF= new G4LogicalVolume(solidLiF, LiF, "logicLiF");
    // G4VPhysicalVolume *physLiF = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicLiF, "physLiF", logicWorld, false, 0, checkOverlaps);
    // G4VisAttributes *liFVis = new G4VisAttributes(G4Color(1., 0., 0., 0.5)); // cyan
    // liFVis->SetForceSolid(true);
    // logicLiF->SetVisAttributes(liFVis);   

    // POLYETHYLENE
    G4Box *solidOuterShieldBox = new G4Box("solidOuterShieldBox", shieldHalfX, shieldHalfY, shieldHalfZ);
    
    // Inner hole 
    G4Tubs *solidInnerDetectorHole = new G4Tubs("solidInnerDetectorHole", 0., innerHoleRadius, innerHoleHalfLen, 0. * deg, 360. * deg);
    
    G4VSolid *solidEncapsulatedShield = new G4SubtractionSolid("solidEncapsulatedShield", solidOuterShieldBox, solidInnerDetectorHole);
    
    logicShield = new G4LogicalVolume(solidEncapsulatedShield, polyethyleneMat, "logicShield");
    G4VPhysicalVolume *physShield = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicShield, "physShield", logicWorld, false, 0, checkOverlaps);

    G4VisAttributes *shieldVis = new G4VisAttributes(G4Color(0.0, 1.0, 1.0, 0.3)); // Light blue / cyan transparent
    shieldVis->SetForceSolid(true);
    logicShield->SetVisAttributes(shieldVis);

    //UPW shield
    G4Box *solidD2OOuterBox = new G4Box("solidD2OOuterBox", d2oHalfX, d2oHalfY, d2oHalfZ);
    G4Box *solidD2OInnerHole = new G4Box("solidD2OInnerHole", shieldHalfX, shieldHalfY, shieldHalfZ);
    G4VSolid *solidD2OShield = new G4SubtractionSolid("solidD2OShield", solidD2OOuterBox, solidD2OInnerHole);
    
    logicD2OShield = new G4LogicalVolume(solidD2OShield, d2oMat, "logicD2OShield");
    G4VPhysicalVolume *physD2OShield = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicD2OShield, "physD2OShield", logicWorld, false, 0, checkOverlaps);

    G4VisAttributes *d2oShieldVis = new G4VisAttributes(G4Color(0.0, 0.5, 1.0, 0.3)); // Light blue / cyan transparent
    d2oShieldVis->SetForceSolid(true);
    logicD2OShield->SetVisAttributes(d2oShieldVis);

    // // 3. Inner Cavern Dimensions (2m width x 2m length x 5m height)
    // G4double inner_X = 100.0 * cm; // half-width (2m total)
    // G4double inner_Y = 100.0 * cm; // half-length (2m total)
    // G4double inner_Z = 250.0 * cm; // half-height (5m total)

    // // 4. Outer Wall Dimensions (Inner + 4m thickness on each side)
    // G4double outer_X = inner_X + 400.0 * cm; // 500 cm half-width
    // G4double outer_Y = inner_Y + 400.0 * cm; // 500 cm half-length
    // G4double outer_Z = inner_Z + 400.0 * cm; // 650 cm half-height

    // // 5. Build Cavern Shell Geometry (Outer Box minus Inner Box)
    // G4Box* outerBox = new G4Box("OuterBox", outer_X, outer_Y, outer_Z);
    // G4Box* innerBox = new G4Box("InnerBox", inner_X, inner_Y, inner_Z);

    // G4SubtractionSolid* solidCavernWalls = new G4SubtractionSolid(
    //     "CavernWalls_Solid", outerBox, innerBox, 0, G4ThreeVector(0,0,0)
    // );

    // G4LogicalVolume* logicCavernWalls = new G4LogicalVolume(
    //     solidCavernWalls, wallMat, "CavernWalls_LV"
    // );

    // G4VPhysicalVolume* physCavernWalls = new G4PVPlacement(
    //     0, G4ThreeVector(0,0,0), logicCavernWalls, "CavernWalls_PV", logicWorld, false, 0, true
    // );

    //LiF Coating
     
    G4Tubs *solidLiF = new G4Tubs("solidLiF", liFInnerRadius, liFOuterRadius, 0.5 *sensLen, 0. *deg, 360. *deg);
    logicLiF= new G4LogicalVolume(solidLiF, LiF, "logicLiF");
    G4VPhysicalVolume *physLiF = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicLiF, "physLiF", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes *liFVis = new G4VisAttributes(G4Color(1., 0., 0., 0.5)); // cyan
    liFVis->SetForceSolid(true);
    logicLiF->SetVisAttributes(liFVis);
    
    
    //He-4 gas
    G4Tubs* solidHe4 = new G4Tubs("solidHe4", 0., gasOuterRadius, 0.5 *sensLen, 0. *deg, 360.*deg);
    logicHe4 = new G4LogicalVolume(solidHe4, He4Gas, "logicHe4");
    G4VPhysicalVolume *physHe4 = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicHe4, "physHe4", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes* he4Vis = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 1.0)); // gradient
    he4Vis->SetForceSolid(true);
    logicHe4->SetVisAttributes(he4Vis);   

    // //Visualization
    // G4VisAttributes *ShellVis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.3)); 
    // ShellVis->SetForceSolid(true);
    // logicShell->SetVisAttributes(ShellVis);
    
    // G4VisAttributes *CapVis = new G4VisAttributes(G4Colour(0.3,0.3,0.3,0.5)); 
    // CapVis->SetForceSolid(true);
    // logicCap->SetVisAttributes(CapVis);


    return physWorld;
}

// // ------------------- SENSITIVE DETECTORS -------------------
// void DetectorConstruction::ConstructSDandField()
// {
//     auto* sdManager = G4SDManager::GetSDMpointer();

//     // LiF coating detectors
//     for (size_t i = 0; i < logicLiFSegments.size(); ++i) {
//         G4String name = "LiFSensitiveDetector_" + std::to_string(i);
//         auto* sd = new SensitiveDetector(name);
//         sdManager->AddNewDetector(sd);
//         logicLiFSegments[i]->SetSensitiveDetector(sd);
//     }

//     // He4 segment detectors
//     for (size_t i = 0; i < logicHe4Segments.size(); ++i) {
//         G4String name = "He4SensitiveDetector_" + std::to_string(i);
//         auto* sd = new SensitiveDetector(name);
//         sdManager->AddNewDetector(sd);
//         logicHe4Segments[i]->SetSensitiveDetector(sd);
//     }
// }

// Sensitive Detector 

void DetectorConstruction::ConstructSDandField()
{
    SensitiveDetector *he4sensDet = new SensitiveDetector("He4SensitiveDetector");
    logicHe4->SetSensitiveDetector(he4sensDet);
    G4SDManager::GetSDMpointer()->AddNewDetector(he4sensDet);

    SensitiveDetector *LiSensDet = new SensitiveDetector("LiSensitiveDetector");
    logicLiF->SetSensitiveDetector(LiSensDet);
    G4SDManager::GetSDMpointer()->AddNewDetector(LiSensDet);

    SensitiveDetector *OuterShellSensDet = new SensitiveDetector("OuterShellSensitiveDetector");
    logicShell->SetSensitiveDetector(OuterShellSensDet);
    G4SDManager::GetSDMpointer()->AddNewDetector(OuterShellSensDet);

    SensitiveDetector *endCap1SensDet = new SensitiveDetector("endCap1SensitiveDetector");
    logicCap1->SetSensitiveDetector(endCap1SensDet);
    G4SDManager::GetSDMpointer()->AddNewDetector(endCap1SensDet);

    SensitiveDetector *endCap2SensDet = new SensitiveDetector("endCap2SensitiveDetector");
    logicCap2->SetSensitiveDetector(endCap2SensDet);
    G4SDManager::GetSDMpointer()->AddNewDetector(endCap2SensDet);

    SensitiveDetector *ShieldSensDet = new SensitiveDetector("ShieldSensitiveDetector");
    logicShield->SetSensitiveDetector(ShieldSensDet);
    G4SDManager::GetSDMpointer()->AddNewDetector(ShieldSensDet);

    SensitiveDetector *D2OShieldSensDet = new SensitiveDetector("D2OShieldSensitiveDetector");
    logicD2OShield->SetSensitiveDetector(D2OShieldSensDet);
    G4SDManager::GetSDMpointer()->AddNewDetector(D2OShieldSensDet);
}


// void DetectorConstruction::ConstructSDandField()
// {
//     SensitiveDetector *LiSensDet = new SensitiveDetector("LiSensitiveDetector");
//     logicLiF->SetSensitiveDetector(LiSensDet);
//     G4SDManager::GetSDMpointer()->AddNewDetector(LiSensDet);
// }