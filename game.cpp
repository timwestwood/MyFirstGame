#include "opengl_screen.hpp"
#include "entity.hpp"
#include <chrono>
#include <cmath>
#include <fstream>

#define BEST_SCORE_FILE_NAME ".my_best_score"

int main(){

    opengl_screen screen(1000, 500, "My First Game");

    screen.show_start_menu();
    while (true){
        if (screen.r_key_is_down()){ // Press [R] to start the game.
            break;
        }
    }

    bool retry = false;

    do {

        entity player(0, 0, 200.0f, 50, 50, 0.5f, 0.5f, 0.5f);
        screen.confine_entity(player);

        entity target(0, 0, 0.0f, 20, 20, 0.0f, 1.0f, 0.0f);
        screen.confine_entity(target);
        place_on_screen_without_overlap(target, player, screen);

        entity enemy(0, 0, 100.0f, 50, 50, 1.0f, 0.0f, 0.0f);
        screen.confine_entity(enemy);
        place_on_screen_without_overlap(enemy, player, screen);

        bool running = true;

        int score = 0;

        // Check for a saved high score.
        int best_score;
        std::ifstream best_score_input_file(BEST_SCORE_FILE_NAME);
        if (best_score_input_file.good()){
            best_score_input_file >> best_score;
            best_score_input_file.close();
        } else {
            best_score = 0;
        }

        std::chrono::steady_clock::time_point time1 = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point time2 = std::chrono::steady_clock::now();

        while(running){

            // If we're not exiting, find the elapsed time interval.
            time2 = std::chrono::steady_clock::now();
            float time_interval = std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count()/1e6f;
            time1 = time2;

            // Check if we're being asked to move the player.
            float x_shift = 0, y_shift = 0;
            if (screen.left_arrow_key_is_down()){
                x_shift -= 1;
            }
            if (screen.right_arrow_key_is_down()){
                x_shift += 1;
            }
            if (screen.up_arrow_key_is_down()){
                y_shift += 1;
            }
            if (screen.down_arrow_key_is_down()){
                y_shift -= 1;
            }

            // We don't want to move faster diagonally...
            float move_size = std::sqrt(y_shift*y_shift + x_shift*x_shift); // Can only actually be 0, 1 or sqrt(2).
            if (move_size > 1.0f){
                x_shift /= move_size;
                y_shift /= move_size;
            }
            // N.B. This scaling causes the player to move more slowly parallel to a wall they're in contact with if they
            // are attempting to move into it than if they're only pressing a 'wall parallel' arrow key, which I think is a
            // nice friction feature.

            player.update_position(time_interval*x_shift, time_interval*y_shift);
            screen.confine_entity(player);

            // Move the enemy towards the player.
            x_shift = player.x_posn - enemy.x_posn;
            y_shift = player.y_posn - enemy.y_posn;
            move_size = std::sqrt(y_shift*y_shift + x_shift*x_shift) + 1e-10f;
            x_shift /= move_size;
            y_shift /= move_size;
            enemy.update_position(time_interval*x_shift, time_interval*y_shift);
            screen.confine_entity(enemy);

            // Check if the player has caught the target.
            if (sprites_overlap(player, target)){

                place_on_screen_without_overlap(target, player, screen);
                score++;

            }

            // Check if the enemy has caught the player.
            if (sprites_overlap(player, enemy)){
                
                screen.show_death_screen();

                while (true){

                    if (screen.escape_key_is_down()){
                        running = false;
                        retry = false;
                        break;
                    } else if (screen.r_key_is_down()){
                        retry = true;
                        break;
                    }

                }

                break;

            }

            // Re-draw the scene.
            screen.clear();
            screen.draw_sprite(target);
            screen.draw_sprite(enemy);
            screen.draw_sprite(player);
            screen.print_score_line(score, best_score);
            screen.display();

            // Check if we should pause the game.
            if (screen.escape_key_is_down()){

                screen.show_pause_menu();

                while (screen.escape_key_is_down()){} // Don't try to detect the in-menu keystrokes until [Esc] has been released or we'll instantly close the game.

                while (true){

                    if (screen.escape_key_is_down()){
                        running = false;
                        retry = false;
                        break;
                    } else if (screen.r_key_is_down()){
                        time1 = std::chrono::steady_clock::now(); // Otherwise the enemy is effectively moving while the game is "paused".
                        break;
                    }

                }

            }

        }

        if (score > best_score){

            std::ofstream best_score_output_file(BEST_SCORE_FILE_NAME);
            best_score_output_file << score;
            best_score_output_file.close();

        }

    } while (retry);

    return 0;

}