// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/FSM/AI_FiniteStateMachine.h"
#include "SoftDesignTraining/SoftDesignTraining.h"
#include "AI/FSM/AI_PatrolState.h"
#include "AI/FSM/AI_BlindState.h"
#include "AI/FSM/AI_ChaseState.h"
#include "AI/FSM/AI_FleeState.h"

AI_FiniteStateMachine::AI_FiniteStateMachine()
: m_currentStateId(EStateType::Invalid),
  m_aiOwner(nullptr)
{
}

AI_FiniteStateMachine::~AI_FiniteStateMachine()
{
    m_possibleStates.Reset();
}

void AI_FiniteStateMachine::Init(AAIBase* aiOwner)
{
    m_aiOwner = aiOwner;
    CreateDesiredStates();
}

void AI_FiniteStateMachine::UpadateFSM(float deltaTime)
{
    UpdateCurrentState(deltaTime);
}

void AI_FiniteStateMachine::CreateDesiredStates()
{
    m_possibleStates.Add(new AI_BaseState());
    m_possibleStates.Add(new AI_PatrolState());
    m_possibleStates.Add(new AI_BlindState());
    m_possibleStates.Add(new AI_ChaseState());


    m_currentStateId = EStateType::Patrol;
}

void AI_FiniteStateMachine::UpdateCurrentState(float deltaTime)
{
    //Evaluate if the agent needs to transition to a new state
    EStateType nextStateId = EStateType::Invalid;
    if (EvaluateCurrentStateTransitions(nextStateId))
    {
        ExecuteTransition(nextStateId);
    }
    else //Keep updating the current State
    {
        GetCurrentState()->OnUpdate(deltaTime,m_aiOwner);
    }
}

bool AI_FiniteStateMachine::EvaluateCurrentStateTransitions(EStateType& nextStateId)
{
    bool needsTransition = false;
    
    //Does agent needs to transition and to which state
    GetCurrentState()->OnEvaluateTransitions(needsTransition, nextStateId,m_aiOwner);

    return needsTransition;
}

void AI_FiniteStateMachine::ExecuteTransition(EStateType nextStateId)
{
    //Exit current state
    GetCurrentState()->OnExit(nextStateId,m_aiOwner);
    //Enter next state
    GetState(nextStateId)->OnEnter(m_currentStateId,m_aiOwner);
    //Next state becomes current, will be updated next frame
    m_currentStateId = nextStateId;
}

AI_BaseState* AI_FiniteStateMachine::GetCurrentState()
{
    
    for (AI_BaseState* baseState : m_possibleStates)
    {
        if (baseState->GetStateId() == m_currentStateId)
        {
            return baseState;
        }
    }

    //invalid state
    return GetState(EStateType::Invalid);
}

AI_BaseState* AI_FiniteStateMachine::GetState(EStateType stateId)
{
   
    for(AI_BaseState* baseState : m_possibleStates)
    {
        if (baseState->GetStateId() == stateId)
        {
            return baseState;
        }
    }

    //invalid state
    return GetState(EStateType::Invalid);
}




