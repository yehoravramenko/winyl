IF NOT EXIST build\ (mkdir build\)
IF NOT EXIST build\obj (mkdir build\obj)

cd /d build

cl /c ..\src\winyl.cpp /Fo: obj\winyl.obj /std:c++17 /WX /Zi

@REM link build\obj\winyl.obj user32.lib shell32.lib gdi32.lib /OUT:build\winlofi.exe /DEBUG:FULL
link obj\winyl.obj /OUT:winyl.exe /DEBUG:FULL