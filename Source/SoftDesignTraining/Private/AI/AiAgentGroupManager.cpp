// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AiAgentGroupManager.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"

AiAgentGroupManager* AiAgentGroupManager::m_Instance = nullptr;

AiAgentGroupManager::AiAgentGroupManager()
{
}

AiAgentGroupManager* AiAgentGroupManager::GetInstance()
{
    if (!m_Instance)
    {
        m_Instance = new AiAgentGroupManager();
    }

    return m_Instance;
}

void AiAgentGroupManager::Destroy()
{
    delete m_Instance;
    m_Instance = nullptr;
}

void AiAgentGroupManager::JoinPursuitGroup(APawn* aiAgent)
{
    if (!aiAgent)
    {
        UE_LOG(LogTemp, Warning, TEXT("JoinPursuitGroup called with null agent."));
        return;
    }

    const bool bWasAlreadyInGroup = m_pursuitAgents.Contains(aiAgent);
    m_pursuitAgents.AddUnique(aiAgent);

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("JoinPursuitGroup called for %s. AlreadyInGroup=%s GroupSize=%d"),
        *aiAgent->GetName(),
        bWasAlreadyInGroup ? TEXT("true") : TEXT("false"),
        m_pursuitAgents.Num());

    if (GEngine && false)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            3.f,
            FColor::Yellow,
            FString::Printf(TEXT("JoinPursuitGroup: %s (%d)"), *aiAgent->GetName(), m_pursuitAgents.Num()));
    }
}

void AiAgentGroupManager::LeavePursuitGroup(APawn* aiAgent)
{
    if (!aiAgent)
    {
        UE_LOG(LogTemp, Warning, TEXT("LeavePursuitGroup called with null agent."));
        return;
    }

    const bool bWasInGroup = m_pursuitAgents.Contains(aiAgent);
    m_pursuitAgents.Remove(aiAgent);

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("LeavePursuitGroup called for %s. WasInGroup=%s GroupSize=%d"),
        *aiAgent->GetName(),
        bWasInGroup ? TEXT("true") : TEXT("false"),
        m_pursuitAgents.Num());

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            3.f,
            FColor::Cyan,
            FString::Printf(TEXT("LeavePursuitGroup: %s (%d)"), *aiAgent->GetName(), m_pursuitAgents.Num()));
    }
}

void AiAgentGroupManager::DissolvePursuitGroup()
{
    UE_LOG(LogTemp, Warning, TEXT("DissolvePursuitGroup called. PreviousGroupSize=%d"), m_pursuitAgents.Num());

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            3.f,
            FColor::Red,
            FString::Printf(TEXT("DissolvePursuitGroup: clearing %d agents"), m_pursuitAgents.Num()));
    }

    m_pursuitAgents.Empty();
}

bool AiAgentGroupManager::IsAgentInPursuitGroup(const APawn* aiAgent) const
{
    return aiAgent && m_pursuitAgents.Contains(const_cast<APawn*>(aiAgent));
}

void AiAgentGroupManager::DrawDebugForAgent(const APawn* aiAgent) const
{
    if (!aiAgent || !IsAgentInPursuitGroup(aiAgent))
    {
        return;
    }

    DrawDebugString(
        aiAgent->GetWorld(),
        FVector(0.f, 0.f, 140.f),
        FString::Printf(TEXT("IN GROUP: %d"), GetAgentIndex(aiAgent)),
        const_cast<APawn*>(aiAgent),
        FColor::Yellow,
        0.f,
        false);
}
int AiAgentGroupManager::GetAgentIndex(const APawn* aiAgent) const
{
    if (!aiAgent) return -1;

    // Find returns INDEX_NONE (-1) if not found
    return m_pursuitAgents.Find(const_cast<APawn*>(aiAgent));
}

int AiAgentGroupManager::GetGroupSize() const
{
    return m_pursuitAgents.Num();
}