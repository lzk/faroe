1. git clone qtftp.git
2. Open qtftp/qtftp.pro and build release
3. Open newui/newui.pro
4. Build new
5. Open build/mac/faroeVOP.xcodeproj
6. Config 
   1.iCloud Mac develop/distribute
   2.copy faroeVOP.icns to Resource
   3.copy toemail to Resource
   4.add script
/Volumes/work/software/Qt5.9.1/5.9.1/clang_64/bin/macdeployqt ${CODESIGNING_FOLDER_PATH} -qmldir=/Volumes/work/software/Qt5.9.1/5.9.1/clang_64/qml
   5.add script
cp -rf /Volumes/work/software/Qt5.9.1/5.9.1/clang_64/qml/QtGraphicalEffects ${CODESIGNING_FOLDER_PATH}/Contents/Resources/qml/