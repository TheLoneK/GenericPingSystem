//Copyright Levon 2026. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GPS_PingWorldMarker.generated.h"

class UNiagaraComponent;

UCLASS(Abstract)
class GENERICPINGSYSTEM_API AGPS_PingWorldMarker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGPS_PingWorldMarker();

	virtual void SetWorldMarkerContextTags(const FGameplayTagContainer& InContextTags);

	void GetWorldMarkerContextTags(FGameplayTagContainer& OutContextTags) const;

	void SetWorldMarkerLegacyTags(const TArray<FName>& InLegacyTags);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	//Context pings passed through, contains tags from the actor pinged and passed through context
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ping System")
	FGameplayTagContainer WorldPingContextTags;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ping System")
	UNiagaraComponent* NiagaraPingComponent;

	//here we go with FName tags again, why I made 2 actors who knows, I just hate monolithic classes i guess
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ping System")
	TArray<FName> LegacyTags;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
