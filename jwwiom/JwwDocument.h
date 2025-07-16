//////////////////////////////////////////////////////////////////////
// Jwwデータ読み込みクラス群 定義ファイル
//
// ・CDocument派生のCJwwDocumentクラス
//   JWWデータ全体を管理するクラス
//
// ・CObject派生のCJwwHeaderクラス
//   JWWのヘッダ情報を保持するクラス
//
// ・CObject派生のCDataクラス
//   JWWの個々のデータを保持する各クラスのベース
//
//  2017/3/6 JWSファイルのブロックに対応

#pragma once

/// SXFユーザ定義線種の最大要素数
#define	USERDEFINEDLTYPEPITCHES	10

/// SXFブロック種類
///
///	"ブロック名@@SfigorgFlag@@1"  部分図(数学座標系)
///	"ブロック名@@SfigorgFlag@@2"  部分図(測地座標系)
///	"ブロック名@@SfigorgFlag@@3"	複合曲線(ハッチングのパス)
///	"ブロック名@@SfigorgFlag@@4"	部品定義
///
///	JW_CADでJWW保存すると、ブロック名に "@@SfigorgFlag@@4" をつけて返す
///	ブロック名の最後が、@@SfigorgFlag@@1 なら、そのままである。
///
#define	SXF_BLOCK_PREFIX	_T("@@Sfigorgflag@@")
#define	SXF_BLOCK_PARTS_ID	_T("@@Sfigorgflag@@4")

/// JWW バージョン ID
/// 2015-01-15	Jw_cad 8.00 のファイルバージョンは700です
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
/// 構造体定義
struct EXTFUNC DPoint
{
	double x;
	double y;
};

// ODAライブラリ
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

bool OdEqual(double x, double y, double z = 1e-5)
{
	return std::abs(x - y) < z;
}

class CJwwDocument;
//////////////////////////////////////////////////////////////////////
/// JWWファイル ヘッダ部読み込みクラス

class EXTFUNC CJwwHeader : public CObject
{
public:
	/// JWW図面に保存される値 ---
	/// JWWのデータファイル宣言(データ 0-7)
	char m_cVersion[8];
	/// バージョンNo.
	DWORD m_Version;
	/// ファイルメモ
	CString m_strMemo;
	/// 図面サイズ
	DWORD m_nZumen;
	/// 画層グループ・画層状態
	/// 書き込み画層グループ番号
	DWORD m_nWriteGLay;
	/// 画層グループの状態(データ 0-15)
	DWORD m_anGLay[16];
	/// グループ別書き込み画層番号(データ 0-15)
	DWORD m_anWriteLay[16];
	/// グループ別縮尺(データ 0-15)
	double m_adScale[16];
	/// グループ別プロテクト状態(データ 0-15)
	DWORD m_anGLayProtect[16];
	/// 画層の状態(データ 0-15)(データ 0-15)
	DWORD m_aanLay[16][16];
	/// 画層のプロテクト状態(データ 0-15)(データ 0-15)
	DWORD m_aanLayProtect[16][16];
	/// 寸法関係の設定
	/// 一の位 寸法線の色(1～9)
	/// 十の位 引出線の色(1～9)
	/// 百の位 寸法点の色(1～9)
	/// 千の位 小数点以下の桁数(0～3)
	/// 万の位 0:mm 1:m
	/// 十万の位 0:点 1:矢印 2:逆矢印
	/// 百万の位 寸法文字種(1～10)
	DWORD m_lnSunpou1;
	/// この値がマイナスの場合は+1000して整数化する
	/// 10000の剰余 寸法値と寸法線の離れ(000-999→0mm～99.9mm)
	/// 10000の商  寸法線の突出長さ(000-999→0mm～99.9mm)
	DWORD m_lnSunpou2;
	/// 1000の剰余 矢印長さ(000-999→0mm～99.9mm)
	/// 10万の剰余 矢印角度(001-080→0.1°～80.0°)
	/// 100万の商  逆矢印の出寸法(000-999→0mm～99.9mm)
	DWORD m_lnSunpou3;
	/// 一の位 寸法値方向を補正しない 0:補正有 1:補正無
	/// 十の位 寸法値を全角にする 0:半角 1:全角 (無視するか別のフォントを使うか)
	/// 百の位 寸法値のカンマをスペースにする 0:カンマ 1:スペース (無視)
	/// 千の位 寸法値のカンマを全角にする 0:半角 1:全角 (無視)
	/// 万の位 寸法値の小数点を全角にする 0:半角 1:全角 (無視)
	/// 十万の位 寸法値に単位を表示する 0:無 1:有
	/// 百万の位 半径値にRを表示する 0:無 1:前 2:後 (要調査)
	/// 千万の位 半径値にカンマを表示する 0:無 1:有
	/// 億の位 半径値に小数点の0を表示 0:無 1:有
	DWORD m_lnSunpou4;
	/// 一の位 寸法値を斜体にする 0:通常 1:斜体
	/// 十の位 寸法値を太字にする 0:通常 1:太字
	/// 百の位 0:十進度 1:度分秒 2:度単位で°の表示なし
	/// 千の位 角度寸法の小数点以下の桁数 0～6
	/// 万の位 寸法図形にする 0:分解 1:寸法にする (無視)
	/// 十万の位 範囲選択のとき、寸法図形を線色、線属性で選択 0 しない 1 する(無視)
	/// 百万の位 表示小数点以下の処理 0:四捨五入 1:切り捨て 2:切り上げ
	DWORD m_lnSunpou5;
	/// 線描画の最大幅
	DWORD m_nMaxDrawWid;
	/// プリンタ出力範囲の原点(X,Y)
	DPoint m_DPPrtGenten;
	/// プリンタ出力倍率
	double m_dPrtBairitsu;
	/// プリンタ90°回転出力、プリンタ出力基準点位置
	DWORD m_nPrtSet;
	/// 目盛設定モード
	/// 一の位 0 指定なし 1 指定あり
	/// 十の位 0 図寸 1 実寸
	/// マイナスのとき目盛り読み取り不可(=SNAP OFF)
	DWORD m_nMemoriMode;
	/// 目盛表示最小間隔ドット (無視)
	double m_dMemoriHyoujiMin;
	/// 目盛表示間隔(X,Y)
	/// GRIDSIZEやSNAPSIZE になる
	double m_dMemoriX;
	double m_dMemoriY;
	/// 目盛基準点(X,Y)
	/// GRIDBASE/SNAPBASE になる
	DPoint m_DpMemoriKijunTen;
	/// 画層名
	/// DWGで画層名に使用できない文字が含まれることがあるので注意(データ 0-15)(データ 0-15)
	CString m_aStrLayName[16][16];
	/// 画層グループ名
	/// DWGで画層名に使用できない文字が含まれることがあるので注意(データ 0-15)
	CString m_aStrGLayName[16];
	/// 日影計算の条件
	double m_dKageLevel;
	double m_dKageIdo;
	DWORD m_nKage9_15JiFlg;
	double m_dKabeKageLevel;
	/// 天空図の条件（Ver.3.00以降)
	double m_dTenkuuZuLevel;
	double m_dTenkuuZuEnkoR;
	/// 2.5Dの計算単位(0以外はmm単位で計算)
	DWORD m_nMMTani3D;
	/// 保存時の画面倍率(読込むと前画面倍率になる)
	double m_dBairitsu;
	DPoint m_DPGenten;
	/// 範囲記憶倍率と基準点(X,Y)
	double m_dHanniBairitsu;
	DPoint m_DPHanniGenten;
	/// マークジャンプ倍率(データ 1～8)
	double m_dZoomJumpBairitsu[9];
	/// マークジャンプ基準点(X,Y)(データ 1～8)
	DPoint m_DPZoomJumpGenten[9];
	/// マークジャンプ倍率 有効画層グループ(データ 1～8)
	DWORD m_nZoomJumpGLay[9];
	/// 文字の描画状態(Ver.4.05以降）
	double m_dDm11;		//ダミー
	double m_dDm12;	//ダミー
	double m_dDm13;		//ダミー
	DWORD m_lnDm1;		//ダミー
	double m_dDm21;		//ダミー
	double m_dDm22;		//ダミー
	double m_dMojiBG;	//(Ver.4.04以前はダミー）
	DWORD m_nMojiBG;	//(Ver.4.04以前はダミー）
	/// 複線間隔(データ 0～9)
	double m_adFukusenSuuchi[10];
	/// 両側複線の留線出の寸法
	double m_dRyoygawaFukusenTomeDe;
	/// 色番号ごとの画面表示色(データ 0-9,100-356)
	DWORD m_aPenColor[357];
	/// 色番号ごとの画面表示線幅(データ 0-9,100-356)
	DWORD m_anPenWidth[357];
	/// 色番号ごとのSXF色名(データ 0-256)
	CString m_astrUDColorName[257];
	/// 色番号ごとのプリンタ出力色、線幅、実点半径(データ 0-9,100-356)
	DWORD m_aPrtPenColor[357];
	DWORD m_anPrtPenWidth[357];
	double m_adPrtTenHankei[357];
	/// 線種番号2から9までのパターン、1ユニットのドット数、ピッチ、プリンタ出力ピッチ(データ2-9,11-15,16-19,30-62)
	/// 倍長線種番号6から9までのパターン、1ユニットのドット数、ピッチ、プリンタ出力ピッチ(データ2-9,11-15,16-19,30-62)
	/// SXF対応線種33個分のパターン、1ユニットのドット数、ピッチ、プリンタ出力ピッチ(データ2-9,11-15,16-19,30-62)
	DWORD m_alLType[63];
	DWORD m_anTokushuSenUintDot[63];
	DWORD m_anTokushuSenPich[63];
	DWORD m_anPrtTokushuSenPich[63];
	/// ランダム線1から5までのパターン、画面表示振幅・ピッチ、プリンタ出力振幅・ピッチ(データ11-15)
	DWORD m_anRandSenWide[16];
	DWORD m_anPrtRandSenWide[16];
	/// 実点を画面描画時の指定半径で描画
	DWORD m_nDrawGamenTen;
	/// 実点をプリンタ出力時、指定半径で書く
	DWORD m_nDrawPrtTen;
	/// BitMap・ソリッドを最初に描画する
	DWORD m_nBitMapFirstDraw;
	/// 逆描画
	DWORD m_nGyakuDraw;
	/// 逆サーチ
	DWORD m_nGyakuSearch;
	/// カラー印刷
	DWORD m_nColorPrint;
	/// 画層順の印刷
	DWORD m_nLayJunPrint;
	/// 色番号順の印刷
	DWORD m_nColJunPrint;
	/// 画層グループまたは画層ごとのプリンタ連続出力指定
	DWORD m_nPrtRenzoku;
	/// プリンタ共通画層(表示のみ画層)のグレー出力指定
	DWORD m_nPrtKyoutsuuGray;
	/// プリンタ出力時に表示のみ画層は出力しない
	DWORD m_nPrtDispOnlyNonDraw;
	/// 作図時間（Ver.2.23以降）
	DWORD m_lnDrawTime;
	/// 2.5Dの始点位置が設定されている時のフラグ（Ver.2.23以降）
	DWORD m_nEyeInit;
	/// 2.5Dの透視図・鳥瞰図・アイソメ図の視点水平角（Ver.2.23以降）
	DWORD m_dEye_H_Ichi_1;
	DWORD m_dEye_H_Ichi_2;
	DWORD m_dEye_H_Ichi_3;
	/// 2.5Dの透視図の視点高さ・視点離れ（Ver.2.23以降）
	double m_dEye_Z_Ichi_1;
	double m_dEye_Y_Ichi_1;
	/// 2.5Dの鳥瞰図の視点高さ・視点離れ（Ver.2.23以降）
	double m_dEye_Z_Ichi_2;
	double m_dEye_Y_Ichi_2;
	/// 2.5Dのアイソメ図の視点垂直角（Ver.2.23以降）
	double m_dEye_V_Ichi_3;
	/// 線の長さ指定の最終値（Ver.2.25以降）
	double m_dSenNagasaSnpou;
	/// 矩形寸法横寸法・縦寸法指定の最終値（Ver.2.25以降）
	double m_dBoxSunpouX;
	double m_dBoxSunpouY;
	/// 円の半径指定の最終値（Ver.2.25以降）
	double m_dEnHankeiSnpou;
	/// ソリッドを任意色で書くフラグ、ソリッドの任意色の既定値（Ver.2.30以降）
	DWORD m_nSolidNinniColor;
	DWORD m_SolidColor;
	/// SXF対応拡張線色定義（Ver.4.20以降）(データ 0-31)
	/// SXF対応拡張線種定義（Ver.4.20以降）(データ 0-31)
	/// SXF対応拡張線種ピッチ（Ver.4.20以降）(データ 0-31)(データ 1-10)
	CString m_astrUDLTypeName[33];
	DWORD m_anUDLTypeSegment[33];
	double m_aadUDLTypePitch[33][11];
	/// 文字種1から10までの文字幅(データ 1-10)
	double m_adMojiX[11];
	/// 文字種1から10までの文字高さ(データ 1-10)
	double m_adMojiY[11];
	/// 文字種1から10までの文字間隔(データ 1-10)
	double m_adMojiD[11];
	/// 文字種1から10までの色番号(データ 1-10)
	DWORD m_anMojiCol[11];
	/// 書込み文字の文字幅、高さ、間隔、色番号、文字番号
	double m_dMojiSizeX;
	double m_dMojiSizeY;
	double m_dMojiKankaku;
	DWORD m_nMojiColor;
	DWORD m_nMojiShu;
	/// 文字位置整理の行間、文字数
	double m_dMojiSeiriGyouKan;
	double m_dMojiSeiriSuu;
	/// 文字基準点のずれ位置使用のフラグ
	DWORD m_nMojiKijunZureOn;
	/// 文字基準点の横方向のずれ位置左、中、右(データ 0-2)
	double m_adMojiKijunZureX[3];
	/// 文字基準点の縦方向のずれ位置下、中、上(データ 0-2)
	double m_adMojiKijunZureY[3];
	/// ドキュメントへのポインタ
	CJwwDocument* m_pDoc;
	/// JWW図面に保存される値
private:
	/// 図寸⇔座標 変換係数
	double m_dJwwScale;
	/// A4～5A 図面の左下済みを(0,0)にするオフセット値
	DPoint m_pJwwOffset;
	/// 出力範囲
	OdGePoint3d m_pJwwMinPt;
	OdGePoint3d m_pJwwMaxPt;
	/// 文字角度補正設定(0:しない 1:する)
	BOOL m_bFixAng;
	/// 文字角度補正設定の角度の許容値
	double m_tFixAng;

public:
	/// コンストラクタ
	CJwwHeader();
	/// デストラクタ
	virtual ~CJwwHeader();
	/// シリアライズ
	DECLARE_SERIAL(CJwwHeader);
	virtual void Serialize(CArchive& ar);

	/// 画層名取得
	CString  GetLayerName(
		int iGLayer,      // [I] 画層グループ番号
		int iLayer);      // [I] 画層番号

	/// 縮尺と変位の適用と初期化
	void DoDataScale(double &dValue);
	void DoDataScale(struct DPoint & pValue);
	void SetDataScale(const double dValue, const struct DPoint & pIn);
	double GetDataScale();
	void GetDataOffset(struct DPoint & pOut);
	/// RAY,XLINEの出力範囲の計算に使う
	void SetMinMaxPt(const OdGePoint3d& min, const OdGePoint3d& max);
	void GetMinMaxPt(OdGePoint3d& min, OdGePoint3d& max) const;
	/// 入力用図面範囲
	void GetDocumentArea(OdGePoint2d& min, OdGePoint2d& max) const;
	/// 文字角度補正設定を取得
	void SetTextOrthoTol();
	BOOL GetTextOrthoTol(double& dTol);

	/// ダンプ
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//////////////////////////////////////////////////////////////////////
/// JWW図形基本クラス

class EXTFUNC CData : public CObject
{
protected:
	DWORD	m_lGroup;		// 曲線属性番号
	BYTE	m_nPenStyle;	// 線種番号
	WORD	m_nPenColor;	// 線色番号
	WORD	m_nPenWidth;	// 線色幅
	WORD	m_nLayer;		// 画層番号
	WORD	m_nGLayer;		// 画層グループ番号
	WORD	m_sFlg;			// 属性フラグ

	CJwwHeader *m_pHeader;	// ヘッダ情報へのポインタ

public:
	// コンストラクタ
	CData();
	CData(const CData & src);
	// デストラクタ
	virtual ~CData();
	// シリアライズ
	DECLARE_SERIAL(CData);
	virtual void Serialize(CArchive& ar);
	CData& operator=(const CData src);
	// JWW,JWSのバージョンを取得する
	DWORD	version(CArchive& ar);
	void	DoDataScale(CArchive& ar, double &val);
	void	DoDataScale(CArchive& ar, DPoint &pnt);
	// ダンプ
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 線種コード (1～64,65)
	enum ePenStyle
	{
		eObjectLayer = 65	// ブロック要素の画層を優先する
	};

	// 属性フラグ
	enum eZukeiFlag
	{
		eGeneric = 0x0000,
		eZukei = 0x0010,	//円・点図形
		eHatch = 0x0020,	//線・円・点・ソリッドのハッチング
		eSunpou = 0x0040,	//寸法の円・点
		eTategu = 0x0080,	//建具の円・点
		eSen = 0x0800,	//線図形
		eTateguSen = 0x1000,	//建具の線
		eSunpouSen = 0x2000,	//寸法線
		eHorakuGai = 0x8000,	//包絡処理対象外の建具の線
		eTate = 0x0020,	//縦文字
		eMakita = 0x0040,	//真北
		eHikage = 0x0080,	//日影
		eHankei = 0x0100,	//半径寸法値
		eChokkei = 0x0200,	//直径寸法値
		eKakudo = 0x0400,	//角度寸法値
		eZokusei = 0x0800,	//図形属性選択（文字）
		eRuikei = 0x1000,	//累計寸法値
		eTateguMoji = 0x2000,	//建具の文字
		eNagasa = 0x4000,	//長さ寸法値
		eTakasa = 0x8000,	//2.5D高さ値
		eSunpouObj = 0x2000	//寸法図形寸法フラグ
	};

	// プロパティ取得・設定
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
	// ヘッダーへのポインタを設定
	void	header(CJwwHeader* pH) { m_pHeader = pH; }
};

//////////////////////////////////////////////////////////////////////
/// 線データクラス

class EXTFUNC CDataSen : public CData
{
protected:
	DPoint m_start;       // 始点
	DPoint m_end;         // 終点

public:
	// シリアライズ
	DECLARE_SERIAL(CDataSen);
	virtual void Serialize(CArchive& ar);
	// プロパティ取得
	OdGePoint3d	start(void) const;
	OdGePoint3d	end(void) const;
	void	start(const OdGePoint3d& point);
	void	end(const OdGePoint3d& point);

	// ダンプ
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//////////////////////////////////////////////////////////////////////
/// 円弧データクラス

class EXTFUNC CDataEnko : public CData
{
protected:
	DPoint m_start;             // 中心点
	double m_dHankei;           // 半径
	double m_radKaishiKaku;     // 開始角
	double m_radEnkoKaku;       // 円弧角
	double m_radKatamukiKaku;   // 傾き角
	double m_dHenpeiRitsu;      // 扁平率
	DWORD m_bZenEnFlg;          // 全円フラグ

public:
	// シリアライズ
	DECLARE_SERIAL(CDataEnko);
	virtual void Serialize(CArchive& ar);
	// プロパティ取得
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

	// ダンプ
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//////////////////////////////////////////////////////////////////////
/// 点データクラス

class EXTFUNC CDataTen : public CData
{
protected:
	DPoint m_start;           // 点座標
	DWORD m_bKariten;         // 仮点フラグ
	DWORD m_nCode;            // 点コード
	double m_radKaitenKaku;   // 回転角
	double m_dBairitsu;       // 倍率

public:
	// シリアライズ
	DECLARE_SERIAL(CDataTen);
	virtual void Serialize(CArchive& ar);
	// プロパティ取得
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

	// ダンプ
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//////////////////////////////////////////////////////////////////////
///
/// 文字データクラス
///
/// ----------------------------------------------------------
/// 印刷文字一式 DWGでいうとFIELDオブジェクト
/// ----------------------------------------------------------
/// &F			ファイル名(完全パス)
/// &f			ファイル名(拡張子なし)
/// %f			ファイル名(拡張子あり) %f1～%ff バイト数
/// %SS			縮尺 (1/200 表記)
/// %ss			縮尺 (1/200 は 200, 5:1 は 5 と表示)
/// %SP %sp		プリンタ出力時の出力倍率補正あり
/// %mm			図面のコメント
/// %m1			図面のコメント１行目
/// %m2			図面のコメント２行目
/// =F			図面の保存日時(日付と時刻）
/// =f			図面の保存日時(日付のみ)
/// =y			図面の保存日時(西暦下２桁)
/// =Y			図面の保存日時(元号)
/// =m			図面の保存日時(月)
/// =ma			図面の保存日時(英語 JAN,FEB,...)
/// =d			図面の保存日時(日)
/// =w			図面の保存日時(曜日)
/// =wa			図面の保存日時(英語 SUN,MON,...)
/// =h			図面の保存日時(時 12時間制)
/// =H			図面の保存日時(時 24時間制)
/// =M			図面の保存日時(分)
/// =S			図面の保存日時(秒)
/// =n			図面の保存日時(英 AM,PM)
/// =N			図面の保存日時(   前,後)
/// %y			現在の日時(西暦下２桁)
/// &Y			現在の日時(元号)
/// &m			現在の日時(月)
/// &ma			現在の日時(英語 JAN,FEB,...)
/// &d			現在の日時(日)
/// &w			現在の日時(曜日)
/// &wa			現在の日時(英語 SUN,MON,...)
/// &h			現在の日時(時 12時間制)
/// &H			現在の日時(時 24時間制)
/// &M			現在の日時(分)
/// &S			現在の日時(秒)
/// &n			現在の日時(英 AM,PM)
/// &N			現在の日時(   前,後)
/// &T			作図時間
/// ----------------------------------------------------------
/// 制御文字一式 DWGでいうとTEXT,MTEXTの書式コード
/// ----------------------------------------------------------
/// ^!			文字(意味不明)
/// ^/           斜体開始
/// ^_           下線開始
/// ^-           取消線開始
/// ^#           斜体,下線,取消線の解除
/// ^$1 ～ ^$9   色変更(色コード1～9に相当する色)
/// ^\"          フォントをＭＳゴシックにする
/// ^&           フォントを元に戻す
/// ^%           文字種の設定に戻す
/// ^*           ^* ～ ^^ までの制御文字は無効でそのまま表示
/// ^^			ここまで制御文字無効
/// ----------------------------------------------------------
/// 特殊文字一式 DWGでいうとTEXT,MTEXTの書式コード
/// ----------------------------------------------------------
/// ^u           次の文字を上付き文字にする(文字高さ半分)
/// ^d           次の文字を下付き文字にする(文字高さ半分)
/// ^c           次の文字を中付き文字にする(文字高さ半分)
/// ^o           次の文字を中心重ね文字にする(文字高さ3/4)
/// ^w           半角２字を中心重ね文字にする ○^w99 で丸数字99
/// ^b ^B ^n     重ね文字(文字送り 半分,1/4,ゼロ)
/// ^1 ～ ^9		行末に設定し、nバイト分の文字幅を圧縮する
/// ・・・		均等割り付け(全角) 行末に指定する
/// ・・・･		均等割り付け(半角) 行末に指定する
///	行末に・を書いて均等割り付けにしたくないときは空白や全角空白を書く
///
//////////////////////////////////////////////////////////////////////
/// 画像描画
///
/// JW-CADの画像は文字図形の文字列に画像のパラメータを記述する
/// 文字列の最初の4文字が「^@BM」の場合、画像のパラメータである
/// CJwwDocument::CreateImageDictionary() によって画像は図面に登録されているので
/// 画像定義のディクショナリから画像定義(IMAGEDEF)を取得して
/// 画像図形(IMAGE)と画像定義のリアクタ(IMAGEDEF_REACTOR)を作る流れになる
///
/// パラメータの例:
///   ^@BMC:\abc.bmp,100,100 ←絶対パス (C:\abc.bmpをサイズ100,100で表示)
///   ^@BMabc.bmp,100,100    ←相対パス (図面ファイルと同じフォルダにあるabc.bmpを表示)
///   ^@BM\abc.bmp,100,100   ←ドライブ無指定絶対パス (図面ファイルと同じドライブにある\abc.bmpを表示)
/// クリッピングと画像の回転:
///   ^@BMabc.bmp,100,100,0.25,0.25,0.5,30 ← 画像ファイルの左下1/4より幅1/2を表示し、30度回転
///   ^@BMabc.bmp,100,100,0,0,1,30 ← クリッピングなし、30度回転のみ
/// 画像ファイルの埋め込み(Ver7.00～):
///   ^@BMP%temp%abc.bmp,100,100 パス名が %temp% になっているものは埋め込みである
///   DWGPREFIX のディレクトリに CJwwDocument::ExtructImageFiles()で展開済み
/// Susieプラグライン:
///	 Susieプラグインを使うとJW-CADはさまざまな画像フォーマットを扱えるようになる
///	 ICAD(ACAD)で利用できない画像ファイルは、CreateImageDictionary()で除外する
///	 利用できる画像フォーマットの拡張子:
///		.bmp, .dib, .rle, .gif, .jpg, .jpeg, .png, .tif, .tiff
/// 補足:
///   カンマがパラメータの区切りになっているので、画像ファイル名にカンマは含まれない
//////////////////////////////////////////////////////////////////////
/// 寸法フラグ
///
/// 寸法フラグはWORDでやり取りする必要があったので修正
//////////////////////////////////////////////////////////////////////

class EXTFUNC CDataMoji : public CData
{
protected:
	WORD m_nSunpouFlg;      // 寸法フラグ
	DPoint m_start;         // 始点座標
	DPoint m_end;           // 終点座標
	DWORD m_nMojiShu;       // 文字種
	double m_dSizeX;        // 文字サイズ横
	double m_dSizeY;        // 文字サイズ縦
	double m_dKankaku;      // 文字間隔
	double m_degKakudo;     // 回転角
	CString m_strFontName;  // フォント名
	CString m_string;       // 文字列

public:
	// シリアライズ
	DECLARE_SERIAL(CDataMoji);
	virtual void Serialize(CArchive& ar);

	// プロパティ取得
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

	// ダンプ
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
	// 文字種フラグ
	enum	eMojishu {
		eMask = 10000,	//マスク値
		eShatai = 10000,	//斜体
		eFutoji = 20000		//太字
	};
	enum eSunpou
	{
		eUnDoc = 0x0001,		// 仕様にない値
		eMoji = 0x0002,			// 寸法の文字設定あり
		eZenkaku = 0x0008,		// 全角
		eTani = 0x0010,			// 単位
		eTaniTsuika = 0x0020,	// 単位追加
		eComma = 0x0040,		// カンマ
		eZero = 0x0080,			// 小数点以下０表示する
		eKirisute = 0x0100,		// 切り捨て
		eKiriage = 0x0200,		// 切り上げ
		eMaeKigo = 0x0400,		// 半径,直径記号を前につける
		eUshiroKigo = 0x0800,	// 半径,直径記号を後ろにつける
		eDigit1 = 0x1000,		// 小数点１桁
		eDigit2 = 0x2000,		// 小数点２桁
		eDigit3 = 0x3000,		// 小数点３桁
	};
};

//////////////////////////////////////////////////////////////////////
/// 寸法データクラス

class EXTFUNC CDataSunpou : public CData
{
protected:
	CDataSen m_Sen;       // 線データ
	CDataMoji m_Moji;     // 文字データ

	WORD m_bSxfMode;      // SXFモードフラグ
	CDataSen m_SenHo1;    //
	CDataSen m_SenHo2;    //
	CDataTen m_Ten1;      //
	CDataTen m_Ten2;      //
	CDataTen m_TenHo1;    //
	CDataTen m_TenHo2;    //

public:
	// シリアライズ
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
	// ダンプ
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//////////////////////////////////////////////////////////////////////
/// ソリッドデータクラス
//////////////////////////////////////////////////////////////////////
/// m_nPenStyleが101以上の場合、円のソリッドとして扱う(形だけ変換)
///
/// 円ソリッドの場合の変数の中身は次の通り
///
/// m_start : 中心
/// m_end.x : 半径
/// m_end.y : 扁平率
/// m_DPoint2.x : 傾き角
/// m_DPoint2.y : 開始角
/// m_DPoint3.x : 円弧角
/// m_DPoint3.y : 扇形、全円等の指定
///
/// ・円ソリッド
///   m_nPenStyle = 101     --- 円ソリッドの指定
///     m_DPoint3.y = -1    --- 外側円弧ソリッド
///     m_DPoint3.y = 0     --- 扇形ソリッド
///     m_DPoint3.y = 5     --- 弓形ソリッド
///     m_DPoint3.y = 100   --- 全円ソリッド
///
/// ・円環ソリッド
///   m_nPenStyle = 105     --- 円環ソリッド1の指定
///   m_nPenStyle = 106     --- 円環ソリッド2の指定
///     m_DPoint3.y = 内側の円半径
///  (円弧と全円のフラグでの違いは無し。円弧角で判断)
///
/// ・円周ソリッド
///   m_nPenStyle = 111     --- 円周ソリッドの指定
///     m_DPoint3.y = 0     --- 円弧
///     m_DPoint3.y = 100   --- 全円
//////////////////////////////////////////////////////////////////////

class EXTFUNC CDataSolid : public CData
{
protected:
	DPoint m_start;     // 第1点座標
	DPoint m_end;       // 第4点座標
	DPoint m_DPoint2;   // 第2点座標
	DPoint m_DPoint3;   // 第3点座標
	DWORD m_Color;      // 塗りつぶし色

public:
	// シリアライズ
	DECLARE_SERIAL(CDataSolid);
	virtual void Serialize(CArchive& ar);
	// プロパティ取得
	DWORD solidColor(void) { return m_Color; }
	OdGePoint3d	location(const int index) const;
	void location(const int index, const OdGePoint3d value);
	// プロパティ取得(円/楕円/ドーナツ/円周)
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
	// ダンプ
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
};

class CDataList;

//////////////////////////////////////////////////////////////////////
/// ブロックデータクラス
/// DWG でいう INSERT
//////////////////////////////////////////////////////////////////////

class EXTFUNC CDataBlock : public CData
{
protected:
	DPoint m_DPKijunTen;	// 配置基準点
	double m_dBairitsuX;	// X方向倍率
	double m_dBairitsuY;	// Y方向倍率
	double m_radKaitenKaku;	// 回転角
	CTypedPtrList<CObList, CDataList*>* m_pDataListList;
	//リストへのポインタ
	DWORD m_nNumber;		// 定義データの通し番号

public:
	// シリアライズ
	DECLARE_SERIAL(CDataBlock);
	virtual void Serialize(CArchive& ar);

	// データリスト取得
	CDataList* GetDataList();
	void init(CJwwDocument *pDoc);
	// プロパティ
	DWORD	blockNo() const;
	OdGePoint3d	start() const;
	double	rotation() const;
	OdGeScale3d scale() const;
	void blockNo(DWORD	value);
	void start(OdGePoint3d	value);
	void rotation(double value);
	void scale(OdGeScale3d value);
	// ダンプ
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//////////////////////////////////////////////////////////////////////
/// データリストクラス
/// DWG でいう BLOCK
// 2014/8/8 m_time は DWORDではなくCTime
//////////////////////////////////////////////////////////////////////

class EXTFUNC CDataList : public CData
{
public:
	DWORD m_nNumber;        // 通し番号
	DWORD m_bReffered;      // 被参照フラグ
	CTime m_time;           // 定義時刻
	CString m_strName;      // ブロック定義名称
	CTypedPtrList<CObList, CData*> m_DataList;   // ブロックに含まれる図形リスト

public:
	// シリアライズ
	DECLARE_SERIAL(CDataList);
	virtual void Serialize(CArchive& ar);
	// プロパティ
	OdString name() const;
	// ダンプ
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//////////////////////////////////////////////////////////////////////
/// JWWファイル インポートドキュメントクラス

class EXTFUNC CJwwDocument : public CObject
{
public:
	/// JWWファイル ヘッダ
	CJwwHeader	m_JwwHeader;
	/// 図形データのリスト(ENTITIES SECTION 相当)
	CTypedPtrList<CObList, CData*>	m_DataList;
	/// ブロックデータ定義部のリスト(BLOCKS SECTION 相当)
	CTypedPtrList<CObList, CDataList*>	m_DataListList;
	/// 画像ファイル名の配列(入力=すべての画像ファイル 入力=埋め込み画像ファイルのみ)
	std::set<CString>	m_JwwEmbbededImageFiles;
	/// JWW図面ファイルのパス(イメージの相対パスを求める際に必要)
	CString	m_jwwPath;
	/// 添付画像出力先ディレクトリ
	CString m_extructDir;
	/// 画層展開を抑制(レポート出力用)
	BOOL m_bReadForReport;
	// 0:一般図形処理中 1:ブロック図形処理中
	int m_iDepth;
	/// カスタムINIファイル "JDraf 2016.ini"の場所
	CString m_iniPath;

public:
	/// コンストラクタ
	CJwwDocument();
	/// デストラクタ
	virtual ~CJwwDocument();
	/// シリアライズ
	DECLARE_SERIAL(CJwwDocument)
	virtual void Serialize(CArchive& ar);
	/// プロパティ
	void setExtructDir(LPCTSTR dir) { m_extructDir = dir; }
	/// カスタムINIファイル "JDraf 2016.ini"の場所
	void setIniPath(LPCTSTR path) { m_iniPath = path; }

protected:
	virtual BOOL OnNewDocument();
	//DECLARE_MESSAGE_MAP()

protected:
	void WriteImageFiles(CArchive& ar);	      // 画像ファイル埋め込み
	void ExtructImageFiles(CArchive& ar);	  // 画像ファイル展開
	/// 全データ消去
	void ClearData();

	// 変換
public:
	// 縮尺と変位の適用と初期化
	void DoDataScale(double &dValue);
	void DoDataScale(struct DPoint & pValue);
	void SetDataScale(const double dValue, const struct DPoint & pIn);
	// 縮尺と変位を取得
	double GetDataScale();
	void GetDataOffset(struct DPoint & pOut);
	// RAY,XLINEの出力範囲の計算に使う
	void SetMinMaxPt(const OdGePoint3d& min, const OdGePoint3d& max);
	void GetMinMaxPt(OdGePoint3d& min, OdGePoint3d& max) const;
	// 入力用図面範囲
	void GetDocumentArea(OdGePoint2d& min, OdGePoint2d& max) const;
	// 主図形の数
	INT_PTR	GetDataCount() const;
	// 主図形の先頭
	POSITION GetHeadPosition() const;
	// 主図形のデータリスト
	CTypedPtrList<CObList, CData*>& GetDataList() { return m_DataList; }
	// ブロック図形のデータリスト
	CTypedPtrList<CObList, CDataList*>* GetBlockList() { return &m_DataListList; }
	// バージョンを取得
	DWORD GetVersion() const;
	// ブロック深さを設定
	void SetDepth(const int depth) { m_iDepth = depth; }
	// ブロック深さを取得
	int GetDepth(void) const { return m_iDepth; }
	// JWW展開用パスを設定
	void SetJwwPath(const CString& path) { m_jwwPath = path; }
	// JWW展開用パスを取得
	CString GetJwwPath() { return m_jwwPath; }

	// 変換
public:
	/// 用紙サイズ定義
	static const double YOUSHI_SIZE[15];
	static const double YOUSHI_TATE[15];
	/// 標準的な図面の縮尺
	static const double HYOJUN_SHAKUDO[8];
	/// JWCAD DXF出力線種名
	static const char*	JWCADDXF_LTYPES[48];
	// ATTN:画層名の付け方 (0 番号と名前 1 番号のみ)
	BOOL	m_bLayerName;
	/// ATTN:線種変換係数
	double m_lineFactor;
	/// ATTN:レイヤ番号が一致したら新しい画層を作らない(0=つくる/1=作らない)
	BOOL m_gMatchLayerNumber;

	/// ダンプ
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
