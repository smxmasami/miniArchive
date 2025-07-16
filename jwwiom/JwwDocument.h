//////////////////////////////////////////////////////////////////////
// Jww�f�[�^�ǂݍ��݃N���X�Q ��`�t�@�C��
//
// �ECDocument�h����CJwwDocument�N���X
//   JWW�f�[�^�S�̂��Ǘ�����N���X
//
// �ECObject�h����CJwwHeader�N���X
//   JWW�̃w�b�_����ێ�����N���X
//
// �ECObject�h����CData�N���X
//   JWW�̌X�̃f�[�^��ێ�����e�N���X�̃x�[�X
//
//  2017/3/6 JWS�t�@�C���̃u���b�N�ɑΉ�

#pragma once

/// SXF���[�U��`����̍ő�v�f��
#define	USERDEFINEDLTYPEPITCHES	10

/// SXF�u���b�N���
///
///	"�u���b�N��@@SfigorgFlag@@1"  �����}(���w���W�n)
///	"�u���b�N��@@SfigorgFlag@@2"  �����}(���n���W�n)
///	"�u���b�N��@@SfigorgFlag@@3"	�����Ȑ�(�n�b�`���O�̃p�X)
///	"�u���b�N��@@SfigorgFlag@@4"	���i��`
///
///	JW_CAD��JWW�ۑ�����ƁA�u���b�N���� "@@SfigorgFlag@@4" �����ĕԂ�
///	�u���b�N���̍Ōオ�A@@SfigorgFlag@@1 �Ȃ�A���̂܂܂ł���B
///
#define	SXF_BLOCK_PREFIX	_T("@@Sfigorgflag@@")
#define	SXF_BLOCK_PARTS_ID	_T("@@Sfigorgflag@@4")

/// JWW �o�[�W���� ID
/// 2015-01-15	Jw_cad 8.00 �̃t�@�C���o�[�W������700�ł�
///
enum	JwwVersionId
{
	eVer230 = 230,	// 2.30-2.51
	eVer300 = 300,	// 3.00-3.50a
	eVer351 = 351,	// 3.51-4.10a
	eVer420 = 420,	// 4.20-5.22
	eVer600 = 600,	// 6.00-6.21a
	eVer700 = 700,	// 7.00-8.24a
	eVerDefault = 700
};

//////////////////////////////////////////////////////////////////////
/// �\���̒�`
struct EXTFUNC DPoint
{
	double x;
	double y;
};

// ODA���C�u����
#define OdString	CString
struct OdGePoint2d
{
	double x;
	double y;
};
struct OdGePoint3d
{
	double x;
	double y;
	double z;
};
struct OdGeScale3d
{
	double sx;
	double sy;
	double sz;
};

#define OdaToRadian(x) (x/180.0*3.141592613)
#define OdaToDegree(x) (x*180.0/3.141592613)

bool OdEqual(double x, double y, double z = 1e-5);

class CJwwDocument;
//////////////////////////////////////////////////////////////////////
/// JWW�t�@�C�� �w�b�_���ǂݍ��݃N���X

class EXTFUNC CJwwHeader : public CObject
{
public:
	/// JWW�}�ʂɕۑ������l ---
	/// JWW�̃f�[�^�t�@�C���錾(�f�[�^ 0-7)
	char m_cVersion[8];
	/// �o�[�W����No.
	DWORD m_Version;
	/// �t�@�C������
	CString m_strMemo;
	/// �}�ʃT�C�Y
	DWORD m_nZumen;
	/// ��w�O���[�v�E��w���
	/// �������݉�w�O���[�v�ԍ�
	DWORD m_nWriteGLay;
	/// ��w�O���[�v�̏��(�f�[�^ 0-15)
	DWORD m_anGLay[16];
	/// �O���[�v�ʏ������݉�w�ԍ�(�f�[�^ 0-15)
	DWORD m_anWriteLay[16];
	/// �O���[�v�ʏk��(�f�[�^ 0-15)
	double m_adScale[16];
	/// �O���[�v�ʃv���e�N�g���(�f�[�^ 0-15)
	DWORD m_anGLayProtect[16];
	/// ��w�̏��(�f�[�^ 0-15)(�f�[�^ 0-15)
	DWORD m_aanLay[16][16];
	/// ��w�̃v���e�N�g���(�f�[�^ 0-15)(�f�[�^ 0-15)
	DWORD m_aanLayProtect[16][16];
	/// ���@�֌W�̐ݒ�
	/// ��̈� ���@���̐F(1�`9)
	/// �\�̈� ���o���̐F(1�`9)
	/// �S�̈� ���@�_�̐F(1�`9)
	/// ��̈� �����_�ȉ��̌���(0�`3)
	/// ���̈� 0:mm 1:m
	/// �\���̈� 0:�_ 1:��� 2:�t���
	/// �S���̈� ���@������(1�`10)
	DWORD m_lnSunpou1;
	/// ���̒l���}�C�i�X�̏ꍇ��+1000���Đ���������
	/// 10000�̏�] ���@�l�Ɛ��@���̗���(000-999��0mm�`99.9mm)
	/// 10000�̏�  ���@���̓ˏo����(000-999��0mm�`99.9mm)
	DWORD m_lnSunpou2;
	/// 1000�̏�] ��󒷂�(000-999��0mm�`99.9mm)
	/// 10���̏�] ���p�x(001-080��0.1���`80.0��)
	/// 100���̏�  �t���̏o���@(000-999��0mm�`99.9mm)
	DWORD m_lnSunpou3;
	/// ��̈� ���@�l������␳���Ȃ� 0:�␳�L 1:�␳��
	/// �\�̈� ���@�l��S�p�ɂ��� 0:���p 1:�S�p (�������邩�ʂ̃t�H���g���g����)
	/// �S�̈� ���@�l�̃J���}���X�y�[�X�ɂ��� 0:�J���} 1:�X�y�[�X (����)
	/// ��̈� ���@�l�̃J���}��S�p�ɂ��� 0:���p 1:�S�p (����)
	/// ���̈� ���@�l�̏����_��S�p�ɂ��� 0:���p 1:�S�p (����)
	/// �\���̈� ���@�l�ɒP�ʂ�\������ 0:�� 1:�L
	/// �S���̈� ���a�l��R��\������ 0:�� 1:�O 2:�� (�v����)
	/// �疜�̈� ���a�l�ɃJ���}��\������ 0:�� 1:�L
	/// ���̈� ���a�l�ɏ����_��0��\�� 0:�� 1:�L
	DWORD m_lnSunpou4;
	/// ��̈� ���@�l���Α̂ɂ��� 0:�ʏ� 1:�Α�
	/// �\�̈� ���@�l�𑾎��ɂ��� 0:�ʏ� 1:����
	/// �S�̈� 0:�\�i�x 1:�x���b 2:�x�P�ʂŁ��̕\���Ȃ�
	/// ��̈� �p�x���@�̏����_�ȉ��̌��� 0�`6
	/// ���̈� ���@�}�`�ɂ��� 0:���� 1:���@�ɂ��� (����)
	/// �\���̈� �͈͑I���̂Ƃ��A���@�}�`����F�A�������őI�� 0 ���Ȃ� 1 ����(����)
	/// �S���̈� �\�������_�ȉ��̏��� 0:�l�̌ܓ� 1:�؂�̂� 2:�؂�グ
	DWORD m_lnSunpou5;
	/// ���`��̍ő啝
	DWORD m_nMaxDrawWid;
	/// �v�����^�o�͔͈͂̌��_(X,Y)
	DPoint m_DPPrtGenten;
	/// �v�����^�o�͔{��
	double m_dPrtBairitsu;
	/// �v�����^90����]�o�́A�v�����^�o�͊�_�ʒu
	DWORD m_nPrtSet;
	/// �ڐ��ݒ胂�[�h
	/// ��̈� 0 �w��Ȃ� 1 �w�肠��
	/// �\�̈� 0 �}�� 1 ����
	/// �}�C�i�X�̂Ƃ��ڐ���ǂݎ��s��(=SNAP OFF)
	DWORD m_nMemoriMode;
	/// �ڐ��\���ŏ��Ԋu�h�b�g (����)
	double m_dMemoriHyoujiMin;
	/// �ڐ��\���Ԋu(X,Y)
	/// GRIDSIZE��SNAPSIZE �ɂȂ�
	double m_dMemoriX;
	double m_dMemoriY;
	/// �ڐ���_(X,Y)
	/// GRIDBASE/SNAPBASE �ɂȂ�
	DPoint m_DpMemoriKijunTen;
	/// ��w��
	/// DWG�ŉ�w���Ɏg�p�ł��Ȃ��������܂܂�邱�Ƃ�����̂Œ���(�f�[�^ 0-15)(�f�[�^ 0-15)
	CString m_aStrLayName[16][16];
	/// ��w�O���[�v��
	/// DWG�ŉ�w���Ɏg�p�ł��Ȃ��������܂܂�邱�Ƃ�����̂Œ���(�f�[�^ 0-15)
	CString m_aStrGLayName[16];
	/// ���e�v�Z�̏���
	double m_dKageLevel;
	double m_dKageIdo;
	DWORD m_nKage9_15JiFlg;
	double m_dKabeKageLevel;
	/// �V��}�̏����iVer.3.00�ȍ~)
	double m_dTenkuuZuLevel;
	double m_dTenkuuZuEnkoR;
	/// 2.5D�̌v�Z�P��(0�ȊO��mm�P�ʂŌv�Z)
	DWORD m_nMMTani3D;
	/// �ۑ����̉�ʔ{��(�Ǎ��ނƑO��ʔ{���ɂȂ�)
	double m_dBairitsu;
	DPoint m_DPGenten;
	/// �͈͋L���{���Ɗ�_(X,Y)
	double m_dHanniBairitsu;
	DPoint m_DPHanniGenten;
	/// �}�[�N�W�����v�{��(�f�[�^ 1�`8)
	double m_dZoomJumpBairitsu[9];
	/// �}�[�N�W�����v��_(X,Y)(�f�[�^ 1�`8)
	DPoint m_DPZoomJumpGenten[9];
	/// �}�[�N�W�����v�{�� �L����w�O���[�v(�f�[�^ 1�`8)
	DWORD m_nZoomJumpGLay[9];
	/// �����̕`����(Ver.4.05�ȍ~�j
	double m_dDm11;		//�_�~�[
	double m_dDm12;	//�_�~�[
	double m_dDm13;		//�_�~�[
	DWORD m_lnDm1;		//�_�~�[
	double m_dDm21;		//�_�~�[
	double m_dDm22;		//�_�~�[
	double m_dMojiBG;	//(Ver.4.04�ȑO�̓_�~�[�j
	DWORD m_nMojiBG;	//(Ver.4.04�ȑO�̓_�~�[�j
	/// �����Ԋu(�f�[�^ 0�`9)
	double m_adFukusenSuuchi[10];
	/// ���������̗����o�̐��@
	double m_dRyoygawaFukusenTomeDe;
	/// �F�ԍ����Ƃ̉�ʕ\���F(�f�[�^ 0-9,100-356)
	DWORD m_aPenColor[357];
	/// �F�ԍ����Ƃ̉�ʕ\������(�f�[�^ 0-9,100-356)
	DWORD m_anPenWidth[357];
	/// �F�ԍ����Ƃ�SXF�F��(�f�[�^ 0-256)
	CString m_astrUDColorName[257];
	/// �F�ԍ����Ƃ̃v�����^�o�͐F�A�����A���_���a(�f�[�^ 0-9,100-356)
	DWORD m_aPrtPenColor[357];
	DWORD m_anPrtPenWidth[357];
	double m_adPrtTenHankei[357];
	/// ����ԍ�2����9�܂ł̃p�^�[���A1���j�b�g�̃h�b�g���A�s�b�`�A�v�����^�o�̓s�b�`(�f�[�^2-9,11-15,16-19,30-62)
	/// �{������ԍ�6����9�܂ł̃p�^�[���A1���j�b�g�̃h�b�g���A�s�b�`�A�v�����^�o�̓s�b�`(�f�[�^2-9,11-15,16-19,30-62)
	/// SXF�Ή�����33���̃p�^�[���A1���j�b�g�̃h�b�g���A�s�b�`�A�v�����^�o�̓s�b�`(�f�[�^2-9,11-15,16-19,30-62)
	DWORD m_alLType[63];
	DWORD m_anTokushuSenUintDot[63];
	DWORD m_anTokushuSenPich[63];
	DWORD m_anPrtTokushuSenPich[63];
	/// �����_����1����5�܂ł̃p�^�[���A��ʕ\���U���E�s�b�`�A�v�����^�o�͐U���E�s�b�`(�f�[�^11-15)
	DWORD m_anRandSenWide[16];
	DWORD m_anPrtRandSenWide[16];
	/// ���_����ʕ`�掞�̎w�蔼�a�ŕ`��
	DWORD m_nDrawGamenTen;
	/// ���_���v�����^�o�͎��A�w�蔼�a�ŏ���
	DWORD m_nDrawPrtTen;
	/// BitMap�E�\���b�h���ŏ��ɕ`�悷��
	DWORD m_nBitMapFirstDraw;
	/// �t�`��
	DWORD m_nGyakuDraw;
	/// �t�T�[�`
	DWORD m_nGyakuSearch;
	/// �J���[���
	DWORD m_nColorPrint;
	/// ��w���̈��
	DWORD m_nLayJunPrint;
	/// �F�ԍ����̈��
	DWORD m_nColJunPrint;
	/// ��w�O���[�v�܂��͉�w���Ƃ̃v�����^�A���o�͎w��
	DWORD m_nPrtRenzoku;
	/// �v�����^���ʉ�w(�\���̂݉�w)�̃O���[�o�͎w��
	DWORD m_nPrtKyoutsuuGray;
	/// �v�����^�o�͎��ɕ\���̂݉�w�͏o�͂��Ȃ�
	DWORD m_nPrtDispOnlyNonDraw;
	/// ��}���ԁiVer.2.23�ȍ~�j
	DWORD m_lnDrawTime;
	/// 2.5D�̎n�_�ʒu���ݒ肳��Ă��鎞�̃t���O�iVer.2.23�ȍ~�j
	DWORD m_nEyeInit;
	/// 2.5D�̓����}�E���Ր}�E�A�C�\���}�̎��_�����p�iVer.2.23�ȍ~�j
	DWORD m_dEye_H_Ichi_1;
	DWORD m_dEye_H_Ichi_2;
	DWORD m_dEye_H_Ichi_3;
	/// 2.5D�̓����}�̎��_�����E���_����iVer.2.23�ȍ~�j
	double m_dEye_Z_Ichi_1;
	double m_dEye_Y_Ichi_1;
	/// 2.5D�̒��Ր}�̎��_�����E���_����iVer.2.23�ȍ~�j
	double m_dEye_Z_Ichi_2;
	double m_dEye_Y_Ichi_2;
	/// 2.5D�̃A�C�\���}�̎��_�����p�iVer.2.23�ȍ~�j
	double m_dEye_V_Ichi_3;
	/// ���̒����w��̍ŏI�l�iVer.2.25�ȍ~�j
	double m_dSenNagasaSnpou;
	/// ��`���@�����@�E�c���@�w��̍ŏI�l�iVer.2.25�ȍ~�j
	double m_dBoxSunpouX;
	double m_dBoxSunpouY;
	/// �~�̔��a�w��̍ŏI�l�iVer.2.25�ȍ~�j
	double m_dEnHankeiSnpou;
	/// �\���b�h��C�ӐF�ŏ����t���O�A�\���b�h�̔C�ӐF�̊���l�iVer.2.30�ȍ~�j
	DWORD m_nSolidNinniColor;
	DWORD m_SolidColor;
	/// SXF�Ή��g�����F��`�iVer.4.20�ȍ~�j(�f�[�^ 0-31)
	/// SXF�Ή��g�������`�iVer.4.20�ȍ~�j(�f�[�^ 0-31)
	/// SXF�Ή��g������s�b�`�iVer.4.20�ȍ~�j(�f�[�^ 0-31)(�f�[�^ 1-10)
	CString m_astrUDLTypeName[33];
	DWORD m_anUDLTypeSegment[33];
	double m_aadUDLTypePitch[33][11];
	/// ������1����10�܂ł̕�����(�f�[�^ 1-10)
	double m_adMojiX[11];
	/// ������1����10�܂ł̕�������(�f�[�^ 1-10)
	double m_adMojiY[11];
	/// ������1����10�܂ł̕����Ԋu(�f�[�^ 1-10)
	double m_adMojiD[11];
	/// ������1����10�܂ł̐F�ԍ�(�f�[�^ 1-10)
	DWORD m_anMojiCol[11];
	/// �����ݕ����̕������A�����A�Ԋu�A�F�ԍ��A�����ԍ�
	double m_dMojiSizeX;
	double m_dMojiSizeY;
	double m_dMojiKankaku;
	DWORD m_nMojiColor;
	DWORD m_nMojiShu;
	/// �����ʒu�����̍s�ԁA������
	double m_dMojiSeiriGyouKan;
	double m_dMojiSeiriSuu;
	/// ������_�̂���ʒu�g�p�̃t���O
	DWORD m_nMojiKijunZureOn;
	/// ������_�̉������̂���ʒu���A���A�E(�f�[�^ 0-2)
	double m_adMojiKijunZureX[3];
	/// ������_�̏c�����̂���ʒu���A���A��(�f�[�^ 0-2)
	double m_adMojiKijunZureY[3];
	/// �h�L�������g�ւ̃|�C���^
	CJwwDocument* m_pDoc;
	/// JWW�}�ʂɕۑ������l
private:
	/// �}���̍��W �ϊ��W��
	double m_dJwwScale;
	/// A4�`5A �}�ʂ̍����ς݂�(0,0)�ɂ���I�t�Z�b�g�l
	DPoint m_pJwwOffset;
	/// �o�͔͈�
	OdGePoint3d m_pJwwMinPt;
	OdGePoint3d m_pJwwMaxPt;
	/// �����p�x�␳�ݒ�(0:���Ȃ� 1:����)
	BOOL m_bFixAng;
	/// �����p�x�␳�ݒ�̊p�x�̋��e�l
	double m_tFixAng;

public:
	/// �R���X�g���N�^
	CJwwHeader();
	/// �f�X�g���N�^
	virtual ~CJwwHeader();
	/// �V���A���C�Y
	DECLARE_SERIAL(CJwwHeader);
	virtual void Serialize(CArchive& ar);

	/// ��w���擾
	CString  GetLayerName(
		int iGLayer,      // [I] ��w�O���[�v�ԍ�
		int iLayer);      // [I] ��w�ԍ�

	/// �k�ڂƕψʂ̓K�p�Ə�����
	void DoDataScale(double &dValue);
	void DoDataScale(struct DPoint & pValue);
	void SetDataScale(const double dValue, const struct DPoint & pIn);
	double GetDataScale();
	void GetDataOffset(struct DPoint & pOut);
	/// RAY,XLINE�̏o�͔͈͂̌v�Z�Ɏg��
	void SetMinMaxPt(const OdGePoint3d& min, const OdGePoint3d& max);
	void GetMinMaxPt(OdGePoint3d& min, OdGePoint3d& max) const;
	/// ���͗p�}�ʔ͈�
	void GetDocumentArea(OdGePoint2d& min, OdGePoint2d& max) const;
	/// �����p�x�␳�ݒ���擾
	void SetTextOrthoTol();
	BOOL GetTextOrthoTol(double& dTol);

	/// �_���v
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//////////////////////////////////////////////////////////////////////
/// JWW�}�`��{�N���X

class EXTFUNC CData : public CObject
{
protected:
public:
	DWORD	m_lGroup;		// �Ȑ������ԍ�
	BYTE	m_nPenStyle;	// ����ԍ�
	WORD	m_nPenColor;	// ���F�ԍ�
	WORD	m_nPenWidth;	// ���F��
	WORD	m_nLayer;		// ��w�ԍ�
	WORD	m_nGLayer;		// ��w�O���[�v�ԍ�
	WORD	m_sFlg;			// �����t���O

	CJwwHeader *m_pHeader;	// �w�b�_���ւ̃|�C���^

public:
	// �R���X�g���N�^
	CData();
	CData(const CData & src);
	// �f�X�g���N�^
	virtual ~CData();
	// �V���A���C�Y
	DECLARE_SERIAL(CData);
	virtual void Serialize(CArchive& ar);
	CData& operator=(const CData src);
	// JWW,JWS�̃o�[�W�������擾����
	DWORD	version(CArchive& ar);
	void	DoDataScale(CArchive& ar, double &val);
	void	DoDataScale(CArchive& ar, DPoint &pnt);
	// �_���v
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif

	// ����R�[�h (1�`64,65)
	enum ePenStyle
	{
		eObjectLayer = 65	// �u���b�N�v�f�̉�w��D�悷��
	};

	// �����t���O
	enum eZukeiFlag
	{
		eGeneric = 0x0000,
		eZukei = 0x0010,	//�~�E�_�}�`
		eHatch = 0x0020,	//���E�~�E�_�E�\���b�h�̃n�b�`���O
		eSunpou = 0x0040,	//���@�̉~�E�_
		eTategu = 0x0080,	//����̉~�E�_
		eSen = 0x0800,	//���}�`
		eTateguSen = 0x1000,	//����̐�
		eSunpouSen = 0x2000,	//���@��
		eHorakuGai = 0x8000,	//������ΏۊO�̌���̐�
		eTate = 0x0020,	//�c����
		eMakita = 0x0040,	//�^�k
		eHikage = 0x0080,	//���e
		eHankei = 0x0100,	//���a���@�l
		eChokkei = 0x0200,	//���a���@�l
		eKakudo = 0x0400,	//�p�x���@�l
		eZokusei = 0x0800,	//�}�`�����I���i�����j
		eRuikei = 0x1000,	//�݌v���@�l
		eTateguMoji = 0x2000,	//����̕���
		eNagasa = 0x4000,	//�������@�l
		eTakasa = 0x8000,	//2.5D�����l
		eSunpouObj = 0x2000	//���@�}�`���@�t���O
	};

	// �v���p�e�B�擾�E�ݒ�
	DWORD	curve(void) const { return m_lGroup; }
	BYTE	style(void) const { return m_nPenStyle; }
	WORD	color(void) const { return m_nPenColor; }
	WORD	width(void) const { return m_nPenWidth; }
	WORD	layer(void) const { return m_nLayer; }
	WORD	group(void) const { return m_nGLayer; }
	WORD	flags(void) const { return m_sFlg; }
	void	curve(const DWORD value) { m_lGroup = value; }
	void	style(const BYTE value) { m_nPenStyle = value; }
	void	color(const WORD value) { m_nPenColor = value; }
	void	width(const WORD value) { m_nPenWidth = value; }
	void	layer(const WORD value) { m_nLayer = value; }
	void	group(const WORD value) { m_nGLayer = value; }
	void	flags(const WORD value) { m_sFlg = value; }
	// �w�b�_�[�ւ̃|�C���^��ݒ�
	void	header(CJwwHeader* pH) { m_pHeader = pH; }
};

//////////////////////////////////////////////////////////////////////
/// ���f�[�^�N���X

class EXTFUNC CDataSen : public CData
{
protected:
	DPoint m_start;       // �n�_
	DPoint m_end;         // �I�_

public:
	// �V���A���C�Y
	DECLARE_SERIAL(CDataSen);
	virtual void Serialize(CArchive& ar);
	// �v���p�e�B�擾
	OdGePoint3d	start(void) const;
	OdGePoint3d	end(void) const;
	void	start(const OdGePoint3d& point);
	void	end(const OdGePoint3d& point);

	// �_���v
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//////////////////////////////////////////////////////////////////////
/// �~�ʃf�[�^�N���X

class EXTFUNC CDataEnko : public CData
{
protected:
	DPoint m_start;             // ���S�_
	double m_dHankei;           // ���a
	double m_radKaishiKaku;     // �J�n�p
	double m_radEnkoKaku;       // �~�ʊp
	double m_radKatamukiKaku;   // �X���p
	double m_dHenpeiRitsu;      // �G����
	DWORD m_bZenEnFlg;          // �S�~�t���O

public:
	// �V���A���C�Y
	DECLARE_SERIAL(CDataEnko);
	virtual void Serialize(CArchive& ar);
	// �v���p�e�B�擾
	OdGePoint3d	center(void) const;
	double	radius(void) const;
	double	flattenings(void) const;
	double	start(void) const;
	double	end(void) const;
	double	tilt(void) const;
	bool	full(void) const;
	void	center(const OdGePoint3d &value);
	void	radius(double value);
	void	flattenings(double value);
	void	start(double value);
	void	end(double value);
	void	tilt(double value);
	void	full(bool value);

	// �_���v
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//////////////////////////////////////////////////////////////////////
/// �_�f�[�^�N���X

class EXTFUNC CDataTen : public CData
{
protected:
	DPoint m_start;           // �_���W
	DWORD m_bKariten;         // ���_�t���O
	DWORD m_nCode;            // �_�R�[�h
	double m_radKaitenKaku;   // ��]�p
	double m_dBairitsu;       // �{��

public:
	// �V���A���C�Y
	DECLARE_SERIAL(CDataTen);
	virtual void Serialize(CArchive& ar);
	// �v���p�e�B�擾
	OdGePoint3d	position(void) const;
	DWORD pseudo(void) const { return m_bKariten; }
	DWORD code(void) const { return m_nCode; }
	double rotation(void) const { return m_radKaitenKaku; }
	double scale(void) const { return m_dBairitsu; }
	void position(const OdGePoint3d & value);
	void pseudo(DWORD value);
	void code(DWORD value);
	void rotation(double value);
	void scale(double value);

	// �_���v
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//////////////////////////////////////////////////////////////////////
///
/// �����f�[�^�N���X
///
/// ----------------------------------------------------------
/// ��������ꎮ DWG�ł�����FIELD�I�u�W�F�N�g
/// ----------------------------------------------------------
/// &F			�t�@�C����(���S�p�X)
/// &f			�t�@�C����(�g���q�Ȃ�)
/// %f			�t�@�C����(�g���q����) %f1�`%ff �o�C�g��
/// %SS			�k�� (1/200 �\�L)
/// %ss			�k�� (1/200 �� 200, 5:1 �� 5 �ƕ\��)
/// %SP %sp		�v�����^�o�͎��̏o�͔{���␳����
/// %mm			�}�ʂ̃R�����g
/// %m1			�}�ʂ̃R�����g�P�s��
/// %m2			�}�ʂ̃R�����g�Q�s��
/// =F			�}�ʂ̕ۑ�����(���t�Ǝ����j
/// =f			�}�ʂ̕ۑ�����(���t�̂�)
/// =y			�}�ʂ̕ۑ�����(����Q��)
/// =Y			�}�ʂ̕ۑ�����(����)
/// =m			�}�ʂ̕ۑ�����(��)
/// =ma			�}�ʂ̕ۑ�����(�p�� JAN,FEB,...)
/// =d			�}�ʂ̕ۑ�����(��)
/// =w			�}�ʂ̕ۑ�����(�j��)
/// =wa			�}�ʂ̕ۑ�����(�p�� SUN,MON,...)
/// =h			�}�ʂ̕ۑ�����(�� 12���Ԑ�)
/// =H			�}�ʂ̕ۑ�����(�� 24���Ԑ�)
/// =M			�}�ʂ̕ۑ�����(��)
/// =S			�}�ʂ̕ۑ�����(�b)
/// =n			�}�ʂ̕ۑ�����(�p AM,PM)
/// =N			�}�ʂ̕ۑ�����(   �O,��)
/// %y			���݂̓���(����Q��)
/// &Y			���݂̓���(����)
/// &m			���݂̓���(��)
/// &ma			���݂̓���(�p�� JAN,FEB,...)
/// &d			���݂̓���(��)
/// &w			���݂̓���(�j��)
/// &wa			���݂̓���(�p�� SUN,MON,...)
/// &h			���݂̓���(�� 12���Ԑ�)
/// &H			���݂̓���(�� 24���Ԑ�)
/// &M			���݂̓���(��)
/// &S			���݂̓���(�b)
/// &n			���݂̓���(�p AM,PM)
/// &N			���݂̓���(   �O,��)
/// &T			��}����
/// ----------------------------------------------------------
/// ���䕶���ꎮ DWG�ł�����TEXT,MTEXT�̏����R�[�h
/// ----------------------------------------------------------
/// ^!			����(�Ӗ��s��)
/// ^/           �Α̊J�n
/// ^_           �����J�n
/// ^-           ������J�n
/// ^#           �Α�,����,������̉���
/// ^$1 �` ^$9   �F�ύX(�F�R�[�h1�`9�ɑ�������F)
/// ^\"          �t�H���g���l�r�S�V�b�N�ɂ���
/// ^&           �t�H���g�����ɖ߂�
/// ^%           ������̐ݒ�ɖ߂�
/// ^*           ^* �` ^^ �܂ł̐��䕶���͖����ł��̂܂ܕ\��
/// ^^			�����܂Ő��䕶������
/// ----------------------------------------------------------
/// ���ꕶ���ꎮ DWG�ł�����TEXT,MTEXT�̏����R�[�h
/// ----------------------------------------------------------
/// ^u           ���̕�������t�������ɂ���(������������)
/// ^d           ���̕��������t�������ɂ���(������������)
/// ^c           ���̕����𒆕t�������ɂ���(������������)
/// ^o           ���̕����𒆐S�d�˕����ɂ���(��������3/4)
/// ^w           ���p�Q���𒆐S�d�˕����ɂ��� ��^w99 �Ŋې���99
/// ^b ^B ^n     �d�˕���(�������� ����,1/4,�[��)
/// ^1 �` ^9		�s���ɐݒ肵�An�o�C�g���̕����������k����
/// �E�E�E		�ϓ�����t��(�S�p) �s���Ɏw�肷��
/// �E�E�E�		�ϓ�����t��(���p) �s���Ɏw�肷��
///	�s���ɁE�������ċϓ�����t���ɂ������Ȃ��Ƃ��͋󔒂�S�p�󔒂�����
///
//////////////////////////////////////////////////////////////////////
/// �摜�`��
///
/// JW-CAD�̉摜�͕����}�`�̕�����ɉ摜�̃p�����[�^���L�q����
/// ������̍ŏ���4�������u^@BM�v�̏ꍇ�A�摜�̃p�����[�^�ł���
/// CJwwDocument::CreateImageDictionary() �ɂ���ĉ摜�͐}�ʂɓo�^����Ă���̂�
/// �摜��`�̃f�B�N�V���i������摜��`(IMAGEDEF)���擾����
/// �摜�}�`(IMAGE)�Ɖ摜��`�̃��A�N�^(IMAGEDEF_REACTOR)����闬��ɂȂ�
///
/// �p�����[�^�̗�:
///   ^@BMC:\abc.bmp,100,100 ����΃p�X (C:\abc.bmp���T�C�Y100,100�ŕ\��)
///   ^@BMabc.bmp,100,100    �����΃p�X (�}�ʃt�@�C���Ɠ����t�H���_�ɂ���abc.bmp��\��)
///   ^@BM\abc.bmp,100,100   ���h���C�u���w���΃p�X (�}�ʃt�@�C���Ɠ����h���C�u�ɂ���\abc.bmp��\��)
/// �N���b�s���O�Ɖ摜�̉�]:
///   ^@BMabc.bmp,100,100,0.25,0.25,0.5,30 �� �摜�t�@�C���̍���1/4��蕝1/2��\�����A30�x��]
///   ^@BMabc.bmp,100,100,0,0,1,30 �� �N���b�s���O�Ȃ��A30�x��]�̂�
/// �摜�t�@�C���̖��ߍ���(Ver7.00�`):
///   ^@BMP%temp%abc.bmp,100,100 �p�X���� %temp% �ɂȂ��Ă�����͖̂��ߍ��݂ł���
///   DWGPREFIX �̃f�B���N�g���� CJwwDocument::ExtructImageFiles()�œW�J�ς�
/// Susie�v���O���C��:
///	 Susie�v���O�C�����g����JW-CAD�͂��܂��܂ȉ摜�t�H�[�}�b�g��������悤�ɂȂ�
///	 ICAD(ACAD)�ŗ��p�ł��Ȃ��摜�t�@�C���́ACreateImageDictionary()�ŏ��O����
///	 ���p�ł���摜�t�H�[�}�b�g�̊g���q:
///		.bmp, .dib, .rle, .gif, .jpg, .jpeg, .png, .tif, .tiff
/// �⑫:
///   �J���}���p�����[�^�̋�؂�ɂȂ��Ă���̂ŁA�摜�t�@�C�����ɃJ���}�͊܂܂�Ȃ�
//////////////////////////////////////////////////////////////////////
/// ���@�t���O
///
/// ���@�t���O��WORD�ł���肷��K�v���������̂ŏC��
//////////////////////////////////////////////////////////////////////

class EXTFUNC CDataMoji : public CData
{
protected:
	WORD m_nSunpouFlg;      // ���@�t���O
	DPoint m_start;         // �n�_���W
	DPoint m_end;           // �I�_���W
	DWORD m_nMojiShu;       // ������
	double m_dSizeX;        // �����T�C�Y��
	double m_dSizeY;        // �����T�C�Y�c
	double m_dKankaku;      // �����Ԋu
	double m_degKakudo;     // ��]�p
	CString m_strFontName;  // �t�H���g��
	CString m_string;       // ������

public:
	// �V���A���C�Y
	DECLARE_SERIAL(CDataMoji);
	virtual void Serialize(CArchive& ar);

	// �v���p�e�B�擾
	WORD	dimension(void) const;
	OdGePoint3d	start(void) const;
	OdGePoint3d	end(void) const;
	DWORD	code(void) const;
	double	sizeX(void) const;
	double	sizeY(void) const;
	double	spacing(void) const;
	OdString	font(void) const;
	double	rotation(void) const;
	OdString	contents(void) const;
	bool	italic(void) const;
	bool	bold(void) const;
	void	dimension(WORD value);
	void	start(OdGePoint3d value);
	void	end(OdGePoint3d end);
	void	code(DWORD value);
	void	sizeX(double value);
	void	sizeY(double value);
	void	spacing(double value);
	void	rotation(double value);
	void	font(OdString value);
	void	contents(OdString & value);

	// �_���v
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
	// ������t���O
	enum	eMojishu {
		eMask = 10000,	//�}�X�N�l
		eShatai = 10000,	//�Α�
		eFutoji = 20000		//����
	};
	enum eSunpou
	{
		eUnDoc = 0x0001,		// �d�l�ɂȂ��l
		eMoji = 0x0002,			// ���@�̕����ݒ肠��
		eZenkaku = 0x0008,		// �S�p
		eTani = 0x0010,			// �P��
		eTaniTsuika = 0x0020,	// �P�ʒǉ�
		eComma = 0x0040,		// �J���}
		eZero = 0x0080,			// �����_�ȉ��O�\������
		eKirisute = 0x0100,		// �؂�̂�
		eKiriage = 0x0200,		// �؂�グ
		eMaeKigo = 0x0400,		// ���a,���a�L����O�ɂ���
		eUshiroKigo = 0x0800,	// ���a,���a�L�������ɂ���
		eDigit1 = 0x1000,		// �����_�P��
		eDigit2 = 0x2000,		// �����_�Q��
		eDigit3 = 0x3000,		// �����_�R��
	};
};

//////////////////////////////////////////////////////////////////////
/// ���@�f�[�^�N���X

class EXTFUNC CDataSunpou : public CData
{
protected:
	CDataSen m_Sen;       // ���f�[�^
	CDataMoji m_Moji;     // �����f�[�^

	WORD m_bSxfMode;      // SXF���[�h�t���O
	CDataSen m_SenHo1;    //
	CDataSen m_SenHo2;    //
	CDataTen m_Ten1;      //
	CDataTen m_Ten2;      //
	CDataTen m_TenHo1;    //
	CDataTen m_TenHo2;    //

public:
	// �V���A���C�Y
	DECLARE_SERIAL(CDataSunpou);
	virtual void Serialize(CArchive& ar);
	bool	sxfSunpou(void) const { return m_bSxfMode != 0; }
	void	sxfSunpou(const bool flg) { m_bSxfMode = (flg ? TRUE : FALSE); }
	CDataSen*	getDimLine(void) { return &m_Sen; }
	CDataMoji*	getDimText(void) { return &m_Moji; }
	CDataSen*	getExt1Line(void) { return &m_SenHo1; }
	CDataSen*	getExt2Line(void) { return &m_SenHo2; }
	CDataTen*	getDim1Pos(void) { return &m_Ten1; }
	CDataTen*	getDim2Pos(void) { return &m_Ten2; }
	CDataTen*	getExt1Pos(void) { return &m_TenHo1; }
	CDataTen*	getExt2Pos(void) { return &m_TenHo2; }
	// �_���v
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//////////////////////////////////////////////////////////////////////
/// �\���b�h�f�[�^�N���X
//////////////////////////////////////////////////////////////////////
/// m_nPenStyle��101�ȏ�̏ꍇ�A�~�̃\���b�h�Ƃ��Ĉ���(�`�����ϊ�)
///
/// �~�\���b�h�̏ꍇ�̕ϐ��̒��g�͎��̒ʂ�
///
/// m_start : ���S
/// m_end.x : ���a
/// m_end.y : �G����
/// m_DPoint2.x : �X���p
/// m_DPoint2.y : �J�n�p
/// m_DPoint3.x : �~�ʊp
/// m_DPoint3.y : ��`�A�S�~���̎w��
///
/// �E�~�\���b�h
///   m_nPenStyle = 101     --- �~�\���b�h�̎w��
///     m_DPoint3.y = -1    --- �O���~�ʃ\���b�h
///     m_DPoint3.y = 0     --- ��`�\���b�h
///     m_DPoint3.y = 5     --- �|�`�\���b�h
///     m_DPoint3.y = 100   --- �S�~�\���b�h
///
/// �E�~�\���b�h
///   m_nPenStyle = 105     --- �~�\���b�h1�̎w��
///   m_nPenStyle = 106     --- �~�\���b�h2�̎w��
///     m_DPoint3.y = �����̉~���a
///  (�~�ʂƑS�~�̃t���O�ł̈Ⴂ�͖����B�~�ʊp�Ŕ��f)
///
/// �E�~���\���b�h
///   m_nPenStyle = 111     --- �~���\���b�h�̎w��
///     m_DPoint3.y = 0     --- �~��
///     m_DPoint3.y = 100   --- �S�~
//////////////////////////////////////////////////////////////////////

class EXTFUNC CDataSolid : public CData
{
protected:
	DPoint m_start;     // ��1�_���W
	DPoint m_end;       // ��4�_���W
	DPoint m_DPoint2;   // ��2�_���W
	DPoint m_DPoint3;   // ��3�_���W
	DWORD m_Color;      // �h��Ԃ��F

public:
	// �V���A���C�Y
	DECLARE_SERIAL(CDataSolid);
	virtual void Serialize(CArchive& ar);
	// �v���p�e�B�擾
	DWORD solidColor(void) { return m_Color; }
	OdGePoint3d	location(const int index) const;
	void location(const int index, const OdGePoint3d value);
	// �v���p�e�B�擾(�~/�ȉ~/�h�[�i�c/�~��)
	void	solidColor(DWORD rgb) { m_Color = rgb; }
	OdGePoint3d	center(void) const;
	double	radius(void) const;
	double	inner(void) const;
	double	flattenings(void) const;
	double	start(void) const;
	double	end(void) const;
	double	tilt(void) const;
	bool	full(void) const;
	bool	bow(void) const;
	bool	circSector(void) const;
	// �_���v
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
};

class CDataList;

//////////////////////////////////////////////////////////////////////
/// �u���b�N�f�[�^�N���X
/// DWG �ł��� INSERT
//////////////////////////////////////////////////////////////////////

class EXTFUNC CDataBlock : public CData
{
protected:
	DPoint m_DPKijunTen;	// �z�u��_
	double m_dBairitsuX;	// X�����{��
	double m_dBairitsuY;	// Y�����{��
	double m_radKaitenKaku;	// ��]�p
	CTypedPtrList<CObList, CDataList*>* m_pDataListList;
	//���X�g�ւ̃|�C���^
	DWORD m_nNumber;		// ��`�f�[�^�̒ʂ��ԍ�

public:
	// �V���A���C�Y
	DECLARE_SERIAL(CDataBlock);
	virtual void Serialize(CArchive& ar);

	// �f�[�^���X�g�擾
	CDataList* GetDataList();
	void init(CJwwDocument *pDoc);
	// �v���p�e�B
	DWORD	blockNo() const;
	OdGePoint3d	start() const;
	double	rotation() const;
	OdGeScale3d scale() const;
	void blockNo(DWORD	value);
	void start(OdGePoint3d	value);
	void rotation(double value);
	void scale(OdGeScale3d value);
	// �_���v
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//////////////////////////////////////////////////////////////////////
/// �f�[�^���X�g�N���X
/// DWG �ł��� BLOCK
// 2014/8/8 m_time �� DWORD�ł͂Ȃ�CTime
//////////////////////////////////////////////////////////////////////

class EXTFUNC CDataList : public CData
{
public:
	DWORD m_nNumber;        // �ʂ��ԍ�
	DWORD m_bReffered;      // ��Q�ƃt���O
	CTime m_time;           // ��`����
	CString m_strName;      // �u���b�N��`����
	CTypedPtrList<CObList, CData*> m_DataList;   // �u���b�N�Ɋ܂܂��}�`���X�g

public:
	// �V���A���C�Y
	DECLARE_SERIAL(CDataList);
	virtual void Serialize(CArchive& ar);
	// �v���p�e�B
	OdString name() const;
	// �_���v
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//////////////////////////////////////////////////////////////////////
/// JWW�t�@�C�� �C���|�[�g�h�L�������g�N���X

class EXTFUNC CJwwDocument : public CObject
{
public:
	/// JWW�t�@�C�� �w�b�_
	CJwwHeader	m_JwwHeader;
	/// �}�`�f�[�^�̃��X�g(ENTITIES SECTION ����)
	CTypedPtrList<CObList, CData*>	m_DataList;
	/// �u���b�N�f�[�^��`���̃��X�g(BLOCKS SECTION ����)
	CTypedPtrList<CObList, CDataList*>	m_DataListList;
	/// �摜�t�@�C�����̔z��(����=���ׂẲ摜�t�@�C�� ����=���ߍ��݉摜�t�@�C���̂�)
	std::set<CString>	m_JwwEmbbededImageFiles;
	/// JWW�}�ʃt�@�C���̃p�X(�C���[�W�̑��΃p�X�����߂�ۂɕK�v)
	CString	m_jwwPath;
	/// �Y�t�摜�o�͐�f�B���N�g��
	CString m_extructDir;
	/// ��w�W�J��}��(���|�[�g�o�͗p)
	BOOL m_bReadForReport;
	// 0:��ʐ}�`������ 1:�u���b�N�}�`������
	int m_iDepth;
	/// �J�X�^��INI�t�@�C�� "JDraf 2016.ini"�̏ꏊ
	CString m_iniPath;

public:
	/// �R���X�g���N�^
	CJwwDocument();
	/// �f�X�g���N�^
	virtual ~CJwwDocument();
	/// �V���A���C�Y
	DECLARE_SERIAL(CJwwDocument)
	virtual void Serialize(CArchive& ar);
	/// �v���p�e�B
	void setExtructDir(LPCTSTR dir) { m_extructDir = dir; }
	/// �J�X�^��INI�t�@�C�� "JDraf 2016.ini"�̏ꏊ
	void setIniPath(LPCTSTR path) { m_iniPath = path; }

protected:
	//virtual BOOL OnNewDocument();
	//DECLARE_MESSAGE_MAP()

protected:
	//void WriteImageFiles(CArchive& ar);	      // �摜�t�@�C�����ߍ���
	//void ExtructImageFiles(CArchive& ar);	  // �摜�t�@�C���W�J
	/// �S�f�[�^����
	void ClearData();

	// �ϊ�
public:
	// �k�ڂƕψʂ̓K�p�Ə�����
	void DoDataScale(double &dValue);
	void DoDataScale(struct DPoint & pValue);
	void SetDataScale(const double dValue, const struct DPoint & pIn);
	// �k�ڂƕψʂ��擾
	double GetDataScale();
	void GetDataOffset(struct DPoint & pOut);
	// RAY,XLINE�̏o�͔͈͂̌v�Z�Ɏg��
	void SetMinMaxPt(const OdGePoint3d& min, const OdGePoint3d& max);
	void GetMinMaxPt(OdGePoint3d& min, OdGePoint3d& max) const;
	// ���͗p�}�ʔ͈�
	void GetDocumentArea(OdGePoint2d& min, OdGePoint2d& max) const;
	// ��}�`�̐�
	INT_PTR	GetDataCount() const;
	// ��}�`�̐擪
	POSITION GetHeadPosition() const;
	// ��}�`�̃f�[�^���X�g
	CTypedPtrList<CObList, CData*>& GetDataList() { return m_DataList; }
	// �u���b�N�}�`�̃f�[�^���X�g
	CTypedPtrList<CObList, CDataList*>* GetBlockList() { return &m_DataListList; }
	// �o�[�W�������擾
	DWORD GetVersion() const;
	// �u���b�N�[����ݒ�
	void SetDepth(const int depth) { m_iDepth = depth; }
	// �u���b�N�[�����擾
	int GetDepth(void) const { return m_iDepth; }
	// JWW�W�J�p�p�X��ݒ�
	void SetJwwPath(const CString& path) { m_jwwPath = path; }
	// JWW�W�J�p�p�X���擾
	CString GetJwwPath() { return m_jwwPath; }

	// �ϊ�
public:
	/// �p���T�C�Y��`
	static const double YOUSHI_SIZE[15];
	static const double YOUSHI_TATE[15];
	/// �W���I�Ȑ}�ʂ̏k��
	static const double HYOJUN_SHAKUDO[8];
	/// JWCAD DXF�o�͐��햼
	static const char*	JWCADDXF_LTYPES[48];
	// ATTN:��w���̕t���� (0 �ԍ��Ɩ��O 1 �ԍ��̂�)
	BOOL	m_bLayerName;
	/// ATTN:����ϊ��W��
	double m_lineFactor;
	/// ATTN:���C���ԍ�����v������V������w�����Ȃ�(0=����/1=���Ȃ�)
	BOOL m_gMatchLayerNumber;

	/// �_���v
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
