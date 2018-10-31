
#include "UpltvBridge.h"

#include<stdio.h>

USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "UpltvAndroid.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "UpltvIos.h"
#endif

#define CallType_BOOL_1       10001
#define CallType_BOOL_2       10002
#define CallType_BOOL_String  10003
#define CallType_String_1     20001
#define CallType_String_2     20002
#define CallType_GDPR_Notify     30001

#define RD_LoadCallback      "rd_key_load_101"
#define RD_ShowCallback      "rd_key_show_102"
#define BK_AndroidCallback   "bk_key_android_103"

//#define IL_READY_PRE "il_ready_"
#define IL_LOAD_PRE  "il_load_"
#define IL_SHOW_PRE  "il_show_"

#define Function_Reward_DidOpen      "reward_didopen"
#define Function_Reward_DidClick     "reward_didclick"
#define Function_Reward_DidClose     "reward_didclose"
#define Function_Reward_DidGivien    "reward_didgiven"
#define Function_Reward_DidAbandon   "reward_didabandon"

#define Function_Interstitial_Didshow    "interstitial_didshow"
#define Function_Interstitial_Didclose   "interstitial_didclose"
#define Function_Interstitial_Didclick   "interstitial_didclick"

#define Function_Banner_DidShow     "banner_didshow"
#define Function_Banner_DidClick    "banner_didclick"
#define Function_Banner_DidRemove   "banner_didremove"

#define Function_Reward_DidLoadFail      "reward_didloadfail"
#define Function_Reward_DidLoadSuccess   "reward_didloadsuccess"

#define Function_Interstitial_DidLoadFail      "interstitial_didloadfail"
#define Function_Interstitial_DidLoadSuccess   "interstitial_didloadsuccess"

#define Function_Invoke_Receive_ID       0

#define Function_Reward_DidOpen_ID       1
#define Function_Reward_DidClick_ID      2
#define Function_Reward_DidClose_ID      3
#define Function_Reward_DidGivien_ID     4
#define Function_Reward_DidAbandon_ID    5

#define Function_Interstitial_Didshow_ID     6
#define Function_Interstitial_Didclose_ID    7
#define Function_Interstitial_Didclick_ID    8


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #define Function_ExitAd_DidShow        "exitad_didshow"
    #define Function_ExitAd_DidClick       "exitad_didclick"
    #define Function_ExitAd_DidClickMore   "exitad_didclickmore"
    #define Function_ExitAd_DidExit        "exitad_onexit"
    #define Function_ExitAd_DidCancel      "exitad_oncancel"
#endif

//#define PrintLog 1

std::map<string, UpltvCallInfo> UpltvBridge::upltvCallMaps;

int64_t UpltvBridge::callIndex = 0;
bool    UpltvBridge::isInited = false;

static void initCallBySelf(bool r) {
    if (r) {
        UpltvBridge::initSdkSuccess();
    }
}

extern "C" void notifyUpltvCallback2(const char* keyi, int v, const char* msg) {
    //log("====> cpp notifyCallback: keyi = %ld, r = %s", keyi, (r?"true":"false"));
    UpltvBridge::runUpltvCallInfo(keyi, v, msg);
}

extern "C" void notifyUpltvCallback(const char* keyi, bool r) {
    //log("====> cpp notifyCallback: keyi = %ld, r = %s", keyi, (r?"true":"false"));
    UpltvBridge::runUpltvCallInfo(keyi, r);
}

extern "C" void invokeCocosCppMethod(const char* name, const char* cpid, const char* msg) {
    #ifdef PrintLog
        if (PrintLog) {
            log("====> cpp invokeCocosCppMethod: name = %s", name?name:"");
            log("====> cpp invokeCocosCppMethod: cpid = %s", cpid?cpid:"");
            log("====> cpp invokeCocosCppMethod: msg  = %s", msg?msg:"");
        }
    #endif

    if (name == nullptr) {
        return;
    }

    bool enableReport = UpltvBridge::isReportOnlineEnable();
    if (enableReport) {
        string fid(cpid?cpid:"");
        string fname(name?name:"");
        string fmsg = "CocosCpp Receive message, function:" + fname + ", placeid:" + fid;
        UpltvBridge::reportOnlineDebug(Function_Invoke_Receive_ID, nullptr, fmsg.c_str());
    }

    if (strcmp(name, Function_Reward_DidLoadSuccess) == 0) {
        UpltvBridge::runUpltvCallInfo(RD_LoadCallback, 0, true, cpid?cpid:"", msg?msg:"");
    } else if (strcmp(name, Function_Reward_DidLoadFail) == 0) {
        UpltvBridge::runUpltvCallInfo(RD_LoadCallback, 1, true, cpid?cpid:"", msg?msg:"");
    }
    else if (strcmp(name, Function_Reward_DidOpen) == 0) {
        UpltvBridge::runUpltvCallInfo(RD_ShowCallback, UpltvAdEventEnum::AdEventType::VIDEO_EVENT_DID_SHOW, cpid?cpid:"", false);
        if (enableReport) {
            UpltvBridge::reportOnlineDebug(Function_Reward_DidOpen_ID, cpid?cpid:"", "Cocos cpp did run on video opened event.");
        }
    }
    else if (strcmp(name, Function_Reward_DidClick) == 0) {
        UpltvBridge::runUpltvCallInfo(RD_ShowCallback, UpltvAdEventEnum::AdEventType::VIDEO_EVENT_DID_CLICK, cpid?cpid:"", false);
        if (enableReport) {
            UpltvBridge::reportOnlineDebug(Function_Reward_DidClick_ID, cpid?cpid:"", "Cocos cpp did run callback on video clicked event.");
        }
    }
    else if (strcmp(name, Function_Reward_DidClose) == 0) {
        UpltvBridge::runUpltvCallInfo(RD_ShowCallback, UpltvAdEventEnum::AdEventType::VIDEO_EVENT_DID_CLOSE, cpid?cpid:"", false);
        if (enableReport) {
           UpltvBridge::reportOnlineDebug(Function_Reward_DidClose_ID, cpid?cpid:"", "Cocos cpp did run callback on video closed event.");
        }
    }
    else if (strcmp(name, Function_Reward_DidGivien) == 0) {
        UpltvBridge::runUpltvCallInfo(RD_ShowCallback, UpltvAdEventEnum::AdEventType::VIDEO_EVENT_DID_GIVEN_REWARD, cpid?cpid:"", false);
        if (enableReport) {
           UpltvBridge::reportOnlineDebug(Function_Reward_DidGivien_ID, cpid?cpid:"", "Cocos cpp did run callback on video reward given event.");
        }
    }
    else if (strcmp(name, Function_Reward_DidAbandon) == 0) {
        UpltvBridge::runUpltvCallInfo(RD_ShowCallback, UpltvAdEventEnum::AdEventType::VIDEO_EVENT_DID_ABANDON_REWARD, cpid?cpid:"", false);
        if (enableReport) {
           UpltvBridge::reportOnlineDebug(Function_Reward_DidAbandon_ID, cpid?cpid:"", "Cocos cpp did run callback on video reward cancel event.");
        }
    }

    // 插屏广告回调
    else if (strcmp(name, Function_Interstitial_DidLoadSuccess) == 0) {
        string skey1(cpid?cpid:"");
        string skey = IL_LOAD_PRE + skey1;
        UpltvBridge::runUpltvCallInfo(skey, 0, true, cpid?cpid:"", msg?msg:"");
    }
    else if (strcmp(name, Function_Interstitial_DidLoadFail) == 0) {
        string skey1(cpid?cpid:"");
        string skey = IL_LOAD_PRE + skey1;
        UpltvBridge::runUpltvCallInfo(skey, 1, true, cpid?cpid:"", msg?msg:"");
    }
    else if (strcmp(name, Function_Interstitial_Didshow) == 0) {
        string skey1(cpid?cpid:"");
        string skey = IL_SHOW_PRE + skey1;
        UpltvBridge::runUpltvCallInfo(skey, UpltvAdEventEnum::AdEventType::INTERSTITIAL_EVENT_DID_SHOW, cpid?cpid:"", false);
        if (enableReport) {
           UpltvBridge::reportOnlineDebug(Function_Interstitial_Didshow_ID, cpid?cpid:"", "Cocos cpp did run callback on il ad shown event.");
        }
    }
    else if (strcmp(name, Function_Interstitial_Didclick) == 0) {
        string skey1(cpid?cpid:"");
        string skey = IL_SHOW_PRE + skey1;
        UpltvBridge::runUpltvCallInfo(skey, UpltvAdEventEnum::AdEventType::INTERSTITIAL_EVENT_DID_CLICK, cpid?cpid:"", false);
        if (enableReport) {
           UpltvBridge::reportOnlineDebug(Function_Interstitial_Didclick_ID, cpid?cpid:"", "Cocos cpp did run callback on il ad clicked event.");
        }
    }
    else if (strcmp(name, Function_Interstitial_Didclose) == 0) {
        string skey1(cpid);
        string skey = IL_SHOW_PRE + skey1;
        UpltvBridge::runUpltvCallInfo(skey, UpltvAdEventEnum::AdEventType::INTERSTITIAL_EVENT_DID_CLOSE, cpid?cpid:"", false);
        if (enableReport) {
           UpltvBridge::reportOnlineDebug(Function_Interstitial_Didclose_ID, cpid?cpid:"", "Cocos cpp did run callback on il ad closed event.");
        }
    }

    // banner广告回调
    else if (strcmp(name, Function_Banner_DidRemove) == 0) {
        UpltvBridge::runUpltvCallInfo(cpid?cpid:"", UpltvAdEventEnum::AdEventType::BANNER_EVENT_DID_REMOVED, cpid?cpid:"", true);
    }
    else if (strcmp(name, Function_Banner_DidShow) == 0) {
        UpltvBridge::runUpltvCallInfo(cpid?cpid:"", UpltvAdEventEnum::AdEventType::BANNER_EVENT_DID_SHOW, cpid?cpid:"", false);
    }
    else if (strcmp(name, Function_Banner_DidClick) == 0) {
        UpltvBridge::runUpltvCallInfo(cpid?cpid:"", UpltvAdEventEnum::AdEventType::BANNER_EVENT_DID_CLICK, cpid?cpid:"", false);
    } else {
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                if (strcmp(name, Function_ExitAd_DidShow) == 0) {
                    UpltvBridge::runUpltvCallInfo(BK_AndroidCallback, UpltvAdEventEnum::AdEventType::EXITAD_EVENT_DID_SHOW, cpid?cpid:"", false);
                }
                else if (strcmp(name, Function_ExitAd_DidClick) == 0) {
                    UpltvBridge::runUpltvCallInfo(BK_AndroidCallback, UpltvAdEventEnum::AdEventType::EXITAD_EVENT_DID_CLICK, cpid?cpid:"", false);
                }
                else if (strcmp(name, Function_ExitAd_DidClickMore) == 0) {
                    UpltvBridge::runUpltvCallInfo(BK_AndroidCallback, UpltvAdEventEnum::AdEventType::EXITAD_EVENT_DID_CLICKMORE, cpid?cpid:"", false);
                }
                else if (strcmp(name, Function_ExitAd_DidExit) == 0) {
                    UpltvBridge::runUpltvCallInfo(BK_AndroidCallback, UpltvAdEventEnum::AdEventType::EXITAD_EVENT_DID_EXIT, cpid?cpid:"", false);
                }
                else if (strcmp(name, Function_ExitAd_DidCancel) == 0) {
                    UpltvBridge::runUpltvCallInfo(BK_AndroidCallback, UpltvAdEventEnum::AdEventType::EXITAD_EVENT_DID_CANCEL, cpid?cpid:"", false);
                }
        #endif
    }

}

static void int2str(const int64_t &int_temp, string &string_temp)
{
    stringstream stream;
    stream<<int_temp;
    string_temp=stream.str();
}

int64_t UpltvBridge::addCallIndex() {
    UpltvBridge::callIndex ++;
    return UpltvBridge::callIndex;
}

void UpltvBridge::reportOnlineDebug(int type, const char* cpPlaceId, const char* msg) {

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

        if (Function_Invoke_Receive_ID == type) {
            UpltvAndroid::reportIvokePluginMethodReceive(msg);
        }
        else if (Function_Reward_DidOpen_ID == type) {
            UpltvAndroid::reportRDShowDid(msg);
        }
        else if (Function_Reward_DidClick_ID == type) {
            UpltvAndroid::reportRDRewardClick(msg);
        }
        else if (Function_Reward_DidClose_ID == type) {
            UpltvAndroid::reportRDRewardClose(msg);
        }
        else if (Function_Reward_DidGivien_ID == type) {
            UpltvAndroid::reportRDRewardGiven(msg);
        }
        else if (Function_Reward_DidAbandon_ID == type) {
            UpltvAndroid::reportRDRewardCancel(msg);
        }
        else if (Function_Interstitial_Didshow_ID == type) {
            UpltvAndroid::reportILShowDid(cpPlaceId, msg);
        }
        else if (Function_Interstitial_Didclose_ID == type) {
            UpltvAndroid::reportILClose(cpPlaceId, msg);
        }
        else if (Function_Interstitial_Didclick_ID == type) {
            UpltvAndroid::reportILClick(cpPlaceId, msg);
        }

    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if (Function_Invoke_Receive_ID == type) {
            UpltvIos::reportIvokePluginMethodReceive(msg);
        }
        else if (Function_Reward_DidOpen_ID == type) {
            UpltvIos::reportRDShowDid(msg);
        }
        else if (Function_Reward_DidClick_ID == type) {
            UpltvIos::reportRDRewardClick(msg);
        }
        else if (Function_Reward_DidClose_ID == type) {
            UpltvIos::reportRDRewardClose(msg);
        }
        else if (Function_Reward_DidGivien_ID == type) {
            UpltvIos::reportRDRewardGiven(msg);
        }
        else if (Function_Reward_DidAbandon_ID == type) {
            UpltvIos::reportRDRewardCancel(msg);
        }
        else if (Function_Interstitial_Didshow_ID == type) {
            UpltvIos::reportILShowDid(cpPlaceId, msg);
        }
        else if (Function_Interstitial_Didclose_ID == type) {
            UpltvIos::reportILClose(cpPlaceId, msg);
        }
        else if (Function_Interstitial_Didclick_ID == type) {
            UpltvIos::reportILClick(cpPlaceId, msg);
        }
    #endif
}

bool UpltvBridge::isReportOnlineEnable() {
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        return UpltvAndroid::isAndroidReportOnlineEnable();
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        return UpltvIos::isIosReportOnlineEnable();
    #else
        return false;
    #endif
}

void UpltvBridge::runUpltvCallInfo(string key, UpltvAdEventEnum::AdEventType type, const char* cpid, bool remove) {
    std::map<string, UpltvCallInfo>::iterator it =  UpltvBridge::upltvCallMaps.find(key);

    if (it != UpltvBridge::upltvCallMaps.end()) {

        UpltvCallInfo info = it->second;

        #ifdef PrintLog
            if (PrintLog) {
                log("====> runUpltvCallInfo 1111 val: etype = %zd, ktype:%d, key: %s", type, info.type, info.key.c_str());
            }
        #endif

        if (CallType_String_1 == info.type) {
            if (0 < info.size && info.call != nullptr) {
                UpltvSdkStringCallback_1 *pp = (UpltvSdkStringCallback_1 *)info.call;
                #ifdef PrintLog
                        if (PrintLog) {
                            log("===> cpp sizeof: %d", info.ofsize);
                            log("===> cpp funC[0]: %d, * %d", pp, *pp);
                        }
                #endif

                if (pp != nullptr && (*(pp)) != nullptr) {
                    log("=====> 111111");
                    (*(pp))(type, cpid);
                } else {
                    log("=====> can't run callback 11, missed the function pointer!!");
                }
                if (remove) {
                    free(pp);
                    info.size = 0;
                    info.call = nullptr;
                }
            }
            if (remove) {
                UpltvBridge::upltvCallMaps.erase(it);
            }
        }
    }
}

void UpltvBridge::runUpltvCallInfo(string key, int i, bool r, const char* cpid, const char* msg) {

    std::map<string, UpltvCallInfo>::iterator it =  UpltvBridge::upltvCallMaps.find(key);
    if (it != UpltvBridge::upltvCallMaps.end()) {

        UpltvCallInfo info = it->second;

        #ifdef PrintLog
            if (PrintLog) {
                log("====> runUpltvCallInfo 2222 val: i = %d, type: %d, size: %d, key: %s", i, info.type, info.size, info.key.c_str());
            }
        #endif

        if (CallType_String_2 == info.type) {
            if (0 < info.size && i < info.size && info.call != nullptr) {
//                void*id = info.funC[i];
//                log("=====> 11111");
//                log("===> cpp id: %d", id);
//                log("=====> 222222");
//                UpltvSdkStringCallback_2 *pp = static_cast<UpltvSdkStringCallback_2 *>(id);
//                log("=====> 33333");
//                log("===> cpp pp: %d", pp);
//                log("===> cpp pp: *%d", (*pp));
                UpltvSdkStringCallback_2 *pp = (UpltvSdkStringCallback_2 *)info.call;
                #ifdef PrintLog
                    if (PrintLog) {
                        log("===> cpp sizeof: %d", info.ofsize);
                        log("===> cpp funC[0]: %d, * %d", pp, *pp);
                        log("===> cpp funC[1]: %d, * %d", (pp + i), *(pp + i));
                    }
                #endif

                if (pp != nullptr && (*(pp + i)) != nullptr) {
                    (*(pp + i))(cpid, msg);
                } else {
                    log("=====> can't run callback22, missed the function pointer!!");
                }
                free(pp);
                info.size = 0;
                info.call = nullptr;
            }

            UpltvBridge::upltvCallMaps.erase(it);
        }
    }
}

void UpltvBridge::runUpltvCallInfo(const char* key, int v, const char* msg) {
    string keyi = key;
    std::map<string, UpltvCallInfo>::iterator it =  UpltvBridge::upltvCallMaps.find(keyi);
    if (it != UpltvBridge::upltvCallMaps.end()) {
        UpltvCallInfo info = it->second;

        #ifdef PrintLog
                if (PrintLog) {
                    log("====> runUpltvCallInfo val: %d, %d, %s", info.type, info.size, info.key.c_str());
                }
        #endif

        if (0 < info.size) {
            if (CallType_GDPR_Notify == info.type) {
                UpltvSdkGDPRNotifyCallback *pp = (UpltvSdkGDPRNotifyCallback *)info.call;
                #ifdef PrintLog
                    if (PrintLog) {
                        log("===> cpp sizeof: %d", info.ofsize);
                        log("===> cpp funC[0]: %d, * %d", pp, *pp);
                    }
                #endif
                if (pp != nullptr && (*pp) != nullptr) {
                    UpltvGDPRPermissionEnum::UPAccessPrivacyInfoStatus status = UpltvGDPRPermissionEnum::UPAccessPrivacyInfoStatus::UPAccessPrivacyInfoStatusUnkown;
                    if (v == 1) {
                        status = UpltvGDPRPermissionEnum::UPAccessPrivacyInfoStatus::UPAccessPrivacyInfoStatusAccepted;
                    }
                    else if (v == 2) {
                        status = UpltvGDPRPermissionEnum::UPAccessPrivacyInfoStatus::UPAccessPrivacyInfoStatusDefined;
                    }
                    (*pp)(status, msg);
                }
                free(pp);
                info.size = 0;
                info.call = nullptr;
                UpltvBridge::upltvCallMaps.erase(it);
            }
                
        }
    }
}

void UpltvBridge::runUpltvCallInfo(const char* key, bool r) {
    string keyi = key;
    std::map<string, UpltvCallInfo>::iterator it =  UpltvBridge::upltvCallMaps.find(keyi);
    if (it != UpltvBridge::upltvCallMaps.end()) {
        UpltvCallInfo info = it->second;

        #ifdef PrintLog
                if (PrintLog) {
                    log("====> runUpltvCallInfo val: %d, %d, %s", info.type, info.size, info.key.c_str());
                }
        #endif

        if (CallType_BOOL_1 == info.type
            || CallType_BOOL_2 == info.type) {

            if (0 < info.size) {
                //UpltvSdkBoolCallback *pp = static_cast<UpltvSdkBoolCallback*>(info.funC.at(0));
                UpltvSdkBoolCallback *pp = (UpltvSdkBoolCallback *)info.call;

                #ifdef PrintLog
                    if (PrintLog) {
                        log("===> cpp sizeof: %d", info.ofsize);
                        log("===> cpp funC[0]: %d, * %d", pp, *pp);
                    }
                #endif
                if (pp != nullptr && (*pp) != nullptr) {
                    (*pp)(r);
                    if (CallType_BOOL_1 == info.type) {
                        initCallBySelf(r);
                    }
                }
                free(pp);
                info.size = 0;
                info.call = nullptr;
                UpltvBridge::upltvCallMaps.erase(it);
            }
        } else if (CallType_BOOL_String == info.type) {
            if (0 < info.size) {
                UpltvSdkStringBoolCallback *pp = (UpltvSdkStringBoolCallback *)info.call;

                #ifdef PrintLog
                    if (PrintLog) {
                        log("===> cpp sizeof: %d", info.ofsize);
                        log("===> cpp funC[0]: %d, * %d", pp, *pp);
                    }
                #endif

                if (pp != nullptr && (*pp) != nullptr) {
                    (*pp)(key, r);
                }
                free(pp);
                info.size = 0;
                info.call = nullptr;
                UpltvBridge::upltvCallMaps.erase(it);
            }
        }
    }
}

UpltvCallInfo UpltvBridge::initUpltvCallInfo(string key, int type, int size, void *ptr) {
    UpltvCallInfo info;
    if (nullptr != ptr) {
        info.call = ptr;
        info.size = size;
    } else {
        info.size = 0;
        info.call = nullptr;
    }
    info.ofsize = 4;
    info.type = type;
    info.key = key;

    return info;
}

void UpltvBridge::initSdkSuccess() {
    UpltvBridge::isInited = true;
}

void UpltvBridge::initSdkByZone(int zone) {
    if (UpltvBridge::isInited) {
        log("====> has inited by : %d", zone);
        return;
    }

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        UpltvSdkBoolCallback callback = initCallBySelf;
        UpltvBridge::initSdkByZoneWithCall(zone, callback);
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UpltvSdkBoolCallback callback = initCallBySelf;
        UpltvBridge::initSdkByZoneWithCall(zone, callback);
    #endif
}

void UpltvBridge::initSdkByZoneWithCall(int zone, UpltvSdkBoolCallback callback) {

    if (UpltvBridge::isInited) {
        log("====> has inited by : %d", zone);
        return;
    }

    int64_t callid = UpltvBridge::addCallIndex();

    string keyi;
    int2str(callid, keyi);


    int ofsize = sizeof(UpltvSdkBoolCallback);
    UpltvSdkBoolCallback * call = (UpltvSdkBoolCallback*)malloc(ofsize*1);
    if (!call) {
        log("====> the initsdk callback，fail to malloc!");
        return;
    }

    *call = callback;

    #ifdef PrintLog
        if (PrintLog) {
            log("===> cpp sizeof: %d", ofsize);
            log("===> cpp funC[0]: %d, * %d", call, *call);
        }
    #endif

    UpltvCallInfo info = initUpltvCallInfo(keyi, CallType_BOOL_1, 1, call);
    info.ofsize = ofsize;
    UpltvBridge::upltvCallMaps.insert(std::make_pair(info.key, info));



    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        UpltvAndroid::initAndroidSdkByZoneWithCall(zone, callid);
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UpltvIos::initIosSdkByZoneWithCall(zone, callid);
    #endif

}

void UpltvBridge::onApplicationFocus(bool hasfocus) {

    if (!UpltvBridge::isInited) {
        log("====> please init the sdk first. ");
        return;
    }

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        UpltvAndroid::onAndroidApplicationFocus(hasfocus);
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    #endif
}

void UpltvBridge::initAbtConfigJson(const char* gameAccountId, bool completeTask, int isPaid,
                                    const char* promotionChannelName, const char* gender, int age, vector<string> *tags) {

    if (!UpltvBridge::isInited) {
        log("====> please init the sdk first. ");
        return;
    }

    string tagstring = "";
    if (tags != nullptr) {
        int size = tags->size();
        tagstring = "{\"array\":[";
        vector<string>::iterator it = tags->begin();
        for (int i = 0; i < size && it != tags->end(); i++, it++) {
            string s(it.base()->c_str());
            tagstring +=  "\"" + s;
            if (i < size - 1)
                tagstring += "\",";
            else
                tagstring += "\"]}";
        }
    }

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        UpltvAndroid::initAndroidAbtConfigJson(gameAccountId, completeTask, isPaid, promotionChannelName, gender, age, tagstring.c_str());
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UpltvIos::initIosAbtConfigJson(gameAccountId, completeTask, isPaid, promotionChannelName, gender, age, tagstring.c_str());
    #endif
}

const char* UpltvBridge::getAbtConfig(const char* cpPlaceId) {
    if (!UpltvBridge::isInited) {
        log("====> please init the sdk first. ");
        return "";
    }

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return UpltvAndroid::getAndroidAbtConfig(cpPlaceId == nullptr ? "" : cpPlaceId);
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return UpltvIos::getIosAbtConfig(cpPlaceId == nullptr ? "" : cpPlaceId);
    #endif
}

void UpltvBridge::showRewardVideoDebugUI() {
    if (!UpltvBridge::isInited) {
        log("====> please init the sdk first. ");
        return;
    }
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        UpltvAndroid::showAndroidRewardVideoDebugUI();
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UpltvIos::showIosRewardVideoDebugUI();
    #endif
}


void UpltvBridge::setRewardVideoLoadCallback(UpltvSdkStringCallback_2 successCall, UpltvSdkStringCallback_2 failCall) {

    if (!UpltvBridge::isInited) {
        log("====> please init the sdk first. ");
        return;
    }

    std::map<string, UpltvCallInfo>::iterator it =  UpltvBridge::upltvCallMaps.find(RD_LoadCallback);
    if (it != UpltvBridge::upltvCallMaps.end()) {
        log("====> the video load callback is exist. ");
        /*#ifdef PrintLog
                if (PrintLog) {
                    UpltvSdkStringCallback_2 *pp = (UpltvSdkStringCallback_2 *)it->second.call;
                    log("===> rdLoadCall cpp sizeof: %d", it->second.ofsize);
                    log("===> rdLoadCall cpp funC[0]: %d, * %d", pp, *pp);
                    log("===> rdLoadCall cpp funC[1]: %d, * %d", (pp + it->second.ofsize), *(pp + 1));
                }
        #endif*/
        return;
    }

    int ofsize = sizeof(UpltvSdkStringCallback_2);
    int size = 2;
    UpltvSdkStringCallback_2 * call = (UpltvSdkStringCallback_2*)malloc(ofsize*size);
    if (!call) {
        log("====> the video load callback，fail to malloc!");
        return;
    }
    memset(call, 0, ofsize*size);
    //log("===> cpp rdLoadCall call: %d", call);
    *call = successCall;
    *(call + 1) = failCall;

    #ifdef PrintLog
        if (PrintLog) {
            log("===> cpp rdLoadCall sizeof: %d", ofsize);
            log("===> cpp rdLoadCall funC[0]: %d, * %d", call, *call);
            log("===> cpp rdLoadCall funC[1]: %d, * %d", (call + ofsize), *(call + 1));
        }
    #endif

    UpltvCallInfo info = initUpltvCallInfo(RD_LoadCallback, CallType_String_2, size, call);
    info.ofsize = ofsize;

    UpltvBridge::upltvCallMaps.insert(std::make_pair(info.key, info));


    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        UpltvAndroid::setAndroidRewardVideoLoadCallback();
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UpltvIos::setIosRewardVideoLoadCallback();
    #endif
}

void UpltvBridge::setRewardVideoShowCallback(UpltvSdkStringCallback_1 callback) {


    if (!UpltvBridge::isInited) {
        log("====> please init the sdk first. ");
        return;
    }

    std::map<string, UpltvCallInfo>::iterator it =  UpltvBridge::upltvCallMaps.find(RD_ShowCallback);
    if (it != UpltvBridge::upltvCallMaps.end()) {
        log("====> the video show callback is exist. ");
        return;
    }

    int ofsize = sizeof(UpltvSdkStringCallback_1);
    UpltvSdkStringCallback_1 * call = (UpltvSdkStringCallback_1*)malloc(ofsize*1);
    if (!call) {
        log("====> the video show callback，fail to malloc!");
        return;
    }
    *call = callback;

    #ifdef PrintLog
        if (PrintLog) {
            log("===> cpp rdShowCall sizeof: %d", ofsize);
            log("===> cpp rdShowCall funC[0]: %d, * %d", call, *call);
        }
    #endif

    UpltvCallInfo info = initUpltvCallInfo(RD_ShowCallback, CallType_String_1, 1, call);
    info.ofsize = ofsize;

    UpltvBridge::upltvCallMaps.insert(std::make_pair(info.key, info));


    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    #endif
}

bool UpltvBridge::isRewardReady() {
    if (!UpltvBridge::isInited) {
        log("====> please init the sdk first. ");
        return false;
    }
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        return UpltvAndroid::isAndroidRewardReady();
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        return UpltvIos::isIosRewardReady();
    #endif
}

void UpltvBridge::showRewardVideo(const char* cpPlaceId) {
    if (!UpltvBridge::isInited) {
        log("====> please init the sdk first. ");
        return;
    }
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        UpltvAndroid::showAndroidRewardVideo(cpPlaceId);
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UpltvIos::showIosRewardVideo(cpPlaceId);
    #endif
}

void UpltvBridge::isInterstitialReadyAsyn(const char* cpPlaceId, UpltvSdkStringBoolCallback callback) {

    if (!UpltvBridge::isInited) {
        log("====> please init the sdk first. ");
        return;
    }

    if (cpPlaceId == nullptr) {
        log("====> the interstitial Ad param 'cpPlaceId' is null.");
        return;
    }

    string skey(cpPlaceId);
    //string skey = IL_READY_PRE + skey1;
    std::map<string, UpltvCallInfo>::iterator it =  UpltvBridge::upltvCallMaps.find(skey);
    if (it != UpltvBridge::upltvCallMaps.end()) {
        log("====> the interstitil load callback of '%s' is exist!", cpPlaceId);
        return;
    }

    int ofsize = sizeof(UpltvSdkStringCallback_1);
    UpltvSdkStringBoolCallback * call = (UpltvSdkStringBoolCallback*)malloc(ofsize*1);
    if (!call) {
        log("====> the interstitil load callback，fail to malloc!");
        return;
    }

    *call = callback;

    #ifdef PrintLog
        if (PrintLog) {
            log("===> cpp ilLoadCall sizeof: %d", ofsize);
            log("===> cpp ilLoadCall funC[0]: %d, * %d", call, *call);
        }
    #endif

    UpltvCallInfo info = initUpltvCallInfo(skey, CallType_BOOL_String, 1, call);
    info.ofsize = ofsize;

    UpltvBridge::upltvCallMaps.insert(std::make_pair(info.key, info));

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        UpltvAndroid::isAndroidInterstitialReadyAsyn(cpPlaceId);
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UpltvIos::isIosInterstitialReadyAsyn(cpPlaceId);
    #endif
}

bool UpltvBridge::isInterstitialReady(const char* cpPlaceId) {
    if (!UpltvBridge::isInited) {
        log("====> please init the sdk first. ");
        return false;
    }
    if (cpPlaceId == nullptr) {
        log("====> the interstitial Ad param 'cpPlaceId' is null.");
        return false;
    }

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        return UpltvAndroid::isAndroidInterstitialReady(cpPlaceId);
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        return UpltvIos::isIosInterstitialReady(cpPlaceId);
    #endif
}

void UpltvBridge::showInterstitialAd(const char* cpPlaceId) {
    if (!UpltvBridge::isInited) {
        log("====> please init the sdk first. ");
        return;
    }
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        UpltvAndroid::showAndroidInterstitialAd(cpPlaceId);
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UpltvIos::showIosInterstitialAd(cpPlaceId);
    #endif
}

void UpltvBridge::setInterstitialLoadCallback(const char* cpPlaceId, UpltvSdkStringCallback_2 loadSuccess, UpltvSdkStringCallback_2 loadFail) {

    if (!UpltvBridge::isInited) {
        log("====> please init the sdk first. ");
        return;
    }

    if (cpPlaceId == nullptr) {
        log("====> the interstitial Ad param 'cpPlaceId' is null.");
        return;
    }

    string skey1(cpPlaceId);
    string skey = IL_LOAD_PRE + skey1;
    std::map<string, UpltvCallInfo>::iterator it =  UpltvBridge::upltvCallMaps.find(skey);
    if (it != UpltvBridge::upltvCallMaps.end()) {
        log("====> the interstitial Ad load callback('%s') is exist. ", cpPlaceId);
        return;
    }


    int ofsize = sizeof(UpltvSdkStringCallback_2);
    UpltvSdkStringCallback_2 * call = (UpltvSdkStringCallback_2*)malloc(ofsize*2);
    if (!call) {
        log("====> the interstitil Ad callback，fail to malloc!");
        return;
    }

    *call = loadSuccess;
    *(call + 1) = loadFail;

    #ifdef PrintLog
        if (PrintLog) {
            log("===> cpp IL Load Call sizeof: %d", ofsize);
            log("===> cpp IL Load Call funC[0]: %d, * %d", call, *call);
            log("===> cpp IL Load Call funC[1]: %d, * %d", (call + ofsize), *(call + 1));
        }
    #endif

    UpltvCallInfo info = initUpltvCallInfo(skey, CallType_String_2, 2, call);
    info.ofsize = ofsize;

    UpltvBridge::upltvCallMaps.insert(std::make_pair(info.key, info));

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        UpltvAndroid::setAndroidInterstitialLoadCallback(cpPlaceId);
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UpltvIos::setIosInterstitialLoadCallback(cpPlaceId);
    #endif
}

void UpltvBridge::showInterstitialDebugUI() {
    if (!UpltvBridge::isInited) {
        log("====> please init the sdk first. ");
        return;
    }
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        UpltvAndroid::showAndroidInterstitialDebugUI();
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UpltvIos::showIosInterstitialDebugUI();
    #endif
}

void UpltvBridge::setInterstitialShowCallback(const char* cpPlaceId, UpltvSdkStringCallback_1 callback) {

    if (!UpltvBridge::isInited) {
        log("====> please init the sdk first. ");
        return;
    }

    if (cpPlaceId == nullptr) {
        log("====> the interstitial Ad param 'cpPlaceId' is null.");
        return;
    }

    string skey1(cpPlaceId);
    string skey = IL_SHOW_PRE + skey1;
    std::map<string, UpltvCallInfo>::iterator it =  UpltvBridge::upltvCallMaps.find(skey);
    if (it != UpltvBridge::upltvCallMaps.end()) {
        log("====> the interstitial Ad load callback('%s') is exist. ", cpPlaceId);
        return;
    }

    int ofsize = sizeof(UpltvSdkStringCallback_1);
    UpltvSdkStringCallback_1 * call = (UpltvSdkStringCallback_1*)malloc(ofsize*1);
    if (!call) {
        log("====> the interstitil Ad load callback，fail to malloc!");
        return;
    }

    *call = callback;

    #ifdef PrintLog
        if (PrintLog) {
            log("===> cpp rdShowCall sizeof: %d", ofsize);
            log("===> cpp rdShowCall funC[0]: %d, * %d", call, *call);
        }
    #endif

    UpltvCallInfo info = initUpltvCallInfo(skey, CallType_String_1, 1, call);
    info.ofsize = ofsize;

    UpltvBridge::upltvCallMaps.insert(std::make_pair(info.key, info));

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    #endif

}

void UpltvBridge::setTopBannerPadingForIphonex(int padding) {
    if (!UpltvBridge::isInited) {
        log("====> please init the sdk first. ");
        return;
    }

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UpltvIos::setIosTopBannerPadingForIphonex(padding);
    #endif
}

void UpltvBridge::setBannerShowCallback(const char* cpPlaceId, UpltvSdkStringCallback_1 callback) {
    if (!UpltvBridge::isInited) {
        log("====> please init the sdk first. ");
        return;
    }

    if (cpPlaceId == nullptr) {
        log("====> the banner Ad param 'cpPlaceId' is null.");
        return;
    }

    string skey(cpPlaceId);
    std::map<string, UpltvCallInfo>::iterator it =  UpltvBridge::upltvCallMaps.find(skey);
    if (it != UpltvBridge::upltvCallMaps.end()) {
        log("====> the banner Ad load callback('%s') is exist. ", cpPlaceId);
        return;
    }

    int ofsize = sizeof(UpltvSdkStringCallback_1);
    UpltvSdkStringCallback_1 * call = (UpltvSdkStringCallback_1*)malloc(ofsize*1);
    if (!call) {
        log("====> the banner Ad load callback，fail to malloc!");
        return;
    }
    *call = callback;

    #ifdef PrintLog
        if (PrintLog) {
            log("===> cpp banner Call sizeof: %d", ofsize);
            log("===> cpp banner Call funC[0]: %d, * %d", call, *call);
        }
    #endif

    UpltvCallInfo info = initUpltvCallInfo(skey, CallType_String_1, 1, call);
    info.ofsize = ofsize;

    UpltvBridge::upltvCallMaps.insert(std::make_pair(info.key, info));
}

void UpltvBridge::removeBannerAdAt(const char*cpPlaceId) {
    if (!UpltvBridge::isInited) {
        log("====> please init the sdk first. ");
        return;
    }
    if (cpPlaceId == nullptr) {
        log("====> the banner Ad param 'cpPlaceId' is null.");
        return;
    }
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        UpltvAndroid::removeIosBannerAdAt(cpPlaceId);
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UpltvIos::removeIosBannerAdAt(cpPlaceId);
    #endif
}

void UpltvBridge::showBannerAdAtTop(const char*cpPlaceId) {
    if (!UpltvBridge::isInited) {
        log("====> please init the sdk first. ");
        return;
    }
    if (cpPlaceId == nullptr) {
        log("====> the banner Ad param 'cpPlaceId' is null.");
        return;
    }
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        UpltvAndroid::showAndroidBannerAdAtTop(cpPlaceId);
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UpltvIos::showIosBannerAdAtTop(cpPlaceId);
    #endif
}

void UpltvBridge::showBannerAdAtBottom(const char*cpPlaceId) {
    if (!UpltvBridge::isInited) {
        log("====> please init the sdk first. ");
        return;
    }
    if (cpPlaceId == nullptr) {
        log("====> the banner Ad param 'cpPlaceId' is null.");
        return;
    }
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        UpltvAndroid::showAndroidBannerAdAtBottom(cpPlaceId);
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UpltvIos::showIosBannerAdAtBottom(cpPlaceId);
    #endif
}

void UpltvBridge::hideBannerAdAtTop() {
    if (!UpltvBridge::isInited) {
        log("====> please init the sdk first. ");
        return;
    }
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        UpltvAndroid::hideAndroidBannerAdAtTop();
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UpltvIos::hideIosBannerAdAtTop();
    #endif
}

void UpltvBridge::hideBannerAdAtBottom() {
    if (!UpltvBridge::isInited) {
        log("====> please init the sdk first. ");
        return;
    }
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        UpltvAndroid::hideAndroidBannerAdAtBottom();
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UpltvIos::hideIosBannerAdAtBottom();
    #endif
}

void UpltvBridge::loadAdsByManual() {
    if (!UpltvBridge::isInited) {
        log("====> please init the sdk first. ");
        return;
    }
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        UpltvAndroid::loadAndroidAdsByManual();
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UpltvIos::loadIosAdsByManual();
    #endif
}

void UpltvBridge::exitApp() {
    if (!UpltvBridge::isInited) {
        log("====> please init the sdk first. ");
        return;
    }
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        UpltvAndroid::exitAndroidApp();
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UpltvIos::exitIosApp();
    #endif
}

void UpltvBridge::setManifestPackageName(const char* pkg) {

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if (pkg == nullptr) {
            log("====> the param 'pkg' is null.");
            return;
        }
        UpltvAndroid::setAndroidManifestPackageName(pkg);
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    #endif
}

void UpltvBridge::onBackPressed() {

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if (!UpltvBridge::isInited) {
            log("====> please init the sdk first. ");
            return;
        }
        UpltvAndroid::onAndroidBackPressed();
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    #endif
}

void UpltvBridge::setBackPressedCallback(UpltvSdkStringCallback_1 callback) {

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

        if (!UpltvBridge::isInited) {
            log("====> please init the sdk first. ");
            return;
        }

        std::map<string, UpltvCallInfo>::iterator it =  UpltvBridge::upltvCallMaps.find(BK_AndroidCallback);
        if (it != UpltvBridge::upltvCallMaps.end()) {
            log("====> the backpress callback is exist. ");
            return;
        }

        int ofsize = sizeof(UpltvSdkStringCallback_1);
        UpltvSdkStringCallback_1 * call = (UpltvSdkStringCallback_1*)malloc(ofsize*1);
        if (!call) {
            log("====> the backpress callback，fail to malloc!");
            return;
        }
        *call = callback;

        #ifdef PrintLog
            if (PrintLog) {
                log("===> cpp rdShowCall sizeof: %d", ofsize);
                log("===> cpp rdShowCall funC[0]: %d, * %d", call, *call);
            }
        #endif

        UpltvCallInfo info = initUpltvCallInfo(BK_AndroidCallback, CallType_String_1, 1, call);
        info.ofsize = ofsize;

        UpltvBridge::upltvCallMaps.insert(std::make_pair(info.key, info));

    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    #endif
}

void UpltvBridge::setCustomerId(const char* androidid) {
    if (androidid == nullptr) {
        log("====> setCustomerId() is called，but androidid is NULL!");
        return;
    }
    //log("====> setCustomerId() is called!");
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            UpltvAndroid::setCustomerId(androidid);
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #endif
}


void UpltvBridge::updateAccessPrivacyInfoStatus(UpltvGDPRPermissionEnum::UPAccessPrivacyInfoStatus status) {
    int value = 0;
    if (status == UpltvGDPRPermissionEnum::UPAccessPrivacyInfoStatus::UPAccessPrivacyInfoStatusAccepted) {
        value = 1;
    }
    else if (status == UpltvGDPRPermissionEnum::UPAccessPrivacyInfoStatus::UPAccessPrivacyInfoStatusDefined) {
        value = 2;
    }
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        UpltvAndroid::updateAccessPrivacyInfoStatus(value);
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UpltvIos::updateIosAccessPrivacyInfoStatus(value);
    #endif
}

void UpltvBridge::notifyAccessPrivacyInfoStatus(UpltvSdkGDPRNotifyCallback callback) {

    int64_t callid = UpltvBridge::addCallIndex();

    string keyi;
    int2str(callid, keyi);


    int ofsize = sizeof(UpltvSdkGDPRNotifyCallback);
    UpltvSdkGDPRNotifyCallback * call = (UpltvSdkGDPRNotifyCallback*)malloc(ofsize*1);
    if (!call) {
        log("====> the notifyAccessPrivacyInfoStatus callback，fail to malloc!");
        return;
    }

    *call = callback;

    #ifdef PrintLog
        if (PrintLog) {
            log("===> cpp sizeof: %d", ofsize);
            log("===> cpp funC[0]: %d, * %d", call, *call);
        }
    #endif

    UpltvCallInfo info = initUpltvCallInfo(keyi, CallType_GDPR_Notify, 1, call);
    info.ofsize = ofsize;
    UpltvBridge::upltvCallMaps.insert(std::make_pair(info.key, info));

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        UpltvAndroid::notifyAccessPrivacyInfoStatus(keyi.c_str());
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UpltvIos::notifyIosAccessPrivacyInfoStatus(keyi.c_str());
    #endif
}

void UpltvBridge::isEuropeanUnionUser(UpltvSdkBoolCallback callback) {
    int64_t callid = UpltvBridge::addCallIndex();

    string keyi;
    int2str(callid, keyi);


    int ofsize = sizeof(UpltvSdkBoolCallback);
    UpltvSdkBoolCallback * call = (UpltvSdkBoolCallback*)malloc(ofsize*1);
    if (!call) {
        log("====> the isEuropeanUnionUser callback，fail to malloc!");
        return;
    }

    *call = callback;

    #ifdef PrintLog
        if (PrintLog) {
            log("===> cpp sizeof: %d", ofsize);
            log("===> cpp funC[0]: %d, * %d", call, *call);
        }
    #endif

    UpltvCallInfo info = initUpltvCallInfo(keyi, CallType_BOOL_2, 1, call);
    info.ofsize = ofsize;
    UpltvBridge::upltvCallMaps.insert(std::make_pair(info.key, info));

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        UpltvAndroid::isEuropeanUnionUser(keyi.c_str());
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UpltvIos::isIosEuropeanUnionUser(keyi.c_str());
    #endif
}

UpltvGDPRPermissionEnum::UPAccessPrivacyInfoStatus UpltvBridge::getAccessPrivacyInfoStatus() {
    int value = 0;
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
       value = UpltvAndroid::getAccessPrivacyInfoStatus();
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        value = UpltvIos::getIosAccessPrivacyInfoStatus();
    #endif
    log("====> the getAccessPrivacyInfoStatus :%d ", value);
    
    switch (value) {
        case 1:
            return UpltvGDPRPermissionEnum::UPAccessPrivacyInfoStatus::UPAccessPrivacyInfoStatusAccepted;
        case 2:
            return UpltvGDPRPermissionEnum::UPAccessPrivacyInfoStatus::UPAccessPrivacyInfoStatusDefined;
        default:
            return UpltvGDPRPermissionEnum::UPAccessPrivacyInfoStatus::UPAccessPrivacyInfoStatusUnkown;
    }
}
