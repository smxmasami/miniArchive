//--------------------------------------------------------------------
// JW�t�@�C���h�L�������g�N���X
//	JWW,JWC,JWS �h�L�������g �N���X�̐e�N���X�Ƃ���
//
//	JWW��JWS�͐F�A���C���A����擾�֐����I�[�o�[���C�h����
//	JWC�͐F�A���C���A����擾�֐����g��Ȃ�
//
//  2017/3/6 JWS�t�@�C���̃u���b�N�ɑΉ�
//--------------------------------------------------------------------
#pragma once

struct DPoint;
class CData;
class CDataList;
class OdString;
class OdGePoint3d;
class OdGePoint2d;

class EXTFUNC CJwDocument : public CDocument
{
public:
	CJwDocument() {};
	// �k�ڂƕψʂ̓K�p�Ə�����
	virtual void DoDataScale(double &dValue) = 0;
	virtual void DoDataScale(struct DPoint & pValue) = 0;
	virtual void SetDataScale(const double dValue, const struct DPoint & pIn) = 0;
	virtual double GetDataScale() = 0;
	// RAY,XLINE�̏o�͔͈͂̌v�Z�Ɏg��
	virtual void SetMinMaxPt(const OdGePoint3d& min, const OdGePoint3d& max) = 0;
	virtual void GetMinMaxPt(OdGePoint3d& min, OdGePoint3d& max) const = 0;
	// ���͗p�}�ʔ͈�
	virtual void GetDocumentArea(OdGePoint2d& min, OdGePoint2d& max) const = 0;
	// �u���b�N�[����ݒ�
	virtual void SetDepth(const int depth) = 0;
	// �u���b�N�[�����擾
	virtual int GetDepth(void) const = 0;
	// JWW�W�J�p�p�X��ݒ�
	virtual void SetJwwPath(const CString& path) = 0;
	// JWW�W�J�p�p�X���擾
	virtual CString GetJwwPath() = 0;
	// ��}�`�̐�
	virtual INT_PTR	GetDataCount() const = 0;
	// ��}�`�̐擪
	virtual POSITION GetHeadPosition() const = 0;
	// ��}�`�̃f�[�^���X�g
	virtual CTypedPtrList<CObList, CData*>& GetDataList() = 0;
	// �u���b�N�}�`�̃f�[�^���X�g
	virtual CTypedPtrList<CObList, CDataList*> *GetBlockList() = 0;
	// �o�[�W�������擾
	virtual DWORD GetVersion() const = 0;
	// �_���v
#ifdef _DEBUG
	virtual void AssertValid() const = 0;
	virtual void Dump(CDumpContext& dc) const = 0;
#endif
};
