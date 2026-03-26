// Fill out your copyright notice in the Description page of Project Settings.
#include "AI/BehaviorTree/Task/BTTask_ProcessPatrolDestination.h"
#include "SoftDesignTraining/SoftDesignTraining.h"
#include "AI/SoftDesignAIController.h"
#include "SoftDesignTraining/SoftDesignTrainingCharacter.h"
#include "AIBase.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
//#include "AI/Navigation/NavigationSystem.h"

EBTNodeResult::Type UBTTask_ProcessPatrolDestination::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (ASoftDesignAIController* aiController = Cast<ASoftDesignAIController>(OwnerComp.GetAIOwner()))
    {
        FVector nextPatrolDestination = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(aiController->GetNextPatrolDestinationKeyID());
        FVector currentPatrolDestination = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(aiController->GetCurrentPatrolDestinationKeyID());
       
        //agent has a new patrol 
        if (nextPatrolDestination != currentPatrolDestination)
        {
            OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(aiController->GetCurrentPatrolDestinationKeyID(), nextPatrolDestination);
            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}


