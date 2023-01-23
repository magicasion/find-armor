#include "head.h"
#include"findArmor.h"
using namespace std;
using namespace cv;
int getColSum(Mat src, int col)//统计所有列像素的总和
{
	int sum = 0;
	int height = src.rows;
	int width = src.cols;
	for (int i = 0; i < height; i++)
	{
		sum = sum + src.at <uchar>(i, col);
	}
	return sum;
}
int getRowSum(Mat src, int row)//统计所有行像素的总和
{
	int sum = 0;
	int height = src.rows;
	int width = src.cols;
	for (int i = 0; i < width; i++)
	{
		sum += src.at <uchar>(row, i);
	}
	return sum;
}
void getP4(Mat src, double angle, Point2f P[4])
{
	int x = src.cols;
	int y = src.rows;
	if (-90 <= angle && angle < -45)
	{
		P[0] = Point2f(x, y);
		P[1] = Point2f(0, y);
		P[2] = Point2f(0, 0);
		P[3] = Point2f(x, 0);
	}
	else if (-45 <= angle && angle <= 45)
	{
		P[0] = Point2f(0, y);
		P[1] = Point2f(0, 0);
		P[2] = Point2f(x, 0);
		P[3] = Point2f(x, y);
	}
	else if (45 < angle && angle <= 90)
	{
		P[3] = Point2f(0, 0);
		P[2] = Point2f(x, 0);
		P[1] = Point2f(x, y);
		P[0] = Point2f(0, y);
	}
	//else
	//{
	//	cout << "角度不符合" << endl;
	//}
	return;
}
int cutLeft(Mat& src)//左右切割
{
	int left, right;
	left = 0;
	right = src.cols - 1;
	int bottom = src.rows - 1, top = 0;
	for (int i = 0; i < src.cols; i++)
	{
		int colValue = getColSum(src, i);//统计所有列像素的总和
		//cout <<i<<" th "<< colValue << endl;
		if (colValue > 0)//扫描直到列像素的总和大于0时，记下当前位置left
		{
			left = i;
			break;
		}
	}

	//继续扫描
	for (int i = src.cols - 1; i > 0; i--)
	{
		int colValue = getColSum(src, i);//统计所有列像素的总和
		if (colValue > 0)//继续扫描直到列像素的总和等于0时，记下当前位置right
		{
			right = i;
			break;
		}
	}
	for (int i = 0; i < src.rows; i++)
	{
		int colValue = getRowSum(src, i);//统计所有行像素的总和
		//cout <<i<<" th "<< colValue << endl;
		if (colValue > 0)//扫描直到行像素的总和大于0时，记下当前位置top
		{
			top = i;
			break;
		}
	}
	for (int i = src.rows - 1; i > 0; i--)
	{
		int colValue = getRowSum(src, i);//统计所有行像素的总和
		//cout << i << " th " << colValue << endl;
		if (colValue > 0)//继续扫描直到行像素的总和等于0时，记下当前位置bottom
		{
			bottom = i;
			break;
		}
	}
	Rect rect = Rect(left, top, right - left, bottom - top);
	src = src(rect);
	return 0;
}
void getPXSum(Mat& src, int& a)//获取所有像素点和
{
	if (a == 0)
	{
		threshold(src, src, 100, 255, THRESH_BINARY);
	}
	else if (a == 1)
	{
		//imwrite("C:/Users/86185/Desktop/装甲板识别测试图片/色块识别5.png", src);;
		cvtColor(src, src, COLOR_BGR2HSV);
		inRange(src, Scalar(100, 100, 190), Scalar(124, 255, 255), src);

		//imshow("hsv处理图", src);
	}
	a = 0;
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			a += src.at <uchar>(i, j);
		}
	}
}
int  getSubtract(Mat& src) //数字识别
{
	Mat img_result;
	int min = 1000000;
	int serieNum = 0;
	char T[50];
	//sprintf(T, "比较图%d", m);
	//namedWindow(T, 0);
	//imshow(T, src);
	//char n[50];
	//sprintf(n,"D:/数字训练模板/2%d.jpg", m);
	//imwrite(n, src);
	threshold(src, src, 112, 255, THRESH_BINARY);
	resize(src, src, Size(32, 48), 0, 0, INTER_LINEAR);
	for (int i = 0; i < 2 * 10; i++) {
		char name[50];
		sprintf_s(name, "D:/数字训练模板/%d.jpg", i);
		Mat Template = imread(name, IMREAD_GRAYSCALE);//读取模板
		threshold(Template, Template, 100, 255, THRESH_BINARY);
		resize(Template, Template, Size(32, 48), 0, 0, INTER_LINEAR);//调整尺寸
		absdiff(Template, src, img_result);//AbsDiff，OpenCV中计算两个数组差的绝对值的函数。
		int diff = 0;
		getPXSum(img_result, diff);//获取所有像素点和
		if (diff < min)//像素点对比
		{
			min = diff;
			serieNum = i;
		}
	}
	//cout << min << endl;
	int Sum = 0;
	if (serieNum > 10)
	{
		if (min > 100000)
		{
			return -1;
		}
	}
	else
	{
		if (min > 156000)
		{
			cout << min << endl;
			return -1;
		}
	}
	return serieNum % 10;
}
double Distance(Point2f A, Point2f B)
{
	return sqrt(pow((A.x - B.x), 2) + pow((A.y - B.y), 2));
}
// 得到中心点,返回颜色,矩形位置
Point2f Getcenter( RotatedRect& A, int& color,Mat src_img)
{
	if (src_img.empty())
	{
		cout << "读取文件错误" << endl;
		return Point(-1,-1);
	}
	//对图像进行相关预处理
	//resize(src_img, src_img, Size(src_img.cols / 2, src_img.rows / 2), 0, 0, INTER_AREA);
	Mat thresholdimg1;
	//imshow("装甲板识别", src_img);
	cvtColor(src_img, thresholdimg1, COLOR_BGR2GRAY);
	threshold(thresholdimg1, thresholdimg1, 160, 255, THRESH_BINARY);
	//imshow("二值化后的图片", thresholdimg1);
	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;
	findContours(thresholdimg1, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	// 计算图像的总体的大小
	int Sumarea = src_img.cols * src_img.rows;
	//装甲板图片的处理
	Point  center;

	Point2f vertex[4];
	vector<RotatedRect> minRects;
	//============================记住切换视频的时候这里需要修改========
	if (contours.size() < 2)
	{
		//cout << "轮廓数量不足" << endl;
		return Point(-1, -1);
		//return 0;
	}
	//将所用形状合格的矩形放入容器内部
	for (int i = 0; i < contours.size(); ++i) {
		RotatedRect minRect = minAreaRect(Mat(contours[i]));
		minRect.points(vertex);
		RotatedRect rect = minAreaRect(contours[i]);
		double area = contourArea(contours[i]);
		double  rectarea = rect.size.area();
		//第一次筛选,筛选出合格的长方体
		if (rect.size.area() < Sumarea / 20000 || area / rectarea < 0.65)
		{
			continue;
		}
		//调整角度
		if (minRect.size.width > minRect.size.height) {
			int angle = minRect.angle;
			angle = (angle + 90) % 180;
			minRect.angle = (double)angle;
			float t = minRect.size.width;
			minRect.size.width = minRect.size.height;
			minRect.size.height = t;
		}
		if (minRect.angle >= 45 && minRect.angle < 135)
		{
			continue;
		}
		if (minRect.size.height / minRect.size.width < 1.66666)
		{
			continue;
		}
		minRects.push_back(minRect);
		//for (int l = 0; l < 4; l++)
		//{
		//	line(src_img, vertex[l], vertex[(l + 1) % 4], Scalar(255, 0, 0), 4, LINE_AA);
		//}
		minRects.push_back(minRect);
	}
	//定义左右的矩形
	RotatedRect leftRect, rightRect;
	//定义两个矩形的相关性
	vector<vector<int>> reliability;
	double area[2], distance = -1, height;
	if (minRects.size() < 2) {
		return Point2f(-1, -1);
	}
	//cout << minRects.size() << endl;
	// 存在问题,有两个相同大小的矩形在里面
	for (int i = 0; i < minRects.size(); i++) {
		for (int j = i + 1; j < minRects.size(); j++) {
			int level = 0;
			vector<int> temp;
			// 这里开始对各项数据进行相关的打分,然后选出最佳的两个矩形
			leftRect = minRects[i];
			rightRect = minRects[j];
			//Mat src1 = src_img.clone();
			// 先对角度进行相关的匹配
			if (((int)leftRect.angle + 90) % 180 == ((int)rightRect.angle + 90) % 180) {
				level += 10;
			}
			else if (abs(((int)leftRect.angle + 90) % 180 - ((int)rightRect.angle + 90) % 180) < 5) {
				level += 8;
			}
			else if (abs(((int)leftRect.angle + 90) % 180 - ((int)rightRect.angle + 90) % 180) < 10) {
				level += 6;
			}
			else if (abs(((int)leftRect.angle + 90) % 180 - ((int)rightRect.angle + 90) % 180) < 30) {
				level += 4;
			}
			else if (abs(((int)leftRect.angle + 90) % 180 - ((int)rightRect.angle + 90) % 180) < 60) {
				level += 1;
			}
			else
			{
				// 严重不合格就退出
				continue;
			}
			// 对长宽进行相关的比对,打分
			double compare = leftRect.size.width / rightRect.size.width;
			if (compare > 0.8 && compare < 1.25)
			{
				level += 10;
			}
			else if (compare > 0.6 && compare < 1.666)
			{
				level += 8;
			}
			else if (compare > 0.5 && compare < 2)
			{
				level += 6;
			}
			else if (compare > 0.4 && compare < 2.5)
			{
				level += 4;
			}
			else if (compare > 0.3 && compare < 3.3333)
			{
				level += 2;
			}
			else
			{
				level += 1;
			}
			compare = leftRect.size.height / rightRect.size.height;
			if (compare > 0.8 && compare < 1.25)
			{
				level += 10;
			}
			else if (compare > 0.6 && compare < 1.666)
			{
				level += 8;
			}
			else if (compare > 0.5 && compare < 2)
			{
				level += 6;
			}
			else if (compare > 0.4 && compare < 2.5)
			{
				level += 4;
			}
			else if (compare > 0.3 && compare < 3.3333)
			{
				level += 2;
			}
			else
			{
				level += 1;
			}
			// 对相差的高度进行相关的匹配,打分
			double half_height = (leftRect.size.height + rightRect.size.height) / 2;
			if (leftRect.center.y == rightRect.center.y) {
				level += 10;
			}
			else if (abs(leftRect.center.y - rightRect.center.y) < 0.2 * half_height) {
				level += 8;
			}
			else if (abs(leftRect.center.y - rightRect.center.y) < 0.4 * half_height) {
				level += 6;
			}
			else if (abs(leftRect.center.y - rightRect.center.y) < 0.8 * half_height) {
				level += 4;
			}
			else if (abs(leftRect.center.y - rightRect.center.y) < half_height) {
				level += 1;
			}
			else
			{
				continue;
			}
			// 对相关的距离进行匹配,打分,距离过远就直接进行退出
			distance = Distance(leftRect.center, rightRect.center);
			height = (leftRect.size.height + rightRect.size.height) / 2;
			if (distance != 0 && distance > 1.1 * height && distance < 3.5 * height) {

				if (distance < 1.6 * height) {
					level += 6;
				}
				else if (distance < 2.0 * height) {
					level += 4;
				}
				else if (distance < 2.5 * height) {
					level += 2;
				}
				else
				{
					level += 1;
				}
			}
			else
			{
				//	cout << "这里退出了" << i << "," << j << endl;
				//	cout << "比例为" << distance / height << endl;
				continue;
			}
			// 统计分数
			temp.push_back(i);
			temp.push_back(j);
			temp.push_back(level);
			reliability.push_back(temp);
		}
	}
	// 没有一个初步合格,就退出
	if (reliability.empty()) {
		return Point2f(-1, -1);
	}
	else {
		//获取最高分的一组矩形
		int maxLevel = 0, index = 0;
		for (int k = 0; k < reliability.size(); k++) {
			if (reliability[k][2] > maxLevel) {
				maxLevel = reliability[k][2];
				index = k;
			}
		}
		if (maxLevel < 21)
		{
			return Point2f(-1, -1);
		}
		Point2f currentCenter(-1, -1);
		currentCenter.x = (minRects[reliability[index][0]].center.x + minRects[reliability[index][1]].center.x) / 2;
		currentCenter.y = (minRects[reliability[index][0]].center.y + minRects[reliability[index][1]].center.y) / 2;
		RotatedRect B[2];
		//得出灯条的相关的矩形
		B[0] = minRects[reliability[index][0]];
		B[1] = minRects[reliability[index][1]];
		Mat src1;
		Point2f P[4];
		B[0].points(P);
		//通过仿射矫正识别出灯条的颜色
		Mat src = Mat::zeros(Size(32, 48), CV_8UC3);
		//得出仿射矫正的点
		Point2f P4[4];
		getP4(src, B[0].angle, P4);
		Mat src_mat = getAffineTransform(P, P4);
		Mat dst1;
		warpAffine(src_img, dst1, src_mat, Size(32, 48));
		//imshow("灯条图", dst1);
		//imwrite("C:/Users/86185/Desktop/装甲板识别测试图片/色块识别3.png", dst1);;
		int num = 1;
		// 通过像素值的点来判断颜色的值
		getPXSum(dst1, num);
		if (num > 0)
		{
			color = 1;
		}
		else
		{
			color = 0;
		}

		//通过计算计算出中心数字的roi区域
		double minwidth;
		distance = Distance(B[0].center, B[1].center);
		float angle = atan((B[0].center.y - B[1].center.y) / (B[0].center.x - B[1].center.x)) * 180 / (float)3.14159;

		if (B[0].size.width > B[1].size.width)
		{
			minwidth = distance - (2 * (double)B[0].size.width);
		}
		else
		{
			minwidth = distance - (2 * (double)B[1].size.width);
		}
		// 获取数字位置的相关的矩形
		if (maxLevel < 21)
		{
			return Point2f(-1, -1);
		}
		else
		{
			//返回中心点
			//waitKey(0);
			A = RotatedRect(currentCenter, Size(minwidth, distance), angle);
			return currentCenter;
		}
	}
}
void sendBtyle(int color, int num)
{
	CSerialPort mySerialPort;//首先将之前定义的类实例化
	if (!mySerialPort.InitPort(1, CBR_19200, 'N', 8, 1, EV_RXCHAR))//是否打开串口，3就是你外设连接电脑的com口，可以在设备管理器查看，然后更改这个参数
	{
		std::cout << "initPort fail !" << std::endl;
	}
	else
	{
		std::cout << "initPort success !" << std::endl;
	}
	if (!mySerialPort.OpenListenThread())//是否打开监听线程，开启线程用来传输返回值
	{
		std::cout << "OpenListenThread fail !" << std::endl;
	}
	else
	{
		std::cout << "OpenListenThread success !" << std::endl;
	}
	if (color == 0)
	{
		unsigned char temp[] = "红色";//动态创建一个数组
		mySerialPort.WriteData(temp, 4);
		unsigned char* temp2 = new unsigned char[4];
		temp2[0] = ' ';
		char str[25];
		itoa(num, str, 10);
		temp2[1] = str[0];
		mySerialPort.WriteData(temp2, 2);
	}
	else
	{
		unsigned char temp[] = "蓝色";//动态创建一个数组
		mySerialPort.WriteData(temp, 4);
		unsigned char* temp2 = new unsigned char[4];
		temp2[0] = ' ';
		char str[25];
		itoa(num, str, 10);
		temp2[1] = str[0];
		mySerialPort.WriteData(temp2, 2);
	}
	Sleep(1000);
	waitKey(0);

}