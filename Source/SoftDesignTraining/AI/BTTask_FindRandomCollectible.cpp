#include "BTTask_FindRandomCollectible.h"
#include "SoftDesignTraining/SDTCollectible.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTTask_FindRandomCollectible::UBTTask_FindRandomCollectible()
{
    NodeName = "Find Random Collectible";

    // This allows the task to show up correctly in the BT Editor
    bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_FindRandomCollectible::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

    // FIX: Ensure the user actually picked a Blackboard Key in the BT Editor
    if (TargetLocationKey.SelectedKeyName.IsNone())
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error: TargetLocationKey not set in BT Task details!"));
        return EBTNodeResult::Failed;
    }

    TArray<AActor*> foundCollectibles;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASDTCollectible::StaticClass(), foundCollectibles);

    // If no actors exist in the world, fail immediately
    if (foundCollectibles.Num() == 0)
    {
        return EBTNodeResult::Failed;
    }

    // Keep looking until we find one not on cooldown or run out of options
    while (foundCollectibles.Num() > 0)
    {
        int32 Index = FMath::RandRange(0, foundCollectibles.Num() - 1);
        ASDTCollectible* CollectibleActor = Cast<ASDTCollectible>(foundCollectibles[Index]);

        if (CollectibleActor && !CollectibleActor->IsOnCooldown())
        {
            FVector TargetLocation = CollectibleActor->GetActorLocation();

            // Success: Write to the blackboard
            BlackboardComp->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetLocation);

            return EBTNodeResult::Succeeded;
        }
        else
        {
            // This one was on cooldown or invalid, remove and try again
            foundCollectibles.RemoveAt(Index);
        }
    }

    // If we reach here, all found collectibles were on cooldown
    return EBTNodeResult::Failed;
}