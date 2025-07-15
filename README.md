### How to setup the plugin in your project (try and document when adding to new project)
### How to add new key binding
### How to add startup movies
For startup movies for your game, you must create a Movies folder under your project's Content folder.
Then in the project settings, under Project->Movies, you can add startup movies to the array and they 
will play in the order you added them.

### How to integrate new widget in the plugin

1. Use the C++ base class MKUI_W_ActivatableBase to inherit from when creating a new widget that you want to be
   activatable and pushed onto the screen
2. Add to GameplayTags.h the declaration and definition of a tag that will represent this widget
3. Create a BP subclass of your newly created widget to actually modify its visual representation
4. In the project settings under the UI Settings (or whatever you called your settings tab of this plugin in the project
   settings), map the newly created BP class to the tag you created for this widget
5. Now, from wherever you want, you can push this widget onto the screen using the MKUI_Subsystem method of
   PushSoftWidgetToStackAsync, and you can choose with this method to with stack to push the widget.