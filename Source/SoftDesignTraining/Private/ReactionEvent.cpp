// Fill out your copyright notice in the Description page of Project Settings.

#include "ReactionEvent.h"
#include "SoftDesignTraining/SoftDesignTraining.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

ReactionEvent::ReactionEvent()
{
	m_Source = NULL;
	m_TargetPosition = FVector::ZeroVector;
	m_ReactionType = ReactionType_Bang;
	m_ReactionLOS = ReactionLOS_Visual;
}

ReactionEvent::ReactionEvent(AActor* aActor, FVector targetPosition, ReactionType reactionType, ReactionLOS reactionLOS)
{
	m_Source = aActor;
	m_TargetPosition = targetPosition;
	m_ReactionType = reactionType;
	m_ReactionLOS = reactionLOS;
}