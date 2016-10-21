#include "CEditItemLayer.h"
#include "CMainLayer.h"

USING_NS_CC;

cocos2d::Scene* CEditItemLayer::createScene(int type) {
	auto pScene = Scene::create();
	auto pLayer = CEditItemLayer::create(type);
	pScene->addChild(pLayer);

	return pScene;
}
CEditItemLayer* CEditItemLayer::create(int type) {
	CEditItemLayer *pRet = new CEditItemLayer();
	if (pRet && pRet->init(type)) {
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}
bool CEditItemLayer::init(int type) {
	cocos2d::Sprite* pBackgroundSprite = cocos2d::Sprite::create("InputLayer.png");
	pBackgroundSprite->setPosition(cocos2d::CCPointZero);
	pBackgroundSprite->setAnchorPoint(ccp((float)0, (float)0));
	pBackgroundSprite->setPosition(ccp((float)0, (float)0));
	addChild(pBackgroundSprite, 0);

	auto nameLabel = LabelTTF::create((type == MENU_MODE) ?
		"메뉴를 입력해주세요." : "이름을 입력해주세요.",
		"Arial", 50, Size(500, 70));
	nameLabel->setAnchorPoint(CCPointZero);
	nameLabel->setColor(Color3B(100, 100, 100));
	nameLabel->setPosition(ccp(100, 1650));
	nameLabel->setTag(100);
	this->addChild(nameLabel);

	nameField = CCTextFieldTTF::textFieldWithPlaceHolder("",
		CCSize(800, 70), kCCTextAlignmentLeft, "Arial", 50);
	nameField->setAnchorPoint(CCPointZero);
	nameField->setColor(Color3B(100, 100, 100));
	nameField->setPosition(ccp(125, 1490));
	nameField->setTag(100);
	this->addChild(nameField);

	if (type == MENU_MODE) {
		auto nameLabel = LabelTTF::create("가격을 입력해주세요.",
			"Arial", 50, Size(500, 70));
		nameLabel->setAnchorPoint(CCPointZero);
		nameLabel->setColor(Color3B(100, 100, 100));
		nameLabel->setPosition(ccp(100, 1350));
		nameLabel->setTag(100);
		this->addChild(nameLabel);

		pBackgroundSprite = cocos2d::Sprite::create("InputBox.png");
		pBackgroundSprite->setAnchorPoint(ccp((float)0, (float)0));
		pBackgroundSprite->setPosition(ccp(100, 1158));
		addChild(pBackgroundSprite, 1);

		priceField = CCTextFieldTTF::textFieldWithPlaceHolder("",
			CCSize(800, 70), kCCTextAlignmentLeft, "Arial", 50);
		priceField->setAnchorPoint(CCPointZero);
		priceField->setColor(Color3B(100, 100, 100));
		priceField->setPosition(ccp(125, 1190));
		priceField->setTag(100);
		this->addChild(priceField, 2);
	}

	// 터치 초기화
	EventListenerTouchAllAtOnce* listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(CEditItemLayer::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(CEditItemLayer::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(CEditItemLayer::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	m_bTouchStarted = false;
	m_bTouchMoved = false;

	// 키보드 초기화
	this->setKeypadEnabled(true);
	m_type = type;
	m_winSize = Director::sharedDirector()->getWinSize();
	return true;
}

void CEditItemLayer::popThisScene(int mode) {
	nameField->detachWithIME();
	if (m_type == MENU_MODE)
		priceField->detachWithIME();

	CMainLayer* p = CDataSingleton::getMainLayer();
	if (mode == OKAY)
		p->releaseSliderValue();
	Director::getInstance()->popScene();
}

// [ 터치 제어 ]
// ==============================================================================================================
void CEditItemLayer::onTouchesBegan(const std::vector<cocos2d::Touch*>&pTouches, cocos2d::Event* pEvent) {
	if (!m_bTouchStarted) {
		Touch* pTouch = (Touch*)pTouches.back();
		Point point = pTouch->getLocation();

		//Director::getInstance()->popScene();
		m_gestureStartPoint.setPoint(point.x, point.y);
		m_gestureStartOrigin.setPoint(point.x, point.y);
		m_bTouchStarted = true;
	}

}
void CEditItemLayer::onTouchesMoved(const std::vector<cocos2d::Touch*>&pTouches, cocos2d::Event* pEvent) {
	if (m_bTouchStarted) {
		Touch* pTouch = (Touch*)pTouches.back();
		Point point = pTouch->getLocation();

		m_bTouchMoved = true;
	}
}
void CEditItemLayer::onTouchesEnded(const std::vector<cocos2d::Touch*>&pTouches, cocos2d::Event* pEvent) {
	if (m_bTouchStarted) {
		Touch* pTouch = (Touch*)pTouches.back();
		Point point = pTouch->getLocation();

		// 단순 클릭인 경우
		if (abs(m_gestureStartOrigin.x - point.x)<TOUCH_RANGE &&
			abs(m_gestureStartOrigin.y - point.y)<TOUCH_RANGE) {
			// 확인버튼 클릭 시
			if (isInOKRect(point)) {
				auto p = CDataSingleton::getInstance();
				p->name = nameField->getString();
				if (m_type == MENU_MODE)
					p->price = priceField->getString();

				popThisScene(OKAY);
			}
			// 뒤로버튼 클릭 시
			else if (isInBackRect(point)) {
				popThisScene(BACK);
			}
			// 이름 입력창 클릭 시
			else if (isInInputNameRect(point)) {
				if(m_type == MENU_MODE) 
					priceField->detachWithIME();
				nameField->attachWithIME();
			}
			// 가격 입력창 클릭 시
			else if (isInInputPriceRect(point)) {
				nameField->detachWithIME();
				if (m_type == MENU_MODE)
					priceField->attachWithIME();
			}
			else {
				nameField->detachWithIME();
				if (m_type == MENU_MODE)
					priceField->detachWithIME();
			}
		}
		m_bTouchStarted = false;
		m_bTouchMoved = false;
	}
}
// ==============================================================================================================
