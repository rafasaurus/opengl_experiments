-- premake5.lua
workspace "HelloWorld"
   configurations { "Debug", "Release" }

project "main"
   kind "ConsoleApp"
   language "C++"
   targetdir ""

   includedirs {
       "/usr/include/GLFW",
       "/usr/include/GL",
       "/usr/local/include"
   }
   libdirs {
       -- add dependency directories here
       "/usr/local/lib",
       "/usr/lib/"
   }
   links {
       "glfw",
       "GLEW",
       "GL"
   }

   files { "**.h", "**.cc" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
