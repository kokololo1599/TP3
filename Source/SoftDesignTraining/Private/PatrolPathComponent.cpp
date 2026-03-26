// Fill out your copyright notice in the Description page of Project Settings.

#include "PatrolPathComponent.h"
#include "SoftDesignTraining/SoftDesignTraining.h"
#include "DrawDebugHelpers.h"
#include "PatrolPathManager.h"

// Sets default values for this component's properties
UPatrolPathComponent::UPatrolPathComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPatrolPathComponent::BeginPlay()
{
	Super::BeginPlay();

	PatrolPathManager* ppm = PatrolPathManager::GetInstance();

	if (ppm)
	{
		ppm->RegisterPath(this);
	}
	
}

void UPatrolPathComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	PatrolPathManager* ppm = PatrolPathManager::GetInstance();

	if (ppm)
	{
		ppm->UnregisterPath(this);
	}
}

// Called every frame
void UPatrolPathComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	static bool showDebugDisplay = true;
	if (!showDebugDisplay)
		return;

	TArray<AActor *>childrens;
	GetOwner()->GetAttachedActors(childrens);

	FVector pathLocation = GetOwner()->GetActorLocation();

	if (childrens.Num() > 0)
	{
		FVector lastPosition = childrens[childrens.Num() - 1]->GetActorLocation();

		for (AActor* children : childrens)
		{
			FVector actorLocation = children->GetActorLocation();

			DrawDebugLine(GetWorld(), actorLocation, lastPosition, FColor::Blue, false);
			DrawDebugLine(GetWorld(), actorLocation, pathLocation, FColor::Cyan, false);

			lastPosition = actorLocation;
		}
	}
}

unsigned long UPatrolPathComponent::GetNWayPoints()
{
	TArray<AActor *>childrens;
	GetOwner()->GetAttachedActors(childrens);

	return childrens.Num();
}

void UPatrolPathComponent::GetNextWayPoint(unsigned long& wayPoint, FVector& nextDestination, unsigned long direction)
{
	TArray<AActor *>childrens;
	GetOwner()->GetAttachedActors(childrens);

	long localWayPoint = (long)wayPoint;

	localWayPoint += direction;
	if (localWayPoint >= childrens.Num())
		localWayPoint = 0;
	if (localWayPoint < 0)
		localWayPoint = childrens.Num() - 1;

	wayPoint = (unsigned long)localWayPoint;

	nextDestination = childrens[wayPoint]->GetActorLocation();
}
