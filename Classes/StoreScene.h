#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "cocos2d.h"
#include <network\SocketIO.h>
#include "ui\CocosGUI.h"

using namespace cocos2d::network;

class CStoreScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	void addWeaponInterface(const char * imgPath, const char * txtPath, cocos2d::Point pt, int tag);

	CREATE_FUNC(CStoreScene);
	void click();
	void buy1();
	void buy2();
	void getScores(SIOClient* client, const std::string& data);
	void fail(SIOClient* client, const std::string& data);
	void hideDlg();
	void ShowDlg(int tag);



	cocos2d::ui::Button* cannonBuyButton;
	cocos2d::ui::Button* missilebuyButton;
	cocos2d::ui::Button* backButton;


private:
//	SIOClient* _client;
	bool bFail;
	int Score;
	cocos2d::ui::Button* button;

};

#endif // __HELLOWORLD_SCENE_H__
