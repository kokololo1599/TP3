// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

enum class EStateType : uint8
{
    Invalid = 0,
    Patrol  = 1, 	
    Chase   = 2,
    Blind   = 3
};
