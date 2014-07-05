#include "stdafx.h"
#include "v8base.h"
#include "resource.h"

namespace v8{
	//*{
	//	"type":"group",
	//	"name":"wnd",
	//	"text":"窗口、控件。",
	//	"remark":[
	//		""
	//	],
	//	"member":[//*
	void MouseFromC(Handle<Object>& val,cs::Mouse* m){
		HandleScope store;
		SET_PROP(val,x,Int32::New(m->X));
		SET_PROP(val,y,Int32::New(m->Y));
		SET_PROP(val,button,Int32::New(m->Button));
		SET_PROP(val,event,Int32::New(m->Event));
		SET_PROP(val,wheel,Int32::New(m->WheelDelta/120));
	}
#define SET_V_FROM_OBJ_PROP(obj,name,v) v1 = obj->Get(String::New(#name));if(!v1->IsUndefined()){v = v1->ToInt32()->Int32Value();}

	Persistent<Object>* GetFuncFromWnd(cs::IWnd* cobj,LPCSTR name,Handle<Function>& func){
		Persistent<Object>* ps = (Persistent<Object>*)cobj->UserDataList[0];
		if(ps==NULL) return 0;
		Handle<Value> prop = (*ps)->Get(String::New(name));
		if(!prop->IsFunction()) return 0;
		func = Handle<Function>::Cast(prop);
		return ps;
	}
	Handle<Value> Message_send(const Arguments& args){
		HandleScope store;
		Handle<Object> self = args.This();
		HWND hWnd = (HWND)(UINT_PTR)self->Get(String::New("hWnd"))->Uint32Value();
		if(!IsWindow(hWnd)) return Undefined();
		UINT id = self->Get(String::New("hWnd"))->Uint32Value();
		WPARAM wParam = (WPARAM)(UINT_PTR)self->Get(String::New("hWnd"))->Uint32Value();
		LPARAM lParam = (LPARAM)(UINT_PTR)self->Get(String::New("hWnd"))->Uint32Value();
		HRESULT rst = ::SendMessage(hWnd,id,wParam,lParam);
		return store.Close(Uint32::New((UINT_PTR)rst));
	}
	//*{
	//	"type":"class",
	//	"name":"Message",
	//	"text":"窗口消息对象，Windows 窗口通过消息回调来触发事件。",
	//	"member":[//*
	class JsMessage{
		//*{
		//	"type":"property",
		//	"name":"hWnd",
		//	"objtype":"integer",
		//	"text":"引发窗口的窗口句柄，这个值和当前窗口的句柄相同。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"id",
		//	"objtype":"integer",
		//	"text":"消息类型。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"wParam",
		//	"objtype":"integer",
		//	"text":"一个整数参数，具体意义依赖于具体的消息类型。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"lParam",
		//	"objtype":"integer",
		//	"text":"一个整数参数，具体意义依赖于具体的消息类型。"
		//}//*
	};
	//*]}//*
	Handle<Value> Mouse_parseMessage(const Arguments& args){
		HandleScope store;
		while(true){
			if(args.Length()<1) break;
			if(!IsFunc(args[0],FUNC_ID_MESSAGE)) break;
			Handle<Object> msg = args[0]->ToObject();
			UINT id = msg->Get(String::New("id"))->Uint32Value();
			if((id<WM_MOUSEFIRST||id>WM_MOUSELAST)&&id!=WM_MOUSELEAVE) break;
			WPARAM wParam = msg->Get(String::New("wParam"))->Uint32Value();
			WPARAM lParam = msg->Get(String::New("lParam"))->Uint32Value();

			Handle<Object> self = args.This();
			self->Set(String::New("button"),Uint32::New(LOWORD(wParam)));
			self->Set(String::New("wheel"),Uint32::New(HIWORD(wParam)));
			self->Set(String::New("x"),Uint32::New(LOWORD(lParam)));
			self->Set(String::New("y"),Uint32::New(HIWORD(lParam)));
			self->Set(String::New("event"),Uint32::New(id));
			return True();
		}
		return Undefined();
	}
	//*,{
	//	"type":"class",
	//	"name":"Mouse",
	//	"text":"鼠标消息对象，鼠标消息使用的参数。",
	//	"member":[//*
	class Mouse{
		//*{
		//	"type":"property",
		//	"name":"x",
		//	"objtype":"integer",
		//	"text":"鼠标位置 x 坐标。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"y",
		//	"objtype":"integer",
		//	"text":"鼠标位置 y 坐标。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"button",
		//	"objtype":"integer",
		//	"text":"触发鼠标动作的按钮。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"event",
		//	"objtype":"integer",
		//	"text":"触发鼠标动作的事件类型。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"wheel",
		//	"objtype":"integer",
		//	"text":"鼠标滚动的方向，这个属性只会是 -120,0,120 这三个值，鼠标每滚动一次，触发一次滚动事件。"
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isMove()",
		//	"text":"当前消息是否是鼠标移动消息。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"返回一个是否指定事件的布尔值。"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isLDown()",
		//	"text":"当前消息是否是鼠标左键按下消息。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"返回一个是否指定事件的布尔值。"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isLUp()",
		//	"text":"当前消息是否是鼠标左键弹起消息。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"返回一个是否指定事件的布尔值。"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isLDBClick()",
		//	"text":"当前消息是否是鼠标左键双击消息。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"返回一个是否指定事件的布尔值。"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isRDown()",
		//	"text":"当前消息是否是鼠标右键按下消息。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"返回一个是否指定事件的布尔值。"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isRUp()",
		//	"text":"当前消息是否是鼠标右键弹起消息。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"返回一个是否指定事件的布尔值。"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isRDBClick()",
		//	"text":"当前消息是否是鼠标右键双击消息。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"返回一个是否指定事件的布尔值。"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isMDown()",
		//	"text":"当前消息是否是鼠标中键按下消息。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"返回一个是否指定事件的布尔值。"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isMUp()",
		//	"text":"当前消息是否是鼠标中键弹起消息。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"返回一个是否指定事件的布尔值。"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isMDBClick()",
		//	"text":"当前消息是否是鼠标中键双击消息。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"返回一个是否指定事件的布尔值。"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isXDown()",
		//	"text":"当前消息是否是鼠标 X（标准三键之外的键）键按下消息。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"返回一个是否指定事件的布尔值。"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isXUp()",
		//	"text":"当前消息是否是鼠标 X（标准三键之外的键）键弹起消息。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"返回一个是否指定事件的布尔值。"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isXDBClick()",
		//	"text":"当前消息是否是鼠标 X（标准三键之外的键）键双击消息。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"返回一个是否指定事件的布尔值。"
		//	}
		//}//*
//*,{
//	"type":"function",
//	"name":"isLeave()",
//	"text":"当前消息是否是鼠标离开当前窗口消息。",
//	"param":[
//	],
//	"return":{
//		"type":"boolean",
//		"text":"返回一个是否指定事件的布尔值。"
//	}
//}//*
//*,{
//	"type":"function",
//	"name":"isWheel()",
//	"text":"当前消息是否是鼠标滚动消息。",
//	"param":[
//	],
//	"return":{
//		"type":"boolean",
//		"text":"返回一个是否指定事件的布尔值。"
//	}
//}//*
//*,{
//	"type":"function",
//	"name":"isLButton()",
//	"text":"当前消息是否是鼠标左键触发的消息。",
//	"param":[
//	],
//	"return":{
//		"type":"boolean",
//		"text":"返回一个是否指定按钮的布尔值。"
//	}
//}//*
//*,{
//	"type":"function",
//	"name":"isX2Button()",
//	"text":"当前消息是否是鼠标 X2 键消息。",
//	"param":[
//	],
//	"return":{
//		"type":"boolean",
//		"text":"返回一个是否指定按钮的布尔值。"
//	}
//}//*
//*,{
//	"type":"function",
//	"name":"isRButton()",
//	"text":"当前消息是否是鼠标右键消息。",
//	"param":[
//	],
//	"return":{
//		"type":"boolean",
//		"text":"返回一个是否指定按钮的布尔值。"
//	}
//}//*
//*,{
//	"type":"function",
//	"name":"isMButton()",
//	"text":"当前消息是否是鼠标中键消息。",
//	"param":[
//	],
//	"return":{
//		"type":"boolean",
//		"text":"返回一个是否指定按钮的布尔值。"
//	}
//}//*
//*,{
//	"type":"function",
//	"name":"isX1Button()",
//	"text":"当前消息是否是鼠标 X1 键消息。",
//	"param":[
//	],
//	"return":{
//		"type":"boolean",
//		"text":"返回一个是否指定按钮的布尔值。"
//	}
//}//*
	};
	//*]}//*
	//这些函数必须作为纯函数而不是类的静态函数才能被 Delegate 的 Add 函数正确识别，虽然本质是一样的。
	void __stdcall Wnd_onCreate(cs::IWnd* cobj){
		HandleScope store;
		Handle<Function> func;
		Persistent<Object>* ps = GetFuncFromWnd(cobj,"onCreate",func);
		if(!ps) return;
		Handle<Value> argv[1];
		CallFunc(*ps,func,0,argv);
	}
	void __stdcall Wnd_onMouse(cs::Mouse* m,cs::IWnd* cobj){
		HandleScope store;
		Handle<Function> func;
		Persistent<Object>* ps = GetFuncFromWnd(cobj,"onMouse",func);
		if(!ps) return;
		Handle<Value> argv[5];
		argv[0] = Int32::New(m->X);
		argv[1] = Int32::New(m->Y);
		argv[2] = Int32::New(m->Button);
		argv[3] = Int32::New(m->Event);
		argv[4] = Int32::New(m->WheelDelta);
		argv[0] = GetEnv()->GetFunc(FUNC_ID_MOUSE)->CallAsConstructor(5,argv);

		CallFunc(*ps,func,1,argv);
	}
	void __stdcall Wnd_onClose(bool& cancel,cs::IWnd* cobj){
		HandleScope store;
		Handle<Function> func;
		Persistent<Object>* ps = GetFuncFromWnd(cobj,"onClose",func);
		if(!ps) return;
		Handle<Value> argv[1];
		Handle<Value> ret = CallFunc(*ps,func,0,argv);
		if(ret->IsTrue()) cancel = true;
	}
	void __stdcall Wnd_onCommand(WORD id,WORD type,cs::IWnd* cobj){
		HandleScope store;
		Handle<Function> func;
		Persistent<Object>* ps = GetFuncFromWnd(cobj,"onCommand",func);
		if(!ps) return;
		Handle<Value> argv[2];
		argv[0] = Int32::New(id);
		argv[1] = Int32::New(type);
		CallFunc(*ps,func,2,argv);
	}
	void __stdcall Wnd_onSize(cs::Twin16 cs,cs::IWnd* cobj){
		HandleScope store;
		Handle<Function> func;
		Persistent<Object>* ps = GetFuncFromWnd(cobj,"onSize",func);
		if(!ps) return;
		Handle<Value> argv[2];
		argv[0] = Int32::New(cs.x);
		argv[1] = Int32::New(cs.y);
		CallFunc(*ps,func,2,argv);
	}
	void __stdcall Wnd_onMessage(cs::Message* msg,cs::IWnd* cobj){
		HandleScope store;
		Handle<Function> func;
		Persistent<Object>* ps = GetFuncFromWnd(cobj,"onMessage",func);
		if(!ps) return;
		Handle<Value> argv[4];
		argv[0] = Uint32::New((UINT_PTR)msg->hWnd);
		argv[1] = Uint32::New(msg->uMsg);
		argv[2] = Uint32::New((UINT_PTR)msg->wParam);
		argv[3] = Uint32::New((UINT_PTR)msg->lParam);
		Handle<Object> obj = GetEnv()->GetFunc(FUNC_ID_MESSAGE)->CallAsConstructor(4,argv)->ToObject();
		argv[0] = obj;
		CallFunc(*ps,func,1,argv);
		Handle<Value> result = obj->Get(String::New("result"));
		if(!result.IsEmpty()&&!result->IsUndefined()){
			msg->Result = result->Uint32Value();
		}
	}
	void __stdcall Layout_onSize(cs::Message* msg,cs::IWnd* cobj){
		if(msg->wParam==SIZE_MINIMIZED) return;
		HandleScope store;
		Persistent<Object>* ps = (Persistent<Object>*)cobj->UserDataList[1];
		if(!ps) return;
		Handle<Value> prop = (*ps)->Get(String::New("param"));
		if(prop.IsEmpty()||prop->IsUndefined()||!IsFunc(prop,FUNC_ID_LAYOUTPARAM)) return;
		prop = prop->ToObject()->Get(String::New("margin"));
		if(prop.IsEmpty()||prop->IsUndefined()||!IsFunc(prop,FUNC_ID_RECT)) return;

		Handle<Object> margin = prop->ToObject();
		int left = margin->Get(String::New("left"))->Int32Value();
		int top = margin->Get(String::New("top"))->Int32Value();
		int right = margin->Get(String::New("right"))->Int32Value();
		int bottom = margin->Get(String::New("bottom"))->Int32Value();
		// Scroll 版本的
		cs::Twin16 cs = *((cs::Twin16*)&msg->lParam);
		int w = cs::Maximum(cobj->MinSize().x,(int)cs.x);
		int h = cs::Maximum(cobj->MinSize().y,(int)cs.y);

		prop = (*ps)->Get(String::New("_setRect"));
		if(prop->IsFunction()){
			Handle<Function> func = Handle<Function>::Cast(prop);
			Handle<Value> argv[4];
			argv[0] = Int32::New(left-cobj->Scr().x);
			argv[1] = Int32::New(top-cobj->Scr().y);
			argv[2] = Int32::New(w-left-right);
			argv[3] = Int32::New(h-top-bottom);
			CallFunc(*ps,func,4,argv);
			cobj->Invalidate();
		}
	}
	template<typename T> class BWnd : public T{
	public:
		BWnd(){
			Persistent<Object>* ps = new Persistent<Object>;
			UserDataList.Add(ps);
			UserDataList.Add(0);
		}
		~BWnd(){
			for(uint i=0;i<2;i++){
				Persistent<Object>* ps = (Persistent<Object>*)UserDataList[i];
				if(ps==0) continue;
				ps->Dispose();
				ps->Clear();
				delete ps;
				UserDataList[i] = 0;
			}
		}
	};
	class BIWnd : public BWnd<cs::IWnd>{};
	template<typename T,typename S,int id> class JsWndObject : public JsHandleObject<T,HWND,S,id>{
		static void _on_create(Handle<Object>& self,BIWnd* cobj){
			Persistent<Object>* ps = (Persistent<Object>*)cobj->UserDataList[0];
			*ps = Persistent<Object>::New(self);
		}
	public:
		static void on_create(Handle<Object>& self,T* cobj){
			_on_create(self,(BIWnd*)cobj);
		}
		static void on_dispose(Handle<Object>& self,T* cobj){
			if(cobj) ::DestroyWindow(*cobj);
		}
	};
#define SetWndCallback(name,on,func) self->SetHiddenValue(String::New(#name),value);\
	if(value->IsFunction()){\
	if(cobj->on.IndexOf(&func)==-1)\
	cobj->on.Add(&func);\
	}else{\
	cobj->on.Delete(&func);\
	}
	//*,{
	//	"type":"class",
	//	"name":"CWnd",
	//	"text":"CWnd 封装一个窗口对象，Windows 系统的任何窗口和子窗口都是一个 CWnd 对象。",
	//	"member":[//*
	class JsWnd : public JsWndObject<BIWnd,JsWnd,TEMPLATE_ID_WND>{
		//*{
		//	"type":"function",
		//	"name":"sendMessage(id,[wParam],[lParam])",
		//	"text":"向当前窗口发送消息，窗口的很多功能是通过消息实现的。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"id",
		//			"text":"消息 id，也就是消息的种类，具体消息的意义，查阅微软的文档。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[wParam]",
		//			"text":"一个整数参数，缺省为 0 。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[lParam]",
		//			"text":"一个整数参数，缺省为 0 。"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"成功返回一个整数，具体意义依赖于消息的类型，如果当前窗口还没有创建，函数失败，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> sendMessage(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				cs::IWnd* cobj;
				Local<Object> self = args.This();
				if(!GetCHandle(cobj,self)) break;
				if(cobj->Handle()==0) break;

				UINT id = GET_ARGS_INT2(0,0,UINT);
				WPARAM wParam = GET_ARGS_INT2(1,0,WPARAM);
				WPARAM lParam = GET_ARGS_INT2(2,0,LPARAM);
				LRESULT rst = cobj->SendMessage(id,wParam,lParam);
				return store.Close(Uint32::New(rst));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getRect()",
		//	"text":"获取窗口外框的矩形，以像素为单位，左上角是（0，0），x 轴方向向右，y 轴方向向下。如果是顶级窗口，以屏幕为参考，如果是子窗口，以父窗口为参考。这个函数可以在窗口生成前调用，返回的是将要生成的窗口的参数。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"Rect",
		//		"text":"成功返回一个 Rect 对象，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> getRect(const Arguments& args){
			HandleScope store;
			while(true){
				cs::IWnd* cobj;
				Local<Object> self = args.This();
				if(!GetCHandle(cobj,self)) break;

				cs::Rect r;
				if(cobj->Handle()==0){
					r.SetValue(0,0,cobj->Param->Size.x,cobj->Param->Size.y);
					r.OffsetLocation(cobj->Param->Location.x,cobj->Param->Location.y);
				}else{
					if(!cobj->GetRect(r)) break;
				}
				Handle<Value> jsr = LoadJsCode(L"new Rect();",L"CWnd.getRect");
				RectJsFromC(jsr,&r);
				return store.Close(jsr);
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getClientSize()",
		//	"text":"获取窗口客户区的尺寸。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"Size",
		//		"text":"成功返回一个 Size 对象，失败返回 undefined。"
		//	},
		//	"remark":[
		//		"客户区是窗口供用户绘图和放置子窗口的区域，它是窗口除去标题栏和边框的区域。"
		//	]
		//}//*
		static Handle<Value> getClientSize(const Arguments& args){
			HandleScope store;
			while(true){
				cs::IWnd* cobj;
				Local<Object> self = args.This();
				if(!GetCHandle(cobj,self)) break;

				cs::Twin16 cs;
				if(!cobj->GetClientSize(cs)) break;
				Handle<Object> jsr = LoadJsCode(L"new Size();",L"CWnd.getClientSize")->ToObject();
				jsr->Set(String::New("x"),Int32::New(cs.x));
				jsr->Set(String::New("y"),Int32::New(cs.y));
				return store.Close(jsr);
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setSize(width,[height])",
		//	"text":"设置窗口（外框）的大小，可以单独设置宽口或高度。这个函数可以在窗口生成前调用，设置它影响窗口生成后的属性。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"width",
		//			"text":"要设置的宽度，如果这个值被设置为 undefined，窗口的宽度不变。窗口宽度有一个最小值，窗口不会被设置为小于这个值的宽度，普通窗口的最小值是 0."
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[height]",
		//			"text":"要设置的高度，如果这个值缺省或者为 undefined，窗口的高度不变。窗口高度有一个最小值，窗口高度不能被设置为小于这个最小值，这个值通常是 0 。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> setSize(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;

				cs::IWnd* cobj;
				Local<Object> self = args.This();
				if(!GetCHandle(cobj,self)) break;

				int cx,cy;
				cx = GET_ARGS_INT(0,0);
				if((args.Length()==1||args[1]->IsUndefined())&&!args[0]->IsUndefined()){
					if(cobj->Handle()==0) cobj->Param->Size.x = cx;
					else if(!cobj->SetWidth(cx)) break;
					return True();
				}
				cy = GET_ARGS_INT(1,0);
				if(args.Length()>1&&args[0]->IsUndefined()&&!args[1]->IsUndefined()){
					if(cobj->Handle()==0) cobj->Param->Size.x = cy;
					else if(!cobj->SetHeight(cy)) break;
					return True();
				}
				if(args.Length()>1&&!args[0]->IsUndefined()&&!args[1]->IsUndefined()){
					if(cobj->Handle()==0) cobj->Param->Size.SetValue(cx,cy);
					else if(!cobj->SetSize(cx,cy)) break;
					return True();
				}
				break;
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setRect(x,y,cx,cy)",
		//	"text":"设置窗口的位置、高度、宽度。如果要单独设置位置或尺寸，使用 setPosition 或 setSize 函数。这个函数可以在窗口生成前调用，设置它影响窗口生成后的属性。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"x",
		//			"text":"要设置的窗口 x 坐标。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"y",
		//			"text":"要设置的窗口 y 坐标。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"cx",
		//			"text":"要设置的窗口宽度。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"cy",
		//			"text":"要设置的窗口高度。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	},
		//	"remark":[
		//		"函数所需的 4 个参数必需设置有意义的值，如果参数不足 4 个，函数会失败。"
		//	]
		//}//*
		static Handle<Value> setRect(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<4) break;

				cs::IWnd* cobj;
				Local<Object> self = args.This();
				if(!GetCHandle(cobj,self)) break;

				int x,y,cx,cy;
				x = GET_ARGS_INT(0,0);
				y = GET_ARGS_INT(1,0);
				cx = GET_ARGS_INT(2,0);
				cy = GET_ARGS_INT(3,0);
				if(cobj->Handle()==0) cobj->Param->SetRect(x,y,cx,cy);
				else{
					if(!cobj->SetDimension(x,y,cx,cy)) break;
				}
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setPosition(x,[y])",
		//	"text":"设置窗口的位置，如果是顶级窗口，以屏幕坐标系为参考，如果是子窗口，以父窗口坐标系为参考。这个函数可以在窗口生成前调用，设置它影响窗口生成后的属性。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"x",
		//			"text":"要设置的 x 坐标，如果这个值被设置为 undefined，窗口的 x 位置不变。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[y]",
		//			"text":"要设置的 y 坐标，如果这个值缺省或者为 undefined，窗口的 y 坐标不变。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> setPosition(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;

				cs::IWnd* cobj;
				Local<Object> self = args.This();
				if(!GetCHandle(cobj,self)) break;

				int x,y;
				x = GET_ARGS_INT(0,0);
				if((args.Length()==1||args[1]->IsUndefined())&&!args[0]->IsUndefined()){
					if(cobj->Handle()==0) cobj->Param->Location.x = x;
					else if(!cobj->SetXLocation(x)) break;
					return True();
				}
				y = GET_ARGS_INT(1,0);
				if(args.Length()>1&&args[0]->IsUndefined()&&!args[1]->IsUndefined()){
					if(cobj->Handle()==0) cobj->Param->Location.y = y;
					else if(!cobj->SetYLocation(y)) break;
					return True();
				}
				if(args.Length()>1&&!args[0]->IsUndefined()&&!args[1]->IsUndefined()){
					if(cobj->Handle()==0) cobj->Param->Location.SetValue(x,y);
					else if(!cobj->SetLocation(x,y)) break;
					return True();
				}
				break;
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"offset(dx,[dy],[dcx],[dcy])",
		//	"text":"调整窗口的位置和大小。可以在窗口没有生成的时候调用这个函数，这些设置的位置参数将会被保存，并且在生成窗口时使用。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"dx",
		//			"text":"要调整的 x 坐标变化量，如果不改变窗口的 x 坐标，设置为 0 。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[dy]",
		//			"text":"要调整的 y 坐标变化量，如果不改变窗口的 y 坐标，设置为 0 。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[dcx]",
		//			"text":"要调整的宽度变化量，如果不改变窗口的宽度，设置为 0 。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[dcy]",
		//			"text":"要调整的高度坐标变化量，如果不改变窗口的高度，设置为 0 。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> offset(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;

				cs::IWnd* cobj;
				Local<Object> self = args.This();
				if(!GetCHandle(cobj,self)) break;

				int dx,dy,cx,cy;
				dx = GET_ARGS_INT(0,0);
				dy = GET_ARGS_INT(1,0);
				cx = GET_ARGS_INT(2,0);
				cy = GET_ARGS_INT(3,0);
				if(cobj->Handle()==0){
					cobj->Param->Location.Offset(dx,dy);
					cobj->Param->Size.Offset(cx,cy);
				}else{
					cobj->Offset(dx,dy,cx,cy);
				}
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"destroy()",
		//	"text":"销毁窗口。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> destroy(const Arguments& args){
			HandleScope store;
			while(true){
				cs::IWnd* cobj;
				Local<Object> self = args.This();
				if(!GetCHandle(cobj,self)) break;

				if(!::DestroyWindow(*cobj)) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"invalidate(rgn)",
		//	"text":"设置一个刷新消息，窗口被重新绘制。调用这个函数后，onDraw 回调函数将会被呼叫。这个函数可以多次调用，连续调用刷新区将会被组合，最终只会引发一次重绘动作。",
		//	"param":[
		//		{
		//			"type":"CRgn|Rect",
		//			"name":"[rgn]",
		//			"text":"设置将要被刷新的区域，它要么缺省，要么设置一个 CRgn 对象或者一个 Rect 对象。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> invalidate(const Arguments& args){
			HandleScope store;
			while(true){
				cs::IWnd* cobj;
				Local<Object> self = args.This();
				if(!GetCHandle(cobj,self)) break;

				bool r = false;
				if(args.Length()>0){
					if(IsTemplate(args[0],TEMPLATE_ID_RGN)){
						cs::Rgn* rgn;
						if(!GetCHandle(rgn,args[0]->ToObject())) break;
						r = cobj->Invalidate(*rgn);
					}
					if(IsFunc(args[0],FUNC_ID_RECT)){
						cs::Rect cr;
						RectJsToC(args[0],&cr);
						r = cobj->Invalidate(cr);
					}
				}else{
					r = cobj->Invalidate();
				}
				if(!r) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setRemPos(name,[flag])",
		//	"text":"设置窗口记住自己的位置和大小，或者取消这个特性。应该在窗口生成前调用这个函数，因为窗口的位置是在生成的时候设置的，所以如果在窗口生成后调用这个函数，虽然在窗口销毁时记住了位置，但是生成时，不会使用这个位置。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"name",
		//			"text":"函数通过当前目录下的脚本同名 json 文件记录窗口的大小和位置，这个名称用于区分不同的窗口，所以一个应用程序不能有同名的窗口。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[flag]",
		//			"text":"记录选项，缺省记录位置和大小。1：记录 X 坐标；2：记录 Y 坐标；4：记录宽度；8：记录高度。这个值可以是上述 4 个值的组合，默认是 15，即记录全部 4 个属性。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> setRemPos(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;

				cs::IWnd* cobj;
				Local<Object> self = args.This();
				if(!GetCHandle(cobj,self)) break;

				cs::String name;
				GetString(args[0],name);
				DWORD flag = GET_ARGS_INT(1,0xf);
				cs::SetWndRememberPos(cobj,name,flag);
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setLayout(layout)",
		//	"text":"设置一个 Layout 对象，当前窗口作为 layout 的顶级窗口，参看 Layout 条目。",
		//	"param":[
		//		{
		//			"type":"Layout",
		//			"name":"layout",
		//			"text":"要绑定的 Layout 对象。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> setLayout(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;

				cs::IWnd* cobj;
				Local<Object> self = args.This();
				if(!GetCHandle(cobj,self)) break;

				Persistent<Object>* pobj = (Persistent<Object>*)cobj->UserDataList[1];
				if(pobj==0){
					pobj = new Persistent<Object>;
					cobj->UserDataList[1] = pobj;
					cobj->GetMsgHook(WM_SIZE)->Add(&Layout_onSize);
				}else{
					if((*pobj)==args[0]) return True();
					(*pobj).Dispose();
					(*pobj).Clear();
				}
				if(IsFunc(args[0],FUNC_ID_LAYOUT)){
					*pobj = Persistent<Object>::New(args[0]->ToObject());
				}else{
					cobj->GetMsgHook(WM_SIZE)->Delete(&Layout_onSize);
					cobj->UserDataList[1] = 0;
					delete pobj;
				}
				if(cobj->Handle()){
					Handle<Object> lay = args[0]->ToObject();
					Handle<Function> refresh = Handle<Function>::Cast(lay->Get(NEW_STR(refresh)));
					if(refresh->IsFunction()){
						Handle<Value> argv[1];
						argv[0] = Uint32::New((UINT_PTR)cobj->Handle());
						CallFunc(lay,refresh,1,argv);
					}
				}
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setFont(font)",
		//	"text":"设置窗口的字体，这个字体对于普通窗体没有任何作用，但是对于 Windows 控件，它是控件显示文字使用的字体，一个字体可以设置给多个窗体。wnd 系统有一个缺省字体，每个控件默认都是使用这个缺省字体。可以使用 CFont 对象的 setToDefault 函数，设置当前字体为缺省字体，改变这个字体，将会影响所有使用这个字体的窗口。",
		//	"param":[
		//		{
		//			"type":"CFont",
		//			"name":"font",
		//			"text":"要设置的字体。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> setFont(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;

				cs::IWnd* cobj;
				Local<Object> self = args.This();
				if(!GetCHandle(cobj,self)) break;

				cs::Font* font;
				if(!GetCHandle(font,args[0]->ToObject())) break;
				if(!cobj->SetFont(*font)) break;
				cobj->Invalidate();
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setCursor(icon)",
		//	"text":"设置窗口的鼠标指针，窗口原来的指针对象会被销毁（如果能够被销毁的话）。",
		//	"param":[
		//		{
		//			"type":"CIcon",
		//			"name":"icon",
		//			"text":"一个有效的图标对象。"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"函数没有返回值。"
		//	}
		//}//*
		static Handle<Value> setCursor(const Arguments& args){
			HandleScope store;
			while(true){
				cs::Frame* cobj;
				Local<Object> self = args.This();
				if(!GetCHandle(cobj,self)) break;

				if(args.Length()<1) break;
				cs::Icon* picon;
				if(IsTemplate(args[0],TEMPLATE_ID_ICON)){
					if(!GetCHandle(picon,args[0]->ToObject())) break;
				}
				bool big = GET_ARGS_VALUE(1,0,Boolean);
				HICON hicon = (HICON)cobj->GetCursor();
				cobj->SetCursor((HCURSOR)*picon);
				picon->Detach();
				picon->Attach(hicon);
				break;
			}
			return Undefined();
		}
	public:
		//*,{
		//	"type":"property",
		//	"name":"text",
		//	"objtype":"string",
		//	"text":"返回或设置窗口的文本，不同窗口文本的意义不同，比如顶级窗口是窗口的标题，而 Edit 控件则是控件的文本。这个参数可以在窗口生成前设置和获取，设置它影响窗口生成后的属性。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"parent",
		//	"objtype":"integer",
		//	"text":"设置/获取窗口的父窗口，如果窗口没有父窗口，返回 0。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"disabled",
		//	"objtype":"boolean",
		//	"text":"设置/获取窗口的 disabled 状态，一个 disabled 窗口不响应用户的操作。这个参数可以在窗口生成前设置和获取，设置它影响窗口生成后的属性。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"style",
		//	"objtype":"integer",
		//	"text":"设置/获取窗口的 style 参数，style 参数控制窗口的各种特性。这个参数可以在窗口生成前设置和获取，设置它影响窗口生成后的属性。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"exStyle",
		//	"objtype":"integer",
		//	"text":"设置/获取窗口的 exStyle 参数，exStyle 参数控制窗口的额外特性。其实就是 style 一个整数不够用，再加一个参数。这个参数可以在窗口生成前设置和获取，设置它影响窗口生成后的属性。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"classStyle",
		//	"objtype":"integer",
		//	"text":"设置/获取窗口的 classStyle 参数，classStyle 是窗口类的风格。这个参数可以在窗口生成前设置和获取，设置它影响窗口生成后的属性。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"fontColor",
		//	"objtype":"integer",
		//	"text":"设置/获取窗口的字体颜色，某些控件可能会特殊处理，这个设置不一定会起作用。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"bkColor",
		//	"objtype":"integer",
		//	"text":"设置/获取窗口的背景颜色。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"onCreate()",
		//	"objtype":"function",
		//	"text":"回调函数，响应创建窗口时的 WM_CREATE 消息的回调函数。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"函数没有返回值。"
		//	}
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"onSize(cx,cy)",
		//	"objtype":"function",
		//	"text":"回调函数，响应创建窗口调整尺寸的 WM_SIZE 消息。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"cx",
		//			"text":"客户区宽度。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"cy",
		//			"text":"客户区高度。"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"函数没有返回值。"
		//	}
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"onCommand(id,type)",
		//	"objtype":"function",
		//	"text":"回调函数，响应创建窗口调整尺寸的 WM_SIZE 消息。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"cx",
		//			"text":"客户区宽度。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"cy",
		//			"text":"客户区高度。"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"函数没有返回值。"
		//	}
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"onMouse(mouse)",
		//	"objtype":"function",
		//	"text":"回调函数，响应鼠标消息。",
		//	"param":[
		//		{
		//			"type":"Mouse",
		//			"name":"mouse",
		//			"text":"鼠标对象，参加 Mouse 条目。"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"函数没有返回值。"
		//	}
		//}//*
		static Handle<Value> get(cs::String& name,BIWnd* cobj,Local<Object>& self){
			if(name==L"text"){
				cs::String title;
				if(cobj->Handle()==0) title = cobj->Param->Text;
				else cobj->GetText(title);
				return String::New((uint16_t*)title.Handle());
			}else if(name==L"parent"){
				return Uint32::New((UINT_PTR)cobj->Parent());
			}else if(name==L"disabled"){
				DWORD style;
				if(cobj->Handle()==0) style = cobj->Param->Style;
				else style = cobj->GetWindowParam(GWL_STYLE);
				return Bool((style&WS_DISABLED)==WS_DISABLED);
			}else if(name==L"style"){
				DWORD style;
				if(cobj->Handle()==0) style = cobj->Param->Style;
				else style = cobj->GetWindowParam(GWL_STYLE);
				return Uint32::New(style);
			}else if(name==L"exStyle"){
				DWORD style;
				if(cobj->Handle()==0) style = cobj->Param->ExStyle;
				else style = cobj->GetWindowParam(GWL_EXSTYLE);
				return Uint32::New(style);
			}else if(name==L"classStyle"){
				DWORD classStyle;
				if(cobj->Handle()==0) classStyle = cobj->Param->ClassStyle;
				else classStyle = ::GetClassLong(*cobj,GCL_STYLE);
				return Uint32::New(classStyle);
			}else if(name==L"className"){
				return NEW_WSTR(cobj->Param->ClassName.Handle());
			}else if(name==L"bkColor"){
				return Uint32::New(cobj->BkColor);
			}else if(name==L"fontColor"){
				return Uint32::New(cobj->FontColor);
			}else if(name==L"id"){
				DWORD id;
				if(cobj->Handle()==0) id = cobj->Param->Identity;
				else id = cobj->GetWindowParam(GWL_ID);
				return Uint32::New(id);
			}else if(name==L"onCreate"){
				return self->GetHiddenValue(String::New("onCreate"));
			}else if(name==L"onMouse"){
				return self->GetHiddenValue(String::New("onMouse"));
			}else if(name==L"onClose"){
				return self->GetHiddenValue(String::New("onClose"));
			}else if(name==L"onCommand"){
				return self->GetHiddenValue(String::New("onCommand"));
			}else if(name==L"onSize"){
				return self->GetHiddenValue(String::New("onSize"));
			}else if(name==L"onMessage"){
				return self->GetHiddenValue(String::New("onMessage"));
			}
			return Undefined();
		}
		static void set(cs::String& name,BIWnd* cobj,Local<Value>& value,Local<Object>& self){
			if(name==L"text"){
				GetString(value,cobj->Param->Text);
				if(cobj->Handle()!=0)
					cobj->SetText(cobj->Param->Text);
			}else if(name==L"parent"){
				uint parent = value->Uint32Value();
				SetParent(*cobj,(HWND)(UINT_PTR)parent);
			}else if(name==L"disabled"){
				bool disabled = value->BooleanValue();
				if(cobj->Handle()==0){
					cobj->Param->Style |= WS_DISABLED;
					if(!disabled) cobj->Param->Style -= WS_DISABLED;
				}
				else cobj->Enable(!disabled);
			}else if(name==L"style"){
				DWORD style = value->Uint32Value();
				if(cobj->Handle()==0) cobj->Param->Style = style;
				else cobj->SetWindowParam(GWL_STYLE,style);
			}else if(name==L"exStyle"){
				DWORD style = value->Uint32Value();
				if(cobj->Handle()==0) cobj->Param->ExStyle = style;
				else cobj->SetWindowParam(GWL_EXSTYLE,style);
			}else if(name==L"classStyle"){
				DWORD classStyle = value->Uint32Value();
				if(cobj->Handle()==0) cobj->Param->ClassStyle = classStyle;
				else ::SetClassLong(*cobj,GCL_STYLE,classStyle);
			}else if(name==L"className"){
				GetString(value,cobj->Param->ClassName);
			}else if(name==L"bkColor"){
				cobj->BkColor = value->Uint32Value();
				cobj->Invalidate();
			}else if(name==L"fontColor"){
				cobj->FontColor = value->Uint32Value();
				cobj->Invalidate();
			}else if(name==L"id"){
				DWORD id = value->Uint32Value();
				if(cobj->Handle()==0) cobj->Param->Identity = (WORD)id;
				else cobj->SetWindowParam(GWL_ID,id);
			}else if(name==L"onCreate"){
				SetWndCallback(onCreate,OnCreate,Wnd_onCreate);
			}else if(name==L"onMouse"){
				SetWndCallback(onMouse,OnMouse,Wnd_onMouse);
			}else if(name==L"onClose"){
				SetWndCallback(onClose,OnClose,Wnd_onClose);
			}else if(name==L"onCommand"){
				SetWndCallback(onCommand,OnCommand,Wnd_onCommand);
			}else if(name==L"onSize"){
				SetWndCallback(onSize,OnSize,Wnd_onSize);
			}else if(name==L"onMessage"){
				self->SetHiddenValue(String::New("onMessage"),value);
				if(value->IsFunction()){
					if(cobj->GetMsgHook(0,0xffff)->IndexOf(&Wnd_onMessage)==-1)
						cobj->GetMsgHook(0,0xffff)->Add(&Wnd_onMessage);
				}else{
					cobj->GetMsgHook(0,0xffff)->Delete(&Wnd_onMessage);
				}			
			}
		}
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			SET_CLA_FUNC(setRemPos);
			SET_CLA_FUNC(getRect);
			SET_CLA_FUNC(getClientSize);
			SET_CLA_FUNC(setSize);
			SET_CLA_FUNC(setRect);
			SET_CLA_FUNC(setPosition);
			SET_CLA_FUNC(offset);
			SET_CLA_FUNC(destroy);
			SET_CLA_FUNC(invalidate);
			SET_CLA_FUNC(setLayout);
			SET_CLA_FUNC(setCursor);
			SET_CLA_FUNC(setFont);

			SET_CLA_ACCESSOR(text);
			SET_CLA_ACCESSOR(parent);
			SET_CLA_ACCESSOR(disabled);
			SET_CLA_ACCESSOR(style);
			SET_CLA_ACCESSOR(exStyle);
			SET_CLA_ACCESSOR(bkColor);
			SET_CLA_ACCESSOR(fontColor);
			SET_CLA_ACCESSOR(id);
			SET_CLA_ACCESSOR(onCreate);
			SET_CLA_ACCESSOR(onMouse);
			SET_CLA_ACCESSOR(onClose);
			SET_CLA_ACCESSOR(onCommand);
			SET_CLA_ACCESSOR(onSize);
			SET_CLA_ACCESSOR(onMessage);
		}
		static void Load(Handle<Object>& glb){
			JsHandleObject::Load(glb,L"CWnd");
		}
	};
	//*],"source":"D:\\SoftProject\\GitLib\\jsuser\\example\\wnd.jsuser"}//*
	class BView : public BWnd<cs::ScrollView>{};
	void __stdcall View_onDropFile(cs::StringMemList* fl,cs::View* cobj){
		HandleScope store;
		Handle<Function> func;
		Persistent<Object>* ps = GetFuncFromWnd(cobj,"onDropFile",func);
		if(!ps) return;
		Handle<Value> argv[1];
		cs::String sl;
		for(uint i=0;i<fl->Count();i++){
			if(!sl.IsNull()) sl += L",";
			sl += (*fl)[i];
		}
		argv[0] = String::New((uint16_t*)sl.Handle());
		CallFunc(*ps,func,1,argv);
	}
	void __stdcall View_onDraw(cs::DC* dc,cs::View* cobj){
		HandleScope store;
		Handle<Function> func;
		Persistent<Object>* ps = GetFuncFromWnd(cobj,"onDraw",func);
		if(!ps) return;
		Handle<Value> argv[1];
		argv[0] = Uint32::New((UINT_PTR)dc->Handle());
		CallFunc(*ps,func,1,argv);
	}
	void __stdcall View_onDoubleDraw(cs::DC* dc,cs::View* cobj){
		HandleScope store;
		Persistent<Object>* ps = (Persistent<Object>*)cobj->UserDataList[0];
		Handle<Value> prop = (*ps)->Get(String::New("onDoubleDraw"));
		if(prop->IsFunction()){
			Handle<Function> func = Handle<Function>::Cast(prop);
			Handle<Value> argv[1];
			argv[0] = Uint32::New((UINT_PTR)dc->Handle());
			CallFunc(*ps,func,1,argv);
		}
	}
	void __stdcall View_onScroll(int dx,int dy,cs::ScrollView* cobj){
		HandleScope store;
		Handle<Function> func;
		Persistent<Object>* ps = GetFuncFromWnd(cobj,"onScroll",func);
		if(!ps) return;
		Handle<Value> argv[2];
		argv[0] = Int32::New(dx);
		argv[1] = Int32::New(dy);
		CallFunc(*ps,func,2,argv);
	}
	//*,{
	//	"type":"class",
	//	"name":"CView",
	//	"text":"CView 是一个通常的窗口对象，集成滚动条绘图等功能。CView 继承自 CWnd。",
	//	"member":[//*
	class JsView : public JsWndObject<BView,JsView,TEMPLATE_ID_VIEW>{
		//*{
		//	"type":"function",
		//	"name":"create(parent)",
		//	"text":"设置窗口记住自己的位置和大小，或者取消这个特性。",
		//	"param":[
		//		{
		//			"type":"integer|CWnd",
		//			"name":"parent",
		//			"text":"生成的窗口是 parent 的子窗口，获取一个窗口的句柄调用 handle 函数。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> create(const Arguments& args){
			HandleScope store;
			while(true){
				BView* cobj;
				Local<Object> self = args.This();
				if(!GetCHandle(cobj,self)) break;

				HWND parent = GET_ARGS_INT2(0,0,HWND);
				if(IsTemplate(args[0],TEMPLATE_ID_WND)){
					cs::IWnd* wnd;
					if(GetCHandle(wnd,args[0]->ToObject())){
						parent = wnd->Handle();
					}
				}

				if(!cobj->Create(parent)) break;
				return True();
			}
			return Undefined();
		}
	public:
		//*,{
		//	"type":"property",
		//	"name":"onDropFile",
		//	"objtype":"function",
		//	"text":"这是一个回调函数，当文件被拖动到窗口是被呼叫。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"files",
		//			"text":"这是一个用逗号分隔的一系列文件名全路径构成的字串。"
		//		}
		//	],
		//	"return":{
		//		"type":"undefined",
		//		"text":"这个函数没有返回值。"
		//	}
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"onCommand(id,type)",
		//	"objtype":"function",
		//	"text":"这是一个回调函数，当窗口菜单或按钮被点击时或者加速键被按下时，这个函数被呼叫。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"id",
		//			"text":"菜单、控件或者加速键的 id，用于识别那个菜单或者控件被点击。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"type",
		//			"text":"当消息是因为菜单被点击产生的时候，这个值是 0，如果是控件产生的，这个值是 1，其它情况参考 Windows 的说明文档。"
		//		}
		//	],
		//	"return":{
		//		"type":"undefined",
		//		"text":"这个函数没有返回值。"
		//	}
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"onDraw(dc)",
		//	"objtype":"function",
		//	"text":"这是一个回调函数，用于绘制窗口。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"dc",
		//			"text":"绘图函数需要使用 dc 参数来向窗口绘图。"
		//		}
		//	],
		//	"return":{
		//		"type":"undefined",
		//		"text":"这个函数没有返回值。"
		//	}
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"onScroll(dx,dy)",
		//	"objtype":"function",
		//	"text":"这是一个回调函数，当窗口滚动时被呼叫。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"dx",
		//			"text":"水平方向滚动像素数，负值向左，正值向右。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"dy",
		//			"text":"垂直方向滚动像素数，负值向上，正值向下。"
		//		}
		//	],
		//	"return":{
		//		"type":"undefined",
		//		"text":"这个函数没有返回值。"
		//	}
		//}//*
		static Handle<Value> get(cs::String& name,BView* cobj,Local<Object>& self){
			if(name==L"onDropFile"){
				return self->GetHiddenValue(String::New("onDropFile"));
			}else if(name==L"onDraw"){
				return self->GetHiddenValue(String::New("onDraw"));
			}else if(name==L"onScroll"){
				return self->GetHiddenValue(String::New("onScroll"));
			}
			return Undefined();
		}
		static void set(cs::String& name,BView* cobj,Local<Value>& value,Local<Object>& self){
			if(name==L"onDropFile"){
				DragAcceptFiles(*cobj,value->IsFunction());
				SetWndCallback(onDropFile,OnDropFiles,View_onDropFile);
			}else if(name==L"onDraw"){
				SetWndCallback(onDraw,OnDraw,View_onDraw);
			}else if(name==L"onScroll"){
				SetWndCallback(onScroll,OnScroll,View_onScroll);
			}
		}
		static void on_create(Handle<Object>& self,BView* cobj){
			JsWnd::on_create(self,(BIWnd*)cobj);
			//cobj->OnScroll.Add(&View_onScroll);
			//cobj->OnDraw.Add(&View_onDraw);
			//cobj->OnDropFiles.Add(&View_onDropFile);
		}
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			SET_CLA_FUNC(create);
			SET_CLA_ACCESSOR(onDropFile);
			SET_CLA_ACCESSOR(onScroll);
			SET_CLA_ACCESSOR(onDraw);
		}
		static void Load(Handle<Object>& glb){
			JsHandleObject::Load(glb,L"CView",&GetEnv()->GetTemplate(TEMPLATE_ID_WND));
		}
	};
	//*]}//*
	class BFrame : public BWnd<cs::Frame>{};
	//*,{
	//	"type":"class",
	//	"name":"CFrame",
	//	"text":"CFrame 封装一个顶级窗口对象，顶级窗口直接显示在屏幕上，而不是作为子窗口显示在父窗口上，通常的主窗口都是顶级窗口。CFrame 继承自 CView。",
	//	"member":[//*
	class JsFrame : public JsWndObject<BFrame,JsFrame,TEMPLATE_ID_FRAME>{
		//*{
		//	"type":"function",
		//	"name":"create([parent],[quit],[show])",
		//	"text":"生成窗口。",
		//	"param":[
		//		{
		//			"type":"integer|CWnd",
		//			"name":"[parent]",
		//			"text":"指定窗口的父窗口句柄，获取一个窗口的句柄调用 handle 函数。注意：顶级窗口也可以有父窗口，一个 CFrame 对象一定是一个顶级窗口，它可以有父窗口，也可以没有父窗口（parent设为0）。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[quit]",
		//			"text":"当关闭窗口的时候，是否退出应用程序，缺省是退出即 true。如果一个应用程序有多个 CFrame 对象，一般只有主窗口关闭时，才应该退出程序，其它 CFrame 的这个参数应该设为 false。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[show]",
		//			"text":"窗口的显示状态：0：隐藏，窗口不可见；1：正常显示状态；2：最小化；3：最大化。这个参数很少使用，窗口的显示状态一般通过 CreateParam 的 style 参数设定。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> create(const Arguments& args){
			HandleScope store;
			while(true){
				cs::Frame* cobj;
				Local<Object> self = args.This();
				if(!GetCHandle(cobj,self)) break;

				HWND parent = GET_ARGS_INT2(0,0,HWND);
				if(IsTemplate(args[0],TEMPLATE_ID_WND)){
					cs::IWnd* wnd;
					if(GetCHandle(wnd,args[0]->ToObject())){
						parent = wnd->Handle();
					}
				}

				int show = GET_ARGS_INT(2,1);
				bool quit = GET_ARGS_VALUE(1,true,Boolean);
				if(!cobj->Create(parent,show,quit)) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"quit()",
		//	"text":"退出应用程序，这个函数会向消息循环发送 WM_QUIT 消息。如果生成窗口时设置了 quit 属性，当关闭窗口时，这个函数会被自动调用。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> quit(const Arguments& args){
			HandleScope store;
			while(true){
				cs::Frame* cobj;
				Local<Object> self = args.This();
				if(!GetCHandle(cobj,self)) break;

				cobj->Quit();
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setIcon(icon,[big])",
		//	"text":"设置窗口的图标，每个窗口都有一个标题栏的小图标和任务栏的大图标。窗口原来的图标会被销毁（如果能够被销毁的话，系统资源图标无法销毁）",
		//	"param":[
		//		{
		//			"type":"CIcon",
		//			"name":"icon",
		//			"text":"一个有效的图标对象。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[big]",
		//			"text":"设置标题栏的小图标还是任务栏的大图标，缺省是 false，小图标。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> setIcon(const Arguments& args){
			HandleScope store;
			while(true){
				cs::Frame* cobj;
				Local<Object> self = args.This();
				if(!GetCHandle(cobj,self)) break;

				if(args.Length()<1) break;
				cs::Icon* picon;
				if(IsTemplate(args[0],TEMPLATE_ID_ICON)){
					if(!GetCHandle(picon,args[0]->ToObject())) break;
				}
				bool big = GET_ARGS_VALUE(1,0,Boolean);
				HICON hicon = cobj->SetIcon(*picon,big);
				picon->Detach();
				picon->Attach(hicon);
				return True();
			}
			return Undefined();
		}
	public:
		static void on_create(Handle<Object>& self,BFrame* cobj){
			JsView::on_create(self,(BView*)cobj);
		}
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			SET_CLA_FUNC(create);
			SET_CLA_FUNC(setIcon);
		}
		static void Load(Handle<Object>& glb){
			JsHandleObject::Load(glb,L"CFrame",&GetEnv()->GetTemplate(TEMPLATE_ID_VIEW));
		}
	};
	//*]}//*
	class BICtrl : public BWnd<cs::ICtrl>{};
	void __stdcall Ctrl_onNotify(cs::Message* msg,cs::ICtrl* cobj){
		LPNMHDR pnh = (LPNMHDR)msg->lParam;
		HandleScope store;
		Handle<Function> func;
		Persistent<Object>* ps = GetFuncFromWnd(cobj,"onNotify",func);
		if(ps){
			Handle<Value> argv[2];
			argv[0] = Uint32::New(pnh->code);
			argv[1] = Uint32::New(msg->lParam);
			Handle<Value> rst = CallFunc(*ps,func,2,argv);
			if(!rst->IsUndefined()) msg->Result = rst->Uint32Value();
		}
	}
	void __stdcall Ctrl_onColor(cs::Message* msg,cs::ICtrl* cobj){
		HandleScope store;
		Persistent<Object>* ps = (Persistent<Object>*)cobj->UserDataList[0];
		if(ps==NULL) return;
		Handle<Value> prop = (*ps)->Get(String::New("onColor"));
		if(prop->IsFunction()){
			Handle<Function> func = Handle<Function>::Cast(prop);
			Handle<Value> argv[1];
			CallFunc(*ps,func,0,argv);
		}
	}
	//*,{
	//	"type":"class",
	//	"name":"CCtrl",
	//	"text":"CCtrl 封装一个 Windows 标准控件，CCtrl 继承自 CWnd。",
	//	"member":[//*
	class JsICtrl : public JsWndObject<BICtrl,JsICtrl,TEMPLATE_ID_CTRL>{
		//*{
		//	"type":"function",
		//	"name":"create(parent)",
		//	"text":"生成控件，必须指定父窗口，否则函数失败。",
		//	"param":[
		//		{
		//			"type":"integer|CWnd",
		//			"name":"parent",
		//			"text":"指定窗口的父窗口句柄，获取一个窗口的句柄调用 handle 函数。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> create(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				cs::ICtrl* cobj;
				Local<Object> self = args.This();
				if(!GetCHandle(cobj,self)) break;
				HWND parent = GET_ARGS_INT2(0,0,HWND);
				if(IsTemplate(args[0],TEMPLATE_ID_WND)){
					cs::IWnd* wnd;
					if(GetCHandle(wnd,args[0]->ToObject())){
						parent = wnd->Handle();
					}
				}

				if(!cobj->Create(parent)) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"onNotify(code,lParam)",
		//	"text":"这是一个回调函数，响应窗口的 WM_NOTIFY 消息。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"code",
		//			"text":"这个参数一般表示 notify 消息的类型，不同的控件有不同的值和意义。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"lParam",
		//			"text":"消息的 lParam 参数，notify 消息的信息都保存在这个参数中，具体的意义和结构在不同的控件和消息中是不同的。"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"如果返回 undefined，相当于没有返回值，否则返回值会作为一个整数返回给系统。"
		//	}
		//}//*
	public:
		static inline void on_create(Handle<Object>& self,BICtrl* cobj){
			JsWnd::on_create(self,(BIWnd*)cobj);
			cobj->OnNotify.BindStd(&Ctrl_onNotify);
			//cobj->OnColor.BindStd(&Ctrl_onColor);
		}
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			SET_CLA_FUNC(create);
		}
		static void Load(Handle<Object>& glb){
			JsHandleObject::Load(glb,L"CCtrl",&GetEnv()->GetTemplate(TEMPLATE_ID_WND));
		}
	};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CLabel",
	//	"text":"CLabel 封装一个 Windows 标准 Static 控件，CLabel 继承自 CCtrl。",
	//	"member":[//*
	class BLabel : public BWnd<cs::Label>{};
	class JsLabel : public JsWndObject<BLabel,JsLabel,TEMPLATE_ID_LABEL>{
	public:
		static void on_create(Handle<Object>& self,BLabel* cobj){
			JsICtrl::on_create(self,(BICtrl*)cobj);
		}
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
		}
	};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CButton",
	//	"text":"CButton 封装一个 Windows 标准 Button（按钮） 控件，CButton 继承自 CCtrl。",
	//	"member":[//*
	class BButton : public BWnd<cs::Button>{};
	class JsButton : public JsWndObject<BButton,JsButton,TEMPLATE_ID_BUTTON>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CGroupBox",
	//	"text":"CGroupBox 封装一个 Windows 标准 Button（但是设置了 BS_GROUPBOX 风格） 控件，CGroupBox 继承自 CCtrl。",
	//	"member":[//*
	class BGroupBox : public BWnd<cs::GroupBox>{};
	class JsGroupBox : public JsWndObject<BGroupBox,JsGroupBox,TEMPLATE_ID_GROUPBOX>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CCheckBox",
	//	"text":"CCheckBox 封装一个 Windows 标准 Button（但是设置了 BS_AUTOCHECKBOX 风格） 控件，CCheckBox 继承自 CCtrl。",
	//	"member":[//*
	class BCheckBox : public BWnd<cs::CheckBox>{};
	class JsCheckBox : public JsWndObject<BCheckBox,JsCheckBox,TEMPLATE_ID_CHECKBOX>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CRadio",
	//	"text":"CRadio 封装一个 Windows 标准 Button（但是设置了 BS_AUTORADIOBUTTON 风格） 控件，CRadio 继承自 CCtrl。",
	//	"member":[//*
	class BRadio : public BWnd<cs::Radio>{};
	class JsRadio : public JsWndObject<BRadio,JsRadio,TEMPLATE_ID_RADIO>{};
	//*]}//*
	class BEdit : public BWnd<cs::Edit>{};
	void __stdcall Edit_onTextChange(cs::Edit* cobj){
		HandleScope store;
		Handle<Function> func;
		Persistent<Object>* ps = GetFuncFromWnd(cobj,"onTextChange",func);
		if(!ps) return;
		Handle<Value> argv[1];
		CallFunc(*ps,func,0,argv);
	}
	//*,{
	//	"type":"class",
	//	"name":"CEdit",
	//	"text":"CEdit 封装一个 Windows 标准编辑控件，CEdit 继承自 CCtrl。",
	//	"member":[//*
	class JsEdit : public JsWndObject<BEdit,JsEdit,TEMPLATE_ID_EDIT>{
	public:
		//*{
		//	"type":"property",
		//	"name":"setMultiLine([multiline])",
		//	"objtype":"function",
		//	"text":"设置编辑控件的多行或者单行属性。",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"[multiline]",
		//			"text":"多行还是单行，缺省是多行 true。"
		//		}
		//	],
		//	"return":{
		//		"type":"undefined",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> setMultiLine(const Arguments& args){
			HandleScope stack;
			while(true){
				BEdit* cobj;
				if(!GetCHandle(cobj,args.This())) break;
				bool ml = GET_ARGS_VALUE(0,true,Boolean);
				if(cobj->Handle()){
					LONG style = cobj->GetWindowParam(GWL_STYLE);
					style |= ES_AUTOVSCROLL|ES_AUTOHSCROLL|ES_MULTILINE|WS_VSCROLL;
					if(ml){
						style -= ES_AUTOHSCROLL;
					}else{
						style -= ES_AUTOVSCROLL|WS_VSCROLL|ES_MULTILINE;
					}
					cobj->SetWindowParam(GWL_STYLE,style);
				}else{
					cobj->Param->Style |= ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_MULTILINE|WS_VSCROLL;
					if(ml){
						cobj->Param->Style -= ES_AUTOHSCROLL;
					}else{
						cobj->Param->Style -= ES_AUTOVSCROLL|ES_MULTILINE|WS_VSCROLL;
					}
				}
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"property",
		//	"name":"onTextChange()",
		//	"objtype":"function",
		//	"text":"这是一个回调函数，当 CEdit 控件的文本发生变化后，这个函数被呼叫。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"undefined",
		//		"text":"这个函数没有返回值。"
		//	}
		//}//*
		static Handle<Value> get(cs::String& name,BEdit* cobj,Local<Object>& self){
			if(name==L"onTextChange"){
				return self->GetHiddenValue(String::New("onTextChange"));
			}else if(name==L"readOnly"){
				return Bool(cobj->GetWindowParam(GWL_STYLE)&ES_READONLY);
			}
			return Undefined();
		}
		static void set(cs::String& name,BEdit* cobj,Local<Value>& value,Local<Object>& self){
			if(name==L"onTextChange"){
				self->SetHiddenValue(String::New("onTextChange"),value);
				if(value->IsFunction()){
					cobj->OnChange.BindStd(&Edit_onTextChange);
				}else{
					cobj->OnChange.Empty();
				}
			}else if(name==L"readOnly"){
				cobj->SetReadOnly(value->BooleanValue());
			}
		}
		static inline void on_create(Handle<Object>& self,BEdit* cobj){
			JsICtrl::on_create(self,(BICtrl*)cobj);
		}
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			SET_CLA_FUNC(setMultiLine);
			SET_CLA_ACCESSOR(onTextChange);
			SET_CLA_ACCESSOR(readOnly);
		}
	};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CDateTimePicker",
	//	"text":"CDateTimePicker 封装一个 Windows 标准 DateTimePicker 控件，CDateTimePicker 继承自 CCtrl。",
	//	"member":[//*
	class BDateTimePicker : public BWnd<cs::DateTimePicker>{};
	class JsDateTimePicker : public JsWndObject<BDateTimePicker,JsDateTimePicker,TEMPLATE_ID_DATETIMEPICKER>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CMonthCalendar",
	//	"text":"CMonthCalendar 封装一个 Windows 标准 MonthCalendar 控件，CMonthCalendar 继承自 CCtrl。",
	//	"member":[//*
	class BMonthCalendar : public BWnd<cs::MonthCalendar>{};
	class JsMonthCalendar : public JsWndObject<BMonthCalendar,JsMonthCalendar,TEMPLATE_ID_MONTHCALENDAR>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CListBox",
	//	"text":"CListBox 封装一个 Windows 标准 ListBox 控件，CListBox 继承自 CCtrl。",
	//	"member":[//*
	class BListBox : public BWnd<cs::ListBox>{};
	class JsListBox : public JsWndObject<BListBox,JsListBox,TEMPLATE_ID_LISTBOX>{
		//*{
		//	"type":"function",
		//	"name":"addItem(str,[index])",
		//	"text":"添加一个条目。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"要添加的字串。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[index]",
		//			"text":"添加的位置，0 表示插入到最前面，-1 或者大于最大索引，会插入到末尾。"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"成功返回插入的位置，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> addItem(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				Local<Object> self = args.This();
				cs::ListBox* handle;
				if(!GetCHandle(handle,self)) break;
				cs::String str;
				GetString(args[0],str);
				int index = -1;
				if(args.Length()>1){
					index = args[1]->Uint32Value();
				}
				int r = handle->AddString(str,index);
				RET(r,Int32);
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"deleteItem(index)",
		//	"text":"删除一个条目。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"删除指定索引的条目。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，如果索引超界，函数失败，返回 undefined。"
		//	}
		//}//*
		static Handle<Value> deleteItem(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				Local<Object> self = args.This();
				cs::ListBox* handle;
				if(!GetCHandle(handle,self)) break;
				int index = args[1]->Uint32Value();
				bool r = handle->DeleteString(index);
				return Bool(r);
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"clear()",
		//	"text":"清除所有项。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> clear(const Arguments& args){
			HandleScope store;
			while(true){
				Local<Object> self = args.This();
				cs::ListBox* handle;
				if(!GetCHandle(handle,self)) break;
				handle->Clear();
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setItem(str,index)",
		//	"text":"设置指定项的文本。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"要设置的文本。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"要设置的项的索引位置。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> setItem(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<2) break;
				Local<Object> self = args.This();
				cs::ListBox* handle;
				if(!GetCHandle(handle,self)) break;
				cs::String str;
				GetString(args[0],str);
				int index = args[1]->Uint32Value();
				bool r = handle->SetText(index,str);
				if(!r) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getItem(index)",
		//	"text":"或者指定项的文本。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"要获取的项的位置索引。"
		//		}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"成功返回一个字串，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> getItem(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				Local<Object> self = args.This();
				cs::ListBox* handle;
				if(!GetCHandle(handle,self)) break;
				int index = args[0]->Uint32Value();
				cs::String str;
				bool r = LB_ERR!=handle->GetText(index,str);
				if(!r) break;
				return store.Close(String::New((uint16_t*)str.Handle()));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getCount()",
		//	"text":"获取项的总数。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"返回一个数字，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> getCount(const Arguments& args){
			HandleScope store;
			while(true){
				Local<Object> self = args.This();
				cs::ListBox* handle;
				if(!GetCHandle(handle,self)) break;
				int r = handle->GetCount();
				RET(r,Int32);
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getSelectItem()",
		//	"text":"获取当前选中项的索引。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"integer|array",
		//		"text":"单选情况，返回选中项的索引，如果没有选中项，返回 -1，多选情况，返回一个数组，数组的元素是选中项的索引，如果函数失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> getSelectItem(const Arguments& args){
			HandleScope store;
			while(true){
				Local<Object> self = args.This();
				cs::ListBox* handle;
				if(!GetCHandle(handle,self)) break;
				if(!handle->Handle()) break;
				DWORD style = handle->GetWindowParam(GWL_STYLE);
				if(style&LBS_MULTIPLESEL){
					int count = handle->GetCount();
					Handle<Array> selA = Array::New();
					if(count==0) return store.Close(selA);
					cs::Memory<int> sels;
					sels.SetLength(count);
					count = handle->GetSelItems(&sels);
					for(int i=0;i<count;i++){
						selA->Set(i,Uint32::New(sels[i]));
					}
					return store.Close(selA);
				}else{
					RET(handle->GetSelItem(),Int32);
				}
				break;
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setSelectItem(index,[index2])",
		//	"text":"设置指定项, 或者范围被选中，范围仅适用于多选列表框。",
		//	"param":[
		//		{
		//			"type":"integer|string",
		//			"name":"index",
		//			"text":"要设置选中的项索引，如果此项为 -1 单选列表框清除选择项，多选列表框无效。这个参数还可以是一个字串，指定包含此字串的项被选中，此时第二个参数是搜索字串的起始位置。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[index2]",
		//			"text":"这个参数只有列表框是多选模式时，才会用到，index 用于指定列表框选择项的下限，index2 用于指定选择项范围的上限；如果 index2 小于 index 则清除这个范围内的选中状态。对于多选情况，如果要选中一个项，不要传第二个参数，因为相同的两个索引，表示清除这个项。"
		//		}
		//	],
		//	"return":{
		//		"type":"integer|boolean",
		//		"text":"单选的情况，返回选择项的索引（-1表示没有选中），多选的情况返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> setSelectItem(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				Local<Object> self = args.This();
				cs::ListBox* handle;
				if(!GetCHandle(handle,self)) break;
				if(!handle->Handle()) break;

				DWORD style = handle->GetWindowParam(GWL_STYLE);
				if(style&LBS_MULTIPLESEL){
					if(args[0]->IsString()) break;
					uint index = GET_ARGS_INT(0,0);
					if(args.Length()<2||args[2]->IsUndefined()){
						handle->SetSelItems(index,index+1);
						handle->SetSelItems(index+1,index+1);
					}else{
						uint index2 = GET_ARGS_INT(1,-1);
						handle->SetSelItems(index,index2);
					}
				}else{
					int seli;
					if(args[0]->IsString()){
						cs::String str;
						GetString(args[0],str);
						int start = GET_ARGS_INT(1,-1);
						seli = handle->SetSelItem(str,start);
					}else{
						seli = GET_ARGS_INT(0,0);
						handle->SetSelItem(seli);
					}
					return store.Close(Int32::New(seli));
				}
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"find(str,[start])",
		//	"text":"查找一个字串，这个查找不区分大小写，只要项文本开头包含这个字串就可以。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"要查找的文本。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[start]",
		//			"text":"从这一项开始查找，默认值是 0（从头查找）。注意这和 Windows 默认的搜索方式有区别，Windows 默认是 start 项是不搜索的，如果从开头搜索应该传 -1."
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"返回符合条件的项的位置索引，如果没有找到，返回 -1，如果函数失败，返回 undefined。"
		//	}
		//}//*
		static Handle<Value> find(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				Local<Object> self = args.This();
				cs::ListBox* handle;
				if(!GetCHandle(handle,self)) break;
				cs::String str;
				GetString(args[0],str);
				int index = GET_ARGS_INT(1,0) - 1;
				int r = handle->FindString(str,index);
				RET(r,Int32);
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"findExact(str,[start])",
		//	"text":"精确查找一个字串，项文本必须和搜索的文本相同，但是不区分大小写",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"要查找的文本。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[start]",
		//			"text":"从这一项开始查找，默认值是 0（从头查找）。注意这和 Windows 默认的搜索方式有区别，Windows 默认是 start 项是不搜索的，如果从开头搜索应该传 -1."
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"返回符合条件的项的位置索引，如果没有找到，返回 -1，如果函数失败，返回 undefined。"
		//	}
		//}//*
		static Handle<Value> findExact(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				Local<Object> self = args.This();
				cs::ListBox* handle;
				if(!GetCHandle(handle,self)) break;
				cs::String str;
				GetString(args[0],str);
				int index = GET_ARGS_INT(1,0) - 1;
				int r = handle->FindExactString(str,index);
				RET(r,Int32);
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"isItemSelected(index)",
		//	"text":"指定项是否被选中状态。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"要查询的项索引."
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"是返回 true，否返回 false，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> isItemSelected(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				Local<Object> self = args.This();
				cs::ListBox* handle;
				if(!GetCHandle(handle,self)) break;
				if(handle->Handle()==0) break;

				uint index = GET_ARGS_INT(0,0);
				bool r = handle->ItemIsSelect(index);
				return Bool(r);
			}
			return Undefined();
		}
		//,{
		//	"type":"function",
		//	"name":"ensureVisible(index)",
		//	"text":"滚动下拉框，让指定索引项显示出来。调用这个函数前，必须已经显示下拉框。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"要显示的项索引。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//
		/*static Handle<Value> ensureVisible(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				Local<Object> self = args.This();
				cs::ListBox* handle;
				if(!GetCHandle(handle,self)) break;
				int index = args[0]->Uint32Value();
				bool r = handle->EnsureVisible(index);
				return Bool(r);
			}
			return Undefined();
		}*/
		public:
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Local<ObjectTemplate> func = ft->PrototypeTemplate();
			SET_CLA_FUNC(addItem);
			SET_CLA_FUNC(deleteItem);
			SET_CLA_FUNC(clear);
			SET_CLA_FUNC(setItem);
			SET_CLA_FUNC(getItem);
			SET_CLA_FUNC(getCount);
			SET_CLA_FUNC(getSelectItem);
			SET_CLA_FUNC(setSelectItem);
			SET_CLA_FUNC(find);
			SET_CLA_FUNC(findExact);
			SET_CLA_FUNC(isItemSelected);
			//SET_CLA_FUNC(ensureVisible);
		}
	};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CComboBox",
	//	"text":"CComboBox 封装一个 Windows 标准 ComboBox 控件，CComboBox 继承自 CCtrl。",
	//	"member":[//*
	class BComboBox : public BWnd<cs::ComboBox>{};
	class JsComboBox : public JsWndObject<BComboBox,JsComboBox,TEMPLATE_ID_COMBOBOX>{
	public:
		//*{
		//	"type":"function",
		//	"name":"addItem(str,[index])",
		//	"text":"添加一个条目。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"要添加的字串。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[index]",
		//			"text":"添加的位置，0 表示插入到最前面，-1 或者大于最大索引，会插入到末尾。"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"成功返回插入的位置，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> addItem(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				Local<Object> self = args.This();
				cs::ComboBox* handle;
				if(!GetCHandle(handle,self)) break;
				cs::String str;
				GetString(args[0],str);
				int index = -1;
				if(args.Length()>1){
					index = args[1]->Uint32Value();
				}
				int r = handle->AddString(str,index);
				RET(r,Int32);
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"deleteItem(index)",
		//	"text":"删除一个条目。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"删除指定索引的条目。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，如果索引超界，函数失败，返回 undefined。"
		//	}
		//}//*
		static Handle<Value> deleteItem(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				Local<Object> self = args.This();
				cs::ComboBox* handle;
				if(!GetCHandle(handle,self)) break;
				int index = args[1]->Uint32Value();
				bool r = handle->DeleteString(index);
				return Bool(r);
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"clear()",
		//	"text":"清除所有项。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> clear(const Arguments& args){
			HandleScope store;
			while(true){
				Local<Object> self = args.This();
				cs::ComboBox* handle;
				if(!GetCHandle(handle,self)) break;
				handle->Clear();
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setItem(str,index)",
		//	"text":"设置指定项的文本。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"要设置的文本。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"要设置的项的索引位置。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> setItem(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<2) break;
				Local<Object> self = args.This();
				cs::ComboBox* handle;
				if(!GetCHandle(handle,self)) break;
				cs::String str;
				GetString(args[0],str);
				int index = args[1]->Uint32Value();
				bool r = handle->SetItemText(index,str);
				if(!r) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getItem(index)",
		//	"text":"或者指定项的文本。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"要获取的项的位置索引。"
		//		}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"成功返回一个字串，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> getItem(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				Local<Object> self = args.This();
				cs::ComboBox* handle;
				if(!GetCHandle(handle,self)) break;
				int index = args[0]->Uint32Value();
				cs::String str;
				bool r = handle->GetItemText(index,str);
				if(!r) break;
				return store.Close(String::New((uint16_t*)str.Handle()));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getCount()",
		//	"text":"获取项的总数。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"返回一个数字，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> getCount(const Arguments& args){
			HandleScope store;
			while(true){
				Local<Object> self = args.This();
				cs::ComboBox* handle;
				if(!GetCHandle(handle,self)) break;
				int r = handle->GetCount();
				RET(r,Int32);
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getCurSelect()",
		//	"text":"获取当前选中项的索引。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"返回选中项的索引，如果没有选中项，返回 -1，如果函数失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> getCurSelect(const Arguments& args){
			HandleScope store;
			while(true){
				Local<Object> self = args.This();
				cs::ComboBox* handle;
				if(!GetCHandle(handle,self)) break;
				int r = handle->GetCurSelect();
				RET(r,Int32);
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setCurSelect(index)",
		//	"text":"设置指定项为被选中。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"要设置选中的项索引。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> setCurSelect(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				Local<Object> self = args.This();
				cs::ComboBox* handle;
				if(!GetCHandle(handle,self)) break;
				int index = args[0]->Uint32Value();
				int r = handle->SetCurSelect(index);
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"find(str,[start])",
		//	"text":"查找一个字串，这个查找不区分大小写，只要项文本开头包含这个字串就可以。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"要查找的文本。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[start]",
		//			"text":"从这一项开始查找，默认值是 0（从头查找）。注意这和 Windows 默认的搜索方式有区别，Windows 默认是 start 项是不搜索的，如果从开头搜索应该传 -1."
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"返回符合条件的项的位置索引，如果没有找到，返回 -1，如果函数失败，返回 undefined。"
		//	}
		//}//*
		static Handle<Value> find(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				Local<Object> self = args.This();
				cs::ComboBox* handle;
				if(!GetCHandle(handle,self)) break;
				cs::String str;
				GetString(args[0],str);
				int index = GET_ARGS_INT(1,0) - 1;
				int r = handle->FindString(str,index);
				RET(r,Int32);
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"findExact(str,[start])",
		//	"text":"精确查找一个字串，项文本必须和搜索的文本相同，但是不区分大小写",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"要查找的文本。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[start]",
		//			"text":"从这一项开始查找，默认值是 0（从头查找）。注意这和 Windows 默认的搜索方式有区别，Windows 默认是 start 项是不搜索的，如果从开头搜索应该传 -1."
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"返回符合条件的项的位置索引，如果没有找到，返回 -1，如果函数失败，返回 undefined。"
		//	}
		//}//*
		static Handle<Value> findExact(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				Local<Object> self = args.This();
				cs::ComboBox* handle;
				if(!GetCHandle(handle,self)) break;
				cs::String str;
				GetString(args[0],str);
				int index = GET_ARGS_INT(1,0) - 1;
				int r = handle->FindExactString(str,index);
				RET(r,Int32);
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"dropDown()",
		//	"text":"展开下拉框。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> dropDown(const Arguments& args){
			HandleScope store;
			while(true){
				Local<Object> self = args.This();
				cs::ComboBox* handle;
				if(!GetCHandle(handle,self)) break;
				bool show = true;
				if(args.Length()>0){
					show = args[0]->BooleanValue();
				}
				handle->ShowDropDown(show);
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"ensureVisible(index)",
		//	"text":"滚动下拉框，让指定索引项显示出来。调用这个函数前，必须已经显示下拉框。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"要显示的项索引。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> ensureVisible(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				Local<Object> self = args.This();
				cs::ComboBox* handle;
				if(!GetCHandle(handle,self)) break;
				int index = args[0]->Uint32Value();
				bool r = handle->EnsureVisible(index);
				return Bool(r);
			}
			return Undefined();
		}
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Local<ObjectTemplate> func = ft->PrototypeTemplate();
			SET_CLA_FUNC(addItem);
			SET_CLA_FUNC(deleteItem);
			SET_CLA_FUNC(clear);
			SET_CLA_FUNC(setItem);
			SET_CLA_FUNC(getItem);
			SET_CLA_FUNC(getCount);
			SET_CLA_FUNC(getCurSelect);
			SET_CLA_FUNC(setCurSelect);
			SET_CLA_FUNC(find);
			SET_CLA_FUNC(findExact);
			SET_CLA_FUNC(dropDown);
			SET_CLA_FUNC(ensureVisible);
		}
	};
	//*],"source":"d:\\SoftProject\\GitLib\\jsuser\\example\\combobox.jsuser"}//*

	//*,{
	//	"type":"class",
	//	"name":"CComboBoxEx",
	//	"text":"CComboBoxEx 封装一个 Windows 标准 ComboBoxEx 控件，CComboBoxEx 继承自 CCtrl。",
	//	"member":[//*
	class BComboBoxEx : public BWnd<cs::ComboBoxEx>{};
	class JsComboBoxEx : public JsWndObject<BComboBoxEx,JsComboBoxEx,TEMPLATE_ID_COMBOBOXEX>{
	public:
		//*{
		//	"type":"function",
		//	"name":"clear()",
		//	"text":"清楚所有项。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> clear(const Arguments& args){
			HandleScope store;
			while(true){
				Local<Object> self = args.This();
				cs::ComboBoxEx* handle;
				if(!GetCHandle(handle,self)) break;
				handle->Clear();
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getCount()",
		//	"text":"获取项的总数。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"返回一个数字，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> getCount(const Arguments& args){
			HandleScope store;
			while(true){
				Local<Object> self = args.This();
				cs::ComboBoxEx* handle;
				if(!GetCHandle(handle,self)) break;
				int r = handle->GetCount();
				RET(r,Int32);
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getCurSelect()",
		//	"text":"获取当前选中项的索引。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"返回选中项的索引，如果没有选中项，返回 -1，如果函数失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> getCurSelect(const Arguments& args){
			HandleScope store;
			while(true){
				Local<Object> self = args.This();
				cs::ComboBoxEx* handle;
				if(!GetCHandle(handle,self)) break;
				int r = handle->GetCurSelect();
				RET(r,Int32);
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setCurSelect(index)",
		//	"text":"设置指定项为被选中。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"要设置选中的项索引。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> setCurSelect(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				Local<Object> self = args.This();
				cs::ComboBoxEx* handle;
				if(!GetCHandle(handle,self)) break;
				int index = args[0]->Uint32Value();
				int r = handle->SetCurSelect(index);
				return True();
			}
			return Undefined();
		}
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Local<ObjectTemplate> func = ft->PrototypeTemplate();
			SET_CLA_FUNC(clear);
			SET_CLA_FUNC(getCount);
			SET_CLA_FUNC(getCurSelect);
			SET_CLA_FUNC(setCurSelect);
		}

	};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CTreeView",
	//	"text":"CTreeView 封装一个 Windows 标准 TreeView 控件，CTreeView 继承自 CCtrl。",
	//	"member":[//*
	class BTreeView : public BWnd<cs::TreeView>{};
	class JsTreeView : public JsWndObject<BTreeView,JsTreeView,TEMPLATE_ID_TREEVIEW>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CToolBar",
	//	"text":"CToolBar 封装一个 Windows 标准 ToolBar 控件，CToolBar 继承自 CCtrl。",
	//	"member":[//*
	class BToolBar : public BWnd<cs::ToolBar>{};
	class JsToolBar : public JsWndObject<BToolBar,JsToolBar,TEMPLATE_ID_TOOLBAR>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CProgressBar",
	//	"text":"CProgressBar 封装一个 Windows 标准 ProgressBar 控件，CProgressBar 继承自 CCtrl。",
	//	"member":[//*
	class BProgressBar : public BWnd<cs::ProgressBar>{};
	class JsProgressBar : public JsWndObject<BProgressBar,JsProgressBar,TEMPLATE_ID_PROGRESSBAR>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CToolTip",
	//	"text":"CToolTip 封装一个 Windows 标准 ToolTip 控件，CToolTip 继承自 CCtrl。",
	//	"member":[//*
	class BToolTip : public BWnd<cs::ToolTip>{};
	class JsToolTip : public JsWndObject<BToolTip,JsToolTip,TEMPLATE_ID_TOOLTIP>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CTabPage",
	//	"text":"CTabPage 封装一个 Windows 标准 Tab 控件，CTabPage 继承自 CCtrl。",
	//	"member":[//*
	class BTabPage : public BWnd<cs::TabPage>{};
	class JsTabPage : public JsWndObject<BTabPage,JsTabPage,TEMPLATE_ID_TABPAGE>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CHotKey",
	//	"text":"CHotKey 封装一个 Windows 标准 HotKey 控件，CHotKey 继承自 CCtrl。",
	//	"member":[//*
	class BHotKey : public BWnd<cs::HotKey>{};
	class JsHotKey : public JsWndObject<BHotKey,JsHotKey,TEMPLATE_ID_HOTKEY>{};
	//*]}//*
	//*,{
	//	"type":"class",
	//	"name":"CListView",
	//	"text":"CListView 封装一个 Windows 标准 ListView 控件，CListView 继承自 CCtrl。",
	//	"member":[//*
	class BListView : public BWnd<cs::ListView>{};
	void __stdcall ListView_onNotify(cs::Message* msg,cs::ICtrl* cobj){
		LPNMHDR pnh = (LPNMHDR)msg->lParam;
		HandleScope store;
		Handle<Function> func;
		Persistent<Object>* ps = GetFuncFromWnd(cobj,"onNotify",func);
		if(ps){
			Handle<Value> argv[2];
			argv[0] = Uint32::New(pnh->code);
			argv[1] = Uint32::New(msg->lParam);
			CallFunc(*ps,func,2,argv);
		}
		LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(msg->lParam);
		if(pNMCD->hdr.code==NM_CUSTOMDRAW){
			LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pNMCD;
			if(lplvcd->nmcd.dwDrawStage==CDDS_PREPAINT){
				msg->Result = CDRF_NOTIFYITEMDRAW;	//告诉控件需要自定义行显示，这会产生 CDDS_ITEMPREPAINT 消息
			}else if(lplvcd->nmcd.dwDrawStage==CDDS_ITEMPREPAINT){
				msg->Result = CDRF_NOTIFYSUBITEMDRAW; //告诉控件需要自定义单元格，这会产生 CDDS_SUBITEM 消息
			}else if(lplvcd->nmcd.dwDrawStage==(CDDS_ITEMPREPAINT | CDDS_SUBITEM)){
				Handle<Object> nmcd = Object::New();
				SET_PROP2(nmcd,iColumn,lplvcd->iSubItem,Uint32);
				SET_PROP2(nmcd,iItem,lplvcd->nmcd.dwItemSpec,Uint32);
				SET_PROP2(nmcd,fontColor,lplvcd->clrText,Uint32);
				SET_PROP2(nmcd,bkColor,lplvcd->clrTextBk,Uint32);

				Handle<Function> func;
				Persistent<Object>* ps = GetFuncFromWnd(cobj,"onCustomDraw",func);
				if(!ps) return;
				Handle<Value> argv[1];
				argv[0] = nmcd;
				CallFunc(*ps,func,1,argv);

				GET_PROP2(nmcd,iColumn,lplvcd->iSubItem,Uint32);
				GET_PROP2(nmcd,iItem,lplvcd->nmcd.dwItemSpec,Uint32);
				GET_PROP2(nmcd,fontColor,lplvcd->clrText,Uint32);
				GET_PROP2(nmcd,bkColor,lplvcd->clrTextBk,Uint32);
				msg->Result = CDRF_DODEFAULT;
			}
		}/*else if(pNMCD->hdr.code!=LVN_ITEMCHANGED){
			LPNMLISTVIEW pnm = (LPNMLISTVIEW)msg->lParam;
			if(pnm->uChanged&(LVIF_STATE)){
				Handle<Function> func;
				Persistent<Object>* ps = GetFuncFromWnd(cobj,"onItemChanged",func);
				if(!ps) return;
				Handle<Value> argv[1];
				Handle<Object> nmcd = Object::New();
				argv[0] = nmcd;
				SET_PROP2(nmcd,oldState,pnm->uOldState,Uint32);
				SET_PROP2(nmcd,iItem,pnm->iItem,Uint32);
				SET_PROP2(nmcd,newState,pnm->uNewState,Uint32);
				CallFunc(*ps,func,1,argv);
			}
		}*/
	}
	class JsListView : public JsWndObject<BListView,JsListView,TEMPLATE_ID_LISTVIEW>{
	public:
		//*{
		//	"type":"function",
		//	"name":"getColumnCount()",
		//	"text":"返回列数。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"成功返回列数，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> getColumnCount(const Arguments& args){
			HandleScope stack;
			while(true){
				Handle<Object> self = args.This();
				cs::ListView* cobj;
				if(!GetCHandle(cobj,self)) break;
				if(cobj->Handle()==0) break;

				return stack.Close(Uint32::New(cobj->GetColumnCount()));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getItemCount()",
		//	"text":"返回项总数。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"成功返回项数，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> getItemCount(const Arguments& args){
			HandleScope stack;
			while(true){
				Handle<Object> self = args.This();
				cs::ListView* cobj;
				if(!GetCHandle(cobj,self)) break;
				if(cobj->Handle()==0) break;

				return stack.Close(Uint32::New(cobj->GetItemCount()));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getItemText(iItem,int iColumn)",
		//	"text":"返回项的文本。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"iItem",
		//			"text":"返回项的项索引。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"iColumn",
		//			"text":"返回项的列索引。"
		//		}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"返回字串，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> getItemText(const Arguments& args){
			HandleScope stack;
			while(true){
				if(args.Length()<2) break;
				Handle<Object> self = args.This();
				cs::ListView* cobj;
				if(!GetCHandle(cobj,self)) break;
				if(cobj->Handle()==0) break;

				cs::String str;
				int index = GET_ARGS_INT(0,0);
				int column = GET_ARGS_INT(1,0);
				if(!cobj->GetItemText(str,index,column)) break;
				return stack.Close(NEW_WSTR(str.Handle()));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setItemText(iItem,int iColumn)",
		//	"text":"设置返回项的文本。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"iItem",
		//			"text":"返回项的项索引。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"iColumn",
		//			"text":"返回项的列索引。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> setItemText(const Arguments& args){
			HandleScope stack;
			while(true){
				if(args.Length()<3) break;
				Handle<Object> self = args.This();
				cs::ListView* cobj;
				if(!GetCHandle(cobj,self)) break;
				if(cobj->Handle()==0) break;

				cs::String str;
				GetString(args[0],str);
				int index = GET_ARGS_INT(1,0);
				int column = GET_ARGS_INT(2,0);
				if(!cobj->SetItemText(str,index,column)) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"deleteColumn(iColumn)",
		//	"text":"删除一列以及其子项。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"iColumn",
		//			"text":"要删除的列。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> deleteColumn(const Arguments& args){
			HandleScope stack;
			while(true){
				if(args.Length()<1) break;
				Handle<Object> self = args.This();
				cs::ListView* cobj;
				if(!GetCHandle(cobj,self)) break;
				if(cobj->Handle()==0) break;

				int index = GET_ARGS_INT(0,0);
				if(!cobj->DelColumn(index)) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"deleteItem(iItem)",
		//	"text":"删除一行。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"iItem",
		//			"text":"行索引。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> deleteItem(const Arguments& args){
			HandleScope stack;
			while(true){
				if(args.Length()<1) break;
				Handle<Object> self = args.This();
				cs::ListView* cobj;
				if(!GetCHandle(cobj,self)) break;
				if(cobj->Handle()==0) break;

				int index = GET_ARGS_INT(0,0);
				if(!cobj->DelItem(index)) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"addColumn(str,[index])",
		//	"text":"添加一列。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"列的名称。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[index]",
		//			"text":"列的位置，-1 和缺省表示添加到最后。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> addColumn(const Arguments& args){
			HandleScope stack;
			while(true){
				if(args.Length()<1) break;
				Handle<Object> self = args.This();
				cs::ListView* cobj;
				if(!GetCHandle(cobj,self)) break;
				if(cobj->Handle()==0) break;
				
				cs::String str;
				GetString(args[0],str);
				int width = GET_ARGS_INT(1,100);
				int index = GET_ARGS_INT(2,-1);
				if(!cobj->AddColumn(str,width,index)) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setCheck(index,[check])",
		//	"text":"设置一个含 CheckBox 风格的行的 Check 状态。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"指定索引的行。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[check]",
		//			"text":"Check 还是 uncheck，缺省是 true。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> setCheck(const Arguments& args){
			HandleScope stack;
			while(true){
				if(args.Length()<1) break;
				Handle<Object> self = args.This();
				cs::ListView* cobj;
				if(!GetCHandle(cobj,self)) break;
				if(cobj->Handle()==0) break;

				int index = GET_ARGS_INT(0,0);
				bool fCheck;
				if(args.Length()>1)
					fCheck = args[1]->BooleanValue();
				else
					fCheck = true;
				ListView_SetCheckState(*cobj,index,fCheck);
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getCheck(index)",
		//	"text":"获取指定索引行的 Check 状态。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"行索引。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 check 状态，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> getCheck(const Arguments& args){
			HandleScope stack;
			while(true){
				if(args.Length()<1) break;
				Handle<Object> self = args.This();
				cs::ListView* cobj;
				if(!GetCHandle(cobj,self)) break;
				if(cobj->Handle()==0) break;

				int index = GET_ARGS_INT(0,0);
				BOOL b = ListView_GetCheckState(*cobj,index);
				return Bool(b);
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setSelected(index)",
		//	"text":"设置指定行为选择。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"行索引。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true ，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> setSelected(const Arguments& args){
			HandleScope stack;
			while(true){
				if(args.Length()<1) break;
				Handle<Object> self = args.This();
				cs::ListView* cobj;
				if(!GetCHandle(cobj,self)) break;
				if(cobj->Handle()==0) break;

				int index = GET_ARGS_INT(0,0);
				DWORD set;
				if(args.Length()>1&&!args[1]->BooleanValue()){
					set = 0;
				}else{
					set = LVIS_SELECTED;
				}
				ListView_SetItemState(*cobj,index,set,LVIS_SELECTED);
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"isSelected(index)",
		//	"text":"指定行是否是选择状态，ListView 可以是多选。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"行索引。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"返回一个表示选择状态的布尔值，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> isSelected(const Arguments& args){
			HandleScope stack;
			while(true){
				if(args.Length()<1) break;
				Handle<Object> self = args.This();
				cs::ListView* cobj;
				if(!GetCHandle(cobj,self)) break;
				if(cobj->Handle()==0) break;

				int index = GET_ARGS_INT(0,0);
				DWORD set = ListView_GetItemState(*cobj,index,LVIS_SELECTED);
				return Bool(set);
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getSelected([start])",
		//	"text":"获取那些行是被选中的，start 是搜索它之后的行，所以可以把返回值作为参数，反复调用这个函数，获取所有选中行索引。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"[start]",
		//			"text":"从这个行之后搜索，缺省值是 -1（因为 0 是不搜索第一行的）。"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"返回选择行的索引，如果没搜索到，返回 -1，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> getSelected(const Arguments& args){
			HandleScope stack;
			while(true){
				Handle<Object> self = args.This();
				cs::ListView* cobj;
				if(!GetCHandle(cobj,self)) break;
				if(cobj->Handle()==0) break;
				
				int index = GET_ARGS_INT(0,-1);
				index = cobj->GetItemIndex(LVNI_SELECTED,index);
				return stack.Close(Uint32::New(index));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"addItem(text,[index])",
		//	"text":"添加一行。大量添加删除数据的时候，可以先隐藏当前控件的显示，完成后再显示出来，否则添加和删除可能极慢。",
		//	"param":[
		//		{
		//			"type":"string|array",
		//			"name":"text",
		//			"text":"如果这个参数是字串，函数添加一行，并且设置第一列的值是这个字串。如果这个参数是数组，添加一行，并且依次用数组的值设置每一列的文本。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[index]",
		//			"text":"添加的位置，-1 （缺省）表示添加到最后。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> addItem(const Arguments& args){
			HandleScope stack;
			while(true){
				if(args.Length()<1) break;
				Handle<Object> self = args.This();
				cs::ListView* cobj;
				if(!GetCHandle(cobj,self)) break;
				if(cobj->Handle()==0) break;

				int index = GET_ARGS_INT(1,-1);
				cs::String str;
				if(args[0]->IsArray()){
					Handle<Array> items = Handle<Array>::Cast(args[0]);
					int count = cs::Minimum(cobj->GetColumnCount(),(int)items->Length());
					if(count==0) break;
					GetString(items->Get(0),str);
					index = cobj->AddItem(str,index);
					for(int i=1;i<count;i++){
						GetString(items->Get(i),str);
						cobj->SetItemText(str,index,i);
					}
				}else{
					GetString(args[0],str);
					cobj->AddItem(str,index);
				}
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"property",
		//	"name":"onCustomDraw()",
		//	"objtype":"function",
		//	"text":"这是一个回调函数，当 CEdit 控件的文本发生变化后，这个函数被呼叫。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"undefined",
		//		"text":"这个函数没有返回值。"
		//	}
		//}
		//,{
		//	"type":"property",
		//	"name":"checkBox",
		//	"objtype":"boolean",
		//	"text":"控件是否在每行的第一列显示一个 CheckBox，缺省值 false。"
		//}
		//,{
		//	"type":"property",
		//	"name":"rowSelect",
		//	"objtype":"boolean",
		//	"text":"当行被选中的时候，是整行还是第一列显示选中，默认值是 false。改变这个属性，必须在控件已经创建完成后设置才有效。"
		//}
		//,{
		//	"type":"property",
		//	"name":"gridLine",
		//	"objtype":"boolean",
		//	"text":"是否显示网格线，默认值是 false。改变这个属性，必须在控件已经创建完成后设置才有效。"
		//}//*
		static Handle<Value> get(cs::String& name,BListView* cobj,Local<Object>& self){
			if(name==L"checkBox"){
				DWORD ex = cobj->GetWindowParam(GWL_EXSTYLE);
				return Bool(ex&LVS_EX_CHECKBOXES);
			}else if(name==L"rowSelect"){
				DWORD ex = cobj->GetWindowParam(GWL_EXSTYLE);
				return Bool(ex&LVS_EX_FULLROWSELECT);
			}else if(name==L"gridLine"){
				DWORD ex = cobj->GetWindowParam(GWL_EXSTYLE);
				return Bool(ex&LVS_EX_GRIDLINES);
			}
			return Undefined();
		}
		static void set(cs::String& name,BListView* cobj,Local<Value>& value,Local<Object>& self){
			if(name==L"checkBox"){
				cobj->SetExStyle(LVS_EX_CHECKBOXES,value->BooleanValue());
			}else if(name==L"rowSelect"){
				cobj->SetExStyle(LVS_EX_FULLROWSELECT,value->BooleanValue());
			}else if(name==L"gridLine"){
				cobj->SetExStyle(LVS_EX_GRIDLINES,value->BooleanValue());
			}else if(name==L"selected"){
			}
		}
		static inline void on_create(Handle<Object>& self,BListView* cobj){
			JsICtrl::on_create(self,(BICtrl*)cobj);
			cobj->Param->AddStyle(LVS_REPORT|LVS_SHOWSELALWAYS);
			cobj->OnNotify.BindStd(&ListView_onNotify);
		}
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			SET_CLA_FUNC(getColumnCount);
			SET_CLA_FUNC(getItemCount);
			SET_CLA_FUNC(getItemText);
			SET_CLA_FUNC(setItemText);
			SET_CLA_FUNC(deleteColumn);
			SET_CLA_FUNC(deleteItem);
			SET_CLA_FUNC(addColumn);
			SET_CLA_FUNC(addItem);
			SET_CLA_FUNC(setSelected);
			SET_CLA_FUNC(getSelected);
			SET_CLA_FUNC(isSelected);
			SET_CLA_FUNC(getCheck);
			SET_CLA_FUNC(setCheck);
			SET_CLA_ACCESSOR(checkBox);
			SET_CLA_ACCESSOR(rowSelect);
			SET_CLA_ACCESSOR(gridLine);
		}
	};
	//*],"source":"D:\\SoftProject\\GitLib\\jsuser\\example\\listview.jsuser"}//*

	//*,{
	//	"type":"class",
	//	"name":"CStatusBar",
	//	"text":"CStatusBar 封装一个 Windows 标准 StatusBar 控件，CStatusBar 继承自 CCtrl。",
	//	"member":[//*
	class BStatusBar : public BWnd<cs::StatusBar>{};
	class JsStatusBar : public JsWndObject<BStatusBar,JsStatusBar,TEMPLATE_ID_STATUSBAR>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CTrackBar",
	//	"text":"CTrackBar 封装一个 Windows 标准 TrackBar 控件，CTrackBar 继承自 CCtrl。",
	//	"member":[//*
	class BTrackBar : public BWnd<cs::TrackBar>{};
	class JsTrackBar : public JsWndObject<BTrackBar,JsTrackBar,TEMPLATE_ID_TRACKBAR>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CUpDown",
	//	"text":"CUpDown 封装一个 Windows 标准 UpDown 控件，CUpDown 继承自 CCtrl。",
	//	"member":[//*
	class BUpDown : public BWnd<cs::UpDown>{};
	class JsUpDown : public JsWndObject<BUpDown,JsUpDown,TEMPLATE_ID_UPDOWN>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CHeader",
	//	"text":"CHeader 封装一个 Windows 标准 Header 控件，CHeader 继承自 CCtrl。",
	//	"member":[//*
	class BHeader : public BWnd<cs::Header>{};
	class JsHeader : public JsWndObject<BHeader,JsHeader,TEMPLATE_ID_HEADER>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CLink",
	//	"text":"CLink 封装一个 Windows 标准 Link 控件，CLink 继承自 CCtrl。",
	//	"member":[//*
	class BLink : public BWnd<cs::Link>{};
	class JsLink : public JsWndObject<BLink,JsLink,TEMPLATE_ID_LINK>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CIpAddress",
	//	"text":"CIpAddress 封装一个 Windows 标准 IpAddress 控件，CIpAddress 继承自 CCtrl。",
	//	"member":[//*
	class BIpAddress : public BWnd<cs::IpAddress>{};
	class JsIpAddress : public JsWndObject<BIpAddress,JsIpAddress,TEMPLATE_ID_IPADDRESS>{};

	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"LayoutParam",
	//	"text":"LayoutParam 是窗口布局系统（Layout）的布局参数。",
	//	"member":[
	//	{
	//		"type":"property",
	//		"name":"margin",
	//		"objtype":"Rect",
	//		"text":"布局的边界。"
	//	},
	//	{
	//		"type":"property",
	//		"name":"weight",
	//		"objtype":"float",
	//		"text":"权重值，每个布局的大小按权重分配。"
	//	},
	//	{
	//		"type":"property",
	//		"name":"length",
	//		"objtype":"integer",
	//		"text":"Layout 的固有长度，Layout 的尺寸是权重分配的长度加上固有长度。"
	//	},
	//	{
	//		"type":"property",
	//		"name":"style",
	//		"objtype":"integer",
	//		"text":"布局的风格，最低位 0 表示垂直布局，1 表示水平布局。"
	//	},
	//	{
	//		"type":"function",
	//		"name":"setVertical()",
	//		"text":"设置当前 Layout 对象为垂直布局模式，垂直布局模式的子 Layout 按垂直方式排列。",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"函数没有返回值。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"setHorizontal()",
	//		"text":"设置当前 Layout 对象为水平布局模式，水平布局模式的子 Layout 按水平方式排列。",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"函数没有返回值。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"isHorizontal()",
	//		"text":"当前布局是否为水平布局模式。",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"返回一个布尔值。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"isVertical()",
	//		"text":"当前布局是否为垂直布局模式，Layout 的初始状态是垂直模式。",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"返回一个布尔值。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"reset()",
	//		"text":"设置布局参数为初始状态：length = 0，weight = 1，margin = （0,0,0,0），style = 0（垂直布局）。",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"函数没有返回值。"
	//		}
	//	}
	//]}
	//,{
	//	"type":"class",
	//	"name":"Layout",
	//	"text":"Layout 是一个窗口布局系统。",
	//	"remark":[
	//		"布局系统规则如下：",
	//		"1. 顶级布局总是充满窗口，但是留出边界，无视weight和length属性。因为顶级Layout只能有一个。",
	//		"2. 一个布局的多个子布局按这个布局是Horizontal或者Vertical属性来水平或竖直顺序排列。",
	//		"3. 子布局的大小按如下规则：水平布局，所有子布局的高是一样的；垂直布局，所有子布局的宽是一样的；都是加上边界宽度后充满父布局。水平布局时子布局的宽度分配和垂直布局子布局的高度分配，都遵循先分配每个子布局指定长度和边界值，剩余尺寸按权重（weight）分配给每个子布局的原则。注意：当剩余尺寸为负数的情况，将按权重缩小每个子布局的尺寸。",
	//		"4. 对于子布局权重总和为 0 的情况（小于0.00001），子布局尺寸按指定长度分配，而无视父布局的尺寸，所以这种情况可能导致子布局超出父布局的情形。",
	//		"当前布局尺寸分配仅仅取决于同级的布局的参数（Param）和父布局的尺寸，而不会受它的子布局的影响，也就是子布局的任何参数不会影响父布局的尺寸。",
	//		"例如：父布局实际分的的长度为110，有2个子布局，子布局的长度都是10，权重分别为1.0和2.0，那么，110 - （10 + 10） = 90，这个90就是按权重分配的可调尺寸，按比例，权重1.0的布局，分得30，权重2.0的布局分的60，他们的实际尺寸分别为10+30和10+60。如果他们的边界值都不是 0 ，那么还要从90里面减去边界值，剩下的尺寸才去分配。"
	//	],
	//	"member":[
	//	{
	//		"type":"property",
	//		"name":"param",
	//		"objtype":"LayoutParam",
	//		"text":"Layout 的布局参数。"
	//	},
	//	{
	//		"type":"property",
	//		"name":"name",
	//		"objtype":"string",
	//		"text":"布局的名称，可以使用 getByName 或者指定名称的布局。"
	//	},
	//	{
	//		"type":"property",
	//		"name":"childs",
	//		"objtype":"array",
	//		"text":"子布局数组，每个元素都是一个布局对象。使用 add、delete、remove、addin等函数添加和移除子对象，不要直接更改数组。可以使用数组的 indexOf 函数查找子对象的索引值。"
	//	},
	//	{
	//		"type":"function",
	//		"name":"getByName(name)",
	//		"text":"获取当前 Layout 对象及其任意级的子项 name 属性和指定的名称相同的项，只返回第一找到的项。",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"name",
	//				"text":"要查找的项的名称。"
	//			}
	//		],
	//		"return":{
	//			"type":"Layout",
	//			"text":"如果找到了指定名称的项，返回这个项，否则返回 undefined。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"ptInLayout(pt)",
	//		"text":"检测一个点是否在这个 Layout 的内部。",
	//		"param":[
	//			{
	//				"type":"Point",
	//				"name":"pt",
	//				"text":"要检测的点。"
	//			}
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"返回一个布尔值。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"reset()",
	//		"text":"重置当前 Layout 对象为初始状态，这将删除它所有的子项，销毁所有的绑定的控件。",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"函数没有返回值。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"parent()",
	//		"text":"获取当前 Layout 的父项，如果没有父项，返回 undefined。",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"Layout",
	//			"text":"返回一个 Layout，如果当前对象没有父项，返回 undefined。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"setControl(className,id,text,style,exstyle)",
	//		"text":"设置当前对象的控件，这个操作会重置原先绑定的控件，控件不会立即生成，直到调用 refresh 函数，控件才会生成。",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"className",
	//				"text":"控件的种类，如果设置了一个空串或无效的控件类名，会被认为是一个普通的 View 对象。"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"id",
	//				"text":"指定控件的 id。"
	//			},
	//			{
	//				"type":"string",
	//				"name":"text",
	//				"text":"控件的文本。"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"style",
	//				"text":"控件的 style 属性。"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"exstyle",
	//				"text":"控件的 exStyle 属性。"
	//			}
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"函数没有返回值。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"refresh(hwnd)",
	//		"text":"刷新当前 Layout 和它的子控件，控件将会调整布局。只需要在顶级 Layout 对象调用一次这个函数。",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"hwnd",
	//				"text":"顶级布局绑定的 owner 窗口句柄。"
	//			}
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"函数没有返回值。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"move(len)",
	//		"text":"移动当前布局，这个操作本质是通过调整 weight 来改变当前布局的位置，所以如果当前布局的同级布局只有一个的时候（它自己），是无法移动的。",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"len",
	//				"text":"移动的距离。"
	//			}
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"函数没有返回值。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"add([index])",
	//		"text":"添加子布局，index 缺省或大于最大插入位置，子布局被添加到末尾。",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"[index]",
	//				"text":"插入位置，-1 或缺省表示添加到末尾。"
	//			}
	//		],
	//		"return":{
	//			"type":"Layout",
	//			"text":"返回新添加的子 Layout 对象。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"del(index)",
	//		"text":"删除一个子布局对象，子布局如果绑定了窗口，会被销毁。",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"index",
	//				"text":"删除的位置，超出子 Layout 的索引范围，函数失败。"
	//			}
	//		],
	//		"return":{
	//			"type":"bool",
	//			"text":"成功返回 true，失败返回 undefined。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"remove(index)",
	//		"text":"移除一个子布局，但是子布局不会被删除，绑定的窗口也不会销毁，这样做的目的是这个子 Layout 还可以被添加到某个位置，也就是可以移动子 Layout 的位置。",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"index",
	//				"text":"移除的索引位置，超出范围，函数会失败。"
	//			}
	//		],
	//		"return":{
	//			"type":"Layout",
	//			"text":"成功返回移除的子 Layout 对象，失败返回undefined。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"addin(lay,[index])",
	//		"text":"。",
	//		"param":[
	//			{
	//				"type":"Layout",
	//				"name":"lay",
	//				"text":"要添加的 Layout 对象。"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"[index]",
	//				"text":"添加的位置，超范围会被添加到末尾。"
	//			}
	//		],
	//		"return":{
	//			"type":"bool",
	//			"text":"成功返回true，失败返回undefined。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"toJson()",
	//		"text":"返回一个对象，这个对象可以被解析为 json 字串。",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"object",
	//			"text":"返回一个特定格式的对象。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"parseJson(json)",
	//		"text":"从一个保存了 Layout 信息的对象初始化当前 Layout，当前 Layout 会先被重置为初始状态。",
	//		"param":[
	//			{
	//				"type":"object",
	//				"name":"json",
	//				"text":"包含了布局信息的 json 对象。"
	//			}
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"函数没有返回值。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"load(file,[code])",
	//		"text":"从一个布局文件加载布局。",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"file",
	//				"text":"布局文件名。"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"[code]",
	//				"text":"保存文件使用的编码，默认是 utf-8。"
	//			}
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"加载成功返回 true，否则返回 false。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"save(file,[code])",
	//		"text":"保存当前布局到一个文件，文件以 json 格式保存。",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"file",
	//				"text":"要保存的文件名。"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"[code]",
	//				"text":"文件使用的编码，默认是 utf-8。"
	//			}
	//		],
	//		"return":{
	//			"type":"Layout",
	//			"text":"成功返回 true，失败返回 false。"
	//		}
	//	}
	//],"source":"D:\\SoftProject\\GitLib\\jsuser\\example\\layout.jsuser"}//*
	class JsMenu : public JsHandleObject<cs::UserMenu,HMENU,JsMenu,TEMPLATE_ID_MENU>{
		static Handle<Value> create(const Arguments& args){
			HandleScope stack;
			while(true){
				Handle<Object> self = args.This();
				cs::UserMenu* cobj;
				if(!GetCHandle(cobj,self)) break;
			}
			return Undefined();
		}
		static void set(cs::String& name,cs::UserMenu* cobj,Local<Value>& value,Local<Object>& self){
		}
		static Handle<Value> get(cs::String& name,cs::UserMenu* cobj,Local<Object>& self){
			;
		}
	public:
		static void init(Handle<FunctionTemplate>& ft){
			;
		}
	};
	void LoadLayout(Handle<Object>& glb);
	void LoadWnd(Handle<Object>& glb){
		HandleScope store;
		LoadJsRes(IDR_JS_WND_BEFORE,L"wnd_before.js");
		Handle<Function> message = GetObjProperty<Function>(glb,L"Message");
		GetEnv()->SetFunc(message,FUNC_ID_MESSAGE);
		SET_OBJ_FUNC(message->Get(String::New("prototype"))->ToObject(),send,Message_send);

		Handle<Function> mouse = GetObjProperty<Function>(glb,L"Mouse");
		GetEnv()->SetFunc(mouse,FUNC_ID_MOUSE);
		//SET_OBJ_FUNC(mouse->Get(String::New("prototype"))->ToObject(),parseMessage,Mouse_parseMessage);

		JsWnd::Load(glb);
		JsView::Load(glb);
		JsFrame::Load(glb);
		JsICtrl::Load(glb);
		Persistent<FunctionTemplate>& ctrlft = GetEnv()->GetTemplate(TEMPLATE_ID_CTRL);
		JsLabel::Load(glb,L"CLabel",&ctrlft);
		JsButton::Load(glb,L"CButton",&ctrlft);
		JsGroupBox::Load(glb,L"CGroupBox",&ctrlft);
		JsCheckBox::Load(glb,L"CCheckBox",&ctrlft);
		JsRadio::Load(glb,L"CRadio",&ctrlft);
		JsEdit::Load(glb,L"CEdit",&ctrlft);
		JsDateTimePicker::Load(glb,L"CDateTimePicker",&ctrlft);
		JsMonthCalendar::Load(glb,L"CMonthCalendar",&ctrlft);
		JsListBox::Load(glb,L"CListBox",&ctrlft);
		JsComboBox::Load(glb,L"CComboBox",&ctrlft);
		JsComboBoxEx::Load(glb,L"CComboBoxEx",&ctrlft);
		JsTreeView::Load(glb,L"CTreeView",&ctrlft);
		JsToolBar::Load(glb,L"CToolBar",&ctrlft);
		JsProgressBar::Load(glb,L"CProgressBar",&ctrlft);
		JsToolTip::Load(glb,L"CToolTip",&ctrlft);
		JsTabPage::Load(glb,L"CTabPage",&ctrlft);
		JsHotKey::Load(glb,L"CHotKey",&ctrlft);
		JsListView::Load(glb,L"CListView",&ctrlft);
		JsStatusBar::Load(glb,L"CStatusBar",&ctrlft);
		JsTrackBar::Load(glb,L"CTrackBar",&ctrlft);
		JsUpDown::Load(glb,L"CUpDown",&ctrlft);
		JsHeader::Load(glb,L"CHeader",&ctrlft);
		JsLink::Load(glb,L"CLink",&ctrlft);
		JsIpAddress::Load(glb,L"CIpAddress",&ctrlft);
		LoadLayout(glb);
	}
	void LoadDialog(Handle<Object>& glb);
	LPCWSTR RequireWnd(LPCWSTR module){
		HandleScope store;
		Handle<Object> glb = v8::GetGlobal();

		cs::StringList sl;
		sl.SplitString(module,L",");
		for(uint i=0;i<sl.Count();i++){
			sl[i].Trim();
		}

		loadComponent(sl,module,L"wnd",glb,L"CWnd",&LoadWnd);
		loadComponent(sl,module,L"dialog",glb,L"CFileDialog",&LoadDialog);
		return L"wnd,dialog";
	}
	//*],//*

	//*
	//	"source":"D:\\SoftProject\\GitLib\\jsuser\\example\\wnd.jsuser"
	//*

	//*}//*
}