
#ifndef _UPLTV_ANDROID_BRIDGE_
#define _UPLTV_ANDROID_BRIDGE_

#include "UpltvBridge.h"
class UpltvAndroid {
public:

    static void initAndroidSdkByZone(int zone);
    static void initAndroidSdkByZoneWithCall(int zone, int64_t callback);
    static void onAndroidApplicationFocus(bool hasfocus);

    static void initAndroidAbtConfigJson(const char * gameAccountId, bool completeTask, int isPaid,
                                         const char* promotionChannelName, const char * gender, int age, const char* tags);

    static const char* getAndroidAbtConfig(const char* cpPlaceId);

    static void showAndroidRewardVideoDebugUI();

    static void setAndroidRewardVideoLoadCallback();

    static bool isAndroidRewardReady();

    static void showAndroidRewardVideo(const char* cpPlaceId);

    static void isAndroidInterstitialReadyAsyn(const char* cpPlaceId);

    static bool isAndroidInterstitialReady(const char* cpPlaceId);

    static void showAndroidInterstitialAd(const char* cpPlaceId);

    static void setAndroidInterstitialLoadCallback(const char* cpPlaceId);

    static void showAndroidInterstitialDebugUI();

    static void removeIosBannerAdAt(const char* cpPlaceId);

    static void showAndroidBannerAdAtTop(const char* cpPlaceId);

    static void showAndroidBannerAdAtBottom(const char* cpPlaceId);

    static void hideAndroidBannerAdAtTop();

    static void hideAndroidBannerAdAtBottom();

    static void loadAndroidAdsByManual();

    static void exitAndroidApp();

    static void onAndroidBackPressed();

    static void setCustomerId(const char* androidid);

    static void setAndroidManifestPackageName(const char* pkg);

    static void isEuropeanUnionUser(const char* key);

    static void updateAccessPrivacyInfoStatus(int value);

    static int getAccessPrivacyInfoStatus();

    static void notifyAccessPrivacyInfoStatus(const char * key);

    static bool isAndroidReportOnlineEnable();

    static void reportILShowDid(const char* cpPlaceId, const char* msg);

    static void reportILClick(const char* cpPlaceId, const char* msg);

    static void reportILClose(const char* cpPlaceId, const char* msg);

    static void reportRDShowDid(const char* msg);

    static void reportRDRewardGiven(const char* msg);

    static void reportRDRewardCancel(const char* msg);

    static void reportRDRewardClick(const char* msg);

    static void reportRDRewardClose(const char* msg);

    static void reportIvokePluginMethodReceive(const char* msg);
};

#endif