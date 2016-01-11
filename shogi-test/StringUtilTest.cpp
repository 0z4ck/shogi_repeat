#include <gtest/gtest.h>

#include "StringUtil.h"

TEST(StringUtilTest, testStartsWith)
{
	std::string str = "�����͓V�C���悭����ł���";
	
	ASSERT_EQ(StringUtil::StartsWith(str, "��"), true);

	ASSERT_EQ(StringUtil::StartsWith(str, "������"), true);
	ASSERT_EQ(StringUtil::StartsWith(str, "�����͓V�C���悭"), true);
	ASSERT_EQ(StringUtil::StartsWith(str, "�����͓V�C���悭����ł���"), true);

	ASSERT_EQ(StringUtil::StartsWith(str, "��"), false);
	ASSERT_EQ(StringUtil::StartsWith(str, "�V�C���悭"), false);
	ASSERT_EQ(StringUtil::StartsWith(str, "�����͓V�C���悭����ł�������������"), false);

	std::wstring wstr = L"�����͓V�C���悭����ł���";

	ASSERT_EQ(StringUtil::StartsWith(wstr, L"��"), true);

	ASSERT_EQ(StringUtil::StartsWith(wstr, L"������"), true);
	ASSERT_EQ(StringUtil::StartsWith(wstr, L"�����͓V�C���悭"), true);
	ASSERT_EQ(StringUtil::StartsWith(wstr, L"�����͓V�C���悭����ł���"), true);

	ASSERT_EQ(StringUtil::StartsWith(wstr, L"��"), false);
	ASSERT_EQ(StringUtil::StartsWith(wstr, L"�V�C���悭"), false);
	ASSERT_EQ(StringUtil::StartsWith(wstr, L"�����͓V�C���悭����ł�������������"), false);
}

TEST(StringUtilTest, testTrim)
{
	ASSERT_EQ(StringUtil::TrimStart(L"�e�X�g", L" �@"), L"�e�X�g");
	ASSERT_EQ(StringUtil::TrimStart(L" �e�X�g", L" �@"), L"�e�X�g");
	ASSERT_EQ(StringUtil::TrimStart(L"�@�@�e�X�g", L" �@"), L"�e�X�g");
	ASSERT_EQ(StringUtil::TrimStart(L" �@�e�X�g", L" �@"), L"�e�X�g");
	ASSERT_EQ(StringUtil::TrimStart(L"�e�X�g ", L" �@"), L"�e�X�g ");
	ASSERT_EQ(StringUtil::TrimStart(L"�e�X�g �@", L" �@"), L"�e�X�g �@");

	ASSERT_EQ(StringUtil::Trim(L"�e�X�g", L" �@"), L"�e�X�g");
	ASSERT_EQ(StringUtil::Trim(L" �e�X�g", L" �@"), L"�e�X�g");
	ASSERT_EQ(StringUtil::Trim(L"�@�@�e�X�g", L" �@"), L"�e�X�g");
	ASSERT_EQ(StringUtil::Trim(L" �@�e�X�g", L" �@"), L"�e�X�g");
	ASSERT_EQ(StringUtil::Trim(L"�e�X�g ", L" �@"), L"�e�X�g");
	ASSERT_EQ(StringUtil::Trim(L"�e�X�g �@", L" �@"), L"�e�X�g");

	ASSERT_EQ(StringUtil::Trim(L"�@ �e�X�g �@", L" �@"), L"�e�X�g");
}

TEST(StringUtilTest, testIndexOfAny)
{
	ASSERT_EQ(StringUtil::IndexOfAny(L"�e�X�g:�e�X�g", L":�F"), 3);
	ASSERT_EQ(StringUtil::IndexOfAny(L"�e�X�g�F�e�X�g:", L":�F"), 3);
	ASSERT_EQ(StringUtil::IndexOfAny(L"�e�X�g:�e�X�g�F", L":�F"), 3);
}

TEST(StringUtilTest, testFormat)
{
	ASSERT_EQ(StringUtil::Format("%d", 1), "1");
	ASSERT_EQ(StringUtil::Format("%3d", 3), "  3");
	ASSERT_EQ(StringUtil::Format("%02d", 2), "02");
	ASSERT_EQ(StringUtil::Format("%s", "test"), "test");
	ASSERT_EQ(StringUtil::Format("%6s", "test"), "  test");
	ASSERT_EQ(StringUtil::Format("%-6s", "test"), "test  ");
}

TEST(StringUtilTest, testConvert)
{
	ASSERT_EQ(StringUtil::ConvertWStringFromString("�e�X�g"), L"�e�X�g");
	ASSERT_EQ(StringUtil::ConvertWStringFromString("�\��"), L"�\��");

	ASSERT_EQ(StringUtil::ConvertStringFromWString(L"�e�X�g"), "�e�X�g");
	ASSERT_EQ(StringUtil::ConvertStringFromWString(L"�\��"), "�\��");
}

TEST(StringUtilTest, testParseTime)
{
	ASSERT_EQ(StringUtil::ParseTime(L"1:05"), 65);
	ASSERT_EQ(StringUtil::ParseTime(L"1 : 5"), 65);

	ASSERT_EQ(StringUtil::ParseTime(L" 21:05 "), 21*60+5);
	ASSERT_EQ(StringUtil::ParseTime(L"12:34:56"), 12*3600+34*60+56);

}
