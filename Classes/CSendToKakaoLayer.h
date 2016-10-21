#pragma once

#include "CCalcLayer.h"
#include "CResultSlider.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

using namespace cocos2d;

class CSendToKakaoLayer : public CCalcLayer {
	// [ 기본 ]
	// ==============================================================================================================
public:
	static cocos2d::Scene* createScene() {
		auto pScene = Scene::create();
		auto pLayer = CSendToKakaoLayer::create();

		pScene->addChild(pLayer);

		return pScene;
	}
	virtual bool init() {
		if (!Layer::init()) { return false; }

		// 배경 이미지 출력
		cocos2d::Sprite* pBackgroundSprite = cocos2d::Sprite::create("background.png");
		pBackgroundSprite->setPosition(cocos2d::CCPointZero);
		pBackgroundSprite->setAnchorPoint(ccp((float)0, (float)0));
		pBackgroundSprite->setPosition(ccp((float)0, (float)0));
		addChild(pBackgroundSprite, 0);

		pBackgroundSprite = cocos2d::Sprite::create("backGroundButtom3.png");
		pBackgroundSprite->setAnchorPoint(ccp((float)0, (float)0));
		pBackgroundSprite->setPosition(ccp((float)0, (float)0));
		addChild(pBackgroundSprite, 5);

		// 터치 초기화
		EventListenerTouchAllAtOnce* listener = EventListenerTouchAllAtOnce::create();
		listener->onTouchesBegan = CC_CALLBACK_2(CSendToKakaoLayer::onTouchesBegan, this);
		listener->onTouchesMoved = CC_CALLBACK_2(CSendToKakaoLayer::onTouchesMoved, this);
		listener->onTouchesEnded = CC_CALLBACK_2(CSendToKakaoLayer::onTouchesEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
		m_gestureStartRect = 0;
		m_bTouchStarted = false;
		m_bTouchMoved = false;
		m_TouchMode = NORMAL_STATE;

		auto p = CDataSingleton::getCalcLayer();
		for (int j = 0; j < MAX_COLS; j++) {
			int x = 0;
			int y = DESIGN_HEIGHT - 150 * j;

			bool mode = false;
			if (p->getResultName(j) != "") mode = true;
			CResultSlider* pCResultSlider = CResultSlider::Create(mode);
			pCResultSlider->setPosition(cocos2d::CCPointZero);
			pCResultSlider->setAnchorPoint(ccp((float)0, (float)0));
			pCResultSlider->setPosition(ccp((float)x, (float)y));
			pCResultSlider->setX(x);
			pCResultSlider->setY(y);
			pCResultSlider->setText(p->getResultName(j));
			pCResultSlider->setTextLabel();

			addChild(pCResultSlider, 3);
			addChild(pCResultSlider->getTextLabel(), 4);

			m_ResultSlider[j] = pCResultSlider;
		}

		// 키보드 초기화
		this->setKeypadEnabled(true);
		m_EndKeyStack = 0;

		CDataSingleton::setSendToKakaoLayer(this);
		m_winSize = Director::sharedDirector()->getWinSize();
		return true;
	}
	CREATE_FUNC(CSendToKakaoLayer);

protected:
	CResultSlider*	m_ResultSlider[MAX_COLS];
	// ==============================================================================================================






	// [ 터치 제어 ]
	// ==============================================================================================================
protected:
	void onTouchesBegan(const std::vector<cocos2d::Touch*>&pTouches, cocos2d::Event* pEvent) {
		if (m_TouchMode == NORMAL_STATE) {
			if (!m_bTouchStarted) {
				Touch* pTouch = (Touch*)pTouches.back();
				Point point = pTouch->getLocation();

				// 종료버튼 스택 초기화
				m_EndKeyStack = 0;

				// 스크롤인 경우
				if (isInMainRect(point)) m_gestureStartRect = MAIN_RECT;

				m_gestureStartPoint.setPoint(point.x, point.y);
				m_gestureStartOrigin.setPoint(point.x, point.y);
				m_bTouchStarted = true;
			}
		}
	}
	void onTouchesMoved(const std::vector<cocos2d::Touch*>&pTouches, cocos2d::Event* pEvent) {
		if (m_TouchMode == NORMAL_STATE) {
			if (m_bTouchStarted) {
				Touch* pTouch = (Touch*)pTouches.back();
				Point point = pTouch->getLocation();

				m_bTouchMoved = true;

				if (m_gestureStartRect == MAIN_RECT && isInMainRect(point))
					Scroll(point);
				else
					m_bTouchStarted = false;
				m_gestureStartPoint.setPoint(point.x, point.y);
			}
		}
	}
	void onTouchesEnded(const std::vector<cocos2d::Touch*>&pTouches, cocos2d::Event* pEvent) {
		if (m_bTouchStarted) {
			Touch* pTouch = (Touch*)pTouches.back();
			Point point = pTouch->getLocation();

			// 단순 클릭인 경우
			if (abs(m_gestureStartOrigin.x - point.x)<TOUCH_RANGE &&
				abs(m_gestureStartOrigin.y - point.y)<TOUCH_RANGE) {
				// 뒤로 버튼 클릭 시
				if (isInBackButtonRect(point)) {
					popThisScene();
				}
				// 한번에 보내기 클릭 시
				if (isInSendAllButtonRect(point)) {
					auto p = CDataSingleton::getCalcLayer();
					kakaoLink(p->getAllResultText());
				}
				if (isInMainRect(point)) {
					pickSendButton(point);
				}
			}
			m_bTouchStarted = false;
			m_bTouchMoved = false;
		}
	}

	void popThisScene() {


		Director::getInstance()->popScene();
	}

protected:
	void Scroll(Point point) {
		int x = 0;
		int y = point.y - m_gestureStartPoint.y;
		if (m_ResultSlider[0]->getY() + y <= DESIGN_HEIGHT) y = 0;
		if (m_ResultSlider[MAX_COLS - 1]->getY() + y >= 200) y = 0;
		for (int i = 0; i < MAX_COLS; i++)
			m_ResultSlider[i]->Scroll(x, y);
	}

	bool isInMainRect(Point point) {
		if (point.x < 0 || point.x > DESIGN_WIDTH) return false;
		if (point.y < BUTTOM_HEIGHT || point.y > DESIGN_HEIGHT) return false;
		return true;
	}
	bool isInBackButtonRect(Point point) {
		if (point.x < 0 || point.x > 197) return false;
		if (point.y < 0 || point.y > 200) return false;
		return true;
	}
	bool isInSendAllButtonRect(Point point) {
		if (point.x < 353 || point.x > 725) return false;
		if (point.y < 0 || point.y > 200) return false;
		return true;
	}
	bool isInSendButtonRect(Point point, int y) {
		if (point.x < 712 || point.x > 1053) return false;
		if (point.y < y + 30 || point.y > y + 120) return false;
		return true;
	}
	// ==============================================================================================================






	// [ 데이터 제어 ]
	// ==============================================================================================================
public:
	void kakaoLink(std::string str) {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		JniMethodInfo t;
		bool isHave = JniHelper::getStaticMethodInfo(t
			, "org/cocos2dx/cpp/AppActivity"
			, "getJavaActivity"
			, "()Ljava/lang/Object;");

		jobject activityObj;


		if (isHave)
		{
			activityObj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
		}

		isHave = JniHelper::getMethodInfo(t
			, "org.cocos2dx/cpp/AppActivity"
			, "kakaoLink"
			, "(Ljava/lang/String;)V");
		if (isHave)
		{
			//const char *cstr = "zzzz\nzzzz";
			//std::string str = "zzzzz\nzzzz";
			const char *cstr = str.c_str();
			jstring jmsg = t.env->NewStringUTF(cstr);
			//jstring jmsg = t.env->NewStringUTF(str);
			t.env->CallVoidMethod(activityObj, t.methodID, jmsg);
		}
#endif
	}

protected:
	void pickSendButton(Point point) {
		auto p = CDataSingleton::getCalcLayer();
		for (int j = 0; j < MAX_ROWS; j++) {
			int y = m_ResultSlider[j]->getY();
			if (isInSendButtonRect(point, y) && m_ResultSlider[j]->getText() != "") {
				std::string str = "뿜빠이 계산결과\n";
				str += p->getResultText(j);
				kakaoLink(str);
			}
		}
	}
};