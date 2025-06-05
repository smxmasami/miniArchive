//****************************************
// JwwDocument.cpp : 実装ファイル
//****************************************

#include "stdafx.h"

#define SXCOL_EXT 100;
#define SXLTP_EXT 30;

// 用紙サイズ毎の横幅(mm)
const double CJwwDocument::YOUSHI_SIZE[15] =
{
	1189.0,    // A0
	841.0,     // A1
	594.0,     // A2
	420.0,     // A3
	297.0,     // A4
	0.0,       // 使用しない
	0.0,       // 使用しない
	0.0,       // 使用しない
	1682.0,    // 2A   ← JWC仕様書には書いていない
	2378.0,    // 3A   ← JWC仕様書には書いていない
	3364.0,    // 4A   ← JWC仕様書には書いていない
	4756.0,    // 5A   ← JWC仕様書には書いていない
	10000.0,   // 10m
	50000.0,   // 50m  ← JWC仕様書には書いていない
	100000.0	// 100m ← JWC仕様書には書いていない
};

// 用紙サイズ毎の縦幅(mm)
//
// 2013-11-04 JWWの仕様を確認したら、10m,50m,100mの用紙サイズが違うことに
// 気が付いたが、JWIMPORTの仕様としては従来通りいくことにした。
// A4-5A は用紙左下を(0,0)にするように図形を移動する。
// 10m,50m,100mは縦横共10m,50m,100mとし、(0,0)は中心にある。
// JWの図形は(0,0)を中心にマイナスの方向にもあることに注意
//
const double CJwwDocument::YOUSHI_TATE[15] =
{
	841.0,     // A0
	594.0,     // A1
	420.0,     // A2
	297.0,     // A3
	210.0,     // A4
	0.0,       // 使用しない
	0.0,       // 使用しない
	0.0,       // 使用しない
	1189.0,    // 2A
	1682.0,    // 3A
	2378.0,    // 4A
	3364.0,    // 5A
	10000.0,   // 10m (JWWは 7073.0 )
	50000.0,   // 50m (JWWは 35366.0 )
	100000.0	// 100m (JWWは 70732.0 )
};

// 標準的な図面の縮尺
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
// CJwwHeader : JWWファイルヘッダ
//****************************************

CJwwHeader::CJwwHeader()
{
	// -1 は、スケールが設定されていないフラグ
	m_dJwwScale = -1;
	// オフセット値は(0,0)に初期化
	m_pJwwOffset.x = 0.0;
	m_pJwwOffset.y = 0.0;
	// 文字の角度補正はしないに初期化
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

	// 書き出し
	if (ar.IsStoring())
	{
		// JWWのデータファイル宣言
		ar.Write(m_cVersion, 8);

		// バージョンNo.
		ar << m_Version;

		// ファイルメモ
		ar << CStringA(m_strMemo);

		// 図面サイズ
		// 0～4 ：A0～A4
		// 8    ：2 A
		// 9    ：3 A
		// 10   ：4 A
		// 11   ：5 A
		// 12   ：10m
		// 13   ：50m
		// 14   ：100m
		ar << m_nZumen;

		// 画層グループ・画層状態
		// 書込画層グループ
		// 画層グループ状態(0：非表示、1：表示のみ、2：編集可能、3：書込)
		// 画層グループの書込画層
		// 画層グループの縮尺(の分母)
		// 画層グループのプロテクト指定
		// 　(0：無指定、1：表示状態変更可能プロテクト指定、2：表示状態固定プロテクト指定)
		// 画層状態(0：非表示、1：表示のみ、2：編集可能、3：書込)
		// 画層のプロテクト指定
		// 　(0：無指定、1：表示状態変更可能プロテクト指定、2：表示状態固定プロテクト指定)
		ar << m_nWriteGLay;

		for (int nGLay = 0; nGLay < 16; nGLay++)
		{
			ar << m_anGLay[nGLay];
			ar << m_anWriteLay[nGLay];
			ar << m_adScale[nGLay];
			ar << m_anGLayProtect[nGLay];

			for (int nLay = 0; nLay < 16; nLay++)
			{
				ar << m_aanLay[nGLay][nLay];
				ar << m_aanLayProtect[nGLay][nLay];
			}
		}

		// ダミー
		for (i = 0, dummy = 0; i < 14; i++)
		{
			ar << dummy;
		}

		// 寸法関係の設定
		// 環境設定ファイルの「S_COMM_8」の③寸法設定が「10」または「11」になっていて、
		// ファイルへの保存ができる場合に有効になり、
		// それ以外は「m_lnSunpou1」～「m_lnSunpou5」は「0」になる。

		// 一位：寸法線色（1～9）
		// 十位：引出線色（1～9）
		// 百位：寸法点色（1～9）
		// 千位：寸法値の少数点以下の桁数（0～3）
		// 万位：寸法単位（0:mm 、1:ｍ）
		// 十万位：寸法線端部（0:点 、1:矢印 、2:逆矢印）
		// 百万位：寸法文字種（1～10）
		ar << m_lnSunpou1;
		// 一位～千位：寸法値の寸法線との離れ
		// 　　　　　　離れ（0～99.9）×１０、マイナスの場合は1000をプラスして整数化する
		// 万位～千万位：寸法線の突出長さ
		// 　　　　　　長さ（0～99.9）×１０、マイナスの場合は1000をプラスして整数化し、更に10000倍する。
		ar << m_lnSunpou2;
		// 一位～百位：矢印長さ
		// 　　　　　　長さ（0～99.9）×１０として整数化する
		// 千位～十万位：矢印角度
		// 　　　　　　角度（0.1～80.0）×１０として整数化し、1000倍する
		// 百万位～億位：逆矢印の出寸法
		// 　　　　　　寸法（0～99.9）×１０として整数化すし、1000000倍する
		ar << m_lnSunpou3;
		// 一位：寸法値方向を補正しない（0：補正有 、1：補正無）
		// 十位：寸法値を全角にする（0：半角 、1：全角）
		// 百位：寸法値のカンマをスペースにする（0：カンマ 、1：スペース）
		// 千位：寸法値のカンマを全角にする（0：半角 、1：全角）
		// 万位：寸法値の少数点を全角にする（0：半角 、1：全角）
		// 十万位：寸法値に単位を表示する（0：表示無 、1：表示有）
		// 百万位：半径値に「R」を表示する（0：表示無 、1：前付表示有 、2：後付表示有）
		// 千万位：半径値にカンマを表示する（0：表示無 、1：表示有）
		// 億位：半径値に少数点の「0」を表示する（0：表示無 、1：表示有）
		ar << m_lnSunpou4;
		// 一位　：寸法値を斜め文字にする（0：通常文字 、1：斜め文字）
		// 十位　：寸法値を太文字にする（0：通常文字 、1：太文字）
		// 百位　：角度単位（0：度単位 、1：度分秒単位 、2：度単位で「ﾟ」の表示なし）
		// 千位　：角度単位の少数点以下の桁数（0～6）
		// 万位　：寸法図形にする（0：寸法図形にしない 、1：寸法図形にする）
		// 十万位：範囲選択のとき寸法図形を線色・種の属性で選択（0：選択しない 、1：選択する）
		// 百万位：表示小数点以下の処理（0:四捨五入　1:切捨　2:切上）(Ver.4.02以降）
		ar << m_lnSunpou5;

		// ダミー
		ar << dummy;

		// 線描画の最大幅
		// 1から100 線幅はピクセル単位で指定
		// -1から-100 線幅は abs(1/N)ミリ単位で指定(-16->1/16ミリ)
		//「線幅を1/100mm単位とする」が設定されているときは-300を指定(-100より小さい)
		ar << m_nMaxDrawWid;

		// プリンタ出力範囲の原点(X,Y)
		ar << m_DPPrtGenten.x;
		ar << m_DPPrtGenten.y;

		// プリンタ出力倍率
		ar << m_dPrtBairitsu;

		// プリンタ90°回転出力、プリンタ出力基準点位置
		// 一位：プリンタ90°回転出力
		// 十位：プリンタ出力基準点位置の指定（0:無指定）（Ver.3.00以降）
		//  　  7:左上   8:中上  9:右上
		//   　 4:左中   5:中中  6:右中
		//    　1:左下   2:中下  3:右下
		ar << m_nPrtSet;

		// 目盛設定モード
		// 一位(0：無指定、1：目盛表示)
		// 十位(0：図面寸法指定、1：実寸指定)
		// －値の時、目盛読取り不可
		ar << m_nMemoriMode;

		// 目盛表示最小間隔ドット
		ar << m_dMemoriHyoujiMin;

		// 目盛表示間隔(X,Y)
		ar << m_dMemoriX;
		ar << m_dMemoriY;

		// 目盛基準点(X,Y)
		ar << m_DpMemoriKijunTen.x;
		ar << m_DpMemoriKijunTen.y;

		// 画層名
		for (n = 0; n <= 15; n++)
		{
			for (k = 0; k <= 15; k++)
			{
				ar << CStringA(m_aStrLayName[n][k]);
			}
		}

		// 画層グループ名
		for (n = 0; n <= 15; n++)
		{
			ar << CStringA(m_aStrGLayName[n]);
		}

		// 日影計算の条件
		// 測定面高さ
		// 緯度
		// 9～15の測定の指定
		// 壁面日影測定面高さ
		ar << m_dKageLevel;
		ar << m_dKageIdo;
		ar << m_nKage9_15JiFlg;
		ar << m_dKabeKageLevel;

		// 天空図の条件（Ver.3.00以降)
		// 測定面高さ
		// 天空図の半径＊２
		if (m_Version >= 300)
		{
			ar << m_dTenkuuZuLevel;
			ar << m_dTenkuuZuEnkoR;
		}
		// 2.5Dの計算単位(0以外はmm単位で計算)
		ar << m_nMMTani3D;

		// 保存時の画面倍率(読込むと前画面倍率になる)
		ar << m_dBairitsu;
		ar << m_DPGenten.x;
		ar << m_DPGenten.y;

		// 範囲記憶倍率と基準点(X,Y)
		ar << m_dHanniBairitsu;
		ar << m_DPHanniGenten.x;
		ar << m_DPHanniGenten.y;

		// マークジャンプ倍率、基準点(X,Y)および画層グループ
		if (m_Version >= 300)
		{
			for (n = 1; n <= 8; n++)
			{
				ar << m_dZoomJumpBairitsu[n];
				ar << m_DPZoomJumpGenten[n].x;
				ar << m_DPZoomJumpGenten[n].y;
				ar << m_nZoomJumpGLay[n];
			}
		}
		else
		{
			for (n = 1; n <= 4; n++)
			{
				ar << m_dZoomJumpBairitsu[n];
				ar << m_DPZoomJumpGenten[n].x;
				ar << m_DPZoomJumpGenten[n].y;
			}
		}

		// 文字の描画状態(Ver.4.05以降）
		if (m_Version >= 300)
		{
			ar << m_dDm11;		//ダミー
			ar << m_dDm12;		//ダミー
			ar << m_dDm13;		//ダミー
			ar << m_lnDm1;			//ダミー
			ar << m_dDm21;		//ダミー
			ar << m_dDm22;		//ダミー
			ar << m_dMojiBG;	//(Ver.4.04以前はダミー）
			//文字列範囲を背景色で描画するときの範囲増寸法
			ar << m_nMojiBG;		//(Ver.4.04以前はダミー）
			//十位:文字（寸法図形､ブロック図形）を最後に描画
			//一位:1 :輪郭・範囲を背景色で描画しない
			//	2 :文字の輪郭を背景色で描画する
			//	3 :文字列範囲を背景色で描画する
		}

		// 複線間隔
		for (n = 0; n <= 9; n++)
		{
			ar << m_adFukusenSuuchi[n];
		}

		// 両側複線の留線出の寸法
		ar << m_dRyoygawaFukusenTomeDe;

		// 色番号ごとの画面表示色、線幅
		// 画面表示色(0：背景色、1～8：線色、9：グレー色)
		// 線幅(1～16)
		for (n = 0; n <= 9; n++)
		{
			ar << m_aPenColor[n];
			ar << m_anPenWidth[n];
		}

		// 色番号ごとのプリンタ出力色、線幅、実点半径
		// プリンタ出力色(0：背景色、1～8：線色、9：グレー色)
		// 線幅(1～500)
		// 実点半径(0.1～10)
		for (n = 0; n <= 9; n++)
		{
			ar << m_aPrtPenColor[n];
			ar << m_anPrtPenWidth[n];
			ar << m_adPrtTenHankei[n];
		}

		// 線種番号2から9までのパターン、1ユニットのドット数、ピッチ、プリンタ出力ピッチ
		for (n = 2; n <= 9; n++)
		{
			ar << m_alLType[n];
			ar << m_anTokushuSenUintDot[n];
			ar << m_anTokushuSenPich[n];
			ar << m_anPrtTokushuSenPich[n];
		}

		// ランダム線1から5までのパターン、画面表示振幅・ピッチ、プリンタ出力振幅・ピッチ
		for (n = 11; n <= 15; n++)
		{
			ar << m_alLType[n];
			ar << m_anRandSenWide[n];
			ar << m_anTokushuSenPich[n];
			ar << m_anPrtRandSenWide[n];
			ar << m_anPrtTokushuSenPich[n];
		}

		// 倍長線種番号6から9までのパターン、1ユニットのドット数、ピッチ、プリンタ出力ピッチ
		for (n = 16; n <= 19; n++)
		{
			ar << m_alLType[n];
			ar << m_anTokushuSenUintDot[n];
			ar << m_anTokushuSenPich[n];
			ar << m_anPrtTokushuSenPich[n];
		}

		// 実点を画面描画時の指定半径で描画
		ar << m_nDrawGamenTen;

		// 実点をプリンタ出力時、指定半径で書く
		ar << m_nDrawPrtTen;

		// BitMap・ソリッドを最初に描画する
		ar << m_nBitMapFirstDraw;
		// 十位：ソリッド描画順
		//  0:画層順        (+1):色順
		//  3:画層逆順      (+2):色逆順
		//  6:印刷出力設定順
		// 一位：
		//  0:BitMap・ソリッドを最初に描画しない
		//  1:BitMap・ソリッドを最初に描画
		//  2:ソリッドを最初に描画
		// 逆描画
		ar << m_nGyakuDraw;

		// 逆サーチ
		ar << m_nGyakuSearch;

		// カラー印刷
		ar << m_nColorPrint;

		// 画層順の印刷
		ar << m_nLayJunPrint;

		// 色番号順の印刷
		ar << m_nColJunPrint;

		// 画層グループまたは画層ごとのプリンタ連続出力指定
		ar << m_nPrtRenzoku;

		// プリンタ共通画層(表示のみ画層)のグレー出力指定
		ar << m_nPrtKyoutsuuGray;

		// プリンタ出力時に表示のみ画層は出力しない
		ar << m_nPrtDispOnlyNonDraw;

		if (m_Version >= 223)
		{
			// 作図時間（Ver.2.23以降）
			ar << m_lnDrawTime;

			// 2.5Dの始点位置が設定されている時のフラグ（Ver.2.23以降）
			// 一位：透視図の視点位置設定済みフラグ
			// 十位：鳥瞰図の視点位置設定済みフラグ
			// 百位：アイソメ図の視点位置設定済みフラグ
			ar << m_nEyeInit;

			// 2.5Dの透視図・鳥瞰図・アイソメ図の視点水平角（Ver.2.23以降）
			ar << m_dEye_H_Ichi_1; //*100必要?
			ar << m_dEye_H_Ichi_2;
			ar << m_dEye_H_Ichi_3;

			// 2.5Dの透視図の視点高さ・視点離れ（Ver.2.23以降）
			ar << m_dEye_Z_Ichi_1;
			ar << m_dEye_Y_Ichi_1;

			// 2.5Dの鳥瞰図の視点高さ・視点離れ（Ver.2.23以降）
			ar << m_dEye_Z_Ichi_2;
			ar << m_dEye_Y_Ichi_2;

			// 2.5Dのアイソメ図の視点垂直角（Ver.2.23以降）
			ar << m_dEye_V_Ichi_3;
		}

		// 線の長さ指定の最終値（Ver.2.25以降）
		if (m_Version >= 225)
		{
			ar << m_dSenNagasaSnpou;
		}

		// 矩形寸法横寸法・縦寸法指定の最終値（Ver.2.25以降）
		if (m_Version >= 225)
		{
			ar << m_dBoxSunpouX;
			ar << m_dBoxSunpouY;
		}

		// 円の半径指定の最終値（Ver.2.25以降）
		if (m_Version >= 225)
		{
			ar << m_dEnHankeiSnpou;
		}

		// ソリッドを任意色で書くフラグ、ソリッドの任意色の既定値（Ver.2.30以降）
		if (m_Version >= 230)
		{
			ar << m_nSolidNinniColor;
			ar << m_SolidColor;	// RGB
		}

		// SXF対応拡張線色定義（Ver.4.20以降）
		// 画面表示色
		// 画面表示線幅
		// 線色名
		// プリンタ出力色
		// プリンタ出力線幅
		// 点半径
		if (m_Version >= 420)
		{
			int n1;

			for (n = 0; n <= 256; n++)	////// 画面表示色
			{
				n1 = n + SXCOL_EXT;		//色番号のオフセット = +100
				ar << m_aPenColor[n1];
				ar << m_anPenWidth[n1];
			}
			for (n = 0; n <= 256; n++)	////// プリンタ出力色
			{
				ar << CStringA(m_astrUDColorName[n]);
				n1 = n + SXCOL_EXT;	//色番号のオフセット = +100
				ar << m_aPrtPenColor[n1];
				ar << m_anPrtPenWidth[n1];
				ar << m_adPrtTenHankei[n1];
			}

			// SXF対応拡張線種定義（Ver.4.20以降）
			// パターン
			// 1ユニットのドット数
			// ピッチ
			// プリンタ出力ピッチ
			// 線種名
			// セグメント数
			// ピッチ線分の長さ、空白長さの繰り返し
			for (n = 0; n <= 32; n++)	////// 線種パターン
			{
				n1 = n + SXLTP_EXT;   //線種番号のオフセット = +30
				ar << m_alLType[n1];
				ar << m_anTokushuSenUintDot[n1];
				ar << m_anTokushuSenPich[n1];
				ar << m_anPrtTokushuSenPich[n1];
			}
			for (n = 0; n <= 32; n++)	////// 線種パラメータ
			{
				ar << CStringA(m_astrUDLTypeName[n]);
				ar << m_anUDLTypeSegment[n];
				for (j = 1; j <= 10; j++)
				{
					ar << m_aadUDLTypePitch[n][j];
				}
			}
		}

		// 文字種1から10までの文字幅、高さ、間隔、色番号
		for (i = 1; i <= 10; i++)
		{
			ar << m_adMojiX[i];
			ar << m_adMojiY[i];
			ar << m_adMojiD[i];
			ar << m_anMojiCol[i];
		}

		// 書込み文字の文字幅、高さ、間隔、色番号、文字番号
		ar << m_dMojiSizeX;
		ar << m_dMojiSizeY;
		ar << m_dMojiKankaku;
		ar << m_nMojiColor;
		ar << m_nMojiShu;

		// 文字位置整理の行間、文字数
		ar << m_dMojiSeiriGyouKan;
		ar << m_dMojiSeiriSuu;

		// 文字基準点のずれ位置使用のフラグ
		ar << m_nMojiKijunZureOn;

		// 文字基準点の横方向のずれ位置左、中、右
		ar << m_adMojiKijunZureX[0];
		ar << m_adMojiKijunZureX[1];
		ar << m_adMojiKijunZureX[2];

		// 文字基準点の縦方向のずれ位置下、中、上
		ar << m_adMojiKijunZureY[0];
		ar << m_adMojiKijunZureY[1];
		ar << m_adMojiKijunZureY[2];

		TRACE("Finish saving header.\n");
	}
	// 読み込み
	else
	{
		// JWWのデータファイル宣言
		ar.Read(m_cVersion, 8);

		// バージョンNo.
		ar >> m_Version;
		if (m_Version < 230)
		{
#ifdef linux
			std::cout << "File version is too old.. Use Jw_cad and save as the latest version." << std::endl;
			return;
#else			
			AfxMessageBox(L"\nFile version is too old.. Use Jw_cad and save as the latest version.\n");
			AfxThrowUserException();
#endif
		}

		// ファイルメモ
		ar >> m_strMemo;

		// 図面サイズ
		// 0～4 ：A0～A4
		// 8    ：2 A
		// 9    ：3 A
		// 10   ：4 A
		// 11   ：5 A
		// 12   ：10m
		// 13   ：50m
		// 14   ：100m
		ar >> m_nZumen;

		// 画層グループ・画層状態
		// 書込画層グループ
		// 画層グループ状態(0：非表示、1：表示のみ、2：編集可能、3：書込)
		// 画層グループの書込画層
		// 画層グループの縮尺(の分母)
		// 画層グループのプロテクト指定
		// 　(0：無指定、1：表示状態変更可能プロテクト指定、2：表示状態固定プロテクト指定)
		// 画層状態(0：非表示、1：表示のみ、2：編集可能、3：書込)
		// 画層のプロテクト指定
		// 　(0：無指定、1：表示状態変更可能プロテクト指定、2：表示状態固定プロテクト指定)
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

		// ダミー
		for (i = 0; i < 14; i++)
		{
			ar >> dummy;
		}

		// 寸法関係の設定
		// 環境設定ファイルの「S_COMM_8」の③寸法設定が「10」または「11」になっていて、
		// ファイルへの保存ができる場合に有効になり、
		// それ以外は「m_lnSunpou1」～「m_lnSunpou5」は「0」になる。

		// 一位：寸法線色（1～9）
		// 十位：引出線色（1～9）
		// 百位：寸法点色（1～9）
		// 千位：寸法値の少数点以下の桁数（0～3）
		// 万位：寸法単位（0:mm 、1:ｍ）
		// 十万位：寸法線端部（0:点 、1:矢印 、2:逆矢印）
		// 百万位：寸法文字種（1～10）
		ar >> m_lnSunpou1;
		// 一位～千位：寸法値の寸法線との離れ
		// 　　　　　　離れ（0～99.9）×１０、マイナスの場合は1000をプラスして整数化する
		// 万位～千万位：寸法線の突出長さ
		// 　　　　　　長さ（0～99.9）×１０、マイナスの場合は1000をプラスして整数化し、更に10000倍する。
		ar >> m_lnSunpou2;
		// 一位～百位：矢印長さ
		// 　　　　　　長さ（0～99.9）×１０として整数化する
		// 千位～十万位：矢印角度
		// 　　　　　　角度（0.1～80.0）×１０として整数化し、1000倍する
		// 百万位～億位：逆矢印の出寸法
		// 　　　　　　寸法（0～99.9）×１０として整数化すし、1000000倍する
		ar >> m_lnSunpou3;
		// 一位：寸法値方向を補正しない（0：補正有 、1：補正無）
		// 十位：寸法値を全角にする（0：半角 、1：全角）
		// 百位：寸法値のカンマをスペースにする（0：カンマ 、1：スペース）
		// 千位：寸法値のカンマを全角にする（0：半角 、1：全角）
		// 万位：寸法値の少数点を全角にする（0：半角 、1：全角）
		// 十万位：寸法値に単位を表示する（0：表示無 、1：表示有）
		// 百万位：半径値に「R」を表示する（0：表示無 、1：前付表示有 、2：後付表示有）
		// 千万位：半径値にカンマを表示する（0：表示無 、1：表示有）
		// 億位：半径値に少数点の「0」を表示する（0：表示無 、1：表示有）
		ar >> m_lnSunpou4;
		// 一位　：寸法値を斜め文字にする（0：通常文字 、1：斜め文字）
		// 十位　：寸法値を太文字にする（0：通常文字 、1：太文字）
		// 百位　：角度単位（0：度単位 、1：度分秒単位 、2：度単位で「ﾟ」の表示なし）
		// 千位　：角度単位の少数点以下の桁数（0～6）
		// 万位　：寸法図形にする（0：寸法図形にしない 、1：寸法図形にする）
		// 十万位：範囲選択のとき寸法図形を線色・種の属性で選択（0：選択しない 、1：選択する）
		// 百万位：表示小数点以下の処理（0:四捨五入　1:切捨　2:切上）(Ver.4.02以降）
		ar >> m_lnSunpou5;

		// ダミー
		ar >> dummy;

		// 線印刷の最大幅(1～100,-1～-100,-300)
		// 1から100 線幅はピクセル単位で指定
		// -1から-100 線幅は abs(1/N)ミリ単位で指定(-16->1/16ミリ)
		//「線幅を1/100mm単位とする」が設定されているときは-300を指定(-100より小さい)
		ar >> m_nMaxDrawWid;

		// プリンタ出力範囲の原点(X,Y)
		ar >> m_DPPrtGenten.x;
		ar >> m_DPPrtGenten.y;

		// プリンタ出力倍率
		ar >> m_dPrtBairitsu;

		// プリンタ90°回転出力、プリンタ出力基準点位置
		// 一位：プリンタ90°回転出力
		// 十位：プリンタ出力基準点位置の指定（0:無指定）（Ver.3.00以降）
		//  　  7:左上   8:中上  9:右上
		//   　 4:左中   5:中中  6:右中
		//    　1:左下   2:中下  3:右下
		ar >> m_nPrtSet;

		// 目盛設定モード
		// 一位(0：無指定、1：目盛表示)
		// 十位(0：図面寸法指定、1：実寸指定)
		// －値の時、目盛読取り不可
		ar >> m_nMemoriMode;

		// 目盛表示最小間隔ドット
		ar >> m_dMemoriHyoujiMin;

		// 目盛表示間隔(X,Y)
		ar >> m_dMemoriX;
		ar >> m_dMemoriY;

		// 目盛基準点(X,Y)
		ar >> m_DpMemoriKijunTen.x;
		ar >> m_DpMemoriKijunTen.y;

		// 画層名
		for (n = 0; n <= 15; n++)
		{
			for (k = 0; k <= 15; k++)
			{
				ar >> m_aStrLayName[n][k];
			}
		}

		// 画層グループ名
		for (n = 0; n <= 15; n++)
		{
			ar >> m_aStrGLayName[n];
		}

		// 日影計算の条件
		// 測定面高さ
		// 緯度
		// 9～15の測定の指定
		// 壁面日影測定面高さ
		ar >> m_dKageLevel;
		ar >> m_dKageIdo;
		ar >> m_nKage9_15JiFlg;
		ar >> m_dKabeKageLevel;

		// 天空図の条件（Ver.3.00以降)
		// 測定面高さ
		// 天空図の半径＊２
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
		// 2.5Dの計算単位(0以外はmm単位で計算)
		ar >> m_nMMTani3D;

		// 保存時の画面倍率(読込むと前画面倍率になる)
		ar >> m_dBairitsu;
		ar >> m_DPGenten.x;
		ar >> m_DPGenten.y;

		// 範囲記憶倍率と基準点(X,Y)
		ar >> m_dHanniBairitsu;
		ar >> m_DPHanniGenten.x;
		ar >> m_DPHanniGenten.y;

		// マークジャンプ倍率、基準点(X,Y)および画層グループ
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
			// 倍率=1,中心(0,0)は未使用の印でもある
			for (; n <= 8; n++)
			{
				m_dZoomJumpBairitsu[n] = 1.0;
				m_DPZoomJumpGenten[n].x = 0.0;
				m_DPZoomJumpGenten[n].y = 0.0;
				m_nZoomJumpGLay[n] = 0xffffffff;
			}
		}

		// 文字の描画状態(Ver.4.05以降）
		if (m_Version >= 300)
		{
			ar >> m_dDm11;		//ダミー
			ar >> m_dDm12;		//ダミー
			ar >> m_dDm13;		//ダミー
			ar >> m_lnDm1;			//ダミー
			ar >> m_dDm21;		//ダミー
			ar >> m_dDm22;		//ダミー
			ar >> m_dMojiBG;	//(Ver.4.04以前はダミー）
			//文字列範囲を背景色で描画するときの範囲増寸法
			ar >> m_nMojiBG;		//(Ver.4.04以前はダミー）
			//十位:文字（寸法図形､ブロック図形）を最後に描画
			//一位:1 :輪郭・範囲を背景色で描画しない
			//	2 :文字の輪郭を背景色で描画する
			//	3 :文字列範囲を背景色で描画する
		}
		else
		{
			m_dMojiBG = 0.0;
			m_nMojiBG = 0;
		}
		// 複線間隔
		for (n = 0; n <= 9; n++)
		{
			ar >> m_adFukusenSuuchi[n];
		}

		// 両側複線の留線出の寸法
		ar >> m_dRyoygawaFukusenTomeDe;

		// 色番号ごとの画面表示色、線幅
		// 画面表示色(0：背景色、1～8：線色、9：グレー色)
		// 線幅(1～16)
		for (n = 0; n <= 9; n++)
		{
			ar >> m_aPenColor[n];
			ar >> m_anPenWidth[n];
		}

		// 色番号ごとのプリンタ出力色、線幅、実点半径
		// プリンタ出力色(0：背景色、1～8：線色、9：グレー色)
		// 線幅(1～500)
		// 実点半径(0.1～10)
		for (n = 0; n <= 9; n++)
		{
			ar >> m_aPrtPenColor[n];
			ar >> m_anPrtPenWidth[n];
			ar >> m_adPrtTenHankei[n];
		}

		// 線種番号2から9までのパターン、1ユニットのドット数、ピッチ、プリンタ出力ピッチ
		for (n = 2; n <= 9; n++)
		{
			ar >> m_alLType[n];
			ar >> m_anTokushuSenUintDot[n];
			ar >> m_anTokushuSenPich[n];
			ar >> m_anPrtTokushuSenPich[n];
		}

		// ランダム線1から5までのパターン、画面表示振幅・ピッチ、プリンタ出力振幅・ピッチ
		for (n = 11; n <= 15; n++)
		{
			ar >> m_alLType[n];
			ar >> m_anRandSenWide[n];
			ar >> m_anTokushuSenPich[n];
			ar >> m_anPrtRandSenWide[n];
			ar >> m_anPrtTokushuSenPich[n];
		}

		// 倍長線種番号6から9までのパターン、1ユニットのドット数、ピッチ、プリンタ出力ピッチ
		for (n = 16; n <= 19; n++)
		{
			ar >> m_alLType[n];
			ar >> m_anTokushuSenUintDot[n];
			ar >> m_anTokushuSenPich[n];
			ar >> m_anPrtTokushuSenPich[n];
		}

		// 実点を画面描画時の指定半径で描画
		ar >> m_nDrawGamenTen;

		// 実点をプリンタ出力時、指定半径で書く
		ar >> m_nDrawPrtTen;

		// BitMap・ソリッドを最初に描画する
		ar >> m_nBitMapFirstDraw;
		// 十位：ソリッド描画順
		//  0:画層順        (+1):色順
		//  3:画層逆順      (+2):色逆順
		//  6:印刷出力設定順
		// 一位：
		//  0:BitMap・ソリッドを最初に描画しない
		//  1:BitMap・ソリッドを最初に描画
		//  2:ソリッドを最初に描画
		// 逆描画
		ar >> m_nGyakuDraw;

		// 逆サーチ
		ar >> m_nGyakuSearch;

		// カラー印刷
		ar >> m_nColorPrint;

		// 画層順の印刷
		ar >> m_nLayJunPrint;

		// 色番号順の印刷
		ar >> m_nColJunPrint;

		// 画層グループまたは画層ごとのプリンタ連続出力指定
		ar >> m_nPrtRenzoku;

		// プリンタ共通画層(表示のみ画層)のグレー出力指定
		ar >> m_nPrtKyoutsuuGray;

		// プリンタ出力時に表示のみ画層は出力しない
		ar >> m_nPrtDispOnlyNonDraw;

		if (m_Version >= 223)
		{
			// 作図時間（Ver.2.23以降）
			ar >> m_lnDrawTime;

			// 2.5Dの始点位置が設定されている時のフラグ（Ver.2.23以降）
			// 一位：透視図の視点位置設定済みフラグ
			// 十位：鳥瞰図の視点位置設定済みフラグ
			// 百位：アイソメ図の視点位置設定済みフラグ
			ar >> m_nEyeInit;

			// 2.5Dの透視図・鳥瞰図・アイソメ図の視点水平角（Ver.2.23以降）
			DWORD dEye_H_Ichi_1, dEye_H_Ichi_2, dEye_H_Ichi_3;
			ar >> dEye_H_Ichi_1;
			ar >> dEye_H_Ichi_2;
			ar >> dEye_H_Ichi_3;
			//m_dEye_H_Ichi_1 = (dEye_H_Ichi_1 / 100);
			//m_dEye_H_Ichi_2 = (dEye_H_Ichi_2 / 100);
			//m_dEye_H_Ichi_3 = (dEye_H_Ichi_3 / 100);
			m_dEye_H_Ichi_1 = dEye_H_Ichi_1;
			m_dEye_H_Ichi_2 = dEye_H_Ichi_2;
			m_dEye_H_Ichi_3 = dEye_H_Ichi_3;

			// 2.5Dの透視図の視点高さ・視点離れ（Ver.2.23以降）
			ar >> m_dEye_Z_Ichi_1;
			ar >> m_dEye_Y_Ichi_1;

			// 2.5Dの鳥瞰図の視点高さ・視点離れ（Ver.2.23以降）
			ar >> m_dEye_Z_Ichi_2;
			ar >> m_dEye_Y_Ichi_2;

			// 2.5Dのアイソメ図の視点垂直角（Ver.2.23以降）
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

		// 線の長さ指定の最終値（Ver.2.25以降）
		if (m_Version >= 225)
		{
			ar >> m_dSenNagasaSnpou;
		}
		else
		{
			m_dSenNagasaSnpou = 0.0;
		}

		// 矩形寸法横寸法・縦寸法指定の最終値（Ver.2.25以降）
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

		// 円の半径指定の最終値（Ver.2.25以降）
		if (m_Version >= 225)
		{
			ar >> m_dEnHankeiSnpou;
		}
		else
		{
			m_dEnHankeiSnpou = 0.0;
		}

		// ソリッドを任意色で書くフラグ、ソリッドの任意色の既定値（Ver.2.30以降）
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

		// SXF対応拡張線色定義（Ver.4.20以降）
		// 画面表示色
		// 画面表示線幅
		// 線色名
		// プリンタ出力色
		// プリンタ出力線幅
		// 点半径
		if (m_Version >= 420)
		{
			int n1;

			for (n = 0; n <= 256; n++)	////// 画面表示色
			{
				n1 = n + SXCOL_EXT;		//色番号のオフセット = +100
				ar >> m_aPenColor[n1];
				ar >> m_anPenWidth[n1];
			}
			for (n = 0; n <= 256; n++)	////// プリンタ出力色
			{
				ar >> m_astrUDColorName[n];
				n1 = n + SXCOL_EXT;	//色番号のオフセット = +100
				ar >> m_aPrtPenColor[n1];
				ar >> m_anPrtPenWidth[n1];
				ar >> m_adPrtTenHankei[n1];
			}

			// SXF対応拡張線種定義（Ver.4.20以降）
			// パターン
			// 1ユニットのドット数
			// ピッチ
			// プリンタ出力ピッチ
			// 線種名
			// セグメント数
			// ピッチ線分の長さ、空白長さの繰り返し
			for (n = 0; n <= 32; n++)	////// 線種パターン
			{
				n1 = n + SXLTP_EXT;   //線種番号のオフセット = +30
				ar >> m_alLType[n1];
				ar >> m_anTokushuSenUintDot[n1];
				ar >> m_anTokushuSenPich[n1];
				ar >> m_anPrtTokushuSenPich[n1];
			}
			for (n = 0; n <= 32; n++)	////// 線種パラメータ
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
			// TODO: SXF色,線種の既定値を設定すべき
		}

		// 文字種1から10までの文字幅、高さ、間隔、色番号
		for (i = 1; i <= 10; i++)
		{
			ar >> m_adMojiX[i];
			ar >> m_adMojiY[i];
			ar >> m_adMojiD[i];
			ar >> m_anMojiCol[i];
		}

		// 書込み文字の文字幅、高さ、間隔、色番号、文字番号
		ar >> m_dMojiSizeX;
		ar >> m_dMojiSizeY;
		ar >> m_dMojiKankaku;
		ar >> m_nMojiColor;
		ar >> m_nMojiShu;

		// 文字位置整理の行間、文字数
		ar >> m_dMojiSeiriGyouKan;
		ar >> m_dMojiSeiriSuu;

		// 文字基準点のずれ位置使用のフラグ
		ar >> m_nMojiKijunZureOn;

		// 文字基準点の横方向のずれ位置左、中、右
		ar >> m_adMojiKijunZureX[0];
		ar >> m_adMojiKijunZureX[1];
		ar >> m_adMojiKijunZureX[2];

		// 文字基準点の縦方向のずれ位置下、中、上
		ar >> m_adMojiKijunZureY[0];
		ar >> m_adMojiKijunZureY[1];
		ar >> m_adMojiKijunZureY[2];

		TRACE("Finish reading header.\n");

		// 縮尺とオフセットを設定
		DPoint pDummy;
		pDummy.x = 0.0;
		pDummy.y = 0.0;
		SetDataScale(-1.0, pDummy);
	}
}

// スケール適用
void CJwwHeader::DoDataScale(double & dValue)
{
	dValue *= m_dJwwScale;
}

// スケール適用
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

// スケール設定
void CJwwHeader::SetDataScale(const double dValue, const struct DPoint & pIn)
{
	// 図面から読み取り、IMPORT用
	if (dValue < 0.0)
	{
		// 既定値は図寸のまま使用する
		m_dJwwScale = 1.0;

		// 設定を読んで、縮尺モードを設定(0:用紙、1:最大縮尺、2:ユーザ指定)
	#ifdef linux
		DWORD dwValue = 0;
	#else	
		DWORD dwValue = GetPrivateProfileInt(_T("JwwIN"), _T("Paper"), 1, m_pDoc->m_iniPath);
		//DWORD dwValue = FXAPI()->fdt_getconfintW(L"JwwIN",L"Paper",1);
		if (dwValue == 1) // 「実寸」一番大きな画層グループ縮尺を適用
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
		else if (dwValue == 2) // ユーザが指定した縮尺を使用
		{
			// 変換縮尺を入力 2013.10.28 値ではなく画層グループ番号で指定する
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
	#endif
		// オフセットを用紙サイズから求めます
		// 0～4:A0～A4, 8:2A, 9:3A, 10:4A, 11:5A
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
	// 引数を設定、EXPORT用
	else
	{
		m_dJwwScale = dValue;
		m_pJwwOffset.x = pIn.x;
		m_pJwwOffset.y = pIn.y;
	}
}

// スケール取得
double CJwwHeader::GetDataScale()
{
	return m_dJwwScale;
}

// オフセット取得
void CJwwHeader::GetDataOffset(struct DPoint & pOut)
{
	pOut.x = m_pJwwOffset.x;
	pOut.y = m_pJwwOffset.y;
}

// 出力範囲設定
void CJwwHeader::SetMinMaxPt(const OdGePoint3d& min, const OdGePoint3d& max)
{
	m_pJwwMinPt = min;
	m_pJwwMaxPt = max;
}

// 出力範囲取得
void CJwwHeader::GetMinMaxPt(OdGePoint3d& min, OdGePoint3d& max) const
{
	min = m_pJwwMinPt;
	max = m_pJwwMaxPt;
}

// 入力用図面範囲
void CJwwHeader::GetDocumentArea(OdGePoint2d& min, OdGePoint2d& max) const
{
	// 左下(0,0)として用紙サイズと書き込み縮尺で図面範囲を設定します
	min.x = 0.0;
	min.y = 0.0;
	max.x = CJwwDocument::YOUSHI_SIZE[m_nZumen] * m_dJwwScale;
	max.y = CJwwDocument::YOUSHI_TATE[m_nZumen] * m_dJwwScale;
	// 用紙サイズが 12:10m, 13:50m, 14:100m のときは中心を(0,0)にします
	if (12 <= m_nZumen && m_nZumen <= 14)
	{
		max /= 2.0;
		min.x = -max.x;
		min.y = -max.y;
	}
}

// 文字角度補正設定を取得
void CJwwHeader::SetTextOrthoTol()
{
#ifdef linux
	m_bFixAng = 0;
	m_tFixAng = 1e-10;
#else
	// アプリケーション名を取得
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
#endif
}

// 文字角度補正設定を取得
BOOL CJwwHeader::GetTextOrthoTol(double& tVal)
{
	tVal = m_tFixAng;
	return m_bFixAng;
}
#ifdef _DEBUG
void CJwwHeader::Dump(CDumpContext &dc) const
{
	CObject::Dump(dc);
	// JWWのデータファイル宣言
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
// CData	: JWW 図形基本クラス
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
	return ((CJwDocument*)ar.m_pDocument)->GetVersion();
}

void	CData::DoDataScale(CArchive& ar, double &val)
{
	((CJwDocument*)ar.m_pDocument)->DoDataScale(val);
}

void	CData::DoDataScale(CArchive& ar, DPoint &pnt)
{
	((CJwDocument*)ar.m_pDocument)->DoDataScale(pnt);
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
	// ヘッダ情報へのポインタを保持
	/* ATTN
	CJwwDocument *pDoc;
	pDoc = static_cast<CJwwDocument*>(ar.m_pDocument);
	m_pHeader = &pDoc->m_JwwHeader;
	*/
	if (ar.IsStoring())
	{
		ar << m_lGroup;            //曲線属性番号
		ar << m_nPenStyle;          //線種番号
		ar << m_nPenColor;          //線色番号
		if (version(ar) >= eVer351)
		{
			ar << m_nPenWidth;      //線色幅
		}
		ar << m_nLayer;             //画層番号
		ar << m_nGLayer;            //画層グループ番号
		ar << m_sFlg;               //属性フラグ
	}
	else
	{
		ar >> m_lGroup;            //曲線属性番号
		ar >> m_nPenStyle;          //線種番号
		ar >> m_nPenColor;          //線色番号
		m_nPenWidth = 0;
		if (version(ar) >= eVer351)
		{
			ar >> m_nPenWidth;      //線色幅
		}
		ar >> m_nLayer;             //画層番号
		ar >> m_nGLayer;            //画層グループ番号
		ar >> m_sFlg;               //属性フラグ
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
// CDataSen : JWW 線データクラス
//****************************************

IMPLEMENT_SERIAL(CDataSen, CData, VERSIONABLE_SCHEMA);

void CDataSen::Serialize(CArchive& ar)
{
	CData::Serialize(ar);

	if (ar.IsStoring())
	{
		DoDataScale(ar, m_start);
		DoDataScale(ar, m_end);

		ar << m_start.x;
		ar << m_start.y;
		ar << m_end.x;
		ar << m_end.y;
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
	OdGePoint3d point(m_start.x, m_start.y, 0.0);
	return point;
}

OdGePoint3d	CDataSen::end(void) const
{
	OdGePoint3d point(m_end.x, m_end.y, 0.0);
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
// CDataEnko : JWW 円弧データクラス
//****************************************

IMPLEMENT_SERIAL(CDataEnko, CData, VERSIONABLE_SCHEMA);

void CDataEnko::Serialize(CArchive& ar)
{
	CData::Serialize(ar);

	if (ar.IsStoring())
	{
		DoDataScale(ar, m_start);
		DoDataScale(ar, m_dHankei);

		ar << m_start.x;
		ar << m_start.y;
		ar << m_dHankei;
		ar << m_radKaishiKaku;
		ar << m_radEnkoKaku;
		ar << m_radKatamukiKaku;
		ar << m_dHenpeiRitsu;
		ar << m_bZenEnFlg;
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

		//TRACE("円弧 x=%f, y=%f, r=%f, st=%f, size=%f, tilt=%f, ratio=%f, flg1=%d\n",
		//	m_start.x, m_start.y, m_dHankei,
		//	OdaToDegree(m_radKaishiKaku), OdaToDegree(m_radEnkoKaku), OdaToDegree(m_radKatamukiKaku),
		//	m_dHenpeiRitsu, m_bZenEnFlg);

		DoDataScale(ar, m_start);
		DoDataScale(ar, m_dHankei);
	}
}

OdGePoint3d	CDataEnko::center(void) const
{
	OdGePoint3d point(m_start.x, m_start.y, 0.0);
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
// CDataTen : JWW 点データクラス
//****************************************

IMPLEMENT_SERIAL(CDataTen, CData, VERSIONABLE_SCHEMA);

void CDataTen::Serialize(CArchive& ar)
{
	CData::Serialize(ar);

	if (ar.IsStoring())
	{
		DoDataScale(ar, m_start);
		DoDataScale(ar, m_dBairitsu);

		ar << m_start.x;
		ar << m_start.y;
		ar << m_bKariten;

		if (100 == m_nPenStyle)
		{
			ar << m_nCode;
			ar << m_radKaitenKaku;
			ar << m_dBairitsu;
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
		m_nPenStyle = 1;	// SHAPEを使用するなら不要

		DoDataScale(ar, m_start);
		DoDataScale(ar, m_dBairitsu);
	}
}

OdGePoint3d	CDataTen::position(void) const
{
	OdGePoint3d point(m_start.x, m_start.y, 0.0);
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
// 文字データクラス

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
		// 文字の寸法フラグは線幅で保存する
		m_nPenWidth = m_nSunpouFlg;
		// 図形共通データを保存
		CData::Serialize(ar);
		// 文字固有のデータを保存
		ar << m_start.x;
		ar << m_start.y;
		ar << m_end.x;
		ar << m_end.y;
		ar << m_nMojiShu;
		ar << m_dSizeX;
		ar << m_dSizeY;
		ar << m_dKankaku;
		ar << m_degKakudo;
		ar << CStringA(m_strFontName);
		ar << CStringA(m_string);
	}
	else
	{
		// 図形共通データを取得
		CData::Serialize(ar);
		// 文字の寸法フラグは線幅にある
		m_nSunpouFlg = m_nPenWidth;
		// 線幅はペン色から設定(0)にする
		m_nPenWidth = 0;
		// 文字固有のデータを取得
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
	OdGePoint3d point(m_start.x, m_start.y, 0.0);
	return point;
}

OdGePoint3d	CDataMoji::end(void) const
{
	OdGePoint3d point(m_end.x, m_end.y, 0.0);
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
			value.insert(0, OdString("@"));
		}
	}
	if (bold())
	{
		value += OdString(" Bold");
	}
	if (italic())
	{
		value += OdString(" Italic");
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
	// 角度補正(2016-12-15)
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
	// 縦書きフォントの設定は CData::m_flags に設定
	WORD flag = flags();
	if (value[0] == _T('@'))
	{
		value.deleteChars(0, 1);
		flags(flag | CData::eTate);
	}
	else
	{
		flags(flag & ~CData::eTate);
	}

	// 文字種の太字・斜体設定を削除(0～9999に切り詰める)
	m_nMojiShu %= CDataMoji::eMask;

	int pos = value.find(OdString(" Bold"));
	if (pos >= 0)
	{
		m_nMojiShu += CDataMoji::eFutoji;
		value.deleteChars(pos, 5);
	}
	pos = value.find(OdString(" Italic"));
	if (pos >= 0)
	{
		m_nMojiShu += CDataMoji::eShatai;
		value.deleteChars(pos, 7);
	}

	// フォント名を設定
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
// CDataSunpou : JWW 寸法データクラス
//****************************************

IMPLEMENT_SERIAL(CDataSunpou, CData, VERSIONABLE_SCHEMA);

void CDataSunpou::Serialize(CArchive& ar)
{
	CData::Serialize(ar);

	if (ar.IsStoring())
	{
		m_Sen.Serialize(ar);
		m_Moji.Serialize(ar);
		if (version(ar) >= eVer420)
		{
			ar << m_bSxfMode;
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
		if (version(ar) >= eVer420)
		{
			ar >> m_bSxfMode;
			m_SenHo1.Serialize(ar);
			m_SenHo2.Serialize(ar);
			m_Ten1.Serialize(ar);
			m_Ten2.Serialize(ar);
			m_TenHo1.Serialize(ar);
			m_TenHo2.Serialize(ar);
		}

		// 寸法図形のレイヤーとグループを修正
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
// CDataSolid	: JWW ソリッドデータクラス
//****************************************

IMPLEMENT_SERIAL(CDataSolid, CData, VERSIONABLE_SCHEMA);

void CDataSolid::Serialize(CArchive& ar)
{
	CData::Serialize(ar);

	if (ar.IsStoring())
	{
		// 普通のソリッドデータの場合
		if (m_nPenStyle < 101)
		{
			DoDataScale(ar, m_start);
			DoDataScale(ar, m_end);
			DoDataScale(ar, m_DPoint2);
			DoDataScale(ar, m_DPoint3);
		}
		// 円のソリッドデータの場合
		else
		{
			DoDataScale(ar, m_start);
			DoDataScale(ar, m_end.x);
			// ドーナツソリッドの場合
			if ((m_nPenStyle == 105) || (m_nPenStyle == 106))
			{
				DoDataScale(ar, m_DPoint3.y);
			}
		}

		ar << m_start.x;
		ar << m_start.y;
		ar << m_end.x;
		ar << m_end.y;
		ar << m_DPoint2.x;
		ar << m_DPoint2.y;
		ar << m_DPoint3.x;
		ar << m_DPoint3.y;

		// ペン番号が10のときは、任意の色が設定されている
		if (10 == m_nPenColor)
		{
			ar << m_Color;	// RGB
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

		// ペン番号が10のときは、任意の色が設定されている
		if (10 == m_nPenColor)
		{
			ar >> m_Color;	// RGB
		}

		// 普通のソリッドデータの場合
		if (m_nPenStyle < 101)
		{
			DoDataScale(ar, m_start);
			DoDataScale(ar, m_end);
			DoDataScale(ar, m_DPoint2);
			DoDataScale(ar, m_DPoint3);
		}
		// 円のソリッドデータの場合
		else
		{
			DoDataScale(ar, m_start);
			DoDataScale(ar, m_end.x);
			// ドーナツソリッドの場合
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
		point = OdGePoint3d(m_start.x, m_start.y, 0.0);
		break;
	case 1:
		point = OdGePoint3d(m_end.x, m_end.y, 0.0);
		break;
	case 2:
		point = OdGePoint3d(m_DPoint2.x, m_DPoint2.y, 0.0);
		break;
	case 3:
		point = OdGePoint3d(m_DPoint3.x, m_DPoint3.y, 0.0);
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
	OdGePoint3d point(m_start.x, m_start.y, 0.0);
	return point;
}

double CDataSolid::radius(void) const
{
	return m_end.x;	// 円環ソリッドでは外径
}

double  CDataSolid::inner(void) const
{
	return m_DPoint3.y; // 円環ソリッドの内径
}

double CDataSolid::flattenings(void) const
{
	return m_end.y;	// 扁平率
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
// CDataBlock	: JWW ブロックデータクラス
//****************************************

IMPLEMENT_SERIAL(CDataBlock, CData, VERSIONABLE_SCHEMA);

void CDataBlock::Serialize(CArchive& ar)
{
	CData::Serialize(ar);

	// リスト部へのポインタを保持
	// 2017/3/6 JWSファイルのブロックに対応
	CJwDocument *pDoc = (CJwDocument *)(ar.m_pDocument);
	m_pDataListList = pDoc->GetBlockList();

	if (ar.IsStoring())
	{
		DoDataScale(ar, m_DPKijunTen);

		ar << m_DPKijunTen.x;
		ar << m_DPKijunTen.y;
		ar << m_dBairitsuX;
		ar << m_dBairitsuY;
		ar << m_radKaitenKaku;
		ar << m_nNumber;
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

// データリスト取得
CDataList* CDataBlock::GetDataList()
{
#ifdef linux
	for(auto it = m_pDataListList->begin() ; it != m_pDataListList->end() ; it++ )
	{
		if( m_nNumber == (*it)->m_nNumber )
		{
			return *it;
		}
	}
#else	
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
#endif
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
	OdGePoint3d point(m_DPKijunTen.x, m_DPKijunTen.y, 0.0);
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
// CDataList : JWW データリストクラス
//****************************************

IMPLEMENT_SERIAL(CDataList, CData, VERSIONABLE_SCHEMA);

void CDataList::Serialize(CArchive& ar)
{
	CData::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << m_nNumber;
		ar << m_bReffered;
		// 2017/2/15 CTimeを出力すると64bit整数を出力してしまうが
		// JWWファイルでは32bit整数を出力すべきである
#ifdef linux
		ar << m_time;
#else	
		UINT32 tm32 = (UINT32)m_time.GetTime();
		ar << tm32;
#endif
		// 2017/2/15 V7.00なのでブロック名の後ろにSXFブロック種別が
		// ブロック名にないときは追加する(4:作図部品)
		if (m_strName.Find(CString("@@SfigorgFlag@@"), 0) < 0)
		{
			m_strName += CString("@@SfigorgFlag@@4");
		}
		// ブロック名はMBCS文字列として出力
		ar << CStringA(m_strName);
	}
	else
	{
		ar >> m_nNumber;
		ar >> m_bReffered;
		// MFCは32bit整数か64bit整数かを適切に判定して読み込んでくれる
		ar >> m_time;
		// MFCはMBCS文字列かUnicode文字列かを適切に判定して読み込んでくれる
		ar >> m_strName;
		// Ver.4.10 以降、名前の後ろに
		// "@@SfigorgFlag@@"に続けて、複合図形種別フラグを付加
		// 1:部分図(数学座標系)
		// 2:部分図(測地座標系)、
		// 3:作図グループ
		// 4:作図部品
	}
#if linux
#else
	m_DataList.Serialize(ar);
#endif
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
// CJwwDocument	: JWW ファイルクラス
//****************************************

CJwwDocument::CJwwDocument()
{
	// ヘッダーからドキュメントへの参照を初期化
	m_JwwHeader.m_pDoc = this;
	// 要素数の調査のときは画像を展開しない(既定はする)
	m_bReadForReport = FALSE;
	// ATTN:画層名の付け方
	m_bLayerName = FALSE;
	// ATTN:線種尺度係数
	m_lineFactor = 1.0;
	// ATTN:同じ番号の画層を作らない
	m_gMatchLayerNumber = TRUE;
}

IMPLEMENT_SERIAL(CJwwDocument, CDocument, VERSIONABLE_SCHEMA);

void CJwwDocument::ClearData()
{
#if linux
	m_DataList.clear();
	m_DataListList.clear();
#else
	while (!m_DataList.IsEmpty())
	{
		delete m_DataList.RemoveHead();
	}
	while (!m_DataListList.IsEmpty())
	{
		delete m_DataListList.RemoveHead();
	}
	m_JwwEmbbededImageFiles.clear();
#endif
}

BOOL CJwwDocument::OnNewDocument()
{
#if linux
	return TRUE;
#else	
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
#endif
}

CJwwDocument::~CJwwDocument()
{
}

#ifdef _WIN32
BEGIN_MESSAGE_MAP(CJwwDocument, CDocument)
END_MESSAGE_MAP()
#endif

void CJwwDocument::Serialize(CArchive& ar)
{
	// 画像ファイルの相対パスを解決するためにJWWファイルパスを記憶
	m_jwwPath = ar.GetFile()->GetFilePath();
	// JWWファイルに書き込み
	if (ar.IsStoring())
	{
		// ヘッダ書込
		m_JwwHeader.Serialize(ar);

		// 図形データの出力
		SetDepth(0);
		m_DataList.Serialize(ar);

		// 図形数調査のときは以下の処理を行わない
		if (m_bReadForReport)
			return;

		// ブロック図形の出力
		SetDepth(1);
		m_DataListList.Serialize(ar);

		// 埋め込み画像の出力
		//WriteImageFiles(ar);
	}
	// JWWファイルから読み込み
	else
	{
		ClearData();
		// ヘッダ読込
		m_JwwHeader.Serialize(ar);
		SetDepth(0);
		// 図形データの入力
		m_DataList.Serialize(ar);
		SetDepth(1);
		// ブロック図形の入力
		m_DataListList.Serialize(ar);
		// 埋め込み画像の展開を開始
		//ExtructImageFiles(ar);
	}
}

#if 0
// ***********************************************************
//   画像ファイル埋め込み
// ***********************************************************
//     DWORD nFiles;
//     -- nFiles 回繰り返し --
//     CSting embFile;
//     DWORD nFileSize;
//     BYTE  baFile[nFileSize];
//     ------------------------
// ***********************************************************
void CJwwDocument::WriteImageFiles(CArchive& ar)
{
	// 画像ファイルの個数を出力
	DWORD	nFiles = (DWORD)m_JwwEmbbededImageFiles.size();
	ar << nFiles;

	// 画像ファイルを圧縮し埋め込む
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
		// 画像ファイル と 圧縮ファイル のパス名を用意する
		//////////////////////////////////////////////////////
		// この集合に画像ファイルのパス名が入っている
		inFile = (*pos);
		// 一時ファイルディレクトリを取得
		GetTempPath(MAX_PATH, (LPTSTR)path);
		GetLongPathName(path, path2, MAX_PATH);
		// 圧縮ファイルのパス名＝一時ファイルディレクトリ＋入力ファイル(ファイルと拡張子)＋".gz"
		_tcscpy_s(path, MAX_PATH, inFile);
		outFile.Format(_T("%s%s.gz"), path2, PathFindFileName(path));
		//////////////////////////////////////////////////////
		// 圧縮ファイルを作成
		//////////////////////////////////////////////////////
		// 画像ファイルを読み込みオープン
		FILE* pFile = NULL;
		_tfopen_s(&pFile, inFile, _T("rb"));
		if (pFile == NULL)
		{
			//printf(_T("\n画像ファイルオープンエラー(%d) : %s"), errno, (LPCTSTR)inFile );
			continue;
		}
		// 圧縮ファイルを書き込みオープン
		memset(buf, 0, BUFSIZ);
		WideCharToMultiByte(CP_ACP, 0, outFile, outFile.GetLength(), (LPSTR)(LPVOID)buf, BUFSIZ, NULL, NULL);
		gzFile	zFile = gzopen((LPCSTR)buf, "wb");
		if (zFile == NULL)
		{
			//printf(_T("\n圧縮ファイルオープンエラー(%d) : %s"), errno, (LPCTSTR)outFile );
			fclose(pFile);
			continue;
		}
		// 画像ファイルをgzipで圧縮
		bool	bFileError = false;
		while (!feof(pFile))
		{
			nReadSize = (UINT)fread(buf, 1, BUFSIZ, pFile);
			if (ferror(pFile))
			{
				//printf(_T("\n画像ファイルリードエラー : %s"), (LPCTSTR)inFile );
				bFileError = true;
				break;
			}
			nWriteSize = gzwrite(zFile, buf, nReadSize);
			if (nWriteSize < 0)
			{
				//printf(_T("\n圧縮ファイルライトエラー : %s"), (LPCTSTR)outFile );
				bFileError = true;
				break;
			}
		}
		// ファイルを閉じる
		fclose(pFile);
		gzclose(zFile);
		//////////////////////////////////////////////////////
		// 圧縮ファイルをJWWファイルに追加する
		//////////////////////////////////////////////////////
		if (!bFileError)
		{
			CFile	gz(outFile, CFile::modeRead | CFile::typeBinary);
			// 圧縮ファイル名(ディレクトリなし)を図面ファイルにコピーする
			_tcscpy_s(path, MAX_PATH, outFile);
			embFile = PathFindFileName(path);
			ar << CStringA(embFile);
			// ファイルサイズを図面ファイルにコピーする
			nFileSize = (UINT)gz.GetLength();
			ar << nFileSize;
			// 圧縮データを図面ファイルにコピーする
			while (nFileSize > 0)
			{
				nReadSize = gz.Read(buf, BUFSIZ);
				ar.Write(buf, nReadSize);
				nFileSize -= nReadSize;
			}
			gz.Close();
		}
		// 圧縮ファイルを削除
		DeleteFile(outFile);
	}
}

// 画像ファイル展開
void CJwwDocument::ExtructImageFiles(CArchive& ar)
{
	// JWWファイルバージョンを確認
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

	// 出力先がなかったらJWWのある位置で代用
	if (m_extructDir.IsEmpty())
	{
		_tcscpy_s(path, MAX_PATH, (LPCTSTR)m_jwwPath);
		PathRemoveFileSpec(path);
		m_extructDir = path;
	}
	// 添付ファイル数で繰り返し
	for (ar >> nFiles; nFiles > 0; --nFiles)
	{
		// 画像ファイル名(パスなし、拡張子あり)を取得
		ar >> strArchiveFile;
		// ファイルサイズを取得
		ar >> nFileSize;
		ASSERT(nFileSize);
		if (nFileSize == 0)
		{
			continue;
		}
		// 画像ファイルの完全パスを作成
		bExtruct = false;
		_tcscpy_s(path, MAX_PATH, m_extructDir);
		PathAppend(path, strArchiveFile);
		if (_tcsicmp(PathFindExtension(path), _T(".gz")) == 0)
		{
			bExtruct = true;
			PathRemoveExtension(path); // なんとか.bmp.gz -> なんとか.bmp
		}
		strImageFile = path;
		// 一時ファイル用ディレクトリ名をファイル名に追加
		GetTempPath(MAX_PATH, (LPTSTR)path2);
		GetLongPathName(path2, path, MAX_PATH);
		PathAppend(path, strArchiveFile);
		strArchiveFile = path;
		// 圧縮ファイルを一時ディレクトリに作成
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
			// 画像ファイルを作成
			//////////////////////////////////////////////////////
			bool	bFileError = false;
			FILE* pFile = NULL;
			_tfopen_s(&pFile, strImageFile, _T("wb"));
			if (pFile == NULL)
			{
				//printf(_T("\n画像ファイルオープンエラー(%d) : %s"), errno, (LPCTSTR)strImageFile );
				continue;
			}
			// 圧縮ファイルを読み込みオープン
			memset(buf, 0, BUFSIZ);
			WideCharToMultiByte(CP_ACP, 0, strArchiveFile, strArchiveFile.GetLength(), (LPSTR)(LPVOID)buf, BUFSIZ, NULL, NULL);
			gzFile	zFile = gzopen((LPCSTR)buf, "rb");
			if (zFile == NULL)
			{
				//printf(_T("\n圧縮ファイルオープンエラー(%d) : %s"), errno, (LPCTSTR)strArchiveFile );
				fclose(pFile);
				continue;
			}
			// 圧縮ファイルから画像ファイルを取得
			INT		zReadSize;
			while (!gzeof(zFile))
			{
				zReadSize = gzread(zFile, buf, BUFSIZ);
				if (zReadSize < 0)
				{
					//printf(_T("\n圧縮ファイルリードエラー : %s"), (LPCTSTR)strArchiveFile );
					bFileError = true;
					break;
				}
				fwrite(buf, 1, zReadSize, pFile);
				if (ferror(pFile))
				{
					//printf(_T("\n画像ファイルライトエラー : %s"), (LPCTSTR)strImageFile );
					bFileError = true;
					break;
				}
			}
			// ファイルを閉じる
			fclose(pFile);
			gzclose(zFile);
			// 一時ファイルを削除
			DeleteFile(strArchiveFile);
			// 展開エラー時は半端な画像ファイルも削除
			if (bFileError)
			{
				DeleteFile(strImageFile);
			}
		}
		// 非圧縮の画像ファイルは図面と同じディレクトリに作成
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
#endif

#ifdef _DEBUG
void CJwwDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CJwwDocument::Dump(class CDumpContext & dc) const
{
	dc << "\nBlocks " << m_DataListList.GetCount();
	dc << "\nEntities " << m_DataList.GetCount();
	dc.Flush();
}
#endif	//_DEBUG

// 縮尺と変位の適用と初期化
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

// RAY,XLINEの出力範囲の計算に使う
void CJwwDocument::SetMinMaxPt(const OdGePoint3d& min, const OdGePoint3d& max)
{
	m_JwwHeader.SetMinMaxPt(min, max);
}

void CJwwDocument::GetMinMaxPt(OdGePoint3d& min, OdGePoint3d& max) const
{
	m_JwwHeader.GetMinMaxPt(min, max);
}

// 入力用図面範囲
void CJwwDocument::GetDocumentArea(OdGePoint2d& min, OdGePoint2d& max) const
{
	m_JwwHeader.GetDocumentArea(min, max);
}

// 主図形の数
INT_PTR	CJwwDocument::GetDataCount() const
{
#ifdef linux
	return m_DataList.size();
#else
	return m_DataList.GetCount();
#endif
}

// 主図形の先頭
POSITION CJwwDocument::GetHeadPosition() const
{
#if linux	
	return 0;
#else
	return m_DataList.GetHeadPosition();
#endif
}

// バージョン番号
DWORD CJwwDocument::GetVersion() const
{
	return m_JwwHeader.m_Version;
}
