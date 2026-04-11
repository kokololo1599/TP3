#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SDTTypes.generated.h"

UENUM(BlueprintType)
enum class EPlayerInteractionBehavior : uint8
{
    Collect UMETA(DisplayName = "Collect"),
    Chase   UMETA(DisplayName = "Chase"),
    Flee    UMETA(DisplayName = "Flee")
};

// This dummy struct forces UHT to parse this file properly
USTRUCT(BlueprintType)
struct FSDTTypesDummy
{
    GENERATED_BODY()
};