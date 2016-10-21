#pragma once

#include "CSlider.h"
#include "Common.h"

class CResultSlider : public CCellSlider {
	// [ 기본 ]
	// ==============================================================================================================
public:
	CResultSlider() {
		m_textLabel = NULL;
		m_name = "";
		m_price = 0.0f;
	}
	virtual ~CResultSlider() {}

	static CResultSlider* Create(bool mode) {
		static std::string SliderNames = { "kakaolink_back.png" };
		if (!mode) SliderNames = { "kakaolink_back2.png" };
		CResultSlider* pSlider = CResultSlider::create(SliderNames.c_str(),
			CCRectMake(0.0f, 0.0f, DESIGN_WIDTH, 150)); 
		return pSlider;
	}

protected:
	static CResultSlider* create(const char* pszFileName, const CCRect& rect) {
		CResultSlider* pSprite = new CResultSlider();
		if (pSprite && pSprite->initWithFile(pszFileName, rect)) {
			pSprite->autorelease();
			return pSprite;
		}
		CC_SAFE_DELETE(pSprite);
		return NULL;
	}
	// ==============================================================================================================






	// [ 데이터 ]
	// ==============================================================================================================
public:
	std::string getText()						{ return m_text; }
	void		setText(std::string text)		{ m_text = text; }

	LabelTTF*	getTextLabel()		{ return (m_textLabel) ? m_textLabel : NULL; }
	void		setTextLabel()		{
		std::string strName = m_text;

		m_textLabel = LabelTTF::create(strName, "Arial", 60,
			Size(DESIGN_WIDTH, 150), TextHAlignment::LEFT, TextVAlignment::CENTER);
		m_textLabel->setAnchorPoint(ccp((float)0.0f, (float)0.0f));
		m_textLabel->setPosition(ccp(50, m_y));
		m_textLabel->setColor(Color3B(50, 50, 50));
	}

protected:
	std::string m_text;
	LabelTTF* m_textLabel;
	// ==============================================================================================================






	// [스크롤]
	// ==============================================================================================================
public:
	void Scroll(int x, int y) {
		m_x = m_x + x;
		m_y = m_y + y;
		setPosition(ccp(m_x, m_y));
		if (m_textLabel) m_textLabel->setPosition(ccp(50, m_y));
	}

protected:

	// ==============================================================================================================
};