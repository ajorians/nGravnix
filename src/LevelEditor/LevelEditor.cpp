#include "LevelEditor.h"
#include "GameBackground.h"

extern "C"
{
#include "SDL/SDL_gfxPrimitives.h"
}

LevelEditor::LevelEditor(SDL_Surface* pScreen, int nWidth, int nHeight, int nMoves)
: m_pScreen(pScreen), m_Pieces(pScreen, &m_BoardMetrics, &m_Selector), m_Selector(pScreen, &m_BoardMetrics)
{
	printf("Width: %d, Height: %d\n", nWidth, nHeight);
	GravnixLibLevelCreate(&m_Gravnix, nWidth, nHeight, nMoves);

	m_BoardMetrics.SetDimensions(nWidth, nHeight);

	m_pBackground = nSDL_LoadImage(image_PuzzleFrenzyGameBackground);
	m_pFont = nSDL_LoadFont(NSDL_FONT_THIN, 0/*R*/, 0/*G*/, 0/*B*/);

        for(int nX=0; nX<nWidth; nX++) {
                for(int nY=0; nY<nHeight; nY++) {
			int nSpot = GetGravnixSpotValue(m_Gravnix, nX, nY), nBoard = GetGravnixBoardValue(m_Gravnix, nX, nY);
			if( nSpot <= NO_PIECE && nBoard <= NO_PIECE ) continue;

			int nValue = nSpot > NO_PIECE ? nSpot : nBoard;
			m_Pieces.CreatePiece(nValue, nX, nY);
                }
        }
}

LevelEditor::~LevelEditor()
{
	GravnixLibFree(&m_Gravnix);
	SDL_FreeSurface(m_pBackground);
	nSDL_FreeFont(m_pFont);
}

bool LevelEditor::Loop()
{
	//Handle keypresses
	if( PollEvents() == false )
		return false;

	//Check for state changes
	if( CheckStateConditions() == false )
		return false;

	//Update screen
	UpdateDisplay();
	
	SDL_Delay(30);
	
	return true;
}

bool LevelEditor::CheckStateConditions()
{
   if( !m_Pieces.IsAnimating() ) {
   }

   return true;
}

bool LevelEditor::PollEvents()
{
	SDL_Event event;
	
	/* Poll for events. SDL_PollEvent() returns 0 when there are no  */
	/* more events on the event queue, our while loop will exit when */
	/* that occurs.                                                  */
	while( SDL_PollEvent( &event ) )
	{
		/* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
		switch( event.type )
		{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) 
				{
					case SDLK_ESCAPE:
						fprintf(stderr, "Hit Escape!n");
					return false;
					break;

					 case SDLK_RETURN:
						SetCurrentPiece();
					break;

                                        case SDLK_LCTRL:
                                        case SDLK_RCTRL:
                                                PrintLevel();
                                        break;

					case SDLK_UP:
                                        case SDLK_8:
                                                Move(Up);
        	                                break;
                                        
                                        case SDLK_DOWN:
                                        case SDLK_2:
                                                Move(Down);
	                                        break;

					case SDLK_RIGHT:
					case SDLK_6:
						Move(Right);
						break;
					
					case SDLK_LEFT:
					case SDLK_4:
						Move(Left);
						break;

					/*case SDLK_PLUS:
						Redo();
						break;

					case SDLK_MINUS:
						Undo();
						break;*/

					default:
						break;
				}
				break;
			
			//Called when the mouse moves
			case SDL_MOUSEMOTION:
				break;
			
			case SDL_KEYUP:
				break;
			
			default:
				break;
		}
	}
	return true;
}

void LevelEditor::UpdateDisplay()
{
	//Draw background
	if( !is_classic ) {
		SDL_BlitSurface(m_pBackground, NULL, m_pScreen, NULL);
	} else {
		SDL_FillRect(m_pScreen, NULL, SDL_MapRGB(m_pScreen->format, 255, 255, 255));
	}

	boxRGBA(m_pScreen, m_BoardMetrics.GetLeft(), m_BoardMetrics.GetTop(), m_BoardMetrics.GetRight(), m_BoardMetrics.GetBottom(),  GAME_BACKGROUND_R, GAME_BACKGROUND_G, GAME_BACKGROUND_B, 230);

	if( !m_Pieces.Animate() )
	{
		SDL_UpdateRect(m_pScreen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}
}

void LevelEditor::SetCurrentPiece()
{
   int nX = m_Selector.GetCurrentX(), nY = m_Selector.GetCurrentY();
   int nBoardValue = GetGravnixBoardValue(m_Gravnix, nX, nY), nSpotValue = GetGravnixSpotValue(m_Gravnix, nX, nY);
   int nValue = nBoardValue > NO_PIECE ? nBoardValue : nSpotValue;
   nValue++;
   if( nValue > STICKY_BLOCK_VALUE )
      nValue = NO_PIECE;
   m_Pieces.DisappearPiece(nX, nY);
   SetGravnixBoardValue(m_Gravnix, nX, nY, NO_PIECE);
   SetGravnixSpotValue(m_Gravnix, nX, nY, NO_PIECE);
   if( (nValue >= RED_BLOCK_VALUE && nValue <= RAINBOW_BLOCK_VALUE) || nValue == BLACKHOLE_BLOCK_VALUE ) {
      SetGravnixSpotValue(m_Gravnix, nX, nY, nValue);
   }
   else {
      SetGravnixBoardValue(m_Gravnix, nX, nY, nValue); 
   }
   if( nValue > NO_PIECE )
      m_Pieces.CreatePiece(nValue, nX, nY);
}

void LevelEditor::Move(Direction eDirection)
{
   m_Selector.Move(eDirection);
}

void LevelEditor::PrintLevel()
{
   char buffer[5000];
   GravnixSave(m_Gravnix, buffer);
   int n = strlen(buffer);
   int nPos = 0;
   while(nPos < n) {
      char* buf = (char*)malloc(101);
      memset(buf, 0, 100);
      int nSize = ((n-nPos) > 100) ? 100 : (n-nPos);
      memcpy(buf, buffer+nPos, nSize);
      buf[nSize] = '\0';
      nPos += nSize;
      printf("%s\n", buf);
      free(buf);
   }

   FILE *fp = fopen("output.gravnix.tns", "w");
   if (fp) {
      fwrite(buffer, strlen(buffer), 1, fp);
   }
   fclose(fp);
}


