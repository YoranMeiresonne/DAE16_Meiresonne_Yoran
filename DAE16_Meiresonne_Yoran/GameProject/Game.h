#pragma once
#include "BaseGame.h"
#include <vector>
#include "SoundEffect.h"
#include "SoundStream.h"
#include "Texture.h"
#include "Player.h"
#include "SVGParser.h"
#include "Enemy.h"
#include "Zen_Chan.h"
#include "Items.h"
#include "PointManager.h"
#include "Level.h"
#include "Camera.h"


class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
	void Collisionchecks();
	void LevelTekst(int LvlNr) const;
	void GameOver();
	void StartScreen() const;
	void NextLevel() const;
	void InitializeMap();

	void GameOverDraw() const;
	void GameOverUpdate();

	void StartGame();

	void EndGameDraw() const;
	void EndGameUpdate();

	void GetCurrentPointFrameRect();
	void SetPointBoundingBox(Rectf rect);
	
	bool LevelDone();

	std::vector<std::vector<std::vector<std::vector<Vector2f>>>> m_Mappen{};
	std::vector<std::vector<std::vector<Vector2f>>> m_TheMap{};
	std::vector<std::vector<std::vector<Vector2f>>> m_EnemyPos{};
	std::vector<std::vector<Vector2f>> m_worldVerticesTable{ }; 
	std::vector<Texture*> m_Tekst{ nullptr }; 
	std::vector<std::string> m_StringTekst;
	std::vector<Level*> m_Level;

	Texture* m_pPointSheet{ new Texture("ItemSheet.png") };

	float m_PointTime{ 0.f };
	float m_AccuTime{ 0.f };
	float m_TransMovement{ 0.f };

	int m_CurrenPointFrame{};
	int m_PointRow{};
	int m_LevelNr{4};

	bool m_ItemIsPickedUp{ false };
	bool m_Start{ true };
	bool m_MapInPosition{ true };
	bool m_SoundOn{ false };;
	bool m_Win{ false };

	Rectf m_PointBounds{};
	Rectf m_SrcPointBounds{};

	PointManager* m_PointManager;
	Camera* m_Camera{};

	SVGParser m_Vertices;

	Player* m_ThePlayer;

	//Sounds
	SoundStream m_BackGroundMusic{ "Sound/Bubble Bobble (NES)_Music_Soundtrack_Playlist-002-Bubble Bobble_(NES)_Music-Game_Start.mp3" };
	SoundStream m_GameOverMusic{ "Sound/Bubble_Bobble_(NES)_Music_Soundtrack_Playlist-006-Bubble Bobble_(NES)_Music-Game_Over.mp3" };
	SoundStream m_EndGameMusic{ "Sound/Bubble Bobble (NES) Music Soundtrack Playlist - 013 - Bubble Bobble (NES) Music - Happy Ending Part 1.mp3" };

	SoundEffect m_Jumping{ "Sound/Sound_Effects/PlayerJump.wav" };
	SoundEffect m_BubbleShooting{ "Sound/Sound_Effects/ShootBubble.wav" };
	SoundEffect m_EnemyDefeated{ "Sound/Sound_Effects/EnemyDefeated.wav" };
	SoundEffect m_PickupItem{ "Sound/Sound_Effects/PickupItem.wav" };
};