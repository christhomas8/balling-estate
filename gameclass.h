#include <SDL.h>
#include <SDL_image.h>

#include <cmath>
#include <vector>
#include <iostream>
#include <string>
#include <random>
#include <bits/stdc++.h>

#include "logger.h"

#ifndef GAMECLASS_H
#define GAMECLASS_H

class Balling
{
public:
    // Contructor which initialize the parameters.
    Balling(int height_, int width_): height(height_), width(width_)
    {
        SDL_Init(SDL_INIT_VIDEO);       // Initializing SDL as Video
        IMG_Init(IMG_INIT_PNG);         // Enable images

        //Logger log;
        log.log_event("Initialized");
        
        window = SDL_CreateWindow("Balling", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, width, height,SDL_WINDOW_OPENGL);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        
        //surface = IMG_Load("../grass.png");
        surface = IMG_Load("../images/road.png");
        surface_redcar = IMG_Load("../images/redcar.png");
        surface_bluecar = IMG_Load("../images/bluecar.png");
        surface_orangecar = IMG_Load("../images/orangecar.png");
        surface_player = IMG_Load("../images/redcar.png");


        if (surface == NULL || surface_redcar == NULL
                || surface_bluecar == NULL || surface_redcar == NULL)
        {
            log.log_event(IMG_GetError());
            std::cout << IMG_GetError() << std::endl;
        }
        texture = SDL_CreateTextureFromSurface(renderer,surface);
        texture_redcar = SDL_CreateTextureFromSurface(renderer,surface_redcar);
        texture_bluecar = SDL_CreateTextureFromSurface(renderer,surface_bluecar);
        texture_orangecar = SDL_CreateTextureFromSurface(renderer,surface_orangecar);
        texture_player = SDL_CreateTextureFromSurface(renderer, surface_player);

        SDL_SetWindowIcon(window, surface_redcar);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);      // setting draw color
        SDL_RenderClear(renderer);      // Clear the newly created window
        SDL_RenderPresent(renderer);    // Reflects the changes done in the window.
    }

    // Destructor
    ~Balling()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
    }

    void draw_line(int x1, int y1, int x2, int y2)
    {
        SDL_SetRenderDrawColor(renderer,0,0,0,0);
        SDL_RenderDrawLine(renderer,x1,y1,x2,y2);
    }

    //rectangle
    void draw_thick_lines()
    {
        SDL_SetRenderDrawColor(renderer,255,255,255,0);
        SDL_RenderDrawRect(renderer,&lines);
        SDL_RenderFillRect(renderer, &lines);
    }

    void draw_circle(int center_x, int center_y, int radius_)
    {
        //Setting the color to be red with 100% opaque (0% transparent)
        if (ballHit > 0)
        {
            SDL_SetRenderDrawColor(renderer,0,0,255,0);
            ballHit = 0;
        }
        else
        {
            SDL_SetRenderDrawColor(renderer,255,0,0,0);
        }

        //Drawing circle
        for (int x=center_x-radius_; x<=center_x+radius_; x++)
        {
            for (int y=center_y-radius_; y<=center_y+radius_; y++)
            {
                if ((std::pow(center_y-y,2)+std::pow(center_x-x,2)) <= std::pow(radius_,2))
                {
                    SDL_RenderDrawPoint(renderer,x,y);
                }
            }
        }
    }

    void light_mode()
    {
        if (toggle_light)
        {
            SDL_SetRenderDrawColor(renderer,ad_bg,bd_bg,cd_bg,0);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer,al_bg,bl_bg,cl_bg,0);
        }
    }

    void sub_lives()
    {
        lives -= 1;
        if (lives > 0)
        {
            //pause_game();
            ball_x = width - (2*car_size + car_pos.x);
        }
        else
        {
            game_over();
        }
    }

    int random_number(int min, int max)
    {
        //int num;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(min,max);

        int num = dis(gen);
        return num;
    }

    void render_hole()
    {
        hole_pos.y += 2;
        if (hole_pos.y + hole_size > height + hole_size)
        {
            hole_pos.y = -hole_size;
            hole_pos.x = random_number(0, width-hole_size);
        }
        
    }

    void render_car()
    {
        car_pos.y += 1;
        if (car_pos.y + car_size > height + car_size)
        {
            selection = random_number(1,3);
            car_pos.y = -car_size;
            car_pos.x = random_number(0, width-car_size);
        }

        switch (selection)
        {
            case 1:
                SDL_RenderCopy(renderer,texture_redcar, NULL, &car_pos);
                break;
            case 2:
                SDL_RenderCopy(renderer,texture_bluecar, NULL, &car_pos);
                break;
            case 3:
                SDL_RenderCopy(renderer,texture_orangecar, NULL, &car_pos);
                break;
            default:
                SDL_RenderCopy(renderer,texture_redcar, NULL, &car_pos);
                break;
        }
    }

    void render_player()
    {
        player_pos.h = player_size;
        player_pos.w = player_size;
        SDL_RenderCopy(renderer,texture_player, NULL, &player_pos);
        
    }

    void ball_control()
    {
        //Ball control and screen position
        if (ball_x >= ball_radius && ball_x <= width - ball_radius)
        {
            ball_x += ball_xvel;
        }
        else if (ball_x > width - ball_radius)
        {
            ball_x = width - ball_radius - 2;
        }
        else if (ball_x < ball_radius)
        {
            ball_x = ball_radius + 2; 
        }
        if (ball_y >= ball_radius && ball_y <= height - ball_radius)
        {
            ball_y += ball_yvel;
        }
        else if (ball_y > height - ball_radius)
        {
            ball_y = height - ball_radius - 2;
        }
        else if (ball_y < ball_radius)
        {
            ball_y = ball_radius + 2;
        }
    }

    void player_control()
    {
        if (player_pos.x >= 0 && player_pos.x <= width - player_size)
        {
            player_pos.x += player_xvel;
        }
        else if (player_pos.x > width - player_size)
        {
            player_pos.x = width - player_size - 2;
        }
        else if (player_pos.x < 0)
        {
            player_pos.x = 0 + 2; 
        }
        if (player_pos.y >= player_size && player_pos.y <= height - player_size)
        {
            player_pos.y += player_yvel;
        }
        else if (player_pos.y > height - player_size)
        {
            player_pos.y = height - player_size - 2;
        }
        else if (player_pos.y < player_size)
        {
            player_pos.y = player_size + 2;
        }
    }

    void pause_game()
    {
        SDL_Delay(2000);
    }

    void check_crash()
    {
        if (    ball_x + ball_radius > car_pos.x && ball_x - ball_radius < car_pos.x + car_size 
             && ball_y + ball_radius > car_pos.y && ball_y < car_pos.y + car_size)
        {
            ballHit += 1;
            sub_lives();

            if (lives < 1)
            {
                continue_game = true;
            }
        }
    }

    void impact()
    {
        if ((player_pos.x > car_pos.x - car_size && player_pos.x < car_pos.x + car_size) 
            && (player_pos.y > car_pos.y - car_size && player_pos.y < car_pos.y + car_size))
        {
            playerHit += 1;
            sub_lives();

            if (lives < 1)
            {
                continue_game = true;
            }

            //
            if (car_pos.x > width/2)
            {
                player_pos.x = player_size;
            }
            else if (car_pos.x < width/2 && car_pos.x + car_size >= width/2)
            {
                player_pos.x = width - (player_size);
            }
            else if (car_pos.x < width/2 && car_pos.x + car_size  < width/2)
            {
                player_pos.x = width - (2 * player_size);
            }
        }
    }

    void game_over()
    {
        endgame = true;
    }

    void initials()
    {
        ball_x = width/2;
        ball_y = height - height/4;
        ball_radius = 30;

        hole_pos.w = hole_size;
        hole_pos.h = hole_size;
        hole_pos.x = random_number(0,(width-hole_size));
        hole_pos.y = (-5) * hole_size;

        car_pos.h = car_size;
        car_pos.w = car_size;
        car_pos.x = random_number(0,(width-car_size));
        car_pos.y = (-6) * car_size;

        lines.w = 20;
        lines.h = 275;
        lines.y = 200;
        lines.x = width/2 - lines.w/2;

        player_pos.x = width/2;
        player_pos.y = height - height/4; 
    }

    void run_game(int width, int height)
    {
        SDL_Event event;
        initials();
        
        while (!(event.type == SDL_QUIT))
        {
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                    case SDL_KEYDOWN:
                        switch (event.key.keysym.sym)
                        {
                            case SDLK_LEFT:
                                ball_xvel = -3;
                                player_xvel = -2;
                                break;
                            case SDLK_RIGHT:
                                ball_xvel = 3;
                                player_xvel = 2;
                                break;
                            case SDLK_UP:
                                ball_yvel = -3;
                                player_yvel = -3;
                                break;
                            case SDLK_DOWN:
                                ball_yvel = 3;
                                player_yvel = 2;
                                break;
                            case SDLK_t:
                                toggle_light = !toggle_light;
                                break;
                            case SDLK_q:
                                log.log_event("Quit button pressed");
                                return;
                            case SDLK_p:
                                paused = true;
                                log.log_event("Paused");
                                paused = false;
                                break;
                            case SDLK_y:
                                if (continue_game == true)
                                {
                                    continue_game = false;
                                    lives = 3;
                                    ball_x = width/2;
                                    ball_y = height - height/4;
                                    car_pos.x = random_number(0,(width-car_size));
                                    car_pos.y = (-6) * car_size;
                                }
                                else 
                                {
                                    return;
                                }
                                break;
                            default:
                                break;
                        }
                        break;
                    case SDL_KEYUP:
                        switch(event.key.keysym.sym)
                        {
                            case SDLK_LEFT:
                                if (ball_xvel < 0)
                                {
                                    ball_xvel = 0;
                                    player_xvel = 0;
                                }
                                break;
                            case SDLK_RIGHT:
                                if (ball_xvel > 0)
                                {
                                    ball_xvel = 0;
                                    player_xvel = 0;
                                }
                                break;
                            case SDLK_UP:
                                if (ball_yvel < 0)
                                {
                                    ball_yvel = 0;
                                    player_yvel = 0;
                                }
                                break;
                            case SDLK_DOWN:
                                if (ball_yvel > 0)
                                {
                                    ball_yvel = 0;
                                    player_yvel = 0;
                                }
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
            }

            //check light or dark mode setting
            light_mode();

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer,texture, NULL, NULL);

            //ball_control(); 
            player_control();  

            //road lines
            draw_thick_lines();
            lines.y += stage_vel;
            if ( lines.y > height)
            {
                lines.y = -lines.h;
            }

            if (continue_game != true) 
            {
                render_car();

                curr_line_height += stage_vel;

                if (curr_line_height > height)
                {
                    curr_line_height = 0;
                }
                
                //Game piece
                render_player();

                impact();
            }

            SDL_RenderPresent(renderer);
        }
    }

private:
    int height;     // Height of the window
    int width;      // Width of the window
    SDL_Renderer *renderer = NULL;      // Pointer for the renderer
    SDL_Window *window = NULL;      // Pointer for the window

    //Background
    SDL_Surface *surface = NULL; //pointer for surface
    SDL_Texture *texture = NULL; //pointer for texture/image

    SDL_Surface *surface_player = NULL;
    SDL_Texture *texture_player = NULL;

    SDL_Surface *surface_redcar = NULL;
    SDL_Texture *texture_redcar = NULL;
    SDL_Surface *surface_bluecar = NULL;
    SDL_Texture *texture_bluecar = NULL;
    SDL_Surface *surface_orangecar = NULL;
    SDL_Texture *texture_orangecar = NULL;

    SDL_Rect car_pos;
    int car_size = 175;
    SDL_Rect hole_pos;
    int hole_size = 175;
    SDL_Rect lines;
    int line_size = 20;
    SDL_Rect player_pos;
    int player_size = 175;

    int player_x;
    int player_y;
    int player_xvel = 0;
    int player_yvel = 0;
    int player_radius;//might delete
    int playerHit = 0;

    //Game States
    int lives = 5;
    bool endgame = false;
    bool pause = false;
    //bool continue_game = false;

    int ball_x;
    int ball_y;
    int ball_xvel = 0;
    int ball_yvel = 0;
    int ball_radius;
    int ballHit = 0;

    int stage_vel = 2;
    int curr_line_height = 0;
    int selection;
    bool toggle_light = false;
    bool paused = false;
    char *p;

    //Light mode colors
    int al_bg = 255;
    int bl_bg= 255;
    int cl_bg = 255;

    //Dark mode colors
    int ad_bg = 0;
    int bd_bg = 0;
    int cd_bg = 0;

    Logger log;

public:
    bool continue_game = false; 
};

#endif