#include "entity.hpp"
#include <algorithm>

entity::~entity(){}

entity::entity(const float x_in, const float y_in, const float speed_in, const int x_size_in, const int y_size_in, const float r_in, const float g_in, const float b_in){

    x_posn = x_in;
    y_posn = y_in;
    speed = speed_in;
    x_size = x_size_in;
    y_size = y_size_in;
    r = r_in;
    g = g_in;
    b = b_in;

}

void entity::update_position(const float x_time_interval, const float y_time_interval){

    x_posn += speed*x_time_interval;
    y_posn += speed*y_time_interval;

}









bool sprites_overlap(const entity& e1, const entity& e2){

    int e1x = (int) e1.x_posn;
    int e2x = (int) e2.x_posn;
    int e1y = (int) e1.y_posn;
    int e2y = (int) e2.y_posn;

    // Start with the possibility of horizontal overlap
    if (e1x <= e2x){

        // e1 is on the left
        if (e2x >= e1x + e1.x_size){
            return false;
        }

    } else {

        // e2 is on the left
        if (e1x >= e2x + e2.x_size){
            return false;
        }

    }

    // If we make it here then the sprites share some x-coordinates,
    // so we check for overlap in y-coordinates.
    if (e1y <= e2y){

        // e1 is below e2
        if (e2y >= e1y + e1.y_size){
            return false;
        }

    } else {

        // e2 is below e1
        if (e1y >= e2y + e2.y_size){
            return false;
        }

    }

    // If we haven't already returned, the sprites overlap in both x- and y-space;
    // i.e. the sprites overlap.
    return true;

}