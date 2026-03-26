// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/FSM/AI_PatrolState.h"
#include "SoftDesignTraining/SoftDesignTraining.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIBase.h"

AI_PatrolState::AI_PatrolState()
{
    m_stateType = EStateType::Patrol;
}

AI_PatrolState::~AI_PatrolState()
{
}

void AI_PatrolState::OnCreate()
{

}

void AI_PatrolState::OnEnter(EStateType fromStateId,AAIBase* aiOwner)
{
    if (aiOwner)
    {
        if (fromStateId == EStateType::Chase)// || fromStateId == EStateType::Flee)
        {
            aiOwner->SetCurrentPatrolDestination(aiOwner->GetNextPatrolDestination());
            if (aiOwner->GetNextPatrolDestination() != FVector::ZeroVector)
            {
                UAIBlueprintHelperLibrary::SimpleMoveToLocation(aiOwner->GetController(), aiOwner->GetNextPatrolDestination());
            }
        }
    }
}

void AI_PatrolState::OnUpdate(float deltTime,AAIBase* aiOwner)
{
    if (aiOwner)
    {
        FVector nextPatrolPosition = FVector::ZeroVector;
        if ((aiOwner->GetActorLocation() - aiOwner->GetNextPatrolDestination()).Size2D() < 100.f)
        {
            aiOwner->ProcessNextPatrolDestination(nextPatrolPosition);
        }

        if (nextPatrolPosition != FVector::ZeroVector)
        {
            UAIBlueprintHelperLibrary::SimpleMoveToLocation(aiOwner->GetController(), nextPatrolPosition);
        }

        aiOwner->ProcessAvoidance();
    }
}

void AI_PatrolState::OnExit(EStateType toStateId,AAIBase* aiOwner)
{

}

void AI_PatrolState::OnEvaluateTransitions(bool& sucess, EStateType& transitionStateId,AAIBase* aiOwner)
{
    sucess = false;
    //Evaluate in order of priority
    if (ShouldTransitionToBlindState(aiOwner))
    {
        transitionStateId = EStateType::Blind;
        sucess = true;
    }
    else if (ShouldTransitionToChaseState(aiOwner))
    {
        transitionStateId = EStateType::Chase;
        sucess = true;
    }
}

bool AI_PatrolState::ShouldTransitionToBlindState(AAIBase* aiOwner)
{
    return aiOwner->IsAgentBlind();
}

bool AI_PatrolState::ShouldTransitionToChaseState(AAIBase* aiOwner)
{
    aiOwner->DetectPlayer();
    return aiOwner->IsTargetPlayerSeen();
}