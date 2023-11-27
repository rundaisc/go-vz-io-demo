package main

/*
#cgo LDFLAGS: -L./ -lVzLPRSDK
#cgo CFLAGS: -I callback


#include "load_sdk.h"
#include <stdlib.h>
void gpio_recv_callback(long, int , int, void *);
*/
import "C"
import (
	"fmt"
	"log"
	"unsafe"
)

type Camera struct {
	Ip string
	Sn string
}

func main() {
	//load sdk
	cameraSdkStatus := C.sdk_client_setup()
	if cameraSdkStatus != 0 {
		fmt.Println("camera sdk load failed!")
		return
	}
	fmt.Println("camera sdk load success")
	defer C.sdk_client_cleanup()

	camera := Camera{
		Ip: "127.0.0.1",
	}

	registerCameraToSDK(camera)
}

func registerCameraToSDK(ca Camera) {
	ip := C.CString(ca.Ip)
	user_pwd := C.CString("admin")
	handle := C.sdk_client_open(ip, 80, user_pwd, user_pwd)
	defer C.free(unsafe.Pointer(ip))
	defer C.free(unsafe.Pointer(user_pwd))
	if handle == 0 {
		log.Fatalf("%s:camera client to sdk failed\n", ca.Sn)
		return
	}

	var a C.int
	udata := unsafe.Pointer(&a)
	gpio_ret := C.sdk_client_set_gpio_recv_callback(handle, C.GPIO_RESULT_CALLBACK(C.gpio_recv_callback), udata)
	if gpio_ret != 0 {
		log.Fatalf("%s: dk client gpio init failed\n", ca.Sn)
		return
	}
	log.Println("%s:sdk client gpio init success\n", ca.Sn)
}
