#pragma once

#include "Common.h"
#include "CCellObject.h"
#include "CEditItemLayer.h"
#include "CDataSingleton.h"
#include "CSlider.h"

class CCalcLayer;

class CMainLayer : public cocos2d::Layer {
public:
	// [ 기본 ]
	// ==============================================================================================================
	static cocos2d::Scene* createScene() {
		auto pScene = Scene::create();
		auto pLayer = CMainLayer::create();

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

		pBackgroundSprite = cocos2d::Sprite::create("backGroundButtom.png");
		pBackgroundSprite->setAnchorPoint(ccp((float)0, (float)0));
		pBackgroundSprite->setPosition(ccp((float)0, (float)0));
		addChild(pBackgroundSprite, 5);


		// 메인셀 출력
		for (int i = 0; i < MAX_COLS; i++) {
			for (int j = 0; j < MAX_ROWS; j++) {
				int x = CELL_WIDTH + CELL_WIDTH * i;
				int y = DESIGN_HEIGHT - CELL_HEIGHT * 2 - CELL_HEIGHT * j;

				CCellObject* pCellObject = CCellObject::Create(OUT_PEAPLE);
				pCellObject->setPosition(cocos2d::CCPointZero);
				pCellObject->setAnchorPoint(ccp((float)0, (float)0));
				pCellObject->setPosition(ccp((float)x, (float)y));
				addChild(pCellObject, 0);

				pCellObject->setX(x);
				pCellObject->setY(y);
				pCellObject->setType(OUT_PEAPLE);
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


		// 터치 초기화
		EventListenerTouchAllAtOnce* listener = EventListenerTouchAllAtOnce::create();
		listener->onTouchesBegan = CC_CALLBACK_2(CMainLayer::onTouchesBegan, this);
		listener->onTouchesMoved = CC_CALLBACK_2(CMainLayer::onTouchesMoved, this);
		listener->onTouchesEnded = CC_CALLBACK_2(CMainLayer::onTouchesEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
		m_gestureStartRect = 0;
		m_bTouchStarted = false;
		m_bTouchMoved = false;
		m_TouchMode = NORMAL_STATE;


		// 키보드 초기화
		this->setKeypadEnabled(true);
		m_EndKeyStack = 0;

		// 변수 초기화
		for (int i = 0; i < MAX_COLS; i++) {
			m_share[i] = 0.0;
			m_payed[i] = 0.0;
		}
		for (int j = 0; j < MAX_ROWS; j++) {
			m_countPeaple[j] = 0.0;
		}



		CDataSingleton::setMainLayer(this);
		m_winSize = Director::sharedDirector()->getWinSize();
		return true;
	}
	CREATE_FUNC(CMainLayer);

	CCellSlider* getSliderCols(int i) { return m_SliderCols[i]; }
	CCellSlider* getSliderRows(int i) { return m_SliderRows[i]; }
	CCellObject* getCellObject(int i, int j) { return m_Board[i][j]; }
	void			resetBoard() {
		for (int i = 0; i < MAX_COLS; i++) {
			for (int j = 0; j < MAX_ROWS; j++) {
				// 개인당 몫을 최신화
				if (m_Board[i][j]->getType() != OUT_PEAPLE) {
					float price = ((float)m_SliderRows[j]->getPrice() / (float)m_countPeaple[j]);
					m_Board[i][j]->setPrice(price);
					removeChild(m_Board[i][j]->getPriceLabel(), true);
					m_Board[i][j]->setPriceLabel();
					addChild(m_Board[i][j]->getPriceLabel(), 2);
				}
			}
		}
	}

protected:
	Size m_winSize;

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
					(m_gestureStartRect == SLIDER_COLS_RECT && isInSliderColsRect(point)) ||
					(m_gestureStartRect == SLIDER_ROWS_RECT && isInSliderRowsRect(point))) {
					Scroll(point);
				}
				else
					m_bTouchStarted = false;
				m_gestureStartPoint.setPoint(point.x, point.y);
			}
		}
	}
	void onTouchesEnded(const std::vector<cocos2d::Touch*>&pTouches, cocos2d::Event* pEvent);
	void Scroll(Point point) {
		int x = point.x - m_gestureStartPoint.x;
		int y = point.y - m_gestureStartPoint.y;
		if (m_gestureStartRect == SLIDER_COLS_RECT) y = 0;
		if (m_gestureStartRect == SLIDER_ROWS_RECT) x = 0;
		if (m_Board[0][0]->getX() + x >= CELL_WIDTH) x = 0;
		if (m_Board[0][0]->getY() + y <= DESIGN_HEIGHT - CELL_HEIGHT * 2) y = 0;
		if (m_Board[MAX_COLS - 1][MAX_ROWS - 1]->getX() + CELL_WIDTH + x <= DESIGN_WIDTH) x = 0;
		if (m_Board[MAX_COLS - 1][MAX_ROWS - 1]->getY() + y >= 200) y = 0;


		for (int i = 0; i < MAX_COLS; i++) {
			for (int j = 0; j < MAX_ROWS; j++) {
				m_Board[i][j]->Scroll(x, y);
			}
		}
		for (int i = 0; i < MAX_COLS; i++)
			m_SliderCols[i]->Scroll(x, y);
		for (int j = 0; j < MAX_ROWS; j++)
			m_SliderRows[j]->Scroll(x, y);

	}

	int m_EnableTouch;		// 터치 on/off
	bool m_bTouchStarted;	// 터치가 시작되었는지
	bool m_bTouchMoved;		// 드래그한 적이 있는지
	Point m_gestureStartPoint;	// 터치가 시작된 좌표
	Point m_gestureStartOrigin;
	int m_gestureStartRect;		// 터치를 시작한 구역
	int m_TouchMode;


	bool isInMainRect(Point point) {
		if (point.x < CELL_WIDTH || point.x > DESIGN_WIDTH) return false;
		if (point.y < BUTTOM_HEIGHT || point.y > DESIGN_HEIGHT - CELL_HEIGHT) return false;
		return true;
	}
	bool isInSliderColsRect(Point point) {
		if (point.x < CELL_WIDTH || point.x > DESIGN_WIDTH) return false;
		if (point.y < DESIGN_HEIGHT - CELL_HEIGHT || point.y > DESIGN_HEIGHT) return false;
		return true;
	}
	bool isInSliderRowsRect(Point point) {
		if (point.x < 0 || point.x > CELL_WIDTH) return false;
		if (point.y < BUTTOM_HEIGHT || point.y > DESIGN_HEIGHT - CELL_HEIGHT) return false;
		return true;
	}
	bool isInResetPositionRect(Point point) {
		if (point.x < 0 || point.x > CELL_WIDTH) return false;
		if (point.y < DESIGN_HEIGHT - CELL_HEIGHT || point.y > DESIGN_HEIGHT) return false;
		return true;
	}
	bool isInCellRect(Point point, int x, int y) {
		if (point.x < x || point.x > x + CELL_WIDTH) return false;
		if (point.y < y || point.y > y + CELL_HEIGHT) return false;
		return true;
	}
	bool isInCalcButtonRect(Point point) {
		if (point.x < 353 || point.x > 725) return false;
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
			if (m_EndKeyStack > 0)
				Director::getInstance()->end();
			m_EndKeyStack++;
		}
	}


protected:
	int m_EndKeyStack;

	// ==============================================================================================================






	// [ 데이터 제어 ]
	// ==============================================================================================================
public:
	void releaseSliderValue() {
		auto p = CDataSingleton::getInstance();

		m_seletedSliderCell->setName(p->name);
		m_seletedSliderCell->setPrice(atoi(p->price.c_str()));
		removeChild(m_seletedSliderCell->getPriceLabel(), true);
		m_seletedSliderCell->setPriceLabel();
		addChild(m_seletedSliderCell->getPriceLabel(), 4);

		releaseCellPrice();
	}
	float getShare(int i) { return m_share[i]; }
	float getPayed(int i) { return m_payed[i]; }
	void			setCountPeaple(float n, int i) { m_countPeaple[i] = n; }
	void			setShare(float n, int i) { m_share[i] = n; }
	void			setPayed(float n, int i) { m_payed[i] = n; }
	void			setSliderColsName(std::string str, int i) { m_SliderCols[i]->setName(str); }

protected:
	CCellObject*	m_Board[MAX_COLS][MAX_ROWS];
	CCellSlider*	m_SliderCols[MAX_COLS];
	CCellSlider*	m_SliderRows[MAX_ROWS];
	CCellSlider*	m_seletedSliderCell; // 선택된 슬라이더 셀
	float			m_countPeaple[MAX_ROWS];
	float			m_share[MAX_COLS];
	float			m_payed[MAX_COLS];



	void resetPosition() {
		for (int i = 0; i < MAX_COLS; i++) {
			for (int j = 0; j < MAX_ROWS; j++) {
				int x = CELL_WIDTH + CELL_WIDTH * i;
				int y = DESIGN_HEIGHT - CELL_HEIGHT * 2 - CELL_HEIGHT * j;
				m_Board[i][j]->setX(x);
				m_Board[i][j]->setY(y);
				m_Board[i][j]->Scroll(0, 0);
			}
		}
		for (int i = 0; i < MAX_COLS; i++) {
			int x = CELL_WIDTH + CELL_WIDTH * i;
			int y = DESIGN_HEIGHT - CELL_HEIGHT;
			m_SliderCols[i]->setX(x);
			m_SliderCols[i]->setY(y);
			m_SliderCols[i]->Scroll(0, 0);
		}
		for (int j = 0; j < MAX_ROWS; j++) {
			int x = 0;
			int y = DESIGN_HEIGHT - CELL_HEIGHT * 2 - CELL_HEIGHT * j;
			m_SliderRows[j]->setX(x);
			m_SliderRows[j]->setY(y);
			m_SliderRows[j]->Scroll(0, 0);
		}
	}
	void pickCell(Point point) {
		for (int i = 0; i < MAX_COLS; i++) {
			for (int j = 0; j < MAX_ROWS; j++) {
				int x = m_Board[i][j]->getX();
				int y = m_Board[i][j]->getY();

				if (isInCellRect(point, x, y)) {
					// 선택된 셀이 이미 '돈 낸 사람'이면 '참여한 사람'으로 바꾼다
					if (m_Board[i][j]->getType() == JOIN_PEAPLE) {
						int check = false;
						int k;
						for (k = 0; k < MAX_COLS; k++) {
							if (m_Board[k][j]->getType() == PAY_PEAPLE) {
								check = true;
								break;
							}
						}
						if (check) {
							removeChild(m_Board[k][j]->getPriceLabel(), true);
							changeCellType(k, j);
							m_Board[k][j]->setPriceLabel();
							addChild(m_Board[k][j]->getPriceLabel(), 2);
							removeChild(m_Board[k][j]->getPriceLabel(), true);
							changeCellType(k, j);
							m_Board[k][j]->setPriceLabel();
							addChild(m_Board[k][j]->getPriceLabel(), 2);

						}
					}
					// 선택된 셀의 '참여도'를 한단계 증가시킨다.
					if (m_Board[i][j]->getType() == PAY_PEAPLE) {
						m_countPeaple[j]--;
					}
					else if (m_Board[i][j]->getType() != JOIN_PEAPLE) {
						m_countPeaple[j]++;
					}
					//m_Board[i][j]->setPrice(m_countPeaple[j]);	// debug
					removeChild(m_Board[i][j]->getPriceLabel(), true);
					changeCellType(i, j);
					m_Board[i][j]->setPriceLabel();
					addChild(m_Board[i][j]->getPriceLabel(), 2);
				}
			}
		}
		releaseCellPrice();
	}
	void pickSliderRowsCell(Point point) {
		for (int j = 0; j < MAX_ROWS; j++) {
			int x = m_SliderRows[j]->getX();
			int y = m_SliderRows[j]->getY();
			if (isInCellRect(point, x, y)) {
				auto pScene = Scene::create();
				auto pLayer = CEditItemLayer::create(MENU_MODE);
				pScene->addChild(pLayer);
				Director::getInstance()->pushScene(pScene);

				m_seletedSliderCell = m_SliderRows[j];
			}
		}
	}
	void pickSliderColsCell(Point point) {
		for (int i = 0; i < MAX_COLS; i++) {
			int x = m_SliderCols[i]->getX();
			int y = m_SliderCols[i]->getY();
			if (isInCellRect(point, x, y)) {
				auto pScene = Scene::create();
				auto pLayer = CEditItemLayer::create(NAME_MODE);
				pScene->addChild(pLayer);
				Director::getInstance()->pushScene(pScene);

				m_seletedSliderCell = m_SliderCols[i];
			}
		}
	}
	void changeCellType(int i, int j) {
		CCellObject* pCellObject = CCellObject::Create(m_Board[i][j]->getType() + 1);
		pCellObject->clone(m_Board[i][j]);
		pCellObject->setAnchorPoint(ccp((float)0, (float)0));
		pCellObject->setType(m_Board[i][j]->getType() + 1);
		addChild(pCellObject, 0);
		removeChild(m_Board[i][j], true);
		m_Board[i][j] = pCellObject;
		pCellObject->Scroll(0, 0);
	}
	void releaseCellPrice() {
		for (int i = 0; i < MAX_COLS; i++) {
			for (int j = 0; j < MAX_ROWS; j++) {
				// 개인당 몫을 최신화
				if (m_Board[i][j]->getType() != OUT_PEAPLE) {
					float price = ((float)m_SliderRows[j]->getPrice() / (float)m_countPeaple[j]);
					m_Board[i][j]->setPrice(price);
					removeChild(m_Board[i][j]->getPriceLabel(), true);
					m_Board[i][j]->setPriceLabel();
					addChild(m_Board[i][j]->getPriceLabel(), 2);
				}
			}
		}
	}
	void calcData() {
		for (int i = 0; i < MAX_COLS; i++) {
			for (int j = 0; j < MAX_ROWS; j++) {
				if (m_Board[i][j]->getType() == JOIN_PEAPLE) {
					m_share[i] += m_Board[i][j]->getPrice();
				}
				else if (m_Board[i][j]->getType() == PAY_PEAPLE) {
					m_share[i] += m_Board[i][j]->getPrice();
					m_payed[i] += m_SliderRows[j]->getPrice();
				}
				else
					m_share[i] += 0.0f;
				m_payed[i] += 0.0f;
			}
		}
	}
	// ==============================================================================================================

};

