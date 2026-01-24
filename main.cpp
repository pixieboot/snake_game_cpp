#include "random.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>

// Randomizes the type of fruit string (visually)
// NOTE: might add different points system depending on the fruit type
std::string_view getRandFruitType(const int x)
{
    switch (x)
    {
    case 1: return "x";
    case 2: return "&";
    case 3: return "$";
    case 4: return "#";
    case 5: return "@";
    case 6: return "%";
    default: return "???";
    }
}

/*  Spawns a fruit in a random spot, also checks if the fruit
 *  and the player are in the same position, if so the fruit
 *  spawn coordinates will be re-randomized
 *
 *  NOTE: in the future refine the logic to base the randomness of the fruit
 *  from the player pos which should reduce the if checking statement
 *
 *  @param &area_dimensions: std::pair consisting of X and Y dimensions for the area
 *  @param &player_position: std::pair consisting of X and Y coordinates of the player
 *  @return: randomized fruit X and Y coordinates based of the parameters
 */
std::pair<int, int> getRandFruitPos(const std::pair<int, int>& area_dimensions,
                                    const std::pair<int, int>& player_position)
{
    const int AREA_WIDTH = area_dimensions.first;
    const int AREA_HEIGHT = area_dimensions.second;
    const int PLAYER_X = player_position.first;
    const int PLAYER_Y = player_position.second;

    int FRUIT_X = Random::get(1, (AREA_WIDTH - 1));
    int FRUIT_Y = Random::get(1, (AREA_HEIGHT - 1));
    if (PLAYER_X == FRUIT_X && PLAYER_Y == FRUIT_Y)
    {
        FRUIT_X = AREA_WIDTH - Random::get(1, (AREA_WIDTH - 1) / 2);
        FRUIT_Y = AREA_HEIGHT - Random::get(1, (AREA_HEIGHT - 1) / 2);
    }
    return std::make_pair(FRUIT_X, FRUIT_Y);
}

/*
 * Spawns the player on a random location based of the area dimensions
 *
 * @param: area dimensions X and Y
 * @returns: random player X and Y coordinates
 */
std::pair<int, int> initPlayerPos(const std::pair<int, int>& area_dimensions)
{
    const int AREA_WIDTH = area_dimensions.first;
    const int AREA_HEIGHT = area_dimensions.second;

    // return std::make_pair(Random::get(2, AREA_WIDTH - 2), Random::get(2, AREA_HEIGHT - 2));
    return std::make_pair(1, 1);
}

std::string_view player()
{
    // std::string player {"0"};
    // for (int i = 0; i < p; i++)
    // {
    //     player = player + "o";
    // }
    // return player;

    return "0";
}

bool isGameOverText(const int i, const int j, const std::pair<int, int>& area_dimensions, const std::string_view text)
{
    const int A_WIDTH = area_dimensions.first;
    const int A_HEIGHT = area_dimensions.second;

    const int ROW = A_HEIGHT / 2;
    const int COL = (A_WIDTH - static_cast<int>(text.size())) / 2;

    return i == ROW && j >= COL && j < COL + static_cast<int>(text.size());
}

char renderCells(
    const int i,
    const int j,
    const std::pair<int, int>& area_dimensions,
    const std::pair<int, int>& player_position,
    const std::pair<int, int>& fruit_position,
    const std::string_view fruit_type,
    const bool game_over_status)
{
    const int A_WIDTH = area_dimensions.first;
    const int A_HEIGHT = area_dimensions.second;

    const int P_X = player_position.first;
    const int P_Y = player_position.second;

    const int F_X = fruit_position.first;
    const int F_Y = fruit_position.second;

    constexpr std::string_view GAME_OVER = "GAME OVER";

    // top
    if (i == 0 && j > 0 && j < A_WIDTH - 1)
    {
        return '^';
    }
    // bottom
    if (i == A_HEIGHT - 1 && j > 0 && j < A_WIDTH - 1)
    {
        return 'v';
    }
    // left
    if (j == 0)
    {
        return '<';
    }
    // right
    if (j == A_WIDTH - 1)
    {
        return '>';
    }
    // fruit
    if (j == F_X && i == F_Y)
    {
        return 'F';
    }
    // player
    if (j == P_X && i == P_Y)
    {
        return 'P';
    }
    // check if its game over
    if (game_over_status && isGameOverText(i, j, area_dimensions, GAME_OVER))
    {
        const int col = (A_WIDTH - static_cast<int>(GAME_OVER.size())) / 2;
        return "GAME OVER"[j - col];
    }
    return ' ';
}

std::string_view renderPoints(const int p)
{
    if (p > 999)
    {
        std::cout << "Points: 999+" << std::endl;
    }
    else
    {
        std::cout << "Points: " << p << std::endl;
    }
    return "???";
}

void renderArea(
    const std::pair<int, int>& area_dimensions,
    const std::pair<int, int>& player_position,
    const std::string_view display_player,
    const std::pair<int, int>& fruit_position,
    const std::string_view fruit_type,
    const int points,
    const bool game_over_status)
{
    const int AREA_WIDTH = area_dimensions.first;
    const int AREA_HEIGHT = area_dimensions.second;

    // needs to be applied outside debug build as system("clear") is performance drop (shell spawn)
    // constexpr const char* CLEAR_SCREEN = "\033[2J\033[H";
    // std::cout << CLEAR_SCREEN;

    system("clear");
    // const std::string SHOW_GAME_OVER{"GAME OVER"};

    renderPoints(points);
    for (int i = 0; i < AREA_HEIGHT; i++)
    {
        for (int j = 0; j < AREA_WIDTH; j++)
        {
            std::cout << renderCells(i, j, area_dimensions, player_position, fruit_position, fruit_type,
                                     game_over_status);
        }
        std::cout << std::endl;
    }
}

int isFruitEaten(
    const std::pair<int, int>& player_position,
    const std::pair<int, int>& fruit_position,
    int* points)
{
    const int P_X = player_position.first;
    const int P_Y = player_position.second;
    const int F_X = fruit_position.first;
    const int F_Y = fruit_position.second;

    if (P_X == F_X && P_Y == F_Y)
    {
        points++;
        return 1;
    }
    else
    {
        return 0;
    }
}

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

int checkForCollisions(const std::pair<int, int>& player_position, const std::pair<int, int>& area_dimensions)
{
    const int P_X = player_position.first;
    const int P_Y = player_position.second;

    const int A_WIDTH = area_dimensions.first;
    const int A_HEIGHT = area_dimensions.second;

    if (P_X == 0 || P_X == A_WIDTH - 1 || P_Y == 0 || P_Y == A_HEIGHT - 1)
    {
        return 1;
    }
    // else if (player_pos_x == player_pos_x) CHECK IF PLAYER EATS ITS OWN TAIL
    return 0;
}

int main()
{
    // set terminal settings for unbuffered input
    setUnbufferedInput();

    // init area dimensions 80x20
    constexpr std::pair<int, int> AREA_DIMENSIONS = std::make_pair(80, 20);

    // init random player position
    std::pair<int, int> player_position = initPlayerPos(AREA_DIMENSIONS);

    // show player
    const std::string_view DISPLAY_PLAYER = player();

    // init random fruit type and position
    std::pair<int, int> fruit_position = getRandFruitPos(AREA_DIMENSIONS, player_position);
    std::string_view fruit_type = getRandFruitType(Random::get(1, 6));

    // init points
    int points = 0;

    // init game over status
    bool game_over_status = false;

    // init first render view
    renderArea(AREA_DIMENSIONS, player_position, DISPLAY_PLAYER, fruit_position, fruit_type, points, game_over_status);

    while (true)
    {
        const unsigned char c = getchar();
        if (c == 119) // w key
        {
            player_position.second -= 1; // player position: y-1
            if (const int is_game_over = checkForCollisions(player_position, AREA_DIMENSIONS); is_game_over == 1)
            {
                game_over_status = true;
            }
            // is_fruit_eaten(player_position, fruit_position);
            renderArea(AREA_DIMENSIONS, player_position, DISPLAY_PLAYER, fruit_position, fruit_type, points,
                       game_over_status);
        }
        else if (c == 115) // s key
        {
            player_position.second += 1; // player position: y+1
            if (const int is_game_over = checkForCollisions(player_position, AREA_DIMENSIONS); is_game_over == 1)
            {
                game_over_status = true;
            }
            renderArea(AREA_DIMENSIONS, player_position, DISPLAY_PLAYER, fruit_position, fruit_type, points,
                       game_over_status);
        }
        else if (c == 97) // a key
        {
            player_position.first -= 1; // player position: x-1
            if (const int is_game_over = checkForCollisions(player_position, AREA_DIMENSIONS); is_game_over == 1)
            {
                game_over_status = true;
            }
            renderArea(AREA_DIMENSIONS, player_position, DISPLAY_PLAYER, fruit_position, fruit_type, points,
                       game_over_status);
        }
        else if (c == 100) // d key
        {
            player_position.first += 1; // player position: x+1
            if (const int is_game_over = checkForCollisions(player_position, AREA_DIMENSIONS); is_game_over == 1)
            {
                game_over_status = true;
            }
            renderArea(AREA_DIMENSIONS, player_position, DISPLAY_PLAYER, fruit_position, fruit_type, points,
                       game_over_status);
        }
        if (game_over_status)
        {
            break;
        }
    }
    return 0;
}
