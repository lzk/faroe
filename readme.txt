1—qtftp
1. git clone qtftp.git
2. Open qtftp/qtftp.pro and build release

2-newui Qt project
1. Open newui/newui.pro
2. Debug 
   shadow build dir: ../build/debug
   qmake (default)
   make (default)
   cp ../../mac/tomail faroeVOP.app/Contoents/Resources
   macdeployqt faroeVOP.app -qmldir=${QTDIR}/qml


3. Release - only make Xcode project
   shadow build dir: ../build/mac
   qmake -spec macx-xcode %{sourceDir}/newui.pro.  (%{buildDir})
   cp -rf ../../mac/* .   (%{buildDir})

4. Build Release to get Xcode project

Xcode build
1. Open build/mac/faroeVOP.xcodeproj
2. Config 
   1.iCloud Mac develop/distribute
   2.signing add other code signing Flags  “-—deep”
   3.copy mac/faroeVOP.icns to Resource
   4.copy mac/toemail to Resource
   5.add script
/Applications/Qt5.9.5/5.9.5/clang_64/bin/macdeployqt ${CODESIGNING_FOLDER_PATH} -qmldir=/Applications/Qt5.9.5/5.9.5/clang_64/qml
   


//6.add script
cp -rf /Volumes/work/software/Qt5.9.4/5.9.4/clang_64/qml/QtGraphicalEffects ${CODESIGNING_FOLDER_PATH}/Contents/Resources/qml/