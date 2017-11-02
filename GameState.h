class GameState
{
protected:
	int score;
	int score2;
	bool game_over;
	bool host_paddle_hit;//TRUE - Host, FALSE - Client
	int bounces;
	int hiscore;

public:
	GameState();
	int getScore() 
	{
		return score;
	}
	int getScore2() 
	{
		return score2;
	}
	int getHiscore()
	{
		return hiscore;
	}
	bool getPaddleHit()
	{
		return host_paddle_hit;
	}
	bool isGameOver()
	{
		return game_over;
	}
	int getBounces()
	{
		return bounces;
	}
	void setPaddleHit(bool b);
	void incrementScore();
	void incrementScore2();
	void setScore(int i);
	void setScore2(int i);
	void resetScore();
	void incrementBounces();
	void resetBounces();
	void setGameOver(bool s);

};