#pragma once
#ifndef SHOGI_STRING_UTIL_H_
#define shoGI_STRING_UTIL_H_

#include <string>


#define REF
#define OUT

template<typename T>
class StringUtilBase
{
public:
	static bool StartsWith(const T& str, const T& value) {
		return str.compare(0, value.length(), value) == 0;
	}

	static T TrimStart(const T& str, const T& trimchar)
	{
		int i;

		for (i = 0; i < (int)str.length(); i++)
		{
			if (trimchar.find(str[i]) == T::npos)
			{
				break;
			}
		}

		return str.substr(i);
	}

	//�@�O����g���~���O
	static T Trim(const T& str, const T& trimchar)
	{
		int start;

		for (start = 0; start < (int)str.length(); start++)
		{
			if (trimchar.find(str[start]) == T::npos)
			{
				break;
			}
		}

		int end;

		for (end = str.length() - 1; end >= 0; end--)
		{
			if (trimchar.find(str[end]) == T::npos)
			{
				break;
			}
		}


		return str.substr(start, end - start + 1);
	}

	static int IndexOfAny(const T& str, const T& any)
	{
		{
			int i;

			for (i = 0; i < (int)str.length(); i++)
			{
				if (any.find(str[i]) != T::npos)
				{
					// ������
					return i;
				}
			}

			return -1;
		}
	}
};


class StringUtil
{
public:
	static bool StartsWith(const std::string& str, const std::string& value) {
		return StringUtilBase<std::string>::StartsWith(str, value);
	}

	static bool StartsWith(const std::wstring& str, const std::wstring& value) {
		return StringUtilBase<std::wstring>::StartsWith(str, value);
	}

	static std::wstring TrimStart(const std::wstring& str, const std::wstring& value) {
		return StringUtilBase<std::wstring>::TrimStart(str, value);
	}

	static std::string TrimStart(const std::string& str, const std::string& value) {
		return StringUtilBase<std::string>::TrimStart(str, value);
	}

	static std::wstring Trim(const std::wstring& str, const std::wstring& value) {
		return StringUtilBase<std::wstring>::Trim(str, value);
	}

	static std::string Trim(const std::string& str, const std::string& value) {
		return StringUtilBase<std::string>::Trim(str, value);
	}

	static int IndexOfAny(const std::string& str, const std::string& any) {
		return StringUtilBase<std::string>::IndexOfAny(str, any);
	}

	static int IndexOfAny(const std::wstring& str, const std::wstring& any) {
		return StringUtilBase<std::wstring>::IndexOfAny(str, any);
	}

	static std::string Format(const char* fmt, ...);
	static std::wstring Format(const wchar_t* fmt, ...);

	static std::wstring ConvertWStringFromString(const std::string& str);
	static std::string  ConvertStringFromWString(const std::wstring& wstr);


	static int ParseTime(const std::wstring& str);

	static std::string Load(std::string filename);

	static int WStringWidth(const std::wstring& str);

	/*-----------------------------------------------------------------------------*/
	/**
	* @brief ���l�̃p�[�X
	* @param str ������
	* @param [out] num  ���l
	* @note
	*/
	/*-----------------------------------------------------------------------------*/

	template<typename T1, typename T2>
	static bool ParseNum(const T1& str, OUT T2& num)
	{
		size_t index = 0;
		bool minus = false;
		bool ret = false;

		num = 0;

		if (str.length() >= 1)
		{
			if (str[0] == '-')
			{
				// �}�C�i�X
				minus = true;
				index++;
			}
			else if (str[0] == '+')
			{
				// �v���X
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

		return ret;
	}


	/*-----------------------------------------------------------------------------*/
	/**
	* @brief ���Ԃ̃p�[�X
	* @param str ������
	* @return ���Ԃ��~���Z�J���h�ɒ��������̂�Ԃ�
	* @note "01:15"�Ȃ� 75000
	*       "01:02:03"�Ȃ� 1*60*60+2*60+3��Ԃ�
	*/
	/*-----------------------------------------------------------------------------*/
	template<typename T1>
	static int ParseTimeMs(const T1& str)
	{
		int time = 0;
		int sec = 0;
		int ms = 0;
		int keta = 0;

		for (auto ch : str)
		{
			if (ch >= '0' && ch <= '9')
			{
				if (keta != 0)
				{
					ms += (ch - '0') * keta;
					keta /= 10;
					if (keta == 0)
					{
						break;
					}
				}
				else
				{
					sec = sec * 10;
					sec += ch - '0';
				}
			}
			else if (ch == ':')
			{
				if (keta == 0)
				{
					time = time * 60 + sec * 60;
					sec = 0;
				}
				else
				{
					break;
				}
			}
			else if (ch == '.')
			{
				if (keta != 0)
				{
					break;
				}

				keta = 100;
			}
			else
			{
				// ���̕����͖���
			}
		}

		return (time + sec) * 1000 + ms;
	}

};


#endif

