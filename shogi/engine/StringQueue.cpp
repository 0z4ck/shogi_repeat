#include "StringQueue.h"



StringQueue::StringQueue()
{
}


StringQueue::~StringQueue()
{
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief �L���[�ɋl��
 * @note  
 */
/*-----------------------------------------------------------------------------*/
void StringQueue::Push(const std::string& str)
{
	std::unique_lock<std::mutex> lock(this->mtx_); // lock_guard���g���Ƃ��̃X�R�[�v�������b�N�����
	if (this->close_)
	{
		return;
	}

	this->queue_.push_back(std::make_unique<std::string>(str));

	this->sem_.post();
}

void StringQueue::Push(const char* str)
{
	std::unique_lock<std::mutex> lock(this->mtx_); // lock_guard���g���Ƃ��̃X�R�[�v�������b�N�����
	if (this->close_)
	{
		return;
	}

	if (str != nullptr)
	{
		this->queue_.push_back(std::make_unique<std::string>(str));
	}
	else
	{
		this->queue_.emplace_back(); // �����v�f��nullptr
	}

	this->sem_.post();
}


/*-----------------------------------------------------------------------------*/
/**
* @brief �L���[�ɋl��
* @param timeoutMs �ő�҂����� ms
* @note
*/
/*-----------------------------------------------------------------------------*/
STRING_QUEUE_ERR StringQueue::Pop(std::string& ref_str, int timeoutMs)
{
	if (this->close_)
	{
		return STRING_QUEUE_ERR::CLOSE;
	}

	if (!this->sem_.wait(timeoutMs))
	{
		return STRING_QUEUE_ERR::TIMEOUT;
	}

	STRING_QUEUE_ERR err = STRING_QUEUE_ERR::OK;

	{
		std::unique_lock<std::mutex> lock(this->mtx_); // lock_guard���g���Ƃ��̃X�R�[�v�������b�N�����

		if (this->close_)
		{
			return  STRING_QUEUE_ERR::CLOSE;
		}

		if (&(*this->queue_.front()) == nullptr)
		{
			err = STRING_QUEUE_ERR::ERR;
		}
		else
		{
			ref_str = *this->queue_.front();
		}

		this->queue_.pop_front();
	}

	return err;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief �L���[����
* @note
*/
/*-----------------------------------------------------------------------------*/
void StringQueue::Close()
{
	std::unique_lock<std::mutex> lock(this->mtx_); // lock_guard���g���Ƃ��̃X�R�[�v�������b�N�����

	this->close_ = true;

	this->sem_.post();
}