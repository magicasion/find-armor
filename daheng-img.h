#pragma once
#include "DxImageProc.h"
#include "GxIAPI.h"
#include "stdio.h"
#define VERIFY_STATUS_RET(status)   if(status != GX_STATUS_SUCCESS) {return status; }
#define VERIFY_STATUS_RET(status);
//��������ľ��
#include <iostream>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;
int DahengInit(GX_DEV_HANDLE& hDevice, GX_FRAME_DATA& frameData, GX_STATUS& status);
void StopGetImage(GX_DEV_HANDLE& hDevice, GX_FRAME_DATA& frameData, GX_STATUS& status);
void GetImage(GX_DEV_HANDLE& hDevice, GX_FRAME_DATA& frameData, GX_STATUS& status, Mat& image);