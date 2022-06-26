# Million Framework project

Million Framework is an Unreal Engine project to make machinimas (fanmade renders) of the game The Idolmaster Million Live Theater Days. Using slew of Unreal Engine features, it is capable of posing and animating idols inside the engine, and is customizable by Producers.

## Engine version
This project uses Unreal Engine 4.27, with UE 5.1 branch coming soon once it's deemed stable by Epic Games. UE 5.0 is skipped due to plethora of reported issues and expected major changes to the codebase in future versions.

## Requirements
- Due to custom shading model used in this project, specifically custom anime shading model, a modified Unreal Engine 4.27 is required, of which you can [grab it here](https://github.com/TheHoodieGuy02/UnrealEngine).
  - It is a modified Unreal Engine 4.27 used for the bigger sister original game project.
  - The page included a prebuilt (albeit slightly outdated) version in the released section.
  - If you see 404 page, [read this](https://www.unrealengine.com/en-US/ue-on-github) to get access to Unreal Engine source code.
  - Shall you don't want to use this customized build, you can remove the materials and remade them in vanilla UE4.
- Visual Studio 2019 or newer to build C++ classes.
  - If you're using the prebuilt editor, Visual Studio 2022 and .NET Framework 4.5.2 and 4.6.0 is required.
  - Make sure following components are installed:
    - Game development with C++
    - Desktop development with C++
    - Desktop development with .NET
- Blender 3.1 for the processor project file.

## Changes from old version
- Things are less hardcoded for ease of maintenance and end-user customization. This includes:
  - Minimal changes to idol skeletal meshes
  - Reworked materials (only compatible with modified engine, see Requirements)
  - Reworked idol Actor for easier customization
- Idols now do not need to share the same skeleton. This resolves hierarchy conflicts with hair bones.

## Contribution
You are welcome to contribute to this project, by forking and making pull request of your changes. However, there are few things to keep in mind:

- Don't clutter MLTD_V2 folder with unnecessary assets.
- Assets must adhere to the naming convention
- Make sure you don't include assets and plugins sourced from paid sources.
- Assets in MLTD_Outfits have to be from the game - altered outfits or those sourced from other games are not allowed to be pushed.
- If you want to modify the script in Blender, modify the source in `SourcePython` folder. Don't alter it inside Blender text editor and keeping the changes there.
- .uproject's EngineAssociation GUID should be kept intact.