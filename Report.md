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

<img src="./fig/flow_chart.jpg" width="300">

### Data Processing
The data processing part is processed in the client. 
First, the accelerator collects the data and processes it using the queue. 
We use the queue to store our data to reduce the time of reserving and deleting data. 
After storing the data, we calculate the weighted average of the data. 
The weight of the data strongly affects the user's experience of playing the game. 
When testing our game, we found that the queue with size two and the weight (0.2, 0.8) is the best parameter.

### Comparison of different ways to process data
The data might encounter noise from the sensor when playing the game, so we use different ways to process the data. 
When the queue size is large, that is, the filter's impulse response is long, the edge of the signal might disappear. 
Also, if the filter is short, the signal isn't smooth. The graph below shows the difference.

<img src="./fig/filter.jpg" width="500">

