#include <Windows.h>
#include <stdio.h>
#include <math.h>
void SaveAsBmp(const char* file, char* lpImg, int imWidth, int imHeight, bool bDir)
//函数用于将图像数据保存为BMP文件
{
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;//BMP文件头信息

	int nNumColors;

	short color = 3;
	short wBitCount = color * 8;
	long BytesPerLine = imWidth * 3;  //WIDTHBYTES(wBitCount*imWidth);

	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = long(imWidth);         // fill in width from parameter 
	bi.biHeight = long(imHeight);       // fill in height from parameter 
	bi.biPlanes = 1;              // must be 1 
	bi.biBitCount = wBitCount;    // from parameter 
	bi.biCompression = BI_RGB;
	bi.biSizeImage = BytesPerLine * imHeight;           // 0's here mean "default" 
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	bf.bfType = 0x4d42;
	bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi.biSizeImage;
	bf.bfReserved1 = 0;
	bf.bfReserved2 = 0;
	bf.bfOffBits = (DWORD)(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));

	FILE* fd;
	fopen_s(&fd,file, "wb");

	fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, fd);
	fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, fd);

	char* /*lpLine0,*/ lpLine;
	short  nline;
	nline = imWidth * color;

	if (bDir)//IPImg存储图像颜色信息，参数bDir确定读入方式（整块/倒行）
		fwrite(lpImg, 1, bi.biSizeImage, fd); // 直接整块写入
	else
	{
		// 倒行写入
		lpLine = lpImg + BytesPerLine * (imHeight - 1);
		for (int i = 0; i < imHeight; i++)
		{
			fwrite(lpLine, 1, BytesPerLine, fd); // 一行写入
			lpLine -= BytesPerLine;
		}
	}


	fclose(fd);
}

char* lpImgData = NULL;
int colImg = 1028, rowImg = 1024; // 宽，高可以改，但是这里宽必须是4的整数倍

//设置像素，第i行，j列的像素 设置颜色为 (r,g,b)
inline void  SetPixel(int j, int i, char r, char g, char b)//设置点像素颜色
{
	if (j >= 0 && j < colImg && i >= 0 && i < rowImg)  // 这里做了有效范围判断
	{//此为坐标i，j有效范围判断
		char* v = lpImgData + (i * colImg + j) * 3;
		v[0] = b;	v[1] = g;	v[2] = r;
	}
}


void drawXDX(float dx,float y,int r,int g,int b)
{
	int i = rowImg / 2 + y + 0.5;
	int startX = colImg / 2 - dx + 0.5;//x范围 0.5是为了四舍五入
	int endX = colImg / 2 + dx + 0.5;
	startX = max(0, startX);
	endX = min(colImg - 1, endX);

	// 在指定的范围内绘制水平线段
	for (int j = startX; j <= endX; j++)
	{
		SetPixel(j, i, r, g, b);  // 红色表示图像上的这一条线段
	}
}
void drawXDY(float x, float dy1,float dy2, int r, int g, int b)
{
	int i = colImg / 2 - x + 0.5;
	int startY = rowImg / 2 + dy2 + 0.5;
	int endY = rowImg / 2 + dy1 + 0.5;
	startY = max(0, startY);
	endY = min(rowImg - 1, endY);

	// 在指定的范围内绘制竖直线段
	for (int j = startY; j <= endY; j++)
	{
		SetPixel(i, j, r, g, b);  // 红色表示图像上的这一条线段
	}
}
//修改该函数 ，通过 数学函数、循环条件控制语句，生成几何图案、或一些复杂图画
//可以通过设置坐标 x,y，数学计算的时候，x,y 可以按浮点数，  x相当于j   y相当于i， 进行图像赋值的时候，x,y 取整转成j,i
//如果坐标轴设置在图像中心，做下平移， j = x+ colImg/2,  i =y +rowImg/2
void  CeateImageData()//生成图像数据
{
	int i, j;
	////
	//////////////////////////
	//举例： 生成一个直线 y = 2x 
	float x, y;

	drawXDX(120.0, -180.0,200,200,40);
	drawXDX(120.0, -80.0, 200, 200, 40);
	drawXDX(80.0, 0.0, 240, 10, 140);
	drawXDY(80.0, 0.0, -80.0, 240, 10, 140);
	drawXDY(-80.0, 0.0, -80.0, 240, 10, 140);
	drawXDY(120.0, -80.0, -180.0, 200, 200, 40);
	drawXDY(-120.0, -80.0, -180.0, 200, 200, 40);

	for (x = -80.0; x <= 80.0; x += 1.0)
	{
		j = int(x + colImg / 2 + 0.5);

		y = sin(100*x);
		i = int(y - 36.0 + rowImg / 2 + 0.5);
		SetPixel(j, i, 255, 120, 10);
	}
	for (x = -120.0; x <= 120.0; x += 1.0)
	{
		j = int(x + colImg / 2 + 0.5);

		y = sin(100*x);
		i = int(y - 120.0 + rowImg / 2 + 0.5);
		SetPixel(j, i, 255, 150, 100);
	}

	for (x = -30.0; x<=30.0; x += 1.0)//画圆
	{
		j = int(x + colImg / 2 + 0.5);

		y = sqrt(900.0 - x * x);
		i = int(y +30.0+ rowImg / 2 + 0.5);
		SetPixel(j, i, 255, 150, 0);
		i = int(-y+30.0 + rowImg / 2 + 0.5);
		SetPixel(j, i, 255, 150, 0);
	}
	//////////////////////////////

}

int main(int argc, char* argv[])
{
	//这里按24位真彩色图像来保存
	lpImgData = new char[colImg * rowImg * 3];
	memset(lpImgData, 255, colImg * rowImg * 3);  //默认设置为255 白色

	CeateImageData();

	const char* fileName = "D:\\code\\23.11.22_imagecreat.bmp";   //生成的图像的路径名
	SaveAsBmp(fileName, lpImgData, colImg, rowImg, true);

	delete lpImgData;

	return 0;
}