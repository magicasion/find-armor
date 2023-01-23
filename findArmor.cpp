#include "head.h"
#include"findArmor.h"
using namespace std;
using namespace cv;
int getColSum(Mat src, int col)//ͳ�����������ص��ܺ�
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
int getRowSum(Mat src, int row)//ͳ�����������ص��ܺ�
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
	//	cout << "�ǶȲ�����" << endl;
	//}
	return;
}
int cutLeft(Mat& src)//�����и�
{
	int left, right;
	left = 0;
	right = src.cols - 1;
	int bottom = src.rows - 1, top = 0;
	for (int i = 0; i < src.cols; i++)
	{
		int colValue = getColSum(src, i);//ͳ�����������ص��ܺ�
		//cout <<i<<" th "<< colValue << endl;
		if (colValue > 0)//ɨ��ֱ�������ص��ܺʹ���0ʱ�����µ�ǰλ��left
		{
			left = i;
			break;
		}
	}

	//����ɨ��
	for (int i = src.cols - 1; i > 0; i--)
	{
		int colValue = getColSum(src, i);//ͳ�����������ص��ܺ�
		if (colValue > 0)//����ɨ��ֱ�������ص��ܺ͵���0ʱ�����µ�ǰλ��right
		{
			right = i;
			break;
		}
	}
	for (int i = 0; i < src.rows; i++)
	{
		int colValue = getRowSum(src, i);//ͳ�����������ص��ܺ�
		//cout <<i<<" th "<< colValue << endl;
		if (colValue > 0)//ɨ��ֱ�������ص��ܺʹ���0ʱ�����µ�ǰλ��top
		{
			top = i;
			break;
		}
	}
	for (int i = src.rows - 1; i > 0; i--)
	{
		int colValue = getRowSum(src, i);//ͳ�����������ص��ܺ�
		//cout << i << " th " << colValue << endl;
		if (colValue > 0)//����ɨ��ֱ�������ص��ܺ͵���0ʱ�����µ�ǰλ��bottom
		{
			bottom = i;
			break;
		}
	}
	Rect rect = Rect(left, top, right - left, bottom - top);
	src = src(rect);
	return 0;
}
void getPXSum(Mat& src, int& a)//��ȡ�������ص��
{
	if (a == 0)
	{
		threshold(src, src, 100, 255, THRESH_BINARY);
	}
	else if (a == 1)
	{
		//imwrite("C:/Users/86185/Desktop/װ�װ�ʶ�����ͼƬ/ɫ��ʶ��5.png", src);;
		cvtColor(src, src, COLOR_BGR2HSV);
		inRange(src, Scalar(100, 100, 190), Scalar(124, 255, 255), src);

		//imshow("hsv����ͼ", src);
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
int  getSubtract(Mat& src) //����ʶ��
{
	Mat img_result;
	int min = 1000000;
	int serieNum = 0;
	char T[50];
	//sprintf(T, "�Ƚ�ͼ%d", m);
	//namedWindow(T, 0);
	//imshow(T, src);
	//char n[50];
	//sprintf(n,"D:/����ѵ��ģ��/2%d.jpg", m);
	//imwrite(n, src);
	threshold(src, src, 112, 255, THRESH_BINARY);
	resize(src, src, Size(32, 48), 0, 0, INTER_LINEAR);
	for (int i = 0; i < 2 * 10; i++) {
		char name[50];
		sprintf_s(name, "D:/����ѵ��ģ��/%d.jpg", i);
		Mat Template = imread(name, IMREAD_GRAYSCALE);//��ȡģ��
		threshold(Template, Template, 100, 255, THRESH_BINARY);
		resize(Template, Template, Size(32, 48), 0, 0, INTER_LINEAR);//�����ߴ�
		absdiff(Template, src, img_result);//AbsDiff��OpenCV�м������������ľ���ֵ�ĺ�����
		int diff = 0;
		getPXSum(img_result, diff);//��ȡ�������ص��
		if (diff < min)//���ص�Ա�
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
// �õ����ĵ�,������ɫ,����λ��
Point2f Getcenter( RotatedRect& A, int& color,Mat src_img)
{
	if (src_img.empty())
	{
		cout << "��ȡ�ļ�����" << endl;
		return Point(-1,-1);
	}
	//��ͼ��������Ԥ����
	//resize(src_img, src_img, Size(src_img.cols / 2, src_img.rows / 2), 0, 0, INTER_AREA);
	Mat thresholdimg1;
	//imshow("װ�װ�ʶ��", src_img);
	cvtColor(src_img, thresholdimg1, COLOR_BGR2GRAY);
	threshold(thresholdimg1, thresholdimg1, 160, 255, THRESH_BINARY);
	//imshow("��ֵ�����ͼƬ", thresholdimg1);
	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;
	findContours(thresholdimg1, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	// ����ͼ�������Ĵ�С
	int Sumarea = src_img.cols * src_img.rows;
	//װ�װ�ͼƬ�Ĵ���
	Point  center;

	Point2f vertex[4];
	vector<RotatedRect> minRects;
	//============================��ס�л���Ƶ��ʱ��������Ҫ�޸�========
	if (contours.size() < 2)
	{
		//cout << "������������" << endl;
		return Point(-1, -1);
		//return 0;
	}
	//��������״�ϸ�ľ��η��������ڲ�
	for (int i = 0; i < contours.size(); ++i) {
		RotatedRect minRect = minAreaRect(Mat(contours[i]));
		minRect.points(vertex);
		RotatedRect rect = minAreaRect(contours[i]);
		double area = contourArea(contours[i]);
		double  rectarea = rect.size.area();
		//��һ��ɸѡ,ɸѡ���ϸ�ĳ�����
		if (rect.size.area() < Sumarea / 20000 || area / rectarea < 0.65)
		{
			continue;
		}
		//�����Ƕ�
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
	//�������ҵľ���
	RotatedRect leftRect, rightRect;
	//�����������ε������
	vector<vector<int>> reliability;
	double area[2], distance = -1, height;
	if (minRects.size() < 2) {
		return Point2f(-1, -1);
	}
	//cout << minRects.size() << endl;
	// ��������,��������ͬ��С�ľ���������
	for (int i = 0; i < minRects.size(); i++) {
		for (int j = i + 1; j < minRects.size(); j++) {
			int level = 0;
			vector<int> temp;
			// ���￪ʼ�Ը������ݽ�����صĴ��,Ȼ��ѡ����ѵ���������
			leftRect = minRects[i];
			rightRect = minRects[j];
			//Mat src1 = src_img.clone();
			// �ȶԽǶȽ�����ص�ƥ��
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
				// ���ز��ϸ���˳�
				continue;
			}
			// �Գ��������صıȶ�,���
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
			// �����ĸ߶Ƚ�����ص�ƥ��,���
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
			// ����صľ������ƥ��,���,�����Զ��ֱ�ӽ����˳�
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
				//	cout << "�����˳���" << i << "," << j << endl;
				//	cout << "����Ϊ" << distance / height << endl;
				continue;
			}
			// ͳ�Ʒ���
			temp.push_back(i);
			temp.push_back(j);
			temp.push_back(level);
			reliability.push_back(temp);
		}
	}
	// û��һ�������ϸ�,���˳�
	if (reliability.empty()) {
		return Point2f(-1, -1);
	}
	else {
		//��ȡ��߷ֵ�һ�����
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
		//�ó���������صľ���
		B[0] = minRects[reliability[index][0]];
		B[1] = minRects[reliability[index][1]];
		Mat src1;
		Point2f P[4];
		B[0].points(P);
		//ͨ���������ʶ�����������ɫ
		Mat src = Mat::zeros(Size(32, 48), CV_8UC3);
		//�ó���������ĵ�
		Point2f P4[4];
		getP4(src, B[0].angle, P4);
		Mat src_mat = getAffineTransform(P, P4);
		Mat dst1;
		warpAffine(src_img, dst1, src_mat, Size(32, 48));
		//imshow("����ͼ", dst1);
		//imwrite("C:/Users/86185/Desktop/װ�װ�ʶ�����ͼƬ/ɫ��ʶ��3.png", dst1);;
		int num = 1;
		// ͨ������ֵ�ĵ����ж���ɫ��ֵ
		getPXSum(dst1, num);
		if (num > 0)
		{
			color = 1;
		}
		else
		{
			color = 0;
		}

		//ͨ�����������������ֵ�roi����
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
		// ��ȡ����λ�õ���صľ���
		if (maxLevel < 21)
		{
			return Point2f(-1, -1);
		}
		else
		{
			//�������ĵ�
			//waitKey(0);
			A = RotatedRect(currentCenter, Size(minwidth, distance), angle);
			return currentCenter;
		}
	}
}
void sendBtyle(int color, int num)
{
	CSerialPort mySerialPort;//���Ƚ�֮ǰ�������ʵ����
	if (!mySerialPort.InitPort(1, CBR_19200, 'N', 8, 1, EV_RXCHAR))//�Ƿ�򿪴��ڣ�3�������������ӵ��Ե�com�ڣ��������豸�������鿴��Ȼ������������
	{
		std::cout << "initPort fail !" << std::endl;
	}
	else
	{
		std::cout << "initPort success !" << std::endl;
	}
	if (!mySerialPort.OpenListenThread())//�Ƿ�򿪼����̣߳������߳��������䷵��ֵ
	{
		std::cout << "OpenListenThread fail !" << std::endl;
	}
	else
	{
		std::cout << "OpenListenThread success !" << std::endl;
	}
	if (color == 0)
	{
		unsigned char temp[] = "��ɫ";//��̬����һ������
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
		unsigned char temp[] = "��ɫ";//��̬����һ������
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