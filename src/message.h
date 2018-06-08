/* Header file for message.c
 * TODO: PARSE AVATAR MOVE MSG
 *
 * CS50 W18 Group 5
 */ 
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
* socket_setup
* @hostname: the hostname provided from user
* @themp: a sock address struct
* @mazePort: the maze port number
* Returns the socket int
*/
int socket_setup(char *hostname, struct sockaddr_in *themp, int mazePort);
