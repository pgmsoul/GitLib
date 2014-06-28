#pragma once

namespace cs{
	void LogShowWindow(bool exit = true);
	void LogSetFontSize(int size);
	void LogHideWindow();
	void Log(LPCWSTR str,int color = 0xffffff,LPCWSTR app = (LPCWSTR)INVALID_HANDLE_VALUE);
	void LogSetBkColor(int color);
	void LogSetMaxLine(int count);
	void LogClear();
}