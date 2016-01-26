#set(SOURCE_FILES src/*.cpp)
file(GLOB SOURCE_FILES
          GUI/*.h
          GUI/*.cpp
          Application/*.h
          Application/*.cpp
          BaseSubsystems/*.h
          BaseSubsystems/*.cpp
          Docs/*.h
          Docs/*.cpp
          Graphics/*.h
          Graphics/*.cpp
          Logic/*.h
          Logic/*.cpp
          Logic/Entity/*.h
          Logic/Entity/*.cpp
          Logic/Entity/Components/*.h
          Logic/Entity/Components/*.cpp
          Logic/Maps/*.h
          Logic/Maps/*.cpp
          Map/*.h
          Map/*.cpp
          Physics/*.h
          Physics/*.cpp)

#include_directories("include")

include_directories(${PROJECT_SOURCE_DIR}/Src)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/include)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/include/CEGUI)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/include/Cg)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/include/Freetype)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/include/OGRE)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/include/zzip)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/include/OIS)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/include/SDL)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/include/PhysX)

if(CMAKE_BUILD_TYPE EQUAL Debug)
    set (PHYSX_LIBRARIES
            LowLevelDEBUG
            PxTaskDEBUG
            SceneQueryDEBUG
            SimulationControllerDEBUG
            libPhysX3VehicleDEBUG
            PhysXProfileSDKDEBUG
            PhysX3ExtensionsDEBUG
            )
else()

endif()

add_executable (Galeon ${SOURCE_FILES})
#link_directories(${PROJECT_SOURCE_DIR}/dependencies/lib/Release)
target_link_libraries(Galeon stdc++ m OgreMain OIS OgreOverlay OgrePaging OgreRTShaderSystem OgreTerrain OgreVolume OgreProperty SDL2 CEGUIBase-0 CEGUICommonDialogs-0 CEGUIOgreRenderer-0 ${PHYSX_LIBRARIES})

#target_include_directories (Logic PUBLIC "src")
#target_link_libraries(Logic LINK_PUBLIC Map Graphics)
