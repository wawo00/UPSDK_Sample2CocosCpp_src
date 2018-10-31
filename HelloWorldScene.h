#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include <ui/UIButton.h>
#include "cocos2d.h"
//#include "../cocos2d/cocos/cocos2d.h"
class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type, int tag);
    void addNewButton(int sizewd, int x, int y, std::string text, int tag);

};


#endif // __HELLOWORLD_SCENE_H__
