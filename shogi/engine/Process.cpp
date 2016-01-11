#include <Windows.h>

#include <iostream>

#include "Process.h"


#define ERROR_LOG(msg, x) std::cerr << __FILE__ << " : " << __LINE__ << " : " << (msg) << (x) << std::endl


Process::Process()
{
}


Process::~Process()
{
	this->Close();
}


/*-----------------------------------------------------------------------------*/
/**
 * @brief �v���Z�X�J�n
 * @param 
 * @note  
 */
/*-----------------------------------------------------------------------------*/
bool Process::Start()
{
	std::unique_lock<std::mutex> lock(this->mtx_);

	if (this->hChildProcess)
	{
		return false; // �΂��悯
	}

	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);

	PROCESS_INFORMATION  pi;
	memset(&pi, 0, sizeof(pi));


	DWORD dwCreationFlags = CREATE_NEW_CONSOLE;
	if (this->StartInfo.CreateNoWindow) 
	{
		// �ǂ����g���́H
		// si.wShowWindow |= SW_HIDE;
		dwCreationFlags |= CREATE_NO_WINDOW;
	}

	si.dwFlags |= STARTF_USESTDHANDLES;

	
	// Std out
	if (this->StartInfo.RedirectStandardOutput)
	{
		this->StdOut.Open();
		si.hStdOutput = this->StdOut.WrteHandle();
	}
	else
	{
		si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE); // �e�v���Z�X�̕W���o�͂��p��
	}

	// STD Error
	if (this->StartInfo.RedirectStandardError)
	{
		this->StdErr.Open();
		si.hStdError = this->StdErr.WrteHandle();
	}
	else
	{
		si.hStdError = GetStdHandle(STD_OUTPUT_HANDLE); // �e�v���Z�X�̕W���o�͂��p��
	}

	// STD IN
	if (this->StartInfo.RedirectStandardInput)
	{
		this->StdIn.Open();
		si.hStdInput = this->StdIn.ReadHandle();
	}
	
	
	BOOL ret = CreateProcess(
		NULL,
		&this->StartInfo.FileName[0],
		NULL,
		NULL,
		TRUE,
		dwCreationFlags,
		NULL,
		(this->StartInfo.WorkingDirectory != "") ? &this->StartInfo.WorkingDirectory[0] : NULL, // fullpath?
		&si,
		&pi);

	if (!ret)
	{
		ERROR_LOG("CreateProcess error = ", GetLastError());
	}
	
	this->StdOut.CloseWrite(); 
	this->StdErr.CloseWrite();
	this->StdIn.CloseRead();

	// �T���v���ł͂��̃n���h���h���Ă邯�ǈӖ��킩���
	if (!CloseHandle(pi.hThread)) { 
		ERROR_LOG("CloseHandle error = ", GetLastError());
	}

	this->hChildProcess = pi.hProcess;

	this->exit_thread_ = std::make_unique<std::thread>(&Process::exitThread, this);


	return ret != FALSE;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief �v���Z�X�I���҂�
* @param timeout msec
* @note
*/
/*-----------------------------------------------------------------------------*/
void Process::WaitForExit(int timeout)
{
	HANDLE handle = this->hChildProcess;

	if (handle != 0)
	{
		if (timeout < 0)
		{
			WaitForSingleObject(handle, INFINITE);
		}
		else
		{
			WaitForSingleObject(handle, timeout);
		}
	}
}


/*-----------------------------------------------------------------------------*/
/**
* @brief �n���h������
* @note
*/
/*-----------------------------------------------------------------------------*/
void Process::Close()
{
	bool join = false;
	{
		std::unique_lock<std::mutex> lock(this->mtx_);
		if (this->hChildProcess)
		{

			this->exited_ = nullptr;

			TerminateProcess(this->hChildProcess, 0);

			CloseHandle(this->hChildProcess);
			this->hChildProcess = 0;

			// �v���Z�X�̃n���h������Ă���A�p�C�v�̃n���h�������
			this->StdIn.Close();
			this->StdErr.Close();
			this->StdOut.Close();
			
			join = true;
		}
	}

	if (join)
	{
		// �ꉞ�I����҂�
		this->exit_thread_->join();
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief Exit�R�[���o�b�N�ݒ� 
* @param
* @note �R�[���o�b�N�͕ʃX���b�h����Ăяo����邱�Ƃɒ���
*/
/*-----------------------------------------------------------------------------*/
void Process::SetExited(std::function<void()> func)
{
	std::unique_lock<std::mutex> lock(this->mtx_);

	this->exited_ = func;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief Exit�̊Ď�
* @note process�̏I�������m������R�[���o�b�N�֐����Ăяo��
*/
/*-----------------------------------------------------------------------------*/
void Process::exitThread()
{
	WaitForSingleObject(this->hChildProcess, INFINITE);

	{
		std::unique_lock<std::mutex> lock(this->mtx_);

		if (this->exited_ != nullptr)
		{
			this->exited_(); // callback������Process�֘A�̏������Ăяo���Ǝ��ʂ��A����Ȃ��Ƃ��Ȃ���ˁH
		}
	}
}
