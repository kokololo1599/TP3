#pragma once

#include "CoreMinimal.h"

class APawn;

/**
 * 
 */
class SOFTDESIGNTRAINING_API AiAgentGroupManager
{
public:   
    static AiAgentGroupManager* GetInstance();
    static void Destroy();

    void JoinPursuitGroup(APawn* aiAgent);
    void LeavePursuitGroup(APawn* aiAgent);
    void DissolvePursuitGroup();
    bool IsAgentInPursuitGroup(const APawn* aiAgent) const;
    void DrawDebugForAgent(const APawn* aiAgent) const;
    int GetAgentIndex(const APawn* aiAgent) const;
    int GetGroupSize() const;

    FVector PlayerLKP;

private:

    //SINGLETON
    AiAgentGroupManager();
    static AiAgentGroupManager* m_Instance;

    TArray<APawn*> m_pursuitAgents;

};
