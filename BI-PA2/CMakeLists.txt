cmake_minimum_required(VERSION 3.25)
project(semestral)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS)

include_directories(libs)

add_executable(semestral src/main.cpp src/CApp.cpp src/CApp.h src/CPlayerStats.cpp src/CPlayerStats.h src/CGame.cpp src/CGame.h src/CBuilding.cpp src/CBuilding.h src/CBuilding.cpp src/CBuilding.h src/CPlayerInputHandler.cpp src/CPlayerInputHandler.h src/CFarm.cpp src/CFarm.h src/CPlayer.cpp src/CPlayer.h src/CFactory.cpp src/CFactory.h src/CMine.cpp src/CMine.h src/CDeposit.h src/CDeposit.cpp src/CAction.cpp src/CAction.h src/CPosition.cpp src/CPosition.h src/CJsonCheck.cpp src/CJsonCheck.h src/CUpgradeBuilding.cpp src/CUpgradeBuilding.h src/CActivateBuildingAction.cpp src/CActivateBuildingAction.h src/CDeActivateBuildingAction.cpp src/CDeActivateBuildingAction.h src/CDestroyBuildingsAction.cpp src/CDestroyBuildingsAction.h src/CEndgameScenarios.cpp src/CEndgameScenarios.cpp src/CEndgameScenarios.h src/CInHandler.cpp src/CInHandler.h src/CInputHandlerAi.cpp src/CInputHandlerAi.h src/CCataclysmicEvent.cpp src/CCataclysmicEvent.h)

target_link_libraries(${PROJECT_NAME})

