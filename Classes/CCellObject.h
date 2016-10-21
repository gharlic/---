#ifndef PpumPai_CellObject
#define PpumPai_CellObject


class CCellObject : public Sprite {
	// [ 기본 ]
	// ==============================================================================================================
public:
	CCellObject() {
		m_priceLabel = NULL;
		m_price = 0.0f;
	}
	virtual ~CCellObject() {}

	static CCellObject* Create(int type) {
		if (type == 3) type = 0;
		static std::string objectNames[4] = {
			"cell1.png",
			"cell2.png",
			"cell3.png",
			"cell1.png",
		};

		CCellObject* pCellObject = CCellObject::create(objectNames[type].c_str(),
			CCRectMake(0.0f, 0.0f, CELL_WIDTH, CELL_HEIGHT));
		return pCellObject; 
	}

	int getType()			{ return m_type; }
	void setType(int type, int mode = 0) { 
		if (mode == 0) {
			if (type == 3) type = 0; m_type = type;
		}
		else if(mode == 1) {
			m_type = type;
		}
	}

protected:
	static CCellObject* create(const char* pszFileName, const CCRect& rect) {
		CCellObject* pSprite = new CCellObject();
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
	float		getPrice()				{ return m_price; }
	void	setPrice(float price)		{ m_price = price; }
	int		getX()					{ return m_x; }
	int		getY()					{ return m_y; }
	void	setX(int x)				{ m_x = x; }
	void	setY(int y)				{ m_y = y; }

	LabelTTF*	getPriceLabel()		{ return (m_priceLabel) ? m_priceLabel : NULL; }
	void		setPriceLabel()		{
		std::string str = Common::FormatWithCommas(m_price);
		if (m_type == OUT_PEAPLE)
			m_priceLabel = LabelTTF::create(" ", "Arial", 40,
			Size(CELL_WIDTH, CELL_HEIGHT), TextHAlignment::CENTER, TextVAlignment::CENTER);
		else
			m_priceLabel = LabelTTF::create(str, "Arial", 40,
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
	}

protected:
	float m_price;
	int m_x;
	int m_y;
	LabelTTF* m_priceLabel;
	// ==============================================================================================================






	// [스크롤]
	// ==============================================================================================================
public:
	void Scroll(int x, int y) {
		m_x = m_x + x;
		m_y = m_y + y;
		setPosition(ccp(m_x, m_y));
		if (m_priceLabel) m_priceLabel->setPosition(ccp(m_x, m_y));
	}

protected:

	// ==============================================================================================================

};

#endif