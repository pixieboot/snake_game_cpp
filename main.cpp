#include "random.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>

// Randomizes the type of fruit string (visually)
// NOTE: might add different points system depending on the fruit type
std::string_view get_rand_fruit_type(const int x)
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
std::pair<int, int> get_rand_fruit_pos(const std::pair<int, int> &area_dimensions, const std::pair<int, int> &player_position)
{
    const int AREA_WIDTH {area_dimensions.first};
    const int AREA_HEIGHT {area_dimensions.second};
    const int PLAYER_X {player_position.first};
    const int PLAYER_Y {player_position.second};

    int FRUIT_X {Random::get(1, (AREA_WIDTH - 1))};
    int FRUIT_Y {Random::get(1, (AREA_HEIGHT - 1))};
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
std::pair<int, int> init_player_pos(const std::pair<int, int> &area_dimensions)
{
    const int AREA_WIDTH {area_dimensions.first};
    const int AREA_HEIGHT {area_dimensions.second};

    return std::make_pair(Random::get(2, AREA_WIDTH - 2), Random::get(2, AREA_HEIGHT - 2));
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

void render_area(
    const std::pair<int, int> &area_dimensions,
    const std::pair<int, int> &player_position,
    const std::string_view display_player,
    const std::pair<int, int> &fruit_position,
    const std::string_view fruit_type,
    const int points,
    const int game_over)
{
    const int AREA_WIDTH {area_dimensions.first};
    const int AREA_HEIGHT {area_dimensions.second};
    const int PLAYER_X {player_position.first};
    const int PLAYER_Y {player_position.second};
    const int FRUIT_X {fruit_position.first};
    const int FRUIT_Y {fruit_position.second};

    system("clear");
    if (game_over != 1)
    {
        if (points > 999)
        {
            std::cout << "Points: 999+" << std::endl;
        }
        else
        {
            std::cout << "Points: " << points << std::endl;
        }
        for (int i = 0; i < AREA_HEIGHT; i++)
        {
            for (int j = 0; j < AREA_WIDTH; j++)
            {
                if (i == 0 && j != 0 && j != AREA_WIDTH -1)
                {
                    std::cout << '^';
                }
                else if (j == 0)
                {
                    std::cout << '<';
                }
                else if (j == AREA_WIDTH - 1)
                {
                    std::cout << '>';
                }
                else if (i == AREA_HEIGHT - 1)
                {
                    std::cout << 'v';
                }
                else if (FRUIT_X == j && FRUIT_Y == i)
                {
                    std::cout << fruit_type;
                }
                else if (PLAYER_X == j && PLAYER_Y == i)
                {
                    std::cout << display_player;
                }
                else std::cout << ' ';
            }
            std::cout << std::endl;
        }
    }
    else
    {
        for (int i = 0; i < AREA_HEIGHT; i++)
        {
            for (int j = 0; j < AREA_WIDTH; j++)
            {
                if (i == 0 && j != 0 && j != AREA_WIDTH -1)
                {
                    std::cout << '^';
                }
                else if (j == 0)
                {
                    std::cout << '<';
                }
                else if (j == AREA_WIDTH - 1)
                {
                    std::cout << '>';
                }
                else if (i == AREA_HEIGHT - 1)
                {
                    std::cout << 'v';
                }
                else if (AREA_HEIGHT / 2 == i && AREA_WIDTH / 2 == j)
                {
                    if (AREA_HEIGHT / 2 == i && (AREA_WIDTH / 2) - 5 == j)
                    {
                        std::cout << "Game Over!";
                    }
                }
                else std::cout << ' ';
            }
            std::cout << std::endl;
        }
    }
}

int main()
{
    struct termios old_tio, new_tio;

    // get the terminal settings for stdin
    tcgetattr(STDIN_FILENO, &old_tio);

    // keeping the old settings to restore them at the end
    new_tio = old_tio;

    // disable buffered i/o and local echo
    new_tio.c_lflag &= ~(ICANON | ECHO);

    // set the new settings
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    // init area dimensions 80x20
    constexpr std::pair<int, int> area_dimensions = std::make_pair(80, 20);
    const int *AREA_X = &area_dimensions.first;
    const int *AREA_Y = &area_dimensions.second;

    // init random player position
    std::pair<int, int> player_position = init_player_pos(area_dimensions);
    const std::string_view display_player = player();

    int *player_x = &player_position.first;
    int *player_y = &player_position.second;

    // init random fruit type and position
    std::pair<int, int> fruit_position = get_rand_fruit_pos(area_dimensions, player_position);
    std::string_view fruit_type = get_rand_fruit_type(Random::get(1, 6));

    std::pair<int, int> *FRUIT = &fruit_position;
    std::string_view *FRUIT_TYPE = &fruit_type;
    const int *FRUIT_X = &fruit_position.first;
    const int *FRUIT_Y = &fruit_position.second;

    // init points
    int points = 0;

    // init game over status
    int game_over = 1;

    // init first render view
    render_area(area_dimensions, player_position, display_player, fruit_position, fruit_type, points, game_over);

    while (true)
    {
        const unsigned char c = getchar();
        if (c == 119)
        {
            *player_y -= 1;
            if (*player_x == *AREA_X || *player_y == *AREA_Y)
            {
                game_over = 1;
            }
            if (*player_x == *FRUIT_X && *player_y == *FRUIT_Y)
            {
                points++;
                *FRUIT = get_rand_fruit_pos(area_dimensions, player_position);
                *FRUIT_TYPE = get_rand_fruit_type(Random::get(1, 6));
            }
            render_area(area_dimensions, player_position, display_player, fruit_position, fruit_type, points, game_over);
        }
        else if (c == 115)
        {
            *player_y += 1;
            if (*player_x == *FRUIT_X && *player_y == *FRUIT_Y)
            {
                points++;
                *FRUIT = get_rand_fruit_pos(area_dimensions, player_position);
                *FRUIT_TYPE = get_rand_fruit_type(Random::get(1, 6));
            }
            render_area(area_dimensions, player_position, display_player, fruit_position, fruit_type, points, game_over);
        }
        else if (c == 97)
        {
            *player_x -= 1;
            if (*player_x == *FRUIT_X && *player_y == *FRUIT_Y)
            {
                points++;
                *FRUIT = get_rand_fruit_pos(area_dimensions, player_position);
                *FRUIT_TYPE = get_rand_fruit_type(Random::get(1, 6));
            }
            render_area(area_dimensions, player_position, display_player, fruit_position, fruit_type, points, game_over);
        }
        else if (c == 100)
        {
            *player_x += 1;
            if (*player_x == *FRUIT_X && *player_y == *FRUIT_Y)
            {
                points++;
                *FRUIT = get_rand_fruit_pos(area_dimensions, player_position);
                *FRUIT_TYPE = get_rand_fruit_type(Random::get(1, 6));
            }
            render_area(area_dimensions, player_position, display_player, fruit_position, fruit_type, points, game_over);
        }
        if (c == 49)
        {
            break;
        }
    }
    return 0;
}