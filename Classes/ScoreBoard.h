#pragma once
#include "cocos2d.h"

class CScoreBoard : public cocos2d::LayerColor
{
public:
	virtual bool init();
	void OK();
	CREATE_FUNC(CScoreBoard);
	

	bool bFail;

};

