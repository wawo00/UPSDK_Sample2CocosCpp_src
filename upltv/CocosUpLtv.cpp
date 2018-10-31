
#include "CocosUpLtv.h"
#include <stdlib.h>
#include <jni.h>
#include <android/log.h>
#include <string>


#define  LOG_TAG    "UpltvBridge"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)



extern "C" {

    extern void notifyUpltvCallback2(const char* keyi, int r, const char* msg);

    extern void notifyUpltvCallback(const char* keyi, bool r);

    extern void invokeCocosCppMethod(const char* name, const char* cpid, const char* msg);

    jstring strToJstring(JNIEnv* env, const char* pStr)
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

    char* jstringTostr(JNIEnv* env, jstring jstr)
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

    JNIEXPORT void JNICALL Java_com_upltv_ads_cpp_UpltvJavaHelper_notifyCocosCppCallback(JNIEnv*  env, jobject thiz, jstring keyi, jboolean r) {

        const char* kk = keyi == nullptr ? "" : jstringTostr(env, keyi);
        notifyUpltvCallback(kk, r);
        //LOGD("==> notifyCallback keyi = %s", kk);
    }

    JNIEXPORT void JNICALL Java_com_upltv_ads_cpp_UpltvJavaHelper_notifyUpltvCallback2(JNIEnv*  env, jobject thiz, jstring keyi, int v, jstring msg) {

        const char* ckey = keyi == nullptr ? "" : jstringTostr(env, keyi);
        const char* cmsg = msg == nullptr ? "" : jstringTostr(env, msg);
        notifyUpltvCallback2(ckey, v, cmsg);
        //LOGD("==> notifyUpltvCallback2 keyi = %s, msg = %s", ckey, cmsg);
    }

    JNIEXPORT void JNICALL Java_com_upltv_ads_cpp_UpltvJavaHelper_invokeCocosCppMethod(JNIEnv*  env, jobject thiz, jstring func, jstring cpid, jstring message) {
        const char* name = func == nullptr ? "" : jstringTostr(env, func);
        const char* adid = cpid == nullptr ? "" : jstringTostr(env, cpid);
        const char* msg = message == nullptr ? "" : jstringTostr(env, message);
        invokeCocosCppMethod(name, adid, msg);
    }
}