#pragma once

#include "common.h"

class CMainLayer;
class CCalcLayer;
class CSendToKakaoLayer;

class CDataSingleton {
private:
	static CDataSingleton* m_pInstance;
	static CMainLayer*		m_pMainLayer;
	static CCalcLayer*		m_pCalcLayer;
	static CSendToKakaoLayer* m_pSendTOKakaoLayer;

public:
	CDataSingleton();
	~CDataSingleton();

	static CDataSingleton*	getInstance();
	static CMainLayer*		getMainLayer();
	static void				setMainLayer(CMainLayer* pCalcLayer);
	static CCalcLayer*		getCalcLayer();
	static void				setCalcLayer(CCalcLayer* pCalcLayer);
	static CSendToKakaoLayer*		getSendToKakaoLayer();
	static void						setSendToKakaoLayer(CSendToKakaoLayer* pCSendToKakaoLayer);

	static std::string name;
	static std::string price;
};
