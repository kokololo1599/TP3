#include "AITimeSlicerSubsystem.h"
#include "AIController.h"
#include "BrainComponent.h"

void UAITimeSlicerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    ActiveControllers.Empty();
    CurrentIndex = 0;
}

void UAITimeSlicerSubsystem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (ActiveControllers.Num() == 0) return;

    double StartTime = FPlatformTime::Seconds();
    int32 ControllersProcessedThisFrame = 0;

    // Loop through controllers, picking up exactly where we left off last frame
    while (ControllersProcessedThisFrame < ActiveControllers.Num())
    {
        if (CurrentIndex >= ActiveControllers.Num())
        {
            CurrentIndex = 0; // Wrap around
        }

        AAIController* Controller = ActiveControllers[CurrentIndex];

        // Manually tick the AI's Brain Component (The Behavior Tree)
        if (IsValid(Controller) && Controller->GetBrainComponent())
        {
            Controller->GetBrainComponent()->TickComponent(DeltaTime, LEVELTICK_All, nullptr);
        }

        CurrentIndex++;
        ControllersProcessedThisFrame++;

        // Check if we have exceeded our time budget
        if ((FPlatformTime::Seconds() - StartTime) >= MaxTimeBudget)
        {
            // Budget exceeded: pause execution. 
            // The remaining agents will wait until the next frame.
            break;
        }

        if (GEngine)
        {
            // Key '1' overwrites the same message so it doesn't spam your screen
            FString DebugMsg = FString::Printf(TEXT("TimeSlicer: Processed %d out of %d agents this frame."),
                ControllersProcessedThisFrame, ActiveControllers.Num());

            // If it didn't process everyone, draw it in RED. Otherwise, GREEN.
            FColor MsgColor = (ControllersProcessedThisFrame < ActiveControllers.Num()) ? FColor::Red : FColor::Green;

            GEngine->AddOnScreenDebugMessage(1, 0.0f, MsgColor, DebugMsg);
        }
    }
}

void UAITimeSlicerSubsystem::RegisterController(AAIController* Controller)
{
    if (Controller && !ActiveControllers.Contains(Controller))
    {
        ActiveControllers.Add(Controller);
    }
}

void UAITimeSlicerSubsystem::UnregisterController(AAIController* Controller)
{
    if (Controller)
    {
        ActiveControllers.Remove(Controller);
        if (CurrentIndex >= ActiveControllers.Num())
        {
            CurrentIndex = 0;
        }
    }
}

TStatId UAITimeSlicerSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UAITimeSlicerSubsystem, STATGROUP_Tickables);
}