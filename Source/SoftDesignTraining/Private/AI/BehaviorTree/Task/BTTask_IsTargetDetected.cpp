// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BehaviorTree/Task/BTTask_IsTargetDetected.h"
#include "SoftDesignTraining/SoftDesignTraining.h"
#include "AI/SoftDesignAIController.h"
#include "SoftDesignTraining/SoftDesignTrainingCharacter.h"
#include "AIBase.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"


EBTNodeResult::Type UBTTask_IsTargetDetected::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (ASoftDesignAIController* aiController = Cast<ASoftDesignAIController>(OwnerComp.GetAIOwner()))
    {
        if (OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Bool>(aiController->GetTargetSeenKeyID()))
        {
            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}



