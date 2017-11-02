#include "GameState.h"

GameState::GameState() 
{
	score = 0;
	score2 = 0;
	hiscore = 0;
	game_over = false;
	host_paddle_hit = true;
	bounces = 0;
}

void GameState::incrementScore()
{
	score++;
}
void GameState::incrementScore2()
{
	score2++;
}
void GameState::incrementBounces()
{
	bounces++;
}

void GameState::resetBounces() 
{
	bounces = 0;
}
void GameState::setPaddleHit(bool b) 
{
	host_paddle_hit = b;
}

void GameState::resetScore() 
{
	if (score > hiscore)
	{
		hiscore = score;
	}
	score = 0;
}

void GameState::setGameOver(bool s)
{
	game_over = s;
}