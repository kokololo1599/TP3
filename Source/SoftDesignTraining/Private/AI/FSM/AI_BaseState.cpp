// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/FSM/AI_BaseState.h"
#include "SoftDesignTraining/SoftDesignTraining.h"
#include "AIBase.h"

AI_BaseState::AI_BaseState()
:m_stateType(EStateType::Invalid)
{
}

AI_BaseState::~AI_BaseState()
{
}

void AI_BaseState::OnCreate()
{

}

void AI_BaseState::OnEnter(EStateType fromStateId,AAIBase* aiOwner)
{

}

void AI_BaseState::OnUpdate(float deltTime,AAIBase* aiOwner)
{

}

void AI_BaseState::OnExit(EStateType toStateId,AAIBase* aiOwner)
{

}

void AI_BaseState::OnEvaluateTransitions(bool& sucess, EStateType& transitionStateId,AAIBase* aiOwner)
{
    sucess = false;
    transitionStateId = EStateType::Invalid;
}
