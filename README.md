# Snake game in cpp

Personal take on and first written text based snake game that runs in a terminal window. Written under Linux OS and cpp language for learning purposes about game development world. This is created with no prior game development research, experience or insight on how games should be planned and created beforehand. It's a very basic, and simple game (obviously with quality bugs) written without any external useful libraries for terminal handling such as curses and ncurses, I decided to try and do it the hard way to fully understand what's happening under the hood. Currently the game can only run on *nix systems.   

# Demo   

<img width="1236" height="912" alt="Screenshot_20260129_234054" src="https://github.com/user-attachments/assets/ef824644-07e2-47f7-b267-ae69bb2d28a4" />   
<img width="1241" height="909" alt="Screenshot_20260129_234043" src="https://github.com/user-attachments/assets/b04aa7b7-eef0-4971-9eb6-f71a0d190974" />   
<img width="1238" height="921" alt="Screenshot_20260129_234016" src="https://github.com/user-attachments/assets/6882cdf3-57aa-4770-93d3-993556cc0fca" />   

Video demo:   
https://github.com/user-attachments/assets/2ff52dab-3415-47fb-bb47-d41d65886297

Known bugs:   
* snake can go the opposite direction if the kb key is held trigger game over even when its not visually showing it
* game over screen can still accept kb inputs and increase points
* pressing any key will speed up the snake as it will refresh while loop because the if for wsad will skip the invalid keys[Screencast_20260129_233522.webm]

