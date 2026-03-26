// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI_StateTypes.h"
#include "AI_BaseState.h"
class AAIBase;
/**
 * 
 */
class SOFTDESIGNTRAINING_API AI_FiniteStateMachine
{
public:
	AI_FiniteStateMachine();
	~AI_FiniteStateMachine();

    void                 Init(AAIBase* aiOwner);
    void                 UpadateFSM(float deltaTime);

private:

    void                 CreateDesiredStates();
    void                 UpdateCurrentState(float deltaTime);
    bool                 EvaluateCurrentStateTransitions(EStateType& nextStateId);
    void                 ExecuteTransition(EStateType nextStateId);

    AI_BaseState*        GetCurrentState();
    AI_BaseState*        GetState(EStateType stateId);

    TArray<AI_BaseState*> m_possibleStates;
    EStateType           m_currentStateId;
    AAIBase*             m_aiOwner;
};
