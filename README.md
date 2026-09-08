# GenericPingSystem
Ping system made to be as lightweight as possible while allowing massive customization through blueprint using GameplayTags or the legacy FName tags if needed.

READ FIRST - Source code is free, download from this repo.


Initial setup.
1. Make sure the plugin in enabled
2. Add the GPSPing component to your character.
3. Be sure to set the Visual classes in the GPS_Ping component settings under Ping System. You can also configure a few other options here.
You can use the visual classes included BP_Ping or BP_MousePing for the Ping Visual, and use BP_Worldmarker for the world marker class.

** If you want to fully create your own visuals or modify the ones included, you should clone them into your own project content or create your own with the
appropriate GPS_PingIndicator or GPS_PingWorldMarker as the parents and DO NOT inherit from the BP classes.
At the end of the day do what you want this is just a design paradigm recommendation.

4. Assuming we are using a character, now you should be able to call TryPing or TryWorldMarker from the GPSPing Component.
5. The TryPing will work initially with no settings being changed, it will use default visuals and no audio.
6. **IMPORTANT** TryWorldMarker NEEDS to have a tracking tag set before the subsystem will allow it to spawn. This lets us keep track of which world markers exist for each client.
End of Setup : This is all that's required to start using or testing it before I get into the details and configuration of the system.



