
#include "Handicap.h"


const std::string HandicapExtention::HandicapString[HANDICAP_NB] =
{
	{ "����"}, 
	{ "������" }, 
	{ "�E������" }, 
	{ "�p����" },   
	{ "��ԗ���" }, 
	{ "�򍁗���" }, 
	{ "�񖇗���" }, 
	{ "�O������" }, 
	{ "�l������" }, 
	{ "�ܖ�����" }, 
	{ "���ܖ�����" },
	{ "�Z������" }, 
	{ "��������" }, 
	{ "�\������" }, 
	{ "���̑�" },
};

/*-----------------------------------------------------------------------------*/
/**
 * @brief �n���f�B�L���b�v��������擾
 * @param handicap �n���f�B�L���b�v
 * @note  
 */
/*-----------------------------------------------------------------------------*/
const std::string& HandicapExtention::ToString(Handicap handicap)
{
	return HandicapString[handicap];
}

/*-----------------------------------------------------------------------------*/
/**
* @brief �荇�������񂩂�n���f�B�L���b�v���擾
* @param str ������
* @note
*/
/*-----------------------------------------------------------------------------*/
Handicap HandicapExtention::FromString(const std::string& str)
{
	for (int i = 0; i < HANDICAP_NB; i++)
	{
		if (HandicapString[i] == str)
		{
			return Handicap(i);
		}
	}

	// �Ȃ��ꍇ
	return Handicap::OTHER;
}


