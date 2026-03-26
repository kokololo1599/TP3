// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoverDetectionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOFTDESIGNTRAINING_API UCoverDetectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCoverDetectionComponent();
    
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

    bool DetectCoverNow(FVector& outCoverLocation, bool showDebugDisplay = false);

    UPROPERTY(EditAnywhere)
    bool DetectCoverEveryFrame;
protected:

    bool DetectCoverInDirection(const FVector& detectionStart, const FRotator& detectionRotation, FVector& outCoverLocation, bool showDebugDisplay = false);
    void DrawDebugPuppeteerCover(bool availableCover);

    UPROPERTY()
    class APawn* m_PawnOwner;
};
