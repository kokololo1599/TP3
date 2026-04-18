// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SDTAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BT_SDTAIController.generated.h"

class AAgentBudgetManager;

/**
 * */
UCLASS(ClassGroup = AI, config = Game)
class SOFTDESIGNTRAINING_API ABT_SDTAIController : public ASDTAIController
{
    GENERATED_BODY()

public:
    ABT_SDTAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    //virtual void Tick(float deltaTime) override;
    void UpdateAgent();

    UBlackboardComponent* GetBlackboardComponent() const { return m_blackboardComponent; }

    // Getters now return FBlackboard::FKey for compatibility
    FBlackboard::FKey GetPlayerPositionBBKeyID() const { return m_playerPositionBBKeyID; }
    FBlackboard::FKey GetPlayerDetectedBBKeyID() const { return m_isPlayerDetectedBBKeyID; }
    FBlackboard::FKey GetLastSeenBBKeyID() const { return m_lastSeenBBKeyID; }

    FBlackboard::FKey GetPlayerPoweredUpBBKeyID() const { return m_isPlayerPoweredUpBBKeyID; }
    FBlackboard::FKey GetFleePositionBBKeyID() const { return m_fleePositionBBKeyID; }

    FBlackboard::FKey GetCollectiblePositionBBKeyID() const { return m_collectiblePositionBBKeyID; }

    FBlackboard::FKey GetShouldInvestigateBBKeyID() const { return m_shouldInvestigateBBKeyID; }
    FBlackboard::FKey GetGroupTargetPositionBBKeyID() const { return m_groupTargetPositionBBKeyID; }

protected:
    UPROPERTY(EditAnywhere, category = Behavior)
    UBehaviorTree* m_aiBehaviorTree;

private:
    UPROPERTY(transient)
    UBehaviorTreeComponent* m_behaviorTreeComponent;

    UPROPERTY(transient)
    UBlackboardComponent* m_blackboardComponent;

    void PerformDetectPlayer();
    void CalculateGroupTargetPosition();

    // Member variables updated to FBlackboard::FKey
    FBlackboard::FKey m_playerPositionBBKeyID;
    FBlackboard::FKey m_isPlayerDetectedBBKeyID;
    FBlackboard::FKey m_lastSeenBBKeyID;

    FBlackboard::FKey m_isPlayerPoweredUpBBKeyID;
    FBlackboard::FKey m_fleePositionBBKeyID;

    FBlackboard::FKey m_collectiblePositionBBKeyID;

    FBlackboard::FKey m_shouldInvestigateBBKeyID;
    FBlackboard::FKey m_groupTargetPositionBBKeyID;

    AAgentBudgetManager* BudgetManager;
    FBlackboard::FKey TimeSpentFindPlayer;
    FBlackboard::FKey TimeSpentFindCollectible;
    FBlackboard::FKey TimeSpentFindFleePoint;

    // For Debug Draw
    bool PlayerFound = false;
    FVector PlayerLKP = FVector::ZeroVector;
    FVector DetectionShape = FVector::ZeroVector;
    FQuat4d DetectRotation = FQuat4d::Identity;
};
