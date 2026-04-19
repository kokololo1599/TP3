#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AITimeSlicerSubsystem.generated.h"

class AAIController;

UCLASS()
class SOFTDESIGNTRAINING_API UAITimeSlicerSubsystem : public UTickableWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Tick(float DeltaTime) override;
    virtual TStatId GetStatId() const override;

    UFUNCTION(BlueprintCallable, Category = "Optimization")
    void RegisterController(AAIController* Controller);

    UFUNCTION(BlueprintCallable, Category = "Optimization")
    void UnregisterController(AAIController* Controller);

    // Fixed time budget per frame (e.g., 0.005 seconds = 5 milliseconds)
    UPROPERTY(EditAnywhere, Category = "Optimization")
    double MaxTimeBudget = 0.00003;

private:
    UPROPERTY()
    TArray<AAIController*> ActiveControllers;

    int32 CurrentIndex = 0;
};
