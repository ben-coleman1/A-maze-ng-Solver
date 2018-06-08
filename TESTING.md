# CS50 w17 Team 5:  Benjamin Coleman, Billy Kwon, Oliver Levy, Lessley Hernandez
# "The Maze Challenge" - TESTING.md
&nbsp;

## Testing Plan for Maze Challenge 
&nbsp;
 - Unit testing
 We fully tested each of the components of the the maze graph data structure by thoroughly going over test and edge cases. 
 - Integration testing
 We then performed integration testing by testing the communication, messaging, datatstructure, and avatar modules in conjunction. We assembled the Maze Challenge modules together and tested it as a whole. We examined the output files and the data structure input/output flow carefully to ensure they act and communicate properly. 
 - Visual and Output Testing
 Another testing method was utilizing visual images and displays to ensure our avatars are acting properly and the maze is properly represented. 
 - Memory Testing 
 We utilized Valgrind for memory testing. 

### Unit Testing
&nbsp; 
 Before the data structure was ever integrated into the overall program, we tested our datastructure separately. Each function was called multiple times with a visual output to ensure that the functions were working properly. The unit tests call functions such as graph_init, remove_path, visit, graph_delete, move_fail, and maze_print in different scenarios with different inputs. The unit tests can be run by commenting out the `TESTING` in the Makefile in the datastruct directory.

### Integration Testing
&nbsp;
We tested parts of modules together and tested how information was passed between them. This was mostly done by checking output while the program was running. We mainly tested the conjunction between recieving and parsing messages, and updating and checking our datastructure. Much of this testing was done using gdb.

### Visual and Output Testing
&nbsp;
We reviewed output from solved mazes to check the validity of our datastructure and the overall functioning of the program. We tracked our avatars moves and what they should have been doing, and marked abnormal behavior. We verified that the maze data structure matched the image of the maze in the mazeport folder. All of our test runs are in the test log which display the avatars' positions, moves, and turns. 

### Memory Testing
&nbsp;
We tested for memory leaks using Valgrind. 

