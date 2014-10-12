extern "C"
{
	#include <os.h>
	#include "SDL/SDL.h"
}

#include "LevelEditor.h"

#define SCREEN_BIT_DEPTH        (16)
#define SCREEN_VIDEO_MODE (SDL_SWSURFACE|SDL_FULLSCREEN|SDL_HWPALETTE)

int main(int argc, char *argv[]) 
{
	int nWidth = 0, nHeight = 0;
	if( 0 == show_2numeric_input("Dimesions", "Specify width and height", "Width", &nWidth, 4, 40, "Height", &nHeight, 4, 40))
		return 0;
        int nMoves = 5;
        if( 0 == show_1numeric_input("Moves", "Specify number of moves", "Moves", &nMoves, 1, 99) )
		return 0;

	//Wait for no keypressed
	while(any_key_pressed()){}

	printf("Initializing SDL.\n");
	
	/* Initialize the SDL library (starts the event loop) */
    if ( SDL_Init(SDL_INIT_VIDEO ) < 0 )
	{
        fprintf(stderr,
                "Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
	
	printf("SDL initialized.\n");
	
	SDL_Surface* pScreen = NULL;
	pScreen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BIT_DEPTH, SCREEN_VIDEO_MODE);

	if( pScreen == NULL )
	{
		fprintf(stderr, "Couldn't set %dx%dx%d video mode: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BIT_DEPTH, SDL_GetError());
		exit(1);
	}
	else
	{
		/* Hides the cursor */
		SDL_ShowCursor(SDL_DISABLE);

		//while(true)
		{
			SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

			LevelEditor le(pScreen, nWidth, nHeight, nMoves);
			/* Game loop */
			while(le.Loop()){}
		}
	}
	
	printf("Quitting SDL.\n");
    
    /* Shutdown all subsystems */
    SDL_Quit();

    printf("Quitting...\n");

   refresh_osscr();//Might have created a file
	return 0;
}
