// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BehaviorTree/Service/BTService_TryAcquireNewPatrolPos.h"
#include "SoftDesignTraining/SoftDesignTraining.h"
#include "AI/SoftDesignAIController.h"
#include "SoftDesignTraining/SoftDesignTrainingCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h" // Added for safety

UBTService_TryAcquireNewPatrolPos::UBTService_TryAcquireNewPatrolPos()
{
    bCreateNodeInstance = true;
}

void UBTService_TryAcquireNewPatrolPos::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if (ASoftDesignAIController* aiController = Cast<ASoftDesignAIController>(OwnerComp.GetAIOwner()))
    {
        // Replaced AAIBase with ASoftDesignTrainingCharacter
        if (ASoftDesignTrainingCharacter* aiCharacter = Cast<ASoftDesignTrainingCharacter>(aiController->GetCharacter()))
        {
            FVector nextPatrolDestination = FVector::ZeroVector;

            /* * NOTE: The following logic is commented out because the patrol
             * functions do not exist in ASoftDesignTrainingCharacter yet.
             * This prevents "Undefined" or "Not Found" compilation errors.
             */

             /*
             // Check if we need a new destination
             if (FVector::ZeroVector == aiCharacter->GetCurrentPatrolDestination()) // Does not exist
             {
                 nextPatrolDestination = aiCharacter->GetNextPatrolDestination(); // Does not exist
                 aiCharacter->SetCurrentPatrolDestination(nextPatrolDestination); // Does not exist
             }
             else if ((aiCharacter->GetActorLocation() - aiCharacter->GetNextPatrolDestination()).Size2D() < 100.f)
             {
                 aiCharacter->ProcessNextPatrolDestination(nextPatrolDestination); // Does not exist
             }
             */
            
             // Currently returning a "null" answer (ZeroVector) as the destination doesn't exist
            nextPatrolDestination = FVector::ZeroVector;

            // Write to blackboard if a valid destination was found (currently will stay Zero)
            if (nextPatrolDestination != FVector::ZeroVector)
            {
                OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(aiController->GetNextPatrolDestinationKeyID(), nextPatrolDestination);
            }
        }
    }
}