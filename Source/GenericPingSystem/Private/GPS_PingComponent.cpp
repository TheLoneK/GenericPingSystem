//Copyright Levon 2026. All Rights Reserved


#include "GPS_PingComponent.h"
#include "GPS_PingIndicator.h"
#include "GPS_PingSubsystem.h"
#include "GPS_PingWorldMarker.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "GPS_Interface.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "GameplayTagAssetInterface.h"

#include "Kismet/KismetSystemLibrary.h"


UGPS_PingComponent::UGPS_PingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UGPS_PingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGPS_PingComponent, LegacyTags);
}


void UGPS_PingComponent::OnRep_LegacyTags()
{
	LegacyTags = GetOwner()->Tags;
}

void UGPS_PingComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register with the Subsystem upon spawning
	if (UWorld* World = GetWorld())
	{
		if (UGPS_PingSubsystem* PingSub = World->GetSubsystem<UGPS_PingSubsystem>())
		{
			PingSub->RegisterPingComponent(this);
		}
	}
}

void UGPS_PingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Unregister when despawned/destroyed
	if (UWorld* World = GetWorld())
	{
		if (UGPS_PingSubsystem* PingSub = World->GetSubsystem<UGPS_PingSubsystem>())
		{
			PingSub->UnregisterPingComponent(this);
		}
	}
	Super::EndPlay(EndPlayReason);
}

void UGPS_PingComponent::TryPing(bool bUseMouseLocation, FGameplayTag RequiredTagToSeePing, FGameplayTagContainer ContextTags, const TArray<FName>& InLegacyTags)
{


	//validity checks
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;

	APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController());
	if (!PC || !PC->IsLocalController()) return;
	//end valid checks

	ClearLegacyTags();


	FVector WorldLocation;
	FVector WorldDirection;

	FGameplayTagContainer PingContextTags;

	// deproject mouse or from center of screen to world
	if (bUseMouseLocation)
	{
		float MouseX, MouseY;
		if (PC->GetMousePosition(MouseX, MouseY))
		{
			PC->DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection);
		}
	}
	else
	{
		int32 ViewportX, ViewportY;
		PC->GetViewportSize(ViewportX, ViewportY);
		PC->DeprojectScreenPositionToWorld(ViewportX / 2.f, ViewportY / 2.f, WorldLocation, WorldDirection);
	}

	// Perform a linetrace into the world
	FHitResult HitResult;
	FVector TraceEnd = WorldLocation + (WorldDirection * PingDistanceMax);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, TraceEnd, PingTraceCollisionChannel))
	{

		//DrawDebugLine(GetWorld(), WorldLocation, HitResult.ImpactPoint, FColor::Green, false, 2.f, 0, 1.f);

		if (CanPing() == false)
		{
			return;
		}

		AActor* HitActor;
		if (HitResult.GetActor())
		{
			HitActor = HitResult.GetActor();
			if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(HitActor))
			{
				TagInterface->GetOwnedGameplayTags(PingContextTags);
			}

			if (HitActor && HitActor->Implements<UGPS_Interface>())
			{
				FGameplayTagContainer TempTags = IGPS_Interface::Execute_GetActorGameplayTags(HitActor);

				PingContextTags.AppendTags(TempTags);
			}

			LegacyTags.Append(HitActor->Tags);
		}
		LegacyTags.Append(InLegacyTags);
		PingContextTags.AppendTags(ContextTags);
		Server_RequestPing(HitResult.ImpactPoint, RequiredTagToSeePing, PingContextTags, LegacyTags);
		CurrentPings++;


		
	}
	//PingContextTags.AppendTags(ContextTags);
	//Server_RequestPing(HitResult.ImpactPoint, RequiredTagToSeePing, PingContextTags);
	//CurrentPings++;
	
}

void UGPS_PingComponent::TryWorldMarker(bool bUseMouseLocation, FGameplayTag RequiredTagToSeePing, FGameplayTag TrackingTag, FGameplayTagContainer ContextTags, const TArray<FName>& InLegacyTags)
{

	
	if(CanPing() == false)
	{
		return;
	}

	ClearLegacyTags();

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;

	APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController());
	if (!PC || !PC->IsLocalController()) return;

	FVector WorldLocation;
	FVector WorldDirection;

	FGameplayTagContainer PingContextTags;

	// deproject mouse or from center of screen to world
	if (bUseMouseLocation)
	{
		float MouseX, MouseY;
		if (PC->GetMousePosition(MouseX, MouseY))
		{
			PC->DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection);
		}
	}
	else
	{
		int32 ViewportX, ViewportY;
		PC->GetViewportSize(ViewportX, ViewportY);
		PC->DeprojectScreenPositionToWorld(ViewportX / 2.f, ViewportY / 2.f, WorldLocation, WorldDirection);
	}

	FHitResult HitResult;
	FVector TraceEnd = WorldLocation + (WorldDirection * PingDistanceMax);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, TraceEnd, PingTraceCollisionChannel))
	{
		AActor* HitActor;
		if (HitResult.GetActor())
		{
			HitActor = HitResult.GetActor();
			if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(HitActor))
			{
				TagInterface->GetOwnedGameplayTags(PingContextTags);
			}

			if (HitActor && HitActor->Implements<UGPS_Interface>())
			{
				FGameplayTagContainer TempTags = IGPS_Interface::Execute_GetActorGameplayTags(HitActor);

				PingContextTags.AppendTags(TempTags);
			}

		}

		PingContextTags.AppendTags(ContextTags);
		Server_RequestWorldMarker(HitResult.ImpactPoint, RequiredTagToSeePing, TrackingTag, PingContextTags, InLegacyTags);
	}
}

void UGPS_PingComponent::ClearAllWorldMarkers()
{
	for (const auto& Pair : WorldMarkersContainer)
	{
		if (AGPS_PingWorldMarker* Marker = Pair.Value)
		{
			if (IsValid(Marker))
			{
				Marker->Destroy();
			}
		}
	}
	WorldMarkersContainer.Empty();
}

bool UGPS_PingComponent::CanPing()
{
	if(UWorld * World = GetWorld())
	{
		float CurrentTime = World->GetTimeSeconds();

		//remove all timestamps that are older than our allowed window
		RecentPingTimestamps.RemoveAll([CurrentTime, this](float Timestamp){return (CurrentTime - Timestamp) > PingWindowSeconds;});

		//check if we have room for another ping
		if (RecentPingTimestamps.Num() >= MaxPingsPerWindow)
		{
			return false; // Rate limit exceeded
		}

		//register new ping attempt timestamp
		RecentPingTimestamps.Add(CurrentTime);
		return true;
	}
	return false;
}

bool UGPS_PingComponent::Server_RequestPing_Validate(FVector PingLocation, FGameplayTag RequiredTag, FGameplayTagContainer ContextTags, const TArray<FName>& InLegacyTags)
{
	return true; // add anti-cheat validation here if necessary
}

void UGPS_PingComponent::Server_RequestPing_Implementation(FVector PingLocation, FGameplayTag RequiredTag, FGameplayTagContainer ContextTags, const TArray<FName>& InLegacyTags)
{
	//server tells the Subsystem to handle the routing
	if (UWorld* World = GetWorld())
	{
		if (UGPS_PingSubsystem* PingSub = World->GetSubsystem<UGPS_PingSubsystem>())
		{
			PingSub->BroadcastPing(PingLocation, RequiredTag, this, ContextTags, InLegacyTags);
		}
	}
}



void UGPS_PingComponent::Client_ReceivePing_Implementation(FVector PingLocation, FGameplayTagContainer ContextTags, const TArray<FName>& InLegacyTags)
{
	
	//this is a checked client rpc so only the client receiving actually spawns anything
	if (PingVisualClass)
	{

		FTransform SpawnTransform(FRotator::ZeroRotator, PingLocation);

		AGPS_PingIndicator* PingIndicator = GetWorld()->SpawnActorDeferred<AGPS_PingIndicator>(PingVisualClass, SpawnTransform, GetOwner(), nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		if(PingIndicator)
		{
			PingIndicator->SetPingContextTags(ContextTags);
			PingIndicator->SetLegacyTags(InLegacyTags);
			UGameplayStatics::FinishSpawningActor(PingIndicator, SpawnTransform);
		}

	}
	/*
	else
	{
		// Fallback debug visual if no class is assigned,  might remove later who knows
		DrawDebugSphere(GetWorld(), PingLocation, 50.f, 16, FColor::Cyan, false, 3.f);
	}
	*/
}

bool UGPS_PingComponent::Server_RequestWorldMarker_Validate(FVector PingLocation, FGameplayTag RequiredTag, FGameplayTag TrackingTag, FGameplayTagContainer ContextTags, const TArray<FName>& InLegacyTags)
{
	return true; //anti-cheat validation here if necessary
}

void UGPS_PingComponent::Server_RequestWorldMarker_Implementation(FVector PingLocation, FGameplayTag RequiredTag, FGameplayTag TrackingTag, FGameplayTagContainer ContextTags, const TArray<FName>& InLegacyTags)
{
	//subsystem routing
	if (UWorld* World = GetWorld())
	{
		if (UGPS_PingSubsystem* PingSub = World->GetSubsystem<UGPS_PingSubsystem>())
		{
			PingSub->BroadcastWorldMarker(PingLocation, RequiredTag,TrackingTag, this, ContextTags, InLegacyTags);
		}
	}
}


void UGPS_PingComponent::Client_ReceiveWorldMarker_Implementation(FVector PingLocation, FGameplayTag TrackingTag, FGameplayTagContainer ContextTags, const TArray<FName>& InLegacyTags)
{

	if(!TrackingTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client_ReceiveWorldMarker called with an invalid TrackingTag. This will not spawn a world marker."));
		return;
	}
	//Check if we already have a ping with this tracking tag
	if (WorldMarkersContainer.Contains(TrackingTag))
	{
		AGPS_PingWorldMarker* OldMarker = WorldMarkersContainer[TrackingTag];

		//ensure it wasnt already destroyed by something else
		if (IsValid(OldMarker))
		{
			OldMarker->Destroy();
		}

		// Remove tracking tag
		WorldMarkersContainer.Remove(TrackingTag);
		
	}

	TSubclassOf<AActor> ClassToSpawn = WorldMarkerVisualClass ? WorldMarkerVisualClass : PingVisualClass;

	if (ClassToSpawn)
	{
		FTransform SpawnTransform(FRotator::ZeroRotator, PingLocation);

		//Spawn Deferred
		AGPS_PingWorldMarker* SpawnedMarker = GetWorld()->SpawnActorDeferred<AGPS_PingWorldMarker>(ClassToSpawn, SpawnTransform, GetOwner(), nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		if (SpawnedMarker)
		{
			SpawnedMarker->SetWorldMarkerContextTags(ContextTags);
			SpawnedMarker->SetWorldMarkerLegacyTags(InLegacyTags);

			UGameplayStatics::FinishSpawningActor(SpawnedMarker, SpawnTransform);

			//Add the newly spawned actor to the tracking map
			WorldMarkersContainer.Add(TrackingTag, SpawnedMarker);
		}
	}
}