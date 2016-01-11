#include "KeyValue.h"

#include <fstream>


#include "StringUtil.h"

/*-----------------------------------------------------------------------------*/
/**
 * @brief �ǂݍ���
 * @note ���{��͖���
 */
/*-----------------------------------------------------------------------------*/
bool KeyValue::Load(const std::string& filename)
{
	this->clear();

	std::ifstream file;

	file.open(filename);

	if (!file.is_open()) {
		return false;
	}

	std::string str;

	while (std::getline(file, str))
	{
		if (str[0] == '#')
		{
			// �R�����g
		}
		else
		{
			int pos = StringUtil::IndexOfAny(str, "=\t");
			if (pos == std::string::npos)
			{
				// �����ꍇ ����
			}
			else
			{
				// �������ꍇ
				std::string key = str.substr(0, pos);
				std::string value = str.substr(pos + 1, str.length() - (pos + 1));

				key = StringUtil::Trim(key, " "); // �O��̋󔒂̓g���~���O
				value = StringUtil::Trim(value, " ");

				(*this)[key] = value;
			}
		}
	}

	file.close();

	return true;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief �ۑ�
*/
/*-----------------------------------------------------------------------------*/
bool KeyValue::Save(const std::string& filename) const
{
	std::ofstream file;

	file.open(filename);

	if (!file.is_open()) {
		return false;
	}

	std::string str;

	for (auto& pair : *this)
	{
		file << pair.first << "=" << pair.second << std::endl;
	}

	file.close();

	return true;
}

