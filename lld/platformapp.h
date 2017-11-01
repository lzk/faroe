#ifndef PLATFROMAPP_H
#define PLATFROMAPP_H
namespace JK {

class PlatformApp
{
public:
    PlatformApp();
    virtual void saveImage(char* filename ,unsigned char* buffer
                   ,int width ,int height ,int bitPerPixel);
    virtual void updateProgress(float progrress);
private:
};

}
#endif // PLATFROMAPP_H
