#include "BTTask_FindBestFleeLocation.h"
#include "SoftDesignTraining/SDTFleeLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

UBTTask_FindBestFleeLocation::UBTTask_FindBestFleeLocation()
{
    NodeName = "Find Best Flee Location";
}

EBTNodeResult::Type UBTTask_FindBestFleeLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    APawn* selfPawn = OwnerComp.GetAIOwner()->GetPawn();
    ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

    if (!selfPawn || !playerCharacter || !blackboard) return EBTNodeResult::Failed;

    UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!navSys) return EBTNodeResult::Failed;

    const FVector selfLocation = selfPawn->GetActorLocation();
    const FVector playerLocation = playerCharacter->GetActorLocation();
    const FVector currentTarget = blackboard->GetValueAsVector(FleeLocationKey.SelectedKeyName);

    float playerDistance = FVector::Dist(selfLocation, playerLocation);

    // =========================================================
    // 1. IMPROVED HYSTERESIS (Trust the current path)
    // =========================================================
    if (blackboard->IsVectorValueSet(FleeLocationKey.SelectedKeyName))
    {
        float distToTarget = FVector::Dist(selfLocation, currentTarget);

        // If we haven't reached the target yet...
        if (distToTarget > 150.f)
        {
            UNavigationPath* currentPath = navSys->FindPathToLocationSynchronously(GetWorld(), selfLocation, currentTarget);
            if (currentPath && currentPath->IsValid())
            {
                float currentClearance = FLT_MAX;
                for (int32 i = 0; i < currentPath->PathPoints.Num() - 1; ++i)
                {
                    currentClearance = FMath::Min(currentClearance, FMath::PointDistToSegment(playerLocation, currentPath->PathPoints[i], currentPath->PathPoints[i + 1]));
                }

                // ONLY recalculate if the path is actually unsafe (< 250)
                // or if the player is EXTREMELY close (Emergency)
                if (currentClearance > 250.f && playerDistance > 200.f)
                {
                    return EBTNodeResult::Succeeded; // Keep going!
                }
            }
        }
    }

    // =========================================================
    // 2. EMERGENCY EVASION
    // =========================================================
    if (playerDistance < 250.f)
    {
        FVector escapeDir = (selfLocation - playerLocation).GetSafeNormal();
        FVector idealEmergencyTarget = selfLocation + escapeDir * 800.f;

        FNavLocation ProjectedLocation;
        if (navSys->ProjectPointToNavigation(idealEmergencyTarget, ProjectedLocation, FVector(500.f, 500.f, 500.f)))
        {
            blackboard->SetValueAsVector(FleeLocationKey.SelectedKeyName, ProjectedLocation.Location);
            return EBTNodeResult::Succeeded;
        }
    }

    // =========================================================
    // 3. NORMAL FLEE SELECTION
    // =========================================================
    float bestSafeScore = -FLT_MAX;
    ASDTFleeLocation* bestSafeLocation = nullptr;
    float bestDangerScore = -FLT_MAX;
    ASDTFleeLocation* bestDangerLocation = nullptr;

    // Standard loop...
    for (TActorIterator<ASDTFleeLocation> it(GetWorld()); it; ++it)
    {
        ASDTFleeLocation* fleeLocation = *it;
        if (!fleeLocation) continue;

        FVector fleeLoc = fleeLocation->GetActorLocation();
        // NEW OPTIMIZATION: Quick distance check
        float distToSelf = FVector::Dist(fleeLoc, selfLocation);
        if (distToSelf > 3000.f) continue; // Skip locations that are too far to be relevant

        // NEW OPTIMIZATION: Dot product check BEFORE pathfinding
        FVector selfToPlayer = (playerLocation - selfLocation).GetSafeNormal();
        FVector selfToFlee = (fleeLoc - selfLocation).GetSafeNormal();
        float dot = FVector::DotProduct(selfToPlayer, selfToFlee);

        // If the flee point is BEHIND the player (dot > 0.5), don't even bother pathfinding
        if (dot > 0.5f) continue;
        UNavigationPath* path = navSys->FindPathToLocationSynchronously(GetWorld(), selfLocation, fleeLoc);

        if (!path || !path->IsValid() || path->PathPoints.Num() < 2) continue;

        float minPathClearance = FLT_MAX;
        for (int32 i = 0; i < path->PathPoints.Num() - 1; ++i)
        {
            minPathClearance = FMath::Min(minPathClearance, FMath::PointDistToSegment(playerLocation, path->PathPoints[i], path->PathPoints[i + 1]));
        }

        float distToPlayer = FVector::Dist(fleeLoc, playerLocation);
        
        float directionScore = -FVector::DotProduct((playerLocation - selfLocation).GetSafeNormal(), (fleeLoc - selfLocation).GetSafeNormal());

        float locationScore = (distToPlayer * 2.0f) + (directionScore * 1000.f) - (distToSelf * 0.5f);

        if (minPathClearance < 300.f)
        {
            if (locationScore > bestDangerScore) { bestDangerScore = locationScore; bestDangerLocation = fleeLocation; }
        }
        else
        {
            if (locationScore > bestSafeScore) { bestSafeScore = locationScore; bestSafeLocation = fleeLocation; }
        }
    }

    // =========================================================
    // 4. APPLY RESULT
    // =========================================================
    if (bestSafeLocation)
    {
        blackboard->SetValueAsVector(FleeLocationKey.SelectedKeyName, bestSafeLocation->GetActorLocation());
    }
    else if (bestDangerLocation)
    {
        blackboard->SetValueAsVector(FleeLocationKey.SelectedKeyName, bestDangerLocation->GetActorLocation());
    }
    else
    {
        // Blind escape fallback
        FVector escapeDir = (selfLocation - playerLocation).GetSafeNormal();
        FNavLocation ProjectedLocation;
        if (navSys->ProjectPointToNavigation(selfLocation + escapeDir * 1000.f, ProjectedLocation, FVector(500.f, 500.f, 500.f)))
        {
            blackboard->SetValueAsVector(FleeLocationKey.SelectedKeyName, ProjectedLocation.Location);
        }
    }

    return EBTNodeResult::Succeeded;
}