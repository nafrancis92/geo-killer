#include "Game.h"

//To-do list
//1.) Menu
//2.) High score list

Game::Game() : m_window("Geometry Killer", sf::Vector2u(600, 800)) {
	RestartClock();

	srand(time(NULL));

	// Setting up class members.

	increment = sf::Vector2i(400, 0);

	player.setRadius(20.f);
	player.setPointCount(3);
	player.setPosition(300, 600);
	player.setFillColor(sf::Color::White);

	playerBullet.setFillColor(sf::Color::Yellow);
	playerBullet.setRadius(3.f);

	enemyA.setFillColor(sf::Color::Green);
	enemyA.setSize(sf::Vector2f(60.f, 60.f));
	enemyA.setPosition(0, 0);

	enemyB.setFillColor(sf::Color::Magenta);
	enemyB.setSize(sf::Vector2f(40.f, 40.f));
	enemyB.setPosition(0, 0);

	playerBullets.push_back(sf::CircleShape(playerBullet));
	enemiesA.push_back(sf::RectangleShape(enemyA));
	enemiesB.push_back(sf::RectangleShape(enemyB));

	movementSpeed = 250.f;
	enemyMovementSpeed = 300.f;
	playerBulletSpeed = 500.f;

	playerScore = 0;

	if (!font.loadFromFile("resources/ARCADECLASSIC.ttf"))
	{
		//return -1;
		std::cout << "problem";
		exit;
	}

	scoreText.setFont(font);
	scoreText.setCharacterSize(40);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setString("0");
	textRect = scoreText.getLocalBounds();
	textPause.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	scoreText.setPosition(m_window.GetWindowSize().x/2, 20);

	textPause.setFont(font);
	textPause.setCharacterSize(60);
	textPause.setFillColor(sf::Color::White);
	textPause.setString("GAME PAUSED");
	textRect = textPause.getLocalBounds();
	textPause.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	textPause.setPosition(sf::Vector2f(m_window.GetWindowSize().x/2.0f, 300));

	textFail1.setFont(font);
	textFail1.setCharacterSize(60);
	textFail1.setFillColor(sf::Color::White);
	textFail1.setString("GAME OVER");
	textRect = textFail1.getLocalBounds();
	textFail1.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	textFail1.setPosition(sf::Vector2f(m_window.GetWindowSize().x / 2.0f, 200));

	textFail2.setFont(font);
	textFail2.setCharacterSize(60);
	textFail2.setFillColor(sf::Color::White);
	textFail2.setString("PRESS R TO RESTART");
	textRect = textFail2.getLocalBounds();
	textFail2.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	textFail2.setPosition(sf::Vector2f(m_window.GetWindowSize().x / 2.0f, 340));

	gameRunning = 1;
	pauseState = 0;
	failState = 0;
	scoreFlag = 0;

	genericTimer = 0;

	if (!buffer1.loadFromFile("resources/audio/shot.ogg"))
	{
		std::cout << "Couldn't load shot.ogg" << std::endl;
		exit;
	}
	
	if (!buffer2.loadFromFile("resources/audio/explosion.ogg"))
	{
		std::cout << "Couldn't load explosion.ogg" << std::endl;
		exit;
	}

	shot.setBuffer(buffer1);
	shot.setVolume(40.f);
	explosion.setBuffer(buffer2);
	explosion.setVolume(40.f);

	direction = 0;
	movement = 300.f;

	directionValue = 1.f;
}

Game::~Game() { }

sf::Time Game::GetElapsed() { return m_elapsed; }
void Game::RestartClock() { m_elapsed = m_clock.restart();
}
Window* Game::GetWindow() { return &m_window; }

void Game::HandleInput() {
	sh_elapsed = shotTimer.getElapsedTime();
	pause_elapsed = pauseTimer.getElapsedTime();

	playerMovementControl.x = 0;
	playerMovementControl.y = 0;


	if (failState && sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		//clear all enemiesA on screen
		enemiesA.erase(enemiesA.begin(), enemiesA.end());
		//clear all enemiesB on screen
		enemiesB.erase(enemiesB.begin(), enemiesB.end());
		//clear any bullets on screen
		playerBullets.erase(playerBullets.begin(), playerBullets.end());
		//reset score
		playerScore = 0;
		scoreText.setString("0");
		//reset player position
		player.setPosition(300, 600);
		//reset enemy vertical speed
		enemyMovementSpeed = 300.f;
		//reset enemyB horizontal speed
		movement = 300.f;
		//reset val used for enemyB direction change
		directionValue = 1.f;
		//reset fail state
		failState = 0;
		//run game again
		gameRunning = 1;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && pause_elapsed.asMilliseconds() >= 150 && !failState)
	{
		pauseState = !pauseState;
		gameRunning = !gameRunning;
		pauseTimer.restart();
	}


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player.getPosition().x > 0)
		--playerMovementControl.x;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player.getPosition().x < 559)
		++playerMovementControl.x;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && player.getPosition().y > 5)
		--playerMovementControl.y;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && player.getPosition().y < 762)
		++playerMovementControl.y;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sh_elapsed.asMilliseconds() >= 300 && playerBullets.size() < 3 && gameRunning)
	{
		playerBullet.setPosition(player.getPosition().x + 18, (player.getPosition().y));
		playerBullets.push_back(sf::CircleShape(playerBullet));
		shot.play();
		shotTimer.restart();
	}

}

void Game::Update() {

	m_window.Update();

	if (gameRunning) {
		MovePlayer(playerMovementControl);
		MoveBullets();
		DestroyBullets();
		DifficultyControl();
		SpawnEnemies();
		MoveEnemies();
		CollisionDetection();
	}
}

void Game::MovePlayer(sf::Vector2i playerMovementControl) {
	sf::Vector2f playerMovement(playerMovementControl);

	if (playerMovementControl.x != 0 && playerMovementControl.y != 0)
		playerMovement *= 0.606f;

	dt =  m_elapsed.asSeconds();

	player.move(playerMovement * movementSpeed * dt);
}

void Game::MoveBullets() {
	bulletMovement.x = 0;
	bulletMovement.y = -500;

	for (size_t i = 0; i < playerBullets.size(); i++)
	{
		playerBullets[i].move(0, -(GetElapsed().asSeconds() * playerBulletSpeed));
	}
}


//remove bullets that leave the screen (or hit enemiesA?)
void Game::DestroyBullets() {
	for (size_t i = 0; i < playerBullets.size(); i++)
	{
		if (playerBullets[i].getPosition().y < 0)
			playerBullets.erase(playerBullets.begin() + i);
	}
}

void Game::SpawnEnemies() {
	sp_elapsedA = spawnTimerA.getElapsedTime();

	if (sp_elapsedA.asSeconds() >= 0.20)
	{
		enemyA.setPosition((rand() % int(m_window.GetWindowSize().x - enemyA.getSize().x)), 0.f);
		enemiesA.push_back(sf::RectangleShape(enemyA));
		spawnTimerA.restart();
	}

	sp_elapsedB = spawnTimerB.getElapsedTime();

	if (sp_elapsedB.asSeconds() >= 1)
	{
		enemyB.setPosition((rand() % (int(m_window.GetWindowSize().x - 100)) + 100), 0.f);
		enemiesB.push_back(sf::RectangleShape(enemyB));
		spawnTimerB.restart();
	}

}

void Game::MoveEnemies() {
	//move A-type enemies
	for (size_t i = 0; i < enemiesA.size(); i++)
	{
		enemiesA[i].move(0, (GetElapsed().asSeconds() * enemyMovementSpeed));

		if (enemiesA[i].getPosition().y > m_window.GetWindowSize().y)
			enemiesA.erase(enemiesA.begin() + i);
	}

	//move B-type enemies
	for (size_t i = 0; i < enemiesB.size(); i++)
	{
		direction_elapsed = directionTimer.getElapsedTime();
		if (direction_elapsed.asSeconds() >= directionValue)
		{
			movement = -movement;
			directionTimer.restart();
		}

		enemiesB[i].move(GetElapsed().asSeconds() * movement, GetElapsed().asSeconds() * enemyMovementSpeed);

		if (enemiesB[i].getPosition().y > m_window.GetWindowSize().y)
			enemiesB.erase(enemiesB.begin() + i);
	}

}

void Game::CollisionDetection() {
	//detect those collisions
	for (size_t i = 0; i < enemiesA.size(); i++)
	{
		if (player.getGlobalBounds().intersects(enemiesA[i].getGlobalBounds()))
		{
			gameRunning = 0;

			failState = 1;
			break;
		}
		for (size_t k = 0; k < playerBullets.size(); k++)
		{
			if (playerBullets[k].getGlobalBounds().intersects(enemiesA[i].getGlobalBounds()))
			{
				playerBullets.erase(playerBullets.begin() + k);
				enemiesA.erase(enemiesA.begin() + i);
				explosion.play();
				playerScore++;
				score = std::to_string(playerScore);
				scoreText.setString(score);
				break;
			}
		}
	}


	for (size_t i = 0; i < enemiesB.size(); i++)
	{
		if (player.getGlobalBounds().intersects(enemiesB[i].getGlobalBounds()))
		{
			gameRunning = 0;
			failState = 1;
			break;
		}
		for (size_t k = 0; k < playerBullets.size(); k++)
		{
			if (playerBullets[k].getGlobalBounds().intersects(enemiesB[i].getGlobalBounds()))
			{
				playerBullets.erase(playerBullets.begin() + k);
				enemiesB.erase(enemiesB.begin() + i);
				explosion.play();
				playerScore++;
				score = std::to_string(playerScore);
				scoreText.setString(score);
				break;
			}
		}
	}
}

void Game::DifficultyControl() {
	if (genericTimer < 20)
		genericTimer++;

	if (scoreFlag && playerScore % 10 == 1)
		scoreFlag = 0;

	//increase enemy speed for every 10 points
	if (playerScore != 0 && playerScore % 10 == 0 && genericTimer >= 20 && !scoreFlag)
	{
		enemyMovementSpeed += 45;
		if (movement < 0)
			movement -= 25;
		else
			movement += 25;
		genericTimer = 0;
		scoreFlag = 1;
	}
}

void Game::Render() {
	m_window.BeginDraw(); // Clear.
	//Draw text first to overlay everything else
	
	m_window.Draw(player);
	for (size_t i = 0; i < playerBullets.size(); i++)
		m_window.Draw(playerBullets[i]);
	for (size_t i = 0; i < enemiesA.size(); i++)
		m_window.Draw(enemiesA[i]);
	for (size_t i = 0; i < enemiesB.size(); i++)
		m_window.Draw(enemiesB[i]);
	if (!gameRunning)
	{
		if (pauseState)
			m_window.Draw(textPause);
		else if (failState)
		{
			m_window.Draw(textFail1);
			m_window.Draw(textFail2);
		}
	}
	m_window.Draw(scoreText);
	m_window.EndDraw(); 
}