------"AEGIS"------

Game creators: Joan Mar�n and Jaume Aviny�

Github 
repository: https://github.com/DevelopmentBeasts/Aegis

License: https://gitlab.gnome.org/GNOME/gimp/blob/master/COPYING



-->Instructions for the execution:



+KEY IMPUTS:

-Movement:
 
W = Jump
 
A = Moving left
 
D = moving right



When you jump and you move, you do not only describe the path of a long parabola,
 
you also gain a little extra boost to your x axis movement velocity. Also when you 
are jumping you can change 
your player's direction as many times as you want in the air. 
This makes the movement very curious and makes the game
very challenging because in this platformer,
 falling down to the holes will kill you. Enjoy!




-Debug functionalities:
 

F10 = God mode where you can fly and don't die
 
F9 = Show colliders
 
F8 = Move the camera freely and independent from the player. Press again to come back to the player (EXTRA FEATURE)
F5 = Save your game
 
F6 = Load your last saved game
 
F1 + JUMP(W) = Start of the first level
 
F4 + JUMP(W) = Start of the second level
 
F2 = Start of current level



-Innovation:

We used a diferent method to create colliders in our game. We did it because we did't want to make every entire 
tile a square collider,
we needed to have half tile colliders and more things we strugled with so we decided to define colliders 
from Tiled as square objects wich we pass to the game using the xml code
generated by the tiled map. You will find the colliders
 system in:
 Map.cpp line 229
 Mapp.cpp line 441
 Mapp.cpp line 464
(we don't know if anyone in other teams has used this system)






Credits:


-Art: 

https://ansimuz.itch.io/magic-cliffs-environment (for the map)

https://rvros.itch.io/pixel-monsters (for the player and future entities)