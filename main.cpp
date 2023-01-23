#include"head.h"
#include"findArmor.h"
using namespace std;
using namespace cv;
Mat src_img;
int main(void)
{
	namedWindow("装甲板识别", 0);
	namedWindow("二值化后的图片", 0);
	VideoCapture capture("C:/装甲板识别/赛场第一视角视频.mp4");
	if (!capture.isOpened())
	{
		cout << "无法打开相机" << endl;
		return -1;
	}
	capture.read(src_img);
	if (src_img.empty())
	{
		cout << "没有获取到图片!" << endl;
		return 0;
	}
	////VideoCapture capture(0);
	bool flag = 0;
	RotatedRect A;
	int color;
	int nums = 0;
	while (capture.read(src_img))
	{
		double timer1 = (double)getTickCount();
		if (src_img.empty())
		{
			cout << "读取文件错误" << endl;
			return 0;
		}
		Rect rect1;
		Point2f currentCenter = Point2f(-1, -1);
		if (flag)
		{
			A.size = Size(A.size.width * 3, A.size.height *1.5);
			rect1 = A.boundingRect();
			if (rect1.x < 0) { rect1.width = rect1.width + rect1.x; rect1.x = 0; }
			if (rect1.y < 0) { rect1.height = rect1.height + rect1.y; rect1.y = 0; }
			if (rect1.y + rect1.height >= src_img.rows) { rect1.height = src_img.rows - rect1.y - 1; };
			if (rect1.x + rect1.width >= src_img.cols) { rect1.width = src_img.cols - rect1.x - 1; };
			//imshow("roi提取", src_img(rect1));
			currentCenter = Getcenter(A, color, src_img(rect1));
			double timer2 = (double)getTickCount();
			if (currentCenter != Point2f(-1, -1))
			{
				//cout << "当前的中心坐标为:" << currentCenter << endl;
				//cout << "A的中心坐标为:" << A.center << endl;
				A.center = Point2f(rect1.x + currentCenter.x, rect1.y + currentCenter.y);
				//cout << "A的映射中心坐标为:" << A.center << endl;
				//画出最小外接矩形
				Point2f P[4];
				A.points(P);
				for (int i = 0; i < 4; i++)
				{
					line(src_img, P[i], P[(i + 1) % 4], Scalar(255, 0, 0), 2, LINE_AA);
				}
				circle(src_img, A.center, 2, Scalar(0, 0, 255), 2, LINE_AA, 0);
				float fps1 = getTickFrequency() / (timer2 - timer1);
				putText(src_img, "FPS1:" + to_string(int(fps1)), Point(100, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);

				imshow("装甲板识别", src_img);
				waitKey(1);
				continue;
			}
			flag = 0;
		}
		//对图像进行相关预处理
		//resize(src_img, src_img, Size(src_img.cols / 2, src_img.rows / 2), 0, 0, INTER_AREA);
		//Mat thresholdimg1;
		////	imshow("装甲板识别", src_img);
		//cvtColor(src_img, thresholdimg1, COLOR_BGR2GRAY);
		//threshold(thresholdimg1, thresholdimg1, 160, 255, THRESH_BINARY);
		////	imshow("二值化后的图片", thresholdimg1);
		//vector<vector<Point>>contours;
		//vector<Vec4i>hierarchy;
		//findContours(thresholdimg1, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
		if (flag == 0)
		{
			currentCenter = Getcenter(A, color, src_img);
		}
		double timer2 = (double)getTickCount();
		//启动定时器
	//找到中心点后进行相关的识别
		if (currentCenter != Point2f(-1, -1))
		{//画出中心点
			Point2f P[4];
			A.points(P);
			//画出最小外接矩形
			//for (int i = 0; i < 4; i++)
			//{
			//	line(src_img, P[i], P[(i + 1) % 4], Scalar(255, 0, 0), 2, LINE_AA);
			//}
			Point2f P4[4];
			Mat src = Mat::zeros(Size(32, 48), CV_8UC3);
			//得出仿射矫正的点
			getP4(src, A.angle, P4);
			Mat src_mat = getAffineTransform(P, P4);
			Mat dst;
			warpAffine(src_img, dst, src_mat, Size(32, 48));
			//imwrite("C:/Users/86185/Desktop/装甲板识别测试图片/roi测试图11.png", dst);
			//imshow("roi", dst);
			cvtColor(dst, dst, COLOR_BGR2GRAY);
			blur(dst, dst, Size(3, 3));
			threshold(dst, dst, 90, 255, THRESH_BINARY);
			//		cutLeft(dst);
			if (!dst.empty())
			{
				//imshow("处理后的图像", dst);
			}
			else
			{
				//return 0;
				continue;
			}
			flag = 1;
			//数字识别需要进行相关的优化
			//if (!dst.empty())
			//{
			//	int Num = getSubtract(dst);
			//	if (Num != -1&&Num<=4)
			//	{
			//		cout << "识别到的数字为:" << Num << endl;
			//		//sendBtyle(color, Num);
			//		//if (color == 1)
			//		//{
			//		//	cout << "颜色为蓝色" << endl;
			//		//	waitKey(0);
			//		//}
			//		//else if(color == 0)
			//		//{
			//		//	cout << "颜色为红色" << endl;
			//		//	waitKey(0);
			//		//}
			//	}

			//}

		}
		float fps1 = getTickFrequency() / (timer2 - timer1);
		float fps2 = getTickFrequency() / ((double)getTickCount() - timer2);
		//putText(src_img, "FPS1:" + to_string(int(fps1)), Point(100, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);
		imshow("装甲板识别", src_img);
		//if (fps1 < 150)
		//{
		//	//cout << "fps1:" << fps1 << endl;
		//	//char filename[100];
		//	//sprintf_s(filename,"C:/Users/13152/Desktop/装甲板识别单帧识别帧率优化/%d.png", nums);
		//	//nums++;
		//	//imwrite(filename, src_img);
		//	char key = waitKey(0);
		//	if (key == 27)
		//	{
		//	char filename[100];
		//	sprintf_s(filename,"C:/Users/13152/Desktop/装甲板识别单帧识别帧率优化/装甲板识别图%d.png", nums);
		//	nums++;
		//	imwrite(filename, src_img);
		//	}
		//}
		char c = waitKey(1);
		if (c == 27)
		{
			waitKey(0);
		}
	}
	capture.release();
	return (0);
}