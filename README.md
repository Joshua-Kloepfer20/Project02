# Project02

## Members: Joshua Kloepfer - pd 5

## problem I am solving:
- Chess is a really fun game.  However, having to meet up with people in person to play can be quite inconvenient.
- I would like to write something very similar to chess.com that gives people the opportunity to connect and play against other strangers or specific other users

## project will be used:
- I would like to write some graphics with a simple menu.  I want the client to be able to queue for a game, challenge a friend, and look at previous games. I would also like to implement a rating system so that people can be paired based on skill.  (if time) I would also like to implement a feature to play against an ai.

## technical design:
- I am considering either using sdl or graphics.h for creating the user interface.
### topics covered in class:
- Networking to have cleints connect to a server
  - The server will be responsible for managing remaining time for both players, storing the board, and relaying eachothers moves and board state to eachother.
  - It will also store users, rating, past games, all the moves in those games, and who is in queue.
  - Clients will display info sent by the server as well as check for the validity of moves
- Shared memory/semaphores
  - I will be using shared memory to store board info such as state and who's move it is
  - I will possibly be using semaphores so that only the subserver who's client's move it is can access the shared memory at a time and have the one not able to move block
  - I also might use shared memory to store the PID of every subserver and what user is connected to them
- reading/writing from a file
  -I will be using files to store info such as users, rating, and past games.
- These also may be signals
  - I am considering using signals to signal the other subserver in a game when it is their move or if the game is over and who has won to then send it to their client
### breaking down who is responsible for what:
- As I am the only member I will be responsible for everything
### data structures I will be using:
- I will most likely use a struct to store the baord and state.  The struct will hold a 2d integer array to represent the board with each index being a square and each number representing a piece.  The struct will also contain an int for who's turn it is.  It may also contain previous move and and int array with the PID of both subservers
### What algorithms and how will I use the data structures
- For the struct I will create one for every game and create a new segment of shared memory
- The subserver who's move it is will read the previous move, send it, access the board (possible semaphore for blocking), and await a response.  The other subserver will wait for the other user to move and repeat the process.  After a move is made it will also be checked if that move is checkmate or a draw.  If it is an ending move it will signal the other subserver that they won and that subserver will tell the client he lost.  I will also send time left to each client after each move and the client wil count down the time so as to not make the server consistently send it.  However, time will also be counted down on server which will be what is used.  If a player's time runs out they lose.

## timeline:
- I would like to have the ability for two clients to play against eachother by 1/17/2022
- I would like to implement queue and rating by 1/19/2022
- I would like to implement graphics by 1/21/2022
