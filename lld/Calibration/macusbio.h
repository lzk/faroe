#ifndef MACUSBIO_H
#define MACUSBIO_H

int CMDIO_OpenDevice();
int CMDIO_CloseDevice();
int CMDIO_BulkWriteEx(int pipe, void *buffer, unsigned int dwLen);
int CMDIO_BulkReadEx(int pipe, void *buffer, unsigned int dwLen);

#endif // MACUSBIO_H
