#include"head.h"
using namespace std;
using namespace cv;

void getPXSum(Mat& src, int& a)//��ȡ�������ص��
{
	threshold(src, src, 100, 255, THRESH_BINARY);
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
	threshold(src, src, 100, 255, THRESH_BINARY);
	resize(src, src, Size(32, 48), 0, 0, INTER_LINEAR);
	for (int i = 0; i < 2 * 10; i++) {
		char name[50];
		sprintf_s(name, "D:/����ѵ��ģ��/%d.jpg", i);
		Mat Template = imread(name, IMREAD_GRAYSCALE);//��ȡģ��
		threshold(Template, Template, 100, 255, THRESH_BINARY);
		resize(Template, Template, Size(32, 48), 0, 0, INTER_LINEAR);//�����ߴ�
		//imshow(name, Template);

		/*����Ҫƥ���ͼ�ֱ��10��ģ���Ӧ���ص�ֵ�����Ȼ���󷵻�ͼƬ������ͼƬ�����ص�ֵ��ƽ���ͣ����ĸ�ģ��ƥ��ʱ�򷵻�ͼƬ��ƽ������С��Ϳ��Եõ����*/
		absdiff(Template, src, img_result);//AbsDiff��OpenCV�м������������ľ���ֵ�ĺ�����
		int diff = 0;
		getPXSum(img_result, diff);//��ȡ�������ص��
		if (diff < min)//���ص�Ա�
		{
			min = diff;
			serieNum = i;
		}
	}
	int Sum = 0;
	getPXSum(src, Sum);
	//printf("��С������%d ", min);
	//printf("ƥ�䵽��%d��ģ��ƥ���������%d\n", serieNum, serieNum%10);

	if (double(min) / double(Sum) > 1)
	{
		return -1;
	}
	return serieNum % 10;
}