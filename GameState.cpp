#include "GameState.h"

GameState::GameState() 
{
	score = 0;
	hiscore = 0;
	game_over = false;
	bounces = 0;
}

void GameState::incrementScore()
{
	score++;
}

void GameState::incrementBounces()
{
	bounces++;
}

void GameState::resetBounces() 
{
	bounces = 0;
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