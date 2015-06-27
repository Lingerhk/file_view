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
    BITMAPFILEHEADER bfh;       // bmp�ļ�ͷ  
    BITMAPINFOHEADER bih;       // bmpͷ��Ϣ  
    RGBQUAD rq[256];            // ��ɫ��  
    int i=0;  
  
    BYTE *data= NULL;//new BYTE[bih.biWidth*bih.biHeight];  
    BYTE *pData24 = NULL;//new BYTE[bih.biWidth*bih.biHeight];  
    int nComponent = 0;  
  
    // ��ͼ���ļ�  
    FILE *f = fopen(strSourceFileName,"rb");  
    if (f==NULL) {
        return 1;  
    }  
    // ��ȡ�ļ�ͷ��ͼ����Ϣ 
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
  
    // ����ͼ���ȡ���  
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
    jcs.image_width = bih.biWidth;          // Ϊͼ�Ŀ�͸ߣ���λΪ����   
    jcs.image_height = bih.biHeight;  
    jcs.input_components = nComponent;          // 1,��ʾ�Ҷ�ͼ�� ����ǲ�ɫλͼ����Ϊ3   
    if (nComponent==1)  
        jcs.in_color_space = JCS_GRAYSCALE; //JCS_GRAYSCALE��ʾ�Ҷ�ͼ��JCS_RGB��ʾ��ɫͼ��   
    else   
        jcs.in_color_space = JCS_RGB;  
  
    jpeg_set_defaults(&jcs);      
    jpeg_set_quality (&jcs, quality, true);  
    jpeg_start_compress(&jcs, TRUE);  
 
    JSAMPROW row_pointer[1];            // һ��λͼ  
    int row_stride;                     // ÿһ�е��ֽ���   
  
    row_stride = jcs.image_width*nComponent; // �����������ͼ,�˴���Ҫ����3  
  
    // ��ÿһ�н���ѹ��  
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