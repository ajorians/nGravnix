extern "C"
{
	#include <os.h>
	#include "SDL/SDL.h"
	#include "ArchiveLib/ArchiveLib.h"
}

#include "Menu.h"
#include "Game.h"
#include "Config.h"
#include "Options.h"
#include "Startup.h"
#include "Help.h"

#define SCREEN_BIT_DEPTH        (16)
#define SCREEN_VIDEO_MODE (SDL_SWSURFACE|SDL_FULLSCREEN|SDL_HWPALETTE)

int main(int argc, char *argv[]) 
{
	if (argc != 2) {
        	if (!config_file_already_written()) {
			write_config_file();
        	}
	}

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

	ArchiveSetCurrentDirectory(argv[0]);
	Config config;

	if( pScreen == NULL )
	{
		fprintf(stderr, "Couldn't set %dx%dx%d video mode: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BIT_DEPTH, SDL_GetError());
		exit(1);
	}
	else
	{
		/* Hides the cursor */
		SDL_ShowCursor(SDL_DISABLE);

		int nLevelNumber = -1;
		char strLevelData[2048];//Big enough for a 18x18 puzzle with solution

		while(true)
		{
			bool bShowHelp = false, bShowOptions = false;
			SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

			if( argc != 2 )
			{
				MainMenu menu(pScreen, &config);
				while(menu.Loop()){}
				if( menu.ShouldQuit() )
					break;
				bShowHelp = menu.ShouldShowHelp();
				bShowOptions = menu.ShowShowOptions();
				strcpy(strLevelData, menu.GetLevel());
				nLevelNumber = menu.GetLevelNumber();
			}
			else
			{
				FILE *fp = fopen(argv[1], "r");
				if (!fp) { return 0; }
				struct stat filestat;
				if (stat(argv[1],&filestat) == -1) { fclose(fp); return 0; }

				fread(strLevelData, 1, filestat.st_size, fp);

				strLevelData[filestat.st_size] = 0;

    				fclose(fp);
			}
			
			if( bShowOptions ) {
				Options ops(pScreen, &config);
				while(ops.Loop()){}
				continue;
			}
			else if( bShowHelp )
			{
				GravnixHelp help(pScreen);
				while(help.Loop()){}
				continue;
			}
			else
			{
				bool bPlay = true;
				while(bPlay)
				{
					Game game(pScreen, strLevelData, nLevelNumber, &config);
					/* Game loop */
					while(game.Loop()){}

					if( game.ShouldQuit() || argc == 2 ) {
						bPlay = false;
						break;
					}

					MainMenu menu(pScreen, &config);
					if( menu.IncrementLevel() ) {
						strcpy(strLevelData, menu.GetLevel());
		                                nLevelNumber = menu.GetLevelNumber();
					} else {
						bPlay = false;
					}
				}
			}

			if( argc == 2 )
				break;//Just play game and exit if playing a level.
		}
	}
	
	printf("Quitting SDL.\n");
    
    /* Shutdown all subsystems */
    SDL_Quit();

    printf("Quitting...\n");

	return 0;
}
