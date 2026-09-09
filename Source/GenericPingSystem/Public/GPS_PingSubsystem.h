//Copyright Levon 2026. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "GPS_PingSubsystem.generated.h"

/**
 * 
 */

class UGPS_PingComponent;



UCLASS()
class GENERICPINGSYSTEM_API UGPS_PingSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:
	// USubsystem inits
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;


	// Register/Unregister called by the components themselves
	void RegisterPingComponent(UGPS_PingComponent* InComponent);
	void UnregisterPingComponent(UGPS_PingComponent* InComponent);

	// Called by the server to route the ping to players with the required tag
	void BroadcastPing(FVector PingLocation, FGameplayTag RequiredTag, UGPS_PingComponent* Instigator, FGameplayTagContainer ContextTags, const TArray<FName>& LegacyTags);

	// Called by the server to route the worldmarker to players with the required tag
	void BroadcastWorldMarker(FVector PingLocation, FGameplayTag RequiredTag,FGameplayTag TrackingTag, UGPS_PingComponent* Instigator, FGameplayTagContainer ContextTags, const TArray<FName>& LegacyTags);


private:
	UPROPERTY()
	TArray<UGPS_PingComponent*> RegisteredPlayers;
	
};
