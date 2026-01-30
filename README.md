# Snake game in C++

Personal take on and first written text based snake game that runs in a terminal window. Written under Linux OS and C++
language for learning about game development world. This is created with no prior game development research, experience
or insight on how games should be planned and created. It's a very basic, and simple game (obviously with good quality
bugs at the moment) written without any external useful libraries for terminal handling such as "curses" and "ncurses",
I decided to try and do it the hard way to fully understand what's happening under the hood. Currently the game can only
run on *nix systems.

# Demo

<img width="680" height="365" alt="start" src="https://github.com/user-attachments/assets/8466589d-cbdf-485b-93aa-4e8cbaec4951" />
<img width="680" height="365" alt="gameplay" src="https://github.com/user-attachments/assets/36aee01e-cefe-4339-872d-2d54f7bc8b1b" />
<img width="680" height="365" alt="gameover" src="https://github.com/user-attachments/assets/2c20f2f5-09a1-4d18-9e50-e63a8a4ecbb7" />   

**Debug mode:**   
<img width="680" height="607" alt="debugmode" src="https://github.com/user-attachments/assets/f11dd1f4-bda9-4def-939a-4faeb54931f1" />

**Video demo:**   
[demo](https://github.com/user-attachments/assets/9c32e5b0-d92b-4f5f-9c6b-c12dd7680e98)

## Building and running:

**Linux:**

A)

1. If needed, `chmod +x build.sh` the provided shell script
2. Run the `build.sh` file from your file explorer or from the terminal with `./build.sh`

B)

1. You can directly download the executable file
   from [here](https://github.com/pixieboot/snake_game_cpp/releases/tag/v0.1.0) and run it from the terminal

**Windows:**
> To be added (currently win is not possible because of "termios.h" lib which is only for *nix systems)

**Known bugs:**

* Snake can go pseudo-like the opposite direction if the kb key is held, it will trigger game over event
* Game over screen can still accept kb inputs and increase points (e.g. holding A during game over screen)
* Pressing any key that has singe ascii character will speed up the snake as it will refresh while loop because if
  for WSAD will skip the invalid keys and continue the while loop

**Future plans:**

* **Adding arrow key support >** This was already attempted but the issue is arrow keys are 3 characters (e.g. on my
  system: 91 67 27, it might be different on others systems) long, and the only temporary solution was calling getchar()
  func 2 additional times and checking with "ifs" if it's the right character since it had to go through first escape
  character. A better approach and solution needs to be applied, and ncurses lib seems to elevate this issue for devs,
  its implementation will be considered in the future.
* **Rewriting replayability logic >** For this, an entire code structure needs to be rewritten and resorted as calling
  main() recursively is a disaster on its own but, I've put it right now as a lazy solution for demoing the game.
* **Figuring out better rendering logic >** Not always but when debugging the terminal flickers meanwhile when running
  on
  an actual terminal its fine, will dig further into this.
