#include "deviceio.h"

#include <unistd.h>
#define delay100ms(x ,y) {if(y) usleep((x)  * 100 * 1000);}

using namespace JK;

DeviceIO::DeviceIO()
{

}

DeviceIO::~DeviceIO()
{
}

