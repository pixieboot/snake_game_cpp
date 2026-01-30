# Snake game in cpp

Personal take on and first written text based snake game that runs in a terminal window. Written under Linux OS and cpp language for learning purposes about game development world. This is created with no prior game development research, experience or insight on how games should be planned and created beforehand. It's a very basic, and simple game (obviously with quality bugs) written without any external useful libraries for terminal handling such as curses and ncurses, I decided to try and do it the hard way to fully understand what's happening under the hood. Currently the game can only run on *nix systems.   

# Demo   

<img width="1236" height="912" alt="Screenshot_20260129_234054" src="https://github.com/user-attachments/assets/ef824644-07e2-47f7-b267-ae69bb2d28a4" />   
<img width="1241" height="909" alt="Screenshot_20260129_234043" src="https://github.com/user-attachments/assets/b04aa7b7-eef0-4971-9eb6-f71a0d190974" />   
<img width="1238" height="921" alt="Screenshot_20260129_234016" src="https://github.com/user-attachments/assets/6882cdf3-57aa-4770-93d3-993556cc0fca" />   

Video demo:   
https://github.com/user-attachments/assets/2ff52dab-3415-47fb-bb47-d41d65886297

# Compiling and running:   
Linux:   
1. `cd [path to the dir where main.cpp and random.h are located]`   
2. `g++ main.cpp -o snake`

Or you can directly download the executable file from [here](https://github.com/pixieboot/snake_game_cpp/releases/tag/v0.1.0) and run it from your terminal

Windows:    
> will be added in the future   

Known bugs:   
* snake can go the opposite direction if the kb key is held trigger game over even when its not visually showing it
* game over screen can still accept kb inputs and increase points
* pressing any key will speed up the snake as it will refresh while loop because the if for wsad will skip the invalid keys and loop continues

Future plans:
* adding arrow key support > (this was already attempted but the issue is arrow keys are 3 characters (e.g. on my system: 91 67 27, it might be different on others systems) long, and the only temporary solution was calling getchar() func 2 additional times and checking with "ifs" if its the right character since it had to go through first escape character. A better approach and solution needs to be applied, and ncurses lib seems to elevate this issue for devs, will be considered)
* rewritting replayability logic > (for this an entire code structure needs to be rewritten and resorted as calling main recursively is a disaster on its own but i've put it right now as a lazy solution for demoing the game yay)
* figuring out better rendering logic > (not always but when debugging the terminal flickers while when running on an actual terminal its fine, will dig further into this)
