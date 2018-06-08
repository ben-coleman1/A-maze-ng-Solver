# CS50 w17 Team 5:  Benjamin Coleman, Billy Kwon, Oliver Levy, Lessley Hernandez
# "The Maze Challenge" - Implementation Spec
&nbsp;

##  Project Description
- Maze Challenge can be found in the top level README.md. 

## About the data structures
### Maze Graph 
    -  MazeGraph is our data structure that consists of many data structures that underlie it. It records aspects of the maze and contains a two dimensional array of nodes that represents coordinate nodes that are initially linked together. Links are removed to represent walls. The structure uses these links to determine how an avatar should move through the maze. We have hard coded instructions in the main of mazeGraph.c and run them with multiple avatars. You can test this with cd'ing into datastruct/, uncommenting the #TESTING line in its Makefile, and running 'make test'. This shows two avatars as they traverse a made-up maze, and print their status.


#### mazeGraph functions
The function below is the skeleton of what we will be working with when implenting our datastructure so far. 
```c
/**************************************************************
 * mazeGraph
 * Represents the entire maze.
 * An array of arrays of coordinate nodes.
 * These nodes are linked to their neighbor nodes through pointers
 * to north, east, west, and south.
 * Keeps track of current Avatar turn
 * Keeps a walls representation as well for printing purposes
 * Robust datastructure to represent the maze, with many functions
 */
typedef struct maze mazeGraph;
/**************************************************************
 * Initialize a fully-linked array of nodes
 * With dimensions (mazeWidth, mazeHeight)
 * Ans the walls (i.e. everything about the maze)
 * Args:
 * 		int mazeWidth	- the width of the maze
 * 		int mazeHeight	- the height of the maze
 * 		int nAvatars	- the number of avatars
 */
mazeGraph *graph_init(int mazeWidth, int mazeHeight, int nAvatars);
/**************************************************************
 * Returns the possible moves of the maze at a coordinate (x,y)
 * User must free the returned array
 * Args:
 * 		mazeGraph *maze	- the maze
 * 		int x			- x coordinate
 * 		int y			- y coordinate
 *
 * Ret:
 * 		Integer array with boolean (1/0)
 * 		[West, North, South, East]
 * 		(According to amazing.h, West = 0, North = 1, etc.
 */
int *possible_moves(mazeGraph *maze, int x, int y);
/*********************************************************
 * Deletes the entire maze
 * This function does not return anything.
 * Args:
 * 		mazeGraph *maze	- the maze
 */
void graph_delete(mazeGraph *maze);
/*********************************************************
 * Returns whether the coordinate at (x,y)
 * has been visited by the avatar specified
 *
 * Args:
 * 		mazeGraph *maze	- the maze
 * 		int x			- x coordinate
 * 		int y			- y coordinate
 * 		int avatar		- the avatar specified
 */
bool has_visited(mazeGraph *maze, int x, int y, int avatar);
/*********************************************************
 * Returns whether the coordinate at (x,y)
 * has been visited by any avatar
 *
 * Args:
 * 		mazeGraph *maze - the maze
 *      int x           - x coordinate
 *      int y           - y coordinate
 */
bool is_visited(mazeGraph *maze, int x, int y);
/*********************************************************
 * Initializes an avatar at the specified coordinate
 * Updates the graph
 * Sets the coordinate at (x,y) to visited by the avatar
 *
 * Args:
 * 		mazeGraph *maze - the maze
 *      int x           - x coordinate
 *      int y           - y coordinate
 *      int avatar		- the avatar specified
 */
void init_avatar(mazeGraph *maze, int x, int y, int avatar);
/*********************************************************
 * Prints the Maze in a nice format
 *
 * Args:
 * 		mazeGraph *maze - the maze
 */

void maze_print(mazeGraph *maze);
/*********************************************************
 * Computes a move success, and updates the walls and the graph
 * Sets the new coordinate as visited by the avatar
 * Removes a "?" from the maze_print() representation
 * Does not alter the graph, as we know now that there is a valid path
 *
 * Args:
 * 		mazeGraph *maze	- the maze
 * 		int x			- x coord from which the move was made
 * 		int y			- y coord from which the move was made
 * 		int direction	- the direction moved from (x,y) to the updated coord
 * 							(i.e. the move message you sent the server)
 */
void move_success(mazeGraph *maze, int x, int y, int direction, int avatar);
/*********************************************************
 * Computes a move fail, and updates the walls and the graph
 * Removes a link between the current coordinate and the coordinate failed
 *
 * Args:
 * 		mazeGraph *maze	- the maze
 * 		int x			- x coord from which the move was made
 * 		int y			- y coord from which the move was made
 * 		int direction	- the direction moved from (x,y)
 * 							(i.e. the move message sent to the server
 *
 */
void move_fail(mazeGraph *maze, int x, int y, int direction);
/*********************************************************
 * Computes whether the previous coord (x,y) from which the avatar traveled
 * is a dead end
 * Assumes it is called after a successful move, and after move_success is run
 * (As move_success updates the new coordinates, etc)
 * Updates the graph by removing a path between (x,y), the old coordinate
 * 	and the previous coordinate, if the old coord is a dead end
 *
 * Args:
 * 		mazeGraph *maze	- the maze
 * 		int x			- the x coordinate from which the avatar just moved
 * 		int y			- the y coordinate from which the avatar just moved
 * 		int direction	- the direction in which the avatar moved from (x,y) to 
 * 							to the current position
 * 		int avatar		- the avatarId
 */
void dead_end_coord(mazeGraph *maze, int x, int y, int direction, int avatar);
```
## AMStartup
- AMStartup is where we call our other processes 
- 
### Peusdocode for `AMStartup`
1. Parse arguments to check validity 
2. Create `AM_INIT` message to send to the `AM_SERVER_PORT`
	- This message specifies the desired number of Avatars in the maze and the difficulty of the maze	
3. Recieve and Parse `AM_INIT_OK` message sent from the server
	- This message contains a unique `MazePort` which is used to communicate with the server about this new maze. 
	- The server begins listening on that new `MazePort` immediately. 
	- The message also includes the `MazeWidth` and `MazeHeight` of the new maze (two more integers).
4. Create and open the log file
	- The log file is used by the Avatars to log their success/progress. 
	- Later the log file is used the Avatars ensure that the `AM_MAZE_SOLVED` message is written to the log file.
5. Create the shared data structure described above. 
6. Start client threads
7. Clean up
	- End thread processes
	- free data structure memory
	- close log file

## Messages
-   Messages.c  sends and receives messages to and from the server. It handles most of our client-server communication.
#### Messages functions
```c
/*******************************************
* Initializes messages 
 * @initMessage: to prep and send the message
 * @commSock: Sock. value we are com.
 * @AvatarId: to find which avatar we are on
 * @difficulty: lvl of dif. for maze
 * returns an bool to verify if msg was sent
 * assume we are passing a malloc msg
 * if int > 0 it was sucessful
 * if int < 0 then it failed
 */ 
 bool initializeMsg(int commSock, int nAvatars,int difficulty);
   
/*******************************************
*Avatar Ready Message
 *@AvatarMsg: used to take in msg for info
 *@commSock: Sock. value we are com. to
 * @AvatarId: to find which avatar we are on
 * returns true if message read was sucessful
 * if was not able to send msg returns false 
 */
bool avatarReadyMsg(int commSock, int avatarId);
  
/*******************************************
*Avatar move Message
 *@AvatarMsg: used to take in msg for info
 *@commSock: Sock. value we are com. to
 * returns true if message read was sucessful
 * if was not able to send msg returns false 
 */
bool avatarMoveMsg(int commSock, int avatarId, int direction);
  
/*******************************************
 * receiveMsg 
 * @AM_Message: Message we are receiving from server
 * @commSock: Sock. value we are com. to
 * @nAvatars:  num of avatars
 * @difficulty: lvl of dif. for maze
 * Takes in the msg and communicates with server
 * return true if sucessful 
 * return false could not receive msg
 */ 
bool receiveMsg(AM_Message *message, int commSock);
  
/*******************************************
* parseInitMsg
 * @AM_Message: Message we are receiving from server
 * @mazePort: Port used for maze
 * @mazeHeight: The height of a maze
 * @mazeWidth: The with of a maze
 * Make sure to use w/ address op. 
 * Does not return anything
 */ 
void parseInitMsg(AM_Message *message, int *mazePort, int *mazeHeight, int *mazeWidth);
/*******************************************
* socket_setup
 * @hostname: 
 * @sockaddr_in *themp:  
 * @mazePort: 
 * returns the socket. 
 */ 
int socket_setup(char *hostname, struct sockaddr_in *themp, int mazePort);
```

#simpleAvatar
### Avatar Script Pseudocode
1. First rule we establish is to move right. Validates data structure. 
	- if this move was successful, then mark the nodes as visited and let the avatar continue exploring the right direction. 
	-  Otherwise, change the node pointer for that cordinate node to NULL to symbolize a wall. To make sure that the Avatars do not explore the same path. Then we will explore the counter-clockwise direction until we find an open path.
3. Mark each node as visited by the Avatar and when an Avatar lands on a node twice then we cut off the node in the direction the Avatar came from. 
4. Otherwise:
	- If the node the Avatar came in on has only one mark, turn around and return along that node, marking it again. This will happen when the Avatar reaches a dead end.
	- If not, the Avatar will go to the right node with the fewest marks, follow that path, and repeat process
5. Each Avatar begins as it own group and untill Avatars are in the same x and y coordinate we will group them together and take the same move during there turns. 
#### simpleAvatar functions
```c
/* firstAvatarMove
 * @mGraph: the mazegraph 
 * @pos: the curr position of the avatar
 * @avatar: the current avatar id
 * init avatar on the first move
 * Does not return anything
 */ 
void firstAvatarMove(mazeGraph* mGraph, XYPos pos, int avatar);
/* decsionMaker
 * @mGraph: the mazegraph 
 * @pos: the curr position of the avatar
 * @prevDirection: Keeps track of the previous direction so N,E,S,W
 * @avatar: the current avatar id
 * @turnNuber: A variable used to keep track of what turn the avatar is on
 * Makes the decision on which way to move that enforces the right hand rule
 * returns an integer which represents the cardinal direction
 */ 
int decsionMaker(mazeGraph* mGraph, XYPos pos, int prevDirection, int avatar, int turnNumber);
/* comparCoord
 * @pos1: curr position
 * @pos2: prev position
 * Nospecific order 
 * This is a function used to check whether the avtar moved from its original position
 * This returns true when pos1 and pos2 are equal to eachother
 * Oherwise it returns false
 */ 
bool comparCoord(XYPos pos1, XYPos pos2);
/* updateAvatarGraph
 * @mGraph: the mazegraph 
 * @pos: the curr position of the avatar
 * @Prevpos: the previous position of the avatar
 * @avatar: the current avatar id
 * @direction: the cardinal direction it was just heading in
 * returns true if move was sucessful 
 * otherwise returns false when move was not sucessful
 */ 
bool updateAvatarGraph(mazeGraph* mGraph, XYPos pos, XYPos Prevpos, int avatar, int direction);
```

## Amazing Client
- 
### Peusdocode for `amazingClient`
1. `AM_AVATAR_READY` messages from all N Avatars assigned to that `MazePort`,
	-  the server will send an identical `AM_AVATAR_TURN` message to each Avatar. This message contains each
	-  Avatar’s current (x,y) position in the maze and a `TurnID` indicating which Avatar is to move first.
2. The if there is no error so far then we [iterate/loop] through the Avatars to find out the order in which the Avatars will move.
	- Check that the avatar ID if it exist and increment turn count 
	- Establish the order of avatars/ establish a queue to keep order of avatars. Update our turn ID.
	- Begin avatar script

#### amazing_client functions
The function below is the skeleton of what we will be working with when implenting our datastructure so far. 
```c
/*initAvatar
 *@ptr: pointer in the init stucture
 * This function initializes the avatar for AMStartup
 * returns nothing 
 */
void *initAvatar(void *ptr);
```
### General Notes
Important to note that in the log files the last avatar move will say that it was blocked by a wall and did not move; however, it actually moves to the finishing the move as seen by the MAZE SOLVED message. Because of the implimintation with the while loop their is a thread that


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


-   Each README (or README.md) explains any assumptions and acknowledges any limitations.
-   Each TESTING (or TESTING.md) shows how we tested the module.
