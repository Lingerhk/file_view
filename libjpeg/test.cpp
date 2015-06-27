#include <windows.h>
#include <stdio.h>
extern "C" {  
    #include "jpeglib.h"  
}
#pragma comment(lib, "libjpeg/jpeg.lib")

int bmptojpg24x(const char *strSourceFileName, const char *strDestFileName, int quality) ;

int main()
{
	bmptojpg24x("screen.bmp", "screen.jpg", 60);
	
	return 0;
}

int bmptojpg24x(const char *strSourceFileName, const char *strDestFileName, int quality)  
{  
    BITMAPFILEHEADER bfh;       // bmp文件头  
    BITMAPINFOHEADER bih;       // bmp头信息  
    RGBQUAD rq[256];            // 调色板  
    int i=0;  
  
    BYTE *data= NULL;//new BYTE[bih.biWidth*bih.biHeight];  
    BYTE *pData24 = NULL;//new BYTE[bih.biWidth*bih.biHeight];  
    int nComponent = 0;  
  
    // 打开图像文件  
    FILE *f = fopen(strSourceFileName,"rb");  
    if (f==NULL) {
        return 1;  
    }  
    // 读取文件头和图像信息 
    fread(&bfh,sizeof(bfh),1,f);
	fread(&bih,sizeof(bih),1,f); 
    
	data= new BYTE[bih.biWidth*bih.biHeight*4];  
    pData24 = new BYTE[bih.biWidth*bih.biHeight*3];  
    fseek(f,bfh.bfOffBits,SEEK_SET);      
    fread(data,bih.biWidth*bih.biHeight*4,1,f);  
    fclose(f);  
    for (i = 0;i<bih.biWidth*bih.biHeight;i++) {
        pData24[i*3] = data[i*4+2];  
        pData24[i*3+1] = data[i*4+1];  
        pData24[i*3+2] = data[i*4];  
     }  
     nComponent = 3;
  
    // 以上图像读取完毕  
    struct jpeg_compress_struct jcs;  
    struct jpeg_error_mgr jem;  
    jcs.err = jpeg_std_error(&jem);  
  
    jpeg_create_compress(&jcs);  
  
    f=fopen(strDestFileName,"wb");  
    if (f==NULL) {
        delete [] data;  
        return 1;  
    }  
    jpeg_stdio_dest(&jcs, f);  
    jcs.image_width = bih.biWidth;          // 为图的宽和高，单位为像素   
    jcs.image_height = bih.biHeight;  
    jcs.input_components = nComponent;          // 1,表示灰度图， 如果是彩色位图，则为3   
    if (nComponent==1)  
        jcs.in_color_space = JCS_GRAYSCALE; //JCS_GRAYSCALE表示灰度图，JCS_RGB表示彩色图像   
    else   
        jcs.in_color_space = JCS_RGB;  
  
    jpeg_set_defaults(&jcs);      
    jpeg_set_quality (&jcs, quality, true);  
    jpeg_start_compress(&jcs, TRUE);  
 
    JSAMPROW row_pointer[1];            // 一行位图  
    int row_stride;                     // 每一行的字节数   
  
    row_stride = jcs.image_width*nComponent; // 如果不是索引图,此处需要乘以3  
  
    // 对每一行进行压缩  
    while (jcs.next_scanline < jcs.image_height) {  
        row_pointer[0] = & pData24[(jcs.image_height-jcs.next_scanline-1) * row_stride];  
        jpeg_write_scanlines(&jcs, row_pointer, 1);  
    }  
  
    jpeg_finish_compress(&jcs);  
    jpeg_destroy_compress(&jcs);  
  
    fclose(f);  
    delete [] data;  
    delete [] pData24;  
  
    return 0;
}  