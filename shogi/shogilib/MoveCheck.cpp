
#include <cstring>

#include "MoveCheck.h"


#include "Position.h"


#define NUMBER_OF(x) (sizeof(x) / sizeof((x)[0]))


const MoveCoord MoveCheck::FuMoveOfs[] = 
{
	{ 0, -1}
};

const MoveCoord MoveCheck::KeiMoveOfs[] = {
	{ -1, -2 }, {1, -2},
};

const MoveCoord MoveCheck::GinMoveOfs[] =
{
	{ -1, -1 },{ 0, -1 },{ 1, -1 },
	{ -1,  1 },          { 1,  1 },
};

const MoveCoord MoveCheck::KinMoveOfs[] = 
{
	{ -1, -1 },{ 0, -1 },{ 1, -1 },
	{ -1,  0 },          { 1,  0 },
	           { 0,  1 },          
};

const MoveCoord MoveCheck::GyokuMoveOfs[] = 
{
	{ -1, -1 },{ 0, -1 },{ 1, -1 },
	{ -1,  0 },          { 1,  0 },
	{ -1,  1 },{ 0,  1 },{ 1,  1 },
};



/*-----------------------------------------------------------------------------*/
/**
 * @brief �ړ��̉۔���@�w���肪�ȉ��̔����̏ꍇ��false��Ԃ�
 * @note 1. �����Ȃ��Ƃ���ɋ�����߂�
 * @note 2. ���
 * @note 3. �ł����l��
 * @note 4. ������u
 */
/*-----------------------------------------------------------------------------*/
bool MoveCheck::IsValid(const Position& pos, const Move& move)
{
	bool ret = true;

	if (move.move_type() == MoveType::PASS)
	{
		return true;
	}

	ret = MoveCheck::MoveCheckParam(pos, move);

	if (ret)
	{
		if (move.move_type() & DROP_FLAG)
		{
			ret = MoveCheck::MoveCheckDrop(pos, move);
		}
		else
		{
			ret = MoveCheck::MoveCheckMove(pos, move);
		}

		// ������u�`�F�b�N
		if (ret)
		{
			ret = !MoveCheck::IsMateLeft(pos, move);
		}
	}

	return ret;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief ����`�F�b�N
*/
/*-----------------------------------------------------------------------------*/
bool MoveCheck::IsCheck(const Position& pos)
{
	bool ret = false;

	if (pos.side_to_move() == WHITE)
	{
		ret = IsMateLeft<WHITE>(pos);
	}
	else
	{
		ret = IsMateLeft<BLACK>(pos);
	}

	return ret;
}


/*-----------------------------------------------------------------------------*/
/**
* @brief �p�����[�^�`�F�b�N
*/
/*-----------------------------------------------------------------------------*/
bool MoveCheck::MoveCheckParam(const Position& pos, const Move& move)
{
	// move�̓��e���m�F

	if (!is_move(move.move_type()))
	{
		return false;
	}

	// �s�[�X�̃`�F�b�N
	if (move.piece() == Piece::NO_PIECE)
	{
		return false;
	}

	// �^�[�����Ⴄ�ꍇ
	if (pos.side_to_move() != color_of(move.piece()) || pos.side_to_move() != move.side())
	{
		// Debug.Assert(false, "�����G���[");
		return false;
	}

	if (!in_board(move.to()))
	{
		return false;
	}

	if (move.move_type() & DROP_FLAG)
	{
		// �ł�

		//�@�]�v�ȃt���O�������Ă��Ȃ�
		if (move.move_type() & (SAME_FLAG | PROMOTION_FLAG))
		{
			return false;
		}

		if (is_promoted(move.piece()))
		{
			return false;
		}
	}
	else
	{
		if (!in_board(move.from()))
		{
			return false;
		}

		if (move.move_type() & PROMOTION_FLAG)
		{
			if (!MoveCheck::CanPromota(move))
			{
				return false;
			}
		}
		else
		{
			if (MoveCheck::ForcePromotion(move.piece(), move.to()))
			{
				return false;
			}
		}
	}

	return true;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief �ł�̃`�F�b�N
*/
/*-----------------------------------------------------------------------------*/
bool MoveCheck::MoveCheckDrop(const Position& pos, const Move& move)
{
	// ������`�F�b�N
	if (!pos.IsHand(pos.side_to_move(), type_of(move.piece())))
	{
		return false;
	}

	// �łƂ���̋�`�F�b�N
	if (pos.GetPiece(move.to()) != Piece::NO_PIECE)
	{
		// ������������ꍇ
		return false;
	}

	switch (move.piece())
	{
	case B_PAWN:
	case B_LANCE:
		if (rank_of(move.to()) <= Rank::RANK_1)
		{
			// 1�i�ڂɂ͑łĂ܂���
			return false;
		}

		break;
	case W_PAWN:
	case W_LANCE:
		// ���
		if (rank_of(move.to()) >= Rank::RANK_9)
		{
			// 9�i�ڂɂ͑łĂ܂���
			return false;
		}
		break;
	case B_KNIGHT:
		if (rank_of(move.to()) <= Rank::RANK_2)
		{
			// 1,2�i�ڂɂ͑łĂ܂���
			return false;
		}

		break;
	case W_KNIGHT:
		if (rank_of(move.to()) >= Rank::RANK_8)
		{
			// 8,9�i�ڂɂ͑łĂ܂���
			return false;
		}

		break;
	}

	if (move.piece() == Piece::B_PAWN || move.piece() == Piece::W_PAWN)
	{
		// ����`�F�b�N
		if (MoveCheck::PieceCountInFile(pos, move.piece(), file_of(move.to())) >= 1)
		{
			// 2���₪��
			return false;
		}

		if (pos.side_to_move() == BLACK)
		{
			if (MoveCheck::IsPawnDropMate<BLACK> (pos, move))
			{
				// �ł����l
				return false;
			}
		}
		else
		{
			if (MoveCheck::IsPawnDropMate<WHITE>(pos, move))
			{
				// �ł����l
				return false;
			}
		}
	}


	return true;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief �ړ������̃`�F�b�N
*/
/*-----------------------------------------------------------------------------*/
bool MoveCheck::MoveCheckMove(const Position& pos, const Move& move)
{
	// �ړ���̃`�F�b�N
	Piece from_piece = pos.GetPiece(move.to());

	if (from_piece != Piece::NO_PIECE)
	{
		// �ړ���Ɍ��݂̃^�[���̃R�}���������̋����
		if (pos.side_to_move() == color_of(from_piece))
		{
			// �����̋�̂���Ƃ���ɂ͈ړ��o���܂���
			return false;
		}
	}

	bool move_check = false;
	
	int move_rank_ofs;
	int move_file_ofs;

	move_rank_ofs = rank_of(move.to()) - rank_of(move.from());
	move_file_ofs = file_of(move.to()) - file_of(move.from());

	switch (move.piece())
	{
	case Piece::B_PAWN: // ��
		move_check = CanMove(FuMoveOfs, NUMBER_OF(FuMoveOfs), move_file_ofs, move_rank_ofs);
		break;
	case Piece::W_PAWN: // ��
					// �`�F�b�N�p�ɕ����𔽓]������
		move_check = CanMove(FuMoveOfs, NUMBER_OF(FuMoveOfs), -move_file_ofs, -move_rank_ofs);
		break;
	case Piece::B_LANCE: // ����
		if (move_rank_ofs >= 0)
		{
			// �t�����ɂ͓����܂���
			move_check = false;
		}
		else
		{
			move_check = CanMoveRank(pos, move);
		}

		break;
	case Piece::W_LANCE: // ����
		if (move_rank_ofs <= 0)
		{
			move_check = false;
		}
		else
		{
			move_check = CanMoveRank(pos, move);
		}

		break;
	case Piece::B_KNIGHT: // �j�n
		move_check = CanMove(KeiMoveOfs, NUMBER_OF(KeiMoveOfs), move_file_ofs, move_rank_ofs);
		break;
	case Piece::W_KNIGHT: // �j�n
		move_check = CanMove(KeiMoveOfs, NUMBER_OF(KeiMoveOfs), -move_file_ofs, -move_rank_ofs);
		break;
	case Piece::B_SILVER: // ��
		move_check = CanMove(GinMoveOfs, NUMBER_OF(GinMoveOfs), move_file_ofs, move_rank_ofs);
		break;
	case Piece::W_SILVER: // ��
		move_check = CanMove(GinMoveOfs, NUMBER_OF(GinMoveOfs), -move_file_ofs, -move_rank_ofs);
		break;
	case Piece::B_BISHOP: // �p
	case Piece::W_BISHOP: // �p
		move_check = CanMoveDiagonal(pos, move);
		break;
	case Piece::B_PRO_BISHOP: // �n
	case Piece::W_PRO_BISHOP: // �n
		if (pos.side_to_move() == Color::WHITE)
		{
			// �`�F�b�N�p�ɕ����𔽓]������
			move_check = CanMove(GyokuMoveOfs, NUMBER_OF(GyokuMoveOfs), -move_file_ofs, -move_rank_ofs);
		}
		else
		{
			move_check = CanMove(GyokuMoveOfs, NUMBER_OF(GyokuMoveOfs), move_file_ofs, move_rank_ofs);
		}

		if (!move_check)
		{
			move_check = CanMoveDiagonal(pos, move);
		}

		break;
	case Piece::B_ROOK: // ���
	case Piece::W_ROOK: // ���

		move_check = CanMoveRank(pos, move);
		if (!move_check)
		{
			move_check = CanMoveFile(pos, move);
		}

		break;
	case Piece::B_PRO_ROOK: // ��
	case Piece::W_PRO_ROOK: // ��
		if (pos.side_to_move() == Color::WHITE)
		{
			// �`�F�b�N�p�ɕ����𔽓]������
			move_check = CanMove(GyokuMoveOfs, NUMBER_OF(GyokuMoveOfs), -move_file_ofs, -move_rank_ofs);
		}
		else
		{
			move_check = CanMove(GyokuMoveOfs, NUMBER_OF(GyokuMoveOfs), move_file_ofs, move_rank_ofs);
		}

		if (!move_check)
		{
			move_check = CanMoveRank(pos, move);
			if (!move_check)
			{
				move_check = CanMoveFile(pos, move);
			}
		}

		break;
	case Piece::B_KING: // ��
		move_check = CanMove(GyokuMoveOfs, NUMBER_OF(GyokuMoveOfs), move_file_ofs, move_rank_ofs);
		break;
	case Piece::W_KING: // ��
					// �`�F�b�N�p�ɕ����𔽓]������
		move_check = CanMove(GyokuMoveOfs, NUMBER_OF(GyokuMoveOfs), -move_file_ofs, -move_rank_ofs);
		break;
	case Piece::B_GOLD:
	case Piece::W_GOLD:
	default:
		// ���̓����������
		if (pos.side_to_move() == Color::WHITE)
		{
			// �`�F�b�N�p�ɕ����𔽓]������
			move_check = CanMove(KinMoveOfs, NUMBER_OF(KinMoveOfs), -move_file_ofs, -move_rank_ofs);
		}
		else
		{
			move_check = CanMove(KinMoveOfs, NUMBER_OF(KinMoveOfs), move_file_ofs, move_rank_ofs);
		}

		break;
	}

	if (!move_check)
	{
		return false;
	}

	return true;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief ������u�`�F�b�N
* @note
*/
/*-----------------------------------------------------------------------------*/
bool MoveCheck::IsMateLeft(const Position& pos, const Move& move)
{
	Position pos_next = pos;
	bool ret = false;

	if (pos_next.DoMove(move))
	{
		if (pos.side_to_move() == WHITE)
		{
			ret = IsMateLeft<WHITE>(pos_next);
		}
		else
		{
			ret = IsMateLeft<BLACK>(pos_next);
		}
	}

	return ret;
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief �c�����̎w���J�E���g�i2���`�F�b�N�Ɏg���܂�
 * @note  
 */
/*-----------------------------------------------------------------------------*/
int MoveCheck::PieceCountInFile(const Position& pos, Piece piece, File file)
{
	int cnt = 0;

	for (Rank rank = RANK_1; rank < RANK_NB; rank += 1)
	{
		if (pos.GetPiece(file, rank) == piece)
		{
			cnt++;
		}
	}

	return cnt;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief �����f�[�^�N���A
* @note
*/
/*-----------------------------------------------------------------------------*/
void MoveCheck::ClearEffectData(int effect_data[SQUARE_NB])
{
	memset(effect_data, 0, sizeof(int) * (int)SQUARE_NB);
}

/*-----------------------------------------------------------------------------*/
/**
* @brief �����쐬
* @note
*/
/*-----------------------------------------------------------------------------*/
template<Color Us>
void MoveCheck::MakeEffectAll(const Position& pos, int effect_data[SQUARE_NB], Piece skip_piece)
{
	Piece piece;

	for (Square sq = SQ_ZERO; sq < SQUARE_NB; sq += 1)
	{
		piece = pos.GetPiece(sq);
		if (color_of(piece) == Us && piece != skip_piece)
		{
			// ���̋����
			// �����f�[�^�̍쐬
			MakeEfectOne<Us>(pos, sq, effect_data);
		}
	}
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief ����̋�̌����쐬
 * @param 
 * @note  
 */
/*-----------------------------------------------------------------------------*/

template<Color Us>
void MoveCheck::MakeEfectOne(const Position& pos, Square square, int effect_data[SQUARE_NB])
{
	Piece piece = pos.GetPiece(square);

	// Debug.Assert(piece.ColorOf() == Color::BLACK, "�����G���[");

	Rank rank = rank_of(square);
	File file = file_of(square);

	// �����f�[�^���쐬
	switch (piece)
	{
	case Piece::B_PAWN: // ��
	case Piece::W_PAWN: // ��
		MakeEffect(Us, effect_data, FuMoveOfs, NUMBER_OF(FuMoveOfs), file, rank);
		break;
	case Piece::B_LANCE: // ����
	case Piece::W_LANCE: // ����
		MakeEffect(pos, effect_data, file, rank, 0, -1);

		break;
	case Piece::B_KNIGHT: // �j�n
	case Piece::W_KNIGHT: // �j�n
		MakeEffect(Us, effect_data, KeiMoveOfs, NUMBER_OF(KeiMoveOfs), file, rank);
		break;
	case Piece::B_SILVER: // ��
	case Piece::W_SILVER: // ��
		MakeEffect(Us, effect_data, GinMoveOfs, NUMBER_OF(GinMoveOfs), file, rank);

		break;
	case Piece::B_BISHOP: // �p
	case Piece::W_BISHOP: // �p
		MakeEffect(pos, effect_data, file, rank, -1, -1);
		MakeEffect(pos, effect_data, file, rank, 1, -1);
		MakeEffect(pos, effect_data, file, rank, -1, 1);
		MakeEffect(pos, effect_data, file, rank, 1, 1);

		break;
	case Piece::B_PRO_BISHOP: // �n
	case Piece::W_PRO_BISHOP: // �n
		MakeEffect(Us, effect_data, GyokuMoveOfs, NUMBER_OF(GyokuMoveOfs), file, rank);

		MakeEffect(pos, effect_data, file, rank, -1, -1);
		MakeEffect(pos, effect_data, file, rank, 1, -1);
		MakeEffect(pos, effect_data, file, rank, -1, 1);
		MakeEffect(pos, effect_data, file, rank, 1, 1);

		break;
	case Piece::B_ROOK: // ���
	case Piece::W_ROOK: // ���
		MakeEffect(pos, effect_data, file, rank, 0, -1);
		MakeEffect(pos, effect_data, file, rank, 0, 1);
		MakeEffect(pos, effect_data, file, rank, -1, 0);
		MakeEffect(pos, effect_data, file, rank, 1, 0);
		break;
	case Piece::B_PRO_ROOK: // ��
	case Piece::W_PRO_ROOK: // ��
		MakeEffect(Us, effect_data, GyokuMoveOfs, NUMBER_OF(GyokuMoveOfs), file, rank);
		MakeEffect(pos, effect_data, file, rank, 0, -1);
		MakeEffect(pos, effect_data, file, rank, 0, 1);
		MakeEffect(pos, effect_data, file, rank, -1, 0);
		MakeEffect(pos, effect_data, file, rank, 1, 0);
		break;
	case Piece::B_KING: // ��
	case Piece::W_KING: // ��
		MakeEffect(Us, effect_data, GyokuMoveOfs, NUMBER_OF(GyokuMoveOfs), file, rank);
		break;
	case Piece::B_GOLD:
	case Piece::W_GOLD:
	default:
		MakeEffect(Us, effect_data, KinMoveOfs, NUMBER_OF(KinMoveOfs), file, rank);

		break;
	}
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief �����쐬
 */
/*-----------------------------------------------------------------------------*/
void MoveCheck::MakeEffect(Color side, int effect_data[SQUARE_NB], const MoveCoord move_ofs[], size_t num, File move_to_file, Rank move_to_rank)
{
	File file;
	Rank rank;

	for (size_t i = 0; i < num;i++ )
	{
		const MoveCoord& ofs = move_ofs[i];

		if (side == BLACK)
		{
			rank = Rank(move_to_rank + ofs.rank);
			file = File(move_to_file + ofs.file);
		}
		else
		{
			rank = Rank(move_to_rank - ofs.rank);
			file = File(move_to_file - ofs.file);
		}

		if (Position::InBoard(file, rank))
		{
			effect_data[ make_square(file, rank)]++; // ����������܂���
		}
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief ��Ԋp�A���Ԃ̗����f�[�^�쐬
*/
/*-----------------------------------------------------------------------------*/
void MoveCheck::MakeEffect(const Position& pos, int effect_data[SQUARE_NB], File file, Rank rank, int file_ofs, int rank_ofs)
{
	for (int i = 0; i < RANK_NB; i++)
	{
		rank += rank_ofs;
		file += file_ofs;

		if (!Position::InBoard(file, rank))
		{
			break;
		}

		Square sq = make_square(file, rank);
		effect_data[sq]++;

		if (pos.GetPiece(sq) != Piece::NO_PIECE)
		{
			// �����΂����ŏI��
			break;
		}
	}
}


/*-----------------------------------------------------------------------------*/
/**
* @brief �ł����l�߃`�F�b�N
*/
/*-----------------------------------------------------------------------------*/
template<Color Us>
bool MoveCheck::IsPawnDropMate(const Position& pos, const Move& move)
{
	Position pos_next = pos;

	if (!pos_next.DoMove(move))
	{
		// �Ֆʓ������̂Ɏ��s�H

		return false;
	}

	// �w������̌����ɑ���̋ʂ����� => ���肵��
	if (!MoveCheck::IsCheck<Us>(pos_next, move.to()))
	{
		return false;
	}

	int effect_data[SQUARE_NB];

	MoveCheck::ClearEffectData(effect_data);

	// �����f�[�^�̍쐬
	MakeEffectAll<Us>(pos_next, effect_data, Piece::NO_PIECE);

	// �w������ɐ��̌��������邩�H
	if (effect_data[move.to()] == 0)
	{
		// �������Ȃ���
		return false;
	}

	// ���ʂ̈ړ���ɐ��̌��������邩�H

	// ����ʂ̈ʒu���擾
	const Color opp_color = opp(Us);
	const Piece king = make_piece(opp_color, KING);

	Square sq;
	if ((sq = pos_next.SearchPiece(king)) == SQUARE_NB)
	{
		// ����ʂ��Ȃ���
		return false;
	}

	Rank king_rank = rank_of(sq);
	File king_file = file_of(sq);

	// ����ʂ̈ړ���̐������𒲂ׂ�
	for (auto& ofs : GyokuMoveOfs)
	{
		Rank rank;
		File file;

		if (Us == BLACK)
		{
			rank = Rank(king_rank - ofs.rank);
			file = File(king_file - ofs.file);
		}
		else
		{
			rank = Rank(king_rank + ofs.rank);
			file = File(king_file + ofs.file);
		}

		if (Position::InBoard(file, rank) && !(pos_next.ColorOf(file, rank) == opp_color))
		{
			// �{�[�h���Ŏ����̋�ȊO�̏ꏊ�̑S���̌����`�F�b�N

			if (effect_data[make_square(file, rank)] == 0)
			{
				// �ړ���Ɍ����͂Ȃ�������
				return false;
			}
		}
	}

	bool ret;
	
	if (Us == BLACK)
	{
		ret = MoveCheck::response<WHITE>(pos_next, move.to(), king);
	}
	else
	{
		ret = MoveCheck::response<BLACK>(pos_next, move.to(), king);
	}

	if (ret)
	{
		// ���肪����
		return false;
	}

	// �����܂ł���Αł����l��
	return true;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief ����ɑ΂��鉞��
*/
/*-----------------------------------------------------------------------------*/
template<Color Us>
bool MoveCheck::response(Position& pos, Square to_sq, Piece skip_piece)
{
	Piece piece;
	Square sq;
	int effect_data[SQUARE_NB];

	Piece cap_piece = pos.GetPiece(to_sq);

	for (sq = SQ_ZERO; sq < SQUARE_NB; sq += 1)
	{
		piece = pos.GetPiece(sq);

		if (color_of(piece) == Us && skip_piece != piece)
		{
			// ���̋����
			// �����f�[�^�̍쐬
			MoveCheck::ClearEffectData(effect_data);
			MakeEfectOne<Us>(pos, sq, effect_data);

			if (effect_data[to_sq] != 0)
			{
				Move movedata(MoveType::NORMAL, sq, to_sq, piece, cap_piece);
				pos.DoMove(movedata);

				if (!MoveCheck::IsMateLeft<Us>(pos))
				{
					return true;
				}

				pos.UndoMove(movedata);
			}
		}
	}

	return false;
}


/*-----------------------------------------------------------------------------*/
/**
* @brief ���蔻��
*/
/*-----------------------------------------------------------------------------*/
template<Color Us>
bool MoveCheck::IsCheck(const Position& pos, Square square)
{
	Piece piece;

	piece = pos.GetPiece(square);

	int effect_data[SQUARE_NB];

	MoveCheck::ClearEffectData(effect_data);

	// �w������̌����f�[�^���쐬
	MoveCheck::MakeEfectOne<Us>(pos, square, effect_data);

	// ����ʂ̈ʒu���擾
	Piece king = make_piece(opp(Us), KING);

	int sq = pos.SearchPiece(king);

	if (sq < SQUARE_NB)
	{
		// ���̋ʂɐ��̗���������
		if (effect_data[sq] != 0)
		{
			return true;
		}
	}

	return false;
}


/*-----------------------------------------------------------------------------*/
/**
 * @brief ���Έʒu�ňړ�����@�����邩�ǂ���
 * @param 
 * @note  
 */
/*-----------------------------------------------------------------------------*/
bool MoveCheck::CanMove(const MoveCoord move_ofs[], int num, int file_ofs, int rank_ofs)
{
	for (int i = 0; i < num; i++)
	{
		const MoveCoord &ofs = move_ofs[i];

		if (ofs.rank == rank_ofs && ofs.file == file_ofs)
		{
			// ��v
			return true;
		}
	}

	return false;
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief �c�����̈ړ��`�F�b�N
 * @note  
 */
/*-----------------------------------------------------------------------------*/
bool MoveCheck::CanMoveRank(const Position& pos, const Move& move)
{
	int ofs;

	// Debug.Assert(move.to() != move.from(), "�����G���[");

	// file�̃`�F�b�N
	if (file_of(move.to()) != file_of(move.from()))
	{
		return false;
	}

	if (move.to() > move.from())
	{
		ofs = FILE_NB;
	}
	else
	{
		ofs = -FILE_NB;
	}

	Square sq = Square(move.from() + ofs);

	for (; sq != move.to(); sq += ofs)
	{
		if (pos.GetPiece(sq) != Piece::NO_PIECE)
		{
			// �w��ꏊ�ɍs���܂łɋ�������ꍇ

			return false;
		}
	}

	return true;
}


/*-----------------------------------------------------------------------------*/
/**
* @brief �������̈ړ��`�F�b�N
* @note
*/
/*-----------------------------------------------------------------------------*/
bool MoveCheck::CanMoveFile(const Position& pos, const Move& move)
{
	int ofs;

	// Debug.Assert(move.to() != move.from(), "�����G���[");

	// rank���Ⴄ�ꍇ�̓G���[
	if (rank_of(move.to()) != rank_of(move.from()))
	{
		return false;
	}

	if (move.to() > move.from())
	{
		ofs = 1;
	}
	else
	{
		ofs = -1;
	}

	Square sq = Square(move.from() + ofs);

	for (; sq != move.to(); sq += ofs)
	{
		if (pos.GetPiece(sq) != Piece::NO_PIECE)
		{
			// �w��ꏊ�ɍs���܂łɋ�������ꍇ

			return false;
		}
	}

	return true;
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief �΂߂̈ړ��`�F�b�N
 */
/*-----------------------------------------------------------------------------*/
bool MoveCheck::CanMoveDiagonal(const Position& pos, const Move& move)
{
	// Debug.Assert(move.to() != move.from(), "�����G���[");

	int rank_ofs, file_ofs;
	int ofs;

	file_ofs = file_of(move.to()) - file_of(move.from());
	rank_ofs = rank_of(move.to()) - rank_of(move.from());

	if (abs(file_ofs) != abs(rank_ofs))
	{
		return false;
	}

	rank_ofs = rank_ofs / abs(rank_ofs);
	file_ofs = file_ofs / abs(file_ofs);

	ofs = (rank_ofs * FILE_NB) + file_ofs;

	Square sq = Square(move.from() + ofs);

	while (move.to() != sq)
	{
		if (!in_board(sq))
		{
			// �ꉞ
			return false;
		}

		if (pos.GetPiece(sq) != Piece::NO_PIECE)
		{
			// �w��ꏊ�ɍs���܂łɋ�������ꍇ

			return false;
		}

		sq += ofs;
	}

	return true;
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief ��葤�̉�����u����
 * @param 
 * @note  
 */
/*-----------------------------------------------------------------------------*/
template<Color Us>
bool MoveCheck::IsMateLeft(const Position& pos)
{
	int effect_data[SQUARE_NB];

	MoveCheck::ClearEffectData(effect_data);

	const Piece king = make_piece(Us, KING);
	Square sq;

	if ((sq = pos.SearchPiece(king)) == SQUARE_NB)
	{
		// �ʂȂ������
		return false;
	}

	// ���̌����Ł[���쐬
	if (Us == BLACK)
	{
		MakeEffectAll<WHITE>(pos, effect_data, Piece::NO_PIECE);
	}
	else
	{
		MakeEffectAll<BLACK>(pos, effect_data, Piece::NO_PIECE);
	}

	// ���ʂɌ��̌��������邩�H
	if (effect_data[sq] == 0)
	{
		// �������Ȃ���
		return false;
	}

	return true;
}


/*-----------------------------------------------------------------------------*/
/**
 * @brief ���蔻��
 * @note  
 */
/*-----------------------------------------------------------------------------*/
bool MoveCheck::CanPromota(const Move& move)
{
	bool ret = false;

	if (!(move.move_type() & MOVE_FLAG))
	{
		// �Տ�̋�𓮂�����̂ݗL��
		return false;
	}

	if (move.move_type() & DROP_FLAG)
	{
		return false;
	}

	Piece piece = move.piece();

	if (is_promoted(piece))
	{
		// �����͐���܂���
		return false;
	}

	if (type_of(piece) == PieceType::GOLD || type_of(piece) == PieceType::KING)
	{
		// ���Ƌ��͐���܂���
		return false;
	}

	if (color_of(piece) == BLACK)
	{
		if (rank_of(move.to()) <= RANK_3 || rank_of(move.from()) <= RANK_3)
		{
			ret = true;
		}
	}
	else
	{
		if (rank_of(move.to()) >= RANK_7 || rank_of(move.from()) >= RANK_7)
		{
			ret = true;
		}
	}

	return ret;
}


/*-----------------------------------------------------------------------------*/
/**
* @brief �������蔻��
* @note
*/
/*-----------------------------------------------------------------------------*/
bool MoveCheck::ForcePromotion(Piece piece, Square square)
{
	if (piece == Piece::B_PAWN || piece == Piece::B_LANCE)
	{
		// ���
		if (rank_of(square) <= Rank::RANK_1)
		{
			return true;
		}
	}
	else if (piece == Piece::W_PAWN || piece == Piece::W_LANCE)
	{
		// ���
		if (rank_of(square) >= Rank::RANK_9)
		{
			// 9�i�ڂɂ͑łĂ܂���
			return true;
		}
	}
	else if (piece == Piece::B_KNIGHT)
	{
		// ���
		if (rank_of(square) <= Rank::RANK_2)
		{
			// 1,2�i�ڂɂ͑łĂ܂���
			return true;
		}
	}
	else if (piece == Piece::W_KNIGHT)
	{
		// ���
		if (rank_of(square) >= Rank::RANK_8)
		{
			// 8,9�i�ڂɂ͑łĂ܂���
			return true;
		}
	}

	return false;
}
