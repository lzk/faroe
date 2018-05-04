#include <stdio.h>
#include "macusbio.h"
#include "ScanCMD.h"
#include "model.h"
#include "string.h"
#include "../ImgFile/ImgFile.h"

//#define IMG_NAME_MODE_ONLY


//-- scan parameter -----------------------------------------------
SC_PAR_T_ sc_par = {SCAN_SOURCE, SCAN_ACQUIRE, SCAN_OPTION, SCAN_DUPLEX, SCAN_PAGE,
                  {IMG_FORMAT, IMG_OPTION, IMG_BIT, IMG_MONO,
                  {IMG_DPI_X, IMG_DPI_Y}, {IMG_ORG_X, IMG_ORG_Y}, IMG_WIDTH, IMG_HEIGHT},
				  //{{MTR_DRIV_TAR, MTR_STAT_MEC, MTR_DIRECT, MTR_MICRO_STEP, MTR_CURRENT, MTR_SPEED, MTR_ACC_STEP, 0},
                  //{MTR_DRIV_TAR, MTR_STAT_MEC, MTR_DIRECT, MTR_MICRO_STEP, MTR_CURRENT, MTR_SPEED, MTR_ACC_STEP, 0}}
				  {{0},
				  {0}}
				  ,{0}};

//IMG_FILE_T ImgFile[2] = {{&sc_par.img}, {&sc_par.img}};
IMG_FILE_T ImgFile[2];
IMAGE_T ImgTemp[2];

int time;

char filename[24];
U8  scanBuf[0x100000];
//-----------------------------------------------
SC_INFO_T_ sc_info;
//-----------------------------------------------

extern char IniFile[256], Profile[64];

extern int BatchNum;

extern U8 SCAN_DOC_SIZE;

//int WritePrivateProfileInt(LPCTSTR   lpAppName, LPCTSTR   lpKeyName, INT   Value, LPCSTR   lpFileName)
//{
//	TCHAR   ValBuf[16];

//	StringCbPrintf(ValBuf, sizeof(ValBuf), TEXT("%i"), Value);

//	TCHAR fileNamce[256];
//	StringCbPrintf(fileNamce, sizeof(fileNamce), TEXT("%s"), lpFileName);

//	return(WritePrivateProfileString(lpAppName, lpKeyName, ValBuf, fileNamce));
//}

int GetPrivateProfileIntA(const char* lpAppName ,const char* lpKeyName ,int nDefault ,const char* lpFileName)
{
    return 0;
}

U32 write_to_ini(void)
{

//	GetModuleFileNameA(NULL, IniFile, sizeof(IniFile));
//	strcpy(strrchr(IniFile, '.'), "_calibration.ini");

//	WritePrivateProfileStringA("OPTION", "SOURCE", (LPSTR)(sc_par.source == I3('FLB') ? "FLB" : "ADF"), IniFile);
//	WritePrivateProfileStringA("OPTION", "FORMAT", (LPSTR)(sc_par.img.format == I3('JPG') ? "JPG" : "TIF"), IniFile);
//	WritePrivateProfileInt(L"OPTION", L"COLOR_MODE", sc_par.img.mono, IniFile);
//	WritePrivateProfileInt(L"OPTION", L"PIXEL_DEPTH", sc_par.img.bit, IniFile);
//	WritePrivateProfileInt(L"OPTION", L"MAIN_SOLU", sc_par.img.dpi.x, IniFile);
//	WritePrivateProfileInt(L"OPTION", L"SUB_SOLU", sc_par.img.dpi.y, IniFile);
//	WritePrivateProfileInt(L"OPTION", L"SCAN_SIDE", sc_par.duplex, IniFile);
//	if ((SCAN_DOC_SIZE != DOC_S_PRNU) && (SCAN_DOC_SIZE != DOC_K_PRNU))
//		WritePrivateProfileInt(L"OPTION", L"SCAN_SIZE", SCAN_DOC_SIZE, IniFile);


	return TRUE;
}

U32 read_from_ini(void)
{
//    GetModuleFileNameA(NULL, IniFile, sizeof(IniFile));
//    strcpy(strrchr(IniFile, '.') , "_calibration.ini");

//    GetPrivateProfileStringA("OPTION", "SOURCE", "ADF", (LPSTR)&sc_par.source, 4, IniFile);
    //sc_par.acquire = 0;
    //if(GetPrivateProfileInt(Profile, "SCAN_TEST_PATTERN", 0, IniFile))
    //  par->acquire |= ACQ_TEST_PATTERN;
    //par->option = 0;
    //par->duplex = GetPrivateProfileInt(Profile, "SCAN_DUPLEX", 1, IniFile);
    //par->page = GetPrivateProfileInt(Profile, "SCAN_PAGE", 0, IniFile);

//    GetPrivateProfileStringA("OPTION", "FORMAT", "JPG", (LPSTR)&sc_par.img.format, 4, IniFile);
//    if (sc_par.img.format != I3('JPG'))
//        sc_par.img.format = I3('RAW');

//    sc_par.img.mono = GetPrivateProfileIntA("OPTION", "COLOR_MODE", IMG_MONO, IniFile);
//    sc_par.img.bit = GetPrivateProfileIntA("OPTION", "PIXEL_DEPTH", IMG_BIT, IniFile);
//    sc_par.img.dpi.x = GetPrivateProfileIntA("OPTION", "MAIN_SOLU", IMG_DPI_X, IniFile);
//    sc_par.img.dpi.y = GetPrivateProfileIntA("OPTION", "SUB_SOLU", IMG_DPI_Y, IniFile);
//    SCAN_DOC_SIZE = GetPrivateProfileIntA("OPTION", "SCAN_SIZE", DOC_SIZE_FULL, IniFile);
//    sc_par.duplex = GetPrivateProfileIntA("OPTION", "SCAN_SIDE", SCAN_AB_SIDE, IniFile);
	return TRUE;
}

int Load_ScanParameter(SC_PAR_T_ *par)
{
	IMAGE_T *img = (IMAGE_T *)&par->img;

/***************************IMAGE_T**********************************/
	par->option = 0;

	if(par->source == SCAN_ADF) {
		//par->duplex = SCAN_AB_SIDE;
		par->duplex = par->duplex;
	}
	else {
		par->duplex = SCAN_A_SIDE;
	}
        
	//par->page = 0;

	if(img->format == I3('JPG')) {
		img->option = IMG_OPT_JPG_FMT444;
	}


	switch(SCAN_DOC_SIZE) {
		case DOC_SIZE_FULL:
			img->width = IMG_300_DOT_X * img->dpi.x/300;
			img->org.x = IMG_300_ORG_X * img->dpi.x/300;

			img->height = IMG_300_DOT_Y *img->dpi.y/300;
			break;

		case DOC_SIZE_A4:
			img->width = IMG_A4_300_DOT_X * img->dpi.x/300;
			img->org.x = IMG_A4_300_ORG_X * img->dpi.x/300;

			img->height = IMG_A4_300_DOT_Y *img->dpi.y/300;
			break;

		case DOC_SIZE_A6:
			img->width = IMG_A6_300_DOT_X * img->dpi.x / 300;
			img->org.x = IMG_A6_300_ORG_X * img->dpi.x / 300;

			img->height = IMG_A6_300_DOT_Y *img->dpi.y / 300;
			break;

		case DOC_SIZE_LT:
			img->width = IMG_LT_300_DOT_X * img->dpi.x/300;
			img->org.x = IMG_LT_300_ORG_X * img->dpi.x/300;

			img->height = IMG_LT_300_DOT_Y *img->dpi.y/300;
			break;

        case DOC_SIZE_LG14:
			img->width = IMG_LG14_300_DOT_X * img->dpi.x/300;
			img->org.x = IMG_LG14_300_ORG_X * img->dpi.x/300;

			img->height = IMG_LG14_300_DOT_Y *img->dpi.y/300;
			break;

		case DOC_SIZE_LL:
			img->width = IMG_LL_300_DOT_X * img->dpi.x/300;
			img->org.x = IMG_LL_300_ORG_X * img->dpi.x/300;

			img->height = IMG_LL_300_DOT_Y *img->dpi.y/300;
			break;

		case DOC_CHART_A01:
			img->width = IMG_CHART_A01_300_DOT_X * img->dpi.x/300;
			img->org.x = IMG_CHART_A01_300_ORG_X * img->dpi.x/300;

			img->height = IMG_CHART_A01_300_DOT_Y *img->dpi.y/300;
			break;

		case DOC_FB_LIFE:
			img->width = IMG_FB_LIFE_300_DOT_X * img->dpi.x/300;
			img->org.x = 0 * img->dpi.x/300;

			img->height = IMG_FB_LIFE_300_DOT_Y *img->dpi.y/300;
			break;

		case DOC_S_PRNU:
			img->width = IMG_S_PRNU_300_DOT_X * img->dpi.x/300;
			img->org.x = 0 * img->dpi.x/300;

			img->height = IMG_S_PRUN_300_DOT_Y *img->dpi.y/300;
			break;

		case DOC_K_PRNU:
			img->width = IMG_K_PRNU_300_DOT_X * img->dpi.x/300;
			img->org.x = 0 * img->dpi.x/300;

			img->height = IMG_K_PRUN_300_DOT_Y *img->dpi.y/300;
			break;

	}
	img->width -= (img->width%8);
	img->height -= (img->height%8);



/***************************MTR_T**********************************/
	if(par->acquire & ACQ_SET_MTR) {

		if(par->source == I3('FLB')) {
			/*FB scan*/
			//par->mtr[0].drive_target = MT_PH;
			//par->mtr[0].state_mechine = SCAN_STATE_MECHINE;
			switch(par->img.dpi.x) {
				case 300:
					par->mtr[0].speed_pps = GetPrivateProfileIntA("FB300x300M", "PPS", 0, IniFile);
					par->mtr[0].direction = GetPrivateProfileIntA("FB300x300M", "DIR", 0, IniFile);
					par->mtr[0].micro_step = GetPrivateProfileIntA("FB300x300M", "MS", 0, IniFile);
					par->mtr[0].currentLV = GetPrivateProfileIntA("FB300x300M", "CLV", 0, IniFile);
                    break;
				case 600:
					par->mtr[0].speed_pps = GetPrivateProfileIntA("FB600x600", "PPS", 0, IniFile); 
					par->mtr[0].direction = GetPrivateProfileIntA("FB600x600", "DIR", 0, IniFile); 
					par->mtr[0].micro_step = GetPrivateProfileIntA("FB600x600", "MS", 0, IniFile); 
					par->mtr[0].currentLV =  GetPrivateProfileIntA("FB600x600", "CLV", 0, IniFile);
					break;
			}
		}
		else {
			/*ADF scan*/
			par->mtr[1].pick_ss_step = GetPrivateProfileIntA("PICK_SS_STEP", "STEP", 0, IniFile);

			//par->mtr[0].drive_target = CMT_PH;
			//par->mtr[0].state_mechine = SCAN_STATE_MECHINE;
			//par->mtr[1].drive_target = BMT_PH;
			//par->mtr[1].state_mechine = STATE_MECHINE_1;
			if(par->img.bit < 24) {
				/*Mono scan*/
				par->mtr[1].speed_pps = GetPrivateProfileIntA("ADFgray", "PICK_PPS", 0, IniFile);
				par->mtr[1].direction = GetPrivateProfileIntA("ADFgray", "PICK_DIR", 0, IniFile);
				par->mtr[1].micro_step = GetPrivateProfileIntA("ADFgray", "PICK_MS", 0, IniFile);
				par->mtr[1].currentLV = GetPrivateProfileIntA("ADFgray", "PICK_CLV", 0, IniFile);

				par->mtr[0].speed_pps = GetPrivateProfileIntA("ADFgray", "FEED_PPS", 0, IniFile); 
				par->mtr[0].direction = GetPrivateProfileIntA("ADFgray", "FEED_DIR", 0, IniFile); 
				par->mtr[0].micro_step = GetPrivateProfileIntA("ADFgray", "FEED_MS", 0, IniFile); 
				par->mtr[0].currentLV = GetPrivateProfileIntA("ADFgray", "FEED_CLV", 0, IniFile); 
			}
			else {
				/*Color scan*/
				if((par->img.dpi.x == 300) && (par->img.dpi.y == 300)) {
					/*300x300DPI scan*/
					par->mtr[1].speed_pps = GetPrivateProfileIntA("ADF300x300color", "PICK_PPS", 0, IniFile);
					par->mtr[1].direction = GetPrivateProfileIntA("ADF300x300color", "PICK_DIR", 0, IniFile);
					par->mtr[1].micro_step = GetPrivateProfileIntA("ADF300x300color", "PICK_MS", 0, IniFile);
					par->mtr[1].currentLV = GetPrivateProfileIntA("ADF300x300color", "PICK_CLV", 0, IniFile);

					par->mtr[0].speed_pps = GetPrivateProfileIntA("ADF300x300color", "FEED_PPS", 0, IniFile); 
					par->mtr[0].direction = GetPrivateProfileIntA("ADF300x300color", "FEED_DIR", 0, IniFile); 
					par->mtr[0].micro_step = GetPrivateProfileIntA("ADF300x300color", "FEED_MS", 0, IniFile); 
					par->mtr[0].currentLV = GetPrivateProfileIntA("ADF300x300color", "FEED_CLV", 0, IniFile); 
				}
				else if((par->img.dpi.x == 300) && (par->img.dpi.y == 600)) {
					/*300x600DPI scan*/
					par->mtr[1].speed_pps = GetPrivateProfileIntA("ADF300x600color", "PICK_PPS", 0, IniFile);
					par->mtr[1].direction = GetPrivateProfileIntA("ADF300x600color", "PICK_DIR", 0, IniFile);
					par->mtr[1].micro_step = GetPrivateProfileIntA("ADF300x600color", "PICK_MS", 0, IniFile);
					par->mtr[1].currentLV = GetPrivateProfileIntA("ADF300x600color", "PICK_CLV", 0, IniFile);

					par->mtr[0].speed_pps = GetPrivateProfileIntA("ADF300x600color", "FEED_PPS", 0, IniFile); 
					par->mtr[0].direction = GetPrivateProfileIntA("ADF300x600color", "FEED_DIR", 0, IniFile); 
					par->mtr[0].micro_step = GetPrivateProfileIntA("ADF300x600color", "FEED_MS", 0, IniFile); 
					par->mtr[0].currentLV = GetPrivateProfileIntA("ADF300x600color", "FEED_CLV", 0, IniFile);	
				}
				else {
					/*600DPI scan*/
					par->mtr[1].speed_pps = GetPrivateProfileIntA("ADF600x600color", "PICK_PPS", 0, IniFile);
					par->mtr[1].direction = GetPrivateProfileIntA("ADF600x600color", "PICK_DIR", 0, IniFile);
					par->mtr[1].micro_step = GetPrivateProfileIntA("ADF600x600color", "PICK_MS", 0, IniFile);
					par->mtr[1].currentLV = GetPrivateProfileIntA("ADF600x600color", "PICK_CLV", 0, IniFile);

					par->mtr[0].speed_pps = GetPrivateProfileIntA("ADF600x600color", "FEED_PPS", 0, IniFile); 
					par->mtr[0].direction = GetPrivateProfileIntA("ADF600x600color", "FEED_DIR", 0, IniFile); 
					par->mtr[0].micro_step = GetPrivateProfileIntA("ADF600x600color", "FEED_MS", 0, IniFile); 
					par->mtr[0].currentLV = GetPrivateProfileIntA("ADF600x600color", "FEED_CLV", 0, IniFile); 
				}

			}
		}

		switch(par->mtr[0].currentLV) {
			case 1:
				par->mtr[0].currentLV = 0;
				break;
			case 2:
				par->mtr[0].currentLV = 2;
				break;
			case 3:
				par->mtr[0].currentLV = 1;
				break;
			case 4:
				par->mtr[0].currentLV = 3;
				break;
		}

		switch(par->mtr[1].currentLV) {
			case 1:
				par->mtr[1].currentLV = 0;
				break;
			case 2:
				par->mtr[1].currentLV = 2;
				break;
			case 3:
				par->mtr[1].currentLV = 1;
				break;
			case 4:
				par->mtr[1].currentLV = 3;
				break;
		}
		
	}
/***************************ME_T**********************************/
	if(par->acquire & ACQ_SET_ME) {
		par->me.leading_edge = GetPrivateProfileIntA("FB_LEADING", "FB_leading", 0, IniFile);
		par->me.img_gap = GetPrivateProfileIntA("CIS_GAP", "cis_gap", 0, IniFile);
		par->me.prefed = GetPrivateProfileIntA("PREFEED", "prefeed", 0, IniFile);
		par->me.postfed = GetPrivateProfileIntA("POSTFEED", "postfeed", 0, IniFile);
		par->me.side_edgeA = GetPrivateProfileIntA("START_PIXEL_A", "start_pixel_A", 0, IniFile);
		par->me.side_edgeB = GetPrivateProfileIntA("START_PIXEL_B", "start_pixel_B", 0, IniFile);
	}

	memcpy(&ImgTemp[0], &sc_par.img, sizeof(IMAGE_T));
	memcpy(&ImgTemp[1], &sc_par.img, sizeof(IMAGE_T));

	ImgFile[0].img = ImgTemp[0];
	ImgFile[1].img = ImgTemp[1];


	write_to_ini();

	//sc_par.acquire |= ACQ_NO_SHADING;
	//sc_par.acquire |= ACQ_TEST_PATTERN;

	return TRUE;
}

int Load_CalibrationParameter(SC_PAR_T_ *par)
{
	IMAGE_T *img = (IMAGE_T *)&par->img;

/***************************IMAGE_T**********************************/
	par->option = 0;

	if(img->format == I3('JPG')) {
		img->option = IMG_OPT_JPG_FMT444;
	}


	switch(SCAN_DOC_SIZE) {
		case DOC_SIZE_FULL:
			img->width = IMG_300_DOT_X * img->dpi.x/300;
			img->org.x = IMG_300_ORG_X * img->dpi.x/300;

			img->height = IMG_300_DOT_Y *img->dpi.y/300;
			break;

		case DOC_SIZE_A4:
			img->width = IMG_A4_300_DOT_X * img->dpi.x/300;
			img->org.x = IMG_A4_300_ORG_X * img->dpi.x/300;

			img->height = IMG_A4_300_DOT_Y *img->dpi.y/300;
			break;

		case DOC_SIZE_LT:
			img->width = IMG_LT_300_DOT_X * img->dpi.x/300;
			img->org.x = IMG_LT_300_ORG_X * img->dpi.x/300;

			img->height = IMG_LT_300_DOT_Y *img->dpi.y/300;
			break;

		case DOC_SIZE_LL:
			img->width = IMG_LL_300_DOT_X * img->dpi.x/300;
			img->org.x = IMG_LL_300_ORG_X * img->dpi.x/300;

			img->height = IMG_LL_300_DOT_Y *img->dpi.y/300;
			break;

		case DOC_FB_LIFE:
			img->width = IMG_FB_LIFE_300_DOT_X * img->dpi.x/300;
			img->org.x = 0 * img->dpi.x/300;

			img->height = IMG_FB_LIFE_300_DOT_Y *img->dpi.y/300;
			break;

		case DOC_S_PRNU:
			img->width = IMG_S_PRNU_300_DOT_X * img->dpi.x/300;
			img->org.x = 0 * img->dpi.x/300;

			img->height = IMG_S_PRUN_300_DOT_Y *img->dpi.y/300;
			break;

		case DOC_K_PRNU:
			img->width = IMG_K_PRNU_300_DOT_X * img->dpi.x/300;
			img->org.x = 0 * img->dpi.x/300;

			img->height = IMG_K_PRUN_300_DOT_Y *img->dpi.y/300;
			//img->height = IMG_K_PRUN_300_DOT_Y *300/img->dpi.y; //Image buf issue
			break;

		case DOC_K_PREFEED:
			img->width = IMG_K_PREFEED_300_DOT_X * img->dpi.x/300;
			img->org.x = 0 * img->dpi.x/300;

			img->height = IMG_K_PREFEED_300_DOT_Y *img->dpi.y/300;
			break;
	}
	img->width -= (img->width%8);
	img->height -= (img->height%8);



/***************************MTR_T**********************************/
	if(par->acquire & ACQ_SET_MTR) {

		if(par->source == I3('FLB')) {
			/*FB scan*/
			//par->mtr[0].drive_target = MT_PH;
			//par->mtr[0].state_mechine = SCAN_STATE_MECHINE;
			switch(par->img.dpi.x) {
				case 300:
					par->mtr[0].speed_pps = GetPrivateProfileIntA("FB300x300M", "PPS", 0, IniFile);
					par->mtr[0].direction = GetPrivateProfileIntA("FB300x300M", "DIR", 0, IniFile);
					par->mtr[0].micro_step = GetPrivateProfileIntA("FB300x300M", "MS", 0, IniFile);
					par->mtr[0].currentLV = GetPrivateProfileIntA("FB300x300M", "CLV", 0, IniFile);
                    break;
				case 600:
					par->mtr[0].speed_pps = GetPrivateProfileIntA("FB600x600", "PPS", 0, IniFile); 
					par->mtr[0].direction = GetPrivateProfileIntA("FB600x600", "DIR", 0, IniFile); 
					par->mtr[0].micro_step = GetPrivateProfileIntA("FB600x600", "MS", 0, IniFile); 
					par->mtr[0].currentLV =  GetPrivateProfileIntA("FB600x600", "CLV", 0, IniFile);
					break;
			}
		}
		else {
			/*ADF scan*/
			par->mtr[1].pick_ss_step = GetPrivateProfileIntA("PICK_SS_STEP", "STEP", 0, IniFile);

			//par->mtr[0].drive_target = CMT_PH;
			//par->mtr[0].state_mechine = SCAN_STATE_MECHINE;
			//par->mtr[1].drive_target = BMT_PH;
			//par->mtr[1].state_mechine = STATE_MECHINE_1;
			if(par->img.bit < 24) {
				/*Mono scan*/
				par->mtr[1].speed_pps = GetPrivateProfileIntA("ADFgray", "PICK_PPS", 0, IniFile);
				par->mtr[1].direction = GetPrivateProfileIntA("ADFgray", "PICK_DIR", 0, IniFile);
				par->mtr[1].micro_step = GetPrivateProfileIntA("ADFgray", "PICK_MS", 0, IniFile);
				par->mtr[1].currentLV = GetPrivateProfileIntA("ADFgray", "PICK_CLV", 0, IniFile);

				par->mtr[0].speed_pps = GetPrivateProfileIntA("ADFgray", "FEED_PPS", 0, IniFile); 
				par->mtr[0].direction = GetPrivateProfileIntA("ADFgray", "FEED_DIR", 0, IniFile); 
				par->mtr[0].micro_step = GetPrivateProfileIntA("ADFgray", "FEED_MS", 0, IniFile); 
				par->mtr[0].currentLV = GetPrivateProfileIntA("ADFgray", "FEED_CLV", 0, IniFile); 
			}
			else {
				/*Color scan*/
				if((par->img.dpi.x == 300) && (par->img.dpi.y == 300)) {
					/*300x300DPI scan*/
					par->mtr[1].speed_pps = GetPrivateProfileIntA("ADF300x300color", "PICK_PPS", 0, IniFile);
					par->mtr[1].direction = GetPrivateProfileIntA("ADF300x300color", "PICK_DIR", 0, IniFile);
					par->mtr[1].micro_step = GetPrivateProfileIntA("ADF300x300color", "PICK_MS", 0, IniFile);
					par->mtr[1].currentLV = GetPrivateProfileIntA("ADF300x300color", "PICK_CLV", 0, IniFile);

					par->mtr[0].speed_pps = GetPrivateProfileIntA("ADF300x300color", "FEED_PPS", 0, IniFile); 
					par->mtr[0].direction = GetPrivateProfileIntA("ADF300x300color", "FEED_DIR", 0, IniFile); 
					par->mtr[0].micro_step = GetPrivateProfileIntA("ADF300x300color", "FEED_MS", 0, IniFile); 
					par->mtr[0].currentLV = GetPrivateProfileIntA("ADF300x300color", "FEED_CLV", 0, IniFile); 
				}
				else if((par->img.dpi.x == 300) && (par->img.dpi.y == 600)) {
					/*300x600DPI scan*/
					par->mtr[1].speed_pps = GetPrivateProfileIntA("ADF300x600color", "PICK_PPS", 0, IniFile);
					par->mtr[1].direction = GetPrivateProfileIntA("ADF300x600color", "PICK_DIR", 0, IniFile);
					par->mtr[1].micro_step = GetPrivateProfileIntA("ADF300x600color", "PICK_MS", 0, IniFile);
					par->mtr[1].currentLV = GetPrivateProfileIntA("ADF300x600color", "PICK_CLV", 0, IniFile);

					par->mtr[0].speed_pps = GetPrivateProfileIntA("ADF300x600color", "FEED_PPS", 0, IniFile); 
					par->mtr[0].direction = GetPrivateProfileIntA("ADF300x600color", "FEED_DIR", 0, IniFile); 
					par->mtr[0].micro_step = GetPrivateProfileIntA("ADF300x600color", "FEED_MS", 0, IniFile); 
					par->mtr[0].currentLV = GetPrivateProfileIntA("ADF300x600color", "FEED_CLV", 0, IniFile);	
				}
				else {
					/*600DPI scan*/
					par->mtr[1].speed_pps = GetPrivateProfileIntA("ADF600x600color", "PICK_PPS", 0, IniFile);
					par->mtr[1].direction = GetPrivateProfileIntA("ADF600x600color", "PICK_DIR", 0, IniFile);
					par->mtr[1].micro_step = GetPrivateProfileIntA("ADF600x600color", "PICK_MS", 0, IniFile);
					par->mtr[1].currentLV = GetPrivateProfileIntA("ADF600x600color", "PICK_CLV", 0, IniFile);

					par->mtr[0].speed_pps = GetPrivateProfileIntA("ADF600x600color", "FEED_PPS", 0, IniFile); 
					par->mtr[0].direction = GetPrivateProfileIntA("ADF600x600color", "FEED_DIR", 0, IniFile); 
					par->mtr[0].micro_step = GetPrivateProfileIntA("ADF600x600color", "FEED_MS", 0, IniFile); 
					par->mtr[0].currentLV = GetPrivateProfileIntA("ADF600x600color", "FEED_CLV", 0, IniFile); 
				}

			}
		}

		switch(par->mtr[0].currentLV) {
			case 1:
				par->mtr[0].currentLV = 0;
				break;
			case 2:
				par->mtr[0].currentLV = 2;
				break;
			case 3:
				par->mtr[0].currentLV = 1;
				break;
			case 4:
				par->mtr[0].currentLV = 3;
				break;
		}

		switch(par->mtr[1].currentLV) {
			case 1:
				par->mtr[1].currentLV = 0;
				break;
			case 2:
				par->mtr[1].currentLV = 2;
				break;
			case 3:
				par->mtr[1].currentLV = 1;
				break;
			case 4:
				par->mtr[1].currentLV = 3;
				break;
		}
		
	}
/***************************ME_T**********************************/
	if(par->acquire & ACQ_SET_ME) {
		par->me.leading_edge = GetPrivateProfileIntA("FB_LEADING", "FB_leading", 0, IniFile);
		par->me.img_gap = GetPrivateProfileIntA("CIS_GAP", "cis_gap", 0, IniFile);
		par->me.prefed = GetPrivateProfileIntA("PREFEED", "prefeed", 0, IniFile);
		par->me.postfed = GetPrivateProfileIntA("POSTFEED", "postfeed", 0, IniFile);
		par->me.side_edgeA = GetPrivateProfileIntA("START_PIXEL_A", "start_pixel_A", 0, IniFile);
		par->me.side_edgeB = GetPrivateProfileIntA("START_PIXEL_B", "start_pixel_B", 0, IniFile);
	}


	write_to_ini();

	return TRUE;
}


int ResetScanFlow(void)
{
	_cancel();
	_JobEnd();

	return TRUE;
}

/*
#define JOB_WAIT_TIMEOUT  5000
int waitJobFinish(int job, int wait_motor_stop)
{
  U32 tick = GetTickCount();
  while((GetTickCount() - tick) < JOB_WAIT_TIMEOUT) {
    if(!_info(&sc_info))
      break;
    if(!(sc_info.JobState & job) && (!wait_motor_stop || !sc_info.MotorMove))
      return TRUE;
    Sleep(100);
  }
  return FALSE;
}
*/
//-------------------------------------------------
//int GammaTransLTCtoGL(unsigned int *pbyRed, unsigned int *pbyGreen,unsigned int *pbyBlue,unsigned int *GLGamma)
//{
//    int i;
//    for(i=0;i<256;i++)
//    {
//		if(i<255){
//		   GLGamma[i]    = ((unsigned int)(*(pbyRed+i*256))&0xffff) + (unsigned int)(((*(pbyRed+((i+1)*256)))&0xffff)<<16); 
//		   GLGamma[i+256] = ((unsigned int)(*(pbyGreen+i*256))&0xffff) + (unsigned int)(((*(pbyGreen+((i+1)*256)))&0xffff)<<16); 
//		   GLGamma[i+256*2]= ((unsigned int)(*(pbyBlue+i*256))&0xffff) + (unsigned int)(((*(pbyBlue+((i+1)*256)))&0xffff)<<16); 
//		}
//		else{
//			GLGamma[i]    =(unsigned int)(*(pbyRed+i*256))&0xffff|((unsigned int)(0xffff)<<16); 
//			GLGamma[i+256] = (unsigned int)(*(pbyGreen+i*256))&0xffff|((unsigned int)(0xffff)<<16); 
//			GLGamma[i+256*2]= (unsigned int)(*(pbyBlue+i*256))&0xffff|((unsigned int)(0xffff)<<16); 
//		}
//
//
//    }
//    return 1;
//}

#define START_STAGE					0x1
#define SCANNING_STAGE				0x2
#define PUSH_TRANSFER_STAGE		0x3

Scan_RET ScannerStatusCheck_(char stage)
{
	Scan_RET result = RETSCAN_OK;

	if (!_info(&sc_info)) {
		printf("INFO command error!!");
		result = RETSCAN_OPENFAIL;
	}
	else {
		if (sc_info.JobID || sc_info.SystemStatus.scanning) {
			if (stage == START_STAGE) {
				printf("Last job not finish!!\n");
				result = RETSCAN_JOB_GOGING;
                return result;
			}
		}
		else {
			if (stage == SCANNING_STAGE) {
				printf("Scan job missing!!\n");
				result = RETSCAN_JOB_MISSING;
                return result;
			}
		}


		if (sc_info.ErrorStatus.cover_open_err) {
			if (stage != PUSH_TRANSFER_STAGE) {
				printf("COVER_OPEN_ERR\n");
				result = RETSCAN_COVER_OPEN;
                return result;
			}
		}
		if (sc_info.ErrorStatus.scan_jam_err) {
			printf("SCAN_JAM_ERR\n");
			result = RETSCAN_PAPER_JAM;
            return result;
		}
		if (sc_info.ErrorStatus.scan_canceled_err) {
			printf("SCAN_CANCELED_ERR\n");
			result = RETSCAN_CANCEL;
            return result;
		}
		if (sc_info.ErrorStatus.scan_timeout_err) {
			printf("SCAN_TIMEOUT_ERR\n");
			result = RETSCAN_TIME_OUT;
            return result;
		}
		if (sc_info.ErrorStatus.multi_feed_err) {
			printf("MULTI_FEED_ERR\n");
			result = RETSCAN_ULTRA_SONIC;
            return result;
		}
		if (sc_info.ErrorStatus.usb_transfer_err) {
			printf("USB_TRANSFER_ERR\n");
			result = RETSCAN_USB_TRANSFERERROR;
            return result;
		}
		if (sc_info.ErrorStatus.wifi_transfer_err) {
			printf("WiFi_TRANSFER_ERR\n");
			result = RETSCAN_WIFI_TRANSFERERROR;
            return result;
		}
		//if(sc_info.ErrorStatus.usb_disk_transfer_err) {
		//	printf("USBDISK_TRANSFER_ERR\n");
		//	result = FALSE;
		//}
		//if(sc_info.ErrorStatus.ftp_transfer_err) {
		//	printf("FTP_TRANSFER_ERR\n");
		//	result = FALSE;
		//}
		//if(sc_info.ErrorStatus.smb_transfer_err) {
		//	printf("SMB_TRANSFER_ERR\n");
		//	result = FALSE;
		//}


		if (stage == START_STAGE) {
			if (sc_info.SensorStatus.adf_document_sensor) {
				printf("ADF document not ready.\n");
				result = RETSCAN_ADFDOC_NOT_READY;
                return result;
			}
			if (sc_info.SensorStatus.adf_paper_sensor) {
				printf("ADF path not ready.\n");
				result = RETSCAN_ADFPATH_NOT_READY;
                return result;
			}
			if (sc_info.SensorStatus.cover_sensor) {
				printf("ADF cover not ready.\n");
				result = RETSCAN_ADFCOVER_NOT_READY;
                return result;
			}
		}
	}

	return result;
}

typedef wchar_t TCHAR;
TCHAR   ValBuf[16];
extern int _gamma();

//-------------------------------------------------


