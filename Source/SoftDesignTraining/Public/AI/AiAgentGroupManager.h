#pragma once
#include "TargetLKPInfo.h"
#include "AIBase.h"
#include "CoreMinimal.h"

/**
 * 
 */
class SOFTDESIGNTRAINING_API AiAgentGroupManager
{
public:   
    static AiAgentGroupManager* GetInstance();
    static void Destroy();

    void RegisterAIAgent(AAIBase* aiAgent);
    void UnregisterAIAgent(AAIBase* aiAgent);

    TargetLKPInfo GetLKPFromGroup(const FString& targetLabel, bool& targetFound);

private:

    //SINGLETON
    AiAgentGroupManager();
    static AiAgentGroupManager* m_Instance;

    TArray<AAIBase*> m_registeredAgents;

};
