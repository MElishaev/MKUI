# MK_UI

MK_UI provides a Common UI primary layout, tagged widget stacks, async widget loading, and shared activatable-widget base classes. Game-specific plugins may supply their own widget classes and assets while using MK_UI to place them on screen.

## Runtime Structure

The runtime flow is:

1. A `UMKUI_W_PrimaryLayout` instance owns the Common UI widget stacks.
2. Each stack is registered on the primary layout with an `MKUI.widgetStack.*` gameplay tag.
3. `UMKUI_Subsystem` stores the active primary layout.
4. An activatable widget class is loaded asynchronously and pushed onto a selected stack.
5. Deactivating the active widget allows its Common UI stack to remove it and reveal the widget below it.

Widget tags and stack tags have different purposes:

- `MKUI.widget.*` identifies a widget class in UI Settings.
- `MKUI.widgetStack.*` identifies the layer that should display the widget.

For example, a dialogue widget may be registered as `MKUI.widget.dialogue` and pushed to `MKUI.widgetStack.modal`.

## Project Setup

These settings belong to the consuming project and should be checked again after moving MK_UI to another project:

1. Enable the Common UI, Common Input, and Enhanced Input plugins.
2. Set **Game Viewport Client Class** to `CommonGameViewportClient`.
3. Set **Game User Settings Class** to `MKUI_GameUserSettings` when using MK_UI's options system.
4. Under **Enhanced Input > User Settings**, enable user settings.
5. Under **Common Input Settings**, assign `InputData_Default`.
6. Under the Windows Common Input platform settings:
   - Add the keyboard/mouse and gamepad controller-data assets.
   - Set **Default Gamepad Name** to `Generic`.
7. Under **UI Settings**, assign the generic input mapping context and map widget tags to their soft Widget Blueprint classes.
8. Under **MKUI Loading Screen Settings**, assign the loading-screen Widget Blueprint if the loading system is used.
9. Ensure the game's Player Controller creates the primary layout and that the layout registers itself with `UMKUI_Subsystem`.
10. Ensure every Common UI stack in the primary layout calls `registerWidgetStack` with its corresponding stack tag.

Project gameplay tags and project settings are not automatically portable just because they refer to plugin content. Prefer native gameplay tags for reusable plugin-owned tags. If a tag is declared in project configuration, copy that configuration when integrating the plugin elsewhere.

The supplied MK_UI Player Controller currently searches for a camera tagged `default` when possessing a pawn. Maps using that behavior need an appropriately tagged camera.

## Adding an Activatable Widget

### 1. Create the native base when logic is reusable

Derive the C++ class from `UMKUI_W_ActivatableBase`. Keep reusable lifecycle and initialization logic in this class, while leaving layout and styling to a Widget Blueprint subclass.

The module containing a public widget subclass must publicly depend on `MK_UI`. Because the class ultimately derives from UMG and Common UI types, that module should also link `UMG` and `CommonUI` directly.

### 2. Create the Widget Blueprint

Create a Widget Blueprint using the native activatable class as its parent. The existing naming convention is `WBP_CAW_*`, where `CAW` means Common Activatable Widget.

Configure the widget's Common UI input behavior according to its role. A screen that must consume player input should be activatable; a passive piece of HUD generally should not become a separate input-consuming screen.

### 3. Choose or add tags

Choose two independently:

- A widget tag used to find the soft widget class, such as `MKUI.widget.dialogue`.
- A stack tag used to choose its screen layer, such as `MKUI.widgetStack.modal` or `MKUI.widgetStack.gameHUD`.

Add new stack tags only when the primary layout genuinely needs another ordering or input layer. Reuse an existing stack when its behavior already matches.

### 4. Register the widget class

In **Project Settings > UI Settings > Widget Reference**, map the widget tag to the Widget Blueprint class. At runtime, `UMKUI_FunctionLibrary::getSoftWidgetClassByTag` resolves that mapping.

A caller may instead hold a soft widget class directly. The tag mapping is useful when the presentation should be replaceable through project settings.

### 5. Push the widget

In Blueprint, use **Push Soft Widget to Stack**. In C++, call `UMKUI_Subsystem::pushSoftWidgetToStackAsync`.

The async push exposes two stages:

- **On Created Before Push**: the widget exists and can receive its required initialization data. Use this for dialogue instantiations, confirmation-screen data, inventory models, and similar dependencies.
- **After Push**: the widget has been added to the stack. Use this for work that requires the screen to already be active or visible.

Required initialization should happen before the push. Otherwise Common UI may activate and briefly display an empty or stale screen.

## MVVM-backed Activatable Widgets

When a native widget owns the ViewModel instance:

1. Add the ViewModel to the Widget Blueprint's MVVM panel.
2. Set its creation type to **Manual**.
3. Bind visual properties to its Field Notify fields.
4. During **On Created Before Push**, initialize the native widget. The native widget sets the manual ViewModel source before publishing its initial state.

Do not also use **Create Instance** for that ViewModel in the Widget Blueprint. Doing so creates a second instance, so the bindings may observe a different object from the one receiving gameplay data.

## CiF Dialogue Example

The CiF plugin's `UW_CifDialogue` demonstrates an activatable MVVM screen that receives its model during **On Created Before Push**, owns its ViewModel, and deactivates when playback ends. Its header documents the required Widget Blueprint contract and input behavior.

## Loading Screens

The loading-screen subsystem displays its configured widget around level loading. Set the loading-screen class and conditions under **MKUI Loading Screen Settings**.

## Startup Movies

Create a `Content/Movies` directory in the consuming project and add the startup movies under **Project Settings > Movies**. Enable the Movie Streamer plugin when required by the project.

## Adding a Key Binding

Document the generic and project-specific key-binding workflow here when it is next exercised and verified.
