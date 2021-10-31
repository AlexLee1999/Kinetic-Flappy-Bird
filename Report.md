## Flappy Bird Using STM32 board Sensor

### Authors  
Chun-Yen Lee Chia-Hung Lin Cyun-Bo Wang
### Motivation

Flappy Bird is a game that uses white space to control the character, and the link(https://www.youtube.com/watch?v=I69adfEqwC0) provides information on how to play the game. 
From the video, when the player presses the space button, the character flies and drops immediately. 
However, the initial video game only requires players to press the space button, which seems boring. 
To enhance the user’s experience, we modified the game and used the STM32 board to control the character in the game. 
The modification gives the player more satisfaction by adding more ways to control the character.

### Design Details
In the original game, there is gravity. 
In our game, we canceled the setting of gravity to make the players more possible actions in the game. 
By tilting forward and backward, we can control the player up and down in the game. 
The detailed flow chart is shown below.

<img src="./fig/flow_chart.png" width="300">
