solution "leyrcl_crypt"
   language "C++"
   location "project"
   targetdir "build/release"

   flags { "Optimize", "NoMinimalRebuild", "NoFramePointer", "FloatFast", "NoBufferSecurityCheck", "NoIncrementalLink", "NoManifest", "StaticRuntime"}

	if os.is("linux") or os.is("macosx") then
		buildoptions {"-m32 -fPIC -ldl -lstdc++"}
		linkoptions  {"-m32 -fPIC -ldl -lstdc++"}
	else
		linkoptions {"/SAFESEH:NO /NXCOMPAT:NO /FIXED /DYNAMICBASE:NO"}
	end
   

   vpaths {
      ["Header Files/*"] = { "src/**.h","src/**.hpp" },
      ["Source Files/*"] = { "src/**.c","src/**.cpp" },
   }

   kind "ConsoleApp"

   configurations { "Debug", "Release" }

   files { "src/**.h", "src/**.cpp" }
   
   includedirs { }
   
   links { }
   
   -- A project defines one build target
   project "leyrcl_crypt"
      targetname "leyrcl_crypt"

      configuration "Release"
         defines { "NDEBUG", "_GENERIC" }
      configuration "Debug"
         defines { "DEBUG", "_GENERIC" }
         flags { "Symbols", "EnableSSE2" }
         targetdir "build/debug"