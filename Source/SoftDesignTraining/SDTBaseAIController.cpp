// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTBaseAIController.h"
#include "AI/AiAgentGroupManager.h"
#include "SoftDesignTraining.h"

ASDTBaseAIController::ASDTBaseAIController(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
    m_ReachedTarget = true;
}

void ASDTBaseAIController::Tick(float deltaTime)
{
    Super::Tick(deltaTime);

    if (APawn* ControlledPawn = GetPawn())
    {
        AiAgentGroupManager::GetInstance()->DrawDebugForAgent(ControlledPawn);
    }

    UpdatePlayerInteraction(deltaTime);
    /*
    if (m_ReachedTarget)
    {
        GoToBestTarget(deltaTime);
    }
    else
    {
        ShowNavigationPath();
    }
    */
}


