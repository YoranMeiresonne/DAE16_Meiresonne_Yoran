#include "pch.h"
#include "Game.h"
#include "utils.h"
#include <iostream>
#include "Player.h"
#include "Matrix2x3.h"
#include "SVGParser.h"
#include "Enemy.h"
#include "Zen_Chan.h"
#include "Level.h"
#include "Texture.h"
#include "string"
#include "PointManager.h"
#include "SoundEffect.h"
#include "SoundStream.h"

Game::Game(const Window& window)
	:BaseGame{ window }
	,m_ThePlayer{}
	,m_PointManager{}
	,m_LevelNr{0}
	,m_SoundOn{false}
	
{
	Initialize();
	
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_Camera = new Camera(GetViewPort().width, GetViewPort().height);
	
	InitializeMap();

	std::string Tekst("NOW IT IS THE BEGINNING OF ");
	std::string Tekst2("A FANTASTIC STORY! LET US ");
	std::string Tekst3("   ""MAKE A JOURNEY TO"); 
	std::string Tekst4("  ""THE CAVE OF MONSTERS!"); 
	std::string Tekst5("     ""GOOD LUCK!"); 

	std::string GameOver("     ""GameOver!");
	std::string EndGame("     ""Victory!");

	m_Tekst.push_back(new Texture(Tekst,  "VCR_OSD_MONO_1.001.ttf", 30, Color4f{1.f,1.f,1.f,1.f}) );  
	m_Tekst.push_back(new Texture(Tekst2, "VCR_OSD_MONO_1.001.ttf", 30, Color4f{ 1.f,1.f,1.f,1.f }) ); 
	m_Tekst.push_back(new Texture(Tekst3, "VCR_OSD_MONO_1.001.ttf", 30, Color4f{ 1.f,1.f,1.f,1.f }) ); 
	m_Tekst.push_back(new Texture(Tekst4, "VCR_OSD_MONO_1.001.ttf", 30, Color4f{ 1.f,1.f,1.f,1.f }) ); 
	m_Tekst.push_back(new Texture(Tekst5, "VCR_OSD_MONO_1.001.ttf", 30, Color4f{ 1.f,1.f,1.f,1.f }) ); 

	m_Tekst.push_back(new Texture(GameOver, "VCR_OSD_MONO_1.001.ttf", 30, Color4f{ 1.f,1.f,1.f,1.f }));
	m_Tekst.push_back(new Texture(EndGame, "VCR_OSD_MONO_1.001.ttf", 30, Color4f{ 1.f,1.f,1.f,1.f }));
	
	m_PointManager = new PointManager();

	m_BackGroundMusic.Play(true);

}

void Game::Cleanup( )
{
	



	for (int i = 0; i < m_TheMap.size(); i++)
	{
		m_TheMap[i].clear();
	}

	for (int i = 0; i < m_Level.size(); i++)
	{
		m_Level[i]->~Level();
	}

	for (size_t i = 0; i < m_Tekst.size(); i++)
	{
		m_Tekst[i] = nullptr; 
	}

	m_PointManager->~PointManager();
	delete m_PointManager;

	m_ThePlayer->~Player();
	delete m_ThePlayer;

	m_Mappen[0].clear();
	m_Level.clear();
	m_worldVerticesTable.clear(); 
}

void Game::Update( float elapsedSec )
{
	float MapPos{ 0.f };
	float IsDone{ 0.f };

	if (m_ThePlayer->IsPlayerAlive() && m_Win == false)
	{
		m_AccuTime += elapsedSec;
		m_Level[m_LevelNr]->GetBubblePlayer(m_ThePlayer->IsBubblePlayer());

		//LEvel/Maps
		if (m_AccuTime > 5.f && m_Start) // Stop StartScreen
		{
			m_AccuTime = 0.f;
			m_Start = false;
			m_ThePlayer->SetStartScreen(false);
			m_ThePlayer->SetLevelPosition(Vector2f(35.f, 18.f));
			m_Level[m_LevelNr]->SetMapInPosition(false);
			m_MapInPosition = false;// A
		}

		if (LevelDone())
		{
			m_LevelNr++;
			if (m_Level.size() - 1 < m_LevelNr)
			{
				m_Win = true;
			}
			else
			{
				m_Level[m_LevelNr]->SetMapInPosition(false);
				m_ThePlayer->GetMapInPosition(false);
				m_ThePlayer->SetLevelPosition(Vector2f(35.f, 18.f));
				m_ThePlayer->PlayerInBubble();
				m_MapInPosition = false; // A
			}

		}

		//Movement
		if (!m_Win)
		{


			if (m_MapInPosition && m_ThePlayer->IsBubblePlayer() && !m_Start)// A
			{
				m_Level[m_LevelNr]->SetMapInPosition(true);// A
				m_ThePlayer->GetMapInPosition(true);
			}

			m_ThePlayer->Update(elapsedSec, m_Mappen[0][m_LevelNr]);
			m_Level[m_LevelNr]->Update(elapsedSec, m_ThePlayer);

			if ((m_ThePlayer->GetBoundingBox().bottom + m_ThePlayer->GetBoundingBox().height) < 0.f)
			{
				m_ThePlayer->SetPosition(m_ThePlayer->GetBoundingBox().left, 500.f);
			}
			
			if (m_MapInPosition == false) // A
			{
				MapPos = -(450.f * (m_LevelNr + 1));
				IsDone = MapPos + m_TransMovement;
				if (0.0f <= IsDone)
				{
					m_MapInPosition = true;
					m_Level[m_LevelNr]->SetMapInPosition(true);
					/*m_MapLock = true;*/
				}
				else
				{
					m_TransMovement += 50.f * elapsedSec;
				}
			}
			Collisionchecks();

			if (m_ItemIsPickedUp)
			{
				m_PointTime += elapsedSec;
				if (m_PointTime > 2.f)
				{
					m_ItemIsPickedUp = false;
					m_PointTime = 0.f;
				}
			}
		}
	}

	else if(m_Win == false)
	{
		GameOverUpdate();
		
	}
	else if (m_Win)
	{
		EndGameUpdate();
	}
	

}

void Game::Draw( ) const
{
	ClearBackground( );

	
	if (m_ThePlayer->IsPlayerAlive() && m_Win == false)
	{
		StartScreen();

		m_Camera->Aim(669.f, 463.f, Vector2f(m_ThePlayer->GetBoundingBox().left, m_ThePlayer->GetBoundingBox().bottom));

		utils::SetColor(Color4f(1.f, 0.f, 0.f, 1.f));

		glPushMatrix();
		m_Level[m_LevelNr]->Draw();
		glPopMatrix();

		NextLevel();

		m_ThePlayer->Draw();

		if (m_ItemIsPickedUp)
		{
			m_pPointSheet->Draw(m_PointBounds, m_SrcPointBounds); 
		}
	}
	else if (m_Win == false)
	{
		GameOverDraw();
	}
	else if (m_Win)
	{
		EndGameDraw();
	}
	m_PointManager->Draw(); 
	

	m_Camera->Reset();

}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
	switch (e.keysym.sym)
	{
	case SDLK_UP:
		if (!m_ThePlayer->IsBubblePlayer() && m_ThePlayer->IsPlayerAlive())
		{
			m_Jumping.Play(0);
		}
		else if (!m_ThePlayer->IsPlayerAlive())
		{
			StartGame();
		}
		break;
	case SDLK_SPACE:
		if (!m_ThePlayer->IsBubblePlayer() && m_ThePlayer->IsPlayerAlive())
		{
			m_BubbleShooting.Play(0);
		}
		else if (!m_ThePlayer->IsPlayerAlive())
		{
			StartGame();
		}
		break;
	case SDLK_r:
		m_ThePlayer->SetPosition(21.f, 21.f);

	default:
		break;
	}
	
	
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case  SDLK_0:
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::Collisionchecks()
{
	//Player/Item Collision (Needs work.

	for (int idx = 0; idx < m_Level[m_LevelNr]->GetNumeItems(); idx++)
	{
		if (utils::IsOverlapping(m_ThePlayer->GetBoundingBox(), m_Level[m_LevelNr]->GetItemBoundingBox(idx)))
		{
			m_PointManager->AddPoints(m_Level[m_LevelNr]->GetPoints(idx));
			SetPointBoundingBox(m_Level[m_LevelNr]->GetItemBoundingBox(idx));
			GetCurrentPointFrameRect();
			m_ItemIsPickedUp = true;
			m_Level[m_LevelNr]->ItemIsHit(idx);
			m_PointManager->PrintPoints();
			m_PickupItem.Play(0);
		}
	}

	//Player_Bubble/Enemy Collision (Needs work)
	
	for (int i = 0; i < m_Level[m_LevelNr]->GetNumEnemy(); i++)
	{
		Rectf EnemyBounds = m_Level[m_LevelNr]->GetEnemyBoundingBox(i);
		if (utils::IsOverlapping(m_ThePlayer->GetBoundingBox(),EnemyBounds) && m_Level[m_LevelNr]->GetBubbleEnemy(i))
		{
			if (m_Level[m_LevelNr]->GetIntTypeBehaviorState(i) == 2) // EnemyBubble hit by player => EnemyDefeated
			{
				m_Level[m_LevelNr]->EnemyIsHit(i, true);
				m_EnemyDefeated.Play(0);
			}
			if (m_Level[m_LevelNr]->GetIntTypeBehaviorState(i) == 1) // Angry Enemy
			{
				m_ThePlayer->PlayerIsAlive(false);
			}
			
		}
		if (m_ThePlayer->BubbleHitEnemy(EnemyBounds, m_Level[m_LevelNr]->GetIntTypeBehaviorState(i)))
		{
			m_Level[m_LevelNr]->EnemyIsHit(i);
		}
	}

}

void Game::LevelTekst(int LvlNr) const
{
	
}

bool Game::LevelDone()
{
	if (m_Level[m_LevelNr]->GetNumEnemy() == 0 && m_Level[m_LevelNr]->GetNumeItems() == 0)
	{
		return true;
	}
	return false;
}

void Game::GameOver()
{

}

void Game::StartScreen() const 
{
	if (m_AccuTime < 5.f && m_Start)
	{

		for (size_t i = 0; i < m_Tekst.size() - 3; i++)
		{
			m_Tekst[i + 1]->Draw(Vector2f(669.f / 6, 363.f - 30 * i ));
		}
		m_ThePlayer->SetStartScreen(true);
		m_ThePlayer->PlayerInBubble();

	}
	
	
}

void Game::NextLevel() const
{
	for (int idx = 0; idx < m_Mappen[0].size(); idx++)
	{
		glPushMatrix();

		glTranslatef(0.f, (-450.f * (idx + 1)) + m_TransMovement, 0.f);

		for (int i = 0; i < m_Mappen[0][idx].size(); i++)
		{

			utils::FillPolygon(m_Mappen[0][idx][i]);
		}

		glPopMatrix();
	}
	

}

void Game::InitializeMap()
{
	m_TheMap.resize(5);
	m_EnemyPos.resize(5);

	m_Vertices.GetVerticesFromSvgFile("Maps/FirstMap.svg", m_TheMap[0]);
	m_Vertices.GetVerticesFromSvgFile("Maps/SecondMap.svg", m_TheMap[1]);
	m_Vertices.GetVerticesFromSvgFile("Maps/ThirdMap.svg", m_TheMap[2]);
	m_Vertices.GetVerticesFromSvgFile("Maps/FourthMap.svg", m_TheMap[3]);
	m_Vertices.GetVerticesFromSvgFile("Maps/FifthMap.svg", m_TheMap[4]);


	m_Vertices.GetVerticesFromSvgFile("EnemyPos/FirstMapEnemyPos.svg", m_EnemyPos[0]);
	m_Vertices.GetVerticesFromSvgFile("EnemyPos/SecondMapEnemyPos.svg", m_EnemyPos[1]);
	m_Vertices.GetVerticesFromSvgFile("EnemyPos/ThirdMapEnemyPos.svg", m_EnemyPos[2]);
	m_Vertices.GetVerticesFromSvgFile("EnemyPos/FourthMapEnemyPos.svg", m_EnemyPos[3]);
	m_Vertices.GetVerticesFromSvgFile("EnemyPos/FifthMapEnemyPos.svg", m_EnemyPos[4]);

	m_Mappen.push_back(m_TheMap);


	m_Level.push_back(new Level(0, m_EnemyPos[0], m_Mappen));
	m_Level.push_back(new Level(1, m_EnemyPos[1], m_Mappen));
	m_Level.push_back(new Level(2, m_EnemyPos[2], m_Mappen));
	m_Level.push_back(new Level(3, m_EnemyPos[3], m_Mappen));
	m_Level.push_back(new Level(4, m_EnemyPos[4], m_Mappen));


	m_ThePlayer = new Player(Vector2f(110.f, 150.f)); 


	m_pPointSheet = new Texture("ItemSheet.png");
}

void Game::GameOverUpdate()
{
	for (int i = 0; i < m_Level.size(); i++)
	{
		m_Level[i]->~Level(); 
	}
	for (int i = 0; i < m_EnemyPos.size(); i++)
	{
		m_EnemyPos[i].clear();
	}
	for (int i = 0; i < m_TheMap.size(); i++)
	{
		m_TheMap[i].clear();
	}
	m_Mappen.clear();
	m_Level.clear();
	m_ThePlayer->~Player();
	

	if (m_SoundOn == false)
	{
		m_BackGroundMusic.Stop();
		m_GameOverMusic.Play(true);
		m_SoundOn = true;
	}
}

void Game::StartGame()
{
	InitializeMap();
	m_LevelNr = 0;
	m_ThePlayer->PlayerIsAlive(true);
	m_Start = true;
	m_MapInPosition = true;
	m_TransMovement = 0.f;
	m_AccuTime = 0.f;
	m_GameOverMusic.Stop();
	m_BackGroundMusic.Play(true);
	m_PointManager->SetPoints(0);
	m_SoundOn = false;

}

void Game::EndGameDraw() const
{
	m_Tekst[7]->Draw(Vector2f(669.f / 6, 263.f));
	
}

void Game::EndGameUpdate()
{
	Vector2f pos = Vector2f(669.f / 6, 163.f);
	if (m_SoundOn == false)
	{
		m_BackGroundMusic.Stop();
		m_GameOverMusic.Play(true);
		m_SoundOn = true;
	}
	m_PointManager->SetPosition(pos);
}

void Game::GetCurrentPointFrameRect() 
{
	m_CurrenPointFrame = 20 % 20;
	m_PointRow = 20 / 6;

	m_SrcPointBounds = Rectf{
		m_CurrenPointFrame * m_pPointSheet->GetWidth() / 9.f,
		m_PointRow * m_pPointSheet->GetHeight() / 6.f ,
		(float)m_pPointSheet->GetWidth() / 9.f,
		(float)m_pPointSheet->GetHeight() / 6.f
	};

}

void Game::SetPointBoundingBox(Rectf rect)
{
	m_PointBounds = Rectf(
		rect.left + 100.f,
		rect.bottom + m_pPointSheet->GetHeight() / 6.f + 20.f,
		m_pPointSheet->GetWidth() / 20.f,
		m_pPointSheet->GetHeight() / 6.f
	);
	
}

void Game::GameOverDraw() const
{
	m_Tekst[6]->Draw(Vector2f(669.f / 6, 263.f));
}

