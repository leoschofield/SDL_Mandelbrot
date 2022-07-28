#include <iostream>
#include <vector>
#include <complex>
#include <SDL2/SDL.h>
#include <math.h>
using namespace std;
const long double WIDTH = 960.0, HEIGHT = 720.0;
const int X = 0, Y = 1;
const int RE_START = -2;
const int RE_END = 1;
const int IM_START = -1;
const int IM_END = 1;
    
SDL_Window *window = SDL_CreateWindow( "Mandelbrot Set", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI );
SDL_Renderer *renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);



int in_mandelbrot_set(complex<long double> c,int iter) 
{
    complex<long double> z(0, 0);
    
    for(int i = 0; i < iter; i++)
    {
        z = ( z * z + c );
    }
    // cout  << "c" << abs(c) << endl;
    //  cout << "z" <<  abs(z) << endl;
    if (abs(z) >      10000000000)
        return 5;
    else if (abs(z) > 5000)
        return 4;
    else if (abs(z) > 2000)
        return 3;        
    else if (abs(z) > 1000)
        return 2;
    else if (abs(z) > 2)
        return 0;
    else
        return 1;    
}

void iteration(int iter, int scale, int x_offset, int y_offset)
{
    
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );
    
    for(long double x = 0; x<WIDTH; x++)
    {
        for(long double y = 0; y<HEIGHT; y++)
        {
            complex<long double> c((x-WIDTH/2.0)/(WIDTH/scale)+x_offset, (y-HEIGHT/2.0)/(HEIGHT/scale)+y_offset);
        //    cout  << "c" << c << endl;
      
            if(in_mandelbrot_set(c,iter) == 2){
               SDL_SetRenderDrawColor( renderer, 255, 255, 0, 255 );
                SDL_RenderDrawPoint(renderer,x,y);  
            }
            else if(in_mandelbrot_set(c,iter) == 3){
               SDL_SetRenderDrawColor( renderer, 0, 255, 0, 255 );
                SDL_RenderDrawPoint(renderer,x,y);  
            }
            else if(in_mandelbrot_set(c,iter) == 4){
               SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
                SDL_RenderDrawPoint(renderer,x,y);  
            }
            else if(in_mandelbrot_set(c,iter) == 5){
               SDL_SetRenderDrawColor( renderer, 100, 0, 255, 255 );
                SDL_RenderDrawPoint(renderer,x,y);  
            }
            if(x == WIDTH/2){
                SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
                SDL_RenderDrawPoint(renderer,x,y);
            }
            if(y == HEIGHT/2){
                SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
                SDL_RenderDrawPoint(renderer,x,y);
            }
        }
    }

    SDL_RenderPresent( renderer );
}

int main( int argc, char *argv[] )
{
    uint64_t samples_played = 0;

    int centre[2] = {WIDTH/2 , HEIGHT/2};

    int scale = 2;
    int x_offset = 0; 
    int y_offset = 0;        

    SDL_Init( SDL_INIT_EVERYTHING );



    if ( NULL == window )
    {
        std::cout << "Could not create window: " << SDL_GetError( ) << std::endl;
        return 1;
    }
    iteration(14,scale,x_offset,y_offset);
    SDL_Event event;
    
    while ( true )r
    {
        if ( SDL_PollEvent( &event ) )
        {
            if ( SDL_QUIT == event.type )
            { break; }

            if ( SDL_MOUSEMOTION == event.type )
            {
                int x, y;
                SDL_GetMouseState( &x, &y );

                //std::cout << x << " : " << y << std::endl;
            }

            if ( SDL_MOUSEBUTTONDOWN == event.type )
            {
                if ( SDL_BUTTON_LEFT == event.button.button )
                { 
                   // std::cout << "Left mouse button is down" << std::endl; 
                }
                else if ( SDL_BUTTON_RIGHT == event.button.button )
                { 
                  //  std::cout << "Right mouse button is down" << std::endl; 
                }
                else if ( SDL_BUTTON_MIDDLE == event.button.button )
                { 
                    //std::cout << "Middle mouse button is down" << std::endl; 
                }
            }

            if ( SDL_MOUSEBUTTONUP == event.type )
            {
                if ( SDL_BUTTON_LEFT == event.button.button )
                { 
                  //  std::cout << "Left mouse button is up" << std::endl; 
                }
                else if ( SDL_BUTTON_RIGHT == event.button.button )
                { 
                  //  std::cout << "Right mouse button is up" << std::endl; 
                }
                else if ( SDL_BUTTON_MIDDLE == event.button.button )
                { 
                 //   std::cout << "Middle mouse button is up" << std::endl; 
                }
            }

            if ( SDL_MOUSEWHEEL == event.type )
            {
                if ( event.wheel.y > 0 ) // scroll up
                { 
                    scale -= 1;
                    iteration(14,scale,x_offset,y_offset);
                 //   std::cout << "Mouse wheel is scrolling up" << std::endl; 
                }
                else if ( event.wheel.y < 0 ) // scroll down
                { 
                    scale += 1;
                    iteration(14,scale,x_offset,y_offset);
                //    std::cout << "Mouse wheel is scrolling down" << std::endl; 
                }

                if ( event.wheel.x > 0 ) // scroll right
                { 
                //    std::cout << "Mouse wheel is scrolling right" << std::endl; 
                }
                else if ( event.wheel.x < 0 ) // scroll left
                { 
                //    std::cout << "Mouse wheel is scrolling left" << std::endl;
                }
            }

            if ( SDL_KEYDOWN == event.type )
            {
                //std::cout << "Key is down" << std::endl;

                if ( SDLK_a == event.key.keysym.sym )
                { 
                    x_offset -= 1;
                    iteration(14,scale,x_offset,y_offset);
                }
                else if ( SDLK_s == event.key.keysym.sym )
                { 
                    y_offset += 1;
                    iteration(14,scale,x_offset,y_offset);
                }
                else if ( SDLK_d == event.key.keysym.sym )
                { 
                    x_offset += 1;
                    iteration(14,scale,x_offset,y_offset);
                }
                else if ( SDLK_w == event.key.keysym.sym )
                { 
                    y_offset -= 1;
                    iteration(14,scale,x_offset,y_offset);
                }
                else if ( SDLK_q == event.key.keysym.sym )
                { 
                    break;
                }
            }
            else if ( SDL_KEYUP == event.type )
            {
                //std::cout << "Key is up" << std::endl;
                if ( SDLK_a == event.key.keysym.sym )
                { 
                   // std::cout << "A is up - Stop moving left" << std::endl; 
                }
                else if ( SDLK_s == event.key.keysym.sym )
                { 
                   // std::cout << "S is up - Stop moving down" << std::endl; 
                }
                else if ( SDLK_d == event.key.keysym.sym )
                {
                    // std::cout << "D is up - Stop moving right" << std::endl; 
                }
                else if ( SDLK_w == event.key.keysym.sym )
                { 
                }
            }
        }
    }
    
    SDL_DestroyWindow( window );
    SDL_Quit( );

    return EXIT_SUCCESS;
}
