# CS50 w17 Team 5:  Benjamin Coleman, Billy Kwon, Oliver Levy, Lessley Hernandez
# "The Maze Challenge" - Design Spec
&nbsp;

## User Interface 
The AMStartup’s only interface with the user is on the command-line.

Example of intput: 
	./AMStartup -n 2 -d 0 -h flume.cs.dartmouth.edu

 *	-n `nAvatars`: (int) the number of Avatars in the maze
 *	-d `Difficulty`: (int) the difficulty level, on the scale 0 (easy) to 9 (excruciatingly difficult)
 *	-h `Hostname`: (char *) the hostname of the server. Our server will be running on flume.cs.dartmouth.edu

## AMStartup
- Startup script for our amazing project

### Input 
The progam takes three parameters.
 - nAvatars: An integer representing the number of avatars to be initialized in the maze. The value must be equal to or greater than 2. (-n )
 - Difficulty: An integer representing the size and complexity of maze. Accepted values range from 0-9, with 0 being the easiest difficulty and 9 being the hardest.(-d )
 - Server: The string path of the server to connect to for running the maze. 

### Output
Error or Success messages.

### Exit Status
 		0 - No errors
 		1 - Incorrect formatting of args
 		2 - Invalid arguments
 		3 - Maze Initialization Failed
 		4 - Stream (file or socket) error
 		5 - Thread error creation


### Data Flow
Communication with Server:
This module will send an `INIT` message to the server containing the number of avatars and the difficulty of the maze. It will also recieve an `INIT_OK` message containing the maze height, width, and port number for the client threads.

## Client Module 
### Input 
Client takes six parameters:
 - `Server`: The IP address of the sever 
 - `Port`: An integer representing the port on the server to connect to.
 - `ID`: An integer represetning the ID
 - `File`: String representing the open log file
 - `NAvatars`: An integer representing the number of avatars. 
 - `Difficulty`: An integer representing the compexity of the maze.

All parameters are passed to this module by the `AMStartup` module.
 Initialize a struct for the avatars to access values needed to initialize the client side: 
- 	`int avatarId` :	This is the current avatars Id that must be from 0-9
- 	`int nAvatars` : 	This value holds the number of avatars on the maze
- 	`int difficulty` : This value holds the level of difficulty
- 	`char *hostname` : To create and connect a socket for each avatar a hostname is needed
- 	`int mazePort` : Recieved from server reply and needed to creat and connect to a socket
- 	`char *logFile` :The log file take all input and details it to help follow along the maze challenge algorithm history
-	`mazeGraph* mGraph` : The data structure that holds helps us thorughout the maze challenge
-	`AM_Message *message` :Points to the last message received
-	`int xPos` :Holds the previous x position
-	`int yPos` :Holds the previous y position

### Output
Error or Success messages.

### Data Flow
Communication with Data Structures:
The client thread that has last moved will update the `mazeGraph` structure. If the avatar is backtracking or has hit a wall, it will remove pointers between coordinate nodes. Otherwise, the threads will update visited nodes after each move to show they have been there. 
Communication with Server. 

## Messages Module
### Purpose
Contains all logic for recieving and sending messages to and from the server.

### Decomposition of Modules 
- `initializeMsg` : Initializes messages to prep and send to set the message to send. 
- `avatarReadyMsg` : Used to take information from AM_Avatar_Ready message this returns true if this was sucessful. Otherwise, returns false. 
- `avatarMoveMsg`:Used to take information from AM_Avatar_Move message this returns true if this was sucessful. Otherwise returns false. 
- `receiveMsg` : Receives message and communicates to server. This function returns true if it was sucessful. Otherwise returns false.
- `parseInitMsg`:Parses the initiation message to get the value from the message given by the server. In this case the maze height and maze width. This function does not return any value. 


### Dataflow through functions 
Most of the data is taken from AMStartup and passed as parameters in message function. In this case, the `AM_Messages, commSock, turnId, mazeWidth, mazeHeight, avatarId, position(pos), direction, and mazePort`. Only `initializeMsg`, `avatarReadyMsg`,`avatarMoveMsg`, `receiveMsg` functions return true/false. 


### Major Data Structures
#### `mazeGraph`:
Our data structure representing the maze with be an undirected graph of coordinate nodes. We will implement the undirected graph using a 2D matrix with the width and height of the maze.
Each node will be a structure containing: 
- Integers for its x and y position
- Coordinate node pointers to point to the nodes above, below, and to the sides of it
- A variable to record which avatars have traversed the node

Each node represents a coordinate within a maze. A pointer between coordinate nodes represents a possible path an avatar could take. A wall or dead end path will be represented by an absence of pointers between two coordinate nodes. The edge coordinate nodes of the maze will have pointers to NULL. For example, the top row of coordinate nodes will not have any coordinate nodes that would be represented  above it, and all the corner coordinate nodes will only have two valid pointers.  The strucuture will be initialized with all pointers between coordinate nodes, and will be updated as avatars traverse the maze.  


## Testing Plan for Maze Challenge 
&nbsp;
 - Unit testing. A small test program to test each module to make sure it does what it’s supposed to do. Unit test for each module through test cases we will begin developing after each function is complete. We plan to go over test and edge cases and create a unit test accordingly. Then move on to the next module. 
 - After unit testing, we plan to start integration testing between communication, messaging, datatstructure, and avatar modules. Assemble the Maze Challenge modules together and test it as a whole. In each case, examine the output files and the data structure input/output flow carefully to be sure they have the correct action saved for each move/Avatar, path, timing, and communication with server is up to date.
 - Then we will begin to test our strategy on the first four mazes and if it works we will continue to optomize and make out code as efficient and bug free as we can. Or we will edit our strategy and begin out testing plan again. 

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