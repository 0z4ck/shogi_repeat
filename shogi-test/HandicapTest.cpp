#include <gtest/gtest.h>

#include "types.h"
#include "Handicap.h"


TEST(HandicapTest, TestIsBlack)
{
	ASSERT_EQ(HandicapExtention::IsBlack(Handicap::HIRATE), true);
	ASSERT_EQ(HandicapExtention::IsBlack(Handicap::OTHER), true);
	ASSERT_EQ(HandicapExtention::IsBlack(Handicap::H2), false); 

}

TEST(HandicapTest, TestToString)
{
	ASSERT_TRUE(HandicapExtention::ToString(Handicap::HIRATE) == "����");
	ASSERT_TRUE(HandicapExtention::ToString(Handicap::OTHER) == "���̑�");
	ASSERT_TRUE(HandicapExtention::ToString(Handicap::KYO) == "������");
	ASSERT_TRUE(HandicapExtention::ToString(Handicap::RIGHT_KYO) == "�E������");
}

TEST(HandicapTest, TestFromString)
{
	ASSERT_EQ(HandicapExtention::FromString("����"), Handicap::HIRATE);
	ASSERT_EQ(HandicapExtention::FromString("������"), Handicap::KYO);
	ASSERT_EQ(HandicapExtention::FromString("�E������"), Handicap::RIGHT_KYO);
	ASSERT_EQ(HandicapExtention::FromString("�p����"), Handicap::KAKU);
	ASSERT_EQ(HandicapExtention::FromString("��ԗ���"), Handicap::HISYA);
	ASSERT_EQ(HandicapExtention::FromString("�򍁗���"), Handicap::HIKYO);

	ASSERT_EQ(HandicapExtention::FromString("�񖇗���"), Handicap::H2);
	ASSERT_EQ(HandicapExtention::FromString("�O������"), Handicap::H3);
	ASSERT_EQ(HandicapExtention::FromString("�l������"), Handicap::H4);
	ASSERT_EQ(HandicapExtention::FromString("�ܖ�����"), Handicap::H5);

	ASSERT_EQ(HandicapExtention::FromString("���ܖ�����"), Handicap::LEFT5);
	ASSERT_EQ(HandicapExtention::FromString("�Z������"), Handicap::H6);
	ASSERT_EQ(HandicapExtention::FromString("��������"), Handicap::H8);
	ASSERT_EQ(HandicapExtention::FromString("�\������"), Handicap::H10);

	ASSERT_EQ(HandicapExtention::FromString("���̑�"), Handicap::OTHER);
}