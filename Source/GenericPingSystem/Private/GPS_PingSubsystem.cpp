//Copyright Levon 2026. All Rights Reserved


#include "GPS_PingSubsystem.h"

#include "GPS_PingComponent.h"
#include "GameFramework/Actor.h"
#include "GPS_Interface.h"

#include "GameplayTagAssetInterface.h"


// boiler plate init and deinit systems, don't evne know if needed but documentation says to do it so I did
void UGPS_PingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

}
void UGPS_PingSubsystem::Deinitialize()
{
	Super::Deinitialize();


}

/*******************************************/
void UGPS_PingSubsystem::RegisterPingComponent(UGPS_PingComponent* InComponent)
{
	if (InComponent && !RegisteredPlayers.Contains(InComponent))
	{
		RegisteredPlayers.Add(InComponent);
	}
}

void UGPS_PingSubsystem::UnregisterPingComponent(UGPS_PingComponent* InComponent)
{
	if (InComponent)
	{
		RegisteredPlayers.Remove(InComponent);
	}
}

void UGPS_PingSubsystem::BroadcastPing(FVector PingLocation, FGameplayTag RequiredTag, UGPS_PingComponent* Instigator, FGameplayTagContainer ContextTags, const TArray<FName>& LegacyTags)
{
	for (UGPS_PingComponent* PingComp : RegisteredPlayers)
	{
		if (!PingComp) continue;

		// optional uncomment if you don't want the sender to see their own ping mark
		//if (PingComp == Instigator) continue;

		// If no required tag is specified, send the ping to all players
		if(!RequiredTag.IsValid())
		{
			
			PingComp->Client_ReceivePing(PingLocation, ContextTags, LegacyTags);
			continue;
		}

		// Check if the owner of this component implements the GameplayTag interface
		if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(PingComp->GetOwner()))
		{


			if (TagInterface->HasMatchingGameplayTag(RequiredTag))
			{
				// The player has the tag, send them the ping directly via Client RPC
				PingComp->Client_ReceivePing(PingLocation, ContextTags, LegacyTags);
				continue;
			}
		}


		//check if implementing the GPS_Interface
		if(PingComp->GetOwner() && PingComp->GetOwner()->Implements<UGPS_Interface>())
		{
			FGameplayTagContainer TempTags = IGPS_Interface::Execute_GetActorGameplayTags(PingComp->GetOwner());

			if(TempTags.HasTag(RequiredTag))
			{
				PingComp->Client_ReceivePing(PingLocation, ContextTags, LegacyTags);
				continue;
			}
		}

		//if no gameplay tag interface, check if the oldschool FName tags are used on the actor directly
		if (PingComp->GetOwner())
		{

			//direct FName tag check
			if (PingComp->GetOwner()->Tags.Contains(RequiredTag.GetTagName()))
			{
	
				PingComp->Client_ReceivePing(PingLocation, ContextTags, LegacyTags);
				continue;
			}

		}



	}
}

void UGPS_PingSubsystem::BroadcastWorldMarker(FVector PingLocation, FGameplayTag RequiredTag, FGameplayTag TrackingTag, UGPS_PingComponent* Instigator, FGameplayTagContainer ContextTags, const TArray<FName>& LegacyTags)
{
	for (UGPS_PingComponent* PingComp : RegisteredPlayers)
	{
		if (!PingComp) continue;

		// optional uncomment if you don't want the sender to see their own ping mark
		//if (PingComp == Instigator) continue;

		if (!RequiredTag.IsValid())
		{
			// If no required tag is specified, send the ping to all players
			PingComp->Client_ReceiveWorldMarker(PingLocation, TrackingTag, ContextTags, LegacyTags);
			continue;
		}

		// Check if the owner of this component implements the GameplayTag interface
		if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(PingComp->GetOwner()))
		{


			if (TagInterface->HasMatchingGameplayTag(RequiredTag))
			{
				// The player has the tag, send them the ping directly via Client RPC
				PingComp->Client_ReceiveWorldMarker(PingLocation, TrackingTag, ContextTags, LegacyTags);
				continue;
			}
		}

		//check if implementing the GPS_Interface
		if (PingComp->GetOwner() && PingComp->GetOwner()->Implements<UGPS_Interface>())
		{
			FGameplayTagContainer TempTags = IGPS_Interface::Execute_GetActorGameplayTags(PingComp->GetOwner());

			if (TempTags.HasTag(RequiredTag))
			{
				PingComp->Client_ReceiveWorldMarker(PingLocation, TrackingTag, ContextTags, LegacyTags);
				continue;
			}
		}

		//if no gameplay tag interface, check if the oldschool FName tags are used on the actor directly
		if (PingComp->GetOwner())
		{

			//direct FName tag check
			if (PingComp->GetOwner()->Tags.Contains(RequiredTag.GetTagName()))
			{

				PingComp->Client_ReceiveWorldMarker(PingLocation, TrackingTag, ContextTags, LegacyTags);
				continue;
			}

		}
	}
}