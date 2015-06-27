/*
* A interesting program for show the star in your desktop
* Get from http://www.lellansin.com Just have a fun!
*/

#include <Windows.h>
#include<time.h>
#ifdef _MSC_VER  
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )  
#endif
 
HDC hScreenDC;      // ��Ļ��ͼ�豸���
POINT pData[500];   // �洢��������
long pColor[500];   // ��ȡԭ���������ɫֵ
int Vx;             // vx��������ˮƽƯ���ٶ�
int Vy;             // vy�������崹ֱ�����ٶ�
int PVx;            // pvx��������ʵ��ˮƽ�ٶ�
int PVy;            // pvy��������ʵ�ʴ�ֱ�ٶ�
int timeCount;      // ��ʱѭ������
 
const int StarNum = 500;// ��������Ϊ500
const int ID_TIMER = 1; // ��ʱ����ʶ��
 
const long   StarColor = 0xFEFFFE;  // ������ɫ
const long StarColDown = 0xFFFFFF;  // ��ѩ������ɫ
const long StarColDuck = 0xFFDDDD;  // ��ɫ��ѩ��ɫ
 
const int ScrnWidth = GetSystemMetrics ( SM_CXSCREEN ); // ȫ�����
const int ScrnHight = GetSystemMetrics ( SM_CYSCREEN ); // ȫ���߶�
 
long Abs(long num)
{
    if (num >= 0)
        return (num);
    else
        return (-num);
}
 
int Random(int max)
{
    return (rand() % max);
}
 
void InitPoint(int i)
{
    pData[i].x = Random(ScrnWidth);
    pData[i].y = Random(5);
    pColor[i]  = GetPixel(hScreenDC, pData[i].x, pData[i].y);
}
 
long GetContrast(int i)
{
    long ColorCmp;  //��ȡ���Աȵ����ɫֵ
    long tempR;     //��ȡColorCmp�ĺ�ɫֵ
    long tempG;     //ͬ��
    long tempB;     //ͬ��
    int  Slope;     //��ȡ����Ʈ��ķ���
 
    if (PVy != 0) 
        Slope = PVx / PVy; // ��pvx/pvy��-1��1֮����slope=0 ��ȡ���·��ĵ����ص�
    else // ��pvx/pvy>1,ȡ���·��ĵ� pvx/pvy<-1 ��ȡ���·�
        Slope = 2; // ��������Ʈ�䷽�����ȡ��һ�����Աȵ�
 
    if (Slope == 0)
        ColorCmp = GetPixel(hScreenDC, pData[i].x, pData[i].y + 1);
    else if (Slope > 1)
        ColorCmp = GetPixel(hScreenDC, pData[i].x + 1, pData[i].y + 1);
    else
        ColorCmp = GetPixel(hScreenDC, pData[i].x - 1, pData[i].y + 1);
 
    //ȷ����ǰλ��û������һ�������ص����ص��ͷ���0�����ڷ�ֹ���ڲ�ͬ�����ص���������Ҷ�
    if (ColorCmp == StarColor)
        return 0;
 
    //�ֱ��ȡColorCmp��Աȵ����,��,��Ĳ�ֵ
    tempB = Abs((ColorCmp >> 16) & 0xff - (pColor[i] >> 16) & 0xff);
    tempG = Abs((ColorCmp >> 8) & 0xff - (pColor[i] >> 8) & 0xff);
    tempR = Abs((ColorCmp) & 0xff - (pColor[i]) & 0xff);
 
    return ( (tempR + tempG + tempB) / 3 ); //���ضԱȶ�
}
 
void DrawPoint(void)
{
    int i;
    for (i = 0; i < StarNum; i++) //��ֹ�����ص���ɸ���
    {
        if (pColor[i] != StarColor && pColor[i] != -1)
        {
            SetPixel(hScreenDC, pData[i].x, pData[i].y, pColor[i]); //��ԭ��һ��λ�õ���ɫ
        }
 
        //�����µ�λ�ã�i%3���ڽ����Ƿ�Ϊ3����ò�ͬ���ٶȣ��Ա��γɲδθ�
        PVx = Random(2) - 1 + Vx * (i % 3);
        PVy = Vy * (i % 3 + 1);
        pData[i].x = pData[i].x + PVx; //+pvx��ǰһ���γɵľ����γ��ٶȵĲ��
        pData[i].y = pData[i].y + PVy; //ͬ��
 
        //ȡ����λ�õ�ԭʼ�����ɫֵ��������һ������Ʈ��ʱ�ָ��˴�����ɫ
        pColor[i] = GetPixel(hScreenDC, pData[i].x, pData[i].y);
 
        //�����ȡ��ɫֵʧ�ܣ����������Ѿ�Ʈ����Ļ�����³�ʼ����GetPixel�����ȡʧ�ܾͷ���-1.
        if (pColor[i] == -1)
        {
            InitPoint(i);
        }
        else
        {
            //�������û���ص������ԱȶȽ�С�������ܶѻ����ͻ�������
            //Random(16)>5���ڷ�ֹĳЩ���������Եı߽�ػ����е�����
            if (pColor[i] != StarColor)
            {
                //GetContrast(i)<50�ж��Ƿ񳬳��Աȶ�50���ֵ����Ϊ�Ǳ�Ե�ͻ�ѻ�����
                if (Random(16) > 5 || GetContrast(i) < 50)
                {
                    SetPixel(hScreenDC, pData[i].x, pData[i].y, StarColor);
                } else
                {
                //���߱����ҵ����Եı߽磬�����ѻ���ѧ������ʼ���Ա㻭�µĵ�����
                    SetPixel(hScreenDC, pData[i].x, pData[i].y - 1, StarColDuck);
                    SetPixel(hScreenDC, pData[i].x - 1, pData[i].y, StarColDuck);
                    SetPixel(hScreenDC, pData[i].x + 1, pData[i].y, StarColDown);
                    InitPoint(i);
                }
            }
        }
    }
}
 
void main()
{
    MSG msg;
    HWND hwnd = GetActiveWindow(); // ��ȡ��ǰ���ڵľ��
 
    int i;
    srand(time(0));
    Vx = Random(4) - 2;
    Vy = Random(2) + 2;
 
    // �����ȡ��Ļ��һ��
    for (i = 0; i < StarNum; i++)
    {
        // ��������
        pData[i].x = Random(ScrnWidth);
        pData[i].y = Random(ScrnHight);
        // ������ɫ
        //pColor[i] = GetPixel(hScreenDC, pData[i].x, pData[i].y);
		pColor[i] = 100;
    }
 
    // ���ü�ʱ��
    SetTimer(hwnd, ID_TIMER, 5, NULL);
 
    // ���������Ļ��ͼ�豸
    hScreenDC = GetDC(0);
 
    // ��ʱ
    timeCount = 0;
 
    // ѭ����ȡ����ϵͳ��������Ϣ
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        switch(msg.message)
        {
        case WM_TIMER: // ʱ���ź�
            {
                if (timeCount > 200)
                {
                    timeCount = 0;
                    Vx = Random(4) - 2;
                    Vy = Random(2) + 2;
                }
                else
                {
                    timeCount += 1;
                }
                DrawPoint();
            }
            break;
        }
    }
}
