Project 2 - Scrabble Junior      
Beatriz Aguiar (up201906230) & Margariva Vieira (up201907907) - Turma 2

The game consists in the two programs: Scrabble Junior & Board Builder.

The Board Builder fulfills the project's requirements, creating a txt file with the proper board file format.
Besides the board size and the validation of the words, using the WORDS.TXT file, this program gives the user full freedom to build the perfect board. 
5x5 was the minimum size chosen. Following teacher Jorge Silva's advice on the existance of single and double letter words (it was said these shouldn't be  
considered), we decided 5 was a good starting point. 
Both "7 tiles per player" and "minimum 2 players" game's rules were also taken into account and, as a result, each board must have, at least, 14 tiles.
Boards with fewer tiles aren't valid game boards.
The created board is saved in the current directory if no directory is provided when asked and, if missing, ".txt" is added to the file's name.

The Scrabble Junior program follows the rules of the following link: "https://www.hasbro.com/common/instruct/Scrabble_Junior.pdf" with a few improvements:

 - We added 3 hints per player. A hint shows the position of a possible move and should be used (ctrl-Z) when the program outputs the sentence "You are obliged
to play" (rules are "if you can play, you MUST play") but the user can't find a possible move.

 - Another improvement was introducing a limit of possible swaps. If the player doesn't have any of the possible tiles to play and the pool isn't empty, rules 
say that two tiles must be swapped. We only allow 3 followed swaps as it might happen that the pool doesn't have any of the needed tiles and this would 
lead, if endless swaps were possible, to an endless ammount of swaps. In our game, if the player has no tiles and has unluckily swapped tiles 3 times, the 
game must go on to the next player's turn.

 - We also make sure to output a warning message, giving some context, everytime an out of the user's control move is done (e.g passing a player's turn due 
to his lack of tiles is followed by the message "Once the pool is empty and you can't play, your turn will be passed.").
-Considering the possibility of this program being used with an invalid board (less than 14 tiles) or the number of players overloading the number of
required tiles, the program will end (assert was used) noticing the used of this error.

The game ends when the board is completly full and the winner is the one with the most points (one point is earned for every word finished). 
No quitters allowed.

Classes, a structure, vectors, filestreams, randoms and containers, such as pairs, were used to try and broaden as much as possible the use of the lectioned subjects.
All in all, we believe to have achieved all the project's objectives.

Student's percentage of participation in the development of the work:
Beatriz Aguiar (up201906230) - 55%
Margariva Vieira (up201907907) - 45%

