

�����A���΋ǃ\�t�g

�R�}���h���C����USI�v���g�R���Ή��̏����G���W���̘A���΋ǂ��ł��܂��B

���g����

shogi <���G���W��>  <���G���W��> [�I�v�V����]
�I�v�V����:
  -B <�ݒ�t�@�C��>   ���ݒ�t�B�A��
  -W <�ݒ�t�@�C��>   ���ݒ�t�B�A��
  -T <��������(�b)> [�b�ǂ�(�b)]  �������Ԑݒ�
  -G <num>            �΋ǐ�
  -M <num>            �ő�萔
  -P                  Ponder��ON�ɂ���
  -P-                 Ponder��OFF�ɂ���
  -S                  ������ւ���ON�ɂ���
  -S-                 ������ւ���OFF�ɂ���
  -L [���O�ۑ��p�X]   ���O�o�͂�ON�A���O�ۑ��p�X��ݒ肷��
  -L-                 ���O�o�͂�OFF�ɂ���
  -N [�����ۑ��p�X]   �����o��ON�A������ۑ�����p�X��ݒ肷��
  -N-                 �����o��OFF�ɂ���
  -K <����> [�萔]    �J�n�ǖʂ��w�肵�������t�@�C���̎萔����ɂ���

�E�� �Ђ悱�������m���A����0.2�b �΋ǐ�10����s����B
shogi.exe hiyoko.exe hiyoko.exe -T 0 0.2 -G 10



������

�E�\�[�X�R�[�h�̕����R�[�h��UTF-8 BOM�t��

�E���s���̕����R�[�h��SJIS
 => VC(mingw���H�j��SJIS Linux��UTF-8

�Evc�̓}���`�o�C�g�����ɐݒ�
 gcc��--exec-charset=cp932 ������
 => gcc�ł�cp932�w��͂�߂�

�E�����ŕۑ����镶�����wstring��Unicode16�ɂ���H(USC2?)
 => VC�� UCS-2, Linux��UCS-4

�E�e�X�g�ɂ�gtest���g���B
�e�X�g����Ƃ��͍\����Test�ɂ��ăr���h����B
�\����Test�ɂ���ƃv���W�F�N�gshogi�������C�u�����Ashogi-test����exe�ɂȂ�B�i���i�͋t�j


�E�q�v���Z�X�e�X�g�p��tchid��Debug�r���h�����Test�ɃR�s�[�����B
Test�ŕ��ʂɃr���h����Test��exe��z�u����悤�ɂ���ƁA
gtest�̃A�h�C��������ɓ��삵�Ȃ��̂��A
�e�X�g�G�N�X�v���[���[�Ƀe�X�g���o�čs���Ȃ��B
���Ȃ����ł�tchild���N������Ă��ē��삪�ւ�H


�E�f�o�b�O���̃I�v�V����
�������ԂȂ��A�b��0.2�b
�΋ǐ�2
shogi.exe hiyoko.exe hiyoko.exe -T 0 0.2 -G 2

shogi.exe gpsfish/gpsfish.exe hiyoko.exe -T 0 0.2 -G 1 -B black.ini


