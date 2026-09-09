//Copyright Levon 2026. All Rights Reserved


#include "GPS_PingWorldMarker.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AGPS_PingWorldMarker::AGPS_PingWorldMarker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;


	// Initialize the Niagara Component and make it the Root Component
	NiagaraPingComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraPingComponent"));
	RootComponent = NiagaraPingComponent;

}

// Called when the game starts or when spawned
void AGPS_PingWorldMarker::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGPS_PingWorldMarker::SetWorldMarkerContextTags(const FGameplayTagContainer& InContextTags)
{
	WorldPingContextTags = InContextTags;
}

void AGPS_PingWorldMarker::GetWorldMarkerContextTags(FGameplayTagContainer& OutContextTags) const
{
	OutContextTags = WorldPingContextTags;
}

void AGPS_PingWorldMarker::SetWorldMarkerLegacyTags(const TArray<FName>& InLegacyTags)
{
	LegacyTags = InLegacyTags;
}

// Called every frame
void AGPS_PingWorldMarker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

