#pragma once

#include "CMainLayer.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

using namespace cocos2d;

class CSendToKakaoLayer;

class CCalcLayer : public CMainLayer {
	// [ 기본 ]
	// ==============================================================================================================
public:
	static cocos2d::Scene* createScene() {
		auto pScene = Scene::create();
		auto pLayer = CCalcLayer::create();

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


		// 레이아웃 출력
		pBackgroundSprite = cocos2d::Sprite::create("backGroundTop.png");
		pBackgroundSprite->setAnchorPoint(ccp((float)0, (float)0));
		pBackgroundSprite->setPosition(ccp((float)CELL_WIDTH, (float)DESIGN_HEIGHT - CELL_HEIGHT));
		addChild(pBackgroundSprite, 1);

		pBackgroundSprite = cocos2d::Sprite::create("backGroundLeft.png");
		pBackgroundSprite->setAnchorPoint(ccp((float)0, (float)0));
		pBackgroundSprite->setPosition(ccp((float)0, (float)BUTTOM_HEIGHT));
		addChild(pBackgroundSprite, 1);


		pBackgroundSprite = cocos2d::Sprite::create("backGroundResetPosition.png");
		pBackgroundSprite->setAnchorPoint(ccp((float)0, (float)0));
		pBackgroundSprite->setPosition(ccp((float)0, (float)DESIGN_HEIGHT - CELL_HEIGHT));
		addChild(pBackgroundSprite, 5);

		pBackgroundSprite = cocos2d::Sprite::create("backGroundButtom2.png");
		pBackgroundSprite->setAnchorPoint(ccp((float)0, (float)0));
		pBackgroundSprite->setPosition(ccp((float)0, (float)0));
		addChild(pBackgroundSprite, 5);


		// 메인셀 출력
		for (int i = 0; i < MAX_COLS; i++) {
			for (int j = 0; j < MAX_ROWS; j++) {
				int x = CELL_WIDTH + CELL_WIDTH * i;
				int y = DESIGN_HEIGHT - CELL_HEIGHT * 2 - CELL_HEIGHT * j;

				CCellObject* pCellObject = CCellObject::Create(CALC_MODE);
				pCellObject->setPosition(cocos2d::CCPointZero);
				pCellObject->setAnchorPoint(ccp((float)0, (float)0));
				pCellObject->setPosition(ccp((float)x, (float)y));
				addChild(pCellObject, 0);

				pCellObject->setX(x);
				pCellObject->setY(y);
				pCellObject->setType(CALC_MODE, 1);
				m_Board[i][j] = pCellObject;
			}
		}

		// 슬라이더 출력
		for (int i = 0; i < MAX_COLS; i++) {
			int x = CELL_WIDTH + CELL_WIDTH * i;
			int y = DESIGN_HEIGHT - CELL_HEIGHT;

			CCellSlider* pCCellSlider = CCellSlider::Create(SLIDER_COLS_TYPE);
			pCCellSlider->setPosition(cocos2d::CCPointZero);
			pCCellSlider->setAnchorPoint(ccp((float)0, (float)0));
			pCCellSlider->setPosition(ccp((float)x, (float)y));
			addChild(pCCellSlider, 3);

			pCCellSlider->setX(x);
			pCCellSlider->setY(y);
			pCCellSlider->SetType(SLIDER_COLS_TYPE);
			m_SliderCols[i] = pCCellSlider;
		}
		for (int j = 0; j < MAX_ROWS; j++) {
			int x = 0;
			int y = DESIGN_HEIGHT - CELL_HEIGHT * 2 - CELL_HEIGHT * j;

			CCellSlider* pCCellSlider = CCellSlider::Create(SLIDER_ROWS_TYPE);
			pCCellSlider->setPosition(cocos2d::CCPointZero);
			pCCellSlider->setAnchorPoint(ccp((float)0, (float)0));
			pCCellSlider->setPosition(ccp((float)x, (float)y));
			addChild(pCCellSlider, 3);

			pCCellSlider->setX(x);
			pCCellSlider->setY(y);
			pCCellSlider->SetType(SLIDER_ROWS_TYPE);
			m_SliderRows[j] = pCCellSlider;
		}

		// 셀 내용 채우기
		std::string str[5] = { "몫", "낸 돈", " ", "낼 돈", "받을 돈" };
		for (int j = 0; j < 5; j++) {
			m_SliderRows[j]->setName(str[j]);
			removeChild(m_SliderRows[j]->getPriceLabel(), true);
			m_SliderRows[j]->setPriceLabel();
			addChild(m_SliderRows[j]->getPriceLabel(), 4);
		}
		for (int i = 0; i < MAX_COLS; i++) {
			CMainLayer* pMainLayer = CDataSingleton::getMainLayer();
			CCellSlider* pCellSlider = pMainLayer->getSliderCols(i);

			std::string str = pCellSlider->getName();
			if (str == "" && pMainLayer->getShare(i) != 0.0f) {
				str = "친구"; str += std::to_string(i + 1);
				pMainLayer->setSliderColsName(str, i);
			}
			m_SliderCols[i]->setName(str);
			removeChild(m_SliderCols[i]->getPriceLabel(), true);
			m_SliderCols[i]->setPriceLabel();
			addChild(m_SliderCols[i]->getPriceLabel(), 4);
		}


		sort();
		debug();

		for (int i = 0; i < MAX_COLS; i++) {
			CMainLayer* pMainLayer = CDataSingleton::getMainLayer();


			for (int j = 0; j < 5; j++) {
				if (j == 0) {
					m_Board[i][j]->setPrice(pMainLayer->getShare(i));
				}
				else if (j == 1) {
					m_Board[i][j]->setPrice(pMainLayer->getPayed(i));
				}
				else if (j == 3) {
					float price = pMainLayer->getShare(i) - pMainLayer->getPayed(i);
					if (price > 0) {
						int temp = (price / 100.0f);
						price = temp * 100;
						m_Board[i][j]->setPrice(price);
					}
				}
				else if (j == 4) {
					//m_Board[i][4]->setPrice(price);
				}
				removeChild(m_Board[i][j]->getPriceLabel(), true);
				m_Board[i][j]->setPriceLabel();
				addChild(m_Board[i][j]->getPriceLabel(), 2);
			}
		}

		// 터치 초기화
		EventListenerTouchAllAtOnce* listener = EventListenerTouchAllAtOnce::create();
		listener->onTouchesBegan = CC_CALLBACK_2(CCalcLayer::onTouchesBegan, this);
		listener->onTouchesMoved = CC_CALLBACK_2(CCalcLayer::onTouchesMoved, this);
		listener->onTouchesEnded = CC_CALLBACK_2(CCalcLayer::onTouchesEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
		m_gestureStartRect = 0;
		m_bTouchStarted = false;
		m_bTouchMoved = false;
		m_TouchMode = NORMAL_STATE;


		// 키보드 초기화
		this->setKeypadEnabled(true);
		m_EndKeyStack = 0;

		CDataSingleton::setCalcLayer(this);
		m_winSize = Director::sharedDirector()->getWinSize();
		return true;
	}
	CREATE_FUNC(CCalcLayer);

	std::string getNameOfShare(int i) { return m_nameOfShare[i]; }
	std::string getResultName(int i) { return m_resultName[i]; }
	std::string getResultText(int i) { return m_resultText[i]; }
	std::string getAllResultText() { return m_AllResultText; }

protected:
	float m_sortedPayed[MAX_COLS];
	float m_sortedShare[MAX_COLS];
	std::string m_nameOfPayed[MAX_COLS];
	std::string m_nameOfShare[MAX_COLS];
	std::string m_resultName[MAX_COLS];
	std::string m_resultText[MAX_COLS];
	std::string m_AllResultText;

	

	void sort() {
		for (int i = 0; i < MAX_COLS; i++) {
			CMainLayer* pMainLayer = CDataSingleton::getMainLayer();
			float price = pMainLayer->getShare(i) - pMainLayer->getPayed(i);
			if (price > 0) {
				m_sortedPayed[i] = price;
			} else m_sortedPayed[i] = 0;
			price = pMainLayer->getPayed(i) - pMainLayer->getShare(i);
			if (price > 0) {
				m_sortedShare[i] = price;
			} else m_sortedShare[i] = 0;
			m_nameOfPayed[i] = pMainLayer->getSliderCols(i)->getName();
			m_nameOfShare[i] = pMainLayer->getSliderCols(i)->getName();

			quickSort(MAX_COLS - 1, 0, m_sortedPayed, m_nameOfPayed);
			quickSort(MAX_COLS - 1, 0, m_sortedShare, m_nameOfShare);
		}
	}
	void quickSort(int top, int bottom, float* arr, std::string* strArr) {
		int middle;
		if (top < bottom) {
			middle = partition(top, bottom, arr, strArr);
			quickSort(top, middle, arr, strArr);
			quickSort(middle + 1, bottom, arr, strArr);
		}
		return;
	}
	int partition(int top, int bottom, float* arr, std::string* strArr) {
		int x = arr[top];
		int i = top - 1;
		int j = bottom + 1;
		float temp;
		std::string tempStr;
		do {
			do {
				j--;
			} while (x <arr[j]);

			do {
				i++;
			} while (x >arr[i]);

			if (i < j) {
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
				tempStr = strArr[i];
				strArr[i] = strArr[j];
				strArr[j] = tempStr;
			}
		} while (i < j);
		return j; 
	}
	void debug() {
		std::string str;
		for (int i = 0; i < MAX_COLS; i++) {
			int sum2 = 0;
			for (int j = 0; j < MAX_COLS; j++) {
				if (i != j) {
					int sum = 0;
					while (m_sortedShare[i] > 0 && m_sortedPayed[j] > 0) {
						m_sortedShare[i] -= 1;
						m_sortedPayed[j] -= 1;
						sum += 1;
					}
					sum /= 100;
					sum *= 100;
					sum2 += sum;

					if (sum == 0) continue;
					if (m_nameOfShare[j] == "") continue;
					if (m_nameOfPayed[i] == "") continue;
					std::string temp = "\n";
					temp += m_nameOfShare[j];
					temp += " -> ";
					temp += m_nameOfPayed[i];
					temp += "  :  ";
					temp += std::to_string(sum);
					str += temp;
				}
				if (m_nameOfPayed[i] == "") continue;
			}
			m_Board[i][4]->setPrice(sum2);
		}
		m_AllResultText = str;
		
		/*
		LabelTTF* label = LabelTTF::create(str, "Arial", 40,
			Size(1080, 1920), TextHAlignment::CENTER, TextVAlignment::CENTER);
		label->setAnchorPoint(ccp((float)0.5f, (float)0.5f));
		label->setPosition(ccp(1080 / 2, 1920 / 2));
		label->setColor(Color3B(0, 0, 0));
		addChild(label, 5);
		*/
		
	}
	// ==============================================================================================================




	// [ 터치 제어 ]
	// ==============================================================================================================
protected:
	void onTouchesBegan(const std::vector<cocos2d::Touch*>&pTouches, cocos2d::Event* pEvent);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>&pTouches, cocos2d::Event* pEvent) {
		if (m_TouchMode == NORMAL_STATE) {
			if (m_bTouchStarted) {
				Touch* pTouch = (Touch*)pTouches.back();
				Point point = pTouch->getLocation();

				m_bTouchMoved = true;

				if ((m_gestureStartRect == MAIN_RECT && isInMainRect(point)) ||
					(m_gestureStartRect == SLIDER_COLS_RECT && isInSliderColsRect(point))) {
					Scroll(point);
				}
				else
					m_bTouchStarted = false;
				m_gestureStartPoint.setPoint(point.x, point.y);
			}
		}
	}
	void onTouchesEnded(const std::vector<cocos2d::Touch*>&pTouches, cocos2d::Event* pEvent);

	void popThisScene() {
		// 변수 초기화
		CMainLayer* pMainLayer = CDataSingleton::getMainLayer();
		for (int i = 0; i < MAX_COLS; i++) {
			pMainLayer->setShare(0.0f, i);
			pMainLayer->setPayed(0.0f, i);
		}
		pMainLayer->resetBoard();
		Director::getInstance()->popScene();
	}

protected:
	void Scroll(Point point) {
		int x = point.x - m_gestureStartPoint.x;
		int y = 0;
		if (m_Board[0][0]->getX() + x >= CELL_WIDTH) x = 0;


		for (int i = 0; i < MAX_COLS; i++) {
			for (int j = 0; j < MAX_ROWS; j++) {
				m_Board[i][j]->Scroll(x, y);
			}
		}
		for (int i = 0; i < MAX_COLS; i++)
			m_SliderCols[i]->Scroll(x, y);
	}

	bool isInBackButtonRect(Point point) {
		if (point.x < 0 || point.x > 197) return false;
		if (point.y < 0 || point.y > 200) return false;
		return true;
	}
	bool isInNextButtonRect(Point point) {
		if (point.x < 382 || point.x > 779) return false;
		if (point.y < 0 || point.y > 200) return false;
		return true;
	}

	// ==============================================================================================================





	// [ 키보드 제어 ]
	// ==============================================================================================================
public:
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event)
	{
		if (keycode == EventKeyboard::KeyCode::KEY_BACK) {
			popThisScene();
		}
	}


protected:

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
			const char *cstr = str.c_str();
			jstring jmsg = t.env->NewStringUTF(cstr);
			t.env->CallVoidMethod(activityObj, t.methodID, jmsg);
		}
#endif
	}

protected:

	// ==============================================================================================================

};