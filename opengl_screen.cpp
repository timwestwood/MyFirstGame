#include "opengl_screen.hpp"
#include "entity.hpp"
#include <algorithm>
#include <random>
#include <string.h>

bool opengl_screen::key_is_down(const int key_code) { glfwPollEvents(); return glfwGetKey(window, key_code) == GLFW_PRESS; }


bool opengl_screen::left_arrow_key_is_down(){ return key_is_down(GLFW_KEY_LEFT); }
bool opengl_screen::right_arrow_key_is_down(){ return key_is_down(GLFW_KEY_RIGHT); }
bool opengl_screen::up_arrow_key_is_down(){ return key_is_down(GLFW_KEY_UP); }
bool opengl_screen::down_arrow_key_is_down(){ return key_is_down(GLFW_KEY_DOWN); }
bool opengl_screen::escape_key_is_down(){ return key_is_down(GLFW_KEY_ESCAPE); }
bool opengl_screen::r_key_is_down(){ return key_is_down(GLFW_KEY_R); }

opengl_screen::~opengl_screen(){

    glfwSetWindowShouldClose(window, true);
    glfwDestroyWindow(window);
    glfwTerminate();

}

opengl_screen::opengl_screen(const int width_in, const int height_in, const char* title) : font("open-sans/OpenSans-Regular.ttf") { // Have to use initialiser list approach because FTGLPixmapFont has no default constructor.

    width = width_in;
    height = height_in;

    glfwInit();
    glewExperimental = true;
    glewInit();

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetWindowShouldClose(window, false);

}

void opengl_screen::display(){

    glfwSwapBuffers(window);
    glFlush();

}

void opengl_screen::clear(){

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

}

#define X_DIM_CONVERSION 0
#define Y_DIM_CONVERSION 1

float opengl_screen::pixels_to_screen_coords(const float px, const int dim_flag){

    // The relative screen coordinates range from -1 to 1.

    if (dim_flag == X_DIM_CONVERSION){
        return -1.0f + 2.0f*(px/float(width));
    } else if (dim_flag == Y_DIM_CONVERSION){
        return -1.0f + 2.0f*(px/float(height));
    }

}

void opengl_screen::vertex(const float x, const float y){

    glVertex2f(pixels_to_screen_coords(x, X_DIM_CONVERSION), pixels_to_screen_coords(y, Y_DIM_CONVERSION));

}

void opengl_screen::draw_sprite(const entity& e){

    // Filled shape
    glColor3f(e.r, e.g, e.b);
    glBegin(GL_POLYGON);
    vertex(e.x_posn, e.y_posn);
    vertex(e.x_posn+e.x_size, e.y_posn);
    vertex(e.x_posn+e.x_size, e.y_posn+e.y_size);
    vertex(e.x_posn, e.y_posn+e.y_size);
    glEnd();

    // Border
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    vertex(e.x_posn, e.y_posn);
    vertex(e.x_posn + e.x_size, e.y_posn);
    vertex(e.x_posn + e.x_size, e.y_posn + e.y_size);
    vertex(e.x_posn, e.y_posn + e.y_size);
    glEnd();
}

float keep_in_range(const float val, const float lower, const float upper){

    return std::max<float>(lower, std::min<float>(val, upper));

}

void opengl_screen::confine_entity(entity& e) const {
    
    e.x_posn = keep_in_range(e.x_posn, 0, width - e.x_size);
    e.y_posn = keep_in_range(e.y_posn, 0, height - e.y_size);
    
}


void opengl_screen::print_score_line(const int score, const int best_score){

    char message[50];
    sprintf(message, "Score: %d    Best: %d", score, best_score);

    glColor3f(0.0f, 0.0f, 0.0f);

    const int font_size = 16;
    glRasterPos2f(pixels_to_screen_coords(0, X_DIM_CONVERSION), pixels_to_screen_coords(height-font_size, Y_DIM_CONVERSION));
    font.FaceSize(font_size);
    font.Render(message);

}

void opengl_screen::show_message_in_centre_of_blank_screen(const char *message){
    
    clear();

    font.FaceSize(16);

    // Centre the message on the screen.
    const FTBBox bounding_box = font.BBox(message);
    const FTPoint bottom_left = bounding_box.Lower();
    const FTPoint top_right = bounding_box.Upper();
    const int message_height = int(std::round(top_right.Yf() - bottom_left.Yf()));
    const int message_width = int(std::round(top_right.Xf() - bottom_left.Xf()));
    const int row = (height - message_height)/2;
    const int col = (width - message_width)/2;

    glColor3f(0.0f, 0.0f, 0.0f);

    glRasterPos2f(pixels_to_screen_coords(col, X_DIM_CONVERSION), pixels_to_screen_coords(row, Y_DIM_CONVERSION));
    font.Render(message);

    display();
    
}

void opengl_screen::show_death_screen(){
    
    show_message_in_centre_of_blank_screen("You died! Press [Esc] to exit the game or [R] to retry.");
    
}

void opengl_screen::show_pause_menu(){
    
    show_message_in_centre_of_blank_screen("Game paused. Press [Esc] to exit the game or [R] to resume.");
    
}

void opengl_screen::show_start_menu(){
    
    show_message_in_centre_of_blank_screen("Using the arrow keys to move, reach the targets before you get caught! Press [R] to start.");
    
}















void place_on_screen_without_overlap(entity& e_to_place, const entity& e_to_avoid, const opengl_screen& screen){
    
    std::random_device dev;
    std::mt19937 mt(dev());

    // Limits should match those in opengl_screen::confine_entity
    std::uniform_int_distribution<> x_dist(0, screen.width - e_to_place.x_size);
    std::uniform_int_distribution<> y_dist(0, screen.height - e_to_place.y_size);

    while (sprites_overlap(e_to_place, e_to_avoid)){

        e_to_place.x_posn = x_dist(mt);
        e_to_place.y_posn = y_dist(mt);

    }
    
}
