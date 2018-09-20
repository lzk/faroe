
#include <QDebug>
#import <Carbon/Carbon.h>
#include <QString>
void saveMultiPageTiffImage(const QString& _tmpPath ,const QString& _imageFilePath)
//void saveMultiPageTiffImage(NSString* tmpPath ,NSString* imageFilePath)
{
    NSString* tmpPath = _tmpPath.toNSString();
    NSString* imageFilePath = _imageFilePath.toNSString();
    imageFilePath = [imageFilePath stringByReplacingOccurrencesOfString:@"$" withString:@"\\$"];
    NSLog(@"image file path:%@" ,imageFilePath);
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

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
NSURL *rootUrl;
bool iCloudCheckLogin(QString& para)
{
    bool isLogin = false;
    rootUrl = [[NSFileManager defaultManager] URLForUbiquityContainerIdentifier:nil];

    if (rootUrl){
        rootUrl = [rootUrl URLByAppendingPathComponent:@"Documents"];
        NSLog(@"iCloud is sign in,documentUrl:%@",rootUrl);
        isLogin = true;
    }else{
        NSLog(@"iCloud is  sign out");
    }
    QJsonObject jsonObj = QJsonDocument::fromJson(para.toUtf8()).object();
    jsonObj.insert("isLogin" ,isLogin);
    para = QString(QJsonDocument(jsonObj).toJson());
    return isLogin;
}


bool iCloudGetFileList(QString& para)
{
    bool success = true;
    if(!rootUrl)
        return false;
    NSFileManager *manager = [NSFileManager defaultManager];
    NSArray* array = [manager contentsOfDirectoryAtURL:rootUrl includingPropertiesForKeys:nil options:NSDirectoryEnumerationSkipsHiddenFiles  error:nil];
    if(!array){
        return false;
    }
    
    QJsonObject jsonObj = QJsonDocument::fromJson(para.toUtf8()).object();
    QJsonArray jarray;
    NSMetadataItem *item;
    NSString *fileName;
    long long fileSize;

    for (int i = 0; i<(int)array.count; i++) {
        QJsonObject obj;
        id url = [array objectAtIndex:i];
        item = [[NSMetadataItem alloc] initWithURL:url];
        fileName = [item valueForAttribute:NSMetadataItemFSNameKey];
        fileSize = [[item valueForAttribute:NSMetadataItemFSSizeKey] longLongValue];
//        NSLog(@"item attribute NSMetadataUbiquitousItemIsUploadedKey is:%@" ,[item valueForAttribute:NSMetadataUbiquitousItemIsUploadedKey] );
//        NSLog(@"item attribute NSMetadataUbiquitousItemIsUploadingKey is:%@" ,[item valueForAttribute:NSMetadataUbiquitousItemIsUploadingKey]);
//        bool isUploaded = [[item valueForAttribute:NSMetadataUbiquitousItemIsUploadedKey] boolValue];
//        bool isUploading = [[item valueForAttribute:NSMetadataUbiquitousItemIsUploadingKey] boolValue];
//        qDebug()<<"isUploaded"<<isUploaded;
//        qDebug()<<"isUploading"<<isUploading;
        if(fileSize < 1){
            success = false;
        }else{
            obj.insert("imageUrl", "qrc:/Images/file.png");
            obj.insert("fileName", QString::fromNSString(fileName));
            obj.insert("fileSize", fileSize);
//        BOOL isDirectory;
//        if([manager fileExistsAtPath:url isDirectory:&isDirectory] && isDirectory){
//            obj.insert("imageUrl", "qrc:/Images/Folder-icon.png");
//        }else{
//            obj.insert("imageUrl", "qrc:/Images/file.png");
//        }
//        obj.insert("fileName", QString::fromNSString([manager displayNameAtPath:url]));
            jarray << obj;
        }
    }
    jsonObj.insert("fileList" ,jarray);
    para = QString(QJsonDocument(jsonObj).toJson());
    return success;
}

#include <QFileInfo>
#include <QFile>

bool iCouldIsExist(const QString& fileName)
{
    if(!rootUrl)
        return false;
    QFileInfo fileInfo(fileName);
    NSString* nsFileName = fileInfo.fileName().toNSString();
    NSURL *iCloudUrl1 = [NSURL URLWithString:nsFileName relativeToURL:rootUrl];
    NSFileManager *manager = [NSFileManager defaultManager];
    bool isExist = [manager isUbiquitousItemAtURL:iCloudUrl1];
    NSLog(@"ns filename:%@ " ,nsFileName);
    qDebug()<<"is exist:" <<isExist;
    return isExist;
}

bool iCloudUpload(const QString& fileName)
{
    if(!rootUrl)
        return false;
    QFileInfo fileInfo(fileName);
    NSString* nsFileName = fileInfo.fileName().toNSString();
    QString couldFileName = fileInfo.absolutePath() + "/tmpfiletoiCloud." + fileInfo.suffix();
//    QString couldFileName = fileInfo.absolutePath() + "/toiCloud_" + fileInfo.fileName();
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
//            [manager moveItemAtURL:fileUrl toURL:iCloudUrl1 error:&error];
//            NSData *data = [NSData dataWithContentsOfFile:nsFilePath];
//            [data writeToURL:iCloudUrl1 options:NSDataWritingAtomic error:&error];
            
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

#include <AppKit/AppKit.h>
#include <QWindow>
void windowShowMinimize(QWindow* window)
{
    NSView* view = (NSView*)window->winId();

    NSWindow* wnd = [view window];

    [wnd miniaturize:nil];
}

void windowSetWindowFrameless(QWindow* window)
{
    NSView* view = (NSView*)window->winId();

    NSWindow* wndd = [view window];

    wndd.titlebarAppearsTransparent = YES;

    wndd.titleVisibility = NSWindowTitleHidden;

    wndd.styleMask |= NSFullSizeContentViewWindowMask;

    [[wndd standardWindowButton:NSWindowZoomButton] setHidden:YES];

    [[wndd standardWindowButton:NSWindowMiniaturizeButton] setHidden:YES];

    [[wndd standardWindowButton:NSWindowCloseButton] setHidden:YES];
}
