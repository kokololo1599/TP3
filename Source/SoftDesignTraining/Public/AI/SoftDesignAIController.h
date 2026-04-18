// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "SoftDesignAIController.generated.h"

UCLASS()
class SOFTDESIGNTRAINING_API ASoftDesignAIController : public AAIController
{
	GENERATED_BODY()
public:
	ASoftDesignAIController();
	~ASoftDesignAIController();

	UBehaviorTreeComponent* GetBehaviorTreeComponent() const { return m_behaviorTreeComponent; }
	UBlackboardComponent*   GetBlackBoardComponent() const { return m_blackboardComponent; }

    uint16                   GetTargetPosBBKeyID() const { return m_targetPosBBKeyID; }
    uint16                   GetTargetSeenKeyID() const { return m_isTargetSeenBBKeyID; }
    uint16                   GetNextPatrolDestinationKeyID() const { return m_nextPatrolDestinationBBKeyID; }
    uint16                   GetCurrentPatrolDestinationKeyID() const { return m_currentPatrolDestinationBBKeyID; }

    void                    StartBehaviorTree(APawn* pawn);
    void                    StopBehaviorTree(APawn* pawn);

protected:
	virtual void OnPossess(APawn* pawn) override;
private:

	UPROPERTY(transient)
	UBehaviorTreeComponent* m_behaviorTreeComponent;

	UPROPERTY(transient)
	UBlackboardComponent* m_blackboardComponent;


    uint16  m_targetPosBBKeyID;
    uint16  m_isTargetSeenBBKeyID;
    uint16  m_nextPatrolDestinationBBKeyID;
    uint16  m_currentPatrolDestinationBBKeyID;
};
