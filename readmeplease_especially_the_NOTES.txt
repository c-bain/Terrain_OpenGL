This openGL project generates a 3D terrain varying from 50 by 50 to 300 by 300. Please read the NOTES with the asterisks(*).


Extra Features implemented:

1) A snowman character controlled by the user can walk along the terrain. Directional keys will be displayed at the start of the program.

2) Fault Algorithm

3) Improved map colouring. To see the full effect of the colouring, it is best to have many variations in height. To achieve
this, please create a large grid(for example, size 300).




MOST IMPORTANT NOTES*************************************************************************************************************************************

*****NOTE: When prompted, only enter 1 integer. For example, type 50 for a terrain of 50 by 50 vertices.

*****NOTE: When constructing very large terrains the snowman may appear to not be present but he is just hiding somewhere in a corner(sneaky fella) 
and shading from the light source[melting would be bad:( ] 
To find him, move the camera around or just keep spamming 'r' until he's in an easy to see location :D


*****NOTE: If the terrain appears flat from above, then use the directional keys to move the camera closer to see the variations in height.


*****NOTE: The spheres in the sky represent the light sources. When the lights are off the terrain is coloured in a greyscale fashion(daytime).
When the lights are turned on(nighttime) the terrain is illuminated in yellow. The terrain will become pitch black if no light source is near(nighttime).
When the light is toggled, whether it's night or day is printed on the command line. 

*****NOTE: The light is shone outwards towards the terrain from the yellow spheres. To see the effect of the light on the entire terrain at once, use 
the light movement keys to move the spheres away from the terrain so the angle of the rays can meet. To check the lighting, it 
is best to create a terrain of size 150 and then use the 'i' and 'b' keys to move the lightsource away. 




********************************************************************************************************************************************************


The controls are outlined when the program is started but they will also be outlined here for your convenience:

 "------------------Base Features----------------" 
 "Press left and right arrows to rotate the scene about the y axis" 
"Press up and down arrows to rotate the scene about the x axis" 
"Press 'w' to toggle wireframe mode"
"Press 'l' to toggle lighting"
"Press 's' to toggle shading mode; Default: Flat Shading "
"Press 'r' to generate a new random terrain"
"Press 't' to draw terrain using triangle strips"
"Press 'y' to draw terrain using quad strips"
"Press 'g to move light0 up"
"Press 'c' to move light0 left" 
Press 'b' to move light0 right" 
"Press 'v' to move light0 down" 
"Press 'i' to move light1 up" 
"Press 'z' to move light1 left"
"Press 'x' to move light1 right" 
"Press 'm' to move light1 down" 


"------------------Extra Features----------------" 
1) Press 'f' to generate terrain using fault algorithm"

2) "Press 'u' to move the snowman up"
   "Press 'j' to move the snowman down"
   "Press 'h' to move the snowman left"
   "Press 'k' to move the snowman right"

3)"Press 'p' to toggle improved map colouring"

After each key press, the selection will be displayed at the top of the command prompt.

-----------------------------------Have a nice day :D   -----------------------------------------------------

