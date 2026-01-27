#include "random.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <vector>

// If debug mode is set to true it will print the current runtime values
void showDebugStats(
    const std::pair<int, int>& area_dimensions,
    const std::vector<std::pair<int, int>>& player_position,
    const std::pair<int, int>& fruit_position,
    const char fruit_type,
    const bool game_over_status,
    const std::pair<int, int>& temp)
{
    std::cout << "AREA > X[" << area_dimensions.first << "] Y[" << area_dimensions.second << "]\n";
    std::cout << "FRUIT > X[" << fruit_position.first << "] Y[" << fruit_position.second << "] | ";
    std::cout << "TYPE > [" << fruit_type << "]\n\n";
    std::cout << "PLAYER\n";
    std::cout << "---------------------\n";
    for (int i = 0; i < player_position.size(); ++i)
    {
        std::cout << "NODE [" << i << "] > X[" << player_position[i].first << "] Y[" << player_position[i].second <<
            "]\n";
    }

    std::cout << "\nTEMP INDEX 0 POS: X:[" << temp.first << "] Y:[" << temp.second << "]\n";
    std::cout << "\nGAME OVER > [" << game_over_status << "]\n";
}

/* Randomizes the type of fruit string (visually)
 *
 * @param const int: random integer between 1 and 6 for the fruit type to be returned
 * NOTE: might add different points system depending on the fruit type
*/
char getRandFruitType(const int x)
{
    switch (x)
    {
    case 1: return 'x';
    case 2: return '&';
    case 3: return '$';
    case 4: return '#';
    case 5: return '@';
    case 6: return '%';
    default: return '?';
    }
}

/*  Spawns a fruit in a random spot, also checks if the fruit
 *  and the player are in the same position, if so the fruit
 *  spawn coordinates will be re-randomized
 *
 *  NOTE: in the future refine the logic to base the randomness of the fruit
 *  from the player pos which should reduce the if checking statement
 *
 *  @param std::pair<int, int>& area_dimensions: std::pair consisting of X and Y dimensions for the area
 *  @param std::pair<int, int>& player_position: std::pair consisting of X and Y coordinates of the player
 *  @return std::pair<int, int>: randomized fruit X and Y coordinates based of the parameters
 */
std::pair<int, int> getRandFruitPos(const std::pair<int, int>& area_dimensions,
                                    const std::pair<int, int>& player_position)
{
    const int a_width = area_dimensions.first;
    const int a_height = area_dimensions.second;
    const int p_x = player_position.first;
    const int p_y = player_position.second;

    int f_x = Random::get(1, (a_width - 1));
    int f_y = Random::get(1, (a_height - 1));
    if (!(a_width == f_x && a_height == f_y))
    {
        f_x = Random::get(1, (a_width - 1) / 2);
        f_y = Random::get(1, (a_height - 1) / 2);
    }
    return std::make_pair(f_x, f_y);
}

/* Spawns the player on a random location based of the area dimensions
 *
 * @param std::pair<int, int> &area_dimensions: area dimensions of X and Y
 * @returns std::pair<int, int>: random player X and Y coordinates
 */
std::pair<int, int> initPlayerPos(const std::pair<int, int>& area_dimensions)
{
    const int a_width = area_dimensions.first;
    const int a_height = area_dimensions.second;

    return std::make_pair(Random::get(2, a_width - 2), Random::get(2, a_height - 2));
}

/*  Calculates given text.size to return boolean if the given text
 *  is within the area where it needs to be printed in the middle of the screen
 *
 *  @param i & j: for loop iterators
 *  @param std::pair<int, int>& area_dimensions: std::pair consisting of X and Y coordinates of area
 *  @param std::string_view text: text that will be calculated to fit in the middle of the screen with text.size()
 *  @param int is_game_over_text: is the text "GAME OVER", alignment purposes with if statement
 *  @return bool: bool if the given text is in the area where it needs to be printed
 */
bool alignGameOverText(
    const int i,
    const int j,
    const std::pair<int, int>& area_dimensions,
    const std::string_view text,
    const int is_game_over_text)
{
    const int a_width = area_dimensions.first;
    const int a_height = area_dimensions.second;
    const int text_size = (a_width - static_cast<int>(text.size())) / 2;
    int row{};

    // "GAME OVER" alignment
    if (is_game_over_text == 1)
    {
        row = a_height / 2;
    }
    else // "Points: x" alignment
    {
        row = (a_height / 2) + 1;
    }

    return i == row && j >= text_size && j < text_size + static_cast<int>(text.size());
}

/*  Returns chars of strings to display game over text status at the
 *  end of the game, it calls align function to check in which area
 *  should it start printing the text in the middle of the screen
 *
 *  @param i & j: for loop iterators
 *  @param std::pair<int, int>& area_dimensions: std::pair consisting of X and Y coordinates of area
 *  @param int points: points to be printed alongside text
 *  @return char: returns a char that needs to be printer to the called
 */
char showGameOverStats(
    const int i,
    const int j,
    const std::pair<int, int>& area_dimensions,
    const int points)
{
    const int a_width = area_dimensions.first;
    const int a_height = area_dimensions.second;

    constexpr const std::string_view game_over_text = "GAME OVER";
    const std::string_view points_text = "Points: " + std::to_string(points); // look for a better solution later

    // prints "GAME OVER"
    if (alignGameOverText(i, j, area_dimensions, game_over_text, 1))
    {
        const int col = (a_width - static_cast<int>(game_over_text.size())) / 2;
        return game_over_text[j - col];
    }

    // prints "Points: X"
    if (alignGameOverText(i, j, area_dimensions, points_text, 0))
    {
        const int col = (a_width - static_cast<int>(points_text.size())) / 2;
        return points_text[j - col];
    }
    return ' ';
}

char renderCells(
    const int i,
    const int j,
    const std::pair<int, int>& area_dimensions,
    const std::vector<std::pair<int, int>>& player_position,
    const std::pair<int, int>& fruit_position,
    const char fruit_type,
    const bool game_over_status,
    const int points)
{
    const int a_width = area_dimensions.first;
    const int a_height = area_dimensions.second;

    const int f_x = fruit_position.first;
    const int f_y = fruit_position.second;

    // top
    if (i == 0 && j > 0 && j < a_width - 1)
    {
        return '^';
    }
    // bottom
    if (i == a_height - 1 && j > 0 && j < a_width - 1)
    {
        return 'v';
    }
    // left
    if (j == 0)
    {
        return '<';
    }
    // right
    if (j == a_width - 1)
    {
        return '>';
    }
    // fruit
    if (j == f_x && i == f_y)
    {
        return fruit_type;
    }
    // check if its game over and print game over stats
    if (game_over_status)
    {
        return showGameOverStats(i, j, area_dimensions, points);
    }
    // player
    for (const auto& [p_x, p_y] : player_position)
    {
        if (p_x == j && p_y == i)
        {
            return 'o';
        }
    }
    return ' ';
}

/*  Displays player collected points on runtime

 *  @param const int: points num passed
 *  @return void: uses std::cout
 */
void showPoints(const int p)
{
    if (p > 999)
    {
        std::cout << "Points: 999+\n";
    }
    else
    {
        std::cout << "Points: " << p << '\n';
    }
}

/*  Renders (std::couts) entire screen with for loops
 *
 *  @param i & j: for loop iterators
 *  @param std::pair<int, int>& player_position: X and Y coordinates of area
 *  @param std::pair<int, int>& area_dimensions: X and Y coordinates of the player
 *  @param std::pair<int, int>& fruit_position: X and Y coordinates of the spawned fruit
 *  @param std::string_view: shows the player figure
 *  @param int points: passed points from main
 *  @param bool game_over_status: true or false if the game is over
 *
 *  @return void
 */
void renderArea(
    const std::pair<int, int>& area_dimensions,
    const std::vector<std::pair<int, int>>& player_position,
    const std::pair<int, int>& fruit_position,
    const char fruit_type,
    const int points,
    const bool game_over_status,
    const bool debug_mode,
    const std::pair<int, int>& temp)
{
    const int a_width = area_dimensions.first;
    const int a_height = area_dimensions.second;

    // needs to be applied outside debug build as system("clear") is performance drop (shell spawn)
    // constexpr const char* CLEAR_SCREEN = "\033[2J\033[H";
    // std::cout << CLEAR_SCREEN;

    system("clear");
    if (debug_mode)
    {
        showDebugStats(area_dimensions, player_position, fruit_position, fruit_type,
                       game_over_status, temp);
    }
    showPoints(points);
    for (int i = 0; i < a_height; i++)
    {
        for (int j = 0; j < a_width; j++)
        {
            std::cout << renderCells(i, j, area_dimensions, player_position, fruit_position, fruit_type,
                                     game_over_status, points);
        }
        std::cout << '\n';
    }
}

/* Checks if the fruit location is the same as player location
 *
 *  @param std::pair<int, int>& player_position: std::pair consisting of X and Y coordinates of the player
 *  @param std::pair<int, int>& fruit_position: std::pair consisting of X and Y coordinates of the fruit spawn
 *  @return: true or false if the fruit has been eaten
 */
bool isFruitEaten(
    const std::pair<int, int>& player_position,
    const std::pair<int, int>& fruit_position)
{
    const int p_x = player_position.first;
    const int p_y = player_position.second;
    const int f_x = fruit_position.first;
    const int f_y = fruit_position.second;

    if (p_x == f_x && p_y == f_y)
    {
        return true;
    }
    else
    {
        return false;
    }
}


/* Sets the terminal buffer to unbuffered mode for
 * non-blocking user input when the snake is controlled
 *
 * @return void;
 */
void setUnbufferedInput()
{
    struct termios old_tio{}, new_tio{};

    // get the terminal settings for stdin
    tcgetattr(STDIN_FILENO, &old_tio);

    // keeping the old settings to restore them at the end
    new_tio = old_tio;

    // disable buffered i/o and local echo
    new_tio.c_lflag &= ~(ICANON | ECHO);

    // set the new settings
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

/*  Checks if player has reached the edges of the area walls
 *
 *  @param std::pair<int, int>& player_position: std::pair consisting of X and Y coordinates of the player
 *  @param std::pair<int, int>& area_dimensions: std::pair consisting of X and Y dimensions for the area
 *  @return: true or false if the player has collided with the wall
 */
bool checkForCollisions(const std::pair<int, int>& player_position, const std::pair<int, int>& area_dimensions)
{
    const int p_x = player_position.first;
    const int p_y = player_position.second;

    const int a_width = area_dimensions.first;
    const int a_height = area_dimensions.second;

    if (p_x == 0 || p_x == a_width - 1 || p_y == 0 || p_y == a_height - 1)
    {
        return true;
    }
    // else if (player_pos_x == player_pos_x) CHECK IF PLAYER EATS ITS OWN TAIL
    return false;
}

int main()
{
    // set terminal settings for unbuffered input
    setUnbufferedInput();

    // set debug mode
    const bool debug_mode = true;

    // init area dimensions 80x21
    constexpr std::pair<int, int> area_dimensions = std::make_pair(80, 21);

    // snake length initialized with player spawn position
    std::vector<std::pair<int, int>> snake_body{initPlayerPos(area_dimensions)};

    // init random fruit type and position // change later for the whole body checking
    std::pair<int, int> fruit_position = getRandFruitPos(area_dimensions, snake_body[0]);
    char fruit_type = getRandFruitType(Random::get(1, 6));

    // init points
    int points = 0;

    // init game over status
    bool game_over_status = false;

    // init first render view
    renderArea(area_dimensions, snake_body, fruit_position, fruit_type, points, game_over_status, debug_mode, {0, 0});

    while (true)
    {
        std::pair<int, int> temp = {snake_body[0].first, snake_body[0].second};
        switch (const unsigned char c = getchar())
        {
        // W key
        case 119:
            snake_body[0].second -= 1; // player position: y-1
            break;
        // S key
        case 115:
            snake_body[0].second += 1; // player position: y+1
            break;
        // A key
        case 97:
            snake_body[0].first -= 1; // player position: x-1
            break;
        // D key
        case 100:
            snake_body[0].first += 1; // player position: x+1
            break;
        default: ;
        }

        if (checkForCollisions(snake_body[0], area_dimensions))
        {
            game_over_status = true;
        }
        if (isFruitEaten(snake_body[0], fruit_position))
        {
            points++;
            fruit_type = getRandFruitType(Random::get(1, 6));
            fruit_position = getRandFruitPos(area_dimensions, snake_body[0]);
            snake_body.emplace_back(temp);
        }
        if (points)
        {
            for (int i = points; i > 0; --i)
            {
                snake_body[i] = snake_body[i - 1];
            }
            // when arr is shifted to the right, we need to insert temp to the 1st index
            // of the arr to have proper display and update of nodes, otherwise we'd have
            // 2 identical nodes (0 and 1) and would not render on 1st point but instead from 2nd
            snake_body[1] = temp;
        }
        renderArea(area_dimensions, snake_body, fruit_position, fruit_type, points,
                   game_over_status, debug_mode, temp);
        if (game_over_status) break;
    }
    return 0;
}
