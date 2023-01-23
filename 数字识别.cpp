#include"head.h"
using namespace std;
using namespace cv;

void getPXSum(Mat& src, int& a)//获取所有像素点和
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
	threshold(src, src, 100, 255, THRESH_BINARY);
	resize(src, src, Size(32, 48), 0, 0, INTER_LINEAR);
	for (int i = 0; i < 2 * 10; i++) {
		char name[50];
		sprintf_s(name, "D:/数字训练模板/%d.jpg", i);
		Mat Template = imread(name, IMREAD_GRAYSCALE);//读取模板
		threshold(Template, Template, 100, 255, THRESH_BINARY);
		resize(Template, Template, Size(32, 48), 0, 0, INTER_LINEAR);//调整尺寸
		//imshow(name, Template);

		/*让需要匹配的图分别和10个模板对应像素点值相减，然后求返回图片的整个图片的像素点值得平方和，和哪个模板匹配时候返回图片的平方和最小则就可以得到结果*/
		absdiff(Template, src, img_result);//AbsDiff，OpenCV中计算两个数组差的绝对值的函数。
		int diff = 0;
		getPXSum(img_result, diff);//获取所有像素点和
		if (diff < min)//像素点对比
		{
			min = diff;
			serieNum = i;
		}
	}
	int Sum = 0;
	getPXSum(src, Sum);
	//printf("最小距离是%d ", min);
	//printf("匹配到第%d个模板匹配的数字是%d\n", serieNum, serieNum%10);

	if (double(min) / double(Sum) > 1)
	{
		return -1;
	}
	return serieNum % 10;
}