// Fill out your copyright notice in the Description page of Project Settings.

#include "SDT_WorldSettings.h"
#include "SoftDesignTraining/SoftDesignTraining.h"
#include "SoftDesignTraining/SoftDesignTrainingCharacter.h"
#include "Engine/World.h"
#include "AIBase.h"

float ASDT_WorldSettings::FixupDeltaSeconds(float DeltaSeconds, float RealDeltaSeconds)
{
    return AWorldSettings::FixupDeltaSeconds(DeltaSeconds, RealDeltaSeconds);
}


void ASDT_WorldSettings::NotifyBeginPlay()
{
    //SoftDesignTrainingModuleImpl::WorldBeginPlay();
    AWorldSettings::NotifyBeginPlay();
}


void ASDT_WorldSettings::UpdateSpawn(UWorld* world, AActor* owner)  // temp: todo create spawner class
{
    if (spawnedActors.Num() == 0)
    {
        curRequiredNumCharacters = numSpawnedCharacters;
    }

    if (spawnedActors.Num() < curRequiredNumCharacters && spawnTemplate != nullptr)
    {
        int32 maxSpawnPerframe = 3;
        uint32 maxTry = 100;
        for (uint32 i = 0; i < maxTry && maxSpawnPerframe > 0 && spawnedActors.Num() < numSpawnedCharacters; i++)
        {
            FVector spawnPos = GenerateSpawnPos();
            AActor* spawnedActor = world->SpawnActor(spawnTemplate, &spawnPos);
            if (spawnedActor)
            {
                ((AAIBase *)spawnedActor)->SpawnDefaultController();
                spawnedActors.Add(spawnedActor);
                maxSpawnPerframe--;
            }
        }
    }
    else if(spawnUnspawnTest)
    {
        curRequiredNumCharacters = 0;
    }
    
    if (spawnedActors.Num() > curRequiredNumCharacters && spawnedActors.Num() > 0)
    {
        spawnedActors[0]->Destroy();
        spawnedActors.RemoveAt(0);
    }
}


FVector ASDT_WorldSettings::GenerateSpawnPos()
{
    FVector spawnPos(spawnRegionCenter.X + (rand() % spawnRegionSize - (spawnRegionSize / 2)), spawnRegionCenter.Y + (rand() % spawnRegionSize - (spawnRegionSize / 2)), spawnRegionCenter.Z);
    return spawnPos;
}
