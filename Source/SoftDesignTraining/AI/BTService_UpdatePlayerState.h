#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SoftDesignTraining\AI\SDTTypes.h"
#include "BTService_UpdatePlayerState.generated.h"

UCLASS()
class SOFTDESIGNTRAINING_API UBTService_UpdatePlayerState : public UBTService
{
    GENERATED_BODY()

public:
    UBTService_UpdatePlayerState();

protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
    UPROPERTY(EditAnywhere, Category = "AI")
    FBlackboardKeySelector StateKey;

    UPROPERTY(EditAnywhere, Category = "AI")
    FBlackboardKeySelector PlayerTargetKey;

    UPROPERTY(EditAnywhere, Category = "AI")
    float DetectionCapsuleForwardStartingOffset = 50.f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float DetectionCapsuleHalfLength = 500.f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float DetectionCapsuleRadius = 250.f;

private:
    bool HasLoSOnHit(const FHitResult& hit, UWorld* World);
};