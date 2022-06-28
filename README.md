# Million Framework project
![](https://img.shields.io/badge/UE%20version-4.27-blue?style=for-the-badge&logo=unrealengine) ![](https://img.shields.io/github/issues-raw/TheHoodieGuy02/MillionFramework?style=for-the-badge) ![](https://img.shields.io/github/repo-size/TheHoodieGuy02/MillionFramework?style=for-the-badge)
![](https://raw.githubusercontent.com/TheHoodieGuy02/MillionFramework/master/Content/Splash/EdSplash.png)

Million Framework is an Unreal Engine project to make machinimas (fanmade renders, MAD) of the game The Idolmaster Million Live Theater Days. Using slew of Unreal Engine features, it is capable of posing and animating idols inside the engine, and is customizable by Producers.

While this project uses assets from the game, it is mainly for making machinima, and as such it is not preparred for fangame programming out of the box and not adhering to standards found in other Unreal Engine projects.

## Engine version
This project uses Unreal Engine 4.27, with UE 5.1 branch coming soon once it's fully released. UE 5.0 is skipped due to plethora of reported issues and expected major changes to the codebase in future versions.

## Requirements
- Due to custom shading model used in this project, specifically custom anime shading model, a modified Unreal Engine 4.27 is required, of which you can [grab it here](https://github.com/TheHoodieGuy02/UnrealEngine).
  - It is a modified Unreal Engine 4.27 used for the bigger sister original game project.
  - The page included a [prebuilt (albeit slightly outdated) version in the released section](https://github.com/TheHoodieGuy02/UnrealEngine/releases/tag/machinima-editor-only). Do note that the prebuilt editor is devoid of gameplay code and will not compile to UE4Game. If you want to make fan game, consider building the engine source yourself.
  - If you see 404 page, [read this](https://www.unrealengine.com/en-US/ue-on-github) to get access to Unreal Engine source code.
  - Shall you don't want to use this customized build, you can remove the materials and remade them in vanilla UE4.
- Visual Studio 2019 or newer to build C++ classes.
  - If you're using the prebuilt editor, Visual Studio 2022 and .NET Framework 4.5.2 and 4.6.0 is required.
  - Make sure following components are installed:
    - Game development with C++
    - Desktop development with C++
    - Desktop development with .NET
- Blender 3.1 for the processor project file.

## Issues

For issues and caveats in this project, see the issue tracker page of this repository. Feel free to contribute to fixing them.

## Changes from old version
- Things are less hardcoded for ease of maintenance and end-user customization. This includes:
  - Minimal changes to idol skeletal meshes
  - Reworked materials (only compatible with modified engine, see Requirements)
  - Reworked idol Actor for easier customization
- Idols now do not need to share the same skeleton asset. This resolves hierarchy conflicts with hair bones.

## Contribution
You are welcome to contribute to this project, by forking and making pull request of your changes. However, there are few things to keep in mind:

- Don't clutter MLTD_V2 folder with unnecessary assets.
- Assets must adhere to the naming convention
- Make sure you don't include assets and plugins sourced from paid sources
- Assets in MLTD_Outfits have to be from the game - altered outfits or those sourced from other games are not allowed to be pushed.
- If you want to modify the script in Blender, modify the source in `SourcePython` folder. Don't alter it inside Blender text editor and keeping the changes there.
- .uproject's EngineAssociation GUID should be kept intact.

## Credits
- Models - BANDAI NAMCO Entertainment
- Code - TheHoodieGuy02
- Materials - TheHoodieGuy02, doomfest
