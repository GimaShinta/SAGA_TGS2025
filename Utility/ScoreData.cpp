#include "ScoreData.h"

ScoreData::ScoreData()
{
}

ScoreData::~ScoreData()
{
}

std::vector<float> ScoreData::GetScoreData() const
{
	return score_data;
}

void ScoreData::SetScoreData(float score)
{
	score_data.push_back(score);
}
