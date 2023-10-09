#ifndef MY_OPENGL_SCREEN_HEADER_7_OCT_2023
#define MY_OPENGL_SCREEN_HEADER_7_OCT_2023

#include <GL/glew.h> // Apparently it's important that this one is included first out of the OpenGL headers.
#include <GLFW/glfw3.h>
#include <FTGL/ftgl.h>

class entity;

class opengl_screen{

    public:

        GLFWwindow* window;
        int width; // in pixels
        int height; // in pixels
        FTGLPixmapFont font;

        ~opengl_screen();
        opengl_screen(const int width_in, const int height_in, const char* title);

        void display();
        void clear();
        float pixels_to_screen_coords(const float px, const int dim_flag);
        void vertex(const float x, const float y);
        void draw_sprite(const entity& e);
        void confine_entity(entity& e) const;
        void print_score_line(const int score, const int best_score);
        void show_message_in_centre_of_blank_screen(const char *message);
        void show_death_screen();
        void show_pause_menu();
        void show_start_menu();

        bool key_is_down(const int key_code);
        bool left_arrow_key_is_down();
        bool right_arrow_key_is_down();
        bool up_arrow_key_is_down();
        bool down_arrow_key_is_down();
        bool escape_key_is_down();
        bool r_key_is_down();

};

void place_on_screen_without_overlap(entity& e_to_place, const entity& e_to_avoid, const opengl_screen& screen);

#endif