1. git clone qtftp.git
2. Open qtftp/qtftp.pro and build release
3. Open newui/newui.pro
4. Build Release
5. Open build/mac/faroeVOP.xcodeproj
6. Config 
   1.iCloud Mac develop/distribute
   2.signing add other code signing Flags  “-—deep”
   3.copy mac/faroeVOP.icns to Resource
   4.copy mac/toemail to Resource
   5.add script
/Applications/Qt5.9.5/5.9.5/clang_64/bin/macdeployqt ${CODESIGNING_FOLDER_PATH} -qmldir=/Applications/Qt5.9.5/5.9.5/clang_64/qml
   


//6.add script
cp -rf /Volumes/work/software/Qt5.9.4/5.9.4/clang_64/qml/QtGraphicalEffects ${CODESIGNING_FOLDER_PATH}/Contents/Resources/qml/