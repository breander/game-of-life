#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <vector>
#include <list>
#include <math.h>
#include <fstream>
#include <strstream>
#include <algorithm>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

#define KEY_SEEN 1
#define KEY_RELEASED 2

// defaults
int sWidth = 1920;
int sHeight = 1080;
int blockSize = 8;
float setFps = 60.0;
bool debug = false;
int rate = 4;

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        for (int x = 1; x < argc; x++)
        {
            //Debug
            if (strcmp(argv[x], "-debug") == 0)
            {
                debug = true;
            }

            if (strcmp(argv[x], "-width") == 0)
            {
                sWidth = std::stoi(argv[x + 1]);
            }

            if (strcmp(argv[x], "-height") == 0)
            {
                sHeight = std::stoi(argv[x + 1]);
            }

            if (strcmp(argv[x], "-fps") == 0)
            {
                setFps = std::stoi(argv[x + 1]);
            }
        }
    }

    if (!al_init())
    {
        printf("couldn't initialize allegro\n");
        return 1;
    }

    if (!al_install_keyboard())
    {
        printf("couldn't initialize keyboard\n");
        return 1;
    }

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / setFps);
    if (!timer)
    {
        printf("couldn't initialize timer\n");
        return 1;
    }

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    if (!queue)
    {
        printf("couldn't initialize queue\n");
        return 1;
    }

    //al_set_new_display_option(ALLEGRO_VSYNC, 2, ALLEGRO_SUGGEST);
    //al_set_new_display_refresh_rate((int)setFps);
    ALLEGRO_DISPLAY *disp = al_create_display(sWidth, sHeight);
    if (!disp)
    {
        printf("couldn't initialize display\n");
        return 1;
    }

    al_init_font_addon();
    al_init_ttf_addon();
    ALLEGRO_FONT *font = al_load_ttf_font("OpenSans-Regular.ttf", 32, 0); //al_create_builtin_font();
    if (!font)
    {
        printf("couldn't initialize font\n");
        return 1;
    }

    if (!al_init_primitives_addon())
    {
        printf("couldn't initialize primitives\n");
        return 1;
    }

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));

    int size = (sWidth/blockSize) * (sHeight/blockSize);
    bool board [size];
    for(int x = 0; x < size ; x++){
        board[x] = false;
    }

    srand(time(0));

    for(int x = 0; x < size ; x++){
        board[x] = rand() % 2;
    }

    al_start_timer(timer);
    double old_time = al_get_time();
    while (1)
    {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            if (key[ALLEGRO_KEY_ENTER]){
                for(int x = 0; x < size ; x++){
                    board[x] = rand() % 2;
                }
            }

            if (key[ALLEGRO_KEY_ESCAPE])
                done = true;

            for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                key[i] &= KEY_SEEN;

            redraw = true;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            break;
        case ALLEGRO_EVENT_KEY_UP:
            key[event.keyboard.keycode] &= KEY_RELEASED;
            break;
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }

        if (done)
            break;

        if (redraw && al_is_event_queue_empty(queue))
        {
            // get the fps
            double new_time = al_get_time();
            double elapsed_time = new_time - old_time;
            float fps = 1.0f / elapsed_time;
            old_time = new_time;

            // black the screen
            al_clear_to_color(al_map_rgb(0, 0, 0));

            int newBoard [size];
            for(int x = 0; x < size ; x++){
                newBoard[x] = false;
            }

            int width = sWidth / blockSize;
            
            for(int x = 0; x < size ; x++){
                int liveNeighbors = 0;

                // NORTH
                if(x - width > 0 && board[x - width])
                    liveNeighbors++;
                // NORTH EAST
                if(x - (width + 1) > 0 && (x - (width + 1) % width != 0) && board[x - width + 1])
                    liveNeighbors++;
                // EAST
                if((x + 1) % width != 0 && board[x + 1])
                    liveNeighbors++;
                // SOUTH EAST
                if((x + 1) % width != 0 && (x + width + 1) < size && board[x + 1 + width])
                    liveNeighbors++;
                // SOUTH
                if(x + width < size && board[x + width])
                    liveNeighbors++;
                // SOUTH WEST
                if(x + width - 1 < size && x % width != 0 && board[x + width - 1])
                    liveNeighbors++;
                // WEST
                if(x % width != 0 && board[x - 1])
                    liveNeighbors++;
                // NORTH WEST
                if(x % width != 0 && (x - width - 1) >= 0 && board[x - 1 - width])
                    liveNeighbors++;

                if(board[x] && (liveNeighbors == 2 || liveNeighbors == 3))
                    newBoard[x] = true;

                if(board[x] == false && liveNeighbors == 3)
                    newBoard[x] = true;
            }
            
            for(int pos = 0; pos < size; pos++){
                if(newBoard[pos]){
                    int y = (pos/width) * blockSize;
                    int x = (pos * blockSize) - (y * width);
                    
                    ALLEGRO_COLOR color = al_map_rgb(255, 255, 255);

                    if(board[pos] != newBoard[pos])
                        color = al_map_rgb(255, 0, 0);

                    al_draw_filled_rectangle(x, y, x+blockSize, y+blockSize, color);
                }
            }

            for(int x = 0; x < size ; x++){
                board[x] = newBoard[x];
            }

            // draw the fps
            al_draw_textf(font, al_map_rgb(0, 0, 255), 0, 0, 0, "Fps: %i", (int)fps);

            al_flip_display();
            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_shutdown_primitives_addon();
    al_shutdown_font_addon();
    al_shutdown_ttf_addon();
    al_destroy_event_queue(queue);

    return 0;
}