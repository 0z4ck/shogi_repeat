#pragma once

#ifndef SHOGI_GAME_H_
#define SHOGI_GAME_H_

#include <memory>
#include <mutex>
#include <thread>


#include "DateTime.h"
#include "EnginePlayer.h"
#include "GameTimer.h"
#include "Handicap.h"
#include "MessageQueue.h"

#include "Notation.h"
#include "History.h"

struct PlayerSetting
{
	std::string Name;
	std::wstring WName;
	std::string FileName;
	std::string SettingFileName;

	// ���Ԑݒ�
	int time  = 0;    // ��������(�b
	int byoyomi = 1; // �b�ǂ�(�b
	
};

struct GameParam
{
	PlayerSetting Black; // ���ݒ�
	PlayerSetting White; // ���ݒ�

	bool PonderON = false; // Ponder�ݒ�

	bool SaveLog = true;      // ���O�ۑ�
	bool SaveNotation = true; // �����ۑ�
	bool DispLog = false;      // ���O��stdout�֏o��

	std::string LogPath = ""; // ���O��ۑ�����p�X
	std::string KifPath = ""; // ������ۑ�����p�X

	Handicap handicap = Handicap::HIRATE; // �荇��

	// �����̓r���ǖʂ���J�n
	bool UseNotation = false; // 
	Notation notation;  // ����
	int Num = -1;            // �J�n�萔

	bool SwapPlayer = true; // ������ւ�

	int MaxPlays = 1; // �΋ǐ�
	int MaxMoves = 256; // ���������ɂ���萔   <=0 �Ŕ���Ȃ�
};

class GameEngineListner : public EnginePlayerListener
{
	MessageQueue* msgq_;

public:
	GameEngineListner(MessageQueue* msgq)
	{
		this->msgq_ = msgq;
	}

	virtual ~GameEngineListner() {}

	virtual void notifyInisialized(Color color);
	virtual void notifyReadyOk(Color color);
	virtual void notifyBestMove(Color color, int transactionNo, const Move& bestmove, const Move& ponder);

	virtual void notifyInfo(Color color, int transactonNO, const PvInfo& str);

	virtual void notifyError(Color color, int error, const std::string& errorMsg);

};

// �A���΋ǂ���N���X
class Game
{
	std::thread th_; // �΋ǂ𓮂����X���b�h
	bool cancel_ = false; // �L�����Z���t���O�@�L�����Z�������Ɨ���
	bool terminate_ = false; // �I���t���O
	bool play_ = false; // �΋ǒ��t���O game_play ����game_end�܂�
	GameParam param_;

	std::unique_ptr<EnginePlayer> black_player_;
	std::unique_ptr<EnginePlayer> white_player_;

	GameEngineListner engine_listner_;
	
	MessageQueue msgq_; // ���b�Z�[�WQ

	std::mutex mtx_;

	Notation notation_; // ����
	History history_; // ����蔻��p

	GameTimer timer_; // ���Ԑ؂�喃

	DateTime start_date_; // �΋ǊJ�n����
	int count_ = 0;  // �΋ǐ�
	int black_win_ = 0;
	int white_win_ = 0;
	bool swap_flag = false;
public:
	Game();
	~Game();

	void Start(const GameParam& param);

	void Cancel() { this->cancel_ = true; this->msgq_.Send(std::make_shared<Message>(MessageID::Cancel)); }
	bool IsCancel() const { return this->cancel_; }

	void Terminate();
	void SaveOption(Color color, const std::string& filename);

private:

	void main_loop();
	bool is_continuous_game_end();

	void game_init();
	void game_start();
	void game_end();
	void game_term();

	void print_log();

	// �C�x���g
	void EventPost(MessagePost& msg);
	void EventCancel(Message& msg);

	void EventTimeout(Message& msg);

	void EventInitialized(MessageInitialized& msg);
	void EventReady(MessageReady& msg);
	void EventBestMove(MessageBestMove& msg);


	// ���̑�
	void LoadOption(Color color, const std::string& filename);
	std::string GetLogName(DateTime date);
	std::string GetKifuFileName(DateTime date, const std::string& black_name, const std::string& white_name);

	std::unique_ptr<EnginePlayer>& current_player() {
		return (this->notation_.position().side_to_move() == BLACK) ? this->black_player_ : this->white_player_;
	}

	std::unique_ptr<EnginePlayer>& opp_player() {
		return (this->notation_.position().side_to_move() == BLACK) ? this->white_player_ : this->black_player_;
	}

	void AddComment(const std::map<int, PvInfo>& pvinfos);

	// �R�[���o�b�N
	void timeout();

};

#endif
