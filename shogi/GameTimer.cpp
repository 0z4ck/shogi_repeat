#include "GameTimer.h"

#include <Windows.h>
#include <chrono>

/*-----------------------------------------------------------------------------*/
/**
 */
/*-----------------------------------------------------------------------------*/
GameTimer::GameTimer()
{
	this->turn_ = Color::BLACK;
	this->started_ = false;
	this->start_time_ = 0;
}

GameTimer::GameTimer(std::function<void(Game*)> timeout, Game* param)
{
	this->turn_ = Color::BLACK;
	this->started_ = false;
	this->start_time_ = 0;

	this->timer_.SetTimeOut(timeout, param);
}


GameTimer::~GameTimer()
{
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief �������Ԑݒ�
 * @note  
 */
/*-----------------------------------------------------------------------------*/
void GameTimer::SetTime(Color color, int time_min, int byoyomi_sec)
{
	if (color == BLACK)
	{
		this->BlackTime.Time = time_min * 60 * 1000;
		this->BlackTime.RemainTime = this->BlackTime.Time;
		this->BlackTime.Byoyomi = byoyomi_sec * 1000;
		this->BlackTime.ElapsedTime = 0;

		this->WhiteTime = this->BlackTime; // ��莞�Ԃ������l��ݒ肷��
	}
	else
	{
		this->WhiteTime.Time = time_min * 60 * 1000;
		this->WhiteTime.RemainTime = this->BlackTime.Time;
		this->WhiteTime.Byoyomi = byoyomi_sec * 1000;
		this->WhiteTime.ElapsedTime = 0;
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief�ĊJ���̏���Ԑݒ�
* @note
*/
/*-----------------------------------------------------------------------------*/
void GameTimer::SetRestartTime(int blackTimeSec, int whiteTimeSec)
{
	int blacktime = blackTimeSec * 1000;
	int whitetime = whiteTimeSec * 1000;

	this->BlackTime.ElapsedTime = blacktime;
	this->WhiteTime.ElapsedTime = whitetime;

	if (this->BlackTime.Time <= blacktime)
	{
		this->BlackTime.RemainTime = 0; // �c�莞�ԂȂ�
		if (this->BlackTime.Byoyomi == 0)
		{
			// ���Ԃ��Ȃ��ꍇ
			if (this->BlackTime.Time != 0)
			{
				this->BlackTime.RemainTime = 60 * 1000; // �b��P��
			}
		}
	}
	else
	{
		this->BlackTime.RemainTime = this->BlackTime.Time - blacktime;
	}

	if (this->WhiteTime.Time <= whitetime)
	{
		this->WhiteTime.RemainTime = 0; // �c�莞�ԂȂ�
		if (this->WhiteTime.Byoyomi == 0)
		{
			// ���Ԃ��Ȃ��ꍇ
			if (this->WhiteTime.Time != 0)
			{
				this->WhiteTime.RemainTime = 60 * 1000; // �b��P��
			}
		}
	}
	else
	{
		this->WhiteTime.RemainTime = this->WhiteTime.Time - whitetime;
	}
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief �v���J�n
 * @note  
 */
/*-----------------------------------------------------------------------------*/
void GameTimer::Start(Color turn)
{
	if (this->started_)
	{
		
	}
	else
	{
		this->started_ = true;
		this->turn_ = turn;

		this->start_time_ = this->GetNowTime();
		int time = (turn == BLACK) ? this->BlackTime.Remain() : this->WhiteTime.Remain();
		
		if (time != 0)
		{
			this->timer_.Start(time + GameTimer::MarginTime);
		}
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief �v����~
* @note
*/
/*-----------------------------------------------------------------------------*/
int GameTimer::Stop()
{
	this->started_ = false;
	this->timer_.Stop();

	return this->CalcTime(GetNowTime());
}


/*-----------------------------------------------------------------------------*/
/**
* @brief�@��Ԑ؂�ւ�
* @note
*/
/*-----------------------------------------------------------------------------*/
int GameTimer::ChangeTurn()
{
	if (!this->started_)
	{
		return 0;
	}

	this->timer_.Stop();

	return this->CalcTime(GetNowTime());
}

/*-----------------------------------------------------------------------------*/
/**
* @brief�@��Ԑ؂�ւ�
* @note
*/
/*-----------------------------------------------------------------------------*/
void GameTimer::TakeTurn()
{
	if (!this->started_)
	{
		return;
	}

	this->turn_ = opp(this->turn_); // ��Ԑ؂�ւ�

	int time = (this->turn_ == BLACK) ? this->BlackTime.Remain() : this->WhiteTime.Remain();

	this->start_time_ = this->GetNowTime();

	if (time != 0)
	{
		// �؂ꕉ���^�C�}�[�v���J�n
		this->timer_.Start(time + GameTimer::MarginTime);
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief�@�c�莞�Ԍv�Z
* @note
*/
/*-----------------------------------------------------------------------------*/
GameRemainTime GameTimer::GetRemainTime(Color color, const GameTime& gameTime) const
{
	TimePoint elapsed_time = 0;
	GameRemainTime remain;

	if (this->turn_ == color && this->started_)
	{
		elapsed_time = (this->GetNowTime() - this->start_time_); // �o�ߎ���ms
	}

	if (gameTime.Time == 0 && gameTime.Byoyomi == 0)
	{
		remain.Time = gameTime.ElapsedTime + (int)elapsed_time; // Total�̌o�ߎ��Ԃɂ���
		remain.Byoyomi = 0;
	}
	else if (gameTime.RemainTime < elapsed_time)
	{
		elapsed_time -= gameTime.RemainTime;

		remain.Time = 0;
		if (gameTime.Byoyomi < elapsed_time)
		{
			remain.Byoyomi = 0;
		}
		else
		{
			remain.Byoyomi = gameTime.Byoyomi - (int)elapsed_time;
		}
	}
	else
	{
		remain.Time = gameTime.RemainTime - (int)elapsed_time;
		remain.Byoyomi = gameTime.Byoyomi;
	}

	remain.ElapsedTime = (int)elapsed_time;
	remain.HaveTime = gameTime.Time;
	remain.HaveByoyomi = gameTime.Byoyomi;

	return remain;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief�@���Ԍv�Z
* @return �b����Ԃ�
* @note
*/
/*-----------------------------------------------------------------------------*/
int GameTimer::CalcTime(TimePoint now_time)
{
	TimePoint elapsed_time = (now_time - this->start_time_); // �o�ߎ���ms
	int expanded_time;
	int old_time;

	// �Œ�P�b���Z�ƕb�����؂�̂Ă͂ǂ����܂���

	if (this->turn_ == BLACK)
	{
		if (this->BlackTime.RemainTime < elapsed_time)
		{
			this->BlackTime.RemainTime = 0;
		}
		else
		{
			this->BlackTime.RemainTime -= static_cast<int>(elapsed_time);
		}

		old_time = this->BlackTime.ElapsedTime;

		this->BlackTime.ElapsedTime += static_cast<int>(elapsed_time);

		expanded_time = (this->BlackTime.ElapsedTime / 1000) - (old_time / 1000);
	}
	else
	{
		if (this->WhiteTime.RemainTime < elapsed_time)
		{
			this->WhiteTime.RemainTime = 0;
		}
		else
		{
			this->WhiteTime.RemainTime -= static_cast<int>(elapsed_time);
		}

		old_time = this->WhiteTime.ElapsedTime;

		this->WhiteTime.ElapsedTime += static_cast<int>(elapsed_time);

		expanded_time = (this->WhiteTime.ElapsedTime / 1000) - (old_time / 1000);
	}

	return expanded_time;
}


/*-----------------------------------------------------------------------------*/
/**
* @brief ���Ԏ擾(ms
* @note
*/
/*-----------------------------------------------------------------------------*/
TimePoint GameTimer::GetNowTime() const
{
	return this->timer_.Now();
}
