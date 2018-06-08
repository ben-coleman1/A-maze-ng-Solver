/* mazeGraph  and mazeNode Structure; Final Progect; Ben, Billy, Lessley, and Oliver
 * 
 * Structures to represent the maze
 * CS50 W18 Group 5
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mazeGraph.h"
#include <string.h>
#include <unistd.h>
//#include "graphics.h"

/******************** Structures *********************/
/* mazeNode
 * Represents a coordinate in the maze
 */
typedef struct node{
// Position of the node
	int x;
	int y;

// Pointers to neighboring nodes
	struct node *east;
	struct node *north;
	struct node *south;
	struct node *west;

// Array of Avatar ID's that have visited this node
	int *visited;
	
} mazeNode;

/* mazeGraph
 * An array of arrays of coordinate nodes
 * Represents the entire graph
 * Keeps track of current Avatar turn
 */
typedef struct maze{
	mazeNode ***graph;
	int **walls;

	int width;
	int height;
	//current Avatar turn	
	int turn;
	int nAvatars;
	bool explorer;
	int **avInfo;
}mazeGraph;

/****************** Prototype Functions *******************/

// Public functions. See mazeGraph.h for details

// Initializes a graph
mazeGraph *graph_init(int mazeWidth, int mazeHeight, int nAvatars);

// Deletes (frees) a graph
void graph_delete(mazeGraph *maze);

// Returns the possible coordinate "links" from a given coord. Must be freed
int *possible_moves(mazeGraph *maze, int x, int y);

// Returns whether the avatar has visited the x,y coord
bool has_visited(mazeGraph *maze, int x, int y, int avatar);

// Returns whether any avatar has visited the x,y coord
bool is_visited(mazeGraph *maze, int x, int y);

// Avatar's move succeeded from (x,y) in direction. Updates the graph
bool move_success(mazeGraph *maze, int x, int y, int direction, int avatar);

// Avatar's move failed from (x,y) in direction. Updates the graph
void move_fail(mazeGraph *maze, int x, int y, int direction);

// Initializes the avatar. Should only be called once per avatar
void avatar_init(mazeGraph *maze, int x, int y, int avatar);

// Computes whether previous coord (x,y) is a dead end. Direction is in the moved direction
void dead_end_coord(mazeGraph *maze, int x, int y, int direction, int avatar);

void coord_print(mazeGraph *maze);

// Static helper functions
static bool hasNorth(mazeNode *node);
static bool hasSouth(mazeNode *node);
static bool hasEast(mazeNode *node);
static bool hasWest(mazeNode *node);
static int *get_visited(mazeGraph *maze, int x, int y);
static int next_coord(int coord, bool is_x,  int direction);
static void visit(mazeGraph *maze, int x, int y, int avatar);
static void find_deadends(mazeGraph *maze);
static void remove_path(mazeGraph *maze, int x, int y, int direction);
// Node functions helper
static mazeNode *node_init(int x, int y, int nAvatars);
static mazeNode *get_node(mazeGraph *maze, int x, int y);
static void node_delete(mazeNode *node);
static int num_on_coord(mazeGraph* maze, int x, int y);
// Helper print functions
static void maze_print_helper(int value, int x, int y);
/****************************************************/




/****************** Local Functions *****************/
// node init function
static mazeNode *node_init(int x, int y, int nAvatars)
{
	// Initialize the node. If null, return null
	mazeNode *node = malloc(sizeof(mazeNode));
	if (node == NULL) {
		return NULL;
	}

	node->x = x;
	node->y = y;
	node->north = NULL;
	node->east = NULL;
	node->south = NULL;
	node->west = NULL;
	
	// Initialize the visited array. If null, exit with error
	node->visited = malloc(nAvatars * sizeof(int));
	if (node->visited == NULL) {
		free(node);
		return NULL;
	}
	memset(node->visited, -1, sizeof(int) * nAvatars);
	// Return the constructed node
	return node;
}



// Delete a node
static void node_delete(mazeNode *node)
{
    free(node->visited);
    free(node);
}



// Return if the node hasNorth
static bool hasNorth(mazeNode *node)
{
    return (node->north != NULL);
}



// Return if the node hasSouth
static bool hasSouth(mazeNode *node)
{
	return (node->south != NULL);
}



// Return if the node hasEast
static bool hasEast(mazeNode *node)
{
    return (node->east != NULL);
}



// Return if the node hasWest
static bool hasWest(mazeNode *node)
{
    return (node->west != NULL);
}



// Return the visited tag for a coordinate (x,y)
static int *get_visited(mazeGraph *maze, int x, int y)
{
    mazeNode *node = get_node(maze, x, y);
    if (node == NULL) {
        return NULL;
    }
    return (node->visited);
}



// get_node
static mazeNode *get_node(mazeGraph *maze, int x, int y)
{
    if ( x < 0 || x > maze->width-1 || y < 0 || y > maze->height-1 ) {
        return NULL;
    }
    return maze->graph[x][y];
}



//print_maze_helper-helper for maze_print
static void maze_print_helper(int value, int x, int y)
{
    switch(value) {
        // Valid path
        case 10:
            printf(" ");
            break;
        // Invalid Path
        case 11:
            printf("x");
            break;
        // Unknown path
		case 12:
			printf("?");
			break;
		// Top wall
		case 13:
			printf("—");
			break;
		// Side wall
		case 14:
			printf("|");
			break;
		// Junction
		case 15:
			printf("+");
			break;
		// Valid non-wall
		case 16:
			printf(" ");
			break;
		// Unknown wall
		case 17:
			printf("?");
			break;
		// Avatar from 0-9
		default:
            printf("%d", value);
    }
}



// coord_to_extend finds the coord in the direction
static int next_coord(int coord, bool is_x,  int direction)
{
	switch(direction) {
		// West
		case 0:
			if (is_x) {
				return coord-1;
			}
			else {
				return coord;
			}
		// North
		case 1:
			if (is_x) {
				return coord;
			}
			else {
				return coord-1;
			}
        // South
		case 2:
			if (is_x) {
				return coord;
			}
			else {
				return coord+1;
        	}
		// East
        case 3:
			if (is_x) {
				return coord+1;
			}
			else {
				return coord;
			}
        // Invalid direction
        default:
            return -1;
    }
}



// visit visits a section of the maze
static void visit(mazeGraph *maze, int x, int y, int avatar)
{
	if (avatar > maze->nAvatars-1 || avatar < 0) {
		return;
	}

	int *visited = get_visited(maze, x, y);
	if (visited == NULL) {
		return;
	}

	maze->avInfo[avatar][0] = x;
    maze->avInfo[avatar][1] = y;
	maze->walls[2*x][2*y] = avatar;

	int i;
	for (i = 0; visited[i] != -1 || i > maze->nAvatars; i++) {
		if (visited[i] == avatar) {
			return;
		}
	}
	if (i == maze -> nAvatars) {
		return;
	}
	visited[i] = avatar;
}



// find_deadends crossreferences the maze with walls found
static void find_deadends(mazeGraph *maze)
{
	int cx, cy;
	bool noPath;
	// We define a coordinate as a "dead end" when a link is present
	// But there is no wall separating it
	for (int i = 0; i < maze->width; i++) {
		for (int j = 0; j < maze->height; j++) {
			mazeNode *node = get_node(maze, i, j);
			cx = 2*i;
			cy = 2*j;
			noPath = (!hasWest(node) && !hasNorth(node)
					&& !hasSouth(node) && !hasEast(node));

			// West
			if (i != 0 && noPath && maze->walls[cx-1][cy] == 16) {
				maze->walls[cx][cy] = 11;
			}
			// North
			if (j != 0 && noPath && maze->walls[cx][cy-1] == 16) {
				maze->walls[cx][cy] = 11;
			}
			// South
			if (j != maze->height-1 && noPath && maze->walls[cx][cy+1] == 16) {
				maze->walls[cx][cy] = 11;
			}
			// East
			if (i != maze->width-1 && noPath && maze->walls[cx+1][cy] == 16) {
				maze->walls[cx][cy] = 11;
			}
		}
	}
}



//remove_visited
static void remove_visited(mazeGraph *maze, int x, int y, int avatar) {
	int *visited = get_visited(maze, x, y);
	if (visited == NULL) {
		return;
	}

	int aInd = -1;
	int i;
	// Move everything down one
	for (i = 0; i < maze->nAvatars && visited[i] != -1; i++) {
		if (aInd == -1) {
			if (visited[i] == avatar) {
				aInd = i;
			}
		}
		else {
			visited[i-1] = visited[i];
		}
	}
	if(i != maze->nAvatars){
		visited[i] = -1;
	}
}


// num_on_coord
// returns the number of avatars on the coordinate
static int num_on_coord(mazeGraph* maze, int x, int y) {
	int ctr = 0;
	for (int i = 0; i < maze->nAvatars; i++) {
		if (maze->avInfo[i][0] == x && maze->avInfo[i][1] == y) {
			ctr++;
		}
	}
	return ctr;
}

/****************************************************/




/****************** Global Functions *****************/
// graph_init
mazeGraph *graph_init(int mazeWidth, int mazeHeight, int nAvatars)
{
	// Initialize the graph. If null return null
	mazeGraph *maze = malloc(sizeof(mazeGraph));
	if(maze == NULL) {
		return NULL;
	}
	// Initialize the array. If null return null
	maze->graph = calloc(mazeWidth, sizeof(mazeNode**));
	if (maze->graph == NULL) {
		free(maze);
		return NULL;
	}

	// Make a walls matrix that holds all spaces and internal walls
	maze->walls = calloc(2*mazeWidth-1,sizeof(int*));
	if (maze->walls == NULL) {
		graph_delete(maze);
		return NULL;
	}
	// Instantiate each of the columns, as well as initial conditions
	for (int i = 0; i < 2*mazeWidth-1; i++) {
		maze->walls[i] = malloc((2*mazeHeight-1)*sizeof(int));
		
		if (maze->walls[i] == NULL) {
			graph_delete(maze);
			return NULL;
		}
		// Set to unknown or junction
		for (int j = 0; j < 2*mazeHeight-1; j++) {
			// Unknown wall
			if (i%2 == 1 || j%2 == 1){
				maze->walls[i][j] = 17;
			}
			// Unknown space
			else{
				maze->walls[i][j] = 10;
			}
			// Junction
			if (i%2 == 1 && j%2 == 1) {
				maze->walls[i][j] = 15;
			}
		}
	}

	maze->avInfo = malloc(nAvatars*sizeof(int*));
	if (maze->avInfo == NULL) {
		graph_delete(maze);
		return NULL;
	}
	for (int i = 0; i < nAvatars; i++) {
		maze->avInfo[i] = malloc((nAvatars+2)*sizeof(int));
		if (maze->avInfo[i] == NULL) {
			graph_delete(maze);
			return NULL;
		}
		for (int j = 0; j < nAvatars+2; j++) {
			maze->avInfo[i][j] = -1;
		}
	}


	maze->width = mazeWidth;
	maze->height = mazeHeight;
	maze->nAvatars = nAvatars;
	maze->explorer = true;

	// Initialize each node on each column
	for(int i = 0; i < mazeWidth; i++){
		mazeNode **currCol = calloc(mazeHeight, sizeof(mazeNode*));
		if(currCol == NULL){
			// free graph in progress
			graph_delete(maze);
			return NULL;
		}
		for(int j = 0; j < mazeHeight; j++){
			currCol[j] = node_init(i, j, nAvatars);
			if(currCol[j] == NULL){
				// free graph in progress
				graph_delete(maze);
				return NULL;
			}
		}
		maze->graph[i] = currCol;
	}
 
	// set all node pointers to adjacent nodes
	for(int i = 0; i < mazeWidth; i++) {
		for(int j = 0; j < mazeHeight; j++) {

			// Set the north direction to the node previous in the column
			if(j != 0) {
				maze->graph[i][j]->north = maze->graph[i][j-1];
			}

			// Set the west direction to the node of the previous column
			if(i != 0) {
				maze->graph[i][j]->west = maze->graph[i-1][j];
			}

			// Set the east direction to the node of the next column
			if(i != (mazeWidth-1)){
				maze->graph[i][j]->east = maze->graph[i+1][j];
			}

			// Set the south direction to the node next in the column
			if(j != (mazeHeight-1)){
				maze->graph[i][j]->south = maze->graph[i][j+1];
			}
		}
	}
	return maze;
}



// graph_delete
void graph_delete(mazeGraph *maze)
{
	// Iterate through the coordinates
	for (int i = 0; i < maze->width; i++) {
		mazeNode **curCol = maze->graph[i];
		if (curCol != NULL) {
			for (int j = 0; j < maze->height; j++) {
				// Delete the current node if it is not null
				if (curCol[j] != NULL) {
					node_delete(curCol[j]);
				}
			}
			free(curCol);
		}
	}
	for (int i = 0; i < 2*(maze->width)-1; i++) {
		int *wallsCol = maze->walls[i];
		if (wallsCol != NULL) {
			free(wallsCol);
		}
	}
	for (int i = 0; i < maze->nAvatars; i++) {
		if (maze->avInfo[i] != NULL) {
			free(maze->avInfo[i]);
		}
	}

	free(maze->avInfo);
	free(maze->graph);
	free(maze->walls);
	free(maze);
}



// possible_moves
int *possible_moves(mazeGraph *maze, int x, int y)
{
	if (maze == NULL) {
		return NULL;
	}
	int *moves = malloc(4*sizeof(int));
	for (int i = 0; i < 4; i++) {
		moves[i] = 0;
	}
	if (moves == NULL) {
		return NULL;
	}
	mazeNode *node = get_node(maze, x, y);
	if (node == NULL) {
		free(moves);
		return NULL;
	}

	if (hasWest(node)) {
		moves[0] = 1;
	}
	if (hasNorth(node)) {
		moves[1] = 1;
	}
	if (hasSouth(node)) {
		moves[2] = 1;
	}
	if (hasEast(node)) {
		moves[3] = 1;
	}

	return moves;
}



// remove_path
void remove_path(mazeGraph *maze, int x, int y, int direction)
{
	if (maze == NULL) {
		return;
	}
	mazeNode *currNode = get_node(maze, x, y);
	mazeNode *otherNode;
	
	switch(direction) {
		// West
		case 0 :
			otherNode = get_node(maze, x-1, y);
			if (otherNode == NULL) {
				return;
			}
			currNode->west = NULL;
			otherNode->east = NULL;
			break;
		// North
		case 1:
			otherNode = get_node(maze, x, y-1);
            if (otherNode == NULL) {
                return;
            }
			currNode->north = NULL;
			otherNode->south = NULL;
			break;
		// South
        case 2:
            otherNode = get_node(maze, x, y+1);
            if (otherNode == NULL) {
				return;
            }
			currNode->south = NULL;
			otherNode->north = NULL;
			break;
		// East
        case 3:
            otherNode = get_node(maze, x+1, y);
			if (otherNode == NULL) {
                return;
            }
			currNode->east = NULL;
			otherNode->west = NULL;
            break;
		// Invalid direction
		default:
			return;
	}
}



// init_avatar
void init_avatar(mazeGraph *maze, int x, int y, int avatar)
{
	visit(maze, x, y, avatar);
}



// move_success
bool move_success(mazeGraph *maze, int x, int y, int direction, int avatar)
{
	// Invalid Cases
    if (maze == NULL) {
        return false;
    }
    if (x < 0 || x > maze->width || y < 0 || y > maze->height) {
        return false;
    }
	
	// Convert coords into extended-coords
	int cx = 2*x, cy = 2*y;
	// Get the wall coords
	int wcx = next_coord(cx, true, direction),
		wcy = next_coord(cy, false, direction);
	
	int nx = next_coord(x, true, direction),
		ny = next_coord(y, false, direction);
	
	if (wcx < 0 || wcy < 0 || wcx > 2*(maze->width)-2 || wcy > 2*(maze->height)-2) {
        return false;
    }

	// Mark the coordinates as a valid path
    maze->walls[cx][cy] = 10;
	maze->walls[wcx][wcy] = 16;

	// Compute which avatars have met
	// Find the union of both visited tags
	int *currvisited = get_visited(maze, x, y);
	int *nextvisited = get_visited(maze, nx, ny);
	int uninvisited[maze->nAvatars];
	for (int i = 0; i < maze->nAvatars; i++) {
		uninvisited[i] = 0;
	}
	int i, j;
	bool b1 = false, b2 = false;
	for (i = 0; i < maze->nAvatars; i++) {
		if (currvisited[i] != -1) {
			uninvisited[currvisited[i]] = 1;
		}
		else {
			b1 = true;
		}
		if (nextvisited[i] != -1) {
			uninvisited[nextvisited[i]] = 1;
		}
		else{
			b2 = true;
		}
		if (b1 && b2) {
			break;
		}
	}
	j = 0;
	for (i = 0; i < maze->nAvatars; i++) {
		if (uninvisited[i] == 1) {
			uninvisited[j] = i;
			j++;
		}
	}
	uninvisited[j] = -1;

	// Add each of the union to the connected array 
	// (i.e. the "direct connections" between avatars)
	for (i = 0; i < maze->nAvatars && uninvisited[i] != -1; i++) {
		for (j = 0; j < maze->nAvatars && uninvisited[j] != -1; j++) {
			maze->avInfo[uninvisited[i]][uninvisited[j]+2] = 1;
		}
	}

	// After the "direct connections" are updated
	// Find how many avatars are "indirectly connected"
	int ctr = 0; 				// A counter
	int secunin[maze->nAvatars];// Stores indirect connections
	int queue[maze->nAvatars];	// A queue of avatars through which to iterate
	
	// "Zero out" queue and union
	for (i = 0; i < maze->nAvatars; i++) {
		secunin[i] = 0;
		queue[i] = -1;
	}
	
	queue[0] = 0; 				// Begin at avatar 0
	j = 1;						// Next queue position
	int *tmp;
	
	// Begin at "direct connections" (d.c.) to avatar 0
	// For each d.c. from avatar 0, 
	// 		Put into queue, add to the indirect connections, and increment counter
	while(ctr < maze->nAvatars && queue[0] != -1) {
		int curr = queue[0];
		for (int i = 0; i < j; i++) {
			queue[i] = queue[i+1];
		}
		j--;
		queue[j] = -1;
		tmp = maze->avInfo[curr];
		for (i = 0; i < maze->nAvatars; i++) {
			if (tmp[i+2] == 1 && curr != i && secunin[i] == 0) {
				secunin[i] = 1;
				ctr++;
				queue[j] = i;
				j++;
			}
		}
	}
	//printf("%d,%d,%d,%d,%d\n%d,%d\n", maze->avInfo[0][0], maze->avInfo[0][1],maze->avInfo[0][2],maze->avInfo[0][3],maze->avInfo[0][4],x,y);
	//printf("%d                                      ",ctr2);
	//fflush(stdout);

	// If all avatars have just been indirectly linked,
	// 		Delete unknown walls, and direct coordinate paths inwards to meet point
	if (ctr == maze->nAvatars && maze->explorer == true) {
		//coord_print(maze); printf("\n");
		printf("EXPLORER PHASE COMPLETE: %d AVATARS HAVE A PATH TO THE OTHERS",ctr);
		maze->explorer = false;
		for (i = 0; i < maze->width; i++) {
			for (j = 0; j < maze->height; j++) {

				if (i != maze->width-1 && maze->walls[2*i+1][2*j] == 17) {
					remove_path(maze, i, j, 3);
				}
				if (j != maze->height-1 && maze->walls[2*i][2*j+1] == 17) {
					remove_path(maze, i, j, 2);
				}
			}
		}
		ctr = 0;
		int numAvatars;
		int seen[maze->nAvatars];
		mazeNode *node, *nnode;
		int k;
		int l = 0;
		bool isseen;
		int av;
		coord_print(maze); printf("\n");
		bool changed = true;
		int paths[maze->nAvatars][2];

		// Delete paths until no paths have been updated from the previous iteration
		// Deletes paths from dead-ends to the first avatar found on the path
		while(changed) {
			// Find the dead-end coordinates from which to iterate
			changed = false;
			//coord_print(maze);
			//sleep(2);
			//printf("--------------------------------------------------\n");

			for (i = 0; i < maze->width; i++) {
				for (j = 0; j < maze->height; j++) {
					node = get_node(maze, i, j);
					if (hasNorth(node) && !hasEast(node) && !hasSouth(node) && !hasWest(node)) {
						nnode = get_node(maze, next_coord(i, true, 1), next_coord(j, false, 1));
						if (hasSouth(nnode)) {
							// Begin iterating north and making the graph directed
							numAvatars = num_on_coord(maze, i, j);
							// If there are avatars on the coordinate, check if they have been added already
							// If this is a new coordinate, add the first avatar on the coordinate and increment counters
							if (numAvatars > 0) {
								isseen = false;
								av = -1;
								// Get the first avatar at the coordinate
								for (k = 0; k < maze->nAvatars; k++) {
									if (maze->avInfo[k][0] == i && maze->avInfo[k][1] == j) {
										av = k;
										break;
									}
								}
								// Check if the avatar has been added
								for (k = 0; k < ctr; k++) {
									if (seen[k] == av) {
										isseen = true;
										break;
									}
								}
								// If the coordinate hasn't been added, add it
								if (!isseen) {
									ctr += numAvatars;
									seen[l] = av;
									l++;
								}
							}
							// If there are no avatars on the coordinate, delete the path to the coordinate
							else {
								changed = true;
								node->north = NULL;
								nnode->south = NULL;
							}
						}
					}
					if (hasEast(node) && !hasNorth(node) && !hasSouth(node) && !hasWest(node)) {
						nnode = get_node(maze, next_coord(i, true, 3), next_coord(j, false, 3));
						if (hasWest(nnode)) {
							// Begin iterating east and making the graph directed
							numAvatars = num_on_coord(maze, i, j);
							// If there are avatars on the coordinate, check if they have been added already
							// If this is a new coordinate, add the first avatar on the coordinate and increment counters
							if (numAvatars > 0) {
								isseen = false;
								av = -1;
								// Get the first avatar at the coordinate
								for (k = 0; k < maze->nAvatars; k++) {
									if (maze->avInfo[k][0] == i && maze->avInfo[k][1] == j) {
										av = k;
										break;
									}
								}
								// Check if the avatar has been added
								for (k = 0; k < ctr; k++) {
									if (seen[k] == av) {
										isseen = true;
										break;
									}
								}
                                    // If the coordinate hasn't been added, add it
                                    if (!isseen) {
                                        ctr += numAvatars;
                                        seen[l] = av;
                                        l++;
                                    }
                                }
                                // If there are no avatars on the coordinate, delete the path to the coordinate
                                else {
									changed = true;
									node->east = NULL;
                                    nnode->west = NULL;
                                }
                            }
						}
					if (hasSouth(node) && !hasNorth(node) && !hasEast(node) && !hasWest(node)) {
						nnode = get_node(maze, next_coord(i, true, 2), next_coord(j, false, 2));
						if(hasNorth(nnode)) {
							// Begin iterating south and making the graph directed
                                numAvatars = num_on_coord(maze, i, j);
                                // If there are avatars on the coordinate, check if they have been added already
                                // If this is a new coordinate, add the first avatar on the coordinate and increment counters
                                if (numAvatars > 0) {
                                    isseen = false;
                                    av = -1;
                                    // Get the first avatar at the coordinate
                                    for (k = 0; k < maze->nAvatars; k++) {
                                        if (maze->avInfo[k][0] == i && maze->avInfo[k][1] == j) {
                                            av = k;
                                            break;
                                        }
                                    }
                                    // Check if the avatar has been added
                                    for (k = 0; k < ctr; k++) {
                                        if (seen[k] == av) {
                                            isseen = true;
                                            break;
                                        }
                                    }
                                    // If the coordinate hasn't been added, add it
                                    if (!isseen) {
                                        ctr += numAvatars;
                                        seen[l] = av;
                                        l++;
                                    }
                                }
                                // If there are no avatars on the coordinate, delete the path to the coordinate
                                else {
									changed = true;
									node->south = NULL;
                                    nnode->north = NULL;
                                }
						}
					}
					if (hasWest(node) && !hasNorth(node) && !hasEast(node) && !hasSouth(node)) {
						nnode = get_node(maze, next_coord(i, true, 0), next_coord(j, false, 0));
						if (hasEast(nnode)) {
							// Begin iterating west and making the graph directed
                                numAvatars = num_on_coord(maze, i, j);
                                // If there are avatars on the coordinate, check if they have been added already
                                // If this is a new coordinate, add the first avatar on the coordinate and increment counters
                                if (numAvatars > 0) {
                                    isseen = false;
                                    av = -1;
                                    // Get the first avatar at the coordinate
                                    for (k = 0; k < maze->nAvatars; k++) {
                                        if (maze->avInfo[k][0] == i && maze->avInfo[k][1] == j) {
                                            av = k;
                                            break;
                                        }
                                    }
                                    // Check if the avatar has been added
                                    for (k = 0; k < ctr; k++) {
                                        if (seen[k] == av) {
                                            isseen = true;
                                            break;
                                        }
                                    }
                                    // If the coordinate hasn't been added, add it
                                    if (!isseen) {
                                        ctr += numAvatars;
                                        seen[l] = av;
                                        l++;
                                    }
                                }
                                // If there are no avatars on the coordinate, delete the path to the coordinate
                                else {
									changed = true;
                                    node->west = NULL;
									nnode->east = NULL;
                                }
						}
					}
				}
			}
		}
		changed = true;
		//coord_print(maze);
		//printf("--------------------------------------------------\n");
		
		// Iteratively direct the maze inward
		// Eventually will find a meet point from the avatar coordinates
		// This meet point will be the shortest meet point possible
		while (changed) {
			changed = false;
			l = 0;
			for (i = 0; i < maze->width; i++) {
				for (j = 0; j < maze->height; j++) {
					isseen = false;
					for (k = 0; k < l; k++) {
						if (paths[k][0] == i && paths[k][1] == j) {
							isseen = true;
						}
					}
					if (!isseen) {
						node = get_node(maze, i, j);
						if (hasNorth(node) && !hasEast(node) && !hasSouth(node) && !hasWest(node)) {
                        	nnode = get_node(maze, next_coord(i, true, 1), next_coord(j, false, 1));
                        	if (hasSouth(nnode)) {
								changed = true;
								nnode->south = NULL;
								paths[l][0] = nnode->x;
                            	paths[l][1] = nnode->y;
                            	l++;
							}
						}
						else if (hasEast(node) && !hasNorth(node) && !hasSouth(node) && !hasWest(node)) {
                        	nnode = get_node(maze, next_coord(i, true, 3), next_coord(j, false, 3));
                        	if (hasWest(nnode)) {
								changed = true;
								nnode->west = NULL;
								paths[l][0] = nnode->x;
                            	paths[l][1] = nnode->y;
                            	l++;
							}
						}
						else if (hasSouth(node) && !hasNorth(node) && !hasEast(node) && !hasWest(node)) {
                        	nnode = get_node(maze, next_coord(i, true, 2), next_coord(j, false, 2));
                        	if(hasNorth(nnode)) {
								changed = true;
								nnode->north = NULL;
								paths[l][0] = nnode->x;
                            	paths[l][1] = nnode->y;
                            	l++;
							}
						}
						else if (hasWest(node) && !hasNorth(node) && !hasEast(node) && !hasSouth(node)) {
                        	nnode = get_node(maze, next_coord(i, true, 0), next_coord(j, false, 0));
                       		if (hasEast(nnode)) {
								changed = true;
								nnode->east = NULL;
								paths[l][0] = nnode->x;
                            	paths[l][1] = nnode->y;
                            	l++;
							}
						}
					}
				}
			}
			//coord_print(maze);
			//printf("--------------------------------------------------\n");
			//sleep(1);
		}
	}
	// Visit the maze
	visit(maze, nx, ny, avatar);
	if (ctr == maze->nAvatars) {
		return true;
	}
	return false;
}



// move_fail
void move_fail(mazeGraph *maze, int x, int y, int direction)
{
	if (maze == NULL) {
		return;
	}
	if (x < 0 || x > maze->width || y < 0 || y > maze->height) {
		return;
	}

	// Convert coords into extended-coords
	int cx = 2*x, cy = 2*y;
	// Get the wall coords
	int wcx = next_coord(cx, true, direction),
		wcy = next_coord(cy, false, direction);

	if (wcx < 0 || wcy < 0 || wcx > 2*(maze->width)-2 || wcy > 2*(maze->height)-2) {
		return;
	}

	if (wcy%2 == 0) {
		maze->walls[wcx][wcy] = 14;
	}
	else {
		maze->walls[wcx][wcy] = 13;
	}
	remove_path(maze, x, y, direction);
}



// has_visited
bool has_visited(mazeGraph *maze, int x, int y, int avatar)
{
	if (is_visited(maze, x, y)) {
		int *visited = get_visited(maze, x, y);
		for (int i = 0; visited[i] != -1 || i > maze->nAvatars; i++) {
			if (visited[i] == avatar) {
				return true;
			}
		}
	}
	return false;
}



// is_visited
bool is_visited(mazeGraph *maze, int x, int y)
{
	int *visited = get_visited(maze, x, y);
	if (visited == NULL) {
		return false;
	}
	return (visited[0]) != -1;
}



// maze_print
void maze_print(mazeGraph *maze)
{
	if (maze == NULL) {
		printf("(null)");
	}
	// Print the maze walls and avatars, as well as dead ends
	else {
		//coord_print(maze);
		int j;
		
		//Update the maze to get sectioned-off areas
		find_deadends(maze);

		// Print the maze
		for (j = 0; j < (maze->height); j++) {
			int cy = 2*j-1;
            int value;

			// Top walls
			if (j == 0) {
				printf("+");
				for (int cx = 0; cx < 2*(maze->width)-1; cx++) {
					printf("—");
				}
				printf("+\n");
			}

			// Middle spaces
			cy++;
			printf("|");
			for (int cx = 0; cx < 2*(maze->width)-1; cx++) {
                value = maze->walls[cx][cy];
                maze_print_helper(value, cx, cy);
			}
			printf("|\n");

			// Bottom walls
			if (j == maze->height-1) {
				printf("+");
				for (int cx = 0; cx < 2*(maze->width)-1; cx++) {
					printf("—");
				}
				printf("+\n");
			}
			// Bottom walls v2
            else {
				cy++;
				printf("|");
				for (int cx = 0; cx < 2*(maze->width)-1; cx++) {
					int value = maze->walls[cx][cy];
					maze_print_helper(value, cx, cy);
                }
				printf("|\n");
			}	
		}
	}
}



//dead_end_coord
void dead_end_coord(mazeGraph *maze, int x, int y, int direction, int avatar)
{
	if (maze->explorer) {
		int currx = next_coord(x, true, direction);
		int curry = next_coord(y, false, direction);
		if (has_visited(maze, currx, curry, avatar)) {
			int *visited = get_visited(maze, x, y);
			// If it is visited by another avatar
			//if (visited[1] != -1) {
				bool remove = false;
				for (int i = 0; i < maze->nAvatars && visited[i] != -1; i++) {
					int av = visited[i];
					// If another avatar is on x,y, remove the visited marker
					if (av != avatar && maze->avInfo[av][0] == x && maze->avInfo[av][1] == y) {
						remove_visited(maze, x, y, avatar);
						//printf("REMOVING VISITED %d,%d,%d,%d for %d\n",x,y,direction,avatar, av);
						remove = true;
						break;
					}
				}
				if (!remove) {
					remove_path(maze, x, y, direction);
				}
			//}
			// Remove the dead end path
			//else {
			//	remove_path(maze, x, y, direction);
			//}
		}
	}
}



// maze_get_walls
int **maze_get_walls(mazeGraph *maze)
{
    return maze->walls;
}



// maze_get_height
int maze_get_height(mazeGraph *maze)
{
    return maze->height;
}



// maze_get_width
int maze_get_width(mazeGraph *maze)
{
    return maze->width;
}


/****************************************************/

void coord_print(mazeGraph *maze)
{
	if (maze == NULL) {
		printf("(null)");
	}
	else {
		for (int j = 0; j < maze->height; j++) {
			// Print northern arrow
			for (int i = 0; i < maze->width; i++) {
				mazeNode *node = maze->graph[i][j];
				if (node == NULL) {
					printf("  (null) ");
				}
				else {
					if (hasNorth(node)) {
						printf("    ↑    ");
					}
					else {
						printf("         ");
					}
				}
			}
			printf("\n");

			// Print node and east/west arrows
			for (int i = 0; i < maze->width; i++) {
                mazeNode *node = maze->graph[i][j];
				if (node == NULL) {
					printf("  (null) ");
				}
				else {
                	if (hasWest(node)) {
                    	if (hasEast(node)) {
							printf("←(%2d,%2d)→", node->x, node->y);
	                	}
						else {
							printf("←(%2d,%2d) ", node->x, node->y);
						}
					}
					else if (hasEast(node)) {
						printf(" (%2d,%2d)→", node->x, node->y);
					}
					else {
						printf(" (%2d,%2d) ", node->x, node->y);
					}
				}
			}
			printf("\n");

			for (int i = 0; i < maze->width; i++) {
                mazeNode *node = maze->graph[i][j];
				if (node == NULL) {
					printf("  (null) ");
				}
				else {
                	if (hasSouth(node)) {
                		printf("    ↓    ");
					}
					else {
						printf("         ");
					}
            	}
			}
			printf("\n");
		}
	}
}


#ifdef TEST

int main (int argc, char** argv)
{
	mazeGraph *maze = graph_init(10, 10, 3);
	remove_path(maze, 2,2,3);
	remove_path(maze, 2,2,1);
	remove_path(maze, 2,2,0);
	remove_path(maze, 2,2,2);
	remove_path(maze, 0,0,3);
	remove_path(maze, 1,1,1);
	remove_path(maze, 0,0,0);
	remove_path(maze, 0,0,2);
	printf("(0,0) is visited: %d\n", is_visited(maze, 0,0));
	printf("Making (0,0) visited by avatar 0\n");
	visit(maze, 0,0,0);
	printf("(0,0) is visited: %d\n", is_visited(maze, 0,0));
	printf("Making (0,0) visited by avatar 1\n");
	visit(maze, 0,0,1);
	printf("(0,0) is visited: %d\n", is_visited(maze, 0,0));
	printf("(0,0) is visited by 0: %d\n", has_visited(maze, 0, 0, 0));
	printf("(0,0) is visited by 1: %d\n", has_visited(maze, 0, 0, 1));
	printf("(0,0) is visited by 2: %d\n", has_visited(maze, 0, 0, 2));

	coord_print(maze);
	//system("clear");
	maze_print(maze);
    printf("%d", maze->walls[4][10]);
	graph_delete(maze);
	printf("Deleted\n");

	maze = graph_init(5,5,1);
	init_avatar(maze, 3,3,0);
	maze_print(maze);
	move_fail(maze, 3,3,3);
	maze_print(maze);
	move_fail(maze, 3,3,1);
	maze_print(maze);
	move_fail(maze, 3,3,2);
	maze_print(maze);
	move_success(maze, 3,3,0,0);
	maze_print(maze);
	move_fail(maze, 2,3,2);
	maze_print(maze);
	move_fail(maze, 2,3,0);
	maze_print(maze);
	move_success(maze,2,3,1,0);
	maze_print(maze);
	move_fail(maze, 2,2,3);
	maze_print(maze);
	move_fail(maze, 2,2,1);
	for (int i = 0; i < 2*maze->height-1; i++) { 
		for (int j = 0; j < 2*maze->width-1; j++) {
			printf("%d, ",maze->walls[j][i]);
		}
		printf("\n");
	}
	maze_print(maze);
	move_success(maze, 2,2,0,0);
	maze_print(maze);
	move_fail(maze, 1,2,2);
	maze_print(maze);
	move_fail(maze, 1,2,0);
	maze_print(maze);
	move_success(maze, 1,2,1,0);
	maze_print(maze);
	move_fail(maze, 1,1,1);
	maze_print(maze);
	move_fail(maze, 1,1,0);
	maze_print(maze);
	move_fail(maze, 1,1,3);
	maze_print(maze);
	coord_print(maze);
    for (int i = 0; i < 2*maze->height-1; i++) {
        for (int j = 0; j < 2*maze->width-1; j++) {
            printf("%d, ",maze->walls[j][i]);
        }
        printf("\n");
    }
	
	// To test proper dead-end creation:
	remove_path(maze, 2,3,3);
	maze_print(maze);
	remove_path(maze, 2,2,2);
	graph_delete(maze);
}
#endif
