
#include "PvInfo.h"

#include <sstream>

#include "Move.h"
#include "StringUtil.h"

#include "Kif.h"

/*-----------------------------------------------------------------------------*/
/**
 * @brief �R���X�g���N�^
 * @note  
 */
/*-----------------------------------------------------------------------------*/
PvInfo::PvInfo(int num, int timeMs, int score, int mate, long long nodes, int depth, int seldepth, const Moves& moves)
{
	this->Num = num;
	this->TimeMs = timeMs;
	this->Score = score;
	this->Mate = mate;
	this->Nodes = nodes;
	this->Depth = depth;
	this->SelDepth = seldepth;
	this->PvMoves = std::make_unique<Moves>(moves);
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief ������ϊ�
 * @note  
 */
/*-----------------------------------------------------------------------------*/
std::string PvInfo::ToString() const
{
	std::stringstream ss;

	// multi pv
	if (this->Num != 0)
	{
		ss << "���" << this->Num;
	}

	// ����
	{
		int min = this->TimeMs / (60 * 1000);
		int sec = (this->TimeMs - (min * 60 * 1000)) / 1000;
		int msec = (this->TimeMs - (min * 60 * 1000)) % 1000;

		ss << StringUtil::Format(" ���� %02d:%02d.%d", min, sec, msec / 100);
	}

	// �[��
	{
		ss << " �[�� " << this->Depth << "/" << this->SelDepth;
	}

	// �m�[�h��
	{
		ss << " �m�[�h�� " << this->Nodes;
	}

	if (this->Mate == MateNone)
	{
		int eval = this->Score;

		ss << " �]���l " << this->Score;

	}
	else
	{
		ss << " �]���l " << ((this->Mate < 0) ? "-�l" : "+�l");
		if (this->Score != 0)
		{
			ss << " " << this->Score;
		}
	}

	// pv;
	if (this->PvMoves->size() != 0)
	{
		ss << " �ǂ݋�";
		for (auto& move : *this->PvMoves)
		{
			ss << " " << ((move.side() == BLACK) ? "��" : "��") <<  Kif::ToMoveString(move);
		}
	}

	return ss.str();
}