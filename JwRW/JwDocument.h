//--------------------------------------------------------------------
// JWファイルドキュメントクラス
//	JWW,JWC,JWS ドキュメント クラスの親クラスとする
//
//	JWWとJWSは色、レイヤ、線種取得関数をオーバーライドする
//	JWCは色、レイヤ、線種取得関数を使わない
//
//  2017/3/6 JWSファイルのブロックに対応
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
	// 縮尺と変位の適用と初期化
	virtual void DoDataScale(double &dValue) = 0;
	virtual void DoDataScale(struct DPoint & pValue) = 0;
	virtual void SetDataScale(const double dValue, const struct DPoint & pIn) = 0;
	virtual double GetDataScale() = 0;
	// RAY,XLINEの出力範囲の計算に使う
	virtual void SetMinMaxPt(const OdGePoint3d& min, const OdGePoint3d& max) = 0;
	virtual void GetMinMaxPt(OdGePoint3d& min, OdGePoint3d& max) const = 0;
	// 入力用図面範囲
	virtual void GetDocumentArea(OdGePoint2d& min, OdGePoint2d& max) const = 0;
	// ブロック深さを設定
	virtual void SetDepth(const int depth) = 0;
	// ブロック深さを取得
	virtual int GetDepth(void) const = 0;
	// JWW展開用パスを設定
	virtual void SetJwwPath(const CString& path) = 0;
	// JWW展開用パスを取得
	virtual CString GetJwwPath() = 0;
	// 主図形の数
	virtual INT_PTR	GetDataCount() const = 0;
#ifdef linux
	// 主図形の先頭
	virtual POSITION GetHeadPosition() const = 0;
	// 主図形のデータリスト
	virtual std::list<CData*>& GetDataList() = 0;
	// ブロック図形のデータリスト
	virtual std::list<CDataList*> *GetBlockList() = 0;
#else	
	// 主図形の先頭
	virtual POSITION GetHeadPosition() const = 0;
	// 主図形のデータリスト
	virtual CTypedPtrList<CObList, CData*>& GetDataList() = 0;
	// ブロック図形のデータリスト
	virtual CTypedPtrList<CObList, CDataList*> *GetBlockList() = 0;
#endif	
	// バージョンを取得
	virtual DWORD GetVersion() const = 0;
	// ダンプ
#ifdef _DEBUG
	virtual void AssertValid() const = 0;
	virtual void Dump(CDumpContext& dc) const = 0;
#endif
};
