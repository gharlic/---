#ifndef PpumPai_Slider
#define PpumPai_Slider

class CCellSlider : public Sprite {
	// [ 기본 ]
	// ==============================================================================================================
public:
	CCellSlider() {
		m_priceLabel = NULL;
		m_name = "";
		m_price = 0.0f;

	}
	virtual ~CCellSlider() {}

	static CCellSlider* Create(int type) {
		static std::string SliderNames[2] = {
			"SliderCols.png",
			"SliderRows.png"
		};
		if (type<0 || type>1) return NULL;

		CCellSlider* pCellSlider = CCellSlider::create(SliderNames[type].c_str(),
			CCRectMake(0.0f, 0.0f, CELL_WIDTH, CELL_HEIGHT));
		return pCellSlider;
	}

	int GetType()			{ return m_type; }
	void SetType(int type)	{ m_type = type; }

protected:
	static CCellSlider* create(const char* pszFileName, const CCRect& rect) {
		CCellSlider* pSprite = new CCellSlider();
		if (pSprite && pSprite->initWithFile(pszFileName, rect)) {
			pSprite->autorelease();
			return pSprite;
		}
		CC_SAFE_DELETE(pSprite);
		return NULL;
	}

	int m_type;
	// ==============================================================================================================





	// [ 데이터 ]
	// ==============================================================================================================
public:
	std::string getName()						{ return m_name; }
	void		setName(std::string name)		{ m_name = name; }
	float		getPrice()						{ return m_price; }
	void		setPrice(float price)				{ m_price = price; }
	int			getX()							{ return m_x; }
	int			getY()							{ return m_y; }
	void		setX(int x)						{ m_x = x; }
	void		setY(int y)						{ m_y = y; }

	LabelTTF*	getPriceLabel()		{ return (m_priceLabel) ? m_priceLabel : NULL; }
	void		setPriceLabel()		{
		std::string strName = m_name;
		std::string strNameAndPrice = m_name;
		if(m_price)strNameAndPrice += "\n";
		strNameAndPrice += Common::FormatWithCommas(m_price);

		if (m_type == SLIDER_COLS_TYPE)
			m_priceLabel = LabelTTF::create(strName, "Arial", 30,
			Size(CELL_WIDTH, CELL_HEIGHT), TextHAlignment::CENTER, TextVAlignment::CENTER);
		else
			m_priceLabel = LabelTTF::create(strNameAndPrice, "Arial", 30,
			Size(CELL_WIDTH, CELL_HEIGHT), TextHAlignment::CENTER, TextVAlignment::CENTER);

		m_priceLabel->setAnchorPoint(ccp((float)0.0f, (float)0.0f));
		m_priceLabel->setPosition(ccp(m_x, m_y));
		m_priceLabel->setColor(Color3B(50, 50, 50));
	}

	void clone(CCellObject* p) {
		m_x = p->getX();
		m_y = p->getY();
		m_price = p->getPrice();
		m_type = p->getType();
		m_name = p->getName();
	}

protected:
	std::string m_name;
	float m_price;
	int m_x;
	int m_y;
	LabelTTF* m_priceLabel;
	// ==============================================================================================================





	// [스크롤]
	// ==============================================================================================================
public:
	void Scroll(int x, int y) {
		if(m_type == 0) m_x = m_x + x;
		if(m_type == 1) m_y = m_y + y;
		setPosition(ccp(m_x, m_y));
		if (m_priceLabel) m_priceLabel->setPosition(ccp(m_x, m_y));
	}

protected:

	// ==============================================================================================================

};

#endif