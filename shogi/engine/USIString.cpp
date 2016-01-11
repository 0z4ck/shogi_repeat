#include <vector>

#include "USIString.h"




/*-----------------------------------------------------------------------------*/
/**
* @brief ���l�̃p�[�X
* @param str ������
* @param [out] num  ���l
* @note
*/
/*-----------------------------------------------------------------------------*/
bool USIString::ParseNum(const std::string& str, int* out_num)
{
	size_t index = 0;
	bool minus = false;
	bool ret = false;
	int num = 0;

	*out_num = 0;

	if (str.length() >= 1)
	{
		if (str[0] == '-')
		{
			// �}�C�i�X
			minus = true;
			index++;
		}
	}

	for (; index < str.length(); index++)
	{
		wchar_t c = str[index];

		if (c >= '0' && c <= '9')
		{
			num = num * 10;
			num = num + (c - '0');
		}
		else if (c == 'K' || c == 'k')
		{
			num = num * 1000;
			break;
		}
		else if (c == 'M' || c == 'm')
		{
			num = num * 1000 * 1000;
			break;
		}
		else
		{
			break;
		}

		// �Œ�1���ȏ゠��Ɛ���
		ret = true;
	}

	if (minus)
	{
		num = -num;
	}

	*out_num = num;

	return ret;
}

/*-----------------------------------------------------------------------------*/
/**
* token����
*/
/*-----------------------------------------------------------------------------*/
std::string USITokenizer::GetToken()
{
	size_t pos = this->pos_;
	
	while (pos < this->input_string_.size() && this->input_string_[pos] == ' ')
	{
		pos++;
	}

	// ���̋�؂蕶���܂ŃJ�E���g
	int start = pos;
	while (pos < this->input_string_.size() && this->input_string_[pos] != ' ')
	{
		pos++;
	}

	this->pos_ = pos + 1;

	unsigned len = pos - start;
	if (len <= 0)
	{
		return "";
	}

	return this->input_string_.substr(start, len);
}

/*-----------------------------------------------------------------------------*/
/**
* token����
*/
/*-----------------------------------------------------------------------------*/
std::string USITokenizer::GetToken(const std::string& next)
{
	size_t pos = this->pos_;

	// �󔒃X�L�b�v
	// while (pos < this->input_string_.size() && this->input_string_[pos] == ' ')
	// {
	// 	pos++;
	// }

	// ���̋�؂蕶���܂ŃJ�E���g

	size_t start = pos;

	pos = this->input_string_.find(" " + next + " ", pos);
	if (pos == std::string::npos)
	{
		// �Ȃ��ꍇ
		pos = this->input_string_.size();
	}

	this->pos_ = pos + 1;

	unsigned len = pos - start;
	if (len <= 0)
	{
		return "";
	}

	return this->input_string_.substr(start, len);
}

/*-----------------------------------------------------------------------------*/
/**
* token����
*/
/*-----------------------------------------------------------------------------*/
std::string USITokenizer::GetTokenName(const std::string& next)
{
	size_t pos = this->pos_;

	// �󔒃X�L�b�v
	while (pos < this->input_string_.size() && this->input_string_[pos] == ' ')
	{
	 	pos++;
	}

	size_t start = pos;

	pos = this->input_string_.find(" " + next + " ", pos);
	if (pos == std::string::npos)
	{
		// �Ȃ��ꍇ
		pos = this->input_string_.size();
	}

	this->pos_ = pos + 1;

	unsigned len = pos - start;
	if (len <= 0)
	{
		return "";
	}

	return this->input_string_.substr(start, len);
}

/*-----------------------------------------------------------------------------*/
/**
* �Ō�܂�
*/
/*-----------------------------------------------------------------------------*/
std::string USITokenizer::GetTokenLast()
{
	size_t pos = this->pos_;

	while (pos < this->input_string_.size() && this->input_string_[pos] == ' ')
	{
		pos++;
	}

	this->pos_ = this->input_string_.size();

	// ���̋�؂蕶���܂ŃJ�E���g
	unsigned len = this->input_string_.size() - pos;
	if (len <= 0)
	{
		return "";
	}


	return this->input_string_.substr(pos, len);
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief �߂�
 */
/*-----------------------------------------------------------------------------*/
void USITokenizer::Back(const std::string& token)
{
	this->pos_ -= token.size(); // �T�C�Y���߂�����
}