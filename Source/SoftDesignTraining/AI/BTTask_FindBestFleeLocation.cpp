#include "BTTask_FindBestFleeLocation.h"
#include "SoftDesignTraining/SDTFleeLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
UBTTask_FindBestFleeLocation::UBTTask_FindBestFleeLocation()
{
    NodeName = "Find Best Flee Location";
}

EBTNodeResult::Type UBTTask_FindBestFleeLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    APawn* selfPawn = OwnerComp.GetAIOwner()->GetPawn();
    ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    if (!selfPawn || !playerCharacter)
    {
        return EBTNodeResult::Failed;
    }

    float bestLocationScore = 0.f;
    ASDTFleeLocation* bestFleeLocation = nullptr;

    for (TActorIterator<ASDTFleeLocation> actorIterator(GetWorld(), ASDTFleeLocation::StaticClass()); actorIterator; ++actorIterator)
    {
        ASDTFleeLocation* fleeLocation = Cast<ASDTFleeLocation>(*actorIterator);
        if (fleeLocation)
        {
            float distToFleeLocation = FVector::Dist(fleeLocation->GetActorLocation(), playerCharacter->GetActorLocation());

            FVector selfToPlayer = playerCharacter->GetActorLocation() - selfPawn->GetActorLocation();
            selfToPlayer.Normalize();

            FVector selfToFleeLocation = fleeLocation->GetActorLocation() - selfPawn->GetActorLocation();
            selfToFleeLocation.Normalize();

            float fleeLocationToPlayerAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(selfToPlayer, selfToFleeLocation)));
            float locationScore = distToFleeLocation + fleeLocationToPlayerAngle * 100.f;

            if (locationScore > bestLocationScore)
            {
                bestLocationScore = locationScore;
                bestFleeLocation = fleeLocation;
            }
        }
    }

    if (bestFleeLocation)
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(FleeLocationKey.SelectedKeyName, bestFleeLocation->GetActorLocation());
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}