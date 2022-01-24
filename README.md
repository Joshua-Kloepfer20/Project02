# ChessOnline

## Members: Joshua Kloepfer - pd 5

## Description of Project:
- My project allows for people to play queue up for chess games with strangers
## Libraries
- SDL2
- SDL2_ttf
- sudo apt-get install libsdl2-dev
- sudo apt-get install libsdl2-ttf-dev
## how to use
- make
- ./server
- ./client
## intructions
- server there is nothing to do other than run
- client will open with 2 blank white boxes
- first box is username
- second is password
- you can use tab to switch between or click on them
- enter tries to login
- click on register if you do not have account
- there will than be just a white box that says queue
- if clicked client will join the queue
- will print in terminal joined queue
- if there is another player in the queue you will be put into a game with them
- Than make moves - did not have time to complete some
  - king cannot move
  - no castle
  - no En Passant
  - no upgrading pawns
- When checkmate occurs both clients will go back to the queue screen and it will print in terminal the result of the ame
- If one client dc the other will return to queue screen
