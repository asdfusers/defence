#pragma once
#include "cocos2d.h"

class COK : public cocos2d::LayerColor
{
public:
	virtual bool init();
	void OK();
	CREATE_FUNC(COK);
	void buttonEffect();

	bool bFail;
};

