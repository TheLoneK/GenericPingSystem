//Copyright Levon 2026. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "GPS_PingComponent.generated.h"

class AGPS_PingIndicator;
class AGPS_PingWorldMarker;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GENERICPINGSYSTEM_API UGPS_PingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UGPS_PingComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AddNameToLegacyTags(FName NameTag) { LegacyTags.Add(NameTag); }

	void ClearLegacyTags() { LegacyTags.Empty(); }

	UFUNCTION()
	void OnRep_LegacyTags();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	
	UFUNCTION(BlueprintCallable, Category = "Ping System", meta = (AutoCreateRefTerm = "InLegacyTags"))
	void TryPing(bool bUseMouseLocation, FGameplayTag RequiredTagToSeePing, FGameplayTagContainer ContextTags, const TArray<FName>& InLegacyTags);
	//be sure to fill in tracking tag, it required for it to spawn so that we can track which markers are spawned. Can spawn the same visual marker if assigned a different tracking tag.
	UFUNCTION(BlueprintCallable, Category = "Ping System", meta = (AutoCreateRefTerm = "InLegacyTags"))
	void TryWorldMarker(bool bUseMouseLocation, FGameplayTag RequiredTagToSeePing,FGameplayTag TrackingTag, FGameplayTagContainer ContextTags, const TArray<FName>& InLegacyTags);

	UFUNCTION(BlueprintCallable, Category = "Ping System")
	void ClearAllWorldMarkers();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestPing(FVector PingLocation, FGameplayTag RequiredTag, FGameplayTagContainer ContextTags, const TArray<FName>& InLegacyTags);

	UFUNCTION(Client, Reliable)
	void Client_ReceivePing(FVector PingLocation, FGameplayTagContainer ContextTags, const TArray<FName>& InLegacyTags);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestWorldMarker(FVector PingLocation, FGameplayTag RequiredTag, FGameplayTag TrackingTag, FGameplayTagContainer ContextTags, const TArray<FName>& InLegacyTags);

	UFUNCTION(Client, Reliable)
	void Client_ReceiveWorldMarker(FVector PingLocation, FGameplayTag TrackingTag, FGameplayTagContainer ContextTags, const TArray<FName>& InLegacyTags);

	UPROPERTY(EditDefaultsOnly, Category = "Ping System")
	TSubclassOf<AGPS_PingIndicator> PingVisualClass;

	UPROPERTY(EditDefaultsOnly, Category = "Ping System")
	TSubclassOf<AGPS_PingWorldMarker> WorldMarkerVisualClass;
	UPROPERTY(EditDefaultsOnly, Category = "Ping System")
	float PingDistanceMax = 15000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Ping System")
	TEnumAsByte<ECollisionChannel> PingTraceCollisionChannel = ECollisionChannel::ECC_Visibility;

	UPROPERTY(EditDefaultsOnly, Category = "Ping System")
	int32 MaxPingsPerWindow = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ping System")
	float PingWindowSeconds = 3.0f;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Ping System")
	TArray<FName> LegacyTags;

private:

	UPROPERTY()
	int32 CurrentPings;

	UPROPERTY()
	TArray<float> RecentPingTimestamps;

	bool CanPing();

	UPROPERTY()
	TMap<FGameplayTag, AGPS_PingWorldMarker*> WorldMarkersContainer;



};

	
