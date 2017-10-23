class GameState
{
protected:
	int score;
	bool game_over;
	int bounces;
	int hiscore;

public:
	GameState();
	int getScore() 
	{
		return score;
	}
	int getHiscore()
	{
		return hiscore;
	}
	bool isGameOver()
	{
		return game_over;
	}
	int getBounces()
	{
		return bounces;
	}
	void incrementScore();
	void resetScore();
	void incrementBounces();
	void resetBounces();
	void setGameOver(bool s);

};