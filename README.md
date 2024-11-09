# c-coursework-comp0002
 
Compiling, assuming the graphics.h and graphics.c files are in the same directory as the unzipped files.  
```
gcc -o robot *.c
```

Executing, assuming the drawapp-4.0.jar file is in the same directory as the robot executable.
```
./robot | java -jar ./drawapp-4.0.jar
```
Command line arguments, each requires the predecessor to also be defined to be parsed.
    - Pixel size of a grid square (unsigned integer > 0). 
        Default value: 20.
    - Maximum map row size (unsigned integer > 5). Also requires column size being passed as an argument. 
        Default value: 20.
    - Maximum map column size (unsigned integer > 5). 
        Default value: 20.
    - Number of markers on map (including home) (unsigned integer > 0). 
        Default value: 2.
    - Map generation type identifier (char, options: 'e' for the eaten map, any other for an empty map). 
        Default value: EMPTY_MAP_ID.

Example of command line arguments:
```
./robot 40 10 15 7 e | java -jar ./drawapp-4.0.jar
```

Display key:
    - Map:
        - Map border: red squares.
        - Obstacles: red squares.
    
    - Robot: green triangle, pointing in it's current direction (where it would move if it moved forward).

    - Non-home marker: blue square.
    - Home marker: purple square. 

The program generates a map, which may or may not include obstacles, depending on the specified map
generation identifier.
The program then places a robot on the map, with a random starting position and direction. The robot
cannot occupy a map space which contains an obstacle.
The program then places a specified number of markers at unique locations on the map, with at least one
(the home marker).
The program then uses a pathfinding algorithm (breadth-first search), combined with a tracing function,
to generate a path between the robot and the first non-home marker.
The robot follows this path, and collects any markers on the way, or the target marker.
The robot will repeat the previous two stages until it has collected all non-home markers.
The robot then uses the same pathfinding approach to generate a path to the home marker, where it will 
terminate.