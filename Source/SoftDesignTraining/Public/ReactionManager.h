// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ReactionEvent.h"

/**
 * 
 */

class SOFTDESIGNTRAINING_API ReactionManager
{
private:
	ReactionManager();

	static ReactionManager* m_Instance;

public:
	TArray<AActor*> m_NPCList;
    TArray<AActor*> m_PlayerInstance;

public:
	static ReactionManager* GetInstance();
	static void Destroy();

    void RegisterPlayer(AActor* npcCharacter);
    void UnregisterPlayer(AActor* npcCharacter);
    //tmp
    AActor* GetPlayer();
    //tmp
	void RegisterNPC(AActor* npcCharacter);
	void UnregisterNPC(AActor* npcCharacter);

	void CreateReactionEvent(FVector targetPosition, float radius, ReactionType reactionType, ReactionLOS reactionLOS);
};
