#pragma once
#pragma warning(disable:4996)
#include<iostream>
#include<stdio.h>
#include <opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include"opencv2/highgui/highgui.hpp"
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<time.h>
#include<vector>
#include<time.h>
#include<math.h>
#include<stdlib.h>
#pragma once
#include <process.h>    
#include "TChar.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <Windows.h>  
using namespace std;
using namespace cv;
Point2f Getcenter(RotatedRect&A, int& color, Mat src_img);
int getColSum(Mat src, int col);
void getP4(Mat src, double angle, Point2f P[4]);
int cutLeft(Mat& src);
void getPXSum(Mat& src, int& a);//获取所有像素点和
int  getSubtract(Mat& src);
double Distance(Point2f A, Point2f B);