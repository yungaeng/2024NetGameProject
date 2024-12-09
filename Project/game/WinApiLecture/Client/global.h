#pragma once


// 미리 컴파일된 헤더를 비활성화 한 후 사용할 전역 헤더
// 윈도우 선언 전 윈속 헤더파일 선언.
#include <WinSock2.h>

#include<Windows.h>
#include<iostream>

#include <vector>
using std::vector;

#include <list>
using std::list;

#include<map>
using std::map;
using std::make_pair;


#include <string>
using std::string;
using std::wstring;

#include <math.h>
#include <assert.h>



#pragma comment(lib,"Msimg32.lib")

#include "define.h"
#include "struct.h"
#include "func.h"
