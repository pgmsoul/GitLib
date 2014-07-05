#include "stdafx.h"
#include "v8base.h"

namespace v8{
	class CallBack : public cs::_class{
	public:
		Persistent<Function> func;
		cs::Function<uint>* proc;
		CallBack():proc(0){
		}
		~CallBack(){
			if(!func.IsEmpty()){
				func.Dispose();
			}
			delete proc;
		}
		void Close(){
			if(!func.IsEmpty()){
				func.Dispose();
			}
			delete proc;
			proc = 0;
		}
	};
	//*{
	//	"type":"class",
	//	"name":"CProc",
	//	"text":"CProc 的实例用来作为 C 函数的特定格式的回调函数，JavaScript 函数不能用于普通 C 函数的回调，而某些 API 函数需要回调函数的方式来完成功能。",
	//	"remark":[
	//		"CProc 支持的 C 回调函数必须是返回值为 32 位或者 void，每个参数也必须是 32 位，而且参数个数不能超过 5 个，大多数 C 回调函数都满足这个条件。"
	//	],
	//	"member":[//*
	class JsApiCallback : public JsWrapObject<CallBack,JsApiCallback>{
	protected:
		//*{
		//		"type":"function",
		//		"name":"create(argn,func)",
		//		"text":"生成一个回调函数，并且返回它。",
		//		"param":[
		//			{
		//				"type":"integer",
		//				"name":"argn",
		//				"text":"对应的 C 回调函数的参数个数，这个值必须是 0 到 5，目前不支持超过 5 个参数的回调函数。必须和对应的 C 回调函数的格式严格匹配（参数个数和参数类型完全符合），否则回调函数被呼叫时，会导致程序崩溃。"
		//			},
		//			{
		//				"type":"function",
		//				"name":"func",
		//				"text":"当回调函数被呼叫时，接收呼叫的 JavaScript 函数，这个 JavaScript 函数被呼叫时，每个参数都返回一个正整数，每个正整数的意义依赖于具体的函数。"
		//			}
		//		],
		//		"return":{
		//			"type":"integer",
		//			"text":"成功返回一个指针，这个指针就是内部的 C 函数，可以传给对应的 Api 函数。失败返回 undefined。"
		//		},
		//		"remark":[
		//			"create 函数如果已经成功，再次调用时，会失败（返回 undefined），如果想重复使用这个对象，可以调用 close 函数，释放内部资源后，再调用 create。",
		//			"如果 create 的返回值没有保存，可以用实例的属性 proc 获取。"
		//		]
		//	}//*
		static Handle<Value> create(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<2) break;
				if(!args[1]->IsFunction()) break;
				Handle<Object> self = args.This();
				CBObject* cobj;
				if(!GetCHandle(cobj,args.This())) break;
				int n = GET_ARGS_INT(0,0);
				if(n<0||n>10) break;
				if(cobj->proc) break; 

				if(!cobj->Create(n)) break; 
				cobj->func = Persistent<Function>::New(Handle<Function>::Cast(args[1]));
				Handle<Value> proc = Uint32::New((UINT_PTR)cobj->proc->Procedure());
				self->SetHiddenValue(NEW_STR(proc),proc);
				return store.Close(proc);
			}
			return Undefined();
		}
		//*,{
		//		"type":"function",
		//		"name":"close()",
		//		"text":"释放内部资源，但是这个对象仍然可以再次使用（调用 create 函数），这是 close 和 dispose 的区别。",
		//		"param":[
		//		],
		//		"return":{
		//			"type":"boolean",
		//			"text":"成功返回 true，失败返回 undefined。"
		//		},
		//		"remark":[
		//			"调用 clsoe 函数之前，必须保证回调函数不再被呼叫，否则会引起程序崩溃。"
		//		]
		//	}//*
		static Handle<Value> close(const Arguments& args){
			HandleScope store;
			while(true){
				Handle<Object> self = args.This();
				CBObject* cobj;
				if(!GetCHandle(cobj,args.This())) break;

				cobj->Close();
				self->SetHiddenValue(NEW_STR(proc),Undefined());
				return True();
			}
			return Undefined();
		}
		class CBObject : public WrapObject{
		public:
			bool Create(uint n){
				switch(n){
				case 0:{
					cs::Function<uint>* p = new cs::Function<uint>();
					p->Bind(this,&CBObject::cb0);
					proc = (cs::Function<uint>*)p;
					   }
					   break;
				case 1:{
					cs::Function<uint,uint>* p = new cs::Function<uint,uint>();
					p->Bind(this,&CBObject::cb1);
					proc = (cs::Function<uint>*)p;
					   }
					   break;
				case 2:{
					cs::Function<uint,uint,uint>* p = new cs::Function<uint,uint,uint>();
					p->Bind(this,&CBObject::cb2);
					proc = (cs::Function<uint>*)p;
					   }
					   break;
				case 3:{
					cs::Function<uint,uint,uint,uint>* p = new cs::Function<uint,uint,uint,uint>();
					p->Bind(this,&CBObject::cb3);
					proc = (cs::Function<uint>*)p;
					   }
					   break;
				case 4:{
					cs::Function<uint,uint,uint,uint,uint>* p = new cs::Function<uint,uint,uint,uint,uint>();
					p->Bind(this,&CBObject::cb4);
					proc = (cs::Function<uint>*)p;
					   }
					   break;
				case 5:{
					cs::Function<uint,uint,uint,uint,uint,uint>* p = new cs::Function<uint,uint,uint,uint,uint,uint>();
					p->Bind(this,&CBObject::cb5);
					proc = (cs::Function<uint>*)p;
					   }
					   break;
				/*case 6:{
					cs::Function<uint,uint,uint,uint,uint,uint,uint>* p = new cs::Function<uint,uint,uint,uint,uint,uint,uint>();
					p->Bind(this,&CBObject::cb6);
					proc = (cs::Function<uint>*)p;
					   }
					   break;
				case 7:{
					cs::Function<uint,uint,uint,uint,uint,uint,uint,uint>* p = new cs::Function<uint,uint,uint,uint,uint,uint,uint,uint>();
					p->Bind(this,&CBObject::cb7);
					proc = (cs::Function<uint>*)p;
					   }
					   break;
				case 8:{
					cs::Function<uint,uint,uint,uint,uint,uint,uint,uint,uint>* p = new cs::Function<uint,uint,uint,uint,uint,uint,uint,uint,uint>();
					p->Bind(this,&CBObject::cb8);
					proc = (cs::Function<uint>*)p;
					   }
					   break;
				case 9:{
					cs::Function<uint,uint,uint,uint,uint,uint,uint,uint,uint,uint>* p = new cs::Function<uint,uint,uint,uint,uint,uint,uint,uint,uint,uint>();
					p->Bind(this,&CBObject::cb9);
					proc = (cs::Function<uint>*)p;
					   }
					   break;
				case 10:{
					cs::Function<uint,uint,uint,uint,uint,uint,uint,uint,uint,uint,uint>* p = new cs::Function<uint,uint,uint,uint,uint,uint,uint,uint,uint,uint,uint>();
					p->Bind(this,&CBObject::cb10);
					proc = (cs::Function<uint>*)p;
					   }
					   break;*/
				default:
					return 0;
				}
				return 1;
			}
			uint cb0(){
				HandleScope stack;
				Handle<Value> argv[1];
				Handle<Value> ret = CallFunc(self,func,0,argv);
				return ret->Uint32Value();
			}
			uint cb1(uint p1){
				HandleScope stack;
				Handle<Value> argv[1];
				argv[0] = Uint32::New(p1);
				Handle<Value> ret = CallFunc(self,func,1,argv);
				return ret->Uint32Value();
			}
			uint cb2(uint p1,uint p2){
				HandleScope stack;
				Handle<Value> argv[2];
				argv[0] = Uint32::New(p1);
				argv[1] = Uint32::New(p2);
				Handle<Value> ret = CallFunc(self,func,2,argv);
				return ret->Uint32Value();
			}
			uint cb3(uint p1,uint p2,uint p3){
				HandleScope stack;
				Handle<Value> argv[3];
				argv[0] = Uint32::New(p1);
				argv[1] = Uint32::New(p2);
				argv[2] = Uint32::New(p3);
				Handle<Value> ret = CallFunc(self,func,3,argv);
				return ret->Uint32Value();
			}
			uint cb4(uint p1,uint p2,uint p3,uint p4){
				HandleScope stack;
				Handle<Value> argv[4];
				argv[0] = Uint32::New(p1);
				argv[1] = Uint32::New(p2);
				argv[2] = Uint32::New(p3);
				argv[3] = Uint32::New(p4);
				Handle<Value> ret = CallFunc(self,func,4,argv);
				return ret->Uint32Value();
			}
			uint cb5(uint p1,uint p2,uint p3,uint p4,uint p5){
				HandleScope stack;
				Handle<Value> argv[5];
				argv[0] = Uint32::New(p1);
				argv[1] = Uint32::New(p2);
				argv[2] = Uint32::New(p3);
				argv[3] = Uint32::New(p4);
				argv[4] = Uint32::New(p5);
				Handle<Value> ret = CallFunc(self,func,5,argv);
				return ret->Uint32Value();
			}
			/*uint cb6(uint p1,uint p2,uint p3,uint p4,uint p5,uint p6){
				HandleScope stack;
				Handle<Value> argv[6];
				argv[0] = Uint32::New(p1);
				argv[1] = Uint32::New(p2);
				argv[2] = Uint32::New(p3);
				argv[3] = Uint32::New(p4);
				argv[4] = Uint32::New(p5);
				argv[5] = Uint32::New(p6);
				Handle<Value> ret = CallFunc(self,func,6,argv);
				return ret->Uint32Value();
			}
			uint cb7(uint p1,uint p2,uint p3,uint p4,uint p5,uint p6,uint p7){
				HandleScope stack;
				Handle<Value> argv[7];
				argv[0] = Uint32::New(p1);
				argv[1] = Uint32::New(p2);
				argv[2] = Uint32::New(p3);
				argv[3] = Uint32::New(p4);
				argv[4] = Uint32::New(p5);
				argv[5] = Uint32::New(p6);
				argv[6] = Uint32::New(p7);
				Handle<Value> ret = CallFunc(self,func,7,argv);
				return ret->Uint32Value();
			}
			uint cb8(uint p1,uint p2,uint p3,uint p4,uint p5,uint p6,uint p7,uint p8){
				HandleScope stack;
				Handle<Value> argv[8];
				argv[0] = Uint32::New(p1);
				argv[1] = Uint32::New(p2);
				argv[2] = Uint32::New(p3);
				argv[3] = Uint32::New(p4);
				argv[4] = Uint32::New(p5);
				argv[5] = Uint32::New(p6);
				argv[6] = Uint32::New(p7);
				argv[7] = Uint32::New(p8);
				Handle<Value> ret = CallFunc(self,func,8,argv);
				return ret->Uint32Value();
			}
			uint cb9(uint p1,uint p2,uint p3,uint p4,uint p5,uint p6,uint p7,uint p8,uint p9){
				HandleScope stack;
				Handle<Value> argv[9];
				argv[0] = Uint32::New(p1);
				argv[1] = Uint32::New(p2);
				argv[2] = Uint32::New(p3);
				argv[3] = Uint32::New(p4);
				argv[4] = Uint32::New(p5);
				argv[5] = Uint32::New(p6);
				argv[6] = Uint32::New(p7);
				argv[7] = Uint32::New(p8);
				argv[8] = Uint32::New(p9);
				Handle<Value> ret = CallFunc(self,func,9,argv);
				return ret->Uint32Value();
			}
			uint cb10(uint p1,uint p2,uint p3,uint p4,uint p5,uint p6,uint p7,uint p8,uint p9,uint p10){
				HandleScope stack;
				Handle<Value> argv[10];
				argv[0] = Uint32::New(p1);
				argv[1] = Uint32::New(p2);
				argv[2] = Uint32::New(p3);
				argv[3] = Uint32::New(p4);
				argv[4] = Uint32::New(p5);
				argv[5] = Uint32::New(p6);
				argv[6] = Uint32::New(p7);
				argv[7] = Uint32::New(p8);
				argv[8] = Uint32::New(p9);
				argv[9] = Uint32::New(p10);
				Handle<Value> ret = CallFunc(self,func,10,argv);
				return ret->Uint32Value();
			}*/
		};
	public:
		//*,{
		//		"type":"property",
		//		"name":"proc",
		//		"objtype":"integer",
		//		"text":"这是一个只读属性，返回 C 回调函数的指针，这个指针用于某些需要回调函数的参数。如果对象没有成功调用 create 函数或者已经调用 close 函数，proc 返回 undefined。"
		//	}//*
		static Handle<Value> get(cs::String& name,WrapObject* cobj,Local<Object>& self){
			if(name==L"proc"){
				return self->GetHiddenValue(NEW_STR(proc));
			}
			return Undefined();
		}
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			SET_CLA_FUNC(create);
			SET_CLA_FUNC(close);
			SET_CLA_ACCESSOR(proc);
		}
	};
	//*],"source":"D:\\SoftProject\\GitLib\\jsuser\\example\\cproc.jsuser"}//*
	void LoadAPICallback(Handle<Object>& glb){
		JsApiCallback::Load(glb,L"CProc",TEMPLATE_ID_API_CALLBACK);
	}
}