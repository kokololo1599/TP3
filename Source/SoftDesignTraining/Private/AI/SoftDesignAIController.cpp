// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SoftDesignAIController.h"
#include "SoftDesignTraining/SoftDesignTraining.h"
#include "AIBase.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

ASoftDesignAIController::ASoftDesignAIController()
: m_targetPosBBKeyID(0),
  m_isTargetSeenBBKeyID(0)
{
	m_behaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	m_blackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

ASoftDesignAIController::~ASoftDesignAIController()
{

}

void ASoftDesignAIController::StartBehaviorTree(APawn* pawn)
{
    if (AAIBase* aiBaseCharacter = Cast<AAIBase>(pawn))
    {
        if (aiBaseCharacter->GetBehaviorTree())
        {
            m_behaviorTreeComponent->StartTree(*aiBaseCharacter->GetBehaviorTree());
        }
    }
}

void ASoftDesignAIController::StopBehaviorTree(APawn* pawn)
{
    if (AAIBase* aiBaseCharacter = Cast<AAIBase>(pawn))
    {
        if (aiBaseCharacter->GetBehaviorTree())
        {
            m_behaviorTreeComponent->StopTree();
        }
    }
}

void ASoftDesignAIController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	if (AAIBase* aiBaseCharacter = Cast<AAIBase>(pawn))
	{
		if (aiBaseCharacter->GetBehaviorTree())
		{
			m_blackboardComponent->InitializeBlackboard(*aiBaseCharacter->GetBehaviorTree()->BlackboardAsset);

            m_targetPosBBKeyID = m_blackboardComponent->GetKeyID("TargetPos");
            m_isTargetSeenBBKeyID = m_blackboardComponent->GetKeyID("TargetIsSeen");
            m_nextPatrolDestinationBBKeyID = m_blackboardComponent->GetKeyID("NextPatrolDest");
            m_currentPatrolDestinationBBKeyID = m_blackboardComponent->GetKeyID("CurrentPatrolDest");

            //Set this agent in the BT
            m_blackboardComponent->SetValue<UBlackboardKeyType_Object>(m_blackboardComponent->GetKeyID("SelfActor"), pawn);
		}
	}
}