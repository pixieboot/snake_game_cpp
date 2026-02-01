# Snake game on the terminal written in C++

Personal take on and first written text based snake game that runs in a terminal window. Written under Linux OS and C++
language for learning about game development world. This is created with no prior game development research, experience
or insight on how games should be planned and created. It's a very basic, and simple game written without any external
useful libraries for terminal handling such as "curses" and "ncurses",
I decided to try and do it the hard way to fully understand what's happening under the hood. Currently, the game can
only
run on *nix systems.

# Screenshots

<img width="680" height="365" alt="start" src="https://github.com/user-attachments/assets/8466589d-cbdf-485b-93aa-4e8cbaec4951" />
<img width="680" height="365" alt="gameplay" src="https://github.com/user-attachments/assets/36aee01e-cefe-4339-872d-2d54f7bc8b1b" />
<img width="680" height="365" alt="gameover" src="https://github.com/user-attachments/assets/2c20f2f5-09a1-4d18-9e50-e63a8a4ecbb7" />   

**Debug mode:**   
<img width="680" height="607" alt="debugmode" src="https://github.com/user-attachments/assets/f11dd1f4-bda9-4def-939a-4faeb54931f1" />

Video demo can be seen [here](https://github.com/user-attachments/assets/9c32e5b0-d92b-4f5f-9c6b-c12dd7680e98)

## Building and running:

**Linux:**

*A)*

1. If needed, `chmod +x build.sh` the provided shell script
2. Run the `build.sh` file from your file explorer or from the terminal with `./build.sh`

*B)*

1. You can directly download the executable file
   from [here](https://github.com/pixieboot/snake_game_cpp/releases/tag/v0.1.0) and run it from the terminal

**Windows:**
> To be added (currently Windows bin is not possible because of "termios.h" lib which is only for *nix systems)

[//]: # (**Future plans:**)

[//]: # ()

[//]: # (* **Adding arrow key support >** This was already attempted but arrow keys are 3 characters long &#40;e.g. on)

[//]: # (  my system: 91 67 27 from getchar&#40;&#41;. It is different on others OS or other unix distros are interpreting it)

[//]: # (  differently as well&#41;, and the only temporary solution was calling getchar&#40;&#41; func 2 additional times to get the proper)

[//]: # (  value through first escape character which will not be the same case in another system. Better approach and solution)

[//]: # (  needs to be applied, and ncurses lib elevates this nuisance or some sort of key mapping.)

# Gameplay details

The gameplay itself is very simple. In the start screen, the player picks where the snake's direction will start.
Snake is moving in a constant line while the player can change its direction with WSAD where W is up, S is down,
A is left and D is right. The goal in the game is for the player to navigate the snake towards fruit that randomly,
spawns everytime the snake eats it, which will then increase snake's size in tail length and also speed up its
movement which gradually makes the game more challenging as more points are acquired. The game is ended if the snake
touches any of the walls or its own tail. Player can either quit the game or continue from anew.

# Technical details

Game is fully rendered with single characters (char) from a loop that draws the area, walls, snake and fruit
within the terminal. The main loop speed is limited with a fixed time stepper function that is accumulating total time
elapsed per loop (frame) and checks is it equals to the given fixed time. If the accumulator becomes equals or higher
than the compared fixed time variable, it will run the frame loop (rendering the image) until the accumulator is reduced
enough until it's lesser than the given time, once it's below the time value, the loop will skip the frames and wait
until the accumulator becomes again equals or higher. This method prevents the snake going too fast when a keyboard key
is held, since holding a key retriggers the loop at the speed of CPU clock and "speeds up" the game. The fruit is
randomly spawned within the area box and its visual type is also randomly picked on each instance from an enum. The
program is not using any 3rd libraries for the terminal settings, so the terminal had to be manually set to raw
non-blocking unbuffered mode for keyboard inputs so the player can consistently change directions of the snake without
them needing to press enter after any inputs or have any delays.