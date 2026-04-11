#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindRandomCollectible.generated.h"

UCLASS()
class SOFTDESIGNTRAINING_API UBTTask_FindRandomCollectible : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_FindRandomCollectible();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
    UPROPERTY(EditAnywhere, Category = "AI")
    FBlackboardKeySelector TargetLocationKey;
};