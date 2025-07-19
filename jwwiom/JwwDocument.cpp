//****************************************
// JwwDocument.cpp : �����t�@�C��
//****************************************

#include "stdafx.h"
#include "JwwDocument.h"

#define SXCOL_EXT 100;
#define SXLTP_EXT 30;

bool OdEqual(double x, double y, double z)
{
	return std::abs(x - y) < z;
}

// �p���T�C�Y���̉���(mm)
const double CJwwDocument::YOUSHI_SIZE[15] =
{
	1189.0,    // A0
	841.0,     // A1
	594.0,     // A2
	420.0,     // A3
	297.0,     // A4
	0.0,       // �g�p���Ȃ�
	0.0,       // �g�p���Ȃ�
	0.0,       // �g�p���Ȃ�
	1682.0,    // 2A   �� JWC�d�l���ɂ͏����Ă��Ȃ�
	2378.0,    // 3A   �� JWC�d�l���ɂ͏����Ă��Ȃ�
	3364.0,    // 4A   �� JWC�d�l���ɂ͏����Ă��Ȃ�
	4756.0,    // 5A   �� JWC�d�l���ɂ͏����Ă��Ȃ�
	10000.0,   // 10m
	50000.0,   // 50m  �� JWC�d�l���ɂ͏����Ă��Ȃ�
	100000.0	// 100m �� JWC�d�l���ɂ͏����Ă��Ȃ�
};

// �p���T�C�Y���̏c��(mm)
//
// 2013-11-04 JWW�̎d�l���m�F������A10m,50m,100m�̗p���T�C�Y���Ⴄ���Ƃ�
// �C���t�������AJWIMPORT�̎d�l�Ƃ��Ă͏]���ʂ肢�����Ƃɂ����B
// A4-5A �͗p��������(0,0)�ɂ���悤�ɐ}�`���ړ�����B
// 10m,50m,100m�͏c����10m,50m,100m�Ƃ��A(0,0)�͒��S�ɂ���B
// JW�̐}�`��(0,0)�𒆐S�Ƀ}�C�i�X�̕����ɂ����邱�Ƃɒ���
//
const double CJwwDocument::YOUSHI_TATE[15] =
{
	841.0,     // A0
	594.0,     // A1
	420.0,     // A2
	297.0,     // A3
	210.0,     // A4
	0.0,       // �g�p���Ȃ�
	0.0,       // �g�p���Ȃ�
	0.0,       // �g�p���Ȃ�
	1189.0,    // 2A
	1682.0,    // 3A
	2378.0,    // 4A
	3364.0,    // 5A
	10000.0,   // 10m (JWW�� 7073.0 )
	50000.0,   // 50m (JWW�� 35366.0 )
	100000.0	// 100m (JWW�� 70732.0 )
};

// �W���I�Ȑ}�ʂ̏k��
const double CJwwDocument::HYOJUN_SHAKUDO[8] =
{
	1.0, // 1/1
	5.0, // 1/5
	10.0, // 1/10
	50.0, // 1/50
	100.0, // 1/100
	500.0, // 1/500
	1000.0, // 1/1000
	5000.0  // 1/5000
};

//****************************************
// CJwwHeader : JWW�t�@�C���w�b�_
//****************************************

CJwwHeader::CJwwHeader()
{
	// -1 �́A�X�P�[�����ݒ肳��Ă��Ȃ��t���O
	m_dJwwScale = -1;
	// �I�t�Z�b�g�l��(0,0)�ɏ�����
	m_pJwwOffset.x = 0.0;
	m_pJwwOffset.y = 0.0;
	// �����̊p�x�␳�͂��Ȃ��ɏ�����
	m_bFixAng = FALSE;
	m_tFixAng = 1e-10;
}

CJwwHeader::~CJwwHeader()
{
}

IMPLEMENT_SERIAL(CJwwHeader, CObject, VERSIONABLE_SCHEMA);

void CJwwHeader::Serialize(CArchive& ar)
{
	int i, j, n, k;
	DWORD dummy;

	// �����o��
	if (ar.IsStoring())
	{
		// JWW�̃f�[�^�t�@�C���錾
		ar.Write(m_cVersion, 8);

		// �o�[�W����No.
		ar << (DWORD)m_Version;

		// �t�@�C������
		ar << CStringA(m_strMemo);

		// �}�ʃT�C�Y
		// 0�`4 �FA0�`A4
		// 8    �F2 A
		// 9    �F3 A
		// 10   �F4 A
		// 11   �F5 A
		// 12   �F10m
		// 13   �F50m
		// 14   �F100m
		ar << (DWORD)m_nZumen;

		// ��w�O���[�v�E��w���
		// ������w�O���[�v
		// ��w�O���[�v���(0�F��\���A1�F�\���̂݁A2�F�ҏW�\�A3�F����)
		// ��w�O���[�v�̏�����w
		// ��w�O���[�v�̏k��(�̕���)
		// ��w�O���[�v�̃v���e�N�g�w��
		// �@(0�F���w��A1�F�\����ԕύX�\�v���e�N�g�w��A2�F�\����ԌŒ�v���e�N�g�w��)
		// ��w���(0�F��\���A1�F�\���̂݁A2�F�ҏW�\�A3�F����)
		// ��w�̃v���e�N�g�w��
		// �@(0�F���w��A1�F�\����ԕύX�\�v���e�N�g�w��A2�F�\����ԌŒ�v���e�N�g�w��)
		ar << (DWORD)m_nWriteGLay;

		for (int nGLay = 0; nGLay < 16; nGLay++)
		{
			ar << (DWORD)m_anGLay[nGLay];
			ar << (DWORD)m_anWriteLay[nGLay];
			ar << (double)m_adScale[nGLay];
			ar << (DWORD)m_anGLayProtect[nGLay];

			for (int nLay = 0; nLay < 16; nLay++)
			{
				ar << (DWORD)m_aanLay[nGLay][nLay];
				ar << (DWORD)m_aanLayProtect[nGLay][nLay];
			}
		}

		// �_�~�[
		for (i = 0, dummy = 0; i < 14; i++)
		{
			ar << (DWORD)dummy;
		}

		// ���@�֌W�̐ݒ�
		// ���ݒ�t�@�C���́uS_COMM_8�v�̇B���@�ݒ肪�u10�v�܂��́u11�v�ɂȂ��Ă��āA
		// �t�@�C���ւ̕ۑ����ł���ꍇ�ɗL���ɂȂ�A
		// ����ȊO�́um_lnSunpou1�v�`�um_lnSunpou5�v�́u0�v�ɂȂ�B

		// ��ʁF���@���F�i1�`9�j
		// �\�ʁF���o���F�i1�`9�j
		// �S�ʁF���@�_�F�i1�`9�j
		// ��ʁF���@�l�̏����_�ȉ��̌����i0�`3�j
		// ���ʁF���@�P�ʁi0:mm �A1:���j
		// �\���ʁF���@���[���i0:�_ �A1:��� �A2:�t���j
		// �S���ʁF���@������i1�`10�j
		ar << (DWORD)m_lnSunpou1;
		// ��ʁ`��ʁF���@�l�̐��@���Ƃ̗���
		// �@�@�@�@�@�@����i0�`99.9�j�~�P�O�A�}�C�i�X�̏ꍇ��1000���v���X���Đ���������
		// ���ʁ`�疜�ʁF���@���̓ˏo����
		// �@�@�@�@�@�@�����i0�`99.9�j�~�P�O�A�}�C�i�X�̏ꍇ��1000���v���X���Đ��������A�X��10000�{����B
		ar << (DWORD)m_lnSunpou2;
		// ��ʁ`�S�ʁF��󒷂�
		// �@�@�@�@�@�@�����i0�`99.9�j�~�P�O�Ƃ��Đ���������
		// ��ʁ`�\���ʁF���p�x
		// �@�@�@�@�@�@�p�x�i0.1�`80.0�j�~�P�O�Ƃ��Đ��������A1000�{����
		// �S���ʁ`���ʁF�t���̏o���@
		// �@�@�@�@�@�@���@�i0�`99.9�j�~�P�O�Ƃ��Đ����������A1000000�{����
		ar << (DWORD)m_lnSunpou3;
		// ��ʁF���@�l������␳���Ȃ��i0�F�␳�L �A1�F�␳���j
		// �\�ʁF���@�l��S�p�ɂ���i0�F���p �A1�F�S�p�j
		// �S�ʁF���@�l�̃J���}���X�y�[�X�ɂ���i0�F�J���} �A1�F�X�y�[�X�j
		// ��ʁF���@�l�̃J���}��S�p�ɂ���i0�F���p �A1�F�S�p�j
		// ���ʁF���@�l�̏����_��S�p�ɂ���i0�F���p �A1�F�S�p�j
		// �\���ʁF���@�l�ɒP�ʂ�\������i0�F�\���� �A1�F�\���L�j
		// �S���ʁF���a�l�ɁuR�v��\������i0�F�\���� �A1�F�O�t�\���L �A2�F��t�\���L�j
		// �疜�ʁF���a�l�ɃJ���}��\������i0�F�\���� �A1�F�\���L�j
		// ���ʁF���a�l�ɏ����_�́u0�v��\������i0�F�\���� �A1�F�\���L�j
		ar << (DWORD)m_lnSunpou4;
		// ��ʁ@�F���@�l���΂ߕ����ɂ���i0�F�ʏ핶�� �A1�F�΂ߕ����j
		// �\�ʁ@�F���@�l�𑾕����ɂ���i0�F�ʏ핶�� �A1�F�������j
		// �S�ʁ@�F�p�x�P�ʁi0�F�x�P�� �A1�F�x���b�P�� �A2�F�x�P�ʂŁu߁v�̕\���Ȃ��j
		// ��ʁ@�F�p�x�P�ʂ̏����_�ȉ��̌����i0�`6�j
		// ���ʁ@�F���@�}�`�ɂ���i0�F���@�}�`�ɂ��Ȃ� �A1�F���@�}�`�ɂ���j
		// �\���ʁF�͈͑I���̂Ƃ����@�}�`����F�E��̑����őI���i0�F�I�����Ȃ� �A1�F�I������j
		// �S���ʁF�\�������_�ȉ��̏����i0:�l�̌ܓ��@1:�؎́@2:�؏�j(Ver.4.02�ȍ~�j
		ar << (DWORD)m_lnSunpou5;

		// �_�~�[
		ar << (DWORD)dummy;

		// ���`��̍ő啝
		// 1����100 �����̓s�N�Z���P�ʂŎw��
		// -1����-100 ������ abs(1/N)�~���P�ʂŎw��(-16->1/16�~��)
		//�u������1/100mm�P�ʂƂ���v���ݒ肳��Ă���Ƃ���-300���w��(-100��菬����)
		ar << (DWORD)m_nMaxDrawWid;

		// �v�����^�o�͔͈͂̌��_(X,Y)
		ar << (double)m_DPPrtGenten.x;
		ar << (double)m_DPPrtGenten.y;

		// �v�����^�o�͔{��
		ar << (double)m_dPrtBairitsu;

		// �v�����^90����]�o�́A�v�����^�o�͊�_�ʒu
		// ��ʁF�v�����^90����]�o��
		// �\�ʁF�v�����^�o�͊�_�ʒu�̎w��i0:���w��j�iVer.3.00�ȍ~�j
		//  �@  7:����   8:����  9:�E��
		//   �@ 4:����   5:����  6:�E��
		//    �@1:����   2:����  3:�E��
		ar << (DWORD)m_nPrtSet;

		// �ڐ��ݒ胂�[�h
		// ���(0�F���w��A1�F�ڐ��\��)
		// �\��(0�F�}�ʐ��@�w��A1�F�����w��)
		// �|�l�̎��A�ڐ��ǎ��s��
		ar << (DWORD)m_nMemoriMode;

		// �ڐ��\���ŏ��Ԋu�h�b�g
		ar << (double)m_dMemoriHyoujiMin;

		// �ڐ��\���Ԋu(X,Y)
		ar << (double)m_dMemoriX;
		ar << (double)m_dMemoriY;

		// �ڐ���_(X,Y)
		ar << (double)m_DpMemoriKijunTen.x;
		ar << (double)m_DpMemoriKijunTen.y;

		// ��w��
		for (n = 0; n <= 15; n++)
		{
			for (k = 0; k <= 15; k++)
			{
				ar << CStringA(m_aStrLayName[n][k]);
			}
		}

		// ��w�O���[�v��
		for (n = 0; n <= 15; n++)
		{
			ar << CStringA(m_aStrGLayName[n]);
		}

		// ���e�v�Z�̏���
		// ����ʍ���
		// �ܓx
		// 9�`15�̑���̎w��
		// �ǖʓ��e����ʍ���
		ar << (double)m_dKageLevel;
		ar << (double)m_dKageIdo;
		ar << (DWORD)m_nKage9_15JiFlg;
		ar << (double)m_dKabeKageLevel;

		// �V��}�̏����iVer.3.00�ȍ~)
		// ����ʍ���
		// �V��}�̔��a���Q
		if (m_Version >= 300)
		{
			ar << (double)m_dTenkuuZuLevel;
			ar << (double)m_dTenkuuZuEnkoR;
		}
		// 2.5D�̌v�Z�P��(0�ȊO��mm�P�ʂŌv�Z)
		ar << (DWORD)m_nMMTani3D;

		// �ۑ����̉�ʔ{��(�Ǎ��ނƑO��ʔ{���ɂȂ�)
		ar << (double)m_dBairitsu;
		ar << (double)m_DPGenten.x;
		ar << (double)m_DPGenten.y;

		// �͈͋L���{���Ɗ�_(X,Y)
		ar << (double)m_dHanniBairitsu;
		ar << (double)m_DPHanniGenten.x;
		ar << (double)m_DPHanniGenten.y;

		// �}�[�N�W�����v�{���A��_(X,Y)����щ�w�O���[�v
		if (m_Version >= 300)
		{
			for (n = 1; n <= 8; n++)
			{
				ar << (double)m_dZoomJumpBairitsu[n];
				ar << (double)m_DPZoomJumpGenten[n].x;
				ar << (double)m_DPZoomJumpGenten[n].y;
				ar << (DWORD)m_nZoomJumpGLay[n];
			}
		}
		else
		{
			for (n = 1; n <= 4; n++)
			{
				ar << (double)m_dZoomJumpBairitsu[n];
				ar << (double)m_DPZoomJumpGenten[n].x;
				ar << (double)m_DPZoomJumpGenten[n].y;
			}
		}

		// �����̕`����(Ver.4.05�ȍ~�j
		if (m_Version >= 300)
		{
			ar << (double)m_dDm11;		//�_�~�[
			ar << (double)m_dDm12;		//�_�~�[
			ar << (double)m_dDm13;		//�_�~�[
			ar << (DWORD)m_lnDm1;			//�_�~�[
			ar << (double)m_dDm21;		//�_�~�[
			ar << (double)m_dDm22;		//�_�~�[
			ar << (double)m_dMojiBG;	//(Ver.4.04�ȑO�̓_�~�[�j
			//������͈͂�w�i�F�ŕ`�悷��Ƃ��͈̔͑����@
			ar << (DWORD)m_nMojiBG;		//(Ver.4.04�ȑO�̓_�~�[�j
			//�\��:�����i���@�}�`��u���b�N�}�`�j���Ō�ɕ`��
			//���:1 :�֊s�E�͈͂�w�i�F�ŕ`�悵�Ȃ�
			//	2 :�����̗֊s��w�i�F�ŕ`�悷��
			//	3 :������͈͂�w�i�F�ŕ`�悷��
		}

		// �����Ԋu
		for (n = 0; n <= 9; n++)
		{
			ar << (double)m_adFukusenSuuchi[n];
		}

		// ���������̗����o�̐��@
		ar << (double)m_dRyoygawaFukusenTomeDe;

		// �F�ԍ����Ƃ̉�ʕ\���F�A����
		// ��ʕ\���F(0�F�w�i�F�A1�`8�F���F�A9�F�O���[�F)
		// ����(1�`16)
		for (n = 0; n <= 9; n++)
		{
			ar << (DWORD)m_aPenColor[n];
			ar << (DWORD)m_anPenWidth[n];
		}

		// �F�ԍ����Ƃ̃v�����^�o�͐F�A�����A���_���a
		// �v�����^�o�͐F(0�F�w�i�F�A1�`8�F���F�A9�F�O���[�F)
		// ����(1�`500)
		// ���_���a(0.1�`10)
		for (n = 0; n <= 9; n++)
		{
			ar << (DWORD)m_aPrtPenColor[n];
			ar << (DWORD)m_anPrtPenWidth[n];
			ar << (double)m_adPrtTenHankei[n];
		}

		// ����ԍ�2����9�܂ł̃p�^�[���A1���j�b�g�̃h�b�g���A�s�b�`�A�v�����^�o�̓s�b�`
		for (n = 2; n <= 9; n++)
		{
			ar << (DWORD)m_alLType[n];
			ar << (DWORD)m_anTokushuSenUintDot[n];
			ar << (DWORD)m_anTokushuSenPich[n];
			ar << (DWORD)m_anPrtTokushuSenPich[n];
		}

		// �����_����1����5�܂ł̃p�^�[���A��ʕ\���U���E�s�b�`�A�v�����^�o�͐U���E�s�b�`
		for (n = 11; n <= 15; n++)
		{
			ar << (DWORD)m_alLType[n];
			ar << (DWORD)m_anRandSenWide[n];
			ar << (DWORD)m_anTokushuSenPich[n];
			ar << (DWORD)m_anPrtRandSenWide[n];
			ar << (DWORD)m_anPrtTokushuSenPich[n];
		}

		// �{������ԍ�6����9�܂ł̃p�^�[���A1���j�b�g�̃h�b�g���A�s�b�`�A�v�����^�o�̓s�b�`
		for (n = 16; n <= 19; n++)
		{
			ar << (DWORD)m_alLType[n];
			ar << (DWORD)m_anTokushuSenUintDot[n];
			ar << (DWORD)m_anTokushuSenPich[n];
			ar << (DWORD)m_anPrtTokushuSenPich[n];
		}

		// ���_����ʕ`�掞�̎w�蔼�a�ŕ`��
		ar << (DWORD)m_nDrawGamenTen;

		// ���_���v�����^�o�͎��A�w�蔼�a�ŏ���
		ar << (DWORD)m_nDrawPrtTen;

		// BitMap�E�\���b�h���ŏ��ɕ`�悷��
		ar << (DWORD)m_nBitMapFirstDraw;
		// �\�ʁF�\���b�h�`�揇
		//  0:��w��        (+1):�F��
		//  3:��w�t��      (+2):�F�t��
		//  6:����o�͐ݒ菇
		// ��ʁF
		//  0:BitMap�E�\���b�h���ŏ��ɕ`�悵�Ȃ�
		//  1:BitMap�E�\���b�h���ŏ��ɕ`��
		//  2:�\���b�h���ŏ��ɕ`��
		// �t�`��
		ar << (DWORD)m_nGyakuDraw;

		// �t�T�[�`
		ar << (DWORD)m_nGyakuSearch;

		// �J���[���
		ar << (DWORD)m_nColorPrint;

		// ��w���̈��
		ar << (DWORD)m_nLayJunPrint;

		// �F�ԍ����̈��
		ar << (DWORD)m_nColJunPrint;

		// ��w�O���[�v�܂��͉�w���Ƃ̃v�����^�A���o�͎w��
		ar << (DWORD)m_nPrtRenzoku;

		// �v�����^���ʉ�w(�\���̂݉�w)�̃O���[�o�͎w��
		ar << (DWORD)m_nPrtKyoutsuuGray;

		// �v�����^�o�͎��ɕ\���̂݉�w�͏o�͂��Ȃ�
		ar << (DWORD)m_nPrtDispOnlyNonDraw;

		if (m_Version >= 223)
		{
			// ��}���ԁiVer.2.23�ȍ~�j
			ar << (DWORD)m_lnDrawTime;

			// 2.5D�̎n�_�ʒu���ݒ肳��Ă��鎞�̃t���O�iVer.2.23�ȍ~�j
			// ��ʁF�����}�̎��_�ʒu�ݒ�ς݃t���O
			// �\�ʁF���Ր}�̎��_�ʒu�ݒ�ς݃t���O
			// �S�ʁF�A�C�\���}�̎��_�ʒu�ݒ�ς݃t���O
			ar << (DWORD)m_nEyeInit;

			// 2.5D�̓����}�E���Ր}�E�A�C�\���}�̎��_�����p�iVer.2.23�ȍ~�j
			ar << (DWORD)m_dEye_H_Ichi_1; //*100�K�v?
			ar << (DWORD)m_dEye_H_Ichi_2;
			ar << (DWORD)m_dEye_H_Ichi_3;

			// 2.5D�̓����}�̎��_�����E���_����iVer.2.23�ȍ~�j
			ar << (double)m_dEye_Z_Ichi_1;
			ar << (double)m_dEye_Y_Ichi_1;

			// 2.5D�̒��Ր}�̎��_�����E���_����iVer.2.23�ȍ~�j
			ar << (double)m_dEye_Z_Ichi_2;
			ar << (double)m_dEye_Y_Ichi_2;

			// 2.5D�̃A�C�\���}�̎��_�����p�iVer.2.23�ȍ~�j
			ar << (double)m_dEye_V_Ichi_3;
		}

		// ���̒����w��̍ŏI�l�iVer.2.25�ȍ~�j
		if (m_Version >= 225)
		{
			ar << (double)m_dSenNagasaSnpou;
		}

		// ��`���@�����@�E�c���@�w��̍ŏI�l�iVer.2.25�ȍ~�j
		if (m_Version >= 225)
		{
			ar << (double)m_dBoxSunpouX;
			ar << (double)m_dBoxSunpouY;
		}

		// �~�̔��a�w��̍ŏI�l�iVer.2.25�ȍ~�j
		if (m_Version >= 225)
		{
			ar << (double)m_dEnHankeiSnpou;
		}

		// �\���b�h��C�ӐF�ŏ����t���O�A�\���b�h�̔C�ӐF�̊���l�iVer.2.30�ȍ~�j
		if (m_Version >= 230)
		{
			ar << (DWORD)m_nSolidNinniColor;
			ar << (DWORD)m_SolidColor;	// RGB
		}

		// SXF�Ή��g�����F��`�iVer.4.20�ȍ~�j
		// ��ʕ\���F
		// ��ʕ\������
		// ���F��
		// �v�����^�o�͐F
		// �v�����^�o�͐���
		// �_���a
		if (m_Version >= 420)
		{
			int n1;

			for (n = 0; n <= 256; n++)	////// ��ʕ\���F
			{
				n1 = n + SXCOL_EXT;		//�F�ԍ��̃I�t�Z�b�g = +100
				ar << (DWORD)m_aPenColor[n1];
				ar << (DWORD)m_anPenWidth[n1];
			}
			for (n = 0; n <= 256; n++)	////// �v�����^�o�͐F
			{
				ar << CStringA(m_astrUDColorName[n]);
				n1 = n + SXCOL_EXT;	//�F�ԍ��̃I�t�Z�b�g = +100
				ar << (DWORD)m_aPrtPenColor[n1];
				ar << (DWORD)m_anPrtPenWidth[n1];
				ar << (double)m_adPrtTenHankei[n1];
			}

			// SXF�Ή��g�������`�iVer.4.20�ȍ~�j
			// �p�^�[��
			// 1���j�b�g�̃h�b�g��
			// �s�b�`
			// �v�����^�o�̓s�b�`
			// ���햼
			// �Z�O�����g��
			// �s�b�`�����̒����A�󔒒����̌J��Ԃ�
			for (n = 0; n <= 32; n++)	////// ����p�^�[��
			{
				n1 = n + SXLTP_EXT;   //����ԍ��̃I�t�Z�b�g = +30
				ar << (DWORD)m_alLType[n1];
				ar << (DWORD)m_anTokushuSenUintDot[n1];
				ar << (DWORD)m_anTokushuSenPich[n1];
				ar << (DWORD)m_anPrtTokushuSenPich[n1];
			}
			for (n = 0; n <= 32; n++)	////// ����p�����[�^
			{
				ar << CStringA(m_astrUDLTypeName[n]);
				ar << (DWORD)m_anUDLTypeSegment[n];
				for (j = 1; j <= 10; j++)
				{
					ar << (double)m_aadUDLTypePitch[n][j];
				}
			}
		}

		// ������1����10�܂ł̕������A�����A�Ԋu�A�F�ԍ�
		for (i = 1; i <= 10; i++)
		{
			ar << (double)m_adMojiX[i];
			ar << (double)m_adMojiY[i];
			ar << (double)m_adMojiD[i];
			ar << (DWORD)m_anMojiCol[i];
		}

		// �����ݕ����̕������A�����A�Ԋu�A�F�ԍ��A�����ԍ�
		ar << (double)m_dMojiSizeX;
		ar << (double)m_dMojiSizeY;
		ar << (double)m_dMojiKankaku;
		ar << (DWORD)m_nMojiColor;
		ar << (DWORD)m_nMojiShu;

		// �����ʒu�����̍s�ԁA������
		ar << (double)m_dMojiSeiriGyouKan;
		ar << (double)m_dMojiSeiriSuu;

		// ������_�̂���ʒu�g�p�̃t���O
		ar << (DWORD)m_nMojiKijunZureOn;

		// ������_�̉������̂���ʒu���A���A�E
		ar << (double)m_adMojiKijunZureX[0];
		ar << (double)m_adMojiKijunZureX[1];
		ar << (double)m_adMojiKijunZureX[2];

		// ������_�̏c�����̂���ʒu���A���A��
		ar << (double)m_adMojiKijunZureY[0];
		ar << (double)m_adMojiKijunZureY[1];
		ar << (double)m_adMojiKijunZureY[2];

		TRACE("Finish saving header.\n");
	}
	// �ǂݍ���
	else
	{
		// JWW�̃f�[�^�t�@�C���錾
		ar.Read(m_cVersion, 8);

		// �o�[�W����No.
		ar >> m_Version;
		if (m_Version < 230)
		{
			printf("\nFile version is too old.. Use Jw_cad and save as the latest version.\n");
			return;
			//AfxMessageBox(L"\nFile version is too old.. Use Jw_cad and save as the latest version.\n");
			//AfxThrowUserException();
		}

		// �t�@�C������
		ar >> m_strMemo;

		// �}�ʃT�C�Y
		// 0�`4 �FA0�`A4
		// 8    �F2 A
		// 9    �F3 A
		// 10   �F4 A
		// 11   �F5 A
		// 12   �F10m
		// 13   �F50m
		// 14   �F100m
		ar >> m_nZumen;

		// ��w�O���[�v�E��w���
		// ������w�O���[�v
		// ��w�O���[�v���(0�F��\���A1�F�\���̂݁A2�F�ҏW�\�A3�F����)
		// ��w�O���[�v�̏�����w
		// ��w�O���[�v�̏k��(�̕���)
		// ��w�O���[�v�̃v���e�N�g�w��
		// �@(0�F���w��A1�F�\����ԕύX�\�v���e�N�g�w��A2�F�\����ԌŒ�v���e�N�g�w��)
		// ��w���(0�F��\���A1�F�\���̂݁A2�F�ҏW�\�A3�F����)
		// ��w�̃v���e�N�g�w��
		// �@(0�F���w��A1�F�\����ԕύX�\�v���e�N�g�w��A2�F�\����ԌŒ�v���e�N�g�w��)
		ar >> m_nWriteGLay;

		for (int nGLay = 0; nGLay < 16; nGLay++)
		{
			ar >> m_anGLay[nGLay];
			ar >> m_anWriteLay[nGLay];
			ar >> m_adScale[nGLay];
			ar >> m_anGLayProtect[nGLay];

			for (int nLay = 0; nLay < 16; nLay++)
			{
				ar >> m_aanLay[nGLay][nLay];
				ar >> m_aanLayProtect[nGLay][nLay];
			}
		}

		// �_�~�[
		for (i = 0; i < 14; i++)
		{
			ar >> dummy;
		}

		// ���@�֌W�̐ݒ�
		// ���ݒ�t�@�C���́uS_COMM_8�v�̇B���@�ݒ肪�u10�v�܂��́u11�v�ɂȂ��Ă��āA
		// �t�@�C���ւ̕ۑ����ł���ꍇ�ɗL���ɂȂ�A
		// ����ȊO�́um_lnSunpou1�v�`�um_lnSunpou5�v�́u0�v�ɂȂ�B

		// ��ʁF���@���F�i1�`9�j
		// �\�ʁF���o���F�i1�`9�j
		// �S�ʁF���@�_�F�i1�`9�j
		// ��ʁF���@�l�̏����_�ȉ��̌����i0�`3�j
		// ���ʁF���@�P�ʁi0:mm �A1:���j
		// �\���ʁF���@���[���i0:�_ �A1:��� �A2:�t���j
		// �S���ʁF���@������i1�`10�j
		ar >> m_lnSunpou1;
		// ��ʁ`��ʁF���@�l�̐��@���Ƃ̗���
		// �@�@�@�@�@�@����i0�`99.9�j�~�P�O�A�}�C�i�X�̏ꍇ��1000���v���X���Đ���������
		// ���ʁ`�疜�ʁF���@���̓ˏo����
		// �@�@�@�@�@�@�����i0�`99.9�j�~�P�O�A�}�C�i�X�̏ꍇ��1000���v���X���Đ��������A�X��10000�{����B
		ar >> m_lnSunpou2;
		// ��ʁ`�S�ʁF��󒷂�
		// �@�@�@�@�@�@�����i0�`99.9�j�~�P�O�Ƃ��Đ���������
		// ��ʁ`�\���ʁF���p�x
		// �@�@�@�@�@�@�p�x�i0.1�`80.0�j�~�P�O�Ƃ��Đ��������A1000�{����
		// �S���ʁ`���ʁF�t���̏o���@
		// �@�@�@�@�@�@���@�i0�`99.9�j�~�P�O�Ƃ��Đ����������A1000000�{����
		ar >> m_lnSunpou3;
		// ��ʁF���@�l������␳���Ȃ��i0�F�␳�L �A1�F�␳���j
		// �\�ʁF���@�l��S�p�ɂ���i0�F���p �A1�F�S�p�j
		// �S�ʁF���@�l�̃J���}���X�y�[�X�ɂ���i0�F�J���} �A1�F�X�y�[�X�j
		// ��ʁF���@�l�̃J���}��S�p�ɂ���i0�F���p �A1�F�S�p�j
		// ���ʁF���@�l�̏����_��S�p�ɂ���i0�F���p �A1�F�S�p�j
		// �\���ʁF���@�l�ɒP�ʂ�\������i0�F�\���� �A1�F�\���L�j
		// �S���ʁF���a�l�ɁuR�v��\������i0�F�\���� �A1�F�O�t�\���L �A2�F��t�\���L�j
		// �疜�ʁF���a�l�ɃJ���}��\������i0�F�\���� �A1�F�\���L�j
		// ���ʁF���a�l�ɏ����_�́u0�v��\������i0�F�\���� �A1�F�\���L�j
		ar >> m_lnSunpou4;
		// ��ʁ@�F���@�l���΂ߕ����ɂ���i0�F�ʏ핶�� �A1�F�΂ߕ����j
		// �\�ʁ@�F���@�l�𑾕����ɂ���i0�F�ʏ핶�� �A1�F�������j
		// �S�ʁ@�F�p�x�P�ʁi0�F�x�P�� �A1�F�x���b�P�� �A2�F�x�P�ʂŁu߁v�̕\���Ȃ��j
		// ��ʁ@�F�p�x�P�ʂ̏����_�ȉ��̌����i0�`6�j
		// ���ʁ@�F���@�}�`�ɂ���i0�F���@�}�`�ɂ��Ȃ� �A1�F���@�}�`�ɂ���j
		// �\���ʁF�͈͑I���̂Ƃ����@�}�`����F�E��̑����őI���i0�F�I�����Ȃ� �A1�F�I������j
		// �S���ʁF�\�������_�ȉ��̏����i0:�l�̌ܓ��@1:�؎́@2:�؏�j(Ver.4.02�ȍ~�j
		ar >> m_lnSunpou5;

		// �_�~�[
		ar >> dummy;

		// ������̍ő啝(1�`100,-1�`-100,-300)
		// 1����100 �����̓s�N�Z���P�ʂŎw��
		// -1����-100 ������ abs(1/N)�~���P�ʂŎw��(-16->1/16�~��)
		//�u������1/100mm�P�ʂƂ���v���ݒ肳��Ă���Ƃ���-300���w��(-100��菬����)
		ar >> m_nMaxDrawWid;

		// �v�����^�o�͔͈͂̌��_(X,Y)
		ar >> m_DPPrtGenten.x;
		ar >> m_DPPrtGenten.y;

		// �v�����^�o�͔{��
		ar >> m_dPrtBairitsu;

		// �v�����^90����]�o�́A�v�����^�o�͊�_�ʒu
		// ��ʁF�v�����^90����]�o��
		// �\�ʁF�v�����^�o�͊�_�ʒu�̎w��i0:���w��j�iVer.3.00�ȍ~�j
		//  �@  7:����   8:����  9:�E��
		//   �@ 4:����   5:����  6:�E��
		//    �@1:����   2:����  3:�E��
		ar >> m_nPrtSet;

		// �ڐ��ݒ胂�[�h
		// ���(0�F���w��A1�F�ڐ��\��)
		// �\��(0�F�}�ʐ��@�w��A1�F�����w��)
		// �|�l�̎��A�ڐ��ǎ��s��
		ar >> m_nMemoriMode;

		// �ڐ��\���ŏ��Ԋu�h�b�g
		ar >> m_dMemoriHyoujiMin;

		// �ڐ��\���Ԋu(X,Y)
		ar >> m_dMemoriX;
		ar >> m_dMemoriY;

		// �ڐ���_(X,Y)
		ar >> m_DpMemoriKijunTen.x;
		ar >> m_DpMemoriKijunTen.y;

		// ��w��
		for (n = 0; n <= 15; n++)
		{
			for (k = 0; k <= 15; k++)
			{
				ar >> m_aStrLayName[n][k];
			}
		}

		// ��w�O���[�v��
		for (n = 0; n <= 15; n++)
		{
			ar >> m_aStrGLayName[n];
		}

		// ���e�v�Z�̏���
		// ����ʍ���
		// �ܓx
		// 9�`15�̑���̎w��
		// �ǖʓ��e����ʍ���
		ar >> m_dKageLevel;
		ar >> m_dKageIdo;
		ar >> m_nKage9_15JiFlg;
		ar >> m_dKabeKageLevel;

		// �V��}�̏����iVer.3.00�ȍ~)
		// ����ʍ���
		// �V��}�̔��a���Q
		if (m_Version >= 300)
		{
			ar >> m_dTenkuuZuLevel;
			double dTenkuuZuEnkoR;
			ar >> dTenkuuZuEnkoR;
			//m_dTenkuuZuEnkoR = dTenkuuZuEnkoR / 2;
			m_dTenkuuZuEnkoR = dTenkuuZuEnkoR;
		}
		else
		{
			m_dTenkuuZuLevel = 0.0;
			m_dTenkuuZuEnkoR = 0.0;
		}
		// 2.5D�̌v�Z�P��(0�ȊO��mm�P�ʂŌv�Z)
		ar >> m_nMMTani3D;

		// �ۑ����̉�ʔ{��(�Ǎ��ނƑO��ʔ{���ɂȂ�)
		ar >> m_dBairitsu;
		ar >> m_DPGenten.x;
		ar >> m_DPGenten.y;

		// �͈͋L���{���Ɗ�_(X,Y)
		ar >> m_dHanniBairitsu;
		ar >> m_DPHanniGenten.x;
		ar >> m_DPHanniGenten.y;

		// �}�[�N�W�����v�{���A��_(X,Y)����щ�w�O���[�v
		if (m_Version >= 300)
		{
			for (n = 1; n <= 8; n++)
			{
				ar >> m_dZoomJumpBairitsu[n];
				ar >> m_DPZoomJumpGenten[n].x;
				ar >> m_DPZoomJumpGenten[n].y;
				ar >> m_nZoomJumpGLay[n];
			}
		}
		else
		{
			for (n = 1; n <= 4; n++)
			{
				ar >> m_dZoomJumpBairitsu[n];
				ar >> m_DPZoomJumpGenten[n].x;
				ar >> m_DPZoomJumpGenten[n].y;
				m_nZoomJumpGLay[n] = 0xffffffff;
			}
			// �{��=1,���S(0,0)�͖��g�p�̈�ł�����
			for (; n <= 8; n++)
			{
				m_dZoomJumpBairitsu[n] = 1.0;
				m_DPZoomJumpGenten[n].x = 0.0;
				m_DPZoomJumpGenten[n].y = 0.0;
				m_nZoomJumpGLay[n] = 0xffffffff;
			}
		}

		// �����̕`����(Ver.4.05�ȍ~�j
		if (m_Version >= 300)
		{
			ar >> m_dDm11;		//�_�~�[
			ar >> m_dDm12;		//�_�~�[
			ar >> m_dDm13;		//�_�~�[
			ar >> m_lnDm1;			//�_�~�[
			ar >> m_dDm21;		//�_�~�[
			ar >> m_dDm22;		//�_�~�[
			ar >> m_dMojiBG;	//(Ver.4.04�ȑO�̓_�~�[�j
			//������͈͂�w�i�F�ŕ`�悷��Ƃ��͈̔͑����@
			ar >> m_nMojiBG;		//(Ver.4.04�ȑO�̓_�~�[�j
			//�\��:�����i���@�}�`��u���b�N�}�`�j���Ō�ɕ`��
			//���:1 :�֊s�E�͈͂�w�i�F�ŕ`�悵�Ȃ�
			//	2 :�����̗֊s��w�i�F�ŕ`�悷��
			//	3 :������͈͂�w�i�F�ŕ`�悷��
		}
		else
		{
			m_dMojiBG = 0.0;
			m_nMojiBG = 0;
		}
		// �����Ԋu
		for (n = 0; n <= 9; n++)
		{
			ar >> m_adFukusenSuuchi[n];
		}

		// ���������̗����o�̐��@
		ar >> m_dRyoygawaFukusenTomeDe;

		// �F�ԍ����Ƃ̉�ʕ\���F�A����
		// ��ʕ\���F(0�F�w�i�F�A1�`8�F���F�A9�F�O���[�F)
		// ����(1�`16)
		for (n = 0; n <= 9; n++)
		{
			ar >> m_aPenColor[n];
			ar >> m_anPenWidth[n];
		}

		// �F�ԍ����Ƃ̃v�����^�o�͐F�A�����A���_���a
		// �v�����^�o�͐F(0�F�w�i�F�A1�`8�F���F�A9�F�O���[�F)
		// ����(1�`500)
		// ���_���a(0.1�`10)
		for (n = 0; n <= 9; n++)
		{
			ar >> m_aPrtPenColor[n];
			ar >> m_anPrtPenWidth[n];
			ar >> m_adPrtTenHankei[n];
		}

		// ����ԍ�2����9�܂ł̃p�^�[���A1���j�b�g�̃h�b�g���A�s�b�`�A�v�����^�o�̓s�b�`
		for (n = 2; n <= 9; n++)
		{
			ar >> m_alLType[n];
			ar >> m_anTokushuSenUintDot[n];
			ar >> m_anTokushuSenPich[n];
			ar >> m_anPrtTokushuSenPich[n];
		}

		// �����_����1����5�܂ł̃p�^�[���A��ʕ\���U���E�s�b�`�A�v�����^�o�͐U���E�s�b�`
		for (n = 11; n <= 15; n++)
		{
			ar >> m_alLType[n];
			ar >> m_anRandSenWide[n];
			ar >> m_anTokushuSenPich[n];
			ar >> m_anPrtRandSenWide[n];
			ar >> m_anPrtTokushuSenPich[n];
		}

		// �{������ԍ�6����9�܂ł̃p�^�[���A1���j�b�g�̃h�b�g���A�s�b�`�A�v�����^�o�̓s�b�`
		for (n = 16; n <= 19; n++)
		{
			ar >> m_alLType[n];
			ar >> m_anTokushuSenUintDot[n];
			ar >> m_anTokushuSenPich[n];
			ar >> m_anPrtTokushuSenPich[n];
		}

		// ���_����ʕ`�掞�̎w�蔼�a�ŕ`��
		ar >> m_nDrawGamenTen;

		// ���_���v�����^�o�͎��A�w�蔼�a�ŏ���
		ar >> m_nDrawPrtTen;

		// BitMap�E�\���b�h���ŏ��ɕ`�悷��
		ar >> m_nBitMapFirstDraw;
		// �\�ʁF�\���b�h�`�揇
		//  0:��w��        (+1):�F��
		//  3:��w�t��      (+2):�F�t��
		//  6:����o�͐ݒ菇
		// ��ʁF
		//  0:BitMap�E�\���b�h���ŏ��ɕ`�悵�Ȃ�
		//  1:BitMap�E�\���b�h���ŏ��ɕ`��
		//  2:�\���b�h���ŏ��ɕ`��
		// �t�`��
		ar >> m_nGyakuDraw;

		// �t�T�[�`
		ar >> m_nGyakuSearch;

		// �J���[���
		ar >> m_nColorPrint;

		// ��w���̈��
		ar >> m_nLayJunPrint;

		// �F�ԍ����̈��
		ar >> m_nColJunPrint;

		// ��w�O���[�v�܂��͉�w���Ƃ̃v�����^�A���o�͎w��
		ar >> m_nPrtRenzoku;

		// �v�����^���ʉ�w(�\���̂݉�w)�̃O���[�o�͎w��
		ar >> m_nPrtKyoutsuuGray;

		// �v�����^�o�͎��ɕ\���̂݉�w�͏o�͂��Ȃ�
		ar >> m_nPrtDispOnlyNonDraw;

		if (m_Version >= 223)
		{
			// ��}���ԁiVer.2.23�ȍ~�j
			ar >> m_lnDrawTime;

			// 2.5D�̎n�_�ʒu���ݒ肳��Ă��鎞�̃t���O�iVer.2.23�ȍ~�j
			// ��ʁF�����}�̎��_�ʒu�ݒ�ς݃t���O
			// �\�ʁF���Ր}�̎��_�ʒu�ݒ�ς݃t���O
			// �S�ʁF�A�C�\���}�̎��_�ʒu�ݒ�ς݃t���O
			ar >> m_nEyeInit;

			// 2.5D�̓����}�E���Ր}�E�A�C�\���}�̎��_�����p�iVer.2.23�ȍ~�j
			DWORD dEye_H_Ichi_1, dEye_H_Ichi_2, dEye_H_Ichi_3;
			ar >> dEye_H_Ichi_1;
			ar >> dEye_H_Ichi_2;
			ar >> dEye_H_Ichi_3;
			//m_dEye_H_Ichi_1 = (DWORD)(dEye_H_Ichi_1 / 100);
			//m_dEye_H_Ichi_2 = (DWORD)(dEye_H_Ichi_2 / 100);
			//m_dEye_H_Ichi_3 = (DWORD)(dEye_H_Ichi_3 / 100);
			m_dEye_H_Ichi_1 = dEye_H_Ichi_1;
			m_dEye_H_Ichi_2 = dEye_H_Ichi_2;
			m_dEye_H_Ichi_3 = dEye_H_Ichi_3;

			// 2.5D�̓����}�̎��_�����E���_����iVer.2.23�ȍ~�j
			ar >> m_dEye_Z_Ichi_1;
			ar >> m_dEye_Y_Ichi_1;

			// 2.5D�̒��Ր}�̎��_�����E���_����iVer.2.23�ȍ~�j
			ar >> m_dEye_Z_Ichi_2;
			ar >> m_dEye_Y_Ichi_2;

			// 2.5D�̃A�C�\���}�̎��_�����p�iVer.2.23�ȍ~�j
			ar >> m_dEye_V_Ichi_3;
		}
		else
		{
			m_lnDrawTime = 0;
			m_nEyeInit = 0;
			m_dEye_H_Ichi_1 = 0;
			m_dEye_H_Ichi_2 = 0;
			m_dEye_H_Ichi_3 = 0;
			m_dEye_Z_Ichi_1 = 0.0;
			m_dEye_Y_Ichi_1 = 0.0;
			m_dEye_Z_Ichi_2 = 0.0;
			m_dEye_Y_Ichi_2 = 0.0;
			m_dEye_V_Ichi_3 = 0.0;
		}

		// ���̒����w��̍ŏI�l�iVer.2.25�ȍ~�j
		if (m_Version >= 225)
		{
			ar >> m_dSenNagasaSnpou;
		}
		else
		{
			m_dSenNagasaSnpou = 0.0;
		}

		// ��`���@�����@�E�c���@�w��̍ŏI�l�iVer.2.25�ȍ~�j
		if (m_Version >= 225)
		{
			ar >> m_dBoxSunpouX;
			ar >> m_dBoxSunpouY;
		}
		else
		{
			m_dBoxSunpouX = 0.0;
			m_dBoxSunpouY = 0.0;
		}

		// �~�̔��a�w��̍ŏI�l�iVer.2.25�ȍ~�j
		if (m_Version >= 225)
		{
			ar >> m_dEnHankeiSnpou;
		}
		else
		{
			m_dEnHankeiSnpou = 0.0;
		}

		// �\���b�h��C�ӐF�ŏ����t���O�A�\���b�h�̔C�ӐF�̊���l�iVer.2.30�ȍ~�j
		if (m_Version >= 230)
		{
			ar >> m_nSolidNinniColor;
			ar >> m_SolidColor;	// RGB
		}
		else
		{
			m_nSolidNinniColor = 0;
			m_SolidColor = 0;
		}

		// SXF�Ή��g�����F��`�iVer.4.20�ȍ~�j
		// ��ʕ\���F
		// ��ʕ\������
		// ���F��
		// �v�����^�o�͐F
		// �v�����^�o�͐���
		// �_���a
		if (m_Version >= 420)
		{
			int n1;

			for (n = 0; n <= 256; n++)	////// ��ʕ\���F
			{
				n1 = n + SXCOL_EXT;		//�F�ԍ��̃I�t�Z�b�g = +100
				ar >> m_aPenColor[n1];
				ar >> m_anPenWidth[n1];
			}
			for (n = 0; n <= 256; n++)	////// �v�����^�o�͐F
			{
				ar >> m_astrUDColorName[n];
				n1 = n + SXCOL_EXT;	//�F�ԍ��̃I�t�Z�b�g = +100
				ar >> m_aPrtPenColor[n1];
				ar >> m_anPrtPenWidth[n1];
				ar >> m_adPrtTenHankei[n1];
			}

			// SXF�Ή��g�������`�iVer.4.20�ȍ~�j
			// �p�^�[��
			// 1���j�b�g�̃h�b�g��
			// �s�b�`
			// �v�����^�o�̓s�b�`
			// ���햼
			// �Z�O�����g��
			// �s�b�`�����̒����A�󔒒����̌J��Ԃ�
			for (n = 0; n <= 32; n++)	////// ����p�^�[��
			{
				n1 = n + SXLTP_EXT;   //����ԍ��̃I�t�Z�b�g = +30
				ar >> m_alLType[n1];
				ar >> m_anTokushuSenUintDot[n1];
				ar >> m_anTokushuSenPich[n1];
				ar >> m_anPrtTokushuSenPich[n1];
			}
			for (n = 0; n <= 32; n++)	////// ����p�����[�^
			{
				ar >> m_astrUDLTypeName[n];
				ar >> m_anUDLTypeSegment[n];
				for (j = 1; j <= 10; j++)
				{
					ar >> m_aadUDLTypePitch[n][j];
				}
			}
		}
		else
		{
			// TODO: SXF�F,����̊���l��ݒ肷�ׂ�
		}

		// ������1����10�܂ł̕������A�����A�Ԋu�A�F�ԍ�
		for (i = 1; i <= 10; i++)
		{
			ar >> m_adMojiX[i];
			ar >> m_adMojiY[i];
			ar >> m_adMojiD[i];
			ar >> m_anMojiCol[i];
		}

		// �����ݕ����̕������A�����A�Ԋu�A�F�ԍ��A�����ԍ�
		ar >> m_dMojiSizeX;
		ar >> m_dMojiSizeY;
		ar >> m_dMojiKankaku;
		ar >> m_nMojiColor;
		ar >> m_nMojiShu;

		// �����ʒu�����̍s�ԁA������
		ar >> m_dMojiSeiriGyouKan;
		ar >> m_dMojiSeiriSuu;

		// ������_�̂���ʒu�g�p�̃t���O
		ar >> m_nMojiKijunZureOn;

		// ������_�̉������̂���ʒu���A���A�E
		ar >> m_adMojiKijunZureX[0];
		ar >> m_adMojiKijunZureX[1];
		ar >> m_adMojiKijunZureX[2];

		// ������_�̏c�����̂���ʒu���A���A��
		ar >> m_adMojiKijunZureY[0];
		ar >> m_adMojiKijunZureY[1];
		ar >> m_adMojiKijunZureY[2];

		TRACE("Finish reading header.\n");

		// �k�ڂƃI�t�Z�b�g��ݒ�
		DPoint pDummy;
		pDummy.x = 0.0;
		pDummy.y = 0.0;
		SetDataScale(-1.0, pDummy);
	}
}

// �X�P�[���K�p
void CJwwHeader::DoDataScale(double & dValue)
{
	dValue *= m_dJwwScale;
}

// �X�P�[���K�p
void CJwwHeader::DoDataScale(struct DPoint & pValue)
{
	if (m_pDoc->GetDepth() == 0)
	{
		pValue.x = pValue.x * m_dJwwScale + m_pJwwOffset.x;
		pValue.y = pValue.y * m_dJwwScale + m_pJwwOffset.y;
	}
	else
	{
		pValue.x = pValue.x * m_dJwwScale;
		pValue.y = pValue.y * m_dJwwScale;
	}
}

// �X�P�[���ݒ�
void CJwwHeader::SetDataScale(const double dValue, const struct DPoint & pIn)
{
	// �}�ʂ���ǂݎ��AIMPORT�p
	if (dValue < 0.0)
	{
		// ����l�͐}���̂܂܎g�p����
		m_dJwwScale = 1.0;

		// �ݒ��ǂ�ŁA�k�ڃ��[�h��ݒ�(0:�p���A1:�ő�k�ځA2:���[�U�w��)
		DWORD dwValue = GetPrivateProfileInt(_T("JwwIN"), _T("Paper"), 1, m_pDoc->m_iniPath);
		//DWORD dwValue = FXAPI()->fdt_getconfintW(L"JwwIN",L"Paper",1);
		if (dwValue == 1) // �u�����v��ԑ傫�ȉ�w�O���[�v�k�ڂ�K�p
		{
			m_dJwwScale = 0.0;
			for (int i = 0; i < 16; i++)
			{
				if (m_dJwwScale < m_adScale[i])
				{
					m_dJwwScale = m_adScale[i];
				}
			}
			if (m_dJwwScale <= 0.0)
			{
				m_dJwwScale = 1.0;
			}
		}
		else if (dwValue == 2) // ���[�U���w�肵���k�ڂ��g�p
		{
			// �ϊ��k�ڂ���� 2013.10.28 �l�ł͂Ȃ���w�O���[�v�ԍ��Ŏw�肷��
			TCHAR buf[32] = { 0 };
			GetPrivateProfileString(_T("JwwIN"), _T("Scale"), _T("0"), buf, 32, m_pDoc->m_iniPath);
			//FXAPI()->fdt_getconfstringA("JwwIN","Scale","0",buf,32);
			int group = _tcstol(buf, NULL, 16);
			if (group < 0 || group >= 16)
			{
				group = 0;
			}
			m_dJwwScale = m_adScale[group];
			//TCHAR buf[32] = { 0 };
			//FXAPI()->fdt_getconfstringW(L"JwwIN",L"Scale",L"1.0",buf,32);
			//m_dJwwScale = _tcstod( buf, NULL );
			if (m_dJwwScale <= 0.0)
			{
				m_dJwwScale = 1.0;
			}
		}
		// �I�t�Z�b�g��p���T�C�Y���狁�߂܂�
		// 0�`4:A0�`A4, 8:2A, 9:3A, 10:4A, 11:5A
		if ((0 <= m_nZumen && m_nZumen <= 4) || (8 <= m_nZumen && m_nZumen <= 11))
		{
			m_pJwwOffset.x = CJwwDocument::YOUSHI_SIZE[m_nZumen] / 2.0 * m_dJwwScale;
			m_pJwwOffset.y = CJwwDocument::YOUSHI_TATE[m_nZumen] / 2.0 * m_dJwwScale;
		}
		else // 12:10m, 13:50m, 14:100m
		{
			m_pJwwOffset.x = 0.0;
			m_pJwwOffset.y = 0.0;
		}
	}
	// ������ݒ�AEXPORT�p
	else
	{
		m_dJwwScale = dValue;
		m_pJwwOffset.x = pIn.x;
		m_pJwwOffset.y = pIn.y;
	}
}

// �X�P�[���擾
double CJwwHeader::GetDataScale()
{
	return m_dJwwScale;
}

// �I�t�Z�b�g�擾
void CJwwHeader::GetDataOffset(struct DPoint & pOut)
{
	pOut.x = m_pJwwOffset.x;
	pOut.y = m_pJwwOffset.y;
}

// �o�͔͈͐ݒ�
void CJwwHeader::SetMinMaxPt(const OdGePoint3d& min, const OdGePoint3d& max)
{
	m_pJwwMinPt = min;
	m_pJwwMaxPt = max;
}

// �o�͔͈͎擾
void CJwwHeader::GetMinMaxPt(OdGePoint3d& min, OdGePoint3d& max) const
{
	min = m_pJwwMinPt;
	max = m_pJwwMaxPt;
}

// ���͗p�}�ʔ͈�
void CJwwHeader::GetDocumentArea(OdGePoint2d& min, OdGePoint2d& max) const
{
	// ����(0,0)�Ƃ��ėp���T�C�Y�Ə������ݏk�ڂŐ}�ʔ͈͂�ݒ肵�܂�
	min.x = 0.0;
	min.y = 0.0;
	max.x = CJwwDocument::YOUSHI_SIZE[m_nZumen] * m_dJwwScale;
	max.y = CJwwDocument::YOUSHI_TATE[m_nZumen] * m_dJwwScale;
	// �p���T�C�Y�� 12:10m, 13:50m, 14:100m �̂Ƃ��͒��S��(0,0)�ɂ��܂�
	if (12 <= m_nZumen && m_nZumen <= 14)
	{
		max.x /= 2.0; max.y /= 2.0;
		//max /= 2.0;
		min.x = -max.x;
		min.y = -max.y;
	}
}

// �����p�x�␳�ݒ���擾
void CJwwHeader::SetTextOrthoTol()
{
	// �A�v���P�[�V���������擾
	TCHAR buf[32];
	CString appname(_T("JwwOut"));
	GetPrivateProfileString(appname, _T("Command"), _T("_JWEXPORT"), buf, 32, m_pDoc->m_iniPath);
	if (_tcscmp(buf, _T("_JWEXPORT")))
	{
		appname = _T("RevJww");
	}
	m_bFixAng = GetPrivateProfileInt(appname, _T("useFixAng"), 0, m_pDoc->m_iniPath);
	int prec = GetPrivateProfileInt(appname, _T("FixAngPrec"), 10, m_pDoc->m_iniPath);
	m_tFixAng = pow(10.0, -prec);
}

// �����p�x�␳�ݒ���擾
BOOL CJwwHeader::GetTextOrthoTol(double& tVal)
{
	tVal = m_tFixAng;
	return m_bFixAng;
}
#ifdef _DEBUG
void CJwwHeader::Dump(CDumpContext &dc) const
{
	CObject::Dump(dc);
	// JWW�̃f�[�^�t�@�C���錾
	dc << "\n----- CJwwHeader -----\n";
	if (strncmp(m_cVersion, "JwwData.", 8) != 0)
	{
		dc << "Invalidate CJwwHeader\n";
		return;
	}
	dc << "\nVersion = " << m_Version;
	dc << "\nFile memo = " << m_strMemo;
	dc << "\nFile size = " << m_nZumen;
	dc << "\n----- CJwwHeader -----\n";
	dc.Flush();
}
#endif	//_DEBUG

//****************************************
// CData	: JWW �}�`��{�N���X
//****************************************

IMPLEMENT_SERIAL(CData, CObject, VERSIONABLE_SCHEMA);

CData::CData()
	: CObject()
	, m_lGroup(0)
	, m_nPenStyle(0)
	, m_nPenColor(0)
	, m_nPenWidth(0)
	, m_nLayer(0)
	, m_nGLayer(0)
	, m_sFlg(0)
	, m_pHeader(0)
{
}

CData::CData(const CData & src)
	: CObject()
	, m_lGroup(src.m_lGroup)
	, m_nPenStyle(src.m_nPenStyle)
	, m_nPenColor(src.m_nPenColor)
	, m_nPenWidth(src.m_nPenWidth)
	, m_nLayer(src.m_nLayer)
	, m_nGLayer(src.m_nGLayer)
	, m_sFlg(src.m_sFlg)
	, m_pHeader(src.m_pHeader)
{
}

CData::~CData()
{
}

DWORD	CData::version(CArchive& ar)
{
	return m_pHeader->m_Version;
}

void	CData::DoDataScale(CArchive& ar, double &val)
{
	((CJwwDocument*)ar.m_pDocument)->DoDataScale(val);
}

void	CData::DoDataScale(CArchive& ar, DPoint &pnt)
{
	((CJwwDocument*)ar.m_pDocument)->DoDataScale(pnt);
}

CData& CData::operator=(const CData src)
{
	m_lGroup = src.m_lGroup;
	m_nPenStyle = src.m_nPenStyle;
	m_nPenColor = src.m_nPenColor;
	m_nPenWidth = src.m_nPenWidth;
	m_nLayer = src.m_nLayer;
	m_nGLayer = src.m_nGLayer;
	m_sFlg = src.m_sFlg;
	m_pHeader = src.m_pHeader;
	return *this;
}

void CData::Serialize(CArchive& ar)
{
	// �w�b�_���ւ̃|�C���^��ێ�
	/* ATTN
	CJwwDocument *pDoc;
	pDoc = static_cast<CJwwDocument*>(ar.m_pDocument);
	m_pHeader = &pDoc->m_JwwHeader;
	*/
	if (ar.IsStoring())
	{
		ar << (DWORD)m_lGroup;            //�Ȑ������ԍ�
		ar << (BYTE)m_nPenStyle;          //����ԍ�
		ar << (WORD)m_nPenColor;          //���F�ԍ�
		if (m_pHeader->m_Version >= eVer351)
		{
			ar << (WORD)m_nPenWidth;      //���F��
		}
		ar << (WORD)m_nLayer;             //��w�ԍ�
		ar << (WORD)m_nGLayer;            //��w�O���[�v�ԍ�
		ar << (WORD)m_sFlg;               //�����t���O
	}
	else
	{
		ar >> m_lGroup;            //�Ȑ������ԍ�
		ar >> m_nPenStyle;          //����ԍ�
		ar >> m_nPenColor;          //���F�ԍ�
		m_nPenWidth = 0;
		if (m_pHeader->m_Version >= eVer351)
		{
			ar >> m_nPenWidth;      //���F��
		}
		ar >> m_nLayer;             //��w�ԍ�
		ar >> m_nGLayer;            //��w�O���[�v�ԍ�
		ar >> m_sFlg;               //�����t���O
	}
}

#ifdef	_DEBUG
void CData::Dump(class CDumpContext & dc) const
{
	CObject::Dump(dc);
	dc << "\n----- CData -----\n";
	dc << "\nLine group " << m_lGroup;
	dc << "\nPen style" << m_nPenStyle;
	dc << "\nPen color " << m_nPenColor;
	dc << "\nPen width " << m_nPenWidth;
	dc << "\nLayer " << m_nLayer;
	dc << "\nLayer group " << m_nGLayer;
	dc << "\nFlag " << m_sFlg;
	dc.Flush();
}
#endif	//_DEBUG

//****************************************
// CDataSen : JWW ���f�[�^�N���X
//****************************************

IMPLEMENT_SERIAL(CDataSen, CData, VERSIONABLE_SCHEMA);

void CDataSen::Serialize(CArchive& ar)
{
	CData::Serialize(ar);

	if (ar.IsStoring())
	{
		DoDataScale(ar, m_start);
		DoDataScale(ar, m_end);

		ar << (double)m_start.x;
		ar << (double)m_start.y;
		ar << (double)m_end.x;
		ar << (double)m_end.y;
	}
	else
	{
		ar >> m_start.x;
		ar >> m_start.y;
		ar >> m_end.x;
		ar >> m_end.y;

		DoDataScale(ar, m_start);
		DoDataScale(ar, m_end);
	}
}

OdGePoint3d	CDataSen::start(void) const
{
	OdGePoint3d point; point.x = m_start.x; point.y = m_start.y; point.z = 0.0;
	//OdGePoint3d point(m_start.x, m_start.y, 0.0);
	return point;
}

OdGePoint3d	CDataSen::end(void) const
{
	OdGePoint3d point; point.x = m_end.x; point.y = m_end.y; point.z = 0.0;
	//OdGePoint3d point(m_end.x, m_end.y, 0.0);
	return point;
}

void CDataSen::start(const OdGePoint3d& point)
{
	m_start.x = point.x;
	m_start.y = point.y;
}

void CDataSen::end(const OdGePoint3d& point)
{
	m_end.x = point.x;
	m_end.y = point.y;
}

#ifdef	_DEBUG
void CDataSen::Dump(class CDumpContext & dc) const
{
	CData::Dump(dc);
	dc << "\n----- CDataSen -----\n";
	dc << "\nStart " << m_start.x << "," << m_start.y;
	dc << "\nEnd " << m_end.x << "," << m_end.y;
	dc.Flush();
}
#endif	//_DEBUG

//****************************************
// CDataEnko : JWW �~�ʃf�[�^�N���X
//****************************************

IMPLEMENT_SERIAL(CDataEnko, CData, VERSIONABLE_SCHEMA);

void CDataEnko::Serialize(CArchive& ar)
{
	CData::Serialize(ar);

	if (ar.IsStoring())
	{
		DoDataScale(ar, m_start);
		DoDataScale(ar, m_dHankei);

		ar << (double)m_start.x;
		ar << (double)m_start.y;
		ar << (double)m_dHankei;
		ar << (double)m_radKaishiKaku;
		ar << (double)m_radEnkoKaku;
		ar << (double)m_radKatamukiKaku;
		ar << (double)m_dHenpeiRitsu;
		ar << (DWORD)m_bZenEnFlg;
	}
	else
	{
		ar >> m_start.x;
		ar >> m_start.y;
		ar >> m_dHankei;
		ar >> m_radKaishiKaku;
		ar >> m_radEnkoKaku;
		ar >> m_radKatamukiKaku;
		ar >> m_dHenpeiRitsu;
		ar >> m_bZenEnFlg;

		//TRACE("�~�� x=%f, y=%f, r=%f, st=%f, size=%f, tilt=%f, ratio=%f, flg1=%d\n",
		//	m_start.x, m_start.y, m_dHankei,
		//	OdaToDegree(m_radKaishiKaku), OdaToDegree(m_radEnkoKaku), OdaToDegree(m_radKatamukiKaku),
		//	m_dHenpeiRitsu, m_bZenEnFlg);

		DoDataScale(ar, m_start);
		DoDataScale(ar, m_dHankei);
	}
}

OdGePoint3d	CDataEnko::center(void) const
{
	OdGePoint3d point = { m_start.x, m_start.y, 0.0 };
	//OdGePoint3d point(m_start.x, m_start.y, 0.0);
	return point;
}

double	CDataEnko::radius(void) const
{
	return m_dHankei;
}

double	CDataEnko::flattenings(void) const
{
	return m_dHenpeiRitsu;
}

double	CDataEnko::start(void) const
{
	return m_radKaishiKaku;
}

double	CDataEnko::end(void) const
{
	return m_radEnkoKaku;
}

double	CDataEnko::tilt(void) const
{
	return m_radKatamukiKaku;
}

bool	CDataEnko::full(void) const
{
	return (m_bZenEnFlg ? true : false);
}

void	CDataEnko::center(const OdGePoint3d &value)
{
	m_start.x = value.x;
	m_start.y = value.y;
}

void	CDataEnko::radius(double value)
{
	m_dHankei = value;
}

void	CDataEnko::flattenings(double value)
{
	m_dHenpeiRitsu = value;
}

void	CDataEnko::start(double value)
{
	m_radKaishiKaku = value;
}

void	CDataEnko::end(double value)
{
	m_radEnkoKaku = value;
}

void	CDataEnko::tilt(double value)
{
	m_radKatamukiKaku = value;
}

void	CDataEnko::full(bool value)
{
	m_bZenEnFlg = (value ? TRUE : FALSE);
}

#ifdef	_DEBUG
void CDataEnko::Dump(class CDumpContext & dc) const
{
	CData::Dump(dc);
	dc << "\n----- CDataEnko -----\n";
	dc << "\nCenter " << m_start.x << "," << m_start.y;
	dc << "\nRadius " << m_dHankei;
	dc << "\nStart angle " << m_radKaishiKaku;
	dc << "\nArc angle " << m_radEnkoKaku;
	dc << "\nSlope angle " << m_radKatamukiKaku;
	dc << "\nEllipse ratio " << m_dHenpeiRitsu;
	dc << "\nIsCircle " << m_bZenEnFlg;
	dc.Flush();
}
#endif	//_DEBUG

//****************************************
// CDataTen : JWW �_�f�[�^�N���X
//****************************************

IMPLEMENT_SERIAL(CDataTen, CData, VERSIONABLE_SCHEMA);

void CDataTen::Serialize(CArchive& ar)
{
	CData::Serialize(ar);

	if (ar.IsStoring())
	{
		DoDataScale(ar, m_start);
		DoDataScale(ar, m_dBairitsu);

		ar << (double)m_start.x;
		ar << (double)m_start.y;
		ar << (DWORD)m_bKariten;

		if (100 == m_nPenStyle)
		{
			ar << (DWORD)m_nCode;
			ar << (double)m_radKaitenKaku;
			ar << (double)m_dBairitsu;
		}
	}
	else
	{
		ar >> m_start.x;
		ar >> m_start.y;
		ar >> m_bKariten;

		if (100 == m_nPenStyle)
		{
			ar >> m_nCode;
			ar >> m_radKaitenKaku;
			ar >> m_dBairitsu;
		}
		else
		{
			m_nCode = 0;
			m_radKaitenKaku = 0.0;
			m_dBairitsu = 1.0;
		}
		m_nPenStyle = 1;	// SHAPE���g�p����Ȃ�s�v

		DoDataScale(ar, m_start);
		DoDataScale(ar, m_dBairitsu);
	}
}

OdGePoint3d	CDataTen::position(void) const
{
	OdGePoint3d point = { m_start.x, m_start.y, 0.0 };
	//OdGePoint3d point(m_start.x, m_start.y, 0.0);
	return point;
}

void CDataTen::position(const OdGePoint3d & value)
{
	m_start.x = value.x;
	m_start.y = value.y;
}

void CDataTen::pseudo(DWORD value)
{
	m_bKariten = value;
}

void CDataTen::code(DWORD value)
{
	m_nCode = value;
}

void CDataTen::rotation(double value)
{
	m_radKaitenKaku = value;
}

void CDataTen::scale(double value)
{
	m_dBairitsu = value;
}

#ifdef	_DEBUG
void CDataTen::Dump(class CDumpContext & dc) const
{
	CData::Dump(dc);
	dc << "\n----- CDataEnko -----\n";
	dc << "\nPoint " << m_start.x << "," << m_start.y;
	dc << "\nisVirtual " << m_bKariten;
	dc << "\nCode " << m_nCode;
	dc << "\nRotation " << m_radKaitenKaku;
	dc << "\nScale " << m_dBairitsu;
	dc.Flush();
}
#endif	//_DEBUG

//////////////////////////////////////////////////////////////////////
// �����f�[�^�N���X

IMPLEMENT_SERIAL(CDataMoji, CData, VERSIONABLE_SCHEMA);

void CDataMoji::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		DoDataScale(ar, m_start);
		DoDataScale(ar, m_end);
		DoDataScale(ar, m_dSizeX);
		DoDataScale(ar, m_dSizeY);
		DoDataScale(ar, m_dKankaku);
		// �����̐��@�t���O�͐����ŕۑ�����
		m_nPenWidth = m_nSunpouFlg;
		// �}�`���ʃf�[�^��ۑ�
		CData::Serialize(ar);
		// �����ŗL�̃f�[�^��ۑ�
		ar << (double)m_start.x;
		ar << (double)m_start.y;
		ar << (double)m_end.x;
		ar << (double)m_end.y;
		ar << (DWORD)m_nMojiShu;
		ar << (double)m_dSizeX;
		ar << (double)m_dSizeY;
		ar << (double)m_dKankaku;
		ar << (double)m_degKakudo;
		ar << CStringA(m_strFontName);
		ar << CStringA(m_string);
	}
	else
	{
		// �}�`���ʃf�[�^���擾
		CData::Serialize(ar);
		// �����̐��@�t���O�͐����ɂ���
		m_nSunpouFlg = m_nPenWidth;
		// �����̓y���F����ݒ�(0)�ɂ���
		m_nPenWidth = 0;
		// �����ŗL�̃f�[�^���擾
		ar >> m_start.x;
		ar >> m_start.y;
		ar >> m_end.x;
		ar >> m_end.y;
		ar >> m_nMojiShu;
		ar >> m_dSizeX;
		ar >> m_dSizeY;
		ar >> m_dKankaku;
		ar >> m_degKakudo;
		ar >> m_strFontName;
		ar >> m_string;

		DoDataScale(ar, m_start);
		DoDataScale(ar, m_end);
		DoDataScale(ar, m_dSizeX);
		DoDataScale(ar, m_dSizeY);
		DoDataScale(ar, m_dKankaku);
	}
}

WORD	CDataMoji::dimension(void) const
{
	return	m_nSunpouFlg;
}

OdGePoint3d	CDataMoji::start(void) const
{
	OdGePoint3d point = { m_start.x, m_start.y, 0.0 };
	//OdGePoint3d point(m_start.x, m_start.y, 0.0);
	return point;
}

OdGePoint3d	CDataMoji::end(void) const
{
	OdGePoint3d point = { m_end.x, m_end.y, 0.0 };
	//OdGePoint3d point(m_end.x, m_end.y, 0.0);
	return point;
}

DWORD	CDataMoji::code(void) const
{
	return m_nMojiShu;
}

double	CDataMoji::sizeX(void) const
{
	return m_dSizeX;
}

double	CDataMoji::sizeY(void) const
{
	return m_dSizeY;
}

double	CDataMoji::spacing(void) const
{
	return m_dKankaku;
}

bool  CDataMoji::italic(void) const
{
	DWORD work = m_nMojiShu;
	if (bold())
		work -= eFutoji;
	return work >= eShatai;
}

bool CDataMoji::bold(void) const
{
	return m_nMojiShu >= eFutoji;
}

OdString CDataMoji::font(void) const
{
	OdString value((LPCTSTR)m_strFontName);
	if (m_sFlg & eTate)
	{
		if (value[0] != _T('@'))
		{
			value.Insert(0, _T("@"));
			//value.insert(0, _T("@"));
		}
	}
	if (bold())
	{
		value += _T(" Bold");
	}
	if (italic())
	{
		value += _T(" Italic");
	}
	return value;
}

double CDataMoji::rotation(void) const
{
	return OdaToRadian(m_degKakudo);
}

OdString CDataMoji::contents(void) const
{
	OdString value((LPCTSTR)m_string);
	return value;
}

void CDataMoji::dimension(WORD value)
{
	m_nSunpouFlg = value;
}

void	CDataMoji::start(OdGePoint3d value)
{
	m_start.x = value.x;
	m_start.y = value.y;
}

void	CDataMoji::end(OdGePoint3d value)
{
	m_end.x = value.x;
	m_end.y = value.y;
}

void	CDataMoji::code(DWORD value)
{
	m_nMojiShu = value;
}

void	CDataMoji::sizeX(double value)
{
	m_dSizeX = value;
}

void	CDataMoji::sizeY(double value)
{
	m_dSizeY = value;
}

void	CDataMoji::spacing(double value)
{
	m_dKankaku = value;
}

void	CDataMoji::rotation(double value)
{
	m_degKakudo = OdaToDegree(value);
	// �p�x�␳(2016-12-15)
	double tVal;
	if (m_pHeader->GetTextOrthoTol(tVal))
	{
		if (OdEqual(m_degKakudo, 0.0, tVal))
			m_degKakudo = 0.0;
		if (OdEqual(m_degKakudo, 90.0, tVal))
			m_degKakudo = 90.0;
		if (OdEqual(m_degKakudo, 180.0, tVal))
			m_degKakudo = 180.0;
		if (OdEqual(m_degKakudo, 270.0, tVal))
			m_degKakudo = 270.0;
		if (OdEqual(m_degKakudo, 360.0, tVal))
			m_degKakudo = 0.0;
	}
}

void	CDataMoji::font(OdString value)
{
	// �c�����t�H���g�̐ݒ�� CData::m_flags �ɐݒ�
	WORD flag = flags();
	if (value[0] == _T('@'))
	{
		value.Delete(0, 1);
		//value.deleteChars(0, 1);
		flags(flag | CData::eTate);
	}
	else
	{
		flags(flag & ~CData::eTate);
	}

	// ������̑����E�Α̐ݒ���폜(0�`9999�ɐ؂�l�߂�)
	m_nMojiShu %= CDataMoji::eMask;

	int pos = value.Find(_T(" Bold"));
	//int pos = value.find(_T(" Bold"));
	if (pos >= 0)
	{
		m_nMojiShu += CDataMoji::eFutoji;
		value.Delete(pos, 5);
		//value.deleteChars(pos, 5);
	}
	pos = value.Find(_T(" Italic"));
	//pos = value.find(_T(" Italic"));
	if (pos >= 0)
	{
		m_nMojiShu += CDataMoji::eShatai;
		value.Delete(pos, 7);
		//value.deleteChars(pos, 7);
	}

	// �t�H���g����ݒ�
	m_strFontName = (LPCTSTR)value;
}

void	CDataMoji::contents(OdString & value)
{
	m_string = (LPCTSTR)value;
}

#ifdef	_DEBUG
void CDataMoji::Dump(class CDumpContext & dc) const
{
	CData::Dump(dc);
	dc << "\n----- CDataMoji -----\n";
	dc << "\nDimType " << m_nSunpouFlg;
	dc << "\nStart " << m_start.x << "," << m_start.y;
	dc << "\nEnd " << m_end.x << "," << m_end.y;
	dc << "\nStyle " << m_nMojiShu;
	dc << "\nWidth " << m_dSizeX;
	dc << "\nHeight " << m_dSizeY;
	dc << "\nSpace " << m_dKankaku;
	dc << "\nAngle " << m_degKakudo;
	dc << "\nFont " << m_strFontName;
	dc << "\nText " << m_string;
	dc.Flush();
}
#endif	//_DEBUG

//****************************************
// CDataSunpou : JWW ���@�f�[�^�N���X
//****************************************

IMPLEMENT_SERIAL(CDataSunpou, CData, VERSIONABLE_SCHEMA);

void CDataSunpou::Serialize(CArchive& ar)
{
	CData::Serialize(ar);

	if (ar.IsStoring())
	{
		m_Sen.Serialize(ar);
		m_Moji.Serialize(ar);
		if (m_pHeader->m_Version >= eVer420)
		{
			ar << (WORD)m_bSxfMode;
			m_SenHo1.Serialize(ar);
			m_SenHo2.Serialize(ar);
			m_Ten1.Serialize(ar);
			m_Ten2.Serialize(ar);
			m_TenHo1.Serialize(ar);
			m_TenHo2.Serialize(ar);
		}
	}
	else
	{
		m_Sen.Serialize(ar);
		m_Moji.Serialize(ar);
		m_bSxfMode = 0;
		if (m_pHeader->m_Version >= eVer420)
		{
			ar >> m_bSxfMode;
			m_SenHo1.Serialize(ar);
			m_SenHo2.Serialize(ar);
			m_Ten1.Serialize(ar);
			m_Ten2.Serialize(ar);
			m_TenHo1.Serialize(ar);
			m_TenHo2.Serialize(ar);
		}

		// ���@�}�`�̃��C���[�ƃO���[�v���C��
		m_Sen.layer(m_nLayer);
		m_Sen.group(m_nGLayer);
		m_Moji.layer(m_nLayer);
		m_Moji.group(m_nGLayer);
		if (m_bSxfMode)
		{
			m_SenHo1.layer(m_nLayer);
			m_SenHo1.group(m_nGLayer);
			m_SenHo2.layer(m_nLayer);
			m_SenHo2.group(m_nGLayer);
			m_Ten1.layer(m_nLayer);
			m_Ten1.group(m_nGLayer);
			m_Ten2.layer(m_nLayer);
			m_Ten2.group(m_nGLayer);
			m_TenHo1.layer(m_nLayer);
			m_TenHo2.group(m_nGLayer);
		}
	}
}

#ifdef	_DEBUG
void CDataSunpou::Dump(class CDumpContext & dc) const
{
	CData::Dump(dc);
	dc << "\n===== CDataSumpou =====\n";
	dc << "\nSXFmode flag " << m_bSxfMode;
	dc << "\n** DimLine **";
	m_Sen.Dump(dc);
	dc << "\n** DimText **";
	m_Moji.Dump(dc);
	dc << "\n** Extension 1 **";
	m_SenHo1.Dump(dc);
	dc << "\n** Extension 2 **";
	m_SenHo2.Dump(dc);
	dc << "\n** Origin 1 **";
	m_Ten1.Dump(dc);
	dc << "\n** Origin 2 **";
	m_Ten2.Dump(dc);
	dc << "\n** Aux 1 **";
	m_TenHo1.Dump(dc);
	dc << "\n** Aux 2 **";
	m_TenHo2.Dump(dc);
	dc << "\n===== CDataSumpou =====\n";
	dc.Flush();
}
#endif	//_DEBUG

//****************************************
// CDataSolid	: JWW �\���b�h�f�[�^�N���X
//****************************************

IMPLEMENT_SERIAL(CDataSolid, CData, VERSIONABLE_SCHEMA);

void CDataSolid::Serialize(CArchive& ar)
{
	CData::Serialize(ar);

	if (ar.IsStoring())
	{
		// ���ʂ̃\���b�h�f�[�^�̏ꍇ
		if (m_nPenStyle < 101)
		{
			DoDataScale(ar, m_start);
			DoDataScale(ar, m_end);
			DoDataScale(ar, m_DPoint2);
			DoDataScale(ar, m_DPoint3);
		}
		// �~�̃\���b�h�f�[�^�̏ꍇ
		else
		{
			DoDataScale(ar, m_start);
			DoDataScale(ar, m_end.x);
			// �h�[�i�c�\���b�h�̏ꍇ
			if ((m_nPenStyle == 105) || (m_nPenStyle == 106))
			{
				DoDataScale(ar, m_DPoint3.y);
			}
		}

		ar << (double)m_start.x;
		ar << (double)m_start.y;
		ar << (double)m_end.x;
		ar << (double)m_end.y;
		ar << (double)m_DPoint2.x;
		ar << (double)m_DPoint2.y;
		ar << (double)m_DPoint3.x;
		ar << (double)m_DPoint3.y;

		// �y���ԍ���10�̂Ƃ��́A�C�ӂ̐F���ݒ肳��Ă���
		if (10 == m_nPenColor)
		{
			ar << (DWORD)m_Color;	// RGB
		}
	}
	else
	{
		ar >> m_start.x;
		ar >> m_start.y;
		ar >> m_end.x;
		ar >> m_end.y;
		ar >> m_DPoint2.x;
		ar >> m_DPoint2.y;
		ar >> m_DPoint3.x;
		ar >> m_DPoint3.y;

		// �y���ԍ���10�̂Ƃ��́A�C�ӂ̐F���ݒ肳��Ă���
		if (10 == m_nPenColor)
		{
			ar >> m_Color;	// RGB
		}

		// ���ʂ̃\���b�h�f�[�^�̏ꍇ
		if (m_nPenStyle < 101)
		{
			DoDataScale(ar, m_start);
			DoDataScale(ar, m_end);
			DoDataScale(ar, m_DPoint2);
			DoDataScale(ar, m_DPoint3);
		}
		// �~�̃\���b�h�f�[�^�̏ꍇ
		else
		{
			DoDataScale(ar, m_start);
			DoDataScale(ar, m_end.x);
			// �h�[�i�c�\���b�h�̏ꍇ
			if ((m_nPenStyle == 105) || (m_nPenStyle == 106))
			{
				DoDataScale(ar, m_DPoint3.y);
			}
		}
	}
}

OdGePoint3d	CDataSolid::location(const int index) const
{
	OdGePoint3d point;
	switch (index)
	{
	case 0:
		point.x = m_start.x;
		point.y = m_start.y;
		point.y = 0.0;
		//point = OdGePoint3d(m_start.x, m_start.y, 0.0);
		break;
	case 1:
		point.x = m_end.x;
		point.y = m_end.y;
		point.y = 0.0;
		//point = OdGePoint3d(m_end.x, m_end.y, 0.0);
		break;
	case 2:
		point.x = m_DPoint2.x;
		point.y = m_DPoint2.y;
		point.y = 0.0;
		//point = OdGePoint3d(m_DPoint2.x, m_DPoint2.y, 0.0);
		break;
	case 3:
		point.x = m_DPoint3.x;
		point.y = m_DPoint3.y;
		point.y = 0.0;
		//point = OdGePoint3d(m_DPoint3.x, m_DPoint3.y, 0.0);
		break;
	}
	return point;
}

void CDataSolid::location(const int index, const OdGePoint3d value)
{
	switch (index)
	{
	case 0:
		m_start.x = value.x;
		m_start.y = value.y;
		break;
	case 1:
		m_end.x = value.x;
		m_end.y = value.y;
		break;
	case 2:
		m_DPoint2.x = value.x;
		m_DPoint2.y = value.y;
		break;
	case 3:
		m_DPoint3.x = value.x;
		m_DPoint3.y = value.y;
		break;
	}
}

OdGePoint3d	CDataSolid::center(void) const
{
	OdGePoint3d point = { m_start.x, m_start.y, 0.0 };
	//OdGePoint3d point(m_start.x, m_start.y, 0.0);
	return point;
}

double CDataSolid::radius(void) const
{
	return m_end.x;	// �~�\���b�h�ł͊O�a
}

double  CDataSolid::inner(void) const
{
	return m_DPoint3.y; // �~�\���b�h�̓��a
}

double CDataSolid::flattenings(void) const
{
	return m_end.y;	// �G����
}

double CDataSolid::start(void) const
{
	return m_DPoint2.y;
}

double CDataSolid::end(void) const
{
	return m_DPoint3.x;
}

double CDataSolid::tilt(void) const
{
	return  m_DPoint2.x;
}

bool CDataSolid::full(void) const
{
	return OdEqual(m_DPoint3.y, 100.0);
}

bool CDataSolid::bow(void) const
{
	return OdEqual(m_DPoint3.y, 5.0);
}

bool CDataSolid::circSector(void) const
{
	return OdEqual(m_DPoint3.y, 0.0);
}

#ifdef	_DEBUG
void CDataSolid::Dump(class CDumpContext & dc) const
{
	CData::Dump(dc);
	dc << "\n----- CDataSolid -----\n";
	dc << "\nPoint1 " << m_start.x << "," << m_start.y;
	dc << "\nPoint2 " << m_DPoint2.x << "," << m_DPoint2.y;
	dc << "\nPoint3 " << m_DPoint3.x << "," << m_DPoint3.y;
	dc << "\nPoint4 " << m_end.x << "," << m_end.y;
	dc << "\nFill color " << m_Color;
	dc.Flush();
}
#endif	//_DEBUG

//****************************************
// CDataBlock	: JWW �u���b�N�f�[�^�N���X
//****************************************

IMPLEMENT_SERIAL(CDataBlock, CData, VERSIONABLE_SCHEMA);

void CDataBlock::Serialize(CArchive& ar)
{
	CData::Serialize(ar);

	// ���X�g���ւ̃|�C���^��ێ�
	// 2017/3/6 JWS�t�@�C���̃u���b�N�ɑΉ�
	CJwwDocument *pDoc = (CJwwDocument *)(ar.m_pDocument);
	m_pDataListList = pDoc->GetBlockList();

	if (ar.IsStoring())
	{
		DoDataScale(ar, m_DPKijunTen);

		ar << (double)m_DPKijunTen.x;
		ar << (double)m_DPKijunTen.y;
		ar << (double)m_dBairitsuX;
		ar << (double)m_dBairitsuY;
		ar << (double)m_radKaitenKaku;
		ar << (DWORD)m_nNumber;
	}
	else
	{
		ar >> m_DPKijunTen.x;
		ar >> m_DPKijunTen.y;
		ar >> m_dBairitsuX;
		ar >> m_dBairitsuY;
		ar >> m_radKaitenKaku;
		ar >> m_nNumber;

		DoDataScale(ar, m_DPKijunTen);
	}
}

// �f�[�^���X�g�擾
CDataList* CDataBlock::GetDataList()
{
	CDataList* pDataList;
	POSITION ListListPos = m_pDataListList->GetHeadPosition();
	while (ListListPos)
	{
		pDataList = m_pDataListList->GetNext(ListListPos);
		if (pDataList->m_nNumber == m_nNumber)
		{
			return pDataList;
		}
	}
	return NULL;
}

void CDataBlock::init(CJwwDocument *pDoc)
{
	m_pDataListList = &(pDoc->m_DataListList);
}

DWORD CDataBlock::blockNo() const
{
	return m_nNumber;
}

OdGePoint3d	CDataBlock::start() const
{
	OdGePoint3d point = { m_DPKijunTen.x, m_DPKijunTen.y, 0.0 };
	//OdGePoint3d point(m_DPKijunTen.x, m_DPKijunTen.y, 0.0);
	return point;
}

double	CDataBlock::rotation() const
{
	return m_radKaitenKaku;
}

OdGeScale3d CDataBlock::scale() const
{
	OdGeScale3d result;
	result.sx = m_dBairitsuX;
	result.sy = m_dBairitsuY;
	result.sz = OdEqual(m_dBairitsuX, m_dBairitsuY) ? m_dBairitsuX : 1.0;
	return result;
}

void CDataBlock::blockNo(DWORD	value)
{
	m_nNumber = value;
}

void CDataBlock::start(OdGePoint3d	value)
{
	m_DPKijunTen.x = value.x;
	m_DPKijunTen.y = value.y;
}

void CDataBlock::rotation(double value)
{
	m_radKaitenKaku = value;
}

void CDataBlock::scale(OdGeScale3d value)
{
	m_dBairitsuX = value.sx;
	m_dBairitsuY = value.sy;
}

#ifdef	_DEBUG
void CDataBlock::Dump(class CDumpContext & dc) const
{
	CData::Dump(dc);
	dc << "\n----- CDataBlock -----\n";
	dc << "\nOrigin " << m_DPKijunTen.x << "," << m_DPKijunTen.y;
	dc << "\nScaleX " << m_dBairitsuX;
	dc << "\nScaleY " << m_dBairitsuY;
	dc << "\nRotation " << m_radKaitenKaku;
	dc << "\nNumber " << m_nNumber;
	dc.Flush();
}
#endif	//_DEBUG

//****************************************
// CDataList : JWW �f�[�^���X�g�N���X
//****************************************

IMPLEMENT_SERIAL(CDataList, CData, VERSIONABLE_SCHEMA);

void CDataList::Serialize(CArchive& ar)
{
	CData::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << (DWORD)m_nNumber;
		ar << (DWORD)m_bReffered;
		// 2017/2/15 CTime���o�͂����64bit�������o�͂��Ă��܂���
		// JWW�t�@�C���ł�32bit�������o�͂��ׂ��ł���
		UINT32 tm32 = (UINT32)m_time.GetTime();
		ar << tm32;
		//ar << m_time;
		// 2017/2/15 V7.00�Ȃ̂Ńu���b�N���̌���SXF�u���b�N��ʂ�
		// �u���b�N���ɂȂ��Ƃ��͒ǉ�����(4:��}���i)
		if (m_strName.Find(_T("@@SfigorgFlag@@")) < 0)
		{
			m_strName += _T("@@SfigorgFlag@@4");
		}
		// �u���b�N����MBCS������Ƃ��ďo��
		ar << CStringA(m_strName);
	}
	else
	{
		ar >> m_nNumber;
		ar >> m_bReffered;
		// MFC��32bit������64bit��������K�؂ɔ��肵�ēǂݍ���ł����
		ar >> m_time;
		// MFC��MBCS������Unicode�����񂩂�K�؂ɔ��肵�ēǂݍ���ł����
		ar >> m_strName;
		// Ver.4.10 �ȍ~�A���O�̌���
		// "@@SfigorgFlag@@"�ɑ����āA�����}�`��ʃt���O��t��
		// 1:�����}(���w���W�n)
		// 2:�����}(���n���W�n)�A
		// 3:��}�O���[�v
		// 4:��}���i
	}
	m_DataList.Serialize(ar);
}

OdString CDataList::name() const
{
	OdString result = (LPCTSTR)m_strName;
	return result;
}

#ifdef	_DEBUG
void CDataList::Dump(class CDumpContext & dc) const
{
	CData::Dump(dc);
	dc << "\n----- CDataBlockList -----\n";
	dc << "\nNumber " << m_nNumber;
	dc << "\nUsed " << m_bReffered;
	dc << "\nTime " << m_time.Format(L"%Y-%m-%d %H:%M:%S");
	dc << "\nName " << m_strName;
	dc.Flush();
}
#endif	//_DEBUG

//****************************************
// CJwwDocument	: JWW �t�@�C���N���X
//****************************************

CJwwDocument::CJwwDocument()
{
	// �w�b�_�[����h�L�������g�ւ̎Q�Ƃ�������
	m_JwwHeader.m_pDoc = this;
	// �v�f���̒����̂Ƃ��͉摜��W�J���Ȃ�(����͂���)
	m_bReadForReport = FALSE;
	// ATTN:��w���̕t����
	m_bLayerName = FALSE;
	// ATTN:����ړx�W��
	m_lineFactor = 1.0;
	// ATTN:�����ԍ��̉�w�����Ȃ�
	m_gMatchLayerNumber = TRUE;
}

IMPLEMENT_SERIAL(CJwwDocument, CObject, VERSIONABLE_SCHEMA);

void CJwwDocument::ClearData()
{
	while (!m_DataList.IsEmpty())
	{
		delete m_DataList.RemoveHead();
	}
	while (!m_DataListList.IsEmpty())
	{
		delete m_DataListList.RemoveHead();
	}
	m_JwwEmbbededImageFiles.clear();
}

//BOOL CJwwDocument::OnNewDocument()
//{
//	if (!CDocument::OnNewDocument())
//		return FALSE;
//	return TRUE;
//}

CJwwDocument::~CJwwDocument()
{
}

//BEGIN_MESSAGE_MAP(CJwwDocument, CObject)
//END_MESSAGE_MAP()

void CJwwDocument::Serialize(CArchive& ar)
{
	// �摜�t�@�C���̑��΃p�X���������邽�߂�JWW�t�@�C���p�X���L��
	m_jwwPath = ar.GetFile()->GetFilePath();
	// JWW�t�@�C���ɏ�������
	if (ar.IsStoring())
	{
		// �w�b�_����
		m_JwwHeader.Serialize(ar);

		// �}�`�f�[�^�̏o��
		SetDepth(0);
		m_DataList.Serialize(ar);

		// �}�`�������̂Ƃ��͈ȉ��̏������s��Ȃ�
		if (m_bReadForReport)
			return;

		// �u���b�N�}�`�̏o��
		SetDepth(1);
		m_DataListList.Serialize(ar);

		// ���ߍ��݉摜�̏o��
		//WriteImageFiles(ar);
	}
	// JWW�t�@�C������ǂݍ���
	else
	{
		ClearData();
		// �w�b�_�Ǎ�
		m_JwwHeader.Serialize(ar);
		SetDepth(0);
		// �}�`�f�[�^�̓���
		m_DataList.Serialize(ar);
		SetDepth(1);
		// �u���b�N�}�`�̓���
		m_DataListList.Serialize(ar);
		// ���ߍ��݉摜�̓W�J���J�n
		//ExtructImageFiles(ar);
	}
}

// ***********************************************************
//   �摜�t�@�C�����ߍ���
// ***********************************************************
//     DWORD nFiles;
//     -- nFiles ��J��Ԃ� --
//     CSting embFile;
//     DWORD nFileSize;
//     BYTE  baFile[nFileSize];
//     ------------------------
// ***********************************************************
/*
void CJwwDocument::WriteImageFiles(CArchive& ar)
{
	// �摜�t�@�C���̌����o��
	DWORD	nFiles = (DWORD)m_JwwEmbbededImageFiles.size();
	ar << nFiles;

	// �摜�t�@�C�������k�����ߍ���
	std::set<CString>::iterator pos;
	for (pos = m_JwwEmbbededImageFiles.begin(); pos != m_JwwEmbbededImageFiles.end(); pos++)
	{
		CString	inFile;
		CString outFile;
		CString embFile;
		UINT	nFileSize;
		UINT	nReadSize;
		INT		nWriteSize;
		BYTE	buf[BUFSIZ];
		TCHAR	path[MAX_PATH];
		TCHAR	path2[MAX_PATH];

		//////////////////////////////////////////////////////
		// �摜�t�@�C�� �� ���k�t�@�C�� �̃p�X����p�ӂ���
		//////////////////////////////////////////////////////
		// ���̏W���ɉ摜�t�@�C���̃p�X���������Ă���
		inFile = (*pos);
		// �ꎞ�t�@�C���f�B���N�g�����擾
		GetTempPath(MAX_PATH, (LPTSTR)path);
		GetLongPathName(path, path2, MAX_PATH);
		// ���k�t�@�C���̃p�X�����ꎞ�t�@�C���f�B���N�g���{���̓t�@�C��(�t�@�C���Ɗg���q)�{".gz"
		_tcscpy_s(path, MAX_PATH, inFile);
		outFile.Format(_T("%s%s.gz"), path2, PathFindFileName(path));
		//////////////////////////////////////////////////////
		// ���k�t�@�C�����쐬
		//////////////////////////////////////////////////////
		// �摜�t�@�C����ǂݍ��݃I�[�v��
		FILE* pFile = NULL;
		_tfopen_s(&pFile, inFile, _T("rb"));
		if (pFile == NULL)
		{
			//printf(_T("\n�摜�t�@�C���I�[�v���G���[(%d) : %s"), errno, (LPCTSTR)inFile );
			continue;
		}
		// ���k�t�@�C�����������݃I�[�v��
		memset(buf, 0, BUFSIZ);
		WideCharToMultiByte(CP_ACP, 0, outFile, outFile.GetLength(), (LPSTR)(LPVOID)buf, BUFSIZ, NULL, NULL);
		gzFile	zFile = gzopen((LPCSTR)buf, "wb");
		if (zFile == NULL)
		{
			//printf(_T("\n���k�t�@�C���I�[�v���G���[(%d) : %s"), errno, (LPCTSTR)outFile );
			fclose(pFile);
			continue;
		}
		// �摜�t�@�C����gzip�ň��k
		bool	bFileError = false;
		while (!feof(pFile))
		{
			nReadSize = (UINT)fread(buf, 1, BUFSIZ, pFile);
			if (ferror(pFile))
			{
				//printf(_T("\n�摜�t�@�C�����[�h�G���[ : %s"), (LPCTSTR)inFile );
				bFileError = true;
				break;
			}
			nWriteSize = gzwrite(zFile, buf, nReadSize);
			if (nWriteSize < 0)
			{
				//printf(_T("\n���k�t�@�C�����C�g�G���[ : %s"), (LPCTSTR)outFile );
				bFileError = true;
				break;
			}
		}
		// �t�@�C�������
		fclose(pFile);
		gzclose(zFile);
		//////////////////////////////////////////////////////
		// ���k�t�@�C����JWW�t�@�C���ɒǉ�����
		//////////////////////////////////////////////////////
		if (!bFileError)
		{
			CFile	gz(outFile, CFile::modeRead | CFile::typeBinary);
			// ���k�t�@�C����(�f�B���N�g���Ȃ�)��}�ʃt�@�C���ɃR�s�[����
			_tcscpy_s(path, MAX_PATH, outFile);
			embFile = PathFindFileName(path);
			ar << CStringA(embFile);
			// �t�@�C���T�C�Y��}�ʃt�@�C���ɃR�s�[����
			nFileSize = (UINT)gz.GetLength();
			ar << nFileSize;
			// ���k�f�[�^��}�ʃt�@�C���ɃR�s�[����
			while (nFileSize > 0)
			{
				nReadSize = gz.Read(buf, BUFSIZ);
				ar.Write(buf, nReadSize);
				nFileSize -= nReadSize;
			}
			gz.Close();
		}
		// ���k�t�@�C�����폜
		DeleteFile(outFile);
	}
}
*/
// �摜�t�@�C���W�J
/*
void CJwwDocument::ExtructImageFiles(CArchive& ar)
{
	// JWW�t�@�C���o�[�W�������m�F
	if (m_JwwHeader.m_Version < eVer700)
	{
		return;
	}
	DWORD	nFiles;
	UINT	nFileSize;
	CString	strArchiveFile;
	CString strImageFile;
	bool	bExtruct;
	TCHAR	path[MAX_PATH];
	TCHAR	path2[MAX_PATH];
	BYTE	buf[BUFSIZ];

	// �o�͐悪�Ȃ�������JWW�̂���ʒu�ő�p
	if (m_extructDir.IsEmpty())
	{
		_tcscpy_s(path, MAX_PATH, (LPCTSTR)m_jwwPath);
		PathRemoveFileSpec(path);
		m_extructDir = path;
	}
	// �Y�t�t�@�C�����ŌJ��Ԃ�
	for (ar >> nFiles; nFiles > 0; --nFiles)
	{
		// �摜�t�@�C����(�p�X�Ȃ��A�g���q����)���擾
		ar >> strArchiveFile;
		// �t�@�C���T�C�Y���擾
		ar >> nFileSize;
		ASSERT(nFileSize);
		if (nFileSize == 0)
		{
			continue;
		}
		// �摜�t�@�C���̊��S�p�X���쐬
		bExtruct = false;
		_tcscpy_s(path, MAX_PATH, m_extructDir);
		PathAppend(path, strArchiveFile);
		if (_tcsicmp(PathFindExtension(path), _T(".gz")) == 0)
		{
			bExtruct = true;
			PathRemoveExtension(path); // �Ȃ�Ƃ�.bmp.gz -> �Ȃ�Ƃ�.bmp
		}
		strImageFile = path;
		// �ꎞ�t�@�C���p�f�B���N�g�������t�@�C�����ɒǉ�
		GetTempPath(MAX_PATH, (LPTSTR)path2);
		GetLongPathName(path2, path, MAX_PATH);
		PathAppend(path, strArchiveFile);
		strArchiveFile = path;
		// ���k�t�@�C�����ꎞ�f�B���N�g���ɍ쐬
		if (bExtruct)
		{
			UINT	nReadSize;
			CFile	gz(strArchiveFile, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
			while (nFileSize > 0)
			{
				nReadSize = (nFileSize < BUFSIZ ? nFileSize : BUFSIZ);
				nReadSize = ar.Read(buf, nReadSize);
				gz.Write(buf, nReadSize);
				nFileSize -= nReadSize;
			}
			gz.Close();
			//////////////////////////////////////////////////////
			// �摜�t�@�C�����쐬
			//////////////////////////////////////////////////////
			bool	bFileError = false;
			FILE* pFile = NULL;
			_tfopen_s(&pFile, strImageFile, _T("wb"));
			if (pFile == NULL)
			{
				//printf(_T("\n�摜�t�@�C���I�[�v���G���[(%d) : %s"), errno, (LPCTSTR)strImageFile );
				continue;
			}
			// ���k�t�@�C����ǂݍ��݃I�[�v��
			memset(buf, 0, BUFSIZ);
			WideCharToMultiByte(CP_ACP, 0, strArchiveFile, strArchiveFile.GetLength(), (LPSTR)(LPVOID)buf, BUFSIZ, NULL, NULL);
			gzFile	zFile = gzopen((LPCSTR)buf, "rb");
			if (zFile == NULL)
			{
				//printf(_T("\n���k�t�@�C���I�[�v���G���[(%d) : %s"), errno, (LPCTSTR)strArchiveFile );
				fclose(pFile);
				continue;
			}
			// ���k�t�@�C������摜�t�@�C�����擾
			INT		zReadSize;
			while (!gzeof(zFile))
			{
				zReadSize = gzread(zFile, buf, BUFSIZ);
				if (zReadSize < 0)
				{
					//printf(_T("\n���k�t�@�C�����[�h�G���[ : %s"), (LPCTSTR)strArchiveFile );
					bFileError = true;
					break;
				}
				fwrite(buf, 1, zReadSize, pFile);
				if (ferror(pFile))
				{
					//printf(_T("\n�摜�t�@�C�����C�g�G���[ : %s"), (LPCTSTR)strImageFile );
					bFileError = true;
					break;
				}
			}
			// �t�@�C�������
			fclose(pFile);
			gzclose(zFile);
			// �ꎞ�t�@�C�����폜
			DeleteFile(strArchiveFile);
			// �W�J�G���[���͔��[�ȉ摜�t�@�C�����폜
			if (bFileError)
			{
				DeleteFile(strImageFile);
			}
		}
		// �񈳏k�̉摜�t�@�C���͐}�ʂƓ����f�B���N�g���ɍ쐬
		else
		{
			UINT	nReadSize;
			CFile	gz(strImageFile, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
			while (nFileSize > 0)
			{
				nReadSize = (nFileSize < BUFSIZ ? nFileSize : BUFSIZ);
				nReadSize = ar.Read(buf, nReadSize);
				gz.Write(buf, nReadSize);
				nFileSize -= nReadSize;
			}
			gz.Close();
		}
	}
}
*/

#ifdef _DEBUG
void CJwwDocument::AssertValid() const
{
	AssertValid();
}

void CJwwDocument::Dump(class CDumpContext & dc) const
{
	dc << "\nBlocks " << m_DataListList.GetCount();
	dc << "\nEntities " << m_DataList.GetCount();
	dc.Flush();
}
#endif	//_DEBUG

// �k�ڂƕψʂ̓K�p�Ə�����
void CJwwDocument::DoDataScale(double &dValue)
{
	m_JwwHeader.DoDataScale(dValue);
}

void CJwwDocument::DoDataScale(struct DPoint & pValue)
{
	m_JwwHeader.DoDataScale(pValue);
}

void CJwwDocument::SetDataScale(const double dValue, const struct DPoint & pIn)
{
	m_JwwHeader.SetDataScale(dValue, pIn);
}

double CJwwDocument::GetDataScale()
{
	return m_JwwHeader.GetDataScale();
}

void CJwwDocument::GetDataOffset(struct DPoint & pOut)
{
	return m_JwwHeader.GetDataOffset(pOut);
}

// RAY,XLINE�̏o�͔͈͂̌v�Z�Ɏg��
void CJwwDocument::SetMinMaxPt(const OdGePoint3d& min, const OdGePoint3d& max)
{
	m_JwwHeader.SetMinMaxPt(min, max);
}

void CJwwDocument::GetMinMaxPt(OdGePoint3d& min, OdGePoint3d& max) const
{
	m_JwwHeader.GetMinMaxPt(min, max);
}

// ���͗p�}�ʔ͈�
void CJwwDocument::GetDocumentArea(OdGePoint2d& min, OdGePoint2d& max) const
{
	m_JwwHeader.GetDocumentArea(min, max);
}

// ��}�`�̐�
INT_PTR	CJwwDocument::GetDataCount() const
{
	return m_DataList.GetCount();
}

// ��}�`�̐擪
POSITION CJwwDocument::GetHeadPosition() const
{
	return m_DataList.GetHeadPosition();
}

// �o�[�W�����ԍ�
DWORD CJwwDocument::GetVersion() const
{
	return m_JwwHeader.m_Version;
}