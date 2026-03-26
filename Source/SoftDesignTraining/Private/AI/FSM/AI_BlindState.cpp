// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/FSM/AI_BlindState.h"
#include "SoftDesignTraining/SoftDesignTraining.h"
#include "AIBase.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AI_BlindState::AI_BlindState()
    :m_timeBeforeBlindIsOver(0.f)
{
    m_stateType = EStateType::Blind;
}

AI_BlindState::~AI_BlindState()
{
}

void AI_BlindState::OnCreate()
{

}

void AI_BlindState::OnEnter(EStateType fromStateId, AAIBase* aiOwner)
{
    m_timeBeforeBlindIsOver = 3.f;

    if (aiOwner)
    {
        UAIBlueprintHelperLibrary::SimpleMoveToLocation(aiOwner->GetController(), aiOwner->GetActorLocation());
    }    

    //Debug
    {
        FVector npcPosition = aiOwner->GetActorLocation();
        FVector npcHead = npcPosition + FVector::UpVector * 200.0f;
        UWorld* npcWorld = aiOwner->GetWorld();
        DrawDebugSphere(npcWorld, npcHead, 30.0f, 32, FColor::Red, false, m_timeBeforeBlindIsOver);
    }
}

void AI_BlindState::OnUpdate(float deltaTime, AAIBase* aiOwner)
{
    if (aiOwner)
    {
        if (m_timeBeforeBlindIsOver > 0)
        {
            m_timeBeforeBlindIsOver -= deltaTime;
        }
        else
        {
            aiOwner->SetAgentIsBlind(false);
        }
    }
}

void AI_BlindState::OnExit(EStateType toStateId, AAIBase* aiOwner)
{

}

void AI_BlindState::OnEvaluateTransitions(bool& sucess, EStateType& transitionStateId, AAIBase* aiOwner)
{
    sucess = false;
    //Evaluate in order of priority
    if (ShouldTransitionToChaseState(aiOwner))
    {
        transitionStateId = EStateType::Chase;
        sucess = true;
    }
    else if (ShouldTransitionToPatrolState(aiOwner))
    {
        transitionStateId = EStateType::Patrol;
        sucess = true;
    }
}

bool AI_BlindState::ShouldTransitionToPatrolState(AAIBase* aiOwner)
{
    return !aiOwner->IsAgentBlind();
}

bool AI_BlindState::ShouldTransitionToChaseState(AAIBase* aiOwner)
{
    aiOwner->DetectPlayer();
    return aiOwner->IsTargetPlayerSeen() && !aiOwner->IsAgentBlind();
}
