#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CalculateTacticalChasePos.generated.h"

UCLASS()
class SOFTDESIGNTRAINING_API UBTTask_CalculateTacticalChasePos : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_CalculateTacticalChasePos();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
    // Expose this so you can select your "PursuitTargetLocation" vector key in the editor
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    struct FBlackboardKeySelector ChaseLocationKey;
};