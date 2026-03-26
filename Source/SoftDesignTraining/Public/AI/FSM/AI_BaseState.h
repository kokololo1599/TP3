// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI_StateTypes.h"

class AAIBase;
/**
 * 
 */
class SOFTDESIGNTRAINING_API AI_BaseState
{
public:
	AI_BaseState();
	virtual ~AI_BaseState();

    virtual void  OnCreate();
    virtual void  OnEnter(EStateType fromStateId,AAIBase* aiOwner);
    virtual void  OnUpdate(float deltaTime,AAIBase* aiOwner);
    virtual void  OnExit(EStateType toStateId,AAIBase* aiOwner);
    virtual void  OnEvaluateTransitions(bool& sucess, EStateType& transitionStateId,AAIBase* aiOwner);

    EStateType    GetStateId() const { return m_stateType; }
protected:
    EStateType    m_stateType;
private:

};
