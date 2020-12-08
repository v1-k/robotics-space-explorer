# Room Explorer and Object finder Robot

## Software Design

## Path Planning
### Basic Algorithm Approach
* Basic Intuition helped us decide that it will be very complex to derive a systematic movement algorithm and design to guide the robot to cover whole maze without knowing maze layout.
* From any early systematic strategies, we came up with showed us that generalizing them might not be possible without avoiding the chance of being stuck in a loop based on the maze layout.
* So, we decided on the implementing an algorithm that would imitate the path covered by a ball bouncing of the walls.

### Ball Bouncing Algorithm
* As to cover the whole maze without being stuck in loops we needed some level of randomness in our algorithm design we decided on imitating the path of a ball bouncing of the walls for our robot.
* If a ball was to bounce of the walls of the maze without losing any of its speed and energy due to collisions with walls it should in theory cover the whole maze if it bounces of at the correct angle.
* As seen in the image below the diagram the robot indicated with the black dot will enter the room, explore the room for the goal and then exit the room to explore someplace else if the goal is not found.
<img src="images/ball_bounce.png" width="256">
