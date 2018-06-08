/* message.c - program used to get received messages 
 * and parse through messages. 
 * 
 * CS50 W18 Group 5
 */ 
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>        	
#include <ctype.h>
#include "amazing.h"
#include "message.h"
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


/************** Local Functions ****************/
/* Initializes messages 
 * @initMessage: to prep and send the message
* @commSock: Sock. value we are com. to
 * returns an bool to verify if msg was sent
 * assume we are passing a malloc msg
 * if int > 0 it was sucessful
 * if int < 0 then it failed
 */ 

 bool initializeMsg(int commSock, int nAvatars,int difficulty)
{
    AM_Message initMessage; 
    memset(&initMessage, 0, sizeof(AM_Message));  // zero the message
   
    // Set the message to send 
    initMessage.type = htonl(AM_INIT);
    initMessage.init.nAvatars = htonl(nAvatars);
    initMessage.init.Difficulty = htonl(difficulty);

	if (send(commSock, &initMessage, sizeof(AM_Message), 0) == 0) {
        return false; 
	} 
    return true;
}
/*Avatar Ready Message
 *@commSock: Sock. value we are com. to
 * @AvatarId: to find which avatar we are on
 * returns true if message read was sucessful
 * if was not able to send msg returns false 
 */
 bool avatarReadyMsg(int commSock, int avatarId)
{

    AM_Message avatarMessage;
    memset(&avatarMessage, 0, sizeof(AM_Message));  // zero the message

    // Set the message to send 
    avatarMessage.type = htonl(AM_AVATAR_READY);
    avatarMessage.avatar_ready.AvatarId = htonl(avatarId);

    if(send(commSock, &avatarMessage, sizeof(AM_Message), 0) == 0) {
      return false;
    }

    return true;
}
/*Avatar move Message
 *@AvatarMsg: used to take in msg for info
 *@commSock: Sock. value we are com. to
 * returns true if message send was sucessful
 * if was not able to send msg returns false 
 */
 bool avatarMoveMsg(int commSock, int avatarId, int direction)
{
    AM_Message avatarMessage;
    memset(&avatarMessage, 0, sizeof(AM_Message));  // zero the message

    // Set the message to send 
    avatarMessage.type = htonl(AM_AVATAR_MOVE);
    avatarMessage.avatar_move.AvatarId = htonl(avatarId);
    avatarMessage.avatar_move.Direction = htonl(direction);

    if (send(commSock, &avatarMessage, sizeof(AM_Message), 0) == 0) {
      return false;
    } 

    return true;
}

/* receiveMsg 
 * @AM_Message: Message we are receiving from server
 * @commSock: Sock. value we are com. to
 * Takes in the msg and communicates with server
 * return true if sucessful 
 * return false if did not receive proper message
 */ 
bool receiveMsg(AM_Message *message, int commSock)
{ 
    if(recv(commSock, message, sizeof(AM_Message), 0) == 0) {
      return false;
    }

    return true;
}

/**************** socket_setup ****************/
/* Note: Taken from chatclient3.c with modifications
 * Take the program and hostname as arguments
 * Exit on any error of arguments or socket setup.
 */
int socket_setup(char *hostname, struct sockaddr_in *themp, int mazePort)
{
  // Look up the hostname specified on command line
  struct hostent *hostp = gethostbyname(hostname);
  if (hostp == NULL) {
    fprintf(stderr, "unknown host: '%s'\n", hostname);
    exit(3);
  }

  // Initialize fields of the server address
  themp->sin_family = AF_INET;
  bcopy(hostp->h_addr_list[0], &themp->sin_addr, hostp->h_length);
  themp->sin_port = htons(mazePort);

  // Create socket with SOCK_STREAM (TCP)
  int comm_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (comm_sock < 0) {
    perror("opening socket");
    exit(2);
  }
  return comm_sock;
}







