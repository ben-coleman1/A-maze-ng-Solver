/* simpleAvatar.h is the header file for simpleAvatar.c 
 * used to test parts of the 
 * 
 *  CS50 W18 Group 5 Final Progect; Ben, Billy, Lessley, and Oliver
 */ 

#include "amazing.h"
#include "mazeGraph.h"
#include <stdlib.h>
#include <stdbool.h>

#ifndef __SIMPLEAVATAR_H
#define __SIMPLEAVATAR_H

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

#endif //__SIMPLEAVATAR_H
