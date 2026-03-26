// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI_BaseState.h"
/**
 * 
 */
class SOFTDESIGNTRAINING_API AI_FleeState : public AI_BaseState
{
public:
    AI_FleeState();
	virtual ~AI_FleeState() override;

    virtual void  OnCreate() override;
    virtual void  OnEnter(EStateType fromStateId, AAIBase* aiOwner) override;
    virtual void  OnUpdate(float deltaTime, AAIBase* aiOwner) override;
    virtual void  OnExit(EStateType toStateId, AAIBase* aiOwner) override;
    virtual void  OnEvaluateTransitions(bool& sucess, EStateType& transitionStateId, AAIBase* aiOwner) override;
private:

    bool          ShouldTransitionToPatrolState(AAIBase* aiOwner);
    bool          ShouldTransitionToChaseState(AAIBase* aiOwner);

    void          NavigateToPos(const FVector& pos, AAIBase* aiOwner);

    float         m_timeToStayInFlee;
    FVector       m_fleeDestination;
};
