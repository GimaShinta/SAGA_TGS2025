#pragma once
#include"../Singleton.h"
#include<vector>

class ScoreData : public Singleton<ScoreData>
{
private:
	//�X�R�A
	std::vector<float> score_data;
	int player_stock;	//�c�@
	bool submitted = false;

public:
	ScoreData();
	~ScoreData();

public:

	std::vector<float> GetScoreData() const;
	void SetScoreData(float score);
	void AddScore(float value);
	float GetTotalScore() const;
	void Reset();
	bool IsSubmitted() const { return submitted; }
	void MarkSubmitted() { submitted = true; }

};