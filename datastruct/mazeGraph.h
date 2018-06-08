/*
 * Header file for mazeGraph
 * Contains all public functions and explanations
 * for the graph used as the datastructure in maze Challenge
 * CS50 W18 Group 5
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/**************************************************************
 * mazeGraph
 * An array of arrays of coordinate nodes
 * Represents the entire graph
 * Keeps track of current Avatar turn
 * Keeps a walls representation as well for printing purposes
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
bool move_success(mazeGraph *maze, int x, int y, int direction, int avatar);
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
/*********************************************************
 * Getter for walls array structure
 *
 * Ret:
 * 		int ** walls representation of the graph for easy printing with graphics
 */

int **maze_get_walls(mazeGraph *maze);
/*********************************************************
 * Getter for the maze height
 * Ret:
 * 		int height	- the height of the maze
 */
int maze_get_height(mazeGraph *maze);
/*********************************************************
 * Getter for the maze width
 * Ret:
 * 		int width	- the width of the maze
 */
int maze_get_width(mazeGraph *maze);

/*********************************************************
 * Testing print function
 * Prints the entire maze, including links and coordinates
 */

void coord_print(mazeGraph *maze);
