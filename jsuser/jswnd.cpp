#include "stdafx.h"
#include "v8base.h"
#include "resource.h"

namespace v8{
	//*{
	//	"type":"group",
	//	"name":"wnd",
	//	"text":"���ڡ��ؼ���",
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
	//	"text":"������Ϣ����Windows ����ͨ����Ϣ�ص��������¼���",
	//	"member":[//*
	class JsMessage{
		//*{
		//	"type":"property",
		//	"name":"hWnd",
		//	"objtype":"integer",
		//	"text":"�������ڵĴ��ھ�������ֵ�͵�ǰ���ڵľ����ͬ��"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"id",
		//	"objtype":"integer",
		//	"text":"��Ϣ���͡�"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"wParam",
		//	"objtype":"integer",
		//	"text":"һ�������������������������ھ������Ϣ���͡�"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"lParam",
		//	"objtype":"integer",
		//	"text":"һ�������������������������ھ������Ϣ���͡�"
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
	//	"text":"�����Ϣ���������Ϣʹ�õĲ�����",
	//	"member":[//*
	class Mouse{
		//*{
		//	"type":"property",
		//	"name":"x",
		//	"objtype":"integer",
		//	"text":"���λ�� x ���ꡣ"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"y",
		//	"objtype":"integer",
		//	"text":"���λ�� y ���ꡣ"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"button",
		//	"objtype":"integer",
		//	"text":"������궯���İ�ť��"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"event",
		//	"objtype":"integer",
		//	"text":"������궯�����¼����͡�"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"wheel",
		//	"objtype":"integer",
		//	"text":"�������ķ����������ֻ���� -120,0,120 ������ֵ�����ÿ����һ�Σ�����һ�ι����¼���"
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isMove()",
		//	"text":"��ǰ��Ϣ�Ƿ�������ƶ���Ϣ��",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"����һ���Ƿ�ָ���¼��Ĳ���ֵ��"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isLDown()",
		//	"text":"��ǰ��Ϣ�Ƿ���������������Ϣ��",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"����һ���Ƿ�ָ���¼��Ĳ���ֵ��"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isLUp()",
		//	"text":"��ǰ��Ϣ�Ƿ���������������Ϣ��",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"����һ���Ƿ�ָ���¼��Ĳ���ֵ��"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isLDBClick()",
		//	"text":"��ǰ��Ϣ�Ƿ���������˫����Ϣ��",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"����һ���Ƿ�ָ���¼��Ĳ���ֵ��"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isRDown()",
		//	"text":"��ǰ��Ϣ�Ƿ�������Ҽ�������Ϣ��",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"����һ���Ƿ�ָ���¼��Ĳ���ֵ��"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isRUp()",
		//	"text":"��ǰ��Ϣ�Ƿ�������Ҽ�������Ϣ��",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"����һ���Ƿ�ָ���¼��Ĳ���ֵ��"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isRDBClick()",
		//	"text":"��ǰ��Ϣ�Ƿ�������Ҽ�˫����Ϣ��",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"����һ���Ƿ�ָ���¼��Ĳ���ֵ��"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isMDown()",
		//	"text":"��ǰ��Ϣ�Ƿ�������м�������Ϣ��",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"����һ���Ƿ�ָ���¼��Ĳ���ֵ��"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isMUp()",
		//	"text":"��ǰ��Ϣ�Ƿ�������м�������Ϣ��",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"����һ���Ƿ�ָ���¼��Ĳ���ֵ��"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isMDBClick()",
		//	"text":"��ǰ��Ϣ�Ƿ�������м�˫����Ϣ��",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"����һ���Ƿ�ָ���¼��Ĳ���ֵ��"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isXDown()",
		//	"text":"��ǰ��Ϣ�Ƿ������ X����׼����֮��ļ�����������Ϣ��",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"����һ���Ƿ�ָ���¼��Ĳ���ֵ��"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isXUp()",
		//	"text":"��ǰ��Ϣ�Ƿ������ X����׼����֮��ļ�����������Ϣ��",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"����һ���Ƿ�ָ���¼��Ĳ���ֵ��"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"isXDBClick()",
		//	"text":"��ǰ��Ϣ�Ƿ������ X����׼����֮��ļ�����˫����Ϣ��",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"����һ���Ƿ�ָ���¼��Ĳ���ֵ��"
		//	}
		//}//*
//*,{
//	"type":"function",
//	"name":"isLeave()",
//	"text":"��ǰ��Ϣ�Ƿ�������뿪��ǰ������Ϣ��",
//	"param":[
//	],
//	"return":{
//		"type":"boolean",
//		"text":"����һ���Ƿ�ָ���¼��Ĳ���ֵ��"
//	}
//}//*
//*,{
//	"type":"function",
//	"name":"isWheel()",
//	"text":"��ǰ��Ϣ�Ƿ�����������Ϣ��",
//	"param":[
//	],
//	"return":{
//		"type":"boolean",
//		"text":"����һ���Ƿ�ָ���¼��Ĳ���ֵ��"
//	}
//}//*
//*,{
//	"type":"function",
//	"name":"isLButton()",
//	"text":"��ǰ��Ϣ�Ƿ�����������������Ϣ��",
//	"param":[
//	],
//	"return":{
//		"type":"boolean",
//		"text":"����һ���Ƿ�ָ����ť�Ĳ���ֵ��"
//	}
//}//*
//*,{
//	"type":"function",
//	"name":"isX2Button()",
//	"text":"��ǰ��Ϣ�Ƿ������ X2 ����Ϣ��",
//	"param":[
//	],
//	"return":{
//		"type":"boolean",
//		"text":"����һ���Ƿ�ָ����ť�Ĳ���ֵ��"
//	}
//}//*
//*,{
//	"type":"function",
//	"name":"isRButton()",
//	"text":"��ǰ��Ϣ�Ƿ�������Ҽ���Ϣ��",
//	"param":[
//	],
//	"return":{
//		"type":"boolean",
//		"text":"����һ���Ƿ�ָ����ť�Ĳ���ֵ��"
//	}
//}//*
//*,{
//	"type":"function",
//	"name":"isMButton()",
//	"text":"��ǰ��Ϣ�Ƿ�������м���Ϣ��",
//	"param":[
//	],
//	"return":{
//		"type":"boolean",
//		"text":"����һ���Ƿ�ָ����ť�Ĳ���ֵ��"
//	}
//}//*
//*,{
//	"type":"function",
//	"name":"isX1Button()",
//	"text":"��ǰ��Ϣ�Ƿ������ X1 ����Ϣ��",
//	"param":[
//	],
//	"return":{
//		"type":"boolean",
//		"text":"����һ���Ƿ�ָ����ť�Ĳ���ֵ��"
//	}
//}//*
	};
	//*]}//*
	//��Щ����������Ϊ��������������ľ�̬�������ܱ� Delegate �� Add ������ȷʶ����Ȼ������һ���ġ�
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
		// Scroll �汾��
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
	//	"text":"CWnd ��װһ�����ڶ���Windows ϵͳ���κδ��ں��Ӵ��ڶ���һ�� CWnd ����",
	//	"member":[//*
	class JsWnd : public JsWndObject<BIWnd,JsWnd,TEMPLATE_ID_WND>{
		//*{
		//	"type":"function",
		//	"name":"sendMessage(id,[wParam],[lParam])",
		//	"text":"��ǰ���ڷ�����Ϣ�����ڵĺܶ๦����ͨ����Ϣʵ�ֵġ�",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"id",
		//			"text":"��Ϣ id��Ҳ������Ϣ�����࣬������Ϣ�����壬����΢����ĵ���"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[wParam]",
		//			"text":"һ������������ȱʡΪ 0 ��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[lParam]",
		//			"text":"һ������������ȱʡΪ 0 ��"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"�ɹ�����һ������������������������Ϣ�����ͣ������ǰ���ڻ�û�д���������ʧ�ܣ�ʧ�ܷ��� undefined��"
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
		//	"text":"��ȡ�������ľ��Σ�������Ϊ��λ�����Ͻ��ǣ�0��0����x �᷽�����ң�y �᷽�����¡�����Ƕ������ڣ�����ĻΪ�ο���������Ӵ��ڣ��Ը�����Ϊ�ο���������������ڴ�������ǰ���ã����ص��ǽ�Ҫ���ɵĴ��ڵĲ�����",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"Rect",
		//		"text":"�ɹ�����һ�� Rect ����ʧ�ܷ��� undefined��"
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
		//	"text":"��ȡ���ڿͻ����ĳߴ硣",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"Size",
		//		"text":"�ɹ�����һ�� Size ����ʧ�ܷ��� undefined��"
		//	},
		//	"remark":[
		//		"�ͻ����Ǵ��ڹ��û���ͼ�ͷ����Ӵ��ڵ��������Ǵ��ڳ�ȥ�������ͱ߿������"
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
		//	"text":"���ô��ڣ���򣩵Ĵ�С�����Ե������ÿ�ڻ�߶ȡ�������������ڴ�������ǰ���ã�������Ӱ�촰�����ɺ�����ԡ�",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"width",
		//			"text":"Ҫ���õĿ�ȣ�������ֵ������Ϊ undefined�����ڵĿ�Ȳ��䡣���ڿ����һ����Сֵ�����ڲ��ᱻ����ΪС�����ֵ�Ŀ�ȣ���ͨ���ڵ���Сֵ�� 0."
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[height]",
		//			"text":"Ҫ���õĸ߶ȣ�������ֵȱʡ����Ϊ undefined�����ڵĸ߶Ȳ��䡣���ڸ߶���һ����Сֵ�����ڸ߶Ȳ��ܱ�����ΪС�������Сֵ�����ֵͨ���� 0 ��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"���ô��ڵ�λ�á��߶ȡ���ȡ����Ҫ��������λ�û�ߴ磬ʹ�� setPosition �� setSize ������������������ڴ�������ǰ���ã�������Ӱ�촰�����ɺ�����ԡ�",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"x",
		//			"text":"Ҫ���õĴ��� x ���ꡣ"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"y",
		//			"text":"Ҫ���õĴ��� y ���ꡣ"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"cx",
		//			"text":"Ҫ���õĴ��ڿ�ȡ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"cy",
		//			"text":"Ҫ���õĴ��ڸ߶ȡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	},
		//	"remark":[
		//		"��������� 4 ���������������������ֵ������������� 4 ����������ʧ�ܡ�"
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
		//	"text":"���ô��ڵ�λ�ã�����Ƕ������ڣ�����Ļ����ϵΪ�ο���������Ӵ��ڣ��Ը���������ϵΪ�ο���������������ڴ�������ǰ���ã�������Ӱ�촰�����ɺ�����ԡ�",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"x",
		//			"text":"Ҫ���õ� x ���꣬������ֵ������Ϊ undefined�����ڵ� x λ�ò��䡣"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[y]",
		//			"text":"Ҫ���õ� y ���꣬������ֵȱʡ����Ϊ undefined�����ڵ� y ���겻�䡣"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"�������ڵ�λ�úʹ�С�������ڴ���û�����ɵ�ʱ����������������Щ���õ�λ�ò������ᱻ���棬���������ɴ���ʱʹ�á�",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"dx",
		//			"text":"Ҫ������ x ����仯����������ı䴰�ڵ� x ���꣬����Ϊ 0 ��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[dy]",
		//			"text":"Ҫ������ y ����仯����������ı䴰�ڵ� y ���꣬����Ϊ 0 ��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[dcx]",
		//			"text":"Ҫ�����Ŀ�ȱ仯����������ı䴰�ڵĿ�ȣ�����Ϊ 0 ��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[dcy]",
		//			"text":"Ҫ�����ĸ߶�����仯����������ı䴰�ڵĸ߶ȣ�����Ϊ 0 ��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"���ٴ��ڡ�",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"����һ��ˢ����Ϣ�����ڱ����»��ơ��������������onDraw �ص��������ᱻ���С�����������Զ�ε��ã���������ˢ�������ᱻ��ϣ�����ֻ������һ���ػ涯����",
		//	"param":[
		//		{
		//			"type":"CRgn|Rect",
		//			"name":"[rgn]",
		//			"text":"���ý�Ҫ��ˢ�µ�������Ҫôȱʡ��Ҫô����һ�� CRgn �������һ�� Rect ����"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"���ô��ڼ�ס�Լ���λ�úʹ�С������ȡ��������ԡ�Ӧ���ڴ�������ǰ���������������Ϊ���ڵ�λ���������ɵ�ʱ�����õģ���������ڴ������ɺ���������������Ȼ�ڴ�������ʱ��ס��λ�ã���������ʱ������ʹ�����λ�á�",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"name",
		//			"text":"����ͨ����ǰĿ¼�µĽű�ͬ�� json �ļ���¼���ڵĴ�С��λ�ã���������������ֲ�ͬ�Ĵ��ڣ�����һ��Ӧ�ó�������ͬ���Ĵ��ڡ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[flag]",
		//			"text":"��¼ѡ�ȱʡ��¼λ�úʹ�С��1����¼ X ���ꣻ2����¼ Y ���ꣻ4����¼��ȣ�8����¼�߶ȡ����ֵ���������� 4 ��ֵ����ϣ�Ĭ���� 15������¼ȫ�� 4 �����ԡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"����һ�� Layout ���󣬵�ǰ������Ϊ layout �Ķ������ڣ��ο� Layout ��Ŀ��",
		//	"param":[
		//		{
		//			"type":"Layout",
		//			"name":"layout",
		//			"text":"Ҫ�󶨵� Layout ����"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"���ô��ڵ����壬������������ͨ����û���κ����ã����Ƕ��� Windows �ؼ������ǿؼ���ʾ����ʹ�õ����壬һ������������ø�������塣wnd ϵͳ��һ��ȱʡ���壬ÿ���ؼ�Ĭ�϶���ʹ�����ȱʡ���塣����ʹ�� CFont ����� setToDefault ���������õ�ǰ����Ϊȱʡ���壬�ı�������壬����Ӱ������ʹ���������Ĵ��ڡ�",
		//	"param":[
		//		{
		//			"type":"CFont",
		//			"name":"font",
		//			"text":"Ҫ���õ����塣"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"���ô��ڵ����ָ�룬����ԭ����ָ�����ᱻ���٣�����ܹ������ٵĻ�����",
		//	"param":[
		//		{
		//			"type":"CIcon",
		//			"name":"icon",
		//			"text":"һ����Ч��ͼ�����"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"����û�з���ֵ��"
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
		//	"text":"���ػ����ô��ڵ��ı�����ͬ�����ı������岻ͬ�����綥�������Ǵ��ڵı��⣬�� Edit �ؼ����ǿؼ����ı���������������ڴ�������ǰ���úͻ�ȡ��������Ӱ�촰�����ɺ�����ԡ�"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"parent",
		//	"objtype":"integer",
		//	"text":"����/��ȡ���ڵĸ����ڣ��������û�и����ڣ����� 0��"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"disabled",
		//	"objtype":"boolean",
		//	"text":"����/��ȡ���ڵ� disabled ״̬��һ�� disabled ���ڲ���Ӧ�û��Ĳ�����������������ڴ�������ǰ���úͻ�ȡ��������Ӱ�촰�����ɺ�����ԡ�"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"style",
		//	"objtype":"integer",
		//	"text":"����/��ȡ���ڵ� style ������style �������ƴ��ڵĸ������ԡ�������������ڴ�������ǰ���úͻ�ȡ��������Ӱ�촰�����ɺ�����ԡ�"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"exStyle",
		//	"objtype":"integer",
		//	"text":"����/��ȡ���ڵ� exStyle ������exStyle �������ƴ��ڵĶ������ԡ���ʵ���� style һ�����������ã��ټ�һ��������������������ڴ�������ǰ���úͻ�ȡ��������Ӱ�촰�����ɺ�����ԡ�"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"classStyle",
		//	"objtype":"integer",
		//	"text":"����/��ȡ���ڵ� classStyle ������classStyle �Ǵ�����ķ��������������ڴ�������ǰ���úͻ�ȡ��������Ӱ�촰�����ɺ�����ԡ�"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"fontColor",
		//	"objtype":"integer",
		//	"text":"����/��ȡ���ڵ�������ɫ��ĳЩ�ؼ����ܻ����⴦��������ò�һ���������á�"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"bkColor",
		//	"objtype":"integer",
		//	"text":"����/��ȡ���ڵı�����ɫ��"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"onCreate()",
		//	"objtype":"function",
		//	"text":"�ص���������Ӧ��������ʱ�� WM_CREATE ��Ϣ�Ļص�������",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"����û�з���ֵ��"
		//	}
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"onSize(cx,cy)",
		//	"objtype":"function",
		//	"text":"�ص���������Ӧ�������ڵ����ߴ�� WM_SIZE ��Ϣ��",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"cx",
		//			"text":"�ͻ�����ȡ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"cy",
		//			"text":"�ͻ����߶ȡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"����û�з���ֵ��"
		//	}
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"onCommand(id,type)",
		//	"objtype":"function",
		//	"text":"�ص���������Ӧ�������ڵ����ߴ�� WM_SIZE ��Ϣ��",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"cx",
		//			"text":"�ͻ�����ȡ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"cy",
		//			"text":"�ͻ����߶ȡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"����û�з���ֵ��"
		//	}
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"onMouse(mouse)",
		//	"objtype":"function",
		//	"text":"�ص���������Ӧ�����Ϣ��",
		//	"param":[
		//		{
		//			"type":"Mouse",
		//			"name":"mouse",
		//			"text":"�����󣬲μ� Mouse ��Ŀ��"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"����û�з���ֵ��"
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
	//	"text":"CView ��һ��ͨ���Ĵ��ڶ��󣬼��ɹ�������ͼ�ȹ��ܡ�CView �̳��� CWnd��",
	//	"member":[//*
	class JsView : public JsWndObject<BView,JsView,TEMPLATE_ID_VIEW>{
		//*{
		//	"type":"function",
		//	"name":"create(parent)",
		//	"text":"���ô��ڼ�ס�Լ���λ�úʹ�С������ȡ��������ԡ�",
		//	"param":[
		//		{
		//			"type":"integer|CWnd",
		//			"name":"parent",
		//			"text":"���ɵĴ����� parent ���Ӵ��ڣ���ȡһ�����ڵľ������ handle ������"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"����һ���ص����������ļ����϶��������Ǳ����С�",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"files",
		//			"text":"����һ���ö��ŷָ���һϵ���ļ���ȫ·�����ɵ��ִ���"
		//		}
		//	],
		//	"return":{
		//		"type":"undefined",
		//		"text":"�������û�з���ֵ��"
		//	}
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"onCommand(id,type)",
		//	"objtype":"function",
		//	"text":"����һ���ص������������ڲ˵���ť�����ʱ���߼��ټ�������ʱ��������������С�",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"id",
		//			"text":"�˵����ؼ����߼��ټ��� id������ʶ���Ǹ��˵����߿ؼ��������"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"type",
		//			"text":"����Ϣ����Ϊ�˵������������ʱ�����ֵ�� 0������ǿؼ������ģ����ֵ�� 1����������ο� Windows ��˵���ĵ���"
		//		}
		//	],
		//	"return":{
		//		"type":"undefined",
		//		"text":"�������û�з���ֵ��"
		//	}
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"onDraw(dc)",
		//	"objtype":"function",
		//	"text":"����һ���ص����������ڻ��ƴ��ڡ�",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"dc",
		//			"text":"��ͼ������Ҫʹ�� dc �������򴰿ڻ�ͼ��"
		//		}
		//	],
		//	"return":{
		//		"type":"undefined",
		//		"text":"�������û�з���ֵ��"
		//	}
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"onScroll(dx,dy)",
		//	"objtype":"function",
		//	"text":"����һ���ص������������ڹ���ʱ�����С�",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"dx",
		//			"text":"ˮƽ�����������������ֵ������ֵ���ҡ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"dy",
		//			"text":"��ֱ�����������������ֵ���ϣ���ֵ���¡�"
		//		}
		//	],
		//	"return":{
		//		"type":"undefined",
		//		"text":"�������û�з���ֵ��"
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
	//	"text":"CFrame ��װһ���������ڶ��󣬶�������ֱ����ʾ����Ļ�ϣ���������Ϊ�Ӵ�����ʾ�ڸ������ϣ�ͨ���������ڶ��Ƕ������ڡ�CFrame �̳��� CView��",
	//	"member":[//*
	class JsFrame : public JsWndObject<BFrame,JsFrame,TEMPLATE_ID_FRAME>{
		//*{
		//	"type":"function",
		//	"name":"create([parent],[quit],[show])",
		//	"text":"���ɴ��ڡ�",
		//	"param":[
		//		{
		//			"type":"integer|CWnd",
		//			"name":"[parent]",
		//			"text":"ָ�����ڵĸ����ھ������ȡһ�����ڵľ������ handle ������ע�⣺��������Ҳ�����и����ڣ�һ�� CFrame ����һ����һ���������ڣ��������и����ڣ�Ҳ����û�и����ڣ�parent��Ϊ0����"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[quit]",
		//			"text":"���رմ��ڵ�ʱ���Ƿ��˳�Ӧ�ó���ȱʡ���˳��� true�����һ��Ӧ�ó����ж�� CFrame ����һ��ֻ�������ڹر�ʱ����Ӧ���˳��������� CFrame ���������Ӧ����Ϊ false��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[show]",
		//			"text":"���ڵ���ʾ״̬��0�����أ����ڲ��ɼ���1��������ʾ״̬��2����С����3����󻯡������������ʹ�ã����ڵ���ʾ״̬һ��ͨ�� CreateParam �� style �����趨��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"�˳�Ӧ�ó����������������Ϣѭ������ WM_QUIT ��Ϣ��������ɴ���ʱ������ quit ���ԣ����رմ���ʱ����������ᱻ�Զ����á�",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"���ô��ڵ�ͼ�꣬ÿ�����ڶ���һ����������Сͼ����������Ĵ�ͼ�ꡣ����ԭ����ͼ��ᱻ���٣�����ܹ������ٵĻ���ϵͳ��Դͼ���޷����٣�",
		//	"param":[
		//		{
		//			"type":"CIcon",
		//			"name":"icon",
		//			"text":"һ����Ч��ͼ�����"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[big]",
		//			"text":"���ñ�������Сͼ�껹���������Ĵ�ͼ�꣬ȱʡ�� false��Сͼ�ꡣ"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
	//	"text":"CCtrl ��װһ�� Windows ��׼�ؼ���CCtrl �̳��� CWnd��",
	//	"member":[//*
	class JsICtrl : public JsWndObject<BICtrl,JsICtrl,TEMPLATE_ID_CTRL>{
		//*{
		//	"type":"function",
		//	"name":"create(parent)",
		//	"text":"���ɿؼ�������ָ�������ڣ�������ʧ�ܡ�",
		//	"param":[
		//		{
		//			"type":"integer|CWnd",
		//			"name":"parent",
		//			"text":"ָ�����ڵĸ����ھ������ȡһ�����ڵľ������ handle ������"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"����һ���ص���������Ӧ���ڵ� WM_NOTIFY ��Ϣ��",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"code",
		//			"text":"�������һ���ʾ notify ��Ϣ�����ͣ���ͬ�Ŀؼ��в�ͬ��ֵ�����塣"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"lParam",
		//			"text":"��Ϣ�� lParam ������notify ��Ϣ����Ϣ����������������У����������ͽṹ�ڲ�ͬ�Ŀؼ�����Ϣ���ǲ�ͬ�ġ�"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"������� undefined���൱��û�з���ֵ�����򷵻�ֵ����Ϊһ���������ظ�ϵͳ��"
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
	//	"text":"CLabel ��װһ�� Windows ��׼ Static �ؼ���CLabel �̳��� CCtrl��",
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
	//	"text":"CButton ��װһ�� Windows ��׼ Button����ť�� �ؼ���CButton �̳��� CCtrl��",
	//	"member":[//*
	class BButton : public BWnd<cs::Button>{};
	class JsButton : public JsWndObject<BButton,JsButton,TEMPLATE_ID_BUTTON>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CGroupBox",
	//	"text":"CGroupBox ��װһ�� Windows ��׼ Button������������ BS_GROUPBOX ��� �ؼ���CGroupBox �̳��� CCtrl��",
	//	"member":[//*
	class BGroupBox : public BWnd<cs::GroupBox>{};
	class JsGroupBox : public JsWndObject<BGroupBox,JsGroupBox,TEMPLATE_ID_GROUPBOX>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CCheckBox",
	//	"text":"CCheckBox ��װһ�� Windows ��׼ Button������������ BS_AUTOCHECKBOX ��� �ؼ���CCheckBox �̳��� CCtrl��",
	//	"member":[//*
	class BCheckBox : public BWnd<cs::CheckBox>{};
	class JsCheckBox : public JsWndObject<BCheckBox,JsCheckBox,TEMPLATE_ID_CHECKBOX>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CRadio",
	//	"text":"CRadio ��װһ�� Windows ��׼ Button������������ BS_AUTORADIOBUTTON ��� �ؼ���CRadio �̳��� CCtrl��",
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
	//	"text":"CEdit ��װһ�� Windows ��׼�༭�ؼ���CEdit �̳��� CCtrl��",
	//	"member":[//*
	class JsEdit : public JsWndObject<BEdit,JsEdit,TEMPLATE_ID_EDIT>{
	public:
		//*{
		//	"type":"property",
		//	"name":"setMultiLine([multiline])",
		//	"objtype":"function",
		//	"text":"���ñ༭�ؼ��Ķ��л��ߵ������ԡ�",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"[multiline]",
		//			"text":"���л��ǵ��У�ȱʡ�Ƕ��� true��"
		//		}
		//	],
		//	"return":{
		//		"type":"undefined",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"����һ���ص��������� CEdit �ؼ����ı������仯��������������С�",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"undefined",
		//		"text":"�������û�з���ֵ��"
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
	//	"text":"CDateTimePicker ��װһ�� Windows ��׼ DateTimePicker �ؼ���CDateTimePicker �̳��� CCtrl��",
	//	"member":[//*
	class BDateTimePicker : public BWnd<cs::DateTimePicker>{};
	class JsDateTimePicker : public JsWndObject<BDateTimePicker,JsDateTimePicker,TEMPLATE_ID_DATETIMEPICKER>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CMonthCalendar",
	//	"text":"CMonthCalendar ��װһ�� Windows ��׼ MonthCalendar �ؼ���CMonthCalendar �̳��� CCtrl��",
	//	"member":[//*
	class BMonthCalendar : public BWnd<cs::MonthCalendar>{};
	class JsMonthCalendar : public JsWndObject<BMonthCalendar,JsMonthCalendar,TEMPLATE_ID_MONTHCALENDAR>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CListBox",
	//	"text":"CListBox ��װһ�� Windows ��׼ ListBox �ؼ���CListBox �̳��� CCtrl��",
	//	"member":[//*
	class BListBox : public BWnd<cs::ListBox>{};
	class JsListBox : public JsWndObject<BListBox,JsListBox,TEMPLATE_ID_LISTBOX>{
		//*{
		//	"type":"function",
		//	"name":"addItem(str,[index])",
		//	"text":"���һ����Ŀ��",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"Ҫ��ӵ��ִ���"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[index]",
		//			"text":"��ӵ�λ�ã�0 ��ʾ���뵽��ǰ�棬-1 ���ߴ����������������뵽ĩβ��"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"�ɹ����ز����λ�ã�ʧ�ܷ��� undefined��"
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
		//	"text":"ɾ��һ����Ŀ��",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"ɾ��ָ����������Ŀ��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true������������磬����ʧ�ܣ����� undefined��"
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
		//	"text":"��������",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"����ָ������ı���",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"Ҫ���õ��ı���"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"Ҫ���õ��������λ�á�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"����ָ������ı���",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"Ҫ��ȡ�����λ��������"
		//		}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"�ɹ�����һ���ִ���ʧ�ܷ��� undefined��"
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
		//	"text":"��ȡ���������",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"����һ�����֣�ʧ�ܷ��� undefined��"
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
		//	"text":"��ȡ��ǰѡ�����������",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"integer|array",
		//		"text":"��ѡ���������ѡ��������������û��ѡ������� -1����ѡ���������һ�����飬�����Ԫ����ѡ������������������ʧ�ܷ��� undefined��"
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
		//	"text":"����ָ����, ���߷�Χ��ѡ�У���Χ�������ڶ�ѡ�б��",
		//	"param":[
		//		{
		//			"type":"integer|string",
		//			"name":"index",
		//			"text":"Ҫ����ѡ�е����������������Ϊ -1 ��ѡ�б�����ѡ�����ѡ�б����Ч�����������������һ���ִ���ָ���������ִ����ѡ�У���ʱ�ڶ��������������ִ�����ʼλ�á�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[index2]",
		//			"text":"�������ֻ���б���Ƕ�ѡģʽʱ���Ż��õ���index ����ָ���б��ѡ��������ޣ�index2 ����ָ��ѡ���Χ�����ޣ���� index2 С�� index ����������Χ�ڵ�ѡ��״̬�����ڶ�ѡ��������Ҫѡ��һ�����Ҫ���ڶ�����������Ϊ��ͬ��������������ʾ�������"
		//		}
		//	],
		//	"return":{
		//		"type":"integer|boolean",
		//		"text":"��ѡ�����������ѡ�����������-1��ʾû��ѡ�У�����ѡ��������� true��ʧ�ܷ��� undefined��"
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
		//	"text":"����һ���ִ���������Ҳ����ִ�Сд��ֻҪ���ı���ͷ��������ִ��Ϳ��ԡ�",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"Ҫ���ҵ��ı���"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[start]",
		//			"text":"����һ�ʼ���ң�Ĭ��ֵ�� 0����ͷ���ң���ע����� Windows Ĭ�ϵ�������ʽ������Windows Ĭ���� start ���ǲ������ģ�����ӿ�ͷ����Ӧ�ô� -1."
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"���ط������������λ�����������û���ҵ������� -1���������ʧ�ܣ����� undefined��"
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
		//	"text":"��ȷ����һ���ִ������ı�������������ı���ͬ�����ǲ����ִ�Сд",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"Ҫ���ҵ��ı���"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[start]",
		//			"text":"����һ�ʼ���ң�Ĭ��ֵ�� 0����ͷ���ң���ע����� Windows Ĭ�ϵ�������ʽ������Windows Ĭ���� start ���ǲ������ģ�����ӿ�ͷ����Ӧ�ô� -1."
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"���ط������������λ�����������û���ҵ������� -1���������ʧ�ܣ����� undefined��"
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
		//	"text":"ָ�����Ƿ�ѡ��״̬��",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"Ҫ��ѯ��������."
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�Ƿ��� true���񷵻� false��ʧ�ܷ��� undefined��"
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
		//	"text":"������������ָ����������ʾ�����������������ǰ�������Ѿ���ʾ������",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"Ҫ��ʾ����������"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
	//	"text":"CComboBox ��װһ�� Windows ��׼ ComboBox �ؼ���CComboBox �̳��� CCtrl��",
	//	"member":[//*
	class BComboBox : public BWnd<cs::ComboBox>{};
	class JsComboBox : public JsWndObject<BComboBox,JsComboBox,TEMPLATE_ID_COMBOBOX>{
	public:
		//*{
		//	"type":"function",
		//	"name":"addItem(str,[index])",
		//	"text":"���һ����Ŀ��",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"Ҫ��ӵ��ִ���"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[index]",
		//			"text":"��ӵ�λ�ã�0 ��ʾ���뵽��ǰ�棬-1 ���ߴ����������������뵽ĩβ��"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"�ɹ����ز����λ�ã�ʧ�ܷ��� undefined��"
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
		//	"text":"ɾ��һ����Ŀ��",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"ɾ��ָ����������Ŀ��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true������������磬����ʧ�ܣ����� undefined��"
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
		//	"text":"��������",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"����ָ������ı���",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"Ҫ���õ��ı���"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"Ҫ���õ��������λ�á�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"����ָ������ı���",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"Ҫ��ȡ�����λ��������"
		//		}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"�ɹ�����һ���ִ���ʧ�ܷ��� undefined��"
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
		//	"text":"��ȡ���������",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"����һ�����֣�ʧ�ܷ��� undefined��"
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
		//	"text":"��ȡ��ǰѡ�����������",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"����ѡ��������������û��ѡ������� -1���������ʧ�ܷ��� undefined��"
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
		//	"text":"����ָ����Ϊ��ѡ�С�",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"Ҫ����ѡ�е���������"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɷ��� true��ʧ�ܷ��� undefined��"
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
		//	"text":"����һ���ִ���������Ҳ����ִ�Сд��ֻҪ���ı���ͷ��������ִ��Ϳ��ԡ�",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"Ҫ���ҵ��ı���"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[start]",
		//			"text":"����һ�ʼ���ң�Ĭ��ֵ�� 0����ͷ���ң���ע����� Windows Ĭ�ϵ�������ʽ������Windows Ĭ���� start ���ǲ������ģ�����ӿ�ͷ����Ӧ�ô� -1."
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"���ط������������λ�����������û���ҵ������� -1���������ʧ�ܣ����� undefined��"
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
		//	"text":"��ȷ����һ���ִ������ı�������������ı���ͬ�����ǲ����ִ�Сд",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"Ҫ���ҵ��ı���"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[start]",
		//			"text":"����һ�ʼ���ң�Ĭ��ֵ�� 0����ͷ���ң���ע����� Windows Ĭ�ϵ�������ʽ������Windows Ĭ���� start ���ǲ������ģ�����ӿ�ͷ����Ӧ�ô� -1."
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"���ط������������λ�����������û���ҵ������� -1���������ʧ�ܣ����� undefined��"
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
		//	"text":"չ��������",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"������������ָ����������ʾ�����������������ǰ�������Ѿ���ʾ������",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"Ҫ��ʾ����������"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
	//	"text":"CComboBoxEx ��װһ�� Windows ��׼ ComboBoxEx �ؼ���CComboBoxEx �̳��� CCtrl��",
	//	"member":[//*
	class BComboBoxEx : public BWnd<cs::ComboBoxEx>{};
	class JsComboBoxEx : public JsWndObject<BComboBoxEx,JsComboBoxEx,TEMPLATE_ID_COMBOBOXEX>{
	public:
		//*{
		//	"type":"function",
		//	"name":"clear()",
		//	"text":"��������",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"��ȡ���������",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"����һ�����֣�ʧ�ܷ��� undefined��"
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
		//	"text":"��ȡ��ǰѡ�����������",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"����ѡ��������������û��ѡ������� -1���������ʧ�ܷ��� undefined��"
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
		//	"text":"����ָ����Ϊ��ѡ�С�",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"Ҫ����ѡ�е���������"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɷ��� true��ʧ�ܷ��� undefined��"
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
	//	"text":"CTreeView ��װһ�� Windows ��׼ TreeView �ؼ���CTreeView �̳��� CCtrl��",
	//	"member":[//*
	class BTreeView : public BWnd<cs::TreeView>{};
	class JsTreeView : public JsWndObject<BTreeView,JsTreeView,TEMPLATE_ID_TREEVIEW>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CToolBar",
	//	"text":"CToolBar ��װһ�� Windows ��׼ ToolBar �ؼ���CToolBar �̳��� CCtrl��",
	//	"member":[//*
	class BToolBar : public BWnd<cs::ToolBar>{};
	class JsToolBar : public JsWndObject<BToolBar,JsToolBar,TEMPLATE_ID_TOOLBAR>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CProgressBar",
	//	"text":"CProgressBar ��װһ�� Windows ��׼ ProgressBar �ؼ���CProgressBar �̳��� CCtrl��",
	//	"member":[//*
	class BProgressBar : public BWnd<cs::ProgressBar>{};
	class JsProgressBar : public JsWndObject<BProgressBar,JsProgressBar,TEMPLATE_ID_PROGRESSBAR>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CToolTip",
	//	"text":"CToolTip ��װһ�� Windows ��׼ ToolTip �ؼ���CToolTip �̳��� CCtrl��",
	//	"member":[//*
	class BToolTip : public BWnd<cs::ToolTip>{};
	class JsToolTip : public JsWndObject<BToolTip,JsToolTip,TEMPLATE_ID_TOOLTIP>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CTabPage",
	//	"text":"CTabPage ��װһ�� Windows ��׼ Tab �ؼ���CTabPage �̳��� CCtrl��",
	//	"member":[//*
	class BTabPage : public BWnd<cs::TabPage>{};
	class JsTabPage : public JsWndObject<BTabPage,JsTabPage,TEMPLATE_ID_TABPAGE>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CHotKey",
	//	"text":"CHotKey ��װһ�� Windows ��׼ HotKey �ؼ���CHotKey �̳��� CCtrl��",
	//	"member":[//*
	class BHotKey : public BWnd<cs::HotKey>{};
	class JsHotKey : public JsWndObject<BHotKey,JsHotKey,TEMPLATE_ID_HOTKEY>{};
	//*]}//*
	//*,{
	//	"type":"class",
	//	"name":"CListView",
	//	"text":"CListView ��װһ�� Windows ��׼ ListView �ؼ���CListView �̳��� CCtrl��",
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
				msg->Result = CDRF_NOTIFYITEMDRAW;	//���߿ؼ���Ҫ�Զ�������ʾ�������� CDDS_ITEMPREPAINT ��Ϣ
			}else if(lplvcd->nmcd.dwDrawStage==CDDS_ITEMPREPAINT){
				msg->Result = CDRF_NOTIFYSUBITEMDRAW; //���߿ؼ���Ҫ�Զ��嵥Ԫ�������� CDDS_SUBITEM ��Ϣ
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
		//	"text":"����������",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"�ɹ�����������ʧ�ܷ��� undefined��"
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
		//	"text":"������������",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"�ɹ�����������ʧ�ܷ��� undefined��"
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
		//	"text":"��������ı���",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"iItem",
		//			"text":"���������������"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"iColumn",
		//			"text":"���������������"
		//		}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"�����ִ���ʧ�ܷ��� undefined��"
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
		//	"text":"���÷�������ı���",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"iItem",
		//			"text":"���������������"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"iColumn",
		//			"text":"���������������"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"ɾ��һ���Լ������",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"iColumn",
		//			"text":"Ҫɾ�����С�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"ɾ��һ�С�",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"iItem",
		//			"text":"��������"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"���һ�С�",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"�е����ơ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[index]",
		//			"text":"�е�λ�ã�-1 ��ȱʡ��ʾ��ӵ����"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"����һ���� CheckBox �����е� Check ״̬��",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"ָ���������С�"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[check]",
		//			"text":"Check ���� uncheck��ȱʡ�� true��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"��ȡָ�������е� Check ״̬��",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"��������"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� check ״̬��ʧ�ܷ��� undefined��"
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
		//	"text":"����ָ����Ϊѡ��",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"��������"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true ��ʧ�ܷ��� undefined��"
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
		//	"text":"ָ�����Ƿ���ѡ��״̬��ListView �����Ƕ�ѡ��",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"��������"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"����һ����ʾѡ��״̬�Ĳ���ֵ��ʧ�ܷ��� undefined��"
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
		//	"text":"��ȡ��Щ���Ǳ�ѡ�еģ�start ��������֮����У����Կ��԰ѷ���ֵ��Ϊ�������������������������ȡ����ѡ����������",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"[start]",
		//			"text":"�������֮��������ȱʡֵ�� -1����Ϊ 0 �ǲ�������һ�еģ���"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"����ѡ���е����������û������������ -1��ʧ�ܷ��� undefined��"
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
		//	"text":"���һ�С��������ɾ�����ݵ�ʱ�򣬿��������ص�ǰ�ؼ�����ʾ����ɺ�����ʾ������������Ӻ�ɾ�����ܼ�����",
		//	"param":[
		//		{
		//			"type":"string|array",
		//			"name":"text",
		//			"text":"�������������ִ����������һ�У��������õ�һ�е�ֵ������ִ������������������飬���һ�У����������������ֵ����ÿһ�е��ı���"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[index]",
		//			"text":"��ӵ�λ�ã�-1 ��ȱʡ����ʾ��ӵ����"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"����һ���ص��������� CEdit �ؼ����ı������仯��������������С�",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"undefined",
		//		"text":"�������û�з���ֵ��"
		//	}
		//}
		//,{
		//	"type":"property",
		//	"name":"checkBox",
		//	"objtype":"boolean",
		//	"text":"�ؼ��Ƿ���ÿ�еĵ�һ����ʾһ�� CheckBox��ȱʡֵ false��"
		//}
		//,{
		//	"type":"property",
		//	"name":"rowSelect",
		//	"objtype":"boolean",
		//	"text":"���б�ѡ�е�ʱ�������л��ǵ�һ����ʾѡ�У�Ĭ��ֵ�� false���ı�������ԣ������ڿؼ��Ѿ�������ɺ����ò���Ч��"
		//}
		//,{
		//	"type":"property",
		//	"name":"gridLine",
		//	"objtype":"boolean",
		//	"text":"�Ƿ���ʾ�����ߣ�Ĭ��ֵ�� false���ı�������ԣ������ڿؼ��Ѿ�������ɺ����ò���Ч��"
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
	//	"text":"CStatusBar ��װһ�� Windows ��׼ StatusBar �ؼ���CStatusBar �̳��� CCtrl��",
	//	"member":[//*
	class BStatusBar : public BWnd<cs::StatusBar>{};
	class JsStatusBar : public JsWndObject<BStatusBar,JsStatusBar,TEMPLATE_ID_STATUSBAR>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CTrackBar",
	//	"text":"CTrackBar ��װһ�� Windows ��׼ TrackBar �ؼ���CTrackBar �̳��� CCtrl��",
	//	"member":[//*
	class BTrackBar : public BWnd<cs::TrackBar>{};
	class JsTrackBar : public JsWndObject<BTrackBar,JsTrackBar,TEMPLATE_ID_TRACKBAR>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CUpDown",
	//	"text":"CUpDown ��װһ�� Windows ��׼ UpDown �ؼ���CUpDown �̳��� CCtrl��",
	//	"member":[//*
	class BUpDown : public BWnd<cs::UpDown>{};
	class JsUpDown : public JsWndObject<BUpDown,JsUpDown,TEMPLATE_ID_UPDOWN>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CHeader",
	//	"text":"CHeader ��װһ�� Windows ��׼ Header �ؼ���CHeader �̳��� CCtrl��",
	//	"member":[//*
	class BHeader : public BWnd<cs::Header>{};
	class JsHeader : public JsWndObject<BHeader,JsHeader,TEMPLATE_ID_HEADER>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CLink",
	//	"text":"CLink ��װһ�� Windows ��׼ Link �ؼ���CLink �̳��� CCtrl��",
	//	"member":[//*
	class BLink : public BWnd<cs::Link>{};
	class JsLink : public JsWndObject<BLink,JsLink,TEMPLATE_ID_LINK>{};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CIpAddress",
	//	"text":"CIpAddress ��װһ�� Windows ��׼ IpAddress �ؼ���CIpAddress �̳��� CCtrl��",
	//	"member":[//*
	class BIpAddress : public BWnd<cs::IpAddress>{};
	class JsIpAddress : public JsWndObject<BIpAddress,JsIpAddress,TEMPLATE_ID_IPADDRESS>{};

	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"LayoutParam",
	//	"text":"LayoutParam �Ǵ��ڲ���ϵͳ��Layout���Ĳ��ֲ�����",
	//	"member":[
	//	{
	//		"type":"property",
	//		"name":"margin",
	//		"objtype":"Rect",
	//		"text":"���ֵı߽硣"
	//	},
	//	{
	//		"type":"property",
	//		"name":"weight",
	//		"objtype":"float",
	//		"text":"Ȩ��ֵ��ÿ�����ֵĴ�С��Ȩ�ط��䡣"
	//	},
	//	{
	//		"type":"property",
	//		"name":"length",
	//		"objtype":"integer",
	//		"text":"Layout �Ĺ��г��ȣ�Layout �ĳߴ���Ȩ�ط���ĳ��ȼ��Ϲ��г��ȡ�"
	//	},
	//	{
	//		"type":"property",
	//		"name":"style",
	//		"objtype":"integer",
	//		"text":"���ֵķ�����λ 0 ��ʾ��ֱ���֣�1 ��ʾˮƽ���֡�"
	//	},
	//	{
	//		"type":"function",
	//		"name":"setVertical()",
	//		"text":"���õ�ǰ Layout ����Ϊ��ֱ����ģʽ����ֱ����ģʽ���� Layout ����ֱ��ʽ���С�",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"����û�з���ֵ��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"setHorizontal()",
	//		"text":"���õ�ǰ Layout ����Ϊˮƽ����ģʽ��ˮƽ����ģʽ���� Layout ��ˮƽ��ʽ���С�",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"����û�з���ֵ��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"isHorizontal()",
	//		"text":"��ǰ�����Ƿ�Ϊˮƽ����ģʽ��",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"����һ������ֵ��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"isVertical()",
	//		"text":"��ǰ�����Ƿ�Ϊ��ֱ����ģʽ��Layout �ĳ�ʼ״̬�Ǵ�ֱģʽ��",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"����һ������ֵ��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"reset()",
	//		"text":"���ò��ֲ���Ϊ��ʼ״̬��length = 0��weight = 1��margin = ��0,0,0,0����style = 0����ֱ���֣���",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"����û�з���ֵ��"
	//		}
	//	}
	//]}
	//,{
	//	"type":"class",
	//	"name":"Layout",
	//	"text":"Layout ��һ�����ڲ���ϵͳ��",
	//	"remark":[
	//		"����ϵͳ�������£�",
	//		"1. �����������ǳ������ڣ����������߽磬����weight��length���ԡ���Ϊ����Layoutֻ����һ����",
	//		"2. һ�����ֵĶ���Ӳ��ְ����������Horizontal����Vertical������ˮƽ����ֱ˳�����С�",
	//		"3. �Ӳ��ֵĴ�С�����¹���ˮƽ���֣������Ӳ��ֵĸ���һ���ģ���ֱ���֣������Ӳ��ֵĿ���һ���ģ����Ǽ��ϱ߽��Ⱥ���������֡�ˮƽ����ʱ�Ӳ��ֵĿ�ȷ���ʹ�ֱ�����Ӳ��ֵĸ߶ȷ��䣬����ѭ�ȷ���ÿ���Ӳ���ָ�����Ⱥͱ߽�ֵ��ʣ��ߴ簴Ȩ�أ�weight�������ÿ���Ӳ��ֵ�ԭ��ע�⣺��ʣ��ߴ�Ϊ���������������Ȩ����Сÿ���Ӳ��ֵĳߴ硣",
	//		"4. �����Ӳ���Ȩ���ܺ�Ϊ 0 �������С��0.00001�����Ӳ��ֳߴ簴ָ�����ȷ��䣬�����Ӹ����ֵĳߴ磬��������������ܵ����Ӳ��ֳ��������ֵ����Ρ�",
	//		"��ǰ���ֳߴ�������ȡ����ͬ���Ĳ��ֵĲ�����Param���͸����ֵĳߴ磬�������������Ӳ��ֵ�Ӱ�죬Ҳ�����Ӳ��ֵ��κβ�������Ӱ�츸���ֵĳߴ硣",
	//		"���磺������ʵ�ʷֵĵĳ���Ϊ110����2���Ӳ��֣��Ӳ��ֵĳ��ȶ���10��Ȩ�طֱ�Ϊ1.0��2.0����ô��110 - ��10 + 10�� = 90�����90���ǰ�Ȩ�ط���Ŀɵ��ߴ磬��������Ȩ��1.0�Ĳ��֣��ֵ�30��Ȩ��2.0�Ĳ��ֵַ�60�����ǵ�ʵ�ʳߴ�ֱ�Ϊ10+30��10+60��������ǵı߽�ֵ������ 0 ����ô��Ҫ��90�����ȥ�߽�ֵ��ʣ�µĳߴ��ȥ���䡣"
	//	],
	//	"member":[
	//	{
	//		"type":"property",
	//		"name":"param",
	//		"objtype":"LayoutParam",
	//		"text":"Layout �Ĳ��ֲ�����"
	//	},
	//	{
	//		"type":"property",
	//		"name":"name",
	//		"objtype":"string",
	//		"text":"���ֵ����ƣ�����ʹ�� getByName ����ָ�����ƵĲ��֡�"
	//	},
	//	{
	//		"type":"property",
	//		"name":"childs",
	//		"objtype":"array",
	//		"text":"�Ӳ������飬ÿ��Ԫ�ض���һ�����ֶ���ʹ�� add��delete��remove��addin�Ⱥ�����Ӻ��Ƴ��Ӷ��󣬲�Ҫֱ�Ӹ������顣����ʹ������� indexOf ���������Ӷ��������ֵ��"
	//	},
	//	{
	//		"type":"function",
	//		"name":"getByName(name)",
	//		"text":"��ȡ��ǰ Layout ���������⼶������ name ���Ժ�ָ����������ͬ���ֻ���ص�һ�ҵ����",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"name",
	//				"text":"Ҫ���ҵ�������ơ�"
	//			}
	//		],
	//		"return":{
	//			"type":"Layout",
	//			"text":"����ҵ���ָ�����Ƶ�������������򷵻� undefined��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"ptInLayout(pt)",
	//		"text":"���һ�����Ƿ������ Layout ���ڲ���",
	//		"param":[
	//			{
	//				"type":"Point",
	//				"name":"pt",
	//				"text":"Ҫ���ĵ㡣"
	//			}
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"����һ������ֵ��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"reset()",
	//		"text":"���õ�ǰ Layout ����Ϊ��ʼ״̬���⽫ɾ�������е�����������еİ󶨵Ŀؼ���",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"����û�з���ֵ��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"parent()",
	//		"text":"��ȡ��ǰ Layout �ĸ�����û�и������ undefined��",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"Layout",
	//			"text":"����һ�� Layout�������ǰ����û�и������ undefined��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"setControl(className,id,text,style,exstyle)",
	//		"text":"���õ�ǰ����Ŀؼ����������������ԭ�Ȱ󶨵Ŀؼ����ؼ������������ɣ�ֱ������ refresh �������ؼ��Ż����ɡ�",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"className",
	//				"text":"�ؼ������࣬���������һ���մ�����Ч�Ŀؼ��������ᱻ��Ϊ��һ����ͨ�� View ����"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"id",
	//				"text":"ָ���ؼ��� id��"
	//			},
	//			{
	//				"type":"string",
	//				"name":"text",
	//				"text":"�ؼ����ı���"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"style",
	//				"text":"�ؼ��� style ���ԡ�"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"exstyle",
	//				"text":"�ؼ��� exStyle ���ԡ�"
	//			}
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"����û�з���ֵ��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"refresh(hwnd)",
	//		"text":"ˢ�µ�ǰ Layout �������ӿؼ����ؼ�����������֡�ֻ��Ҫ�ڶ��� Layout �������һ�����������",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"hwnd",
	//				"text":"�������ְ󶨵� owner ���ھ����"
	//			}
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"����û�з���ֵ��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"move(len)",
	//		"text":"�ƶ���ǰ���֣��������������ͨ������ weight ���ı䵱ǰ���ֵ�λ�ã����������ǰ���ֵ�ͬ������ֻ��һ����ʱ�����Լ��������޷��ƶ��ġ�",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"len",
	//				"text":"�ƶ��ľ��롣"
	//			}
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"����û�з���ֵ��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"add([index])",
	//		"text":"����Ӳ��֣�index ȱʡ�����������λ�ã��Ӳ��ֱ���ӵ�ĩβ��",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"[index]",
	//				"text":"����λ�ã�-1 ��ȱʡ��ʾ��ӵ�ĩβ��"
	//			}
	//		],
	//		"return":{
	//			"type":"Layout",
	//			"text":"��������ӵ��� Layout ����"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"del(index)",
	//		"text":"ɾ��һ���Ӳ��ֶ����Ӳ���������˴��ڣ��ᱻ���١�",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"index",
	//				"text":"ɾ����λ�ã������� Layout ��������Χ������ʧ�ܡ�"
	//			}
	//		],
	//		"return":{
	//			"type":"bool",
	//			"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"remove(index)",
	//		"text":"�Ƴ�һ���Ӳ��֣������Ӳ��ֲ��ᱻɾ�����󶨵Ĵ���Ҳ�������٣���������Ŀ��������� Layout �����Ա���ӵ�ĳ��λ�ã�Ҳ���ǿ����ƶ��� Layout ��λ�á�",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"index",
	//				"text":"�Ƴ�������λ�ã�������Χ��������ʧ�ܡ�"
	//			}
	//		],
	//		"return":{
	//			"type":"Layout",
	//			"text":"�ɹ������Ƴ����� Layout ����ʧ�ܷ���undefined��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"addin(lay,[index])",
	//		"text":"��",
	//		"param":[
	//			{
	//				"type":"Layout",
	//				"name":"lay",
	//				"text":"Ҫ��ӵ� Layout ����"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"[index]",
	//				"text":"��ӵ�λ�ã�����Χ�ᱻ��ӵ�ĩβ��"
	//			}
	//		],
	//		"return":{
	//			"type":"bool",
	//			"text":"�ɹ�����true��ʧ�ܷ���undefined��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"toJson()",
	//		"text":"����һ���������������Ա�����Ϊ json �ִ���",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"object",
	//			"text":"����һ���ض���ʽ�Ķ���"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"parseJson(json)",
	//		"text":"��һ�������� Layout ��Ϣ�Ķ����ʼ����ǰ Layout����ǰ Layout ���ȱ�����Ϊ��ʼ״̬��",
	//		"param":[
	//			{
	//				"type":"object",
	//				"name":"json",
	//				"text":"�����˲�����Ϣ�� json ����"
	//			}
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"����û�з���ֵ��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"load(file,[code])",
	//		"text":"��һ�������ļ����ز��֡�",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"file",
	//				"text":"�����ļ�����"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"[code]",
	//				"text":"�����ļ�ʹ�õı��룬Ĭ���� utf-8��"
	//			}
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"���سɹ����� true�����򷵻� false��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"save(file,[code])",
	//		"text":"���浱ǰ���ֵ�һ���ļ����ļ��� json ��ʽ���档",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"file",
	//				"text":"Ҫ������ļ�����"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"[code]",
	//				"text":"�ļ�ʹ�õı��룬Ĭ���� utf-8��"
	//			}
	//		],
	//		"return":{
	//			"type":"Layout",
	//			"text":"�ɹ����� true��ʧ�ܷ��� false��"
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