#include "CDataSingleton.h"
#include "CMainLayer.h"
#include "CCalcLayer.h"
#include "CSendToKakaoLayer.h"

USING_NS_CC;

CDataSingleton::CDataSingleton() {}
CDataSingleton::~CDataSingleton() {}

CDataSingleton* CDataSingleton::getInstance() {
	if (!m_pInstance) {
		m_pInstance = new CDataSingleton;
	}
	return m_pInstance;
}


CMainLayer*		CDataSingleton::getMainLayer() {
	return m_pMainLayer;
}
void CDataSingleton::setMainLayer(CMainLayer* pMainLayer) {
	m_pMainLayer = pMainLayer;
}
CCalcLayer*		CDataSingleton::getCalcLayer() {
	return m_pCalcLayer;
}
void CDataSingleton::setCalcLayer(CCalcLayer* pCalcLayer) {
	m_pCalcLayer = pCalcLayer;
}
CSendToKakaoLayer*		CDataSingleton::getSendToKakaoLayer() {
	return m_pSendTOKakaoLayer;
}
void					CDataSingleton::setSendToKakaoLayer(CSendToKakaoLayer* pCSendToKakaoLayer) {
	m_pSendTOKakaoLayer = pCSendToKakaoLayer;
}

CDataSingleton* CDataSingleton::m_pInstance = NULL;
CMainLayer* CDataSingleton::m_pMainLayer = NULL;
CCalcLayer* CDataSingleton::m_pCalcLayer = NULL;
CSendToKakaoLayer* CDataSingleton::m_pSendTOKakaoLayer = NULL;
std::string CDataSingleton::name = "";
std::string CDataSingleton::price = "";


