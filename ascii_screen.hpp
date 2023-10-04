#ifndef MY_ASCII_SCREEN_HEADER_2_OCT_2023
#define MY_ASCII_SCREEN_HEADER_2_OCT_2023

#include <Windows.h>

class entity;

class ascii_screen{

    public:

        int width; // width in characters
        int height; // height in characters
        char *chars; // characters to show on the screen

        // Things Windows needs
        HANDLE h_screen;
        DWORD bytes_written;

        ~ascii_screen();
        ascii_screen(const int width_in_chars, const int height_in_chars);

        void display();
        void clear();
        void draw_sprite(const entity& e);
        void confine_entity(entity& e) const;
        void print_score_line(const int score, const int best_score);
        void show_message_in_centre_of_blank_screen(const char *message);
        void show_death_screen();
        void show_pause_menu();
        void show_start_menu();

};

void place_on_screen_without_overlap(entity& e_to_place, const entity& e_to_avoid, const ascii_screen& screen);

#endif