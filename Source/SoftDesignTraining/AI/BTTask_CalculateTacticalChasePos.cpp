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

    if (!selfPawn || !playerCharacter || !blackboard || !groupManager)
    {
        return EBTNodeResult::Failed;
    }

    const FVector prevTarget = blackboard->GetValueAsVector(ChaseLocationKey.SelectedKeyName);
    if (!FMath::IsNearlyEqual(prevTarget.Z, -500.f, 0.1f)) {
        if (FVector::Dist2D(prevTarget, selfPawn->GetActorLocation()) < 5.0f) {
            blackboard->SetValueAsVector(ChaseLocationKey.SelectedKeyName, groupManager->PlayerLKP);
            return EBTNodeResult::Succeeded;
        }
    }

    const FVector playerLocation = blackboard->GetValueAsVector(PlayerLKPKey.SelectedKeyName);
    const FVector playerForward = blackboard->GetValueAsVector(PlayerLKFKey.SelectedKeyName);
    const FVector playerRight = playerForward.Cross(FVector(0.f, 0.f, -1.0f));


    FVector targetLocation = playerLocation;

    int32 myIndex = groupManager->GetAgentIndex(selfPawn);

    if (myIndex >= 1)
    {
        float angle = (myIndex * (2.0f * PI / groupManager->GetGroupSize())) + PI;

        FVector offset = 1000.0f * (playerForward * FMath::Cos(angle)) + (playerRight * FMath::Sin(angle));

        targetLocation = playerLocation + offset;
    }

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
            targetLocation = playerLocation;
        }
    }

    blackboard->SetValueAsVector(ChaseLocationKey.SelectedKeyName, targetLocation);

    return EBTNodeResult::Succeeded;
}