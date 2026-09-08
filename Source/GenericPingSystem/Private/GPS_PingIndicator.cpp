// Fill out your copyright notice in the Description page of Project Settings.


#include "GPS_PingIndicator.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AGPS_PingIndicator::AGPS_PingIndicator()
{

	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;

	// Initialize the Niagara Component and make it the Root Component
	NiagaraPingComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraPingComponent"));
	RootComponent = NiagaraPingComponent;


	SetLifeSpan(3.0f);

}

void AGPS_PingIndicator::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);


}


void AGPS_PingIndicator::SetPingContextTags(const FGameplayTagContainer& InContextTags)
{
	PingContextTags = InContextTags;
}

void AGPS_PingIndicator::SetLegacyTags(const TArray<FName>& InLegacyTags)
{
	LegacyTags = InLegacyTags;
}

// Called when the game starts or when spawned
void AGPS_PingIndicator::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void AGPS_PingIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

