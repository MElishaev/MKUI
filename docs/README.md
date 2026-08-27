# MK_UI

MK_UI provides a Common UI primary layout, tagged widget stacks, async widget loading, and shared activatable-widget base classes. Game-specific plugins may supply their own widget classes and assets while using MK_UI to place them on screen.

## Repository and Host-Project Model

MK_UI is developed as its own repository, but it must be loaded by an Unreal project to build and run. Keep a small C++ host project as the clean integration and smoke-test environment. A game project and the host project should pin a known MK_UI commit instead of silently sharing an unversioned copy.

The recommended layout inside a consuming project is:

```text
ProjectRoot/
|-- ProjectName.uproject
|-- Config/
|-- Content/
|-- Source/
`-- Plugins/
    `-- MK_UI/
        |-- MK_UI.uplugin
        |-- Content/
        `-- Source/
```

A Git submodule at `Plugins/MK_UI` is the preferred internal-project integration. The consuming repository records the tested plugin commit while plugin development remains in the MK_UI repository.

## Installing MK_UI in a New Project

1. Use a C++ project built with the same Unreal Engine version supported by the plugin.
2. Clone or add the MK_UI repository at `ProjectRoot/Plugins/MK_UI`. Do not copy `Binaries/` or `Intermediate/` from another project.
3. Enable **MK_UI** in the consuming project's `.uproject` file. This makes the
   integration explicit and reproducible instead of relying on a local Editor
   preference. The plugin descriptor enables its engine-plugin dependencies:

   ```json
   "Plugins": [
       {
           "Name": "MK_UI",
           "Enabled": true
       }
   ]
   ```

   If the project is already open, enabling the plugin through **Edit > Plugins**
   and accepting the restart prompt produces the equivalent project-descriptor
   change.
4. Restart the editor when requested, regenerate project files if necessary, and perform a clean Development Editor build.
5. Enable **Show Plugin Content** in the Content Browser when assigning MK_UI assets in Project Settings.
6. Commit the consuming project's MKUI-related `Config` changes and, when using a submodule, the selected MK_UI commit.

When updating MK_UI, first test the new commit in the host project. Then advance the consuming game's pinned commit and repeat the game-specific build, Blueprint-load, PIE, and packaging checks.

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

Use the Unreal Editor paths below for normal project setup. The INI examples
show where Unreal serializes those selections so the resulting configuration
can be reviewed and version controlled; editing the INI directly is an
alternative for automation or projects that cannot be opened in the Editor.

1. Enable the Common UI, Common Input, and Enhanced Input plugins.
2. Under **Engine > General Settings > Default Classes**, set **Game Viewport
   Client Class** to `CommonGameViewportClient`.
3. In the same section, set **Game User Settings Class** to
   `MKUI_GameUserSettings` when using MK_UI's options system.
4. Open **Project Settings > Engine > Enhanced Input**, expand **User
   Settings**, and enable **Enable User Settings**.
5. Open **Project Settings > Game > Common Input Settings**, assign
   `InputData_Default` to **Input Data**, and enable **Enhanced Input Support**.
   Restart the Editor if prompted.
6. On the same page, expand **Platform Input > Windows**:
   - Add `ControllerData_MouseAndKeyboard` and `ControllerData_Gamepad` to
     **Controller Data**.
   - Set **Default Gamepad Name** to `Generic`.
7. Under **UI Settings > Layout**, assign the primary layout Widget Blueprint.
   `UMKUI_LocalPlayerSubsystem` creates one root for each local player when that
   player receives a Player Controller.
8. Under **UI Settings > Input**, assign the generic input mapping context, then
   map widget tags to their soft Widget Blueprint classes under **Widget Reference**.
9. If using the volume options, assign the consuming project's Master, Music, and SFX Sound Classes and its default Sound Mix under **UI Settings > Audio**.
10. Under **MKUI Loading Screen Settings**, enable the loading-screen system and
    assign its Widget Blueprint if the loading system is used.
11. Ensure every Common UI stack in the primary layout calls `registerWidgetStack` with its corresponding stack tag.

The two default-class selections above serialize to the consuming project's
`Config/DefaultEngine.ini` and should be version controlled:

```ini
[/Script/Engine.Engine]
GameViewportClientClassName=/Script/CommonUI.CommonGameViewportClient
GameUserSettingsClassName=/Script/MK_UI.MKUI_GameUserSettings
```

To configure the included layout in the Editor, open **Project Settings > UI
Settings > Layout** and set **Primary Layout Class** to
`WBP_CUW_PrimaryLayout`. Unreal serializes that selection to the consuming
project's `Config/DefaultGame.ini`, where it should be version controlled as:

```ini
[/Script/MK_UI.MKUI_DeveloperSettings]
mPrimaryLayoutClass=/MK_UI/UI/Widgets/WBP_CUW_PrimaryLayout.WBP_CUW_PrimaryLayout_C
```

For MK_UI's generic actions, open **Project Settings > Game > UI Settings >
Input** and set **Input Mapping Context** to `IMC_UI_GenericActions`. Together
with steps 4-6, the Editor serializes the reusable input setup as follows:

```ini
; Config/DefaultInput.ini
[/Script/EnhancedInput.EnhancedInputDeveloperSettings]
bEnableUserSettings=True

; Config/DefaultGame.ini
[/Script/MK_UI.MKUI_DeveloperSettings]
mInputMappingContext=/MK_UI/UI/CommonInputData/IMC_UI_GenericActions.IMC_UI_GenericActions

[/Script/CommonInput.CommonInputSettings]
InputData=/MK_UI/UI/CommonInputData/InputData_Default.InputData_Default_C
bEnableEnhancedInputSupport=True

[CommonInputPlatformSettings_Windows CommonInputPlatformSettings]
DefaultGamepadName=Generic
+ControllerData=/MK_UI/UI/CommonInputData/ControllerData_MouseAndKeyboard.ControllerData_MouseAndKeyboard_C
+ControllerData=/MK_UI/UI/CommonInputData/ControllerData_Gamepad.ControllerData_Gamepad_C
```

To configure the supplied screens in the Editor, open **Project Settings >
Game > UI Settings > Widget Reference**, add a **Widget Map** entry for each
row below, and select the tag and Widget Blueprint in that row:

| Widget tag | Widget Blueprint |
|---|---|
| `MKUI.widget.pressAnyKeyScreen` | `WBP_CAW_PressAnyKey` |
| `MKUI.widget.mainMenuScreen` | `WBP_CAW_MainMenu` |
| `MKUI.widget.confirmScreen` | `WBP_CAW_ConfirmScreen` |
| `MKUI.widget.storyScreen` | `WBP_CAW_StoryScreen` |
| `MKUI.widget.optionsScreen` | `WBP_CAW_OptionsScreen` |
| `MKUI.widget.keyRemapScreen` | `WBP_CAW_KeyRemapScreen` |
| `MKUI.widget.creditsScreen` | `WBP_CAW_CreditsScreen` |

Unreal stores those selections in the existing
`[/Script/MK_UI.MKUI_DeveloperSettings]` section of
`Config/DefaultGame.ini` as `mWidgetMap`. Only copy mappings whose tags and
widgets are owned by MK_UI. A consuming game's HUD, interaction UI, and other
game-specific mappings remain project-owned and must be configured by that
project.

Project gameplay tags and project settings are not automatically portable just because they refer to plugin content. Prefer native gameplay tags for reusable plugin-owned tags. If a tag is declared in project configuration, copy that configuration when integrating the plugin elsewhere.

The authoritative consumer configuration belongs in the project's `Config/DefaultEngine.ini`, `Config/DefaultGame.ini`, and `Config/DefaultInput.ini`. Plugin configuration does not replace this project configuration. Copy only reusable MKUI settings; do not copy widget mappings, maps, gameplay tags, or asset paths owned by another game.

### Primary Layout Ownership

MK_UI coordinates UI across three different runtime lifetimes:

| UI phase | Runtime owner | Lifetime |
|---|---|---|
| Startup movies | Unreal's startup movie system | Before gameplay and before a local player or Player Controller exists |
| Loading screens | `UMKUI_LoadingScreenSubsystem` | GameInstance lifetime, including map loads where the gameplay UI may be unavailable |
| Main menu, HUD, menus, modals, and other interactive UI | `UMKUI_LocalPlayerSubsystem` and its primary layout | Local-player lifetime, after that player receives a Player Controller |

In a normal single-player game, the local player exists for almost the whole
game session and survives map travel. Its primary layout is created when a
local Player Controller becomes available. If Unreal replaces that controller,
the subsystem removes the old controller-owned layout and creates a new one for
the same local player.

The consuming project only assigns the primary layout class under **UI Settings
> Layout**. It does not manually create the local-player subsystem. Unreal
creates one `UMKUI_LocalPlayerSubsystem` for every local player, and MK_UI's
async widget nodes route stack operations through the subsystem belonging to
their owning Player Controller or widget.

The consuming project owns its GameMode and Player Controller. GameMode must not
create UI because it is server-authoritative and does not exist on remote
clients. A Player Controller may request screens, but it does not need to derive
from an MK_UI controller or create the reusable root layout.

`UMKUI_Subsystem` is no longer the preferred owner for the primary layout or
widget stacks. New integrations should use the local-player path above. The
separate `UMKUI_LoadingScreenSubsystem` remains a GameInstance subsystem because
loading screens are global and must work during travel independently of a local
player's interactive layout.

For migration, projects that leave **UI Settings > Layout > Primary Layout
Class** unset may continue creating a layout in their existing Player Controller
Blueprint and registering it with `UMKUI_Subsystem`. That compatibility facade
forwards to the primary local player. Do not configure automatic layout creation
while also creating the root in a Player Controller; MK_UI will reject and
remove the duplicate legacy layout.

MK_UI does not supply a GameMode or Player Controller. The consuming project
must use its own framework classes and decide when to request its first screen.
Do not add game-specific camera selection, pawn behavior, hardware benchmarking,
or map startup logic to the reusable plugin.

Keep the `UMKUI_Subsystem` compatibility facade only while an existing consumer
still registers a controller-created primary layout. After every consumer uses
the configured local-player-owned layout, remove the facade and this migration
guidance together.

## Audio Options Integration

MK_UI owns the volume-option behavior and stores configurable soft references to the audio routing assets. The consuming project owns the actual audio content and routing hierarchy.

For a minimal setup, the consuming project should provide:

1. A Master Sound Class.
2. Music and SFX Sound Classes routed beneath the Master class.
3. A Sound Mix used by `MKUI_GameUserSettings` to apply class overrides.
4. Music and SFX test sounds routed to their respective classes.
5. The four corresponding references under **UI Settings > Audio**.

The MKUI host project should keep a small looping music sample and a clearly distinguishable SFX sample. This verifies that the Master, Music, and SFX sliders affect the intended channels independently. These samples are test-project content, not runtime MK_UI dependencies.

Any game may replace the host project's Sound Classes, Sound Mix, and sounds without modifying MK_UI code.

## Integration Verification

Before accepting an MK_UI version in a consuming project:

1. Build after removing stale plugin `Binaries/` and `Intermediate/` artifacts.
2. Open the editor and check the log for missing modules, classes, gameplay tags, and asset packages.
3. Use Reference Viewer on plugin maps and primary widgets to confirm they do not depend on content from an unrelated game plugin.
4. Verify that the primary layout registers all expected stacks.
5. Push and dismiss an activatable widget on every configured stack.
6. Switch between keyboard/mouse and gamepad input.
7. Change, apply, cancel, save, and reload options, including the three volume channels.
8. Exercise key remapping when the consuming project enables it.
9. Travel between maps and verify the loading screen when that subsystem is enabled.
10. Cook and launch a Development package; a successful Editor session alone is not sufficient integration coverage.

## Plugin Development Testing

This section is for projects that actively develop or validate MK_UI. It is not
required when integrating MK_UI as a consumer.

To show the plugin's temporary Options test entry, open **Project Settings >
Game > UI Settings** in the Editor:

1. Under **Development**, enable **Enable Development Test Options**.
2. Under **Options**, add `MKUI.image.testImage` to **Options Screen Soft Image
   Map** and select any texture suitable for validating the description-image
   presentation.

The entry is excluded from Shipping builds even when the setting remains
enabled. A missing optional image produces a warning and leaves the image empty;
it does not prevent the Options screen from opening. MKUIHost enables this test
entry because it is the clean plugin-development host.

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

`UMKUI_LoadingScreenSubsystem` automatically displays its configured widget
during map loading, while the new world is starting, and for the configured
minimum hold time. It is a GameInstance subsystem so it remains available
across map travel.

For normal setup, open **Project Settings > Game > MKUI Loading Screen
Settings** in the Editor:

1. Enable **Enable Loading Screen**.
2. Set the loading-screen widget class to `WBP_LoadingScreen`.
3. Set the post-load hold time. Use a short value such as `0.1` for the
   integration host; tune this for the consuming game's streaming behavior.
4. Set the loading-task warning timeout. The timeout reports a stuck task but
   does not force the screen to close over an unready game.
5. Enable **Show Loading Screen In Editor** while testing in PIE. Packaged games
   do not require this option.

For MKUIHost, those Editor selections serialize to `Config/DefaultGame.ini` as:

```ini
[/Script/MK_UI.MKUI_LoadingScreenSettings]
mbEnableLoadingScreen=True
mSoftLoadingScreenWidgetClass=/MK_UI/UI/Widgets/WBP_LoadingScreen.WBP_LoadingScreen_C
mSecsToHoldLoadingScreenAfterLoad=0.100000
mLoadingTaskWarningTimeout=30.000000
mbShowLoadingScreenInEditor=True
```

Game or plugin systems that perform additional asynchronous initialization use
a loading task instead of a map-name Data Table. In Blueprint, get
`MKUI_LoadingScreenSubsystem` from the Game Instance, call **Begin Loading
Task**, store the returned `MKUI Loading Task`, and call **Complete** on that
same task when the operation finishes.

The equivalent C++ flow is:

```cpp
UMKUI_LoadingTask* loadingTask = loadingScreenSubsystem->beginLoadingTask(
    FText::FromString(TEXT("Restoring saved game")));

// Complete this from the asynchronous operation's game-thread callback.
loadingTask->complete();
```

Each active task keeps the screen visible and supplies a diagnostic reason.
The subsystem retains active task objects, but callers must retain their task
reference until they can complete it. The screen closes only after map loading,
world startup, every active task, and the minimum hold time have finished.

## Startup Movies

Create a `Content/Movies` directory in the consuming project and add the startup movies under **Project Settings > Movies**. Enable the Movie Streamer plugin when required by the project.

## Adding a Key Binding

MK_UI displays and edits player-mappable bindings owned by the consuming
project. The plugin does not define the consuming game's gameplay actions or
decide when their mapping contexts are active.

For normal setup in the Unreal Editor:

1. Under **Project Settings > Engine > Enhanced Input**, enable user settings.
2. Create the consuming project's Input Actions and Input Mapping Contexts.
3. For each rebindable Input Action, expand **User Settings**, create its inline
   **Player Mappable Key Settings**, and assign a unique mapping name, display
   name, and display category. One settings object per action is sufficient;
   keyboard and gamepad mappings can inherit it.
4. In each Input Mapping Context entry, leave **Setting Behavior** as **Inherit
   Settings from Action** unless that particular mapping needs different
   metadata.
5. When activating a gameplay mapping context, add it through the Enhanced Input
   Local Player Subsystem with **Notify User Settings** enabled. Contexts that
   are registered only for configuration may instead be passed directly to the
   Enhanced Input User Settings **Register Input Mapping Context** function.

The Controls screen lists mappings from registered contexts and filters them for
the player's current input type. Therefore an empty Controls tab normally means
that no context containing player-mappable entries was registered for that local
player.

In C++, an active context can be registered while it is added:

```cpp
FModifyContextOptions options;
options.bNotifyUserSettings = true;
inputSubsystem->AddMappingContext(gameplayMappingContext, priority, options);
```
