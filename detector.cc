#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    G4Track *track = aStep->GetTrack();
    
    track->SetTrackStatus(fStopAndKill);

    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

    G4ThreeVector posPhoton = preStepPoint->GetPosition();
    G4ThreeVector momPhoton = preStepPoint->GetMomentum();
    G4double enPhoton = preStepPoint->GetKineticEnergy();

    G4double minEnergy = 1.9075*eV;
    G4double maxEnergy = 4.1328*eV;
    G4double resEnergy = (maxEnergy - minEnergy)/1024;

    G4int channel = -1;
    if ((enPhoton > minEnergy)&&(enPhoton < maxEnergy)){
        channel = (enPhoton - minEnergy)/resEnergy;
    }

    G4cout << "Photon energy: " << enPhoton/eV << " Channel: "<< channel << G4endl;

    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn(0, evt);
    man->FillNtupleIColumn(1, channel);
    man->AddNtupleRow(0);
}
