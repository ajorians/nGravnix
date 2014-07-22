#include "Game.h"
#ifdef USE_GRAPHIC_YOU_WIN
#include "YouWinGraphic.h"
#endif

extern "C"
{
#include "SDL/SDL_gfxPrimitives.h"
}

Game::Game(SDL_Surface* pScreen, const char* pstrFile, int nLevelNumber, Config* pConfig)
: m_pScreen(pScreen), m_Background(pScreen, pConfig), m_StartMessage(pScreen)
#ifdef USE_GRAPHIC_YOU_WIN
, m_pWinGraphic(NULL)
#else
, m_YouWinMessage(pScreen)
#endif
, m_nLevelNumber(nLevelNumber), m_pConfig(pConfig), m_MovesRemainingLabel(pScreen, &m_BoardMetrics), m_bGameOver(false), m_Pieces(pScreen, &m_BoardMetrics, pConfig, &m_MovesRemainingLabel), m_bShouldQuit(false)
{
	GravnixLibCreate(&m_Gravnix, pstrFile);
	m_MovesRemainingLabel.SetGravnixLib(&m_Gravnix);

	int nWidth = GetGravnixBoardWidth(m_Gravnix), nHeight = GetGravnixBoardHeight(m_Gravnix);

	m_BoardMetrics.SetDimensions(nWidth, nHeight);

	m_pFont = nSDL_LoadFont(NSDL_FONT_THIN, 0/*R*/, 0/*G*/, 0/*B*/);

        for(int nX=0; nX<nWidth; nX++) {
                for(int nY=0; nY<nHeight; nY++) {
			int nType = GetSpotValue(nX, nY);
			if( nType <= 0 ) continue;

			m_Pieces.CreatePiece(nType, nX, nY);
                }
        }

	if( m_pConfig->GetShowOpeningMoveLimit() ) {
		char buffer[32];
		char buf[8];
		strcpy(buffer, "Move\nLimit: ");
		Puz_itoa(GetGravnixMoveLimit(m_Gravnix), buf, 8);
		strcat(buffer, buf);
		m_StartMessage.CreateMessage(buffer);
	}
}

Game::~Game()
{
	GravnixLibFree(&m_Gravnix);
#ifdef USE_GRAPHIC_YOU_WIN
	if( m_pWinGraphic != NULL )
		SDL_FreeSurface(m_pWinGraphic);
#endif
	nSDL_FreeFont(m_pFont);
}

bool Game::Loop()
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

bool Game::ShouldQuit()
{
   return m_bShouldQuit;
}

int Game::GetSpotValue(int nX, int nY)
{
   int nRet = GetGravnixSpotValue(m_Gravnix, nX, nY);
   if( nRet == 0 )
      nRet = GetGravnixBoardValue(m_Gravnix, nX, nY);

   return nRet;
}

Direction Game::GetSlideDirection()
{
   Direction eRet = Left;
   int nDirection = GetGravnixDirection(m_Gravnix);
   if( nDirection == GRAVNIXLIB_DIRECTION_UP )
      eRet = Up;
   else if( nDirection == GRAVNIXLIB_DIRECTION_LEFT )
      eRet = Left;
   else if( nDirection == GRAVNIXLIB_DIRECTION_RIGHT )
      eRet = Right;
   else if( nDirection == GRAVNIXLIB_DIRECTION_DOWN )
      eRet = Down;

   return eRet;
}

bool Game::CheckStateConditions()
{
   if( m_StartMessage.HasMessage() && ((!m_StartMessage.IsAnimating() && !m_StartMessage.IsStayDuration()) || m_StartMessage.FinishFast()) && !m_bGameOver ) {
      m_StartMessage.ClearMessage();
      m_Timer.Start();
   }

   if( !m_Pieces.IsAnimating() ) {
      Direction eDirection = GetSlideDirection();

      for(int nX=0; nX<GetGravnixBoardWidth(m_Gravnix); nX++) {
         for(int nY=0; nY<GetGravnixBoardHeight(m_Gravnix); nY++) {
            if( STICKY_BLOCK_VALUE == GetGravnixBoardValue(m_Gravnix, nX, nY ) ) {
               m_Pieces.HidePiece(nX, nY, STICKY_BLOCK_VALUE, IsGravnixPieceStuck(m_Gravnix, nX, nY)==GRAVNIXLIB_STUCK);
               if( GetGravnixSpotValue(m_Gravnix, nX, nY) == GLASS_BLOCK_VALUE )
                  m_Pieces.ChangePiece(nX, nY, GLASS_BLOCK_VALUE, STUCK_GLASS_BLOCK_VALUE);
            }
         }
      }

      bool bAnyDropping = false;
      if( eDirection == Left || eDirection == Up ) { 
         for(int nX=0; nX<GetGravnixBoardWidth(m_Gravnix); nX++) {
            for(int nY=0; nY<GetGravnixBoardHeight(m_Gravnix); nY++) {
               if( GRAVNIXLIB_SHOULD_DROP == ShouldGravnixPieceDrop(m_Gravnix, nX, nY ) ) {
                  m_Pieces.DropPiece(nX, nY, GetSlideDirection());
                  DropGravnixPiece(m_Gravnix, nX, nY);
                  bAnyDropping = true;
               }
            }
         }
      } else {
         for(int nX=GetGravnixBoardWidth(m_Gravnix)-1; nX>=0; nX--) {
            for(int nY=GetGravnixBoardHeight(m_Gravnix)-1; nY>=0; nY--) {
               if( GRAVNIXLIB_SHOULD_DROP == ShouldGravnixPieceDrop(m_Gravnix, nX, nY ) ) {
                  m_Pieces.DropPiece(nX, nY, GetSlideDirection());
                  DropGravnixPiece(m_Gravnix, nX, nY);
                  bAnyDropping = true;
               }
            }
         }
      }

      if( bAnyDropping ) {
         //bool bAnyShrinking = false;
         for(int nX=0; nX<GetGravnixBoardWidth(m_Gravnix); nX++) {
            for(int nY=0; nY<GetGravnixBoardHeight(m_Gravnix); nY++) {
               if( BLACKHOLE_BLOCK_VALUE == GetGravnixBoardValue(m_Gravnix, nX, nY ) &&  GRAVNIXLIB_SHOULD_SHRINK == ShouldGravnixPieceShrink(m_Gravnix, nX, nY ) ) {
                  m_Pieces.ShrinkPiece(nX, nY, BLACKHOLE_BLOCK_VALUE);
                  //bAnyShrinking = true;
               }
            }
         }
         return true;
      }

      bool bAnyFading = false;
      for(int nX=0; nX<GetGravnixBoardWidth(m_Gravnix); nX++) {
         for(int nY=0; nY<GetGravnixBoardHeight(m_Gravnix); nY++) {
            if( GRAVNIXLIB_SHOULD_DISAPPEAR == ShouldGravnixPieceDisappear(m_Gravnix, nX, nY ) ) {
               m_Pieces.DisappearPiece(nX, nY);
               bAnyFading = true;
            }
         }
      }

      GravnixRemoveMatches(m_Gravnix);

      if( bAnyFading )
         return true;

      if( !m_bGameOver && IsGravnixSolved(m_Gravnix) == GRAVNIXLIB_SOLVED ) {
         m_bGameOver = true;
#ifndef USE_GRAPHIC_YOU_WIN
         m_YouWinMessage.CreateMessage("Level\nComplete!!\n******");
#endif
         m_Timer.Stop();
         if( m_nLevelNumber >= 0 ) {
            m_pConfig->SetBeatLevel(m_nLevelNumber, m_Timer.GetElapsed());
         }
      }

      if( m_YouWinMessage.HasMessage() && ((!m_YouWinMessage.IsAnimating() && !m_YouWinMessage.IsStayDuration()) || m_YouWinMessage.FinishFast()) )
         return false;
   }

   return true;
}

bool Game::PollEvents()
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
						m_bShouldQuit = true;
					return false;
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

					case SDLK_PLUS:
						Redo();
						break;

					case SDLK_MINUS:
						Undo();
						break;

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

void Game::UpdateDisplay()
{
	//Draw background
	m_Background.DrawBackground();

#ifdef DRAW_BOARD_BACKGROUND
#ifdef DRAW_EACH_SPOT
	int nWidth = GetGravnixBoardWidth(m_Gravnix), nHeight = GetGravnixBoardHeight(m_Gravnix);
        for(int nX=0; nX<nWidth; nX++) {
           for(int nY=0; nY<nHeight; nY++) {
              int nType = GetGravnixSpotValue(m_Gravnix, nX, nY);
              if( nType == 0 )
                 nType = GetGravnixBoardValue(m_Gravnix, nX, nY);
              if( nType == 0 ) {
                 int x = m_BoardMetrics.GetXPos(nX), y = m_BoardMetrics.GetYPos(nY), nPieceSize = m_BoardMetrics.GetPieceSize();
                 boxRGBA(m_pScreen, x, y, x+nPieceSize, y+nPieceSize, GAME_BACKGROUND_R, GAME_BACKGROUND_G, GAME_BACKGROUND_B, 230);
              }
           }
        }
#else
	boxRGBA(m_pScreen, m_BoardMetrics.GetLeft(), m_BoardMetrics.GetTop(), m_BoardMetrics.GetRight(), m_BoardMetrics.GetBottom(),  GAME_BACKGROUND_R, GAME_BACKGROUND_G, GAME_BACKGROUND_B, 230);
#endif
#endif

	if( m_StartMessage.HasMessage() && m_StartMessage.Animate() ){ return; }
	if( !m_YouWinMessage.HasMessage() && !m_Pieces.Animate() )
	{
		SDL_UpdateRect(m_pScreen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}

#ifdef USE_GRAPHIC_YOU_WIN
	if( m_bGameOver ) {
		if( m_pWinGraphic == NULL ) {
		m_pWinGraphic = nSDL_LoadImage(image_HitoriYouWin);
                SDL_SetColorKey(m_pWinGraphic, SDL_SRCCOLORKEY, SDL_MapRGB(m_pWinGraphic->format, 255, 255, 255));
		}

		SDL_Rect rectWin;
		rectWin.x = (SCREEN_WIDTH - m_pWinGraphic->w)/2;
		rectWin.y = (SCREEN_HEIGHT - m_pWinGraphic->h)/2;
		rectWin.w = m_pWinGraphic->w;
		rectWin.h = m_pWinGraphic->h;
	
		SDL_BlitSurface(m_pWinGraphic, NULL, m_pScreen, &rectWin);	
	}
#else
	if( m_YouWinMessage.HasMessage() && !m_YouWinMessage.Animate() )
#endif
        {
                //SDL_UpdateRect(m_pScreen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        }
}

void Game::Move(Direction eDirection)
{
	if( m_StartMessage.HasMessage() && (m_StartMessage.IsAnimating() || m_StartMessage.IsStayDuration()) ) {
                m_StartMessage.SetFinishFast();
                return;
        }
        if( m_YouWinMessage.HasMessage() && (m_YouWinMessage.IsAnimating() || m_YouWinMessage.IsStayDuration()) ) {
                m_YouWinMessage.SetFinishFast();
                return;
        }

        if( m_bGameOver || m_Pieces.IsAnimating() )
                return;

	int nDirection = GRAVNIXLIB_DIRECTION_NONE;
	if( eDirection == Up )
		nDirection = GRAVNIXLIB_DIRECTION_UP;
	else if( eDirection == Left )
                nDirection = GRAVNIXLIB_DIRECTION_LEFT;
	else if( eDirection == Right )
                nDirection = GRAVNIXLIB_DIRECTION_RIGHT;
	else if( eDirection == Down )
                nDirection = GRAVNIXLIB_DIRECTION_DOWN;

	if( GRAVNIXLIB_CAN_SLIDE == SlideGravnixSpots(m_Gravnix, nDirection, 0) ) {
	}
}

void Game::Undo()
{
   if( m_bGameOver )
      return;

   if( GRAVNIXLIB_CANNOT_UNDO != GravnixUndo(m_Gravnix) ) {
      m_Pieces.ClearPieces();

      int nWidth = GetGravnixBoardWidth(m_Gravnix), nHeight = GetGravnixBoardHeight(m_Gravnix);

      for(int nX=0; nX<nWidth; nX++) {
         for(int nY=0; nY<nHeight; nY++) {
            int nType = GetGravnixSpotValue(m_Gravnix, nX, nY);
            if( nType > 0 )
                m_Pieces.CreatePiece(nType, nX, nY);
            int nBoardType = GetGravnixBoardValue(m_Gravnix, nX, nY);
            if( nBoardType > 0 ) 
                m_Pieces.CreatePiece(nBoardType, nX, nY);
            if( nBoardType == STICKY_BLOCK_VALUE && nType > 0 ) {
               m_Pieces.HidePiece(nX, nY, STICKY_BLOCK_VALUE, IsGravnixPieceStuck(m_Gravnix, nX, nY)==GRAVNIXLIB_STUCK);
               if( nType == GLASS_BLOCK_VALUE )
                  m_Pieces.ChangePiece(nX, nY, GLASS_BLOCK_VALUE, STUCK_GLASS_BLOCK_VALUE);
            }
          }
      }      
   }
}

void Game::Redo()
{
   if( m_bGameOver )
      return;

   if( GRAVNIXLIB_CANNOT_REDO != GravnixRedo(m_Gravnix) ) {
      m_Pieces.ClearPieces();

      int nWidth = GetGravnixBoardWidth(m_Gravnix), nHeight = GetGravnixBoardHeight(m_Gravnix);

      for(int nX=0; nX<nWidth; nX++) {
         for(int nY=0; nY<nHeight; nY++) {
            int nType = GetGravnixSpotValue(m_Gravnix, nX, nY);
            if( nType > 0 ) 
                m_Pieces.CreatePiece(nType, nX, nY);
            int nBoardType = GetGravnixBoardValue(m_Gravnix, nX, nY);
            if( nBoardType > 0 )
                m_Pieces.CreatePiece(nBoardType, nX, nY);
            if( nBoardType == STICKY_BLOCK_VALUE && nType > 0 ) {
               m_Pieces.HidePiece(nX, nY, STICKY_BLOCK_VALUE, IsGravnixPieceStuck(m_Gravnix, nX, nY)==GRAVNIXLIB_STUCK);
               if( nType == GLASS_BLOCK_VALUE )
                  m_Pieces.ChangePiece(nX, nY, GLASS_BLOCK_VALUE, STUCK_GLASS_BLOCK_VALUE);
            }
          }
      }
   }
}

