#pragma once
#include "cocos2d.h"

class CFailDlg : public cocos2d::LayerColor
{
public:
	virtual bool init();
	void OK();
	CREATE_FUNC(CFailDlg);
	void buttonEffect();

	bool bFail;
};

