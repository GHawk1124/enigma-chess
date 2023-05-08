Everytime we pass a fen string to the engine, a new instance of board is created, so all our variables are reset, in particular the king position
variables. Therefore, I changed the wKingPos and bKingPos variables to -1 in the board header file, and then put an if statement at the beginning
of genAllValidMoves to check if these variables have the value of -1. If they do, we iterate through the board and give these variables the appropriate
values. In this way, as the engine is looking ahead, these variables will be modified appropriately in makeMove (whenever a king is moved) and therefore
it won't iterate through the entire board all the time to establish the location of the kings.

THIS METHOD IS ONLY OPTIMAL IF OUR MINIMAX ALGORITHM USES makeMove() TO MAKE MOVES AND LOOK AHEAD. IF IT IS INSTEAD SIMPLY CREATING NEW INSTANCES OF BOARD
OR DECODING FEN STRINGS, THIS METHOD WON'T WORK
