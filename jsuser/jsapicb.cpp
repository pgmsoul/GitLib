#include "stdafx.h"
#include "v8base.h"

namespace v8{
	class CallBack : public cs::_class{
	public:
		Persistent<Function> func;
		cs::Function<uint>* proc;
		cs::Event			wait;
		CallBack():proc(0){
			wait.Create(1,0);
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
	//	"text":"CProc ��ʵ��������Ϊ C �������ض���ʽ�Ļص�������JavaScript ��������������ͨ C �����Ļص�����ĳЩ API ������Ҫ�ص������ķ�ʽ����ɹ��ܡ�",
	//	"remark":[
	//		"CProc ֧�ֵ� C �ص����������Ƿ���ֵΪ 32 λ���� void��ÿ������Ҳ������ 32 λ�����Ҳ����������ܳ��� 5 ��������� C �ص��������������������"
	//	],
	//	"member":[//*
	class JsApiCallback : public JsWrapObject<CallBack,JsApiCallback,TEMPLATE_ID_API_CALLBACK>{
	protected:
		//*{
		//		"type":"function",
		//		"name":"create(argn,func)",
		//		"text":"����һ���ص����������ҷ�������",
		//		"param":[
		//			{
		//				"type":"integer",
		//				"name":"argn",
		//				"text":"��Ӧ�� C �ص������Ĳ������������ֵ������ 0 �� 5��Ŀǰ��֧�ֳ��� 5 �������Ļص�����������Ͷ�Ӧ�� C �ص������ĸ�ʽ�ϸ�ƥ�䣨���������Ͳ���������ȫ���ϣ�������ص�����������ʱ���ᵼ�³��������"
		//			},
		//			{
		//				"type":"function",
		//				"name":"func",
		//				"text":"���ص�����������ʱ�����պ��е� JavaScript ��������� JavaScript ����������ʱ��ÿ������������һ����������ÿ�������������������ھ���ĺ�����"
		//			}
		//		],
		//		"return":{
		//			"type":"integer",
		//			"text":"�ɹ�����һ��ָ�룬���ָ������ڲ��� C ���������Դ�����Ӧ�� Api ������ʧ�ܷ��� undefined��"
		//		},
		//		"remark":[
		//			"create ��������Ѿ��ɹ����ٴε���ʱ����ʧ�ܣ����� undefined����������ظ�ʹ��������󣬿��Ե��� close �������ͷ��ڲ���Դ���ٵ��� create��",
		//			"��� create �ķ���ֵû�б��棬������ʵ�������� proc ��ȡ��"
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
		//		"text":"�ͷ��ڲ���Դ���������������Ȼ�����ٴ�ʹ�ã����� create ������������ close �� dispose ������",
		//		"param":[
		//		],
		//		"return":{
		//			"type":"boolean",
		//			"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//		},
		//		"remark":[
		//			"���� clsoe ����֮ǰ�����뱣֤�ص��������ٱ����У������������������"
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
			typedef struct PARAM0 : public cs::_struct{
				uint ret;
				Persistent<Object>* self;
				Persistent<Function>* func;
				cs::Event* wait;
			}PARAM0;
			static void _cb0(void* param){
				PARAM0* p = (PARAM0*)param;
				HandleScope stack;
				Handle<Value> argv[1];
				Handle<Value> ret = CallFunc(*p->self,*p->func,0,argv);
				p->ret = ret->Uint32Value();
				p->wait->Signal();
			}
			uint cb0(){
				PARAM0 param;
				param.wait = &wait;
				param.self = &self;
				param.func = &func;
				if(GetEnv()->IsMainThread()){
					_cb0(&param);
				}else{
					GetEnv()->queue.PostCallback(_cb0,&param);
					wait.Wait();
				}
				return param.ret;
			}
			typedef struct PARAM1 : public cs::_struct{
				uint ret;
				uint param1;
				Persistent<Object>* self;
				Persistent<Function>* func;
				cs::Event* wait;
			}PARAM1;
			static void _cb1(void* param){
				PARAM1* p = (PARAM1*)param;
				HandleScope stack;
				Handle<Value> argv[1];
				argv[0] = Uint32::New(p->param1);
				Handle<Value> ret = CallFunc(*p->self,*p->func,1,argv);
				p->ret = ret->Uint32Value();
				p->wait->Signal();
			}
			uint cb1(uint p1){
				PARAM1 param;
				param.wait = &wait;
				param.self = &self;
				param.func = &func;
				param.param1 = p1;
				if(GetEnv()->IsMainThread()){
					_cb1(&param);
				}else{
					GetEnv()->queue.PostCallback(_cb1,&param);
					wait.Wait();
				}
				return param.ret;
			}
			typedef struct PARAM2 : public cs::_struct{
				uint ret;
				uint param1,param2;
				Persistent<Object>* self;
				Persistent<Function>* func;
				cs::Event* wait;
			}PARAM2;
			static void _cb2(void* param){
				PARAM2* p = (PARAM2*)param;
				HandleScope stack;
				Handle<Value> argv[1];
				argv[0] = Uint32::New(p->param1);
				argv[1] = Uint32::New(p->param2);
				Handle<Value> ret = CallFunc(*p->self,*p->func,2,argv);
				p->ret = ret->Uint32Value();
				p->wait->Signal();
			}
			uint cb2(uint p1,uint p2){
				PARAM2 param;
				param.wait = &wait;
				param.self = &self;
				param.func = &func;
				param.param1 = p1;
				param.param2 = p2;
				if(GetEnv()->IsMainThread()){
					_cb2(&param);
				}else{
					GetEnv()->queue.PostCallback(_cb2,&param);
					wait.Wait();
				}
				return param.ret;
			}
			typedef struct PARAM3 : public cs::_struct{
				uint ret;
				uint param1,param2,param3;
				Persistent<Object>* self;
				Persistent<Function>* func;
				cs::Event* wait;
			}PARAM3;
			static void _cb3(void* param){
				PARAM3* p = (PARAM3*)param;
				HandleScope stack;
				Handle<Value> argv[3];
				argv[0] = Uint32::New(p->param1);
				argv[1] = Uint32::New(p->param2);
				argv[2] = Uint32::New(p->param3);
				Handle<Value> ret = CallFunc(*p->self,*p->func,3,argv);
				p->ret = ret->Uint32Value();
				p->wait->Signal();
			}
			uint cb3(uint p1,uint p2,uint p3){
				PARAM3 param;
				param.wait = &wait;
				param.self = &self;
				param.func = &func;
				param.param1 = p1;
				param.param2 = p2;
				param.param3 = p3;
				if(GetEnv()->IsMainThread()){
					_cb3(&param);
				}else{
					GetEnv()->queue.PostCallback(_cb3,&param);
					wait.Wait();
				}
				return param.ret;
			}
			typedef struct PARAM4 : public cs::_struct{
				uint ret;
				uint param1,param2,param3,param4;
				Persistent<Object>* self;
				Persistent<Function>* func;
				cs::Event* wait;
			}PARAM4;
			static void _cb4(void* param){
				PARAM4* p = (PARAM4*)param;
				HandleScope stack;
				Handle<Value> argv[4];
				argv[0] = Uint32::New(p->param1);
				argv[1] = Uint32::New(p->param2);
				argv[2] = Uint32::New(p->param3);
				argv[3] = Uint32::New(p->param4);
				Handle<Value> ret = CallFunc(*p->self,*p->func,4,argv);
				p->ret = ret->Uint32Value();
				p->wait->Signal();
			}
			uint cb4(uint p1,uint p2,uint p3,uint p4){
				PARAM4 param;
				param.wait = &wait;
				param.self = &self;
				param.func = &func;
				param.param1 = p1;
				param.param2 = p2;
				param.param3 = p3;
				param.param4 = p4;
				if(GetEnv()->IsMainThread()){
					_cb4(&param);
				}else{
					GetEnv()->queue.PostCallback(_cb4,&param);
					wait.Wait();
				}
				return param.ret;
			}
			typedef struct PARAM5 : public cs::_struct{
				uint ret;
				uint param1,param2,param3,param4,param5;
				Persistent<Object>* self;
				Persistent<Function>* func;
				cs::Event* wait;
			}PARAM5;
			static void _cb5(void* param){
				PARAM5* p = (PARAM5*)param;
				HandleScope stack;
				Handle<Value> argv[5];
				argv[0] = Uint32::New(p->param1);
				argv[1] = Uint32::New(p->param2);
				argv[2] = Uint32::New(p->param3);
				argv[3] = Uint32::New(p->param4);
				argv[4] = Uint32::New(p->param5);
				Handle<Value> ret = CallFunc(*p->self,*p->func,5,argv);
				p->ret = ret->Uint32Value();
				p->wait->Signal();
			}
			uint cb5(uint p1,uint p2,uint p3,uint p4,uint p5){
				PARAM5 param;
				param.wait = &wait;
				param.self = &self;
				param.func = &func;
				param.param1 = p1;
				param.param2 = p2;
				param.param3 = p3;
				param.param4 = p4;
				param.param5 = p5;
				if(GetEnv()->IsMainThread()){
					_cb5(&param);
				}else{
					GetEnv()->queue.PostCallback(_cb5,&param);
					wait.Wait();
				}
				return param.ret;
			}
			typedef struct PARAM6 : public cs::_struct{
				uint ret;
				uint param1,param2,param3,param4,param5,param6;
				Persistent<Object>* self;
				Persistent<Function>* func;
				cs::Event* wait;
			}PARAM6;
			static void _cb6(void* param){
				PARAM6* p = (PARAM6*)param;
				HandleScope stack;
				Handle<Value> argv[6];
				argv[0] = Uint32::New(p->param1);
				argv[1] = Uint32::New(p->param2);
				argv[2] = Uint32::New(p->param3);
				argv[3] = Uint32::New(p->param4);
				argv[4] = Uint32::New(p->param5);
				argv[5] = Uint32::New(p->param6);
				Handle<Value> ret = CallFunc(*p->self,*p->func,6,argv);
				p->ret = ret->Uint32Value();
				p->wait->Signal();
			}
			uint cb6(uint p1,uint p2,uint p3,uint p4,uint p5,uint p6){
				PARAM6 param;
				param.wait = &wait;
				param.self = &self;
				param.func = &func;
				param.param1 = p1;
				param.param2 = p2;
				param.param3 = p3;
				param.param4 = p4;
				param.param5 = p5;
				param.param6 = p6;
				if(GetEnv()->IsMainThread()){
					_cb6(&param);
				}else{
					GetEnv()->queue.PostCallback(_cb6,&param);
					wait.Wait();
				}
				return param.ret;
			}
			/*uint cb7(uint p1,uint p2,uint p3,uint p4,uint p5,uint p6,uint p7){
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
		//		"text":"����һ��ֻ�����ԣ����� C �ص�������ָ�룬���ָ������ĳЩ��Ҫ�ص������Ĳ������������û�гɹ����� create ���������Ѿ����� close ������proc ���� undefined��"
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
		JsApiCallback::Load(glb,L"CProc");
	}
}