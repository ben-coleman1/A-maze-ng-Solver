# CS50 w17 Team 5:  Benjamin Coleman, Billy Kwon, Oliver Levy, Lessley Hernandez
# "The Maze Challenge" - Top Lvl README
&nbsp;


##  The Maze Challenge: 
 ```
 You’re on a Caribbean Island vacation with some of your friends. It’s all sunshine and fruit drinks, when a mysterious fellow walks up to you on the beach. He offers each of you a week’s stay at the finest resort on the island if you and your friends can solve the resort’s garden maze challenge. After a brief consultation, you and your friends agree. There are some rules, the fellow says, but you all agree anyway and follow him to the resort.

He tells you that you will all be led into the maze while blindfolded. He goes on to say that some time later he will return, and if you and your friends are all together, you will win the challenge. Once you’re blindfolded and led into the maze, he instructs you and your friends to listen for a bell to ring three times at which point each of you may remove the blindfold. Each of you are then blindfolded and led into the maze. You all assume you’re together, but when the blindfolds are removed, you realize that while you are indeed in the maze, you are no longer with your friends - you can see no one. The walls of the maze are too high to see over, and they are too thick with thorns to see through or to climb over. You decide that the best course of action is to go through the maze looking for your friends. You quickly realize that you can call out to them and they can hear you and respond with hints as to where they are. Remembering that you and your friends have a limited amount of time to get back together before the man returns, you dash off through the maze to find them.

 ```



## Example command lines:

Make from outside folder, and AMStartup will appear as an executable in the outside folder

./AMStartup -n 2 -d 4 -h flume.cs.dartmouth.edu

 -   Parses command line arguments as per the specifications
 -	-n nAvatars: (int) the number of Avatars in the maze
 -	-d Difficulty: (int) the difficulty level, on the scale 0 (easy) to 9 (excruciatingly difficult)
 -	-h Hostname: (char *) the hostname of the server. Our server will be running on flume.cs.dartmouth.edu

## Instructions for Gtk

Logging in:
&nbsp;
Begin by `ssh -XY USER@flume.cs.dartmouth.edu` into flume. You will need xQuartz (which is already downloaded on the Sudikofff-005 computers - It is a bit of a hassle to download and install). The -XY option will allow the server to display graphics from the server to the computer.

Change the Makefiles:
&nbsp;
Un-comment `DISP` ("#DISP = -DDISPLAY `pkg-config --cflags gtk+-2.0 --libs`") in the `src` and `datastruct` makefiles
Un-comment the `graphics.o` object file ("OBJS = mazeGraph.o #graphics.o") in the `datastruct` makefile

Memory Errors:
&nbsp;
As a note, running valgrind with these options un-commented will produce many error messages, as the `Gtk` package avoids freeing memory until it is killed by the parent process. Running valgrind with these options commented will not produce errors, however. We have cleared this issue with Prof. Gevorg Grigoryan.

## Exit  Status:
 	0 - No errors
 	1 - Incorrect formatting of args
 	2 - Invalid arguments
 	3 - Maze Initialization Failed
 	4 - Stream (file or socket) error
 	5 - Thread error creation

### Our Maze Challenge Directory:
├── src
│	└── AMStartup.c
│	├── README.md
│	├── Makefile
│	├── amazing_client.c
│	├── amazing_client.h
│	├── amazing.h
│	├── message.c
│	├── message.h
│	├── simpleAvatar.c
│	└── simpleAvatar.h
├── datastruct
│   ├── graphics.c
│   ├── graphics.h
│   ├── Makefile
│   ├── mazeGraph.c
│   ├── mazeGraph.h
│	└── README.md
│
├── DESIGN.md
├── IMPLEMENTATION.md
├── results
│	└── Amazing_<USER>_<N>_<D>.log
├── Makefile
└── README.md

Assumptions:
- We are assuming that everyone that runs AMStartup.c is able to have Gtk display. If you do nothave Gtk display you can comment it out on the makefile in datastruct/Makefile.
- The server is running. 

Limitation: 
- Gtk has a variable .5 second lag. 
 
