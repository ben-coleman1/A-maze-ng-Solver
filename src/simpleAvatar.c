/* simpleAvatar.c is a file used to run the algorithm  
 * used to test parts of the 
 * 
 * Pseudo code: 
 * 1. Initializing: the avatar on the maze
 * 2. decisionS: Moving to the 3(East) or the next best direction 
 * 3. Move in  AM atrtup 
 * 4. update the pos
 *     - whether the move was sucessful or not
 *     - updates walls 
 *     - mark the node as visited
 *     - update 
 * 5. Check if with another avatar (grouping)
 * 
 * 
 *  CS50 W18 Group 5 Final Progect; Ben, Billy, Lessley, and Oliver
 */ 
#include "amazing.h"
#include "mazeGraph.h"
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

//initial move of the avatar. 
void firstAvatarMove(mazeGraph* mGraph, XYPos pos, int avatar)
{
    init_avatar(mGraph, pos.x, pos.y, avatar);
}

/*
 * 
 */ 
int decsionMaker(mazeGraph* mGraph, XYPos pos, int prevDirection, int avatar, int turnNumber)
{
    //if its the first turn for the avatars 
    if(turnNumber == 0){
        firstAvatarMove(mGraph, pos, avatar);
        return 3; 
    }

    // //check for walls
    int* moves = possible_moves(mGraph, pos.x, pos.y);
    if(moves == NULL){
        printf("Moves is NULL \n");
        return -1;
    }
    int east[4] = {2,3,1,0}; // South, East, North, West
    int south[4] = {0,2,3,1}; //west, south, east, north 
    int west[4] = {1,0,2,3}; // North, west, south, 
    int north[4] = {3,1,0,2};//
    int *direction;
 
    //maze_print(mGraph);
    switch(prevDirection){
        case 0:
            direction = west;
            break;
        case 1:
            direction = north;
            break;
        case 2:
            direction = south;
            break;
        case 3:
            direction = east;
            break;
        default:
            fprintf(stderr, " invalid move \n");
		exit(133);
            return -1;
    }
	/*
	// NEW UNTESTED SECTION
	// If it is visited by another avatar, then do left hand rule (or other)
	if (is_visited(mGraph, pos.x, pos.y)) {
		for (int i = 0; i < 4; i++) {
			if (moves[direction[3-i]] == 1) {
				free(moves);
				return direction[3-i];
			}
		}
	}
	// END OF NEW UNTESTED SECTION. delete "else" below
	*/
	//else {
    	for(int i =0; i<4; i++){
        	if(moves[direction[i]] == 1){
            	free(moves);
				// printf("Returned smart move in direction %d PREV DIR:%d \n", direction[i], prevDirection);
            	return direction[i];
        	}
    	}
	//}
	free(moves);
	//printf("BAD MOVE:ERROR");
	fflush(stdout);
	return M_NULL_MOVE;
}
//
bool comparCoord(XYPos pos1, XYPos pos2)
{
     int x1 = pos1.x;
     int y1 = pos1.y;
     int x2 = pos2.x;
     int y2 = pos2.y;
     if(x1 == x2 && y1 == y2){
         return true;
     }else{      
         return false; 
    }
 }

 // NOTES: return true if successful move, false if wall. Used to determine last successful direction
 bool updateAvatarGraph(mazeGraph* mGraph, XYPos pos, XYPos prevPos, int avatar, int direction)
 {
    if(mGraph == NULL){
        fprintf(stderr, "Maze Graph is  == to NULL.\n");
        return NULL; 
    }
    //compares coordinates
    if(comparCoord(prevPos, pos)){
        //if prev == pos then the move failed
        move_fail(mGraph,prevPos.x, prevPos.y, direction);
        return false;
    } else {
        //checks in dead_end coord
        dead_end_coord(mGraph, prevPos.x, prevPos.y, direction, avatar);
        //if prev != pos then move was sucessful 
        
		move_success(mGraph, prevPos.x, prevPos.y, direction, avatar);
        return true;
    }
}
