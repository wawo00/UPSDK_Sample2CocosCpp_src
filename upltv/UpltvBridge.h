
#ifndef _UPLTV_BRIDGE_
#define _UPLTV_BRIDGE_

#include <string>
#include "cocos2d.h"

using namespace std;

class UpltvGDPRPermissionEnum {

public:
    enum class UPAccessPrivacyInfoStatus
    {
        UPAccessPrivacyInfoStatusUnkown = 0,
        UPAccessPrivacyInfoStatusAccepted = 1,
        UPAccessPrivacyInfoStatusDefined = 2,
    };

};

class UpltvAdEventEnum
{
public:
    enum class AdEventType
    {
        // 激励视屏回调事件类型
        VIDEO_EVENT_DID_SHOW = 0,
        VIDEO_EVENT_DID_CLICK = 1,
        VIDEO_EVENT_DID_CLOSE = 2,
        VIDEO_EVENT_DID_GIVEN_REWARD = 3,
        VIDEO_EVENT_DID_ABANDON_REWARD = 4,

        // 插屏回调事件类型
        INTERSTITIAL_EVENT_DID_SHOW = 5,
        INTERSTITIAL_EVENT_DID_CLICK = 6,
        INTERSTITIAL_EVENT_DID_CLOSE = 7,

        // Banner广告事件类型
        BANNER_EVENT_DID_SHOW = 8,
        BANNER_EVENT_DID_CLICK = 9,
        BANNER_EVENT_DID_REMOVED = 10,

        // exit 广告事件类型
        EXITAD_EVENT_DID_SHOW = 11,
        EXITAD_EVENT_DID_CLICK = 12,
        EXITAD_EVENT_DID_CLICKMORE = 13,
        EXITAD_EVENT_DID_EXIT = 14,
        EXITAD_EVENT_DID_CANCEL = 15,
    };

};

typedef void (*UpltvSdkBoolCallback)(bool r);
typedef void (*UpltvSdkStringBoolCallback)(const char* cpadid, bool r);
typedef void (*UpltvSdkStringCallback_1)(UpltvAdEventEnum::AdEventType type, const char* cpadid);
typedef void (*UpltvSdkStringCallback_2)(const char* cpadid, const char* msg);
typedef void (*UpltvSdkGDPRNotifyCallback)(UpltvGDPRPermissionEnum::UPAccessPrivacyInfoStatus status, const char* msg);

typedef struct UpltvCallInfo_
{
    int      size;
    int      type;
    string   key;
    int      ofsize;
    void * call;
} UpltvCallInfo;

class UpltvBridge {

public:
    /**
    * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    * >>>> CPP -- SDK初始化及App生命周期接口
    * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    */


    /**
     * 初始化upltv广告sdk
     * 请务必优先完成sdk初始化后，才能正常使用SDK的其它API接口
     * @param zone 产品发行的区域，0:海外，1:中国大陆，2:自动根据ip定位
     */
    static void initSdkByZone(int zone);

    /**
    * 初始化upltv广告sdk
    * 请务必优先完成sdk初始化后，才能正常使用SDK的其它API接口
    * @param zone 产品发行的区域，0:海外，1:中国大陆，2:自动根据ip定位
    * @param callback SDK初始化完成后的回调接口, 回调接口包含一个布尔参数 callback(boolean)，true表示成功，否则失败
    */
    static void initSdkByZoneWithCall(int zone, UpltvSdkBoolCallback callback);

    /**
     * 当前游戏或应用进行后台或前台时，请调用此接口通知upsdk状态的改变
     * 进行后台时，参数hasfocus设置为fasle，进行前台活跃时，设置为true
     * @param hasfocus
     */
    static void onApplicationFocus(bool hasfocus);


public:
    /**
     * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     * >>>> CPP -- SDK A/B广告测试相关接口
     * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     */

    /**
     * 进行A/B测试时，请在初始化SDK之后，调用此方法完成A/B Test 初始化
     * @param gameAccountId          const char*类型， 用户在游戏中的帐号id（必填）
     * @param completeTask           bool类型，是否完成了游戏中的新手任务
     * @param isPaid                 int类型，是否付费用户，0则未付费
     * @param promotionChannelName   const char*类型，推广渠道，没有可以传 ""
     * @param gender                 const char*，"M", "F"，未知可以传""
     * @param age                    int类型，未知可以传-1
     * @param tags                   vector类型，标签，没有可以传 nullptr
     */
    static void initAbtConfigJson(const char* gameAccountId, bool completeTask, int isPaid,
                                  const char* promotionChannelName, const char* gender, int age, vector<string> *tags);

    /**
     * 完成A/B Testing初始化后，通过此方法获取结果
     * 为了保证正确获取结果，调用时间建议延后initAbtConfigJson() 2秒以上
     * @param cpPlaceId  string类型， 广告位
     * @return const char*
     */
    static const char* getAbtConfig(const char* cpPlaceId);

public:
    /**
     * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     * >>>> CPP -- SDK 激励视屏广告相关接口
     * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     */

    /**
     * 打开激励视屏的debug界面
     */
    static void showRewardVideoDebugUI();

    /**
     * 设置激励视屏加载回调接口
     * 用于监听当前激励视屏的加载结果（成功或失败）
     * 此接口一旦回调，内部会自动释放，再次监听时需要重新设定回调接口
     * @param successCall 激励视屏加载成功时回调，successCall(cpadid, msg)
     * @param failCall    激励视屏加载失败时回调，failCall(cpadid, msg)
     */
    static void setRewardVideoLoadCallback(UpltvSdkStringCallback_2 successCall, UpltvSdkStringCallback_2 failCall);

    /**
     * 设置激励视屏展示回调接口，用于监听激励视屏广告的在某次展示时诸如点击，关闭，奖励发放等事件回调
     * 展示接口的引用会被内部保存，不会释放
     * 回调接口调用方式，callback(type, cpid)，type表示事件类型，有：展示回调，点击回调，关闭回调，激励发放成功回调，激励发放失败回调五种类型
     * @param callback(type, cpid)
     */
    static void setRewardVideoShowCallback(UpltvSdkStringCallback_1 callback);

    /**
     * 判断激励视屏是否准备好
     * 同步返回boolean结果，true 表示广告准备就绪可以展示，false表示广告还在请求中无法展示
     * 通常在showRewardVideo(cpPlaceId)前，调用此方法
     * @return bool
     */
    static bool isRewardReady();

    /**
     * 展示激励视屏
     * @param cpPlaceId 激励视屏展示时的广告位，用于业务打点，便于区分收益来源
     */
    static void showRewardVideo(const char* cpPlaceId);

public:
    /**
     * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     * >>>> CPP -- SDK 插屏广告相关接口
     * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     */

    /**
     * 根据广告位，判断某个插屏广告是否准备就绪r
     * 异步调用callback(cpid, r)，参数cpid为广告位，r为广告加载结果，true表示广告准备就绪可以展示，false表示广告还在请求中无法展示
     * @param cpPlaceId 广告位，必须正确指定，不能为空
     * @param callback 回调接口
     */
    static void isInterstitialReadyAsyn(const char* cpPlaceId, UpltvSdkStringBoolCallback callback);

    /**
     * 根据广告位，判断某个插屏广告是否准备就绪
     * 同步返回bool结果，true 表示广告准备就绪可以展示，false表示广告还在请求中无法展示
     * @param cpPlaceId 广告位
     * @return bool
     */
    static bool isInterstitialReady(const char* cpPlaceId);

    /**
     * 根据广告位，展示某个插屏广告
     * @param cpPlaceId 广告位
     */
    static void showInterstitialAd(const char* cpPlaceId);

    /**
     * 根据广告位，设置某个插屏广告加载回调接口
     * 用于监听插屏广告的加载结果（成功或失败）
     * 此接口一旦完成回调，内部会自动释放，再次监听时需要重新设定回调接口
     * @param cpPlaceId   广告位
     * @param loadSuccess 加载成功的回调接口, loadSuccess(cpid, message)
     * @param loadFail    加载失败的回调接口, loadFail(cpid, message)
     */
    static void setInterstitialLoadCallback(const char* cpPlaceId, UpltvSdkStringCallback_2 loadSuccess, UpltvSdkStringCallback_2 loadFail);

    /**
     * 设置插屏广告展示时的回调接口，用于监听插屏广告的在某次展示时诸如点击，关闭等事件回调
     * 插件展示回调接口的引用会被内部保存，不会释放
     * 回调接口调用方式，callback(type, cpid)，type表示事件类型，有：展示回调，点击回调，关闭回调三种类型
     * @param cpPlaceId
     * @param callback
     */
    static void setInterstitialShowCallback(const char* cpPlaceId, UpltvSdkStringCallback_1 callback);


    static void showInterstitialDebugUI();

public:
    /**
     * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     * >>>> CPP -- SDK Banner广告相关接口
     * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     */

    /**
     * 移除某个广告位的banner广告
     * @param cpPlaceId banner广告位
     */
    static void removeBannerAdAt(const char*cpPlaceId);

    /**
     * 将某个广告位的banner广告展示在屏幕顶部
     * @param cpPlaceId
     */
    static void showBannerAdAtTop(const char*cpPlaceId);

    /**
     * 将某个广告位的banner广告展示在屏幕底部
     * @param cpPlaceId
     */
    static void showBannerAdAtBottom(const char*cpPlaceId);


    /**
     * 隐藏当前屏幕的顶部banner广告
     */
    static void hideBannerAdAtTop();

    /**
     * 隐藏当前屏幕的底部广告
     */
    static void hideBannerAdAtBottom();

    /**
     * 设置某个banner广告位的展示的回调接口，回调接口会被保存只有调用upltv:removeBannerAdAt(cpPlaceId)才会被删除
     * @param cpPlaceId banner广告位
     * @param callback
     */
    static void setBannerShowCallback(const char* cpPlaceId, UpltvSdkStringCallback_1 callback);

    /**
    * 对Iphonex手机，顶部Banner被状态栏遮挡时，可以通过调节顶部Banner的偏移值，解决此问题
    * @param padding: 顶部Banner的偏移值，如32，则状态样会向下偏移32像素
    * Android平台不支持此功能
    * supported from 3002
    */
    static void setTopBannerPadingForIphonex(int padding);

public:
    /**
     * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     * >>>> CPP -- SDK 手动控制Upltv广告的加载
     * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     */

    /**
     * 满足需求：不希望在初始化自动加载广告，且要求根据游戏自主选择合适的时机进行广告加载
     * 运行条件：当sdk默认禁用广告自动加载的功能，且upltv后台云配也关闭此功能时
     * 如果以上条件不成立，即使调用以下方法，SDK也会自动忽略
     */
    static void loadAdsByManual();


    /**
     * 退出应用
     */
    static void exitApp();

public:
    /**
     * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     * >>>> CPP -- SDK Android平台 特殊接口
     * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     */

    /**
     * 仅用于android平台
     * 当mainifest的packagename与实际的名字不一致时，需要通过此方法设置当前Manifest中定义的PackageName
     * @param pkg Manifest中定义的PackageName
     */
    static void setManifestPackageName(const char* pkg);

    /**
     * 仅用于android平台
     * 用于展示upltv在安卓平台的退出广告
     */
    static void onBackPressed();

    /**
     * 设置激退出广告回调接口，用于监听退出广告的在某次展示时如打开，点击，关闭等事件回调
     * 展示接口的引用会被内部保存，不会释放
     * @param callback
     */
    static void setBackPressedCallback(UpltvSdkStringCallback_1 callback);

    /**
     * 向统计包的传递CustomerId(仅Android支持，对于非GP的包，可以传androidid)
     * 请在初始UPSDK之前调用
     * Version 3004(subversion 5) and above support this method
     */
    static void setCustomerId(const char* androidid);

public:
    /**
     * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     * >>>> CPP -- SDK GDPR接口
     * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     */

    /**
     * 外部进行GDPR授权时，将用户授权结果同步到UPSDK时，调用此方法
     * 请在初始UPSDK之前调用
     * @param status
     * Version 3003 and above support this method
     */
    static void updateAccessPrivacyInfoStatus(UpltvGDPRPermissionEnum::UPAccessPrivacyInfoStatus status);

    /**
     * 获取当前GDPR授权状态，返回UpltvGDPRPermissionEnum枚举值
     * 可在初始UPSDK之前调用
     * Version 3003 and above support this method
     */
    static UpltvGDPRPermissionEnum::UPAccessPrivacyInfoStatus getAccessPrivacyInfoStatus();

    /**
     * 弹出授权窗口，向用户说明重要数据收集的情况并询问用户是否同意授权
     * 如果用户拒绝授权将放弃相关数据的收集
     * 请在初始化UPSDK之前调用
     * @param callback
     * Version 3003 and above support this method
     */
    static void notifyAccessPrivacyInfoStatus(UpltvSdkGDPRNotifyCallback callback);

    /**
     * 判断用户是否属于欧盟地区
     * 异步回调，可以在初始化UPSDK之前调用
     * Version 3003 and above support this method
     */
    static void isEuropeanUnionUser(UpltvSdkBoolCallback callback);

public:
    /**
     * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     * >>>> CPP -- 插件内部调用接口
     * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     */
    static void runUpltvCallInfo(const char* key, bool r);
    static void runUpltvCallInfo(const char* key, int v, const char* msg);
    static void runUpltvCallInfo(string key, UpltvAdEventEnum::AdEventType type, const char* cpid, bool remove);
    static void runUpltvCallInfo(string key, int i, bool r, const char* cpid, const char* msg);
    static void initSdkSuccess();

    static void reportOnlineDebug(int type, const char* cpPlaceId, const char* msg);
    static bool isReportOnlineEnable();

private:

    static int64_t addCallIndex();
    static UpltvCallInfo initUpltvCallInfo(string key, int type, int size, void *ptr);
    static std::map<string, UpltvCallInfo> upltvCallMaps;
    static int64_t callIndex;

    static bool isInited;

};


#endif

