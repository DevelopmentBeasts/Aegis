------"AEGIS"------

Game creators: Joan Marín and Jaume Avinyó

Github 
repository: https://github.com/DevelopmentBeasts/Aegis

License: https://gitlab.gnome.org/GNOME/gimp/blob/master/COPYING



-->Instructions for the execution:



+KEY IMPUTS:

-Movement:
 
W = Jump
 
A = Moving left
 
D = moving right


changed the player movement system, hope you like it;

-Debug functionalities:
 

F10 = God mode where you can fly and don't die
 
F9 = Show colliders
 
F8 = Move the camera freely and independent from the player. Press again to come back to the player (EXTRA FEATURE)
F5 = Save your game
 
F6 = Load your last saved game
 
F1 + JUMP(W) = Start of the first level
 
F2 + JUMP(W) = Start of the second level
 
F3 = Start of current level



-Innovation:




changed the collider constructor and the callback method to adapt it to entity type collisions. Collision.cpp

-Art: 

https://ansimuz.itch.io/magic-cliffs-environment (for the map)

https://rvros.itch.io/pixel-monsters (for the player and future entities)



WORK:
Jaume--------------
player.cpp (All+ changing collider system)
Render.cpp (fixed all functions that got broken with dt and entity system)
Entity system (Added player to the entity system and created the player from scene, previously maded a entity system wich end up not working correctly)
Scene.cpp (fixed debug functionalities that got broken since last release, changed the camera system when changing the map)
App.cpp (Added the timer and all functions necessary for the delta time and the numbers shown on screen)
Fixed all numbers witn delta time system
added brofiler to the code and solved memmory leaks and lag in the game.
map: changed the second map reducing the lag by 80%

Joan----
Added j1EntityManager, j1Entity, Enemy:j1Entity
Added A* PathFinding
Map::Draw() -> Blit what is whown in the window only

