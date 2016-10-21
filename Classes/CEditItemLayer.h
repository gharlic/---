#pragma once

#include "common.h"
#include "CDataSingleton.h"

class CMainLayer;

class CEditItemLayer : public cocos2d::Layer, public cocos2d::CCIMEDelegate {
public:
	CEditItemLayer() {}
	virtual ~CEditItemLayer() {}

	// [ 기본 ]
	// ==============================================================================================================
	static cocos2d::Scene* createScene(int type);
	static CEditItemLayer* create(int type);

	virtual bool init(int type);

	void	setType(int type)		{ m_type = type; }
	int		getType()				{ return m_type; }
	std::string	getName()				{ return m_name; }
	std::string	getPrice()				{ return m_price; }
	void	popThisScene(int mode);

protected:
	std::string m_price;
	std::string m_name;

	int m_type;
	Size m_winSize;
	CCTextFieldTTF* nameField;
	CCTextFieldTTF* priceField;
	// ==============================================================================================================




	// [ 터치 제어 ]
	// ==============================================================================================================
	void onTouchesBegan(const std::vector<cocos2d::Touch*>&pTouches, cocos2d::Event* pEvent);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>&pTouches, cocos2d::Event* pEvent);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>&pTouches, cocos2d::Event* pEvent);

protected:
	bool m_bTouchStarted;	// 터치가 시작되었는지
	bool m_bTouchMoved;		// 드래그한 적이 있는지
	Point m_gestureStartPoint;	// 터치가 시작된 좌표
	Point m_gestureStartOrigin;

	bool isInOKRect(Point point) {
		if (point.x < DESIGN_WIDTH - 200 || point.x > DESIGN_WIDTH) return false;
		if (point.y < DESIGN_HEIGHT - 132 || point.y > DESIGN_HEIGHT) return false;
		return true;
	}
	bool isInBackRect(Point point) {
		if (point.x < 0 || point.x > 220) return false;
		if (point.y < DESIGN_HEIGHT - 132 || point.y > DESIGN_HEIGHT) return false;
		return true;

	}
	bool isInInputNameRect(Point point) {
		if (point.x < 140.0f || point.x > 975.0f) return false;
		if (point.y < 1458.0f || point.y > 1599.0f) return false;
		return true;
	}
	bool isInInputPriceRect(Point point) {
		if (point.x < 140.0f || point.x > 975.0f) return false;
		if (point.y < 1158.0f || point.y > 1299.0f) return false;
		return true;
	}
	// ==============================================================================================================






	// [ 키보드 제어 ]
	// ==============================================================================================================
public:
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event)
	{
		if (keycode == EventKeyboard::KeyCode::KEY_BACK) {
			popThisScene(BACK);
		}
	}


protected:

	// ==============================================================================================================
};