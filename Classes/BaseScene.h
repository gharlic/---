#ifndef __BASE_SCENE_H__
#define __BASE_SCENE_H__
/*유니코드 저장을 위한 더미 한글입니다 지우지 마세요*/
#include "cocos2d.h"

class BaseScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuCloseCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(BaseScene);
};

#endif
