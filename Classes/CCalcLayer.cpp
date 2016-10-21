#include "CCalcLayer.h"
#include "CSendToKakaoLayer.h"

void CCalcLayer::onTouchesBegan(const std::vector<cocos2d::Touch*>&pTouches, cocos2d::Event* pEvent) {
	if (m_TouchMode == NORMAL_STATE) {
		if (!m_bTouchStarted) {
			Touch* pTouch = (Touch*)pTouches.back();
			Point point = pTouch->getLocation();

			// 종료버튼 스택 초기화
			m_EndKeyStack = 0;

			// 스크롤인 경우
			if (isInMainRect(point)) m_gestureStartRect = MAIN_RECT;
			if (isInSliderColsRect(point)) m_gestureStartRect = SLIDER_COLS_RECT;
			if (isInResetPositionRect(point)) m_gestureStartRect = RESET_POSITION_RECT;

			m_gestureStartPoint.setPoint(point.x, point.y);
			m_gestureStartOrigin.setPoint(point.x, point.y);
			m_bTouchStarted = true;

			// 위치 리셋인 경우
			if (isInResetPositionRect(point))
				resetPosition();
			// 뒤로 버튼 클릭 시
			if (isInBackButtonRect(point)) {
				popThisScene();
			}
			// 카카오톡으로 전송 버튼 클릭 시
			if (isInNextButtonRect(point)) {
				sort();
				std::string str = "[뿜빠이]";
				str += m_AllResultText;
				kakaoLink(str);
			}
		}
	}
}
void CCalcLayer::onTouchesEnded(const std::vector<cocos2d::Touch*>&pTouches, cocos2d::Event* pEvent) {
	if (m_TouchMode == NORMAL_STATE) {
		if (m_bTouchStarted) {
			Touch* pTouch = (Touch*)pTouches.back();
			Point point = pTouch->getLocation();

			m_bTouchStarted = false;
			m_bTouchMoved = false;
		}
	}
}