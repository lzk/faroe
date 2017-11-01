#include "ImgFile.h"
#define FALSE 0

int ImgFile_Open(IMG_FILE_T *imgfile, char *filename)
{
	int result = FALSE;
//	switch(imgfile->img.format) {
//        case IMG_FMT_JPG:
//			result = Jpeg_OpenFile(imgfile, filename);
//			break;
//        case IMG_FMT_TIF:
//			result = Tiff_OpenFile(imgfile, filename);
//			break;
//        case IMG_FMT_RAW:
//			result = Raw_OpenFile(imgfile, filename);
//			break;
//        case IMG_FMT_BMP:
//			result = Bmp_OpenFile(imgfile, filename);
//			break;
//	}
    if(imgfile->img.format == IMG_FMT_JPG){
        result = Jpeg_OpenFile(imgfile, filename);
    }else if(imgfile->img.format == IMG_FMT_TIF){
        result = Tiff_OpenFile(imgfile, filename);
    }else if(imgfile->img.format == IMG_FMT_RAW){
        result = Raw_OpenFile(imgfile, filename);
    }else if(imgfile->img.format == IMG_FMT_BMP){
        result = Bmp_OpenFile(imgfile, filename);
    }
	return result;
}

int ImgFile_Write(IMG_FILE_T *imgfile, void *data, int size)
{
	int result = FALSE;
//	switch(imgfile->img.format) {
//        case IMG_FMT_JPG:
//			result = Jpeg_WriteFile(imgfile, data, size);
//			break;
//        case IMG_FMT_TIF:
//			result = Tiff_WriteFile(imgfile, data, size);
//			break;
//        case IMG_FMT_RAW:
//			result = Raw_WriteFile(imgfile, data, size);
//			break;
//        case IMG_FMT_BMP:
//			result = Bmp_WriteFile(imgfile, data, size);
//			break;
//	}
    if(imgfile->img.format == IMG_FMT_JPG){
        result = Jpeg_WriteFile(imgfile, data, size);
    }else if(imgfile->img.format == IMG_FMT_TIF){
        result = Tiff_WriteFile(imgfile, data, size);
    }else if(imgfile->img.format == IMG_FMT_RAW){
        result = Raw_WriteFile(imgfile, data, size);
    }else if(imgfile->img.format == IMG_FMT_BMP){
        result = Bmp_WriteFile(imgfile, data, size);
    }
	return result;
}

int ImgFile_Close(IMG_FILE_T *imgfile, int lines)
{
	int result = FALSE;
//	switch(imgfile->img.format) {
//        case IMG_FMT_JPG:
//			result = Jpeg_CloseFile(imgfile, lines);
//			break;
//        case IMG_FMT_TIF:
//			result = Tiff_CloseFile(imgfile);
//			break;
//        case IMG_FMT_RAW:
//			result = Raw_CloseFile(imgfile);
//			break;
//        case IMG_FMT_BMP:
//			result = Bmp_CloseFile(imgfile);
//			break;
//	}
    if(imgfile->img.format == IMG_FMT_JPG){
        result = Jpeg_CloseFile(imgfile, lines);
    }else if(imgfile->img.format == IMG_FMT_TIF){
        result = Tiff_CloseFile(imgfile);
    }else if(imgfile->img.format == IMG_FMT_RAW){
        result = Raw_CloseFile(imgfile);
    }else if(imgfile->img.format == IMG_FMT_BMP){
        result = Bmp_CloseFile(imgfile);
    }
	return result;
}

