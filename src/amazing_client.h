/* simpleAvatar.c is a file used to run the algorithm  
 * used to test parts of the 
 * 
 *  CS50 W18 Group 5 Final Progect; Ben, Billy, Lessley, and Oliver
 */ 
#include "mazeGraph.h"
#include "simpleAvatar.h"
#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>        	
#include <ctype.h>
#include "amazing.h"
#include <unistd.h>    			
#include <sys/types.h>
#include <pwd.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>

#include <netdb.h>          // socket-related structures
#include <arpa/inet.h>      // socket-related calls
#include <sys/select.h>     // select-related stuff 
#include <errno.h>          // error handling
#include <signal.h>
/*initAvatar
 *@ptr: pointer in the init stucture
 * This function initializes the avatar for AMStartup
 * returns nothing 
 */
void *initAvatar(void *ptr);

