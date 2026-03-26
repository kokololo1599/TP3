// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BehaviorTree/Task/BTTask_IsAgentBlind.h"
#include "SoftDesignTraining/SoftDesignTraining.h"
#include "AI/SoftDesignAIController.h"
#include "SoftDesignTraining/SoftDesignTrainingCharacter.h"
#include "AIBase.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
//#include "AI/Navigation/NavigationSystem.h"

EBTNodeResult::Type UBTTask_IsAgentBlind::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (ASoftDesignAIController* aiController = Cast<ASoftDesignAIController>(OwnerComp.GetAIOwner()))
    {
        if (AAIBase* aiBase = Cast<AAIBase>(aiController->GetCharacter()))
        {
            if (aiBase->IsAgentBlind())
            {
                return EBTNodeResult::Succeeded;
            }
        }

    }

    return EBTNodeResult::Failed;
}


