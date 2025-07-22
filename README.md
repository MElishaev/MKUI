### How to setup the plugin in your project (try and document when adding to new project)

There are different steps to setup the project, not necessarily in the following order.  
I'm not sure, but it may be that if the plugin was developed with those settings, it will pass with the project settings
to the new project? I don't think so, because this is plugin, and we package and export the plugin and not the project,
so I assume the project settings doesn't pass over

1. You can bring in some settings manually,
2. In the project settings, searching for "viewport", you will find "Game Viewport Client Class". This needs to be
   changed to "CommonGameViewportClient" if CommonUI is enabled.
3. In the project settings, searching for "game user settings", you will find the game user settings class that you
   should override to use the one we developed in MKUI_GameUserSettings.
4. Project Settings > Engine > Enhanced Input > User Settings > Enable User Settings – Checkbox to enable (in 5.6 there
   is something new there with slot that i don't know yet for what reason)
5. Project Settings > Set common input Data to the InputData_Default asset. Additionally, under the Windows dropdown
   there, add the controller data for gamepad and keyboard+mouse - this will map between the keys to their icons. Also -
   Default Gamepad Name needs to be set to Generic.
6. In the MKUI Loading Screen Settings under the project settings, set the loading screen WBP.
7. Under the UI settings (the plugin's settings) in the project settings, setup the WBPs for the different widget tags
   of your game  ![img](readmeData/img.png)
8. note that the gameplay tags are located in each project in /Config/DefaultGameplayTags.ini - in the project, we added
   1 gameplay tag (credit screen widget) through the project settings - this means it won't pass over when packaging the
   plugin. Need to add this manually.
9. NOTE: depends on how you use this plugin, but if it is copy pasted from the original, it may be needed to add the
   relevant dependencies to build.cs.
10. For now, the UI player controller (cpp) searches for a camera with tag "default" to posses it. If you created a new
    map
    and added a camera for possession, you need to add the tag to the camera for it to be possessed.
11. Remember to set in the BeginPlay of GameMode the custom settings in the options menu that you've added which doesn't
    have a built-in UE method that applies them on your game.

### How to work with loading screen?

The loading screen is automatically displayed when loading levels.  
This is based on PreLoad and PostLoad functions of the loading system or something.

### How to add new key binding

### How to add startup movies
For startup movies for your game, you must create a Movies folder under your project's Content folder.
Then in the project settings, under Project->Movies, you can add startup movies to the array and they
will play in the order you added them.  
Now, for the movies to be able to be played, the "Movie Streamer" plugin should be enabled.

### How to integrate new widget in the plugin

1. Use the C++ base class MKUI_W_ActivatableBase to inherit from when creating a new widget that you want to be
   activatable and pushed onto the screen. By activatable, the meaning is that you want this widget to consume input.
2. Add to GameplayTags.h the declaration and definition of a tag that will represent this widget. (this can also be done
   through the UI settings in the project settings where you can add them manually without touching and recompiling the
   code)
3. Create a BP subclass of your newly created widget to actually modify its visual representation.
4. In the project settings under the UI Settings (or whatever you called your settings tab of this plugin in the project
   settings), map the newly created BP class to the tag you created for this widget.
5. Now, from wherever you want, you can push this widget onto the screen using the MKUI_Subsystem method of
   PushSoftWidgetToStackAsync, and you can choose with this method which stack to push to.