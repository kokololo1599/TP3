// Fill out your copyright notice in the Description page of Project Settings.

#include "CoverPropertiesComponent.h"
#include "SoftDesignTraining/SoftDesignTraining.h"


// Sets default values for this component's properties
UCoverPropertiesComponent::UCoverPropertiesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

    CoverEnabled = true;
    CoverCostMS = 0.2f;
}


// Called when the game starts
void UCoverPropertiesComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UCoverPropertiesComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
}

