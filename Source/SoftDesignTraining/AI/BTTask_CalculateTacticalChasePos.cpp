#include "BTTask_CalculateTacticalChasePos.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "AI/AiAgentGroupManager.h" 

UBTTask_CalculateTacticalChasePos::UBTTask_CalculateTacticalChasePos()
{
    NodeName = "Calculate Tactical Chase Position";
}

EBTNodeResult::Type UBTTask_CalculateTacticalChasePos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    APawn* selfPawn = OwnerComp.GetAIOwner()->GetPawn();
    ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
    AiAgentGroupManager* groupManager = AiAgentGroupManager::GetInstance();

    // Safety check
    if (!selfPawn || !playerCharacter || !blackboard || !groupManager)
    {
        return EBTNodeResult::Failed;
    }

    const FVector playerLocation = playerCharacter->GetActorLocation();
    FVector targetLocation = playerLocation; // Default to direct pursuit

    // NOTE: You will need to implement GetAgentIndex in AiAgentGroupManager
    // It should return the index (0, 1, 2, etc.) of this pawn in the pursuit group array.
    int32 myIndex = groupManager->GetAgentIndex(selfPawn);

    // Apply tactical offsets based on group index
    if (myIndex == 1)
    {
        // 1. The Interceptor: Predict where the player is going (1.5 seconds ahead)
        targetLocation = playerLocation + (playerCharacter->GetVelocity() * 1.5f);
    }
    else if (myIndex == 2)
    {
        // 2. Right Flanker: 450 units to the player's right
        targetLocation = playerLocation + (playerCharacter->GetActorRightVector() * 450.f);
    }
    else if (myIndex == 3)
    {
        // 3. Left Flanker: 450 units to the player's left
        targetLocation = playerLocation - (playerCharacter->GetActorRightVector() * 450.f);
    }
    else if (myIndex > 3)
    {
        // 4. Trailer: Cut off retreat 400 units behind the player
        targetLocation = playerLocation - (playerCharacter->GetActorForwardVector() * 400.f);
    }

    // Project the calculated point onto the NavMesh so the AI doesn't try to run into a wall
    UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (navSys)
    {
        FNavLocation projectedLocation;
        if (navSys->ProjectPointToNavigation(targetLocation, projectedLocation, FVector(500.f, 500.f, 500.f)))
        {
            targetLocation = projectedLocation.Location;
        }
        else
        {
            // If the flank spot is off the map/inside a wall, fall back to direct chase
            targetLocation = playerLocation;
        }
    }

    // Write the final vector to the Blackboard
    blackboard->SetValueAsVector(ChaseLocationKey.SelectedKeyName, targetLocation);

    return EBTNodeResult::Succeeded;
}