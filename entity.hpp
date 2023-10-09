#ifndef MY_ENTITY_HEADER_2_OCT_2023
#define MY_ENTITY_HEADER_2_OCT_2023

class entity{

    public:

        float x_posn;
        float y_posn;
        float speed; // in pixels per second
        int x_size; // in pixels
        int y_size; // in pixels
        float r;
        float g;
        float b;

        ~entity();
        entity(const float x_in, const float y_in, const float speed_in, const int x_size_in, const int y_size_in, const float r_in, const float g_in, const float b_in);
        void update_position(const float x_time_interval, const float y_time_interval);

};

bool sprites_overlap(const entity& e1, const entity& e2);

#endif