// Fill out your copyright notice in the Description page of Project Settings.

#include "DesignTrainingMovementComponent.h"
#include "SoftDesignTraining/SoftDesignTraining.h"
#include "CoverPropertiesComponent.h"

void UDesignTrainingMovementComponent::SetUpdatedComponent(USceneComponent* NewUpdatedComponent)
{
    Super::SetUpdatedComponent(NewUpdatedComponent);

    SoftDesignCharacterOwner = Cast<ASoftDesignTrainingCharacter>(GetPawnOwner());
}

void UDesignTrainingMovementComponent::InitializeComponent()
{
    Super::InitializeComponent();
}

void UDesignTrainingMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDesignTrainingMovementComponent::TakeCover(const FVector& worldPosition)
{
    SoftDesignCharacterOwner->bIsInCover = true;
}

void UDesignTrainingMovementComponent::StopCover()
{
    SoftDesignCharacterOwner->bIsInCover = false;
}

bool UDesignTrainingMovementComponent::ValidateCover(FHitResult& coverHitResult)
{
    // Very expensive cover validation test

    AActor* hitActor = coverHitResult.GetActor();

    UCoverPropertiesComponent* coverProperties = Cast<UCoverPropertiesComponent>(hitActor->GetComponentByClass(UCoverPropertiesComponent::StaticClass()));

    if (coverProperties != NULL && coverProperties->CoverEnabled)
    {
        float maxSleepTime = (coverProperties->CoverCostMS / 1000.0f);
        float sleepTime = maxSleepTime * 0.7f + (maxSleepTime * 0.3 * FMath::FRand()); // max time +/- 30%
        FPlatformProcess::Sleep(sleepTime);
        
        return true;
    }

    return false;
}
