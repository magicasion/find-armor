#include "daheng-img.h"
int DahengInit(GX_DEV_HANDLE& hDevice, GX_FRAME_DATA& frameData, GX_STATUS& status)
{
    //�豸��ʼ��
    GXInitLib();
    // ���豸
    status = GXOpenDeviceByIndex(1, &hDevice);
    if (status != GX_STATUS_SUCCESS)
    {
        printf("Open device failed!\n");
        return 0;
    }
    //�ɼ���������
    //1.���òɼ�ģʽ
    status = GXSetEnum(hDevice, GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);
    if (status != GX_STATUS_SUCCESS)
    {
        printf("Set acquisition mode failed!\n");
        return 0;
    }
    //2.�ɼ��ٶȼ���
    status = GXSetInt(hDevice, GX_INT_ACQUISITION_SPEED_LEVEL, 4);//�ɼ��ٶ�Ϊ4
    VERIFY_STATUS_RET(status);
    //3.����ģʽ
    status = GXSetEnum(hDevice, GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_OFF);
    VERIFY_STATUS_RET(status);
    //4.�Զ�����
    status = GXSetEnum(hDevice, GX_ENUM_GAIN_SELECTOR, GX_GAIN_SELECTOR_ALL);
    VERIFY_STATUS_RET(status);
    status = GXSetEnum(hDevice, GX_ENUM_GAIN_AUTO, GX_GAIN_AUTO_CONTINUOUS);
    VERIFY_STATUS_RET(emStatus);
    //5.�Զ���ƽ��
    status = GXSetEnum(hDevice, GX_ENUM_BALANCE_WHITE_AUTO, GX_BALANCE_WHITE_AUTO_CONTINUOUS);
    //6.�ֶ������ع�ʱ��
    status = GXSetEnum(hDevice, GX_ENUM_EXPOSURE_MODE, GX_EXPOSURE_MODE_TIMED);
    status = GXSetFloat(hDevice, GX_FLOAT_EXPOSURE_TIME, 10000.0000);
    //7.����ͼ���ʽ
    status = GXSetEnum(hDevice, GX_ENUM_PIXEL_FORMAT, GX_PIXEL_FORMAT_BAYER_RG8);
    //�����ɼ�
    status = GXSendCommand(hDevice, GX_COMMAND_ACQUISITION_START);
    if (status != GX_STATUS_SUCCESS)
    {
        printf("Stop acquisition failed!\n");
    }
    // ��ȡͼ��
    int64_t bufferSize;
    status = GXGetInt(hDevice, GX_INT_PAYLOAD_SIZE, &bufferSize);
    if (status != GX_STATUS_SUCCESS)
    {
        cout << "daheng GetInt failed!" << endl;
    }
    //Ϊͼ����仺��
    frameData.pImgBuf = malloc(bufferSize);
    cout << "��������ʼ�����!" << endl;
    return 1;
}
void GetImage(GX_DEV_HANDLE& hDevice, GX_FRAME_DATA& frameData, GX_STATUS& status, Mat& image)
{
    status = GXGetImage(hDevice, &frameData, 10);
    if (status != GX_STATUS_SUCCESS)
    {
        cout << "daheng get image failed!" << endl;
        return;
    }
    image = Mat(frameData.nHeight, frameData.nWidth, CV_8UC3);
    // ת��ͼ���ʽ
    DxRaw8toRGB24(frameData.pImgBuf, image.data, frameData.nWidth, frameData.nHeight, RAW2RGB_NEIGHBOUR, DX_PIXEL_COLOR_FILTER(BAYERRG), false);
}
void StopGetImage(GX_DEV_HANDLE& hDevice, GX_FRAME_DATA& frameData, GX_STATUS& status)
{
    //ֹͣͼ��ɼ�
    status = GXSendCommand(hDevice, GX_COMMAND_ACQUISITION_STOP);
    if (status != GX_STATUS_SUCCESS)
    {
        printf("Stop acquisition failed!\n");
    }
    //�ͷŻ���
    free(frameData.pImgBuf);
    frameData.pImgBuf = NULL;
    // �ر��豸
    status = GXCloseDevice(hDevice);
    cout << "daheng closed" << endl;
}