// Fill out your copyright notice in the Description page of Project Settings.

#include "BT_SDTAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SDTUtils.h"

ABT_SDTAIController::ABT_SDTAIController(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer),
      m_playerPositionBBKeyID(0),
      m_isPlayerDetectedBBKeyID(0),
      m_lastSeenBBKeyID(0),
      m_isPlayerPoweredUpBBKeyID(0),
      m_fleePositionBBKeyID(0),
      m_collectiblePositionBBKeyID(0),
      m_shouldInvestigateBBKeyID(0),
      m_groupTargetPositionBBKeyID(0)
{
    m_blackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
    m_behaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void ABT_SDTAIController::BeginPlay()
{
    AAIController::BeginPlay();

    m_blackboardComponent->InitializeBlackboard(*(m_aiBehaviorTree->BlackboardAsset));

    m_playerPositionBBKeyID = m_blackboardComponent->GetKeyID("PlayerPosition");
    m_isPlayerDetectedBBKeyID = m_blackboardComponent->GetKeyID("PlayerDetected");
    m_lastSeenBBKeyID = m_blackboardComponent->GetKeyID("LastSeen");

    m_isPlayerPoweredUpBBKeyID = m_blackboardComponent->GetKeyID("PlayerPoweredUp");
    m_fleePositionBBKeyID = m_blackboardComponent->GetKeyID("FleePosition");

    m_collectiblePositionBBKeyID = m_blackboardComponent->GetKeyID("CollectiblePosition");

    m_shouldInvestigateBBKeyID = m_blackboardComponent->GetKeyID("ShouldInvestigate");
    m_groupTargetPositionBBKeyID = m_blackboardComponent->GetKeyID("GroupTargetPosition");

    // Set this agent in the BT
    m_blackboardComponent->SetValue<UBlackboardKeyType_Object>(m_blackboardComponent->GetKeyID("SelfActor"), GetPawn());
    TimeSpentFindPlayer = m_blackboardComponent->GetKeyID("TimeSpentFindPlayer");
    TimeSpentFindCollectible = m_blackboardComponent->GetKeyID("TimeSpentFindCollectible");
    TimeSpentFindFleePoint = m_blackboardComponent->GetKeyID("TimeSpentFindFleePoint");

    m_behaviorTreeComponent->StartTree(*m_aiBehaviorTree);


}

void ABT_SDTAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

}



void ABT_SDTAIController::PerformDetectPlayer()
{
    auto selfPawn = GetPawn();

    auto movcomp = GetCharacter()->GetMovementComponent();
    if (movcomp->IsFlying() || movcomp->IsFalling())
    {
        return;
    }

    FVector detectionStartLocation = selfPawn->GetActorLocation() + selfPawn->GetActorForwardVector() * m_DetectionCapsuleForwardStartingOffset;
    FVector detectionEndLocation = detectionStartLocation + selfPawn->GetActorForwardVector() * m_DetectionCapsuleHalfLength * 2;
    DetectRotation = selfPawn->GetActorQuat() * selfPawn->GetActorUpVector().ToOrientationQuat();
    DetectionShape = detectionStartLocation + m_DetectionCapsuleHalfLength * GetPawn()->GetActorForwardVector();

    TArray<TEnumAsByte<EObjectTypeQuery>> detectionTraceObjectTypes;
    detectionTraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_PLAYER));

    TArray<FHitResult> allDetectionHits;
    GetWorld()->SweepMultiByObjectType(allDetectionHits, detectionStartLocation, detectionEndLocation, FQuat::Identity, detectionTraceObjectTypes, FCollisionShape::MakeSphere(m_DetectionCapsuleRadius));

    FHitResult detectionHit;
    GetHightestPriorityDetectionHit(allDetectionHits, detectionHit);

    auto playerDetected = detectionHit.GetComponent() && (detectionHit.GetComponent()->GetCollisionObjectType() == COLLISION_PLAYER);

    if (playerDetected)
    {
        // Direct raycast to check for walls
        FHitResult hit;
        GetWorld()->LineTraceSingleByChannel(hit, detectionStartLocation, detectionHit.ImpactPoint, ECollisionChannel::ECC_WorldStatic);
        playerDetected = hit.GetComponent() && (hit.GetComponent()->GetCollisionObjectType() == COLLISION_PLAYER);
    }

    if (playerDetected)
    {
        DrawDebugLine(GetWorld(), detectionStartLocation, detectionHit.ImpactPoint, FColor::Red, false, 0.1f);
        m_blackboardComponent->SetValue<UBlackboardKeyType_Bool>(m_isPlayerDetectedBBKeyID, true);
        m_blackboardComponent->SetValue<UBlackboardKeyType_Vector>(m_playerPositionBBKeyID, detectionHit.GetActor()->GetActorLocation());
        m_blackboardComponent->SetValue<UBlackboardKeyType_Float>(m_lastSeenBBKeyID, UGameplayStatics::GetRealTimeSeconds(GetWorld()));
    }
    else
    {
        m_blackboardComponent->SetValue<UBlackboardKeyType_Bool>(m_isPlayerDetectedBBKeyID, false);
    }
}


