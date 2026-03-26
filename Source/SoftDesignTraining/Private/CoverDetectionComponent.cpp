// Fill out your copyright notice in the Description page of Project Settings.

#include "CoverDetectionComponent.h"
#include "SoftDesignTraining/SoftDesignTraining.h"
#include "DesignTrainingMovementComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UCoverDetectionComponent::UCoverDetectionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
    bAutoActivate = true;
	PrimaryComponentTick.bCanEverTick = true;

    DetectCoverEveryFrame = false;
}

// Called when the game starts
void UCoverDetectionComponent::BeginPlay()
{
	Super::BeginPlay();

    AActor* ownerActor = GetOwner();
    if (!ensureMsgf(Cast<APawn>(ownerActor), TEXT("%s must update a component owned by a Pawn"), *GetName()))
    {
        return;
    }

    m_PawnOwner = CastChecked<APawn>(ownerActor);
	
}


// Called every frame
void UCoverDetectionComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

    if (DetectCoverEveryFrame)
    {
        FVector coverLocation = FVector::ZeroVector;
        bool coverFound = DetectCoverNow(coverLocation);
        DrawDebugPuppeteerCover(coverFound);
    }
}

bool UCoverDetectionComponent::DetectCoverNow(FVector& outCoverLocation, bool showDebugDisplay)
{
    FVector detectionLoc = m_PawnOwner->GetActorLocation();
    FRotator frontAngle = m_PawnOwner->GetActorRotation();
    FRotator backAngle = m_PawnOwner->GetActorRotation() + FRotator(0.0f, 180.0f, 0.0f);
    FRotator rightAngle = m_PawnOwner->GetActorRotation() + FRotator(0.0f, -90.0f, 0.0f);
    FRotator leftAngle = m_PawnOwner->GetActorRotation() + FRotator(0.0f, 90.0f, 0.0f);

    bool coverFound = false;
    if (!coverFound) //detect front
    {
        coverFound = DetectCoverInDirection(detectionLoc, frontAngle, outCoverLocation, showDebugDisplay);
    }

    if (!coverFound) //detect back
    {
        coverFound = DetectCoverInDirection(detectionLoc, backAngle, outCoverLocation, showDebugDisplay);
    }
    if (!coverFound) //detect right
    {
        coverFound = DetectCoverInDirection(detectionLoc, rightAngle, outCoverLocation, showDebugDisplay);
    }
    if (!coverFound) //detect left
    {
        coverFound = DetectCoverInDirection(detectionLoc, leftAngle, outCoverLocation, showDebugDisplay);
    }

    return coverFound;
}

bool UCoverDetectionComponent::DetectCoverInDirection(const FVector& detectionStart, const FRotator& detectionRotation, FVector& outCoverLocation, bool showDebugDisplay)
{
    if (m_PawnOwner)
    {
        UWorld* currentWorld = GetWorld();

        FVector coverTestStart = detectionStart;
        FVector coverTestEnd = coverTestStart + 400.0f * detectionRotation.Vector();
        FQuat shapeRot = FQuat::Identity;

        static FName InitialCoverSweepTestName = TEXT("InitialCoverSweepTest");
        currentWorld->DebugDrawTraceTag = showDebugDisplay ? InitialCoverSweepTestName : NAME_None;

        FCollisionShape collShape = FCollisionShape::MakeSphere(m_PawnOwner->GetSimpleCollisionRadius());
        FCollisionQueryParams collQueryParams(InitialCoverSweepTestName, false, m_PawnOwner);
        FCollisionObjectQueryParams collObjQueryParams(ECC_WorldStatic);
        FHitResult hitResult;
        if (currentWorld->SweepSingleByObjectType(hitResult, coverTestStart, coverTestEnd, shapeRot, collObjQueryParams, collShape, collQueryParams))
        {
            UDesignTrainingMovementComponent* charMovement = Cast<UDesignTrainingMovementComponent>(m_PawnOwner->GetMovementComponent());
            if (charMovement->ValidateCover(hitResult))
            {
                //cover detected
                outCoverLocation = hitResult.Location;
                return true;
            }
        }
    }

    return false;
}

void UCoverDetectionComponent::DrawDebugPuppeteerCover(bool availableCover)
{
    const FVector TopOfCapsule = m_PawnOwner->GetActorLocation() + FVector(0.f, 0.f, m_PawnOwner->GetSimpleCollisionHalfHeight());

    // Velocity
    {
        const FVector DebugLocation = TopOfCapsule;

        if (availableCover)
        {
            FColor DebugColor = FColor::Green;
            FString DebugText = FString::Printf(TEXT("Take Cover"));
            DrawDebugString(GetWorld(), DebugLocation + FVector(0.f, 0.f, 5.f), DebugText, nullptr, DebugColor, 0.f, true);
        }
        else
        {
            FColor DebugColor = FColor::Red;
            FString DebugText = FString::Printf(TEXT("No Cover"));
            DrawDebugString(GetWorld(), DebugLocation + FVector(0.f, 0.f, 5.f), DebugText, nullptr, DebugColor, 0.f, true);
        }
    }
}