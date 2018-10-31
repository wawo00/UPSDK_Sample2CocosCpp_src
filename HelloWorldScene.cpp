#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#include "upltv/UpltvBridge.h"

USING_NS_CC;


Scene *HelloWorld::createScene() {
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char *filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
    //////////////////////////////
    // 1. super init first
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0) {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    } else {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
        float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello Cpp, By Sam", "fonts/Marker Felt.ttf", 12);
    if (label == nullptr) {
        problemLoading("'fonts/Marker Felt.ttf'");
    } else {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                origin.y + label->getContentSize().height - 5));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");
    if (sprite == nullptr) {
        problemLoading("'HelloWorld.png'");
    } else {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x,
                                 visibleSize.height / 2 + origin.y - 100 +
                                 label->getContentSize().height));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }


    //UpltvBrigde::testCall("haha, haha.", 1);

    int left = origin.x + 60;
    int sizewd = 70;
    int top = origin.y + visibleSize.height - 30;
    int disWD = 90;
    int disHT = 35;
    int x = left, y = top;


    //auto button = cocos2d::ui::Button::create();
    addNewButton(sizewd, x, y, "init", 1001);

    y -= disHT;
    addNewButton(sizewd, x, y, "initByCall", 1002);

    y -= disHT;
    addNewButton(sizewd, x, y, "initABtest", 1003);

    y -= disHT;
    addNewButton(sizewd, x, y, "getABConfig", 1004);

    // 第二行, 激励视屏测试
    x += disWD;
    y = top;

    addNewButton(sizewd, x, y, "rdDebugUI", 2001);

    y -= disHT;
    addNewButton(sizewd, x, y, "rdLoadCall", 2002);

    y -= disHT;
    addNewButton(sizewd, x, y, "rdShowCall", 2003);

    y -= disHT;
    addNewButton(sizewd, x, y, "rdReady", 2004);

    y -= disHT;
    addNewButton(sizewd, x, y, "rdShow", 2005);


    // 第三行, 插屏广告测试
    x += disWD;
    y = top;

    addNewButton(sizewd, x, y, "iLShowUI", 3001);

    y -= disHT;
    addNewButton(sizewd, x, y, "iLLoadCall", 3002);

    y -= disHT;
    addNewButton(sizewd, x, y, "iLShowCall", 3003);

    y -= disHT;
    addNewButton(sizewd, x, y, "iLReadyAsyn", 3004);

    y -= disHT;
    addNewButton(sizewd, x, y, "iLShow", 3005);

    // 第四行, Banner广告测试
    x += disWD;
    y = top;

    addNewButton(sizewd, x, y, "bannerCall", 4001);

    y -= disHT;
    addNewButton(sizewd, x, y, "topBanner", 4002);

    y -= disHT;
    addNewButton(sizewd, x, y, "bottomBanner", 4003);

    y -= disHT;
    addNewButton(sizewd, x, y, "hideAll", 4004);

    y -= disHT;
    addNewButton(sizewd, x, y, "removeAll", 4005);

    // 第五行, Banner广告测试
    x += disWD;
    y = top;

    addNewButton(sizewd, x, y, "setPkg", 5001);

    y -= disHT;
    addNewButton(sizewd, x, y, "onBackPressed", 5002);

    y -= disHT;
    addNewButton(sizewd, x, y, "loadByManual", 5003);

    y -= disHT;
    addNewButton(sizewd, x, y, "exitApp", 5004);

    y -= disHT;
    addNewButton(sizewd, x, y, "GDPR", 5005);

    return true;
}

void HelloWorld::addNewButton(int sizewd, int x, int y, std::string text, int tag) {
    auto button = cocos2d::ui::Button::create();
    button->loadTextureNormal("res/feedback.png");
    button->loadTexturePressed("res/feedback_active.png");
    button->setTitleText(text);
    button->setTitleFontSize(10);
    button->setPosition(Vec2(x, y));
    button->setTag(tag);
    button->setTitleAlignment(TextHAlignment::CENTER);
    button->setScale9Enabled(true);
    button->setContentSize(Size(sizewd, 26));
    button->addTouchEventListener(CC_CALLBACK_2(HelloWorld::touchEvent, this, tag));
    this->addChild(button, 1);
}

void sdkCallback(bool r) {
    log("====> cpp initSdkCallback(%s)", (r ? "true" : "false"));
}

void iLReadyCallback(const char* cpPlaceId, bool r) {
    log("====> cpp iLReadyCallback(%s, %s)", cpPlaceId, (r ? "true" : "false"));
}

void rdLoadCallSuccess(const char *cpid, const char *msg) {
    log("====> reward video didLoad Success at cpid: %s", (cpid != nullptr ? cpid : ""));
}


void rdLoadCallFail(const char *cpid, const char *msg) {
    log("====> reward video didLoad Fail at cpid: %s", (cpid != nullptr ? cpid : ""));
}

void iLLoadCallSuccess(const char *cpid, const char *msg) {
    log("====> iLLoadCallSuccess at cpid: %s", (cpid != nullptr ? cpid : ""));
}

void iLLoadCallFail(const char *cpid, const char *msg) {
    log("====> iLLoadCallFail  at cpid: %s", (cpid != nullptr ? cpid : ""));
}

void rdShowCallback(UpltvAdEventEnum::AdEventType type, const char *cpid) {
    string s = "unkown";
    switch (type) {
        case UpltvAdEventEnum::AdEventType::VIDEO_EVENT_DID_SHOW:{
            s = "Did_Show";
        }
            break;
        case UpltvAdEventEnum::AdEventType::VIDEO_EVENT_DID_CLICK:{
            s = "Did_Click";
        }
            break;
        case UpltvAdEventEnum::AdEventType::VIDEO_EVENT_DID_CLOSE:{
            s = "Did_Close";
        }
            break;
        case UpltvAdEventEnum::AdEventType::VIDEO_EVENT_DID_GIVEN_REWARD:{
            s = "Did_Give";
        }
            break;
        case UpltvAdEventEnum::AdEventType::VIDEO_EVENT_DID_ABANDON_REWARD:{
            s = "Did_Abandon";
        }
            break;
    }
    log("====> reward video showCall event: %s, at cpid: %s", (s.c_str() != nullptr ? s.c_str() : ""), cpid?cpid:"");
    //log("====> reward video show event: %s at cpid: %s", s.c_str, (cpid != nullptr ? cpid : ""));
}

void iLShowCallback(UpltvAdEventEnum::AdEventType type, const char *cpid) {
    string s = "unkown";
    switch (type) {
        case UpltvAdEventEnum::AdEventType::INTERSTITIAL_EVENT_DID_SHOW:{
            s = "Did_Show";
        }
            break;
        case UpltvAdEventEnum::AdEventType::INTERSTITIAL_EVENT_DID_CLICK:{
            s = "Did_Click";
        }
            break;
        case UpltvAdEventEnum::AdEventType::INTERSTITIAL_EVENT_DID_CLOSE:{
            s = "Did_Close";
        }
            break;
    }
    log("====> interstitial ad showCall event: %s, at cpid: %s", (s.c_str() != nullptr ? s.c_str() : ""), cpid?cpid:"");
}

void exitAdCallback(UpltvAdEventEnum::AdEventType type, const char *cpid) {
    string s = "unkown";
    switch (type) {
        case UpltvAdEventEnum::AdEventType::EXITAD_EVENT_DID_SHOW:{
            s = "Did_Show";
        }
            break;
        case UpltvAdEventEnum::AdEventType::EXITAD_EVENT_DID_CLICK:{
            s = "Did_Click";
        }
            break;
        case UpltvAdEventEnum::AdEventType::EXITAD_EVENT_DID_CLICKMORE:{
            s = "Did_Click_More";
        }
            break;
        case UpltvAdEventEnum::AdEventType::EXITAD_EVENT_DID_EXIT:{
            s = "Did_Eixt";
        }
            break;
        case UpltvAdEventEnum::AdEventType::EXITAD_EVENT_DID_CANCEL:{
            s = "Did_Cancel";
        }
            break;
    }
    log("====> exitAdCallback Call event: %s, at cpid: %s", (s.c_str() != nullptr ? s.c_str() : ""), cpid?cpid:"");
}

void bannerCallback(UpltvAdEventEnum::AdEventType type, const char *cpid) {
    string s = "unkown";
    switch (type) {
        case UpltvAdEventEnum::AdEventType::BANNER_EVENT_DID_SHOW:{
            s = "Did_Show";
        }
            break;
        case UpltvAdEventEnum::AdEventType::BANNER_EVENT_DID_CLICK:{
            s = "Did_Click";
        }
            break;
        case UpltvAdEventEnum::AdEventType::BANNER_EVENT_DID_REMOVED:{
            s = "Did_Removed";
        }
            break;
    }
    log("====> Banner ad call event: %s, at cpid: %s", (s.c_str() != nullptr ? s.c_str() : ""), cpid?cpid:"");
}

void gdprUeropeanLocationCallback(bool isUeropeanUser) {
    log("====> gdprUeropeanLocationCallback: %s", isUeropeanUser?"yes":"no");
}

void dgprNotifyCallback(UpltvGDPRPermissionEnum::UPAccessPrivacyInfoStatus status, const char* msg) {
    string s = "unkown";
    switch (status) {
        case UpltvGDPRPermissionEnum::UPAccessPrivacyInfoStatus::UPAccessPrivacyInfoStatusAccepted:{
            s = "Accepted";
        }
            break;
        case UpltvGDPRPermissionEnum::UPAccessPrivacyInfoStatus::UPAccessPrivacyInfoStatusDefined:{
            s = "Defined";
        }
            break;
        
    }
    log("====> dgprNotifyCallback result: %s msg: %s", s.c_str(), msg?msg:"");
}

void HelloWorld::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type, int tag) {
    //log("====> cpp button touch type: %zd", type);

    if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
        log("====> cpp button touch tag: %d", tag);
        string ilkey = "inter_aaa";//"Interstitial_GameStart";
//        const char* bannertopkey = "BannerAd";
        const char* bannertopkey = "banner_aaa";
        const char* bannerbottomkey = "banner_bbb";
        switch (tag) {
            case 1001: {
                UpltvBridge::initSdkByZone(0);
            }
                break;
            case 1002: {
                UpltvSdkBoolCallback callback = sdkCallback;
                UpltvBridge::initSdkByZoneWithCall(0, callback);
            }
                break;
            case 1003: {
                vector<string> tags = {"This is the first element.", "The second one.",
                                       "The last one."};
                UpltvBridge::initAbtConfigJson("u89731", true, 0, "Facebook", "", -1, &tags);
            }
                break;
            case 1004: {
                string r = UpltvBridge::getAbtConfig("pass");
                log("===> cpp getAbtConfig r: %s", r.c_str());
            }
                break;
            case 2001: {
                UpltvBridge::showRewardVideoDebugUI();
            }
                break;
            case 2002: {
                UpltvSdkStringCallback_2 loadSuccess = rdLoadCallSuccess;
                UpltvSdkStringCallback_2 loadFail = rdLoadCallFail;
                log("====> cpp print load s address: %ld", loadSuccess);
                log("====> cpp print load f address: %ld", loadFail);
                UpltvBridge::setRewardVideoLoadCallback(loadSuccess, loadFail);
            }
                break;
            case 2003: {
                log("====> cpp print showCall address: %ld", rdShowCallback);
                UpltvBridge::setRewardVideoShowCallback(rdShowCallback);
            }
                break;
            case 2004: {
                bool r = UpltvBridge::isRewardReady();
                log("====> cpp print isRewardReady : %s", (r?"true":"false"));
            }
                break;
            case 2005: {
                if (UpltvBridge::isRewardReady()) {
                    UpltvBridge::showRewardVideo(nullptr);
                }
            }
                break;
            case 3001: {
                UpltvBridge::showInterstitialDebugUI();
            }
                break;
            case 3002: {
                UpltvBridge::setInterstitialLoadCallback(ilkey.c_str(), iLLoadCallSuccess, iLLoadCallFail);
            }
                break;
            case 3003: {
                UpltvBridge::setInterstitialShowCallback(ilkey.c_str(), iLShowCallback);
            }
                break;
            case 3004: {
                UpltvBridge::isInterstitialReadyAsyn(ilkey.c_str(), iLReadyCallback);
            }
                break;
            case 3005: {
                if (UpltvBridge::isInterstitialReady(ilkey.c_str())) {
                    UpltvBridge::showInterstitialAd(ilkey.c_str());
                }
            }
                break;
            case 4001: {
                UpltvBridge::setBannerShowCallback(bannertopkey, bannerCallback);
                UpltvBridge::setBannerShowCallback(bannerbottomkey, bannerCallback);
            }
                break;
            case 4002: {
                UpltvBridge::showBannerAdAtTop(bannertopkey);
            }
                break;
            case 4003: {
                UpltvBridge::showBannerAdAtBottom(bannerbottomkey);
            }
                break;
            case 4004: {
                UpltvBridge::hideBannerAdAtTop();
                UpltvBridge::hideBannerAdAtBottom();
            }
                break;
            case 4005: {
                UpltvBridge::removeBannerAdAt(bannertopkey);
                UpltvBridge::removeBannerAdAt(bannerbottomkey);
            }
                break;
            case 5001: {
                UpltvBridge::setManifestPackageName("com.cc.test");
            }
                break;
            case 5002: {
                UpltvBridge::onBackPressed();
                UpltvBridge::setBackPressedCallback(exitAdCallback);
            }
                break;
            case 5003: {
                UpltvBridge::loadAdsByManual();
            }
                break;
            case 5004: {
                UpltvBridge::exitApp();
            }
                break;
            case 5005:{
                UpltvGDPRPermissionEnum::UPAccessPrivacyInfoStatus status = UpltvBridge::getAccessPrivacyInfoStatus();
                if (status != UpltvGDPRPermissionEnum::UPAccessPrivacyInfoStatus::UPAccessPrivacyInfoStatusUnkown) {
                    log("====> getAccessPrivacyInfoStatus is not unkown: %d", status);
                }
                UpltvBridge::updateAccessPrivacyInfoStatus(UpltvGDPRPermissionEnum::UPAccessPrivacyInfoStatus::UPAccessPrivacyInfoStatusUnkown);
                UpltvBridge::isEuropeanUnionUser(gdprUeropeanLocationCallback);
                if (status == UpltvGDPRPermissionEnum::UPAccessPrivacyInfoStatus::UPAccessPrivacyInfoStatusUnkown) {
                    UpltvBridge::notifyAccessPrivacyInfoStatus(dgprNotifyCallback);
                }
            }
            break;
            default: {
                
            }
            break;
        }
    }
}


void HelloWorld::menuCloseCallback(Ref *pSender) {
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
