#pragma once
#include "Window.h"

class Game {
public:
	Game();
	~Game();

	void HandleInput();
	void Update();
	void Render();

	Window* GetWindow();

	sf::Time GetElapsed();
	void RestartClock();
private:
	void MovePlayer(sf::Vector2i);
	void MoveBullets();
	void DestroyBullets();
	void SpawnEnemies();
	void MoveEnemies();
	void CollisionDetection();
	void DifficultyControl();

	Window m_window;
	sf::Clock m_clock;
	sf::Time m_elapsed;

	sf::CircleShape player;
	sf::CircleShape playerBullet;
	sf::RectangleShape enemyA;
	sf::RectangleShape enemyB;

	std::vector<sf::CircleShape> playerBullets;
	std::vector<sf::RectangleShape> enemiesA;
	std::vector<sf::RectangleShape> enemiesB;

	float movementSpeed;
	float enemyMovementSpeed;
	float playerBulletSpeed;

	sf::Vector2f bulletMovement;

	sf::Font font;

	sf::Vector2i playerMovementControl;

	sf::Time frameTime;
	float dt;

	sf::Clock shotTimer;
	sf::Time sh_elapsed;

	sf::Clock spawnTimerA;
	sf::Clock spawnTimerB;
	sf::Time sp_elapsedA;
	sf::Time sp_elapsedB;

	int playerScore;

	sf::Text scoreText;
	std::string score;
	sf::Text textPause;
	sf::Text textFail1;
	sf::Text textFail2;

	bool gameRunning;
	bool pauseState;
	bool failState;
	bool scoreFlag;

	sf::Event event;

	int genericTimer;

	sf::SoundBuffer buffer1;
	sf::SoundBuffer buffer2;
	sf::Sound shot;
	sf::Sound explosion;

	sf::Clock pauseTimer;
	sf::Time pause_elapsed;

	std::vector<float> spawnPos;

	sf::Vector2i increment;

	bool direction;
	float movement;

	sf::Clock directionTimer;
	sf::Time direction_elapsed;

	float directionValue;

	sf::FloatRect textRect;
};