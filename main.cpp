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


void audio_callback(void* userdata, uint8_t* stream, int len)
{
    uint64_t* samples_played = (uint64_t*)userdata;
    float* fstream = (float*)(stream);
    static const float volume = 0.2;
    static const float frequency1 = 432.0;
    static const float frequency2 = 440.0;
    static const float sample_rate = 44100.0;
    for(int sid = 0; sid < (len / 8); ++sid)
    {
        double time = (*samples_played + sid) / sample_rate;
        fstream[2 * sid + 0] = volume * sin((frequency1 * 2.0 * M_PI * time)); /* L */
        fstream[2 * sid + 1] = volume * sin((frequency2 * 2.0 * M_PI * time)); /* R */
    }

    *samples_played += (len / 8);
}

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


    SDL_AudioSpec audio_spec_want, audio_spec;
    SDL_memset(&audio_spec_want, 0, sizeof(audio_spec_want));

    audio_spec_want.freq     = 44100;
    audio_spec_want.format   = AUDIO_F32;
    audio_spec_want.channels = 2;
    audio_spec_want.samples  = 512;
    audio_spec_want.callback = audio_callback;
    audio_spec_want.userdata = (void*)&samples_played;

    SDL_AudioDeviceID audio_device_id = SDL_OpenAudioDevice(
        NULL, 0,
        &audio_spec_want, &audio_spec,
        SDL_AUDIO_ALLOW_FORMAT_CHANGE
    );

    if(!audio_device_id)
    {
        // fprintf(stderr, "Error creating SDL audio device. SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    if ( NULL == window )
    {
        std::cout << "Could not create window: " << SDL_GetError( ) << std::endl;
        return 1;
    }
    iteration(14,scale,x_offset,y_offset);
    SDL_Event event;
    
    while ( true )
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




// #include <cstdint>
// #include <SDL2/SDL.h>
// #include <math.h>

// // void cb_record(void *userdata, Uint8 *stream, int len) {
// //     //qDebug() << "...Record";
// //     //Copy audio from stream
// //     memcpy( &gRecordingBuffer[ gBufferBytePosition ], stream, len );

// //     //Move along buffer
// //     gBufferBytePosition += len;
// // }


// // void cb_playback(void *userdata, Uint8 *stream, int len) {
// //     //qDebug() << "...Playback  " << gBufferBytePosition;
// //     //Copy audio to stream
// //     memcpy( stream, &gRecordingBuffer[ gBufferBytePosition ], len );

// //     //Move along buffer
// //     gBufferBytePosition += len;
// // }

// void audio_callback(void* userdata, uint8_t* stream, int len)
// {
//     uint64_t* samples_played = (uint64_t*)userdata;
//     float* fstream = (float*)(stream);
//     static const float volume = 0.2;
//     static const float frequency1 = 432.0;
//     static const float frequency2 = 440.0;
//     static const float sample_rate = 44100.0;
//     for(int sid = 0; sid < (len / 8); ++sid)
//     {
//         double time = (*samples_played + sid) / sample_rate;
//         fstream[2 * sid + 0] = volume * sin((frequency1 * 2.0 * M_PI * time)); /* L */
//         fstream[2 * sid + 1] = volume * sin((frequency2 * 2.0 * M_PI * time)); /* R */
//     }

//     *samples_played += (len / 8);
// }

// int main(int argc, char* argv[])
// {
//     uint64_t samples_played = 0;

//     if(SDL_Init(SDL_INIT_AUDIO) < 0)
//     {
//         // fprintf(stderr, "Error initializing SDL. SDL_Error: %s\n", SDL_GetError());
//         return -1;
//     }


//     SDL_AudioSpec audio_spec_want, audio_spec;
//     SDL_memset(&audio_spec_want, 0, sizeof(audio_spec_want));

//     audio_spec_want.freq     = 44100;
//     audio_spec_want.format   = AUDIO_F32;
//     audio_spec_want.channels = 2;
//     audio_spec_want.samples  = 512;
//     audio_spec_want.callback = audio_callback;
//     audio_spec_want.userdata = (void*)&samples_played;

//     SDL_AudioDeviceID audio_device_id = SDL_OpenAudioDevice(
//         NULL, 0,
//         &audio_spec_want, &audio_spec,
//         SDL_AUDIO_ALLOW_FORMAT_CHANGE
//     );

//     if(!audio_device_id)
//     {
//         // fprintf(stderr, "Error creating SDL audio device. SDL_Error: %s\n", SDL_GetError());
//         SDL_Quit();
//         return -1;
//     }

//     int window_width  = 600;
//     int window_height = 600;
//     SDL_Window* window;
//     {
//         window = SDL_CreateWindow(
//             "SDL Tone Generator",
//             SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
//             window_width, window_height,
//             SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
//         );

//         if(!window)
//         {
//             // fprintf(stderr, "Error creating SDL window. SDL_Error: %s\n", SDL_GetError());
//             SDL_Quit();
//             return -1;
//         }
//     }

//     SDL_PauseAudioDevice(audio_device_id, 0);

//     bool running = true;
//     while(running)
//     {
//         // Process input
//         SDL_Event sdl_event;
//         while(SDL_PollEvent(&sdl_event) != 0)
//         {
//             if(sdl_event.type == SDL_QUIT)
//                 running = false;
//         }
//     }

//     SDL_DestroyWindow(window);
//     SDL_CloseAudioDevice(audio_device_id);
//     SDL_Quit();

//     return 0;
// }
