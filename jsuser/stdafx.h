#pragma once

#include <concise/conciseui.h>

#include <V8/include/v8.h>

#ifdef _DEBUG
#pragma comment(lib,"v8_baseD.lib")
#pragma comment(lib,"v8_snapshotD.lib")
#else
#pragma comment(lib,"v8_base.lib")
#pragma comment(lib,"v8_snapshot.lib")
#endif
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"ws2_32.lib")

using namespace v8;