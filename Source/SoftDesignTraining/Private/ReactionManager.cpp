// Fill out your copyright notice in the Description page of Project Settings.

#include "ReactionManager.h"
#include "SoftDesignTraining/SoftDesignTraining.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "AIBase.h"
#include "ObjectPartition.h"
#include "SoftDesignTraining/SDTUtils.h"

ReactionManager* ReactionManager::m_Instance;

ReactionManager::ReactionManager()
{
}

ReactionManager* ReactionManager::GetInstance()
{
	if (!m_Instance)
	{
		m_Instance = new ReactionManager();
	}

	return m_Instance;
}

void ReactionManager::Destroy()
{
	delete m_Instance;
	m_Instance = nullptr;
}

void ReactionManager::RegisterNPC(AActor* npcCharacter)
{
	m_NPCList.Add(npcCharacter);

    ObjectPartition* op = ObjectPartition::GetInstance();
    op->RegisterObject(npcCharacter);

}

void ReactionManager::UnregisterNPC(AActor* npcCharacter)
{
	m_NPCList.Remove(npcCharacter);

    ObjectPartition* op = ObjectPartition::GetInstance();
    op->UnregisterObject(npcCharacter);

}

AActor* ReactionManager::GetPlayer()
{
    if (m_PlayerInstance.Num() != 0)
    {
        return m_PlayerInstance[0];
    }

    return NULL;
}

void ReactionManager::RegisterPlayer(AActor* player)
{
    m_PlayerInstance.Add(player);
}

void ReactionManager::UnregisterPlayer(AActor* player)
{
    m_PlayerInstance.Remove(player);
}

void ReactionManager::CreateReactionEvent(FVector targetPosition, float radiusSQ, ReactionType reactionType, ReactionLOS reactionLOS)
{
	ReactionEvent newEvent(NULL, targetPosition, reactionType, reactionLOS);

	int npcCount = m_NPCList.Num();
	for (int i = 0; i < npcCount; ++i )
	{
		AActor* npcChar = m_NPCList[i];
		if (npcChar)
		{
			FVector npcPosition = npcChar->GetActorLocation();
			FVector diffToNpc = npcPosition - targetPosition;
			float distNpc = diffToNpc.SizeSquared();

			if ( distNpc < radiusSQ)
			{ 
				UWorld* npcWorld = npcChar->GetWorld();
				FVector npcHead = npcPosition + FVector::UpVector * 200.0f;
				bool hasHit = true;

				if (reactionLOS == ReactionLOS_Visual)
				{
                    FVector targetHigh = targetPosition + FVector::UpVector * 100.0f;
                    hasHit = !SDTUtils::Raycast(npcWorld, npcHead, targetHigh);
				}

				if ( hasHit )
				{
					AAIBase* npcAiBase = Cast<AAIBase>(npcChar);
					if (npcAiBase != NULL)
					{
						npcAiBase->ProcessReaction(&newEvent);
					}
				}				
			}
		}
	}
}