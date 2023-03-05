#include "head.h"
#include"findArmor.h"
using namespace std;
using namespace cv;
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
double Distance(Point2f A, Point2f B)
{
	return sqrt(pow((A.x - B.x), 2) + pow((A.y - B.y), 2));
}
// �õ����ĵ�,������ɫ,����λ��
Point2f Getcenter( RotatedRect& A, int& color,Mat src_img)
{
	//���ͼ���Ƿ����
	if (src_img.empty())
	{
		cout << "��ȡ�ļ�����" << endl;
		return Point(-1,-1);
	}
	//��ͼ��������Ԥ����
	Mat thresholdimg1;
	//imshow("װ�װ�ʶ��", src_img);
	cvtColor(src_img, thresholdimg1, COLOR_BGR2GRAY);
	threshold(thresholdimg1, thresholdimg1, 160, 255, THRESH_BINARY);
	//���߿�
	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;
	findContours(thresholdimg1, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	// ����ͼ�������Ĵ�С
	int Sumarea = src_img.cols * src_img.rows;
	//װ�װ�ͼƬ�Ĵ���
	Point2f vertex[4];//����һ������
	vector<RotatedRect> minRects;
	if (contours.size() < 2)
	{
		return Point(-1, -1);
	}
	//��������״�ϸ�ľ��η��������ڲ�
	for (int i = 0; i < contours.size(); ++i) {
		RotatedRect minRect = minAreaRect(contours[i]);
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
	}
	//�������ҵľ���
	RotatedRect leftRect, rightRect;
	//�����������ε������
	vector<vector<int>> reliability;
	double distance = -1;
	double height;
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
		// �ֹ���Ҳ�˳�
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