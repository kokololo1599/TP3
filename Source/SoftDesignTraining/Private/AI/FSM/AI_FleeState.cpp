// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/FSM/AI_FleeState.h"
#include "AI/FSM/AI_ChaseState.h"
#include "SoftDesignTraining/SoftDesignTraining.h"
#include "AIBase.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AI_FleeState::AI_FleeState()
    :m_timeToStayInFlee(0.f)
{
    m_stateType = EStateType::Invalid;
}

AI_FleeState::~AI_FleeState()
{
}

void AI_FleeState::OnEnter(EStateType fromStateId, AAIBase* aiOwner)
{
   
}

void AI_FleeState::OnCreate()
{

}

void AI_FleeState::OnUpdate(float deltaTime, AAIBase* aiOwner)
{
   
}

void AI_FleeState::OnExit(EStateType toStateId, AAIBase* aiOwner)
{

}

void AI_FleeState::OnEvaluateTransitions(bool& sucess, EStateType& transitionStateId, AAIBase* aiOwner)
{
    sucess = false;
}

bool AI_FleeState::ShouldTransitionToPatrolState(AAIBase* aiOwner)
{
    return false;
}

bool AI_FleeState::ShouldTransitionToChaseState(AAIBase* aiOwner)
{
    return false;
}

void AI_FleeState::NavigateToPos(const FVector& pos, AAIBase* aiOwner)
{
    UAIBlueprintHelperLibrary::SimpleMoveToLocation(aiOwner->GetController(), pos);
}
