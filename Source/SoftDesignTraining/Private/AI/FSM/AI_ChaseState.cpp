// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/FSM/AI_ChaseState.h"
#include "SoftDesignTraining/SoftDesignTraining.h"
#include "AIBase.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AI_ChaseState::AI_ChaseState()
{
    m_stateType = EStateType::Chase;
}

AI_ChaseState::~AI_ChaseState()
{
}

void AI_ChaseState::OnCreate()
{

}

void AI_ChaseState::OnEnter(EStateType fromStateId, AAIBase* aiOwner)
{
   
}

void AI_ChaseState::OnUpdate(float deltTime, AAIBase* aiOwner)
{
    if (aiOwner)
    {
        DrawDebugSphere(aiOwner->GetWorld(), aiOwner->GetActorLocation() + FVector(0.f, 0.f, 100.f), 15.0f, 32, FColor::Black);

        FVector targetPosition = aiOwner->GetTargetPlayerPos();

        if (targetPosition != FVector::ZeroVector)
        {
            UAIBlueprintHelperLibrary::SimpleMoveToLocation(aiOwner->GetController(), targetPosition);
        }

        aiOwner->ProcessAvoidance();
    }
}

void AI_ChaseState::OnExit(EStateType toStateId, AAIBase* aiOwner)
{

}

void AI_ChaseState::OnEvaluateTransitions(bool& sucess, EStateType& transitionStateId, AAIBase* aiOwner)
{
    sucess = false;
    //Evaluate in order of priority
    if (ShouldTransitionToBlindState(aiOwner))
    {
        transitionStateId = EStateType::Blind;
        sucess = true;
    }
    else if (ShouldTransitionToPatrolState(aiOwner))
    {
        transitionStateId = EStateType::Patrol;
        sucess = true;
    }
}

bool AI_ChaseState::ShouldTransitionToBlindState(AAIBase* aiOwner)
{
    return aiOwner->IsAgentBlind();
}

bool AI_ChaseState::ShouldTransitionToPatrolState(AAIBase* aiOwner)
{
    aiOwner->DetectPlayer();
    return !aiOwner->IsTargetPlayerSeen();
}

bool AI_ChaseState::ShouldTransitionToFleeState(AAIBase* aiOwner)
{
    return false;
}
