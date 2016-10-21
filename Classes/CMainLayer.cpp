#include "CMainLayer.h"
#include "CCalcLayer.h"

void CMainLayer::onTouchesBegan(const std::vector<cocos2d::Touch*>&pTouches, cocos2d::Event* pEvent) {
	if (m_TouchMode == NORMAL_STATE) {
		if (!m_bTouchStarted) {
			Touch* pTouch = (Touch*)pTouches.back();
			Point point = pTouch->getLocation();

			// 종료버튼 스택 초기화
			m_EndKeyStack = 0;

			// 스크롤인 경우
			if (isInMainRect(point)) m_gestureStartRect = MAIN_RECT;
			if (isInSliderColsRect(point)) m_gestureStartRect = SLIDER_COLS_RECT;
			if (isInSliderRowsRect(point)) m_gestureStartRect = SLIDER_ROWS_RECT;
			if (isInResetPositionRect(point)) m_gestureStartRect = RESET_POSITION_RECT;

			m_gestureStartPoint.setPoint(point.x, point.y);
			m_gestureStartOrigin.setPoint(point.x, point.y);
			m_bTouchStarted = true;

			// 위치 리셋인 경우
			if (isInResetPositionRect(point))
				resetPosition();
			// 계산하기 버튼 클릭 시
			if (isInCalcButtonRect(point)) {
				calcData();
				auto scene = CCalcLayer::createScene();
				Director::getInstance()->pushScene(scene);
			}
		}
	}
}
void CMainLayer::onTouchesEnded(const std::vector<cocos2d::Touch*>&pTouches, cocos2d::Event* pEvent) {
	if (m_TouchMode == NORMAL_STATE) {
		if (m_bTouchStarted) {
			Touch* pTouch = (Touch*)pTouches.back();
			Point point = pTouch->getLocation();

			// 단순 클릭인 경우
			if (abs(m_gestureStartOrigin.x - point.x)<TOUCH_RANGE &&
				abs(m_gestureStartOrigin.y - point.y)<TOUCH_RANGE) {
				// 메인 셀 클릭 시
				if (isInMainRect(point)) {
					pickCell(point);
				}
				// 세로 슬라이더 클릭 시
				if (isInSliderRowsRect(point)) {
					pickSliderRowsCell(point);
				}
				// 가로 슬라이더 클릭 시
				if (isInSliderColsRect(point)) {
					pickSliderColsCell(point);
				}
			}
			m_bTouchStarted = false;
			m_bTouchMoved = false;
		}
	}
}