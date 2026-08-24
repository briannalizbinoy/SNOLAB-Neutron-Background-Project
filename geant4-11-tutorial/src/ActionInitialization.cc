#include "ActionInitialization.hh"
#include "PrimaryGenerator.hh"   
#include "RunAction.hh"
#include "SteppingAction.hh"



ActionInitialization::ActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
    RunAction *MasterrunAction = new RunAction();
    SetUserAction(MasterrunAction); 
}

void ActionInitialization::Build() const
{
    PrimaryGenerator *generator = new PrimaryGenerator();
    SetUserAction(generator);

    RunAction *runAction = new RunAction();
    SetUserAction(runAction);

    SteppingAction *steppingAction = new SteppingAction(runAction);
    SetUserAction(steppingAction);

}


