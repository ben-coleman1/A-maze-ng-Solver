# CS50 w17 Team 5:  Benjamin Coleman, Billy Kwon, Oliver Levy, Lessley Hernandez
# "The Maze Challenge" - README
&nbsp;

##  Project Description
- Maze Challenge can be found in the top level README.md. 

##Description of modules in src

- `AMStartup` : Startup script for our amazing project. This parses arguments and begins the threads, the avatars, or clients. This program has `#ifdef DISPLAY`s which are switches to turn on and off the gdk graphics. As a note if you valgrind with gdk on, this will output memory leaks (because of gdk's nature of not freeing memory). If `DISPLAY` is turned on, `AMStartup` will create a dedicated gdk thread which will refresh display graphics every .5 seconds, as dictated by `graphics.h`. The main thread then waits to join each avatar thread, then kills the graphics process, exiting out and returning an exit status.

- `amazing.h` : This file contains the common defines for the Amazing Project. The file is not altered from its original state from the cs50 webpage, and contains client-server communication structures, i.e.,  files for all sorts of useful constants, typedefs, structures, unions, and function declarations.

- `messages(.c .h)` : The program used to get received messages. This directs the creation, parsing, and sending of messages to and from the server.

- `simpleAvatar(.c .h)` : The directives for the avatar. This program runs the algorithm and updates the underlying mazeGraph structure.

If more detail is needed please read Implementation and Design markdown. 