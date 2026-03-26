// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/WorldSettings.h"
#include "SDT_WorldSettings.generated.h"

class AAIBase;
/**
 * 
 */
UCLASS()
class SOFTDESIGNTRAINING_API ASDT_WorldSettings : public AWorldSettings
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, Category = "Globals", BlueprintReadWrite)
    int32 numSpawnedCharacters = 125;

    UPROPERTY(EditAnywhere, Category = "Globals", BlueprintReadWrite)
    int32 spawnRegionSize = 3000;

    UPROPERTY(EditAnywhere, Category = "Globals", BlueprintReadWrite)
    FVector spawnRegionCenter;

    UPROPERTY(EditAnywhere, Category = "Globals", BlueprintReadWrite)
    bool spawnUnspawnTest = false;

    UPROPERTY(EditAnywhere, Category = "Globals")
    TSubclassOf< AAIBase > spawnTemplate;

    float FixupDeltaSeconds(float DeltaSeconds, float RealDeltaSeconds) override;

    void NotifyBeginPlay() override;
    void UpdateSpawn(UWorld* world, AActor* owner);

protected:
    FVector GenerateSpawnPos();

    int32 curRequiredNumCharacters = 0;
    TArray<AActor*> spawnedActors;
};
