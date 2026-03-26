// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SoftDesignTraining/SoftDesignTrainingCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DesignTrainingMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SOFTDESIGNTRAINING_API UDesignTrainingMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
    virtual void SetUpdatedComponent(USceneComponent* NewUpdatedComponent) override;

protected:

    //Init
    virtual void InitializeComponent() override;

    //Tick
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

protected:

    /** Character movement component belongs to */
    UPROPERTY()
    ASoftDesignTrainingCharacter* SoftDesignCharacterOwner;

public:
    UFUNCTION(BlueprintCallable, Category = "Character Movement: Cover")
    bool ValidateCover(FHitResult& coverHitResult);

    UFUNCTION(BlueprintCallable, Category = "Character Movement: Cover")
    virtual void TakeCover(const FVector& worldPosition);

    UFUNCTION(BlueprintCallable, Category = "Character Movement: Cover")
    virtual void StopCover();
};
