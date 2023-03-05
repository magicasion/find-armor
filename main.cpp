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
	//����ʾ����Ƶ
	namedWindow("װ�װ�ʶ��", 0);
	DahengInit(hDevice, frameData, status);
	cout << "hDevice" << hDevice << endl;
	bool flag = 0;
	RotatedRect A;
	int color;
	while (waitKey(1) != 27)
	{
		GetImage(hDevice, frameData, status, src_img);
		//���巵�ص�ͷ��ؾ���
		Rect rect1;
		Point2f currentCenter = Point2f(-1, -1);
		//���ö�ʱ��1
		double timer1 = (double)getTickCount();
		if (src_img.empty())
		{
			cout << "��ȡ�ļ�����" << endl;
			return 0;
		}
		//��ȡͼ��ķ��ص��Լ����ؾ���
	    currentCenter = Getcenter(A, color, src_img);
		//A.size = Size(A.size.width * 3, A.size.height * 1.5);
		//�����ת�����������
		rect1 = A.boundingRect();
		//�Է��صľ�����д����ж�
		if (rect1.x < 0) { rect1.width = rect1.width + rect1.x; rect1.x = 0; }
		if (rect1.y < 0) { rect1.height = rect1.height + rect1.y; rect1.y = 0; }
		if (rect1.y + rect1.height >= src_img.rows) { rect1.height = src_img.rows - rect1.y - 1; };
		if (rect1.x + rect1.width >= src_img.cols) { rect1.width = src_img.cols - rect1.x - 1; };
		//imshow("roi��ȡ", src_img(rect1));
		//���ö�ʱ��2

		/*һ֡ͼ��������*/
		//ѡ����ȷ��ͼ��,�������ݷ���
		if (currentCenter != Point2f(-1, -1))
		{
			A.center = Point2f(currentCenter.x,currentCenter.y);
			//cout << "A��ӳ����������Ϊ:" << A.center << endl;
			//������С��Ӿ���
			Point2f P[4];
			A.points(P);
			for (int i = 0; i < 4; i++)
			{
				line(src_img, P[i], P[(i + 1) % 4], Scalar(255, 0, 0), 2, LINE_AA);
			}
			circle(src_img,currentCenter, 2, Scalar(0, 0, 255), 2, LINE_AA, 0);
			//��΢�ȴ�
			waitKey(1);
		}
		//�����֡��
		double timer2 = (double)getTickCount();
		float fps = getTickFrequency() / (timer2 - timer1);
		putText(src_img, "FPS1:" + to_string(int(fps)), Point(100, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);
		imshow("װ�װ�ʶ��", src_img);
	    //��������ʶ��
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