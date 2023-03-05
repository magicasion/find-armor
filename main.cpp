#include"head.h"
#include"findArmor.h"
#include"daheng-img.h"
//#include"daheng-img.h"
using namespace std;
using namespace cv;
Mat src_img;
GX_DEV_HANDLE hDevice = NULL;
GX_FRAME_DATA frameData = { 0 };
GX_STATUS status;
int main(void)
{
	//导入示例视频
	namedWindow("装甲板识别", 0);
	DahengInit(hDevice, frameData, status);
	cout << "hDevice" << hDevice << endl;
	bool flag = 0;
	RotatedRect A;
	int color;
	while (waitKey(1) != 27)
	{
		GetImage(hDevice, frameData, status, src_img);
		//定义返回点和返回矩阵
		Rect rect1;
		Point2f currentCenter = Point2f(-1, -1);
		//设置定时器1
		double timer1 = (double)getTickCount();
		if (src_img.empty())
		{
			cout << "读取文件错误" << endl;
			return 0;
		}
		//获取图像的返回点以及返回矩阵
	    currentCenter = Getcenter(A, color, src_img);
		//A.size = Size(A.size.width * 3, A.size.height * 1.5);
		//算出旋转矩阵的最大矩阵
		rect1 = A.boundingRect();
		//对返回的矩阵进行错误判断
		if (rect1.x < 0) { rect1.width = rect1.width + rect1.x; rect1.x = 0; }
		if (rect1.y < 0) { rect1.height = rect1.height + rect1.y; rect1.y = 0; }
		if (rect1.y + rect1.height >= src_img.rows) { rect1.height = src_img.rows - rect1.y - 1; };
		if (rect1.x + rect1.width >= src_img.cols) { rect1.width = src_img.cols - rect1.x - 1; };
		//imshow("roi提取", src_img(rect1));
		//设置定时器2

		/*一帧图像分析完毕*/
		//选出正确的图像,返回数据分析
		if (currentCenter != Point2f(-1, -1))
		{
			A.center = Point2f(currentCenter.x,currentCenter.y);
			//cout << "A的映射中心坐标为:" << A.center << endl;
			//画出最小外接矩形
			Point2f P[4];
			A.points(P);
			for (int i = 0; i < 4; i++)
			{
				line(src_img, P[i], P[(i + 1) % 4], Scalar(255, 0, 0), 2, LINE_AA);
			}
			circle(src_img,currentCenter, 2, Scalar(0, 0, 255), 2, LINE_AA, 0);
			//略微等待
			waitKey(1);
		}
		//计算出帧率
		double timer2 = (double)getTickCount();
		float fps = getTickFrequency() / (timer2 - timer1);
		putText(src_img, "FPS1:" + to_string(int(fps)), Point(100, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);
		imshow("装甲板识别", src_img);
	    //进行数字识别
		char c = waitKey(1);
		if (c == 27)
		{
			break;
		}
	}
	StopGetImage(hDevice, frameData, status);
	//capture.release();
	return (0);
}