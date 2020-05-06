# Crossy Road

[![license](https://img.shields.io/badge/license-MIT-green)](LICENSE)
[![docs](https://img.shields.io/badge/docs-yes-brightgreen)](docs/README.md)

Read [this document](https://cliutils.gitlab.io/modern-cmake/chapters/basics/structure.html) to understand the project
layout.

**Author**: Namrata - [`vajrala3@illinois.edu`](mailto:example@illinois.edu)

**PROJECT DESCRIPTION**

* Crossy Road is a popular arcade-like game that implements a 
character and multiple obstacles that it needs to overcome.
* The character (or the player) needs to cross a road with 
obstacles of different velocities and shapes coming at it. 
* The difficulty levels of the game can be changed by changing 
the speed that the player wants to play with
* The score that the player recieves is based on how many lanes 
the player successfully passes 
* The player can move based on keyboard commands 
* There are two types of obstacles:
    * **Pink obstacles:** if the crosser hits these, the game ends
    * **Blue obstacles:** the crosser has to jump onto these
    to be safe. If the crosser hits the background, 
    they're out
* The overall objective of the game is to get the player across the 
street

* There are two versions to this game:
    * **Single player:**
        * The objective is for the player to get across to 
        the other end.
        * The player will get a higher score the higher 
        up the board it goes and the higher the speed is set to
        
    * **Double player:**
        * In this version, the game can be played with two players
        on one device.
        * If any player loses while getting across the board, the 
        game ends for both players
        * If the game ends for both players, the player with the 
        best score wins the game
        * If any player gets to the top and wins, they are 
        automatically the winner of the game
        * If both players end on the same level, it's 
        a tie game
        
    **Keys needed for game:**
    
    Key | Action
    ------------ | -------------
    up arrow | crosser moves up
    down arrow | crosser moves down
    left arrow | crosser moves left
    right arrow | crosser moves right
    w | second crosser moves up
    s | second crosser moves down
    a | second crosser moves left
    d | second crosser moves right
    q | reset/restart the game
    
    **Program Arguments for the game:**
    
    Flag | Action
    ------------ | -------------
    --speed= | int speed value of obstacles
    --name= | string name of player
    --multiplayer= | bool true if double player or false if single player
    

**BACKGROUND KNOWLEDGE**

The background knowledge I have going into coding this game consists 
of some knowledge of the use of cinder. As a previous 126 project, 
we coded the snake game which utilized block functionality.
However, I don't know a lot of how to build a project like this from 
scratch as a lot of the implementation in snake was already given to us.
I also faced many issues with the project such as random
crashes and issues with audio files. I want to be able to build a 
functioning game from scratch while implementing each little aspect of
the game that makes it so interesting. It also implements a lot of 
different features that are different than anything I have encountered 
in this class before. For example, the obstacles that I am creating
are going to have different features that are efficiently structured.
I am also going to incorporate elements dealing with audio, special 
effects, graphics, shapes, etc that are going to be interesting to learn 
about. I'll also be working with a grid and calculating 
different dimensional aspects that I haven't dealt with before.
I am looking forward to using new libraries and building a 
functioning and fun game from scratch. 

**EXTERNAL LIBRARY**

The new external library for this project is 
[SqliteModernCpp](https://github.com/SqliteModernCpp/sqlite_modern_cpp/tree/dev).

This library enables score saving functionality for 
the game. All the scores and names of the game are 
stored in a database and the highest scores are
retrieved.

**TIMELINE**

* Week One: 
    1. Set up library in the project 
    2. Thoroughly read through the provided tutorials and 
    understand how library works
    3. Go through cinder API to learn the new functionality 
    needed for my game
    4. Set up basic layout of the game
    5. Make the different classes required for my character and 
    obstacles 
    6. Design the entire structure of the game
    7. Test some functionality to check if project works 
    8. Get started on displaying characters 
    
* Week Two:
    1. Implement funcionality of character
    2. Implement all the obstacles 
    3. Make the functionality be user friendly for interaction
    4. Set up the speed, graphics, and look of the board
    5. Implement movement for obstacles 
    6. Add it so that the character can move based on different 
    keys 
    
* Week three:
    1. Implement the score 
    2. Make sure collisions with obstacles makes the game end
    3. Test if different edge cases work with the game
    4. Add final features such as sound 
    5. Fix any bugs/errors that occur
    6. Implement anything that is not finished by week two 
    7. Have a complete running application
    8. Implement extra features if time (multiplayer)
    

**STRETCH GOALS**

Extra features for this game include:
* Greater user interaction with the game and the user
* Multiplayer functionality 

**Instructions for project**
* Make sure you have an IDE such as 
Clion set up
* Set up [Cinder](https://libcinder.org/)
* Clone the game's repository 
* open the cinder project that you 
set up 
* Clone the repository into this project
* Make sure 
[SqliteModernCpp](https://github.com/SqliteModernCpp/sqlite_modern_cpp/tree/dev)
is set up and running:

In CMakeLists under the tests folder:
```
# Database library
FetchContent_Declare(
        sqlite-modern-cpp
        GIT_REPOSITORY https://github.com/SqliteModernCpp/sqlite_modern_cpp.git
        GIT_TAG a0f96c10f04d96ccea1b8a49d5998c96e7887bf4
)
```
```
# Adds sqlite_modern_cpp.
FetchContent_GetProperties(sqlite-modern-cpp)
if (NOT sqlite-modern-cpp_POPULATED)
    FetchContent_Populate(sqlite-modern-cpp)
    add_library(sqlite-modern-cpp INTERFACE)
    target_include_directories(sqlite-modern-cpp INTERFACE ${sqlite-modern-cpp_SOURCE_DIR}/hdr)
endif ()
```
In CMakeLists under the src folder:
```
ci_make_library(
        LIBRARY_NAME mylibrary
        CINDER_PATH  ${CINDER_PATH}
        SOURCES      ${SOURCE_LIST}
        INCLUDES     "${FinalProject_SOURCE_DIR}/include"
        LIBRARIES   sqlite-modern-cpp sqlite3
        BLOCKS      Cinder-ImGui
)
```
* Make sure external library [Gflags](https://github.com/gflags/gflags) is set up and running 

In CMakeLists under the tests folder:

```
FetchContent_GetProperties(gflags)
if(NOT gflags_POPULATED)
    FetchContent_Populate(gflags)
    add_subdirectory(${gflags_SOURCE_DIR} ${gflags_BINARY_DIR})
endif()
```

```
FetchContent_Declare(
        gflags
        GIT_REPOSITORY https://github.com/gflags/gflags.git
        GIT_TAG v2.2.2
)
```
In CMakeLists under the src folder:

```
ci_make_library(
        LIBRARY_NAME mylibrary
        CINDER_PATH  ${CINDER_PATH}
        SOURCES      ${SOURCE_LIST}
        INCLUDES     "${FinalProject_SOURCE_DIR}/include"
        LIBRARIES   gflags sqlite-modern-cpp sqlite3
        BLOCKS      Cinder-ImGui
)
```
* Run the project to check if it works