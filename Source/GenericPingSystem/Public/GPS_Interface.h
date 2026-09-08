// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "GPS_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGPS_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GENERICPINGSYSTEM_API IGPS_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ping System")
	FGameplayTagContainer GetActorGameplayTags() const;



};
