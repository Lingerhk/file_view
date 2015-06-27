/*
* A interesting program for show the star in your desktop
* Get from http://www.lellansin.com Just have a fun!
*/

#include <Windows.h>
#include<time.h>
#ifdef _MSC_VER  
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )  
#endif
 
HDC hScreenDC;      // 屏幕画图设备句柄
POINT pData[500];   // 存储星星坐标
long pColor[500];   // 存取原来坐标的颜色值
int Vx;             // vx星星整体水平漂移速度
int Vy;             // vy星星总体垂直下落速度
int PVx;            // pvx单个星星实际水平速度
int PVy;            // pvy单个星星实际垂直速度
int timeCount;      // 计时循环次数
 
const int StarNum = 500;// 星星数量为500
const int ID_TIMER = 1; // 定时器标识符
 
const long   StarColor = 0xFEFFFE;  // 星星颜色
const long StarColDown = 0xFFFFFF;  // 积雪星星颜色
const long StarColDuck = 0xFFDDDD;  // 深色积雪颜色
 
const int ScrnWidth = GetSystemMetrics ( SM_CXSCREEN ); // 全屏宽度
const int ScrnHight = GetSystemMetrics ( SM_CYSCREEN ); // 全屏高度
 
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
    long ColorCmp;  //存取作对比点的颜色值
    long tempR;     //存取ColorCmp的红色值
    long tempG;     //同理
    long tempB;     //同理
    int  Slope;     //存取星星飘落的方向
 
    if (PVy != 0) 
        Slope = PVx / PVy; // 若pvx/pvy在-1和1之间则slope=0 就取正下方的的像素点
    else // 若pvx/pvy>1,取右下方的点 pvx/pvy<-1 则取左下方
        Slope = 2; // 根据星星飘落方向决定取哪一点做对比点
 
    if (Slope == 0)
        ColorCmp = GetPixel(hScreenDC, pData[i].x, pData[i].y + 1);
    else if (Slope > 1)
        ColorCmp = GetPixel(hScreenDC, pData[i].x + 1, pData[i].y + 1);
    else
        ColorCmp = GetPixel(hScreenDC, pData[i].x - 1, pData[i].y + 1);
 
    //确定当前位置没有与另一个星星重叠，重叠就返回0；用于防止由于不同星星重叠造成星星乱堆
    if (ColorCmp == StarColor)
        return 0;
 
    //分别获取ColorCmp与对比点的蓝,绿,红的差值
    tempB = Abs((ColorCmp >> 16) & 0xff - (pColor[i] >> 16) & 0xff);
    tempG = Abs((ColorCmp >> 8) & 0xff - (pColor[i] >> 8) & 0xff);
    tempR = Abs((ColorCmp) & 0xff - (pColor[i]) & 0xff);
 
    return ( (tempR + tempG + tempB) / 3 ); //返回对比度
}
 
void DrawPoint(void)
{
    int i;
    for (i = 0; i < StarNum; i++) //防止星星重叠造成干扰
    {
        if (pColor[i] != StarColor && pColor[i] != -1)
        {
            SetPixel(hScreenDC, pData[i].x, pData[i].y, pColor[i]); //还原上一个位置的颜色
        }
 
        //设置新的位置，i%3用于将星星分为3类采用不同的速度，以便形成参次感
        PVx = Random(2) - 1 + Vx * (i % 3);
        PVy = Vy * (i % 3 + 1);
        pData[i].x = pData[i].x + PVx; //+pvx与前一点形成的距离差，形成速度的差别
        pData[i].y = pData[i].y + PVy; //同理
 
        //取的新位置的原始点的颜色值，用于下一步星星飘过时恢复此处的颜色
        pColor[i] = GetPixel(hScreenDC, pData[i].x, pData[i].y);
 
        //如果获取颜色值失败，表明星星已经飘出屏幕，重新初始化，GetPixel如果获取失败就返回-1.
        if (pColor[i] == -1)
        {
            InitPoint(i);
        }
        else
        {
            //如果星星没有重叠，若对比度较小（即不能堆积）就画出星星
            //Random(16)>5用于防止某些连续而明显的边界截获所有的星星
            if (pColor[i] != StarColor)
            {
                //GetContrast(i)<50判断是否超出对比度50这个值就认为是边缘就会堆积星星
                if (Random(16) > 5 || GetContrast(i) < 50)
                {
                    SetPixel(hScreenDC, pData[i].x, pData[i].y, StarColor);
                } else
                {
                //否者表明找到明显的边界，画出堆积的学，并初始化以便画新的的星星
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
    HWND hwnd = GetActiveWindow(); // 获取当前窗口的句柄
 
    int i;
    srand(time(0));
    Vx = Random(4) - 2;
    Vy = Random(2) + 2;
 
    // 随机获取屏幕上一点
    for (i = 0; i < StarNum; i++)
    {
        // 保存坐标
        pData[i].x = Random(ScrnWidth);
        pData[i].y = Random(ScrnHight);
        // 保存颜色
        //pColor[i] = GetPixel(hScreenDC, pData[i].x, pData[i].y);
		pColor[i] = 100;
    }
 
    // 设置计时器
    SetTimer(hwnd, ID_TIMER, 5, NULL);
 
    // 获得整个屏幕画图设备
    hScreenDC = GetDC(0);
 
    // 计时
    timeCount = 0;
 
    // 循环获取操作系统发来的消息
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        switch(msg.message)
        {
        case WM_TIMER: // 时钟信号
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
