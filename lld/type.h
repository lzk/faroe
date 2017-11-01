#ifndef TYPE_H
#define TYPE_H

namespace JK {
//#ifndef UINT32
//#define UINT32	unsigned int
//#define UINT16	unsigned short
//#define UINT8	unsigned char
//#endif
#define U32	unsigned int
#define U16	unsigned short
#define U8	unsigned char
#define S32 int
#define S16 short
#define	S8  char

#define	I4(i) ((((i) & 0xff)<<24)+(((i) & 0xff00)<<8)+(((i) & 0xff0000)>>8)+(((i) & 0xff000000)>>24))
#define I3(i) ((((i) & 0xff)<<16)+(((i) & 0xff00))+(((i) & 0xff0000)>>16))
#define	I2(i) ((((i) & 0xff)<<8)+(((i) & 0xff00)>>8))
#define	I1(i) ((i) & 0xff)
#ifndef NULL
#define NULL 0
#endif
}
#endif // TYPE_H
