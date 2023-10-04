#ifndef MY_ENTITY_HEADER_2_OCT_2023
#define MY_ENTITY_HEADER_2_OCT_2023

class entity{

    public:

        float x_posn;
        float y_posn;
        float speed; // in characters per second
        int x_size; // in characters
        int y_size; // in characters
        char sprite_char;

        ~entity();
        entity(const float x_in, const float y_in, const float speed_in, const int x_size_in, const int y_size_in, const char sprite_char_in);
        void update_position(const float x_time_interval, const float y_time_interval);

};

bool sprites_overlap(const entity& e1, const entity& e2);

#endif