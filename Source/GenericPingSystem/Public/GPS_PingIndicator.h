//Copyright Levon 2026. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GPS_PingIndicator.generated.h"


class UNiagaraComponent;


UCLASS(Abstract)
class GENERICPINGSYSTEM_API AGPS_PingIndicator : public AActor
{
	GENERATED_BODY()
	
public:	

	AGPS_PingIndicator();

	// Required for replicating variables
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetPingContextTags(const FGameplayTagContainer& InContextTags);

	void SetLegacyTags(const TArray<FName>& InLegacyTags);




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Context pings passed through, contains tags from the actor pinged and passed through context
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ping System")
	FGameplayTagContainer PingContextTags;

	//legacy tags using FName, idk who would use this shit anymore but here it is if you do
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ping System")
	TArray<FName> LegacyTags;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ping System")
	UNiagaraComponent* NiagaraPingComponent;






private:


	
};
