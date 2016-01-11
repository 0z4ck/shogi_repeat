#pragma once
#ifndef SHOGI_PV_INFO_H_
#define SHOGI_PV_INFO_H_

#include <string>
#include <memory>
class Moves;

struct PvInfo
{
	int Num;    // multipv�̒l�@���ݒ�̏ꍇ��0
	int TimeMs; // ����
	int Score;    // �X�R�A
	int Mate;	  // 0 �ʏ�X�R�A < 0 ���� > 0 ����
	long long Nodes;  // �m�[�h��
	int Depth;        // �[��
	int SelDepth;
	std::unique_ptr<Moves> PvMoves; // �ǂ݋�
	PvInfo() {};
	PvInfo(int num, int timeMs, int score, int mate, long long nodes, int depth, int seldepth, const Moves& moves);

	static const int MateNone = 0;
	static const int MateBlack = 1; // ��肩�猩�Ď���������
	static const int MateWhite = -1;  // ��肩�猩�đ��肪����

	std::string ToString() const;
};




#endif
