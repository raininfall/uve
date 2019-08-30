CCACHE=ccache
CC=cl.exe
MRC=D:\Windows Kits\10\bin\10.0.17763.0\x64\rc.exe
CXX=cl.exe
GC=go
AS=ml64.exe

SH=link.exe -dll
LD=link.exe
GC_LD=go
AR=link.exe -lib
GC_AR=go

uve_CC=-Ox -fp:fast -Iinclude -IC:\\\\Users\\\\lygjc\\\\AppData\\\\Roaming\\\\.xmake\\\\packages\\\\l\\\\libuv\\\\v1.31.0\\\\e9943905ed82ee1333489c077b05a482\\\\include /MT -nologo
uve_AR=-nologo -machine:x64
test-uve_CXX=-Ox -fp:fast -Iinclude -IC:\\\\Users\\\\lygjc\\\\AppData\\\\Roaming\\\\.xmake\\\\packages\\\\g\\\\gtest\\\\1.8.1\\\\e9943905ed82ee1333489c077b05a482\\\\include -IC:\\\\Users\\\\lygjc\\\\AppData\\\\Roaming\\\\.xmake\\\\packages\\\\l\\\\libuv\\\\v1.31.0\\\\e9943905ed82ee1333489c077b05a482\\\\include /MT -nologo
test-uve_LD=-libpath:build\\\\windows\\\\x64\\\\release -libpath:C:\\\\Users\\\\lygjc\\\\AppData\\\\Roaming\\\\.xmake\\\\packages\\\\l\\\\libuv\\\\v1.31.0\\\\e9943905ed82ee1333489c077b05a482\\\\lib -libpath:C:\\\\Users\\\\lygjc\\\\AppData\\\\Roaming\\\\.xmake\\\\packages\\\\g\\\\gtest\\\\1.8.1\\\\e9943905ed82ee1333489c077b05a482\\\\lib uve.lib uv_a.lib gmock.lib gmock_main.lib gtest.lib gtest_main.lib advapi32.lib iphlpapi.lib psapi.lib user32.lib userenv.lib ws2_32.lib kernel32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib -nologo -dynamicbase -nxcompat -machine:x64

default:  uve test-uve

all:  uve test-uve

.PHONY: default all  uve test-uve

uve: build\windows\x64\release\uve.lib
build\windows\x64\release\uve.lib: build\.objs\uve\windows\x64\release\src\request.c.obj build\.objs\uve\windows\x64\release\src\buffer.c.obj build\.objs\uve\windows\x64\release\src\bufio.c.obj
	@echo linking.release uve.lib
	-@mkdir build\windows\x64\release > NUL 2>&1
	@$(AR) $(uve_AR) -out:build\\windows\\x64\\release\\uve.lib build\\.objs\\uve\\windows\\x64\\release\\src\\request.c.obj build\\.objs\\uve\\windows\\x64\\release\\src\\buffer.c.obj build\\.objs\\uve\\windows\\x64\\release\\src\\bufio.c.obj > build/.build.log 2>&1

build\.objs\uve\windows\x64\release\src\request.c.obj: src\request.c
	@echo compiling.release src\request.c
	-@mkdir build\.objs\uve\windows\x64\release\src > NUL 2>&1
	@$(CC) -c $(uve_CC) -Fobuild\\.objs\\uve\\windows\\x64\\release\\src\\request.c.obj src\\request.c > build/.build.log 2>&1

build\.objs\uve\windows\x64\release\src\buffer.c.obj: src\buffer.c
	@echo compiling.release src\buffer.c
	-@mkdir build\.objs\uve\windows\x64\release\src > NUL 2>&1
	@$(CC) -c $(uve_CC) -Fobuild\\.objs\\uve\\windows\\x64\\release\\src\\buffer.c.obj src\\buffer.c > build/.build.log 2>&1

build\.objs\uve\windows\x64\release\src\bufio.c.obj: src\bufio.c
	@echo compiling.release src\bufio.c
	-@mkdir build\.objs\uve\windows\x64\release\src > NUL 2>&1
	@$(CC) -c $(uve_CC) -Fobuild\\.objs\\uve\\windows\\x64\\release\\src\\bufio.c.obj src\\bufio.c > build/.build.log 2>&1

test-uve: build\windows\x64\release\test-uve.exe
build\windows\x64\release\test-uve.exe: build\windows\x64\release\uve.lib build\.objs\test-uve\windows\x64\release\test\main.cpp.obj build\.objs\test-uve\windows\x64\release\test\test_bufio_peek.cpp.obj build\.objs\test-uve\windows\x64\release\test\test_bufio_peek_not_enough.cpp.obj build\.objs\test-uve\windows\x64\release\test\test_bufio_peek_twice.cpp.obj
	@echo linking.release test-uve.exe
	-@mkdir build\windows\x64\release > NUL 2>&1
	@$(LD) $(test-uve_LD) -out:build\\windows\\x64\\release\\test-uve.exe build\\.objs\\test-uve\\windows\\x64\\release\\test\\main.cpp.obj build\\.objs\\test-uve\\windows\\x64\\release\\test\\test_bufio_peek.cpp.obj build\\.objs\\test-uve\\windows\\x64\\release\\test\\test_bufio_peek_not_enough.cpp.obj build\\.objs\\test-uve\\windows\\x64\\release\\test\\test_bufio_peek_twice.cpp.obj > build/.build.log 2>&1

build\.objs\test-uve\windows\x64\release\test\main.cpp.obj: test\main.cpp
	@echo compiling.release test\main.cpp
	-@mkdir build\.objs\test-uve\windows\x64\release\test > NUL 2>&1
	@$(CXX) -c $(test-uve_CXX) -Fobuild\\.objs\\test-uve\\windows\\x64\\release\\test\\main.cpp.obj test\\main.cpp > build/.build.log 2>&1

build\.objs\test-uve\windows\x64\release\test\test_bufio_peek.cpp.obj: test\test_bufio_peek.cpp
	@echo compiling.release test\test_bufio_peek.cpp
	-@mkdir build\.objs\test-uve\windows\x64\release\test > NUL 2>&1
	@$(CXX) -c $(test-uve_CXX) -Fobuild\\.objs\\test-uve\\windows\\x64\\release\\test\\test_bufio_peek.cpp.obj test\\test_bufio_peek.cpp > build/.build.log 2>&1

build\.objs\test-uve\windows\x64\release\test\test_bufio_peek_not_enough.cpp.obj: test\test_bufio_peek_not_enough.cpp
	@echo compiling.release test\test_bufio_peek_not_enough.cpp
	-@mkdir build\.objs\test-uve\windows\x64\release\test > NUL 2>&1
	@$(CXX) -c $(test-uve_CXX) -Fobuild\\.objs\\test-uve\\windows\\x64\\release\\test\\test_bufio_peek_not_enough.cpp.obj test\\test_bufio_peek_not_enough.cpp > build/.build.log 2>&1

build\.objs\test-uve\windows\x64\release\test\test_bufio_peek_twice.cpp.obj: test\test_bufio_peek_twice.cpp
	@echo compiling.release test\test_bufio_peek_twice.cpp
	-@mkdir build\.objs\test-uve\windows\x64\release\test > NUL 2>&1
	@$(CXX) -c $(test-uve_CXX) -Fobuild\\.objs\\test-uve\\windows\\x64\\release\\test\\test_bufio_peek_twice.cpp.obj test\\test_bufio_peek_twice.cpp > build/.build.log 2>&1

clean:  clean_uve clean_test-uve

clean_uve: 
	@del /F /Q build\windows\x64\release\uve.lib > NUL 2>&1
	@del /F /Q build\.objs\uve\windows\x64\release\src\request.c.obj > NUL 2>&1
	@del /F /Q build\.objs\uve\windows\x64\release\src\buffer.c.obj > NUL 2>&1
	@del /F /Q build\.objs\uve\windows\x64\release\src\bufio.c.obj > NUL 2>&1

clean_test-uve:  clean_uve
	@del /F /Q build\windows\x64\release\test-uve.exe > NUL 2>&1
	@del /F /Q build\.objs\test-uve\windows\x64\release\test\main.cpp.obj > NUL 2>&1
	@del /F /Q build\.objs\test-uve\windows\x64\release\test\test_bufio_peek.cpp.obj > NUL 2>&1
	@del /F /Q build\.objs\test-uve\windows\x64\release\test\test_bufio_peek_not_enough.cpp.obj > NUL 2>&1
	@del /F /Q build\.objs\test-uve\windows\x64\release\test\test_bufio_peek_twice.cpp.obj > NUL 2>&1

