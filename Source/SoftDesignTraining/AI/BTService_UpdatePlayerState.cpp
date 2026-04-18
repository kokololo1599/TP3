#include "BTService_UpdatePlayerState.h"
#include "SoftDesignTraining/SoftDesignTraining.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "SoftDesignTraining/SDTUtils.h"
#include "AI/AiAgentGroupManager.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"

UBTService_UpdatePlayerState::UBTService_UpdatePlayerState()
{
    NodeName = "Update Player State";
    bCreateNodeInstance = true;
}

void UBTService_UpdatePlayerState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* selfPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (!selfPawn) return;

    ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!playerCharacter) return;

    UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
    if (!blackboard) return;

    AiAgentGroupManager* groupManager = AiAgentGroupManager::GetInstance();
    if (!groupManager) return;

    FVector detectionStartLocation = selfPawn->GetActorLocation() + selfPawn->GetActorForwardVector() * DetectionCapsuleForwardStartingOffset;
    FVector detectionEndLocation = detectionStartLocation + selfPawn->GetActorForwardVector() * DetectionCapsuleHalfLength * 2;

    TArray<TEnumAsByte<EObjectTypeQuery>> detectionTraceObjectTypes;
    detectionTraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_PLAYER)); // Assuming defined in SoftDesignTraining.h

    TArray<FHitResult> allDetectionHits;
    GetWorld()->SweepMultiByObjectType(allDetectionHits, detectionStartLocation, detectionEndLocation, FQuat::Identity, detectionTraceObjectTypes, FCollisionShape::MakeSphere(DetectionCapsuleRadius));

    // Determine Behavior
    EPlayerInteractionBehavior newBehavior = EPlayerInteractionBehavior::Collect;
    bool bCanSeePlayer = false;

    for (const FHitResult& hit : allDetectionHits)
    {
        if (hit.GetComponent() && hit.GetComponent()->GetCollisionObjectType() == COLLISION_PLAYER)
        {
            if (HasLoSOnHit(hit, GetWorld()))
            {
                bCanSeePlayer = true;
                break;
            }
        }
    }

    const bool bPlayerPoweredUp = SDTUtils::IsPlayerPoweredUp(GetWorld());

    if (bPlayerPoweredUp)
    {
        newBehavior = EPlayerInteractionBehavior::Flee;
        blackboard->ClearValue(PlayerTargetKey.SelectedKeyName);
    }
    else if (bCanSeePlayer)
    {
        groupManager->JoinPursuitGroup(selfPawn);
        newBehavior = EPlayerInteractionBehavior::Chase;
        blackboard->SetValueAsObject(PlayerTargetKey.SelectedKeyName, playerCharacter);
    }
    else if (groupManager->IsAgentInPursuitGroup(selfPawn))
    {
        newBehavior = EPlayerInteractionBehavior::Chase;
        blackboard->SetValueAsObject(PlayerTargetKey.SelectedKeyName, playerCharacter);
    }
    else
    {
        blackboard->ClearValue(PlayerTargetKey.SelectedKeyName);
    }

    blackboard->SetValueAsEnum(StateKey.SelectedKeyName, (uint8)newBehavior);

    //DrawDebugCapsule(GetWorld(), detectionStartLocation + DetectionCapsuleHalfLength * selfPawn->GetActorForwardVector(), DetectionCapsuleHalfLength, DetectionCapsuleRadius, selfPawn->GetActorQuat() * selfPawn->GetActorUpVector().ToOrientationQuat(), FColor::Blue);
}

bool UBTService_UpdatePlayerState::HasLoSOnHit(const FHitResult& hit, UWorld* World)
{
    if (!hit.GetComponent()) return false;

    TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
    TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

    FVector hitDirection = hit.ImpactPoint - hit.TraceStart;
    hitDirection.Normalize();

    FHitResult losHit;
    FCollisionQueryParams queryParams;
    queryParams.AddIgnoredActor(hit.GetActor());

    World->LineTraceSingleByObjectType(losHit, hit.TraceStart, hit.ImpactPoint + hitDirection, TraceObjectTypes, queryParams);

    return losHit.GetActor() == nullptr;
}
