-- premake5.lua
workspace "DTrade"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "DTrade"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

include "DTrade"