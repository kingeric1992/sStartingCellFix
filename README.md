# sStartingCellFix
A skse64 plugin to fix sStartingCell Skyrim.ini setting CTD for pre 1.6.1130.

# Abstract
Skyrim SE/AE has a Skyrim.ini setting `sStartingCell=` under [`General`] section, which allows loading a starting cell directly upon game launch without user interaction. However the functionality is broken and would cause CTD upon load.

Bethesda eventually fixed the problem in release 1.6.1130, and this plugin is to fix the same bug in prior versions.

# Installation
Place `sStartingCellFix.dll` under `DATA/SKSE/Plugins`, or use respective mod organizer/manager to install the plugin.

# Build Dependency
This project requires following vcpkg package to build:
* [alandtse's commonlibSSE-NG fork](https://github.com/alandtse/CommonLibVR/tree/ng)
* [xbyak](https://github.com/herumi/xbyak)

# Changelog
* April.13.2025 initial release.
