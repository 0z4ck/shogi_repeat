#pragma once
#ifndef ENGINE_PLAYER_H_
#define ENGINE_PLAYER_H_

#include <map>
#include <mutex>
#include <memory>
#include <thread>
#include <string>

#include "Move.h"
#include "Timer.h"
#include "Position.h"
#include "PvInfo.h"

class USIEngine;
class USIOptions;
class Notation;
class GameTimer;


// �������
enum class EnginePlayerState {
	NONE,
	INITIALIZING, // usi -> usiok �܂�
	INISIALIZED,  // usiok����isready�𑗂�܂�
	WAIT_READY,   // isready -> readyok�܂�
	GO,           // go -> bestmove
	PONDER,       // go -> bestmove or go -> ponderhit -> bestmove
	STOP,         // stop -> bestmove
	IDLE,         // isready���I���������

	TERMINATING, // quit -> �v���Z�X���S�܂�
};


// �C�x���g�ʒm�R�[���o�b�N 
// ��color�����͎��ʎq�̂ق����悢����
class EnginePlayerListener
{
public:
	EnginePlayerListener() {}
	virtual ~EnginePlayerListener() {}

	virtual void notifyInisialized(Color color) = 0;
	virtual void notifyReadyOk(Color color) = 0;
	virtual void notifyBestMove(Color color, int transactionNo, const Move& bestmove, const Move& ponder) = 0;

	virtual void notifyInfo(Color color, int transactonNO, const PvInfo& info) = 0;

	virtual void notifyError(Color color, int errnoNo, const std::string& errorMsg) = 0;
};

// go���N�G�X�g
struct GoRequest
{
	enum Type {
		NORMAL,   // �ʏ�΋Ǘp
		TIME_INFINITY, // ���Ԗ�����
		PONDER,   // ��ǂ�
//		MATE,     // �l��
//		MOVETIME,    // ���ԑ��w��
	};

	GoRequest::Type ReqType;

	int Btime;
	int Wtime;
	int Byoyomi;

	// type��MOVETIME�̂Ƃ��ȉ��̂R���g����
	int64_t Nodes; // �m�[�h�� �}�C�i�X�̒l���Ɩ��g�p
	int Depth;     // �[��
	int Time;      // ����ms

	std::string Sfen;
	std::string Moves;
	int transactionNo;
	Position Pos;

	GoRequest()
	{
		this->ReqType = Type::TIME_INFINITY;
		this->Sfen = "";
		this->Moves = "";
	}

	GoRequest(int btime, int wtime, int byoyomi)
	{
		this->ReqType = Type::NORMAL;
		this->Btime = btime;
		this->Wtime = wtime;
		this->Byoyomi = byoyomi;
		this->Sfen = "";
		this->Moves = "";
	}

	GoRequest(GoRequest::Type type)
	{
		this->ReqType = type;
		this->Sfen = "";
		this->Moves = "";
	}
};


class EnginePlayer
{
	Color color_;
	std::unique_ptr<USIEngine> engine_;
	std::unique_ptr<std::thread> th_;
	std::mutex mtx_;

	EnginePlayerState state_;

	bool cancel_ = false; // �L�����Z���t���O
	EnginePlayerListener* lisnter_;
	
	int transactionCounter_;
	int transactionNo_;
	Position pos_;
	std::map<int, PvInfo> pv_info_;

	bool is_go_req_ = false;
	GoRequest go_req_;

	std::unique_ptr<Move> ponder_;

	std::string name_;
	std::string author_;

	Timer<EnginePlayer*> timer_;

public:
	EnginePlayer(Color color, EnginePlayerListener* lisnter);
	~EnginePlayer();


	bool Init(const std::string& filename);
	void Terminate();
	void Ready();

	int Go(Notation& notation, const GameTimer& time_info);
	int Ponder(const Notation& notation);
	void Stop();

	void Analyze();

	void GameStart();
	void GameOver(Color color);


	void SetOptions(const std::map<std::string, std::string>& opt_name_value);
	const USIOptions& GetOptions() const;

	const std::string& Name() const { return this->name_; }
	const std::string& Author() const { return this->author_; }

	const std::map<int, PvInfo>& PvInfos() const {
		return this->pv_info_;
	}

private:
	
	// �R�}���h��M����
	void receive_thread();
	void receive_command(const std::string& str);
	void parse_option(const std::string& str);

	void parse_bestmove(const std::string& str);
	void parse_info(const std::string& str);
	void handleIdleState();

	void ExecGoReeust(const GoRequest& req);
	void send_cmd(const std::string &cmd);

	static std::string time_str(TimePoint tp);

	void InitTimeout();
};


#endif
