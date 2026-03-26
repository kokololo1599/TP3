// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BehaviorTree/Task/BTTask_ProcessAvoidance.h"
#include "SoftDesignTraining/SoftDesignTraining.h"
#include "AI/SoftDesignAIController.h"
#include "SoftDesignTraining/SoftDesignTrainingCharacter.h"
#include "AIBase.h"
#include "ObjectPartition.h"
#include "DrawDebugHelpers.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
//#include "AI/Navigation/NavigationSystem.h"



EBTNodeResult::Type UBTTask_ProcessAvoidance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (ASoftDesignAIController* aiController = Cast<ASoftDesignAIController>(OwnerComp.GetAIOwner()))
    {
        if (AAIBase* aiBase = Cast<AAIBase>(aiController->GetCharacter()))
        {
            FVector npcPosition = aiBase->GetActorLocation();
            FVector npcHead = npcPosition + FVector::UpVector * 200.0f;
            UWorld* npcWorld = GetWorld();

            ObjectPartition* op = ObjectPartition::GetInstance();
            if (op)
            {
                TArray<AActor*> actors;
                op->GatherObjectsAroundPosition(aiBase, npcPosition, 250.f, actors);
                if (actors.Num() > 0)
                {
                    DrawDebugSphere(npcWorld, npcHead, 20.0f, 32, FColor::Magenta);
                }
            }
        }
    }

   

    return EBTNodeResult::InProgress;
}
