# ChessOnline - the right way

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
- ./client "ip of server"
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
- Than make moves - But some chess rules do not apply here
  - castling makes no sense and is completely random.  Why is it in the game?
  - Drawing.  We go for either win or loss no draws
  - En passant.  While this makes more sense than castling it still makes very little sense.
  - Resigning.  Similar to drawing we do not accept cowards.  
- Other than that it is played like a normal game and once the game ends you will be brough back to the queue screen if you would like to queue up again
