#include "ascii_screen.hpp"
#include "entity.hpp"
#include <algorithm>
#include <random>
#include <string.h>

ascii_screen::~ascii_screen(){

    delete[] chars;

}

// See https://learn.microsoft.com/en-us/windows/console/
ascii_screen::ascii_screen(const int width_in_chars, const int height_in_chars){

    width = width_in_chars;
    height = height_in_chars;

    chars = new char[width*height];

    // Open a console window and assign it to the game.
    FreeConsole();
    AllocConsole();
    h_screen = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(h_screen);
    bytes_written = 0;

    const char *title = "My First Game";
    SetConsoleTitle(title);

    // Set the console window to the desired size.
    SetWindowPos(GetConsoleWindow(), NULL, 0, 0, 0, 0, SWP_NOSIZE);
    SetConsoleScreenBufferSize(h_screen, {(short)width, (short)height});
    const SMALL_RECT size_rect = {0, 0, short(width-1), short(height-1)};
    SetConsoleWindowInfo(h_screen, TRUE, &size_rect);

    // Hide the cursor.
    const CONSOLE_CURSOR_INFO info = {1, FALSE};
    SetConsoleCursorInfo(h_screen, &info);

}

void ascii_screen::display(){

	WriteConsoleOutputCharacter(h_screen, chars, width*height, {0, 0}, &bytes_written);

}

void ascii_screen::clear(){

    for (int n = 0; n < width*height; n++){
        chars[n] = ' ';
    }

}

void ascii_screen::draw_sprite(const entity& e){

    const int x = (int) e.x_posn;
    const int y = (int) e.y_posn;

    for (int i = 0; i < e.y_size; i++){

        const int row = i+y;

        for (int j = 0; j < e.x_size; j++){

            const int col = j+x;

            chars[col + row*width] = e.sprite_char;

        }

    }

}

float keep_in_range(const float val, const float lower, const float upper){

    return std::max<float>(lower, std::min<float>(val, upper));

}

void ascii_screen::confine_entity(entity& e) const {

    e.x_posn = keep_in_range(e.x_posn, 0, width - e.x_size);
    e.y_posn = keep_in_range(e.y_posn, 1, height - e.y_size);

}


void ascii_screen::print_score_line(const int score, const int best_score){

    sprintf(chars, "Score: %d    Best: %d", score, best_score);

}

void ascii_screen::show_message_in_centre_of_blank_screen(const char *message){

    clear();

    // Centre the message on the screen.
    const int message_length = strlen(message);
    const int row = height/2;
    const int col = (width - message_length)/2;

    sprintf(&chars[row*width + col], message);

    display();

}

void ascii_screen::show_death_screen(){

    show_message_in_centre_of_blank_screen("You died! Press [Esc] to exit the game or [R] to retry.");

}

void ascii_screen::show_pause_menu(){

    show_message_in_centre_of_blank_screen("Game paused. Press [Esc] to exit the game or [R] to resume.");

}

void ascii_screen::show_start_menu(){

    show_message_in_centre_of_blank_screen("Using the arrow keys to move, reach the targets before you get caught! Press [R] to start.");

}















void place_on_screen_without_overlap(entity& e_to_place, const entity& e_to_avoid, const ascii_screen& screen){

    std::random_device dev;
    std::mt19937 mt(dev());

    // Limits should match those in ascii_screen::confine_entity
    std::uniform_int_distribution<> x_dist(0, screen.width - e_to_place.x_size);
    std::uniform_int_distribution<> y_dist(1, screen.height - e_to_place.y_size);

    while (sprites_overlap(e_to_place, e_to_avoid)){

        e_to_place.x_posn = x_dist(mt);
        e_to_place.y_posn = y_dist(mt);

    }

}
