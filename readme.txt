1—qtftp
1. git clone qtftp.git
2. Open qtftp/qtftp.pro 
3. shadow build dir: build/Release
4. build release
5. Close project

2-newui Qt project
1. Open newui/newui.pro
2. Debug 
   shadow build dir: ../build/debug
   qmake (default)
   make (default)
   cp -rf ../../mac/Resources/* faroeVOP.app/Contents/Resources
   macdeployqt faroeVOP.app -qmldir=${QTDIR}/qml


3. Release - only make Xcode project
   shadow build dir: ../build/release
   qmake -spec macx-xcode %{sourceDir}/newui.pro  (%{buildDir})
   cp -rf ../../mac/* .   (%{buildDir})

4. Build Release to get Xcode project

Xcode build
1. Open build/release/faroeVOP.xcodeproj
2. Config 
   1.iCloud Mac develop/distribute
   2.signing add other code signing Flags  “-—deep”
   3.add script
/Volumes/work/software/Qt/latest/clang_64/bin/macdeployqt ${CODESIGNING_FOLDER_PATH} -qmldir=/Volumes/work/software/Qt/latest/clang_64/qml
/Users/aimee/Qt5.9.6/latest/clang_64/bin/macdeployqt ${CODESIGNING_FOLDER_PATH} -qmldir=/Users/aimee/Qt5.9.6/latest/clang_64/qml
   4.add script
mkdir -p ${CODESIGNING_FOLDER_PATH}/Contents/Resources/ && cp -rf Resources/* ${CODESIGNING_FOLDER_PATH}/Contents/Resources/
   


//6.add script
cp -rf /Volumes/work/software/Qt5.9.4/5.9.4/clang_64/qml/QtGraphicalEffects ${CODESIGNING_FOLDER_PATH}/Contents/Resources/qml/