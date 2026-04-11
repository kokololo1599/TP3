#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindBestFleeLocation.generated.h"

UCLASS()
class SOFTDESIGNTRAINING_API UBTTask_FindBestFleeLocation : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_FindBestFleeLocation();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
    UPROPERTY(EditAnywhere, Category = "AI")
    FBlackboardKeySelector FleeLocationKey;
};