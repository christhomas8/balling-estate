#include <SDL.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <SDL_image.h>
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

        Logger log;
        log.log_event("Initialized");
        
        window = SDL_CreateWindow("Balling", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, width, height,SDL_WINDOW_OPENGL);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        
        surface = IMG_Load("../grass.png");
        surface_hole = IMG_Load("../hole.png");

        if (surface == NULL)
        {
            std::cout << IMG_GetError() << std::endl;
        }
        if (surface_hole == NULL)
        {
            std::cout << IMG_GetError() << std::endl;
        }
        texture = SDL_CreateTextureFromSurface(renderer,surface);
        texture_hole = SDL_CreateTextureFromSurface(renderer,surface_hole);

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

    void draw_circle(int center_x, int center_y, int radius_)
    {
        //Setting the color to be red with 100% opaque (0% transparent)
        SDL_SetRenderDrawColor(renderer,255,0,0,0);

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

    int random_number(int min, int max)
    {
        //int num;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(min,max);

        int num = dis(gen);
        std::cout << num << std::endl;
        return num;
    }

    void render_hole()
    {
        int start_point = random_number(hole_size,width-hole_size);
    }

    void run_game(int width, int height)
    {
        SDL_Event event;
        ball_x = width/2;
        ball_y = height/2;
        ball_radius = 30;

        hole_pos.w = hole_size;
        hole_pos.h = hole_size;
        hole_pos.x = width/2;
        hole_pos.y = 0;
        
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
                                ball_xvel = -5;
                                break;
                            case SDLK_RIGHT:
                                ball_xvel = 5;
                                break;
                            case SDLK_UP:
                                ball_yvel = -5;
                                break;
                            case SDLK_DOWN:
                                ball_yvel = 5;
                                break;
                            case SDLK_t:
                                toggle_light = !toggle_light;
                                break;
                            case SDLK_q:
                                std::cout << "Quitting.." << std::endl;
                                return;
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
                                }
                                break;
                            case SDLK_RIGHT:
                                if (ball_xvel > 0)
                                {
                                    ball_xvel = 0;
                                }
                                break;
                            case SDLK_UP:
                                if (ball_yvel < 0)
                                {
                                    ball_yvel = 0;
                                }
                                break;
                            case SDLK_DOWN:
                                if (ball_yvel > 0)
                                {
                                    ball_yvel = 0;
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
            

            //check light or dark mode setting
            light_mode();


            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer,texture, NULL, NULL);

            hole_pos.y = curr_line_height;


            SDL_RenderCopy(renderer,texture_hole, NULL, &hole_pos);
 
            draw_circle(ball_x, ball_y, ball_radius);

            curr_line_height += stage_vel;

            if (curr_line_height > height)
            {
                curr_line_height = 0;
            }
            draw_line(width/4, curr_line_height, width*3/4, curr_line_height);
            




            SDL_RenderPresent(renderer);
        }
    }

private:
    int height;     // Height of the window
    int width;      // Width of the window
    SDL_Renderer *renderer = NULL;      // Pointer for the renderer
    SDL_Window *window = NULL;      // Pointer for the window

    SDL_Surface *surface = NULL; //pointer for surface
    SDL_Texture *texture = NULL; //pointer for texture/image

    SDL_Surface *surface_hole = NULL;
    SDL_Texture *texture_hole = NULL;

    SDL_Rect hole_pos;
    int hole_size = 150;

    int ball_x;
    int ball_y;
    int ball_xvel = 0;
    int ball_yvel = 0;
    int ball_radius;
    int stage_vel = 1;
    int curr_line_height = 0;
    bool toggle_light = false;

    //Light mode colors
    int al_bg = 255;
    int bl_bg= 255;
    int cl_bg = 255;

    //Dark mode colors
    int ad_bg = 0;
    int bd_bg = 0;
    int cd_bg = 0;
};

#endif