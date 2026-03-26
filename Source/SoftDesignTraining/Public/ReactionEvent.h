// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */

enum ReactionType
{
	ReactionType_Bang,
	ReactionType_Bing,
	ReactionType_Boom,
    ReactionType_Blind,
    ReactionType_Explosion,
    ReactionType_ForceFlee,
	ReactionType_PlacedBomb,
	ReactionType_Count,
};

enum ReactionLOS
{
	ReactionLOS_Visual,
	ReactionLOS_Sound,
};

struct ReactionEvent
{
	ReactionEvent();
	ReactionEvent(AActor* aActor, FVector targetPosition, ReactionType reactionType, ReactionLOS reactionLOS);
	
	AActor* m_Source;
	FVector m_TargetPosition;
	ReactionType m_ReactionType;
	ReactionLOS m_ReactionLOS;
};