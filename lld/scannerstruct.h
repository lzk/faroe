#ifndef SCANNERSTRUCT_H
#define SCANNERSTRUCT_H
namespace JK {

#define U32	unsigned int
#define U16	unsigned short
#define U8	unsigned char
#define S32 int
#define S16 short
#define	S8  char

#ifndef NULL
#define NULL 0
#endif

//enum{
//    CODE_STA = (('S'&0x000000ff) << 24) | (('T'&0x000000ff) << 16) | (('A'&0x000000ff)<< 8),
//    CODE_RAW = (('R'&0x000000ff) << 24) | (('A'&0x000000ff) << 16) | (('W'&0x000000ff)<< 8),
//    CODE_JPG = (('J'&0x000000ff) << 24) | (('P'&0x000000ff) << 16) | (('G'&0x000000ff)<< 8),
//    CODE_TIF = (('T'&0x000000ff) << 24) | (('I'&0x000000ff) << 16) | (('F'&0x000000ff)<< 8),
//    CODE_BMP = (('B'&0x000000ff) << 24) | (('M'&0x000000ff) << 16) | (('P'&0x000000ff)<< 8),
//    CODE_ADF = (('A'&0x000000ff) << 24) | (('D'&0x000000ff) << 16) | (('F'&0x000000ff)<< 8),
//    CODE_FLB = (('F'&0x000000ff) << 24) | (('L'&0x000000ff) << 16) | (('B'&0x000000ff)<< 8),
//};
enum{
    CODE_STA = 'S' | (('T'&0x000000ff) << 8) | (('A'&0x000000ff)<< 16),
    CODE_RAW = 'R' | (('A'&0x000000ff) << 8) | (('W'&0x000000ff)<< 16),
    CODE_JPG = 'J' | (('P'&0x000000ff) << 8) | (('G'&0x000000ff)<< 16),
    CODE_TIF = 'T' | (('I'&0x000000ff) << 8) | (('F'&0x000000ff)<< 16),
    CODE_BMP = 'B' | (('M'&0x000000ff) << 8) | (('P'&0x000000ff)<< 16),
    CODE_ADF = 'A' | (('D'&0x000000ff) << 8) | (('F'&0x000000ff)<< 16),
    CODE_FLB = 'F' | (('L'&0x000000ff) << 8) | (('B'&0x000000ff)<< 16),
};

// image bit ---------------
#define IMG_1_BIT	1
#define IMG_8_BIT	8
#define IMG_16_BIT	16
#define IMG_24_BIT	24
#define IMG_48_BIT	48

// mono source -------------
#define IMG_COLOR			0
#define IMG_MONO_R			1
#define IMG_MONO_G			2
#define IMG_MONO_B			3
#define IMG_1CH_TRUE_MONO   4
#define IMG_3CH_TRUE_MONO   5
#define IMG_MONO_BW			6
#define IMG_MONO_MIX		7

// image optione -------------------
#define IMG_OPT_THUMB        0x0200
#define IMG_OPT_PAGE         0x0100
#define IMG_OPT_JPG_FMT444   0x0080
#define IMG_OPT_JPG_QUALITY  0x007f

//document size-------------------
#define DOC_SIZE_FULL		0
#define DOC_SIZE_A4			1
#define DOC_SIZE_LT			2
#define DOC_SIZE_LL			3

// image size dots
#define IMG_300_DOT_X		2592
#define IMG_300_DOT_Y		3600

#define IMG_A4_300_DOT_X	2480
#define IMG_A4_300_DOT_Y	3512

#define IMG_LT_300_DOT_X	2536//2552
#define IMG_LT_300_DOT_Y	3296

#define IMG_LL_300_DOT_X	2592//2552
#define IMG_LL_300_DOT_Y	32400//10800

//image size org dots
#define IMG_300_ORG_X		0
#define IMG_300_ORG_Y		0

#define IMG_A4_300_ORG_X		46
#define IMG_A4_300_ORG_Y		0

#define IMG_LT_300_ORG_X		19
#define IMG_LT_300_ORG_Y		0

#define IMG_LL_300_ORG_X		0
#define IMG_LL_300_ORG_Y		0

#define JOB_FLB			'F'
#define JOB_ADF			'A'
#define JOB_ANY			'C'

#define SCAN_FLB		IMG_FMT_FLB
#define SCAN_ADF		IMG_FMT_ADF
#define SCAN_A_SIDE		1
#define SCAN_B_SIDE		2
#define SCAN_AB_SIDE	3

// default scan parameters
#define SCAN_SOURCE   SCAN_ADF   //'FLB', 'ADF'
#define SCAN_ACQUIRE  ACQ_PICK_SS//ACQ_NO_SHADING
#define SCAN_OPTION   0
#define SCAN_DUPLEX   SCAN_AB_SIDE   // 1: A_side, 2: B_side, 3: AB_sides
#define SCAN_PAGE     0   // 0: adf sensor detect
#define IMG_FORMAT    IMG_FMT_JPG//I3('JPG')  //'JPG', 'RAW'
#define IMG_BIT       IMG_24_BIT      // 8, 16, 24, 48
#define IMG_MONO      IMG_COLOR
#define IMG_OPTION    IMG_OPT_JPG_FMT444
#define IMG_DPI_X     300
#define IMG_DPI_Y     300
#define IMG_ORG_X     0
#define IMG_ORG_Y     0
#define IMG_WIDTH     IMG_300_DOT_X   // 8.64"
#define IMG_HEIGHT    IMG_300_DOT_Y   // 12"


/*Motor drive port define*/
#define MT_PH	0
#define BMT_PH	1
#define CMT_PH	2

/*State mechine define*/
#define STATE_MECHINE_0		1
#define STATE_MECHINE_1		2
#define STATE_MECHINE_2		4
#define SCAN_STATE_MECHINE		8

#define MTR_DRIV_TAR	0
#define MTR_STAT_MEC	0
#define MTR_SPEED		0
#define MTR_ACC_STEP	0
#define MTR_DIRECT		0
#define MTR_MICRO_STEP  0
#define MTR_CURRENT		0

//---- MOTOR JOB TASK ----
#define JOB_SCAN				1
#define JOB_CALIBRATION			2
#define JOB_ULTRASONIC			3
#define JOB_FLASH_ACCESS		4

#define JOB_ADF_LOAD_PAPER	1 + 10
#define JOB_ADF_EJECT_PAPER	2 + 10
#define JOB_ADF_RESET_HOME	3 + 10
#define JOB_ADF_MOTOR_TEST	4 + 10
#define JOB_ADF_LIFE_TEST	5 + 10

#define JOB_FLB_LOAD_PAPER	1 + 20
#define JOB_FLB_EJECT_PAPER	2 + 20
#define JOB_FLB_RESET_HOME	3 + 20
#define JOB_FLB_LIFE_TEST	4 + 20

//Scan error code
#define ADF_NOT_READY_ERR		0x01
#define DOC_NOT_READY_ERR		0x02
#define HOME_NOT_READY_ERR		0x03
#define SCAN_JAM_ERR			0x04
#define COVER_OPEN_ERR			0x05

typedef struct SC_JOB_STRUCT {
    U32	code;
    U8	request;
    U8	mode; //ADF = 'A', FLB = 'F'
    U8  reserved;
    U8	id;
} SC_JOB_T;
typedef struct SC_JOB_STA_STRUCT {
    U32	code;
    U8	ack;
    U8	reserved[2];
    union {U8  id; U8 err;};
} SC_JOB_STA_T;

//---------2015/01/19 add

typedef struct SC_STATUS_STRUCT {
    U32	code;
    U8	request;
    U8  reserved[2];
    U8	id;
} SC_STATUS_T;

typedef struct SC_STATUS_DATA_STRUCT {
    U8 system;
    U8 sensor;
    U8 error;
} SC_STATUS_DATA_T;

//---------2015/01/19 end
//------------------------------
typedef struct SC_PAR_STRUCT {
    U32	code;
    U16	length;
    U8  reserved;
    U8	id;
} SC_PAR_T;
typedef struct MTR_STRUCT {
    U8 drive_target;
    U8 state_mechine;
    U8 direction;
    U8 micro_step;
    U8 currentLV;
    U8 padding1[3];
    U16 speed_pps;
    U16 acc_step;
    U16 pick_ss_step;
    U16 padding2;
} MTR_T;
typedef struct IMAGE_STRUCT {
    U32 format;	// 'RAW', 'JPG', 'TIF', 'BMP', 'PDF', 'PNG'
    U16	option;	// 0
    U8	bit;	// 1:BW, 8:Gray8, 16:Gray16, 24:Color24, 48:Color48
    U8	mono;	// 0:'MONO', 1:'R', 2:'G', 4:'B', 8:'IR', 7:'NTSC'
    struct{U16 x; U16 y;} dpi;
    struct{U32 x; U32 y;} org;
    U32 width;
    U32 height;
} IMAGE_T;
typedef struct SC_PAR_DATA_STRUCT {
    //- ACQUIRE --
    U32 source;	// 'ADF'/'FLB'/'POS'/'NEG', 'FW'/'PAR'/'HW', 'HOST', 'WIFI', 'ETH'
    U32 acquire;//bit1: Prefeed enable bit2: 0:white 1:dark  scan bit3: 0:normal scan 1:stillmode scan  bit4: 0:continue calibration  1:calibration over autogohome = 1
    U16 acq_opt;// 0
    U8	duplex;	// 1:'A', 2:'B', 3:'D'
    U8	page;	// 0 for infinity pages
    //- IMAGE ----
    //U32 format;	// 'RAW', 'JPG', 'TIF', 'BMP', 'PDF', 'PNG'
    //U16	img_opt;// 0
    //U8	bit;	// 1:BW, 8:Gray8, 16:Gray16, 24:Color24, 48:Color48
    //U8	mono;	// 0:'MONO', 1:'R', 2:'G', 4:'B', 8:'IR', 7:'NTSC'
    //struct{U16 x; U16 y;} dpi;
    //struct{U32 x; U32 y;} org;
    //struct{U32 w; U32 h;} dot;
    IMAGE_T  img;
    MTR_T   mtr[2];

    U16 leading_edge;
    U16 img_gap;
    U16 prefed;
    U16 postfed;
    U16 side_edgeA;
    U16 side_edgeB;

    U16 unknow1;
    U16 unknow2;
    //- shading ---
//	U16	AFE_OffsetCode[2][3];
//	U16	AFE_GainCode[2][3];
//	float AFE_OffsetValue[2][3];
//	float AFE_GainValue[2][3];
//	U32 shading_size;
} SC_PAR_DATA_T;

//#define ACQ_AUTO_SCAN		0x01
//#define ACQ_SHADING			0x02
//#define ACQ_GAMMA			0x04
//#define ACQ_MIRROR			0x80
//#define ACQ_START_HOME		(0x01 << 8)
//#define ACQ_BACK_TRACK		(0x02 << 8)
//#define ACQ_AUTO_GO_HOME	(0x04 << 8)
//#define ACQ_EJECT_PAPER		(0x10 << 8)
//#define ACQ_PICKUP_HOME		(0x20 << 8)
//
//#define ACQ_STILL_SCAN		(0x01 << 16)
//#define ACQ_TEST_PATTERN	(0x02 << 16)
//#define ACQ_RUNIN_IMAGE		(0x04 << 16)
//#define ACQ_RUNIN			(0x08 << 16)

//#define ACQ_SHADING			 0x01
//#define ACQ_GAMMA			(0x01 << 1)
//#define ACQ_MIRROR			(0x01 << 2)
//#define ACQ_LAMP_OFF		(0x01 << 3)
//#define ACQ_START_HOME		(0x01 << 4)
//#define ACQ_BACK_TRACK_OFF	(0x01 << 5)
//#define ACQ_AUTO_GO_HOME	(0x01 << 6)
//#define ACQ_STILL_SCAN		(0x01 << 7)
//#define ACQ_STARTSTOP_TEST	(0x01 << 8)
//#define ACQ_WRITE_FLASH		(0x01 << 9)  //only for calibration to use
//#define ACQ_RODLENS			(0x01 << 10) //for RodLens scan use
//#define ACQ_DUSTBKG			(0x01 << 11) //for Dust bkg Scan, don't need to pass to fw.

#define ACQ_PAGE_READ		(0x01)
#define ACQ_NO_MIRROR		(0x02)
#define ACQ_NO_SHADING		(0x04)
#define ACQ_BACK_SCAN		(0x08)
#define ACQ_Ultra_Sonic		(0x01 << 5)
#define ACQ_CROP_DESKEW		(0x01 << 8)
#define ACQ_PAGE_FILL		(0x02 << 8)
#define ACQ_LEFT_ALIGN		(0x04 << 8)
#define ACQ_AUTO_COLOR		(0x08 << 8)
#define ACQ_AUTO_LEVEL		(0x10 << 8)
#define ACQ_DETECT_COLOR	(0x20 << 8)
#define ACQ_DETECT_BW		(0x40 << 8)

#define ACQ_MOTOR_OFF		(0x01 << 16)    // scan without moving motor
#define ACQ_NO_PP_SENSOR	(0x02 << 16)    // ADF scan without Doc/ADF sensor detection, Flatbed scan without home sensor detection
#define ACQ_LAMP_OFF		(0x04 << 16)
#define ACQ_TEST_PATTERN	(0x08 << 16)    // test pattern image
#define ACQ_PSEUDO_SENSOR	(0x10<< 16)
#define ACQ_CALIBRATION		(0x20 << 16)  //Park test for calibration
#define ACQ_SET_MTR			(0x40 << 16)  //Park test for set motor par from console
#define ACQ_LIFE_TEST		(0x80 << 16)  //Park life test

#define ACQ_PICK_SS			(0x02 << 24)  //Park pick ss test

typedef struct SC_PAR_STA_STRUCT {
    U32	code;
    U8	ack;
    U8	reserved[2];
    union {U8  id; U8 err;};
} SC_PAR_STA_T;
//---------------------------------
typedef struct SC_SCAN_STRUCT {
    U32	code;
    U8  reserved[3];
    U8	id;
} SC_SCAN_T;
typedef struct SC_SCAN_STA_STRUCT {
    U32	code;
    U8	ack;
    U8	reserved[2];
    union {U8  id; U8 err;};
} SC_SCAN_STA_T;
//--------------------------------
typedef struct SC_INFO_STRUCT {
    U32	code;
    U8	length;
    U8  reserved[2];
    U8	id;
} SC_INFO_T;
//typedef struct SC_INFO_DATA_STRUCT {
//	U32	code;
//	U16 ValidPage[2];
//	U32 ValidPageSize[2];
//	U16 ImageWidth[2];
//	U16 ImageLength[2];
//	U8	PageNo[2];
//	U8	EndPage[2];
//	U8	EndDocument;
//	U8	Cancel;
//	U8	Error;
//	U8	reserved;
///*	U32	code;
//	U16 ValidPage[2];
//	U32 ValidPageSize[2];
//	U16 ImageWidth[2];
//	U16 ImageLength[2];
//	U8	PageNo[2];
//	U8	EndPage[2];
//	U8	UltraSonic; U8 PaperJam; U8 CoverOpen; U8 Cancel; U8 key;
//	U8	EndDocument;
//	U8	reserved[14+16];*/
//} SC_INFO_DATA_T;

typedef struct SC_INFO_DATA_STRUCT {
    U32 code;        // 4
    U16 PageNum[2];      // 4
    U32 ValidPageSize[2];  // 8
    U16 ImageWidth[2];    // 4
    U16 ImageHeight[2];    // 4
    U8  EndPage[2];      // 4
    U8  EndScan[2];
    U8  UltraSonic;      // 4
    U8  PaperJam;
    U8  CoverOpen;
    U8  Cancel;
    U8  key;        // 4
    U8  MotorMove;
    U8  AdfSensor;
    U8  DocSensor;
    U8  HomeSensor;      // 4
    U8  JobOwner;
    U8  reserve1[2];
    U8  reserve2[4];    // 4
    U32 JobState;      // 4
} SC_INFO_DATA_T;

/*sensor parameter*/
typedef struct SENSOR_CHK_STRUCT {
    struct {
        U32 doc : 1;		//DOC
        U32 scan : 1;		//SCAN
        U32 cover : 1;	//COVER
        U32 home : 1;		//HOME
        U32 deskew : 1;	//DESKEW
        U32 lid : 1;		//LID
        U32 x1 : 1;		//PAPER_X_SIZE_ENCODE1
        U32 x2 : 1;		//PAPER_X_SIZE_ENCODE2
        U32 x3 : 1;		//PAPER_X_SIZE_ENCODE3
        U32 y1 : 1;		//PAPER_Y_SIZE_1
        U32 y2 : 1;		//PAPER_Y_SIZE_2
        U32 y3 : 1;		//PAPER_Y_SIZE_3
        U32 reserved : 20;
    } val;
}SENSOR_CHK_T;

//-----------------------------
typedef struct SC_IMG_STRUCT {
    U32	code;
    U32 length: 24;
    U32 side:	8;
    //U8	length[3];
    //U8	side;
} SC_IMG_T;
typedef struct SC_IMG_STA_STRUCT {
    U32	code;
    U32 ack: 8;
    U32 length: 24;
    //U8	ack;
    //U8	length[2];
    //U8	err;
} SC_IMG_STA_T;
#define LENGTH
//-----------------------------
typedef struct SC_CNL_STRUCT {
    U32	code;
    U8	reserved[3];
    U8  id;
} SC_CNL_T;
typedef struct SC_CNL_STA_STRUCT {
    U32	code;
    U8	ack;
    U8	reserved[2];
    union {U8  id; U8 err;};
} SC_CNL_STA_T;
//-----------------------------
typedef struct SC_STOP_STRUCT {
    U32 code;
    U8	reserved[3];
    U8	id;
}SC_STOP_T;
typedef struct SC_STOP_STA_STRUCT {
    U32 code;
    U8	ack;
    U8	reserved[2];
    union {U8  id; U8 err;};
}SC_STOP_STA_T;
//-----------------------------
#define OPT_STILL_IMG	0x0
// 0: 'SCAN', 'STIL': Still scan, 'MOVE': motor Move only, 'CALI': Calibration scan
// request
// type (b31~b24):		0=image, 1=still-image, 2=motor moving, 3=FW update, 4=table update
// request(b23~b16):	ShadingTable, bit22: Gamma, GammaTable, bit21: contrast, bit20: brightness,
//						Auto_Level, Auto_Color,
//------------------------------
typedef struct SC_AFE_STRUCT {
    U32	code;		//'AFE'
    U16	length;		// 36
    U8  zero[2];	// 0, 0
} SC_AFE_T;
typedef struct SC_AFE_DATA_STRUCT {
    U16 GainCode[2][3];
    U16 OffsetCode[2][3];
    U16 ShutterPixel[2][3];
} SC_AFE_DATA_T;

typedef struct SC_AFE_STA_STRUCT {
    U32	code;		// 'STA'
    U8	ack;		// 'A' or 'E'
    U8	zero[3];	// 0, 0, 0
} SC_AFE_STA_T;
//---------------------------------
typedef struct SC_SHAD_STRUCT {
    U32	code;		//'SHAD'
    U32 type: 8;	// 0:(default), 16:(16+16), 12:(12+4), 10:(10+6), 8:(8+8)
    U32 length: 24;	// 3 channel shading data size
} SC_SHAD_T;
typedef struct SC_SHAD_STA_STRUCT {
    U32	code;		// 'STA'
    U8	ack;		// 'A' or 'E'
    U8	zero[3];	// 0, 0, 0
} SC_SHAD_STA_T;
//------------------------------
typedef struct SC_MOTO_STRUCT {
    U32	code;		//'MOTO'
    U8	StepType;	// 0:(Full Step), 1:(Half Step), 2:(Quarter Step), 3:(1/8 step), 4:(1/16 step)
    U8	CurrentSel;	// 0, 1, 2, 3
    U8  Dir;		// 1: forward, 2: backward, 3: forward and backward
    U8	MotorID;	// 'F': flatbed, 'A': ADF, 'P': pickup
} SC_MOTO_T;
typedef struct SC_MOTO_DATA_STRUCT {
    U32 forward_step;	U32 backward_step;
} SC_MOTO_DATA_T;

typedef struct SC_MOTO_STA_STRUCT {
    U32	code;		// 'STA'
    U8	ack;		// 'A' or 'E'
    U8	zero[3];	// 0, 0, 0
} SC_MOTO_STA_T;
//---------------------------------

typedef struct REGBIT_STRUCT {
    U32 addr; U32 clear; U32 set;
} REGBIT_T;

typedef struct AFEREG_STRUCT {
    U16 addr; U16 data;
} AFEREG_T;

typedef struct NVRW_STRUCT {
    U32	code;  //'NVR', 'NVW'
    U8	straddr;
    U8	length;
    U8	reserved[2];
} S_NVRW;

typedef struct NVW_data_STRUCT {
    U8  *data;
} S_NVW_data;


typedef struct NVR_data_STRUCT {//can be change format, total size, etc...
    U8	*data;
} S_NVR_data;


typedef struct NVRW_status_STRUCT {
    U32	code;
    U8	ack;
    U8	reserved[2];
    union {U8  id; U8 err;};
} S_NVRW_status;
typedef struct SC_ADF_CHECK_STRUCT {
    U32	code;
    U8	request;
    U8  reserved[2];
    U8	id;
} SC_ADF_CHECK_T;

typedef struct SC_ADF_CHECK_STA_STRUCT {
    U32	code;
    U8	ack;
    U8	reserved[2];
    union {U8  id; U8 err;};
} SC_ADF_CHECK_STA_T;
typedef struct fw_version_ack_STRUCT {
    U32	code;
    U8	reserved[4];
} fw_version_ack;


typedef struct fw_version_get_STRUCT {
    U32	version;
    U8  check;
    U8  length;
    U8	reserved[2];
} fw_version_get;


}
#endif // SCANNERSTRUCT_H
