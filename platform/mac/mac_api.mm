
#import <Carbon/Carbon.h>
#include <QString>
void saveMultiPageTiffImage(const QString& _tmpPath ,const QString& _imageFilePath)
//void saveMultiPageTiffImage(NSString* tmpPath ,NSString* imageFilePath)
{
    NSString* tmpPath = _tmpPath.toNSString();
    NSString* imageFilePath = _imageFilePath.toNSString();
    NSString *cmd;
    if ([[NSFileManager defaultManager] fileExistsAtPath:imageFilePath isDirectory:nil]) {
        cmd = [NSString stringWithFormat:@"tiffutil -cat \"%@\" \"%@\" -out \"%@\"" ,imageFilePath ,tmpPath, imageFilePath];
    }else{
        cmd = [NSString stringWithFormat:@"tiffutil -cat \"%@\" -out \"%@\"", tmpPath ,imageFilePath];
    }

    system([cmd cStringUsingEncoding:NSUTF8StringEncoding]);
    NSLog(@"cmd: %@" ,cmd);
}

#include <QStringList>
#include <QDir>
#include <QCoreApplication>
void openEmail(const QString& subject ,const QString& recipient ,const QStringList& attachment)
{
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cdUp();
    dir.cd("Resources");
    QString toEmail = dir.absolutePath() + "/toemail";
    NSTask *task = nil;
    NSMutableArray *a = [NSMutableArray arrayWithObjects:
            toEmail.toNSString()
            ,subject.toNSString()
            ,recipient.toNSString()
            ,nil];
    foreach (QString attach, attachment) {
        [a addObject: attach.toNSString()];
    }
    task = [NSTask launchedTaskWithLaunchPath:@"/usr/bin/osascript"
                                    arguments:a];
}

void openApplication(const QString& appName ,const QStringList& fileList)
{
    NSTask *task = nil;
    NSMutableArray *a = [NSMutableArray arrayWithObjects:
            @"-a"
            ,appName.toNSString()
            ,nil];
    foreach (QString filename, fileList) {
        [a addObject: filename.toNSString()];
    }
    task = [NSTask launchedTaskWithLaunchPath:@"/usr/bin/open"
                                    arguments:a];
}

NSURL *rootUrl;
bool iCloudCheckLogin()
{
    rootUrl = [[NSFileManager defaultManager] URLForUbiquityContainerIdentifier:nil];

    if (rootUrl){
        rootUrl = [rootUrl URLByAppendingPathComponent:@"Documents"];
        NSLog(@"iCloud is sign in,documentUrl:%@",rootUrl);
        return true;
    }else{
        NSLog(@"iCloud is  sign out");
    }
    return false;
}

#include <QFileInfo>
#include <QDebug>
#include <QFile>
bool iCloudUpload(const QString& fileName)
{
    if(!rootUrl)
        return false;
    QFileInfo fileInfo(fileName);
    NSString* nsFileName = fileInfo.fileName().toNSString();
    QString couldFileName = fileInfo.absolutePath() + "/toiCloud_" + fileInfo.fileName();
    QFile::copy(fileName, couldFileName);
    NSString* nsFilePath = couldFileName.toNSString();
//    NSString* nsFilePath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
//    nsFilePath = [nsFilePath stringByAppendingPathComponent:nsFileName];
//    QFile::copy(fileName, QString::fromNSString(nsFilePath));
    
    NSLog(@"ns filepath:%@ ,filename:%@" ,nsFilePath ,nsFileName);
    NSURL *iCloudUrl1 = [NSURL URLWithString:nsFileName relativeToURL:rootUrl];

    NSFileManager *manager = [NSFileManager defaultManager];
    if ([manager fileExistsAtPath:nsFilePath])
    {
        NSError *error = nil;
        if ([manager isUbiquitousItemAtURL:iCloudUrl1]){
            NSData *data = [NSData dataWithContentsOfFile:nsFilePath];
            [data writeToURL:iCloudUrl1 options:NSDataWritingAtomic error:&error];
            dispatch_async(dispatch_get_main_queue(), ^{
                NSLog(@"1%@",error);
            });
        } else{
            NSURL *fileUrl = [NSURL fileURLWithPath:nsFilePath];
            [manager setUbiquitous:YES itemAtURL:fileUrl destinationURL:iCloudUrl1 error:&error];

            NSLog(@"ns iCloudUrl1:%@ ,fileUrl:%@" ,iCloudUrl1 ,fileUrl);
            dispatch_sync(dispatch_get_main_queue(), ^{
                NSLog(@"2%@",error);
            });
        }
        if(!error){
            NSLog(@"file1 upload success");
            return true;
        }
    } else
    {
        NSLog(@"file1 not exsit");
    }
    return false;

}

