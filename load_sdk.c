#include "load_sdk.h"
#include <stdio.h>
#include <dlfcn.h>

void *_handle = NULL;
RESULT_CALLBACK _result_fun = NULL;

GPIO_RESULT_CALLBACK _gpio_result_fun = NULL;

int gVZLPRC_PLATE_INFO_CALLBACK(long handle, void *pUserData,
													TH_PlateResult *pResult, int uNumPlates,
													int eResultType,
													VZ_LPRC_IMAGE_INFO *pImgFull,
													VZ_LPRC_IMAGE_INFO *pImgPlateClip) {

                                                        if (_result_fun) {
                                                            _result_fun(handle, pUserData, pResult->license, pImgFull, pImgPlateClip);
                                                        }

                                                        return 0;
                                                    }

void gVZLPRC_GPIO_RECV_CALLBACK(long handle,int nGPIOId,int nVal, void *pUserData) {

                                                        if (_gpio_result_fun) {
                                                            _gpio_result_fun(handle, nGPIOId,nVal, pUserData);
                                                        }


                                                        return;
                                                    }

int sdk_client_setup()
{
    typedef int (*FPTR)();
    _handle = dlopen("./libVzLPRSDK.so", 1);
    FPTR fptr = (FPTR)dlsym(_handle, "VzLPRClient_Setup");

    int result = (*fptr)();
    return result;
}

void sdk_client_cleanup() {
    if (_handle == NULL) {
        return;
    }

    typedef void (*FPTR)();
    FPTR fptr = (FPTR)dlsym(_handle, "VzLPRClient_Cleanup");
    (*fptr)();
    dlclose(_handle);
    _handle = NULL;
}


long sdk_client_open(const char *pStrIP, short wPort, const char *pStrUserName, const char *pStrPassword) {
    if (_handle == NULL) {
        return 0;
    }

    typedef long (*FPTR)(const char *, short, const char *, const char *);
    FPTR fptr = (FPTR)dlsym(_handle, "VzLPRClient_Open");

    long result = (*fptr)(pStrIP, wPort, pStrUserName, pStrPassword);
    return result;
}

int sdk_client_close(long handle) {
    if (_handle == NULL) {
        return -1;
    }

    typedef int (*FPTR)(long);
    FPTR fptr = (FPTR)dlsym(_handle, "VzLPRClient_Close");

    int result = (*fptr)(handle);
    return result;
}

int sdk_client_set_plate_info_callback(long handle, RESULT_CALLBACK func, void *pUserData, int bEnableImage) {
    if (_handle == NULL) {
        return -1;
    }

    _result_fun = func;

    typedef int (*FPTR)(long, VZLPRC_PLATE_INFO_CALLBACK, void *, int);
    FPTR fptr = (FPTR)dlsym(_handle, "VzLPRClient_SetPlateInfoCallBack");

    int result = (*fptr)(handle, gVZLPRC_PLATE_INFO_CALLBACK, pUserData, bEnableImage);
    return result;
}

int sdk_client_force_trigger(long handle) {
     if (_handle == NULL) {
        return -1;
    }

    typedef int (*FPTR)(long);
    FPTR fptr = (FPTR)dlsym(_handle, "VzLPRClient_ForceTrigger");

    int result = (*fptr)(handle);
    return result;
}




int  sdk_client_set_gpio_recv_callback(long handle, GPIO_RESULT_CALLBACK func, void *pUserData) {
    if (_handle == NULL) {
        return -1;
    }

    _gpio_result_fun = func;

    typedef int (*FPTR)(long, VZLPRC_GPIO_RECV_CALLBACK, void *);
    FPTR fptr = (FPTR)dlsym(_handle, "VzLPRClient_SetGPIORecvCallBack");

     int result = (*fptr)(handle, gVZLPRC_GPIO_RECV_CALLBACK, pUserData);
    return result;
}

int sdk_get_serial_no(long handle,VZ_DEV_SERIAL_NUM *serial_No) {
     if (_handle == NULL) {
        return -1;
    }

    typedef int (*FPTR)(long,VZ_DEV_SERIAL_NUM *);
    FPTR fptr = (FPTR)dlsym(_handle, "VzLPRClient_GetSerialNumber");

    int result = (*fptr)(handle,serial_No);
    return result;
}