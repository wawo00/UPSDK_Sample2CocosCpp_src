#include "UpltvAndroid.h"


#define CLASS_JAVA_COCOS_NAME "com/upltv/ads/cpp/UpltvJavaHelper"
#define CLASS_JAVA_SDK_NAME "com/up/ads/cocoscpp/CppProxy"
using namespace cocos2d;

USING_NS_CC;

static jstring strToJstring(JNIEnv* env, const char* pStr)
{
    int        strLen    = strlen(pStr);

    jclass     jstrObj   = (*env).FindClass("java/lang/String");
    jmethodID  methodId  = (*env).GetMethodID(jstrObj, "<init>", "([BLjava/lang/String;)V");
    jbyteArray byteArray = (*env).NewByteArray(strLen);
    jstring    encode    = (*env).NewStringUTF("utf-8");

    (*env).SetByteArrayRegion(byteArray, 0, strLen, (jbyte*)pStr);

    jstring jStr = (jstring)(*env).NewObject(jstrObj, methodId, byteArray, encode);

    (*env).DeleteLocalRef(jstrObj);
    (*env).DeleteLocalRef(byteArray);
    (*env).DeleteLocalRef(encode);
    //(*env).DeleteLocalRef(methodId);
    return jStr;
}

static char* jstringTostr(JNIEnv* env, jstring jstr)
{
    char* pStr = NULL;

    jclass     jstrObj   = (*env).FindClass("java/lang/String");
    jstring    encode    = (*env).NewStringUTF("utf-8");
    jmethodID  methodId  = (*env).GetMethodID(jstrObj, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray byteArray = (jbyteArray)(*env).CallObjectMethod(jstr, methodId, encode);
    jsize      strLen    = (*env).GetArrayLength(byteArray);
    jbyte      *jBuf     = (*env).GetByteArrayElements(byteArray, JNI_FALSE);

    if (jBuf)
    {
        pStr = (char*)malloc(strLen + 1);

        if (!pStr)
        {
            return NULL;
        }

        memcpy(pStr, jBuf, strLen);

        pStr[strLen] = '\0';
    }

    (*env).ReleaseByteArrayElements(byteArray, jBuf, 0);

    (*env).DeleteLocalRef(jstrObj);
    (*env).DeleteLocalRef(byteArray);
    (*env).DeleteLocalRef(encode);
    //(*env).DeleteLocalRef(methodId);

    return pStr;
}

static void callHelpler() {

    cocos2d::JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_COCOS_NAME ,"invokeCppSdk", "()V");
    if (!isHave) {
        log("===> cpp jni->%s/callHelpler: not exist!!!!!", CLASS_JAVA_COCOS_NAME);
    }
    else {
        //log("jni->%s/callHelpler: exist !!!!", CLASS_JAVA_SDK_NAME);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}

static bool initSdkNow(int zone) {
    cocos2d::JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"initSdkByZone", "(I)V");
    if (!isHave) {
        log("===> cpp jni->%s/callJni: not exist!!!!!", CLASS_JAVA_SDK_NAME);
        return false;
    }
    else {
        //log("jni->%s/callJni: exist !!!!", CLASS_JAVA_SDK_NAME);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, zone);
        return true;
    }
}

static bool initSdkNow(int zone, int64_t callback) {
    cocos2d::JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"initSdkByZoneWithCall", "(IJ)V");
    if (!isHave) {
        log("===> cpp jni->%s/callJni: not exist!!!!!", CLASS_JAVA_SDK_NAME);
        return false;
    }
    else {
        //log("jni->%s/callJni: exist !!!!", CLASS_JAVA_SDK_NAME);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, zone, callback);
        return true;
    }
}


void  UpltvAndroid::initAndroidSdkByZone(int zone) {
    callHelpler();
    initSdkNow(zone);
}

void  UpltvAndroid::initAndroidSdkByZoneWithCall(int zone, int64_t callback) {
    callHelpler();
    initSdkNow(zone, callback);
}

void UpltvAndroid::onAndroidApplicationFocus(bool hasfocus) {
    cocos2d::JniMethodInfo minfo;
    string sigs = "(Z)V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"onApplicationFocus", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni onApplicationFocus(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
    }
    else {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, hasfocus);
    }
}

void UpltvAndroid::initAndroidAbtConfigJson(const char * gameAccountId, bool completeTask, int isPaid,
                                            const char* promotionChannelName, const char * gender, int age, const char* tags) {
    cocos2d::JniMethodInfo minfo;
    string sigs = "(Ljava/lang/String;ZILjava/lang/String;Ljava/lang/String;ILjava/lang/String;)V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"initAbtConfigJsonForCpp", sigs.c_str());
    if (!isHave) {
        log("===> cpp ni->%s/callJni initAbtConfigJsonForCpp(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
    }
    else {
        //log("jni->%s/callJni initAbtConfigJsonForCpp(): exist !!!!", CLASS_JAVA_SDK_NAME);
        jstring jgameid = strToJstring(minfo.env, gameAccountId==nullptr?"":gameAccountId);
        jstring jcname = strToJstring(minfo.env, promotionChannelName==nullptr?"":promotionChannelName);
        jstring jgender = strToJstring(minfo.env, gender==nullptr?"":gender);
        jstring jtags = strToJstring(minfo.env, tags==nullptr?"":tags);

        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jgameid, completeTask, isPaid,
                                        jcname, jgender, age, jtags);
    }
}

const char* UpltvAndroid::getAndroidAbtConfig(const char* cpPlaceId) {
    cocos2d::JniMethodInfo minfo;
    string sigs = "(Ljava/lang/String;)Ljava/lang/String;";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"getAbtConfigForCpp", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni getAndroidAbtConfig(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
        return "";
    }
    else {
        //log("jni->%s/callJni getAndroidAbtConfig(): exist !!!!", CLASS_JAVA_SDK_NAME);
        jstring jcpPlaceId = strToJstring(minfo.env, cpPlaceId==nullptr?"":cpPlaceId);
        jobject  jobject1 = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID, jcpPlaceId);
        if (jobject1 == nullptr) {
            return "";
        }
        return jstringTostr(minfo.env, (jstring)jobject1);
    }
}

void UpltvAndroid::showAndroidRewardVideoDebugUI() {
    cocos2d::JniMethodInfo minfo;
    string sigs = "()V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"showRewardDebugActivityForCpp", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni showAndroidRewardVideoDebugUI(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
    }
    else {
        //log("jni->%s/callJni showAndroidRewardVideoDebugUI(): exist !!!!", CLASS_JAVA_SDK_NAME);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}

void UpltvAndroid::setAndroidRewardVideoLoadCallback() {
    cocos2d::JniMethodInfo minfo;
    string sigs = "()V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"setRewardVideoLoadCallbackForCpp", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni setAndroidRewardVideoLoadCallback(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
    }
    else {
        //log("jni->%s/callJni setAndroidRewardVideoLoadCallback(): exist !!!!", CLASS_JAVA_SDK_NAME);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}

void UpltvAndroid::setCustomerId(const char* androidid) {
    cocos2d::JniMethodInfo minfo;
    string sigs = "(Ljava/lang/String;)V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"setCustomerIdForCpp", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni setCustomerId(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
    }
    else {
        //log("jni->%s/callJni setCustormerId(): exist !!!!", CLASS_JAVA_SDK_NAME);
        jstring jandroidid = strToJstring(minfo.env, androidid==nullptr?"":androidid);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jandroidid);
    }
}

bool UpltvAndroid::isAndroidRewardReady() {
    cocos2d::JniMethodInfo minfo;
    string sigs = "()Z";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"isRewardReady", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni isAndroidRewardReady(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
        return false;
    }
    else {
        //log("jni->%s/callJni isAndroidRewardReady(): exist !!!!", CLASS_JAVA_SDK_NAME);
        return minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
    }
}

void UpltvAndroid::showAndroidRewardVideo(const char* cpPlaceId) {
    cocos2d::JniMethodInfo minfo;
    string sigs = "(Ljava/lang/String;)V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"showRewardVideo", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni showAndroidRewardVideo(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
    }
    else {
        //log("jni->%s/callJni showAndroidRewardVideo(): exist !!!!", CLASS_JAVA_SDK_NAME);
        jstring jcpPlaceId = strToJstring(minfo.env, cpPlaceId==nullptr?"video_reward":cpPlaceId);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jcpPlaceId);
    }
}

void UpltvAndroid::isAndroidInterstitialReadyAsyn(const char* cpPlaceId) {
    cocos2d::JniMethodInfo minfo;
    string sigs = "(Ljava/lang/String;)V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"isInterstitialReadyForCpp", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni isAndroidInterstitialReadyAsyn(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
    }
    else {
        //log("jni->%s/callJni isAndroidInterstitialReadyAsyn(): exist !!!!", CLASS_JAVA_SDK_NAME);
        jstring jcpPlaceId = strToJstring(minfo.env, cpPlaceId==nullptr?"":cpPlaceId);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jcpPlaceId);
    }
}

bool UpltvAndroid::isAndroidInterstitialReady(const char* cpPlaceId) {
    cocos2d::JniMethodInfo minfo;
    string sigs = "(Ljava/lang/String;)Z";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"isInterstitialReady", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni isAndroidInterstitialReady(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
        return false;
    }
    else {
        //log("jni->%s/callJni isAndroidInterstitialReady(): exist !!!!", CLASS_JAVA_SDK_NAME);
        jstring jcpPlaceId = strToJstring(minfo.env, cpPlaceId==nullptr?"":cpPlaceId);
        return minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID, jcpPlaceId);
    }
}

void UpltvAndroid::showAndroidInterstitialAd(const char* cpPlaceId) {
    cocos2d::JniMethodInfo minfo;
    string sigs = "(Ljava/lang/String;)V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"showInterstitialForCpp", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni showAndroidInterstitialAd(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
    }
    else {
        //log("jni->%s/callJni showAndroidInterstitialAd(): exist !!!!", CLASS_JAVA_SDK_NAME);
        jstring jcpPlaceId = strToJstring(minfo.env, cpPlaceId==nullptr?"":cpPlaceId);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jcpPlaceId);
    }
}

void UpltvAndroid::setAndroidInterstitialLoadCallback(const char* cpPlaceId) {
    cocos2d::JniMethodInfo minfo;
    string sigs = "(Ljava/lang/String;)V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"setInterstitialCallbackAt", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni setAndroidInterstitialLoadCallback(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
    }
    else {
        //log("jni->%s/callJni setAndroidInterstitialLoadCallback(): exist !!!!", CLASS_JAVA_SDK_NAME);
        jstring jcpPlaceId = strToJstring(minfo.env, cpPlaceId==nullptr?"":cpPlaceId);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jcpPlaceId);
    }
}

void UpltvAndroid::showAndroidInterstitialDebugUI() {
    cocos2d::JniMethodInfo minfo;
    string sigs = "()V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"showInterstitialDebugActivityForCpp", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni showAndroidInterstitialDebugUI(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
    }
    else {
        //log("jni->%s/callJni showAndroidInterstitialDebugUI(): exist !!!!", CLASS_JAVA_SDK_NAME);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}

void UpltvAndroid::removeIosBannerAdAt(const char* cpPlaceId) {
    cocos2d::JniMethodInfo minfo;
    string sigs = "(Ljava/lang/String;)V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"removeBanner", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni removeIosBannerAdAt(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
    }
    else {
        //log("jni->%s/callJni removeIosBannerAdAt(): exist !!!!", CLASS_JAVA_SDK_NAME);
        jstring jcpPlaceId = strToJstring(minfo.env, cpPlaceId==nullptr?"":cpPlaceId);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jcpPlaceId);
    }
}

void UpltvAndroid::showAndroidBannerAdAtTop(const char* cpPlaceId) {
    cocos2d::JniMethodInfo minfo;
    string sigs = "(Ljava/lang/String;)V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"showTopBanner", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni showAndroidBannerAdAtTop(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
    }
    else {
        //log("jni->%s/callJni showAndroidBannerAdAtTop(): exist !!!!", CLASS_JAVA_SDK_NAME);
        jstring jcpPlaceId = strToJstring(minfo.env, cpPlaceId==nullptr?"":cpPlaceId);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jcpPlaceId);
    }
}

void UpltvAndroid::showAndroidBannerAdAtBottom(const char* cpPlaceId) {
    cocos2d::JniMethodInfo minfo;
    string sigs = "(Ljava/lang/String;)V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"showBottomBanner", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni showAndroidBannerAdAtBottom(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
    }
    else {
        //log("jni->%s/callJni showAndroidBannerAdAtBottom(): exist !!!!", CLASS_JAVA_SDK_NAME);
        jstring jcpPlaceId = strToJstring(minfo.env, cpPlaceId==nullptr?"":cpPlaceId);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jcpPlaceId);
    }
}

void UpltvAndroid::hideAndroidBannerAdAtTop() {
    cocos2d::JniMethodInfo minfo;
    string sigs = "()V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"hideTopBanner", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni hideAndroidBannerAdAtTop(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
    }
    else {
        //log("jni->%s/callJni hideAndroidBannerAdAtTop(): exist !!!!", CLASS_JAVA_SDK_NAME);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}


void UpltvAndroid::hideAndroidBannerAdAtBottom() {
    cocos2d::JniMethodInfo minfo;
    string sigs = "()V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"hideBottomBanner", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni hideAndroidBannerAdAtBottom(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
    }
    else {
        //log("jni->%s/callJni hideAndroidBannerAdAtBottom(): exist !!!!", CLASS_JAVA_SDK_NAME);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}

void UpltvAndroid::loadAndroidAdsByManual() {
    cocos2d::JniMethodInfo minfo;
    string sigs = "()V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"loadAnroidAdsByManual", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni loadAndroidAdsByManual(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
    }
    else {
        //log("jni->%s/callJni loadAndroidAdsByManual(): exist !!!!", CLASS_JAVA_SDK_NAME);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}

void UpltvAndroid::exitAndroidApp() {
    cocos2d::JniMethodInfo minfo;
    string sigs = "()V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"exitAndroidApp", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni exitAndroidApp(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
    }
    else {
        //log("jni->%s/callJni exitAndroidApp(): exist !!!!", CLASS_JAVA_SDK_NAME);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}

void UpltvAndroid::onAndroidBackPressed() {
    cocos2d::JniMethodInfo minfo;
    string sigs = "()V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"onBackPressed", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni onAndroidBackPressed(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
    }
    else {
        //log("jni->%s/callJni onAndroidBackPressed(): exist !!!!", CLASS_JAVA_SDK_NAME);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}

void UpltvAndroid::setAndroidManifestPackageName(const char* pkg) {
    cocos2d::JniMethodInfo minfo;
    string sigs = "(Ljava/lang/String;)V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"setManifestPackageName", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni setAndroidManifestPackageName(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
    }
    else {
        //log("jni->%s/callJni setAndroidManifestPackageName(): exist !!!!", CLASS_JAVA_SDK_NAME);
        jstring jpkg = strToJstring(minfo.env, pkg==nullptr?"":pkg);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jpkg);
    }
}

void UpltvAndroid::notifyAccessPrivacyInfoStatus(const char * key) {
    cocos2d::JniMethodInfo minfo;
    string sigs = "(Ljava/lang/String;)V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"notifyAccessPrivacyInfoStatus", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni notifyAccessPrivacyInfoStatus(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
    }
    else {
        //log("jni->%s/callJni setAndroidManifestPackageName(): exist !!!!", CLASS_JAVA_SDK_NAME);
        jstring jkey = strToJstring(minfo.env, key==nullptr?"":key);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jkey);
    }
}

void UpltvAndroid::isEuropeanUnionUser(const char* key) {
    cocos2d::JniMethodInfo minfo;
    string sigs = "(Ljava/lang/String;)V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"isEuropeanUnionUser", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni isEuropeanUnionUser(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
    }
    else {
        //log("jni->%s/callJni setAndroidManifestPackageName(): exist !!!!", CLASS_JAVA_SDK_NAME);
        jstring jkey = strToJstring(minfo.env, key==nullptr?"":key);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jkey);
    }
}

void UpltvAndroid::updateAccessPrivacyInfoStatus(int value) {
    cocos2d::JniMethodInfo minfo;
    string sigs = "(I)V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"updateAccessPrivacyInfoStatus", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni updateAccessPrivacyInfoStatus(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
    }
    else {
        //log("jni->%s/callJni setAndroidManifestPackageName(): exist !!!!", CLASS_JAVA_SDK_NAME);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, value);
    }
}

int UpltvAndroid::getAccessPrivacyInfoStatus() {
    cocos2d::JniMethodInfo minfo;
    string sigs = "()I";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"getAccessPrivacyInfoStatus", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni getAccessPrivacyInfoStatus(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
        return 0;
    }
    else {
        //log("jni->%s/callJni setAndroidManifestPackageName(): exist !!!!", CLASS_JAVA_SDK_NAME);
        jint ji = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID);
    }
}

bool UpltvAndroid::isAndroidReportOnlineEnable() {
    cocos2d::JniMethodInfo minfo;
    string sigs = "()Z";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME ,"isReportOnlineEnable", sigs.c_str());
    if (!isHave) {
        log("jni->%s/callJni isReportOnlineEnable(): not exist!!!!!", CLASS_JAVA_SDK_NAME);
        return false;
    }
    else {
        //log("jni->%s/callJni isReportOnlineEnable(): exist !!!!", CLASS_JAVA_SDK_NAME);
        return minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
    }
}

static void callVoidJavaWithString2(const char* methodname, const char* value1, const char* value2) {
    cocos2d::JniMethodInfo minfo;
    string sigs = "(Ljava/lang/String;Ljava/lang/String;)V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME , methodname, sigs.c_str());
    if (!isHave) {
        log("===> cpp jni->%s/callJni: %s not exist!!!!!", methodname, CLASS_JAVA_SDK_NAME);
        return;
    }
    else {
        //log("jni->%s/callJni: %s exist !!!!", methodname, CLASS_JAVA_SDK_NAME);
        jstring jvalue1 = strToJstring(minfo.env, value1==nullptr?"":value1);
        jstring jvalue2 = strToJstring(minfo.env, value2==nullptr?"":value2);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jvalue1, jvalue2);
    }
}

static void callVoidJavaWithString(const char* methodname, const char* value1) {
    cocos2d::JniMethodInfo minfo;
    string sigs = "(Ljava/lang/String;Ljava/lang/String;)V";
    bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_JAVA_SDK_NAME , methodname, sigs.c_str());
    if (!isHave) {
        log("===> cpp jni->%s/callJni: %s not exist!!!!!", methodname, CLASS_JAVA_SDK_NAME);
        return;
    }
    else {
        //log("jni->%s/callJni: %s exist !!!!", methodname, CLASS_JAVA_SDK_NAME);
        jstring jvalue1 = strToJstring(minfo.env, value1==nullptr?"":value1);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jvalue1);
    }
}

void UpltvAndroid::reportILShowDid(const char* cpPlaceId, const char* msg) {
    
    callVoidJavaWithString2("reportILShowDid", cpPlaceId, msg);
}

void UpltvAndroid::reportILClick(const char* cpPlaceId, const char* msg) {
    callVoidJavaWithString2("reportILClick", cpPlaceId, msg);
}

void UpltvAndroid::reportILClose(const char* cpPlaceId, const char* msg) {
    callVoidJavaWithString2("reportILClose", cpPlaceId, msg);
}

void UpltvAndroid::reportRDShowDid(const char* msg) {
    callVoidJavaWithString("reportRDShowDid", msg);
}

void UpltvAndroid::reportRDRewardGiven(const char* msg) {
    callVoidJavaWithString("reportRDRewardGiven", msg);
}

void UpltvAndroid::reportRDRewardCancel(const char* msg) {
    callVoidJavaWithString("reportRDRewardCancel", msg);
}

void UpltvAndroid::reportRDRewardClick(const char* msg) {
    callVoidJavaWithString("reportRDRewardClick", msg);
}

void UpltvAndroid::reportRDRewardClose(const char* msg) {
    callVoidJavaWithString("reportRDRewardClose", msg);
}

void UpltvAndroid::reportIvokePluginMethodReceive(const char* msg) {
    callVoidJavaWithString("reportIvokePluginMethodReceive", msg);
}
