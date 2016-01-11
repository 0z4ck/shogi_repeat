#include "USIEngine.h"
#include "Path.h"

USIEngine::USIEngine()
{
}


USIEngine::~USIEngine()
{
	this->Terminate();
}


/*-----------------------------------------------------------------------------*/
/**
* @brief ������
* @param filename �t�B�A����
* @note
*/
/*-----------------------------------------------------------------------------*/
bool USIEngine::Initialize(const std::string& filename)
{
	if (Initialized) return false;

	this->process_ = std::make_unique<Process>();
	this->string_queue_ = std::make_unique<StringQueue>();

	
	this->process_->StartInfo.FileName = filename;
	// this->process_.StartInfo.CreateNoWindow = true;
	this->process_->StartInfo.RedirectStandardInput = true;
	this->process_->StartInfo.RedirectStandardOutput = true;
	this->process_->StartInfo.WorkingDirectory = Path::FolderPath(Path::FullPath(filename));

	bool ret = this->process_->Start();

	if (ret) {
		this->stdin_thread_ = std::unique_ptr<std::thread>(new std::thread(&USIEngine::stdout_read, this));
		this->Initialized = true;
	}
	else
	{
		this->string_queue_ = nullptr;
		this->process_ = nullptr;
	}

	return ret;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief �I��
* @param filename �t�B�A����
* @note
*/
/*-----------------------------------------------------------------------------*/
void USIEngine::Terminate()
{
	if (Initialized)
	{
		Initialized = false;

		this->string_queue_->Close();

		this->process_->WaitForExit(10000);
		this->process_->Close();

		// stdin�̏I���҂�
		this->stdin_thread_->join();
	}
}


/*-----------------------------------------------------------------------------*/
/**
* @brief Stdout����ǂ݂����X���b�h
* @note
*/
/*-----------------------------------------------------------------------------*/
void USIEngine::stdout_read()
{
	while (!this->process_->StdOut.IsEof())
	{
		std::string str;

		if (this->process_->StdOut.ReadLine(str))
		{
			this->string_queue_->Push(str);
		}
		else
		{
			// �ǂݏo���G���[�Œ��f
			this->string_queue_->Push(nullptr);
			break;
		}
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief �ǂݏo��
* @note
*/
/*-----------------------------------------------------------------------------*/
STRING_QUEUE_ERR USIEngine::ReadLine(std::string& str, int timeout)
{
	return this->string_queue_->Pop(str, timeout);
}

/*-----------------------------------------------------------------------------*/
/**
* @brief ��������
* @note
*/
/*-----------------------------------------------------------------------------*/
void USIEngine::WriteLine(const std::string& str)
{
	this->process_->StdIn.Write(str + "\n");
}


/*-----------------------------------------------------------------------------*/
/**
 * @brief �I�v�V�����ݒ�
 * @param name
 * @param value
 * @note  
 */
/*-----------------------------------------------------------------------------*/
void USIEngine::SetOption(const std::string& name, bool value)
{
	if (this->options_.Contents(name))
	{
		this->options_[name]->SetValue(value);
	}
}


void USIEngine::SetOption(const std::string& name, int value)
{
	if (this->options_.Contents(name))
	{
		this->options_[name]->SetValue(value);
	}
}

void USIEngine::SetOption(const std::string& name, const std::string& value)
{
	if (this->options_.Contents(name))
	{
		this->options_[name]->SetValue(value);
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief �I�v�V�����o�^
* @param str note�ɏ������I�v�V����������
* @note  option name <id> type <t> default <x> [min <y> max <z>] {var <v>}
* 
*/
/*-----------------------------------------------------------------------------*/
bool USIEngine::AddOption(const std::string& str)
{
	return this->options_.AddOption(str);
}

/*-----------------------------------------------------------------------------*/
/**
* @brief �I�v�V�������O,�l�ňꊇ�ݒ�
*/
/*-----------------------------------------------------------------------------*/
void USIEngine::SetOptions(const std::map<std::string, std::string>& opt_name_value)
{
	for (auto& opt : opt_name_value)
	{
		this->SetOption(opt.first, opt.second);
	}
}
