solution "leyrcl"
   language "C++"
   location "project"
   targetdir "build/release"

   flags { "Optimize", "NoMinimalRebuild", "NoFramePointer", "FloatFast", "NoBufferSecurityCheck", "NoIncrementalLink", "NoManifest", "NoRTTI", "NoExceptions", "StaticRuntime", "NoImportLib"}

	if os.is("linux") or os.is("macosx") then
		buildoptions {"-m32 -fPIC -ldl -lstdc++"}
		linkoptions  {"-m32 -fPIC -ldl -lstdc++"}
	else
		linkoptions {"/SAFESEH:NO /NXCOMPAT:NO /FIXED /DYNAMICBASE:NO /ENTRY:newmain"}
	end
   

   vpaths {
      ["Header Files/*"] = { "src/**.h","src/**.hpp" },
      ["Source Files/*"] = { "src/**.c","src/**.cpp" },
   }

   kind "ConsoleApp"

   configurations { "Debug", "Release" }

   files { "src/**.h", "src/**.cpp" }
   
   includedirs { }
   
   links { "ws2_32", "Vfw32.lib", "./libs/wcrt.lib", "./libs/libx264.lib", "winmm" }
   
   -- A project defines one build target
   project "leyrcl"
      targetname "leyrcl"

      configuration "Release"
         defines { "NDEBUG", "_GENERIC" }
      configuration "Debug"
         defines { "DEBUG", "_GENERIC" }
         flags { "Symbols", "EnableSSE2" }
         targetdir "build/debug"