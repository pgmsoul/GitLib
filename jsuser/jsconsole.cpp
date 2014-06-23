#include "stdafx.h"
#include "v8base.h"

namespace v8{
	Handle<Value> console_show(const Arguments& args){
		cs::Console* _h_console = cs::GetConsole();
		if(!_h_console->Show()) return Undefined();
		return True();
	}
	Handle<Value> console_close(const Arguments& args){
		cs::Console* _h_console = cs::GetConsole();
		_h_console->Close();
		return Undefined();
	}
	Handle<Value> console_write(const Arguments& args){
		HandleScope store;
		cs::String str;
		GetString(args[0],str);
		cs::Console* _h_console = cs::GetConsole();
		_h_console->Write(str); 
		return Undefined();
	}
	Handle<Value> console_writeln(const Arguments& args){
		HandleScope store;
		cs::String str;
		GetString(args[0],str);
		str += L"\r\n";
		cs::Console* _h_console = cs::GetConsole();
		_h_console->Write(str);
		return Undefined();
	}
	Handle<Value> console_setColor(const Arguments& args){
		HandleScope store;
		if(args.Length()<1) return Undefined();
		cs::Console* _h_console = cs::GetConsole();
		int color = args[0]->Int32Value();
		_h_console->SetTextColor(color);
		return Undefined();
	}
	Handle<Value> console_readln(const Arguments& args){
		HandleScope store;
		cs::String str;
		cs::Console* _h_console = cs::GetConsole();
		DWORD len = _h_console->ReadLn(str);
		return store.Close(String::New((uint16_t*)str.Handle()));
	}
	//*]}//*
	void LoadConsole(Handle<Object>& glb){
		Local<Object> console = Object::New();
		glb->Set(String::New("Console"),console);
		SET_OBJ_FUNC(console,show,console_show);
		SET_OBJ_FUNC(console,close,console_close);
		SET_OBJ_FUNC(console,write,console_write);
		SET_OBJ_FUNC(console,log,console_writeln);
		SET_OBJ_FUNC(console,readln,console_readln);
		SET_OBJ_FUNC(console,setColor,console_setColor);
	}
}