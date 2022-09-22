#pragma once
#include "Polygon2D.h"

#define MAX_SCORE (8)
#define ANIM_SCORE_SPLIT_X (10)  // ‰æ‘œ‚Ì•ªŠ„”(‰¡)
#define ANIM_SCORE_SPLIT_Y (1)  // ‰æ‘œ‚Ì•ªŠ„”(c)
#define ScoreWidht (80)
#define ScoreHeight (100)

typedef struct
{
	int Score;

}ScoreInfo;

class Score
{
public:
	Score();
	~Score();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void AddScore_1P(int i);
	void AddScore_2P(int i);

	void SetGameEndScore(int score);
	ScoreInfo *GetScore_1P();
	ScoreInfo *GetScore_2P();
	int GetGameEndScore();
	Score GetScore();


private:

	ScoreInfo* m_pScore1P;
	ScoreInfo* m_pScore2P;
	ScoreInfo* m_pEndScore;
	DrawBuffer m_polygon;
	int m_GameEndScore;
};

