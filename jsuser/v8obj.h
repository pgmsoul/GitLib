#pragma once
/**	C++��̻���֪ʶ
C++�ĺ���˼���Ƕ��󣬷����ǰ󶨵������ϵģ��Զ���Ĳ������ǶԳ�Ա�����ĵ��á�����ϵͳ�ĺ�
��ϵͳ��Դ��ͨ�� C �ķ�ʽ������ C++ �ķ�ʽ�����õġ�Ҳ����ͨ������������һ�������ͨ����Ӧ��
���������������в�����JavaScript ���﷨�������Ƕ���ķ�ʽ������ JavaScript ֻ�й��캯����û
�����������ĸ����Ϊ JavaScript ѹ����û���û��������ٶ���Ļ��ƣ����Բ�����ȫ�հ� C++ �ķ�
ʽ��������һ���棬������������������ҵ�����̵�����������ҵ��������һ���������޹ص�������
��������Ļ����Ƿǳ���Ҫ�ġ����� JavaScript ���Զ���������µĺ�����ǣ��û������ֶ�����ҵ��
����β��������Ϊ JavaScript û�оֲ������ĸ������ִ������޷��� C++ �����Զ����þֲ������
���������������������
Ϊ���ֲ� JavaScript �﷨��������ȱ�ݣ���������� Class ������
**/
namespace v8{
	template<typename R> class JsObject{
	public:
		static Handle<Value> Create(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();
			R::on_create(self,args);
			return store.Close(self);
		}
		static void on_create(Handle<Object>& self,const Arguments& args){}
		static void _set(Local<String> property, Local<Value> value,const AccessorInfo& info){
			HandleScope store;
			Local<Object> self = info.This();
			cs::String name;
			GetString(property,name);
			R::set(name,value,self);
		}
		static Handle<Value> _get(Local<String> property,const AccessorInfo& info){
			HandleScope store;
			Local<Object> self = info.This();
			cs::String name;
			GetString(property,name);
			return store.Close(R::get(name,self));
		}
		static inline void set(cs::String& name,Local<Value>& value,Local<Object>& self){}
		static inline Handle<Value> get(cs::String& name,Local<Object>& self){}
		static inline void init(Handle<FunctionTemplate>& ft){}
		static void Load(Handle<Object>& glb,LPCWSTR name,DWORD tid = 0){
			HandleScope store;
			Handle<FunctionTemplate> ft = FunctionTemplate::New(&R::Create);

			R::init(ft);

			glb->Set(String::New((uint16_t*)name),ft->GetFunction());
			if(tid!=0)
				GetEnv()->SetTemplate(ft,tid);
		}
	};
	//���ذ󶨵�C++��������, ����Ѿ� dispose ��, ���ָ��Ϊ 0 .
	template<typename T> inline bool GetCHandle(T*& handle,Handle<Object>& self){
		handle = (T*)getInternelPointer(self,0);
		return handle!=NULL;
	}
	template<typename T> inline void SetCHandle(T* handle,Handle<Object>& self){
		setInternelPointer(self,0,handle);
	}
	//C++��������
	template<typename T> bool Dispose(Handle<Object>& self){
		T* cobj = (T*)getInternelPointer(self,0);
		if(!cobj) return false;
		setInternelPointer(self,0,0);
		Persistent<Object>* _self = (Persistent<Object>*)getInternelPointer(self,1);
		if(_self){
			setInternelPointer(self,1,0);
			_self->ClearWeak();
			_self->Dispose();
			_self->Clear();
			delete _self;
			delete cobj;
		}
		return true;
	}
	//����������󶨵�C++���󣬺�dispose��ͬ���ǣ��������������C++���󣬶��Ƿ�������JavaScript�����ÿա�
	template<typename T> T* Detach(Handle<Object>& self){
		T* cobj = (T*)getInternelPointer(self,0);
		setInternelPointer(self,0,0);
		Persistent<Object>* _self = (Persistent<Object>*)getInternelPointer(self,1);
		if(_self){
			setInternelPointer(self,1,0);
			_self->ClearWeak();
			_self->Dispose();
			_self->Clear();
			delete _self;
		}
		return cobj;
	}
	/*@class CObject
	@text �����ر�ָ��, ����C��ͷ����(���캯��)������һ���ڲ� C++ ����, CObject�����ǵĻ��ࡣ
	@text ����CObject�̳еĶ��� new ��ʱ�򶼿��Դ�һ��Boolean���Ͳ���, ����ָ���Ƿ��������C++����, 
	ȱʡ�� false ʱ������JavaScript�����ʱ��Ҳ�����ɶ�Ӧ��C++����������������Ϊ true, 
	�����������ڲ���C++����, ���ǰ�һ���Ѵ��ڵ�C++����. 
	@text ��������new CObjectһ��ֻ�ɿ��ڲ�������, ����û���JavaScript������ʹ�� new CObject(true)������һ
	������, ����������Զ��һ���ն���(isDisposed()����true).
	@text һ��CObject����ͨ��ִ�� dispose ���ͷ����󶨵���Դ��һ�� dispose �����ã��˶����
	��Ϊһ����ͨ��JavaScript�����ڲ���C++�����޷����ٴθ�ֵ��
	@text ����ͨ��isOwned()�������ж����������͵�CObject����, �˺���ȡ�ص�ֵ������ʱ�Ĳ�����ͬ, ��һ��bool����.
	@text һ��CObjectʵ�������ڶ�ջ����֮�µģ�JavaScript�����Ķ�ջ��ͨ��Auto����������ʵ�ֵġ�һ��
	CObject����������һ��Auto�����ĺ��������ɣ�ͨ������ new ������ô���������󣬴˶���ᱻ�Զ�
	dispose�����ǰ����͵Ķ����ܴ����ƣ�ʵ����һ�������͵�CObject�����һ����ͨ��JavaScript����
	û���κ�����*/
	/*@range_begin*/
	template<typename T,typename R> class JsCObject{
	public:
		//���������������У�����һ����һ��owned���󣬶���һ����û�б� dispose��
		static void weakCallback (Persistent<Value> value, void *data) {
			Persistent<Object>* _self = (Persistent<Object>*)data;
			T* cobj = (T*)getInternelPointer(*_self,0);
			setInternelPointer(*_self,0,0);
			setInternelPointer(*_self,1,0);
			_self->ClearWeak();
			_self->Dispose();
			_self->Clear();
			delete _self;
			delete cobj;
		}
		//��Ĺ��캯����
		static v8::Handle<Value> Create(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();
			if(self==GetGlobal()){
				//������ֱ�ӵ��ù��캯����������ʹ�� new ����һ������
				ThrowException(String::New("use new to create object, do not call constructor direct."));
				return Undefined();
			}
			//�����һ�������� true�����ɿն��� 
			/*if(args.Length()>0&&args[0]->IsTrue()){
				setInternelPointer(self,0,0);
				setInternelPointer(self,1,0);
				return store.Close(self);
			}*/
			T* cobj;
			//�����һ��������һ����������ʾһ������ָ�룬������������C++����
			if(args.Length()>0&&args[0]->IsUint32()){
				uint val = args[0]->Uint32Value();
				if(!(0xFFFF0000&val)&&val!=0){
					ThrowException(String::New("bind pointer is not a valid pointer, high word is zero."));
					return Undefined();
				}
				cobj = (T*)(UINT_PTR)val;
				setInternelPointer(self,0,cobj);
				if((args.Length()<=1||args[1]->IsFalse())&&cobj!=0){
					Persistent<Object>* _self = new Persistent<Object>();
					*_self = Persistent<Object>::New(self);
					setInternelPointer(self,1,_self);
					_self->MakeWeak(_self, weakCallback);
					_self->MarkIndependent();
				}else{
					//����һ��������һ��ָ�룬�ڶ��������� true ��ʱ������һ����װ���͵Ķ���������������ڲ�cpp����
					//ָ���Լ���֪���Ƿ��Ѿ�����װ��һ��ָ��һ��ֻ�ܱ���װһ�Σ������Ǽ򵥵İ�װ��������û�����⴦��
					setInternelPointer(self,1,0);
				}
			}else{//���������������Ч�������µ�C++����
				cobj = new T();
				Persistent<Object>* _self = new Persistent<Object>();
				*_self = Persistent<Object>::New(self);
				setInternelPointer(self,0,cobj);
				setInternelPointer(self,1,_self);
				_self->MakeWeak(_self, weakCallback);
				_self->MarkIndependent();
			}
			if(cobj!=0){
				Handle<Function> push = GetEnv()->GetFunc(FUNC_ID_STACK_PUSH);
				Handle<Value> argv[1];
				argv[0] = self;
				CallFunc(GetGlobal(),push,1,argv);
				R::on_create(self,cobj);
			}

			return store.Close(self);
		}
		/*@function void dispose()
		@text ����һ��CObjec������ڲ�C++����, һ��ִ�����������, ���CObject���󽫻���һ����ͨ��
		JavaScript����, �Ӷ�����ʹ�����ĺ��Ĺ���. һ��ֻ�ж�������ʱ�����Ҫ�����������.
		@text dispose�����ĵ����Ƿǳ���Ҫ��, ����һ�����ڶ�ջ�����µ�CObject����, JavaScript�����޷�
		֪����ЩC++����ʹ���˶���ϵͳ��Դ, ��ΪJavaScript�������ռ����ǲ�ȷ����ʱ������, ��������˴�
		����CObject����, ϵͳ��Դ�ܿ��ܻ�Ѹ�ٺľ�, ����GDI��Դ�Ƿǳ����޵�, ������ԶԶ���ܴ��������ռ���
		������±��ľ�, ���Ա��뼰ʱ���ֶ�������Щ��Դ. ���Auto�����Ķ�ջ����, ���Ա�֤CObject����
		�ں����˳���, ���Զ�����dispose, �Ӷ�ʡȥ�ֶ�������鷳. �������һ�������ڲ���������ʹ���˴�����
		ϵͳ��Դ, �����ֶ�����dispose��ʱ�����õ���Դ.*/
		static Handle<Value> dispose(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();

			T* cobj = (T*)getInternelPointer(self,0);
			R::on_dispose(self,cobj);
			if(cobj){
				setInternelPointer(self,0,0);
			}

			Persistent<Object>* _self = (Persistent<Object>*)getInternelPointer(self,1);
			if(_self){
				setInternelPointer(self,1,0);
				//���cobj�лص��������������汣���� _self �˴��޷�֪ͨ cobj ���� _self ��Ч������������ cobj�������� _self ��
				delete cobj;//�ǰ���
				if(!_self->IsEmpty()){
					_self->ClearWeak();
					_self->Dispose();
					_self->Clear();
				}
				delete _self;
			}
			return True();
		}
		/*@function string toString()
		@text ȡ�ö��������
		@return string ����һ������������Ƶ��ִ�.*/

		/*@function bool isDisposed()
		@text һ��CObject������ڲ�C++ָ���Ƿ�Ϊ�ա�һ��CObject����һ�������� dispose
		���ڲ���C++����ᱻ���ٻ��ͷš�
		@return bool ָʾCObject�����ڲ���C++�����Ƿ���Ч*/
		static Handle<Value> isDisposed(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();
			T* cobj;
			return Bool(!GetCHandle<T>(cobj,self));
		}
		/*@function bool isOwned()
		@text �ж�һ��CObject�����Ƿ�ӵ�����󶨵��ڲ�C++ָ��, ���������Ƕ������dispose
		��ʱ���Ƿ�ɾ�����ָ�롣
		@return ����˺������� true����ʾ�������Լ������ڲ�C++������Դ��ִ��dispose
		����ʱ��C++���󱻻��գ��󶨱���Ϊ�գ��������false����ʾ���������������
		����C++��Դ��ִ�� dispose �����������ǰ��ڲ��󶨵�C++������Ϊ�գ������˵
		�Ǹ�C++��Դʵ�����Ǳ����JavaScript�������ġ�������� undefined����ʾ��
		�����Ѿ��� dispose ���ˣ��������������͡�*/
		static Handle<Value> isOwned(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();
			return Bool(0!=getInternelPointer(self,1));
		}
		/*@function integer obj()
		@text ��ȡ�ڲ�C++����ľ����
		@return integer �����ڲ������ָ��, ��������Ѿ� dispose������ 0 .*/
		static Handle<Value> obj(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();
			T* cobj;
			GetCHandle(cobj,self);
			return store.Close(Uint32::New((UINT_PTR)cobj));
		}
		/*@function bool attach(handle)
		@text ��һ����Դ����󶨵����������, �Ӷ�����ʹ���������ķ����������в���.
		@param handle -- integer
		@text Ҫ�󶨵ľ��, ÿ�ֶ����Ӧ�ľ��������ȷ.
		@return bool ָʾ�����Ƿ�ɹ�*/
		static Handle<Value> bind(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1||!args[0]->IsUint32()) break;
				Local<Object> self = args.This();
				T* cobj;
				if(GetCHandle(cobj,self)){
					R::on_dispose(self,cobj);
					delete cobj;
				}
				cobj = GET_ARGS_INT2(0,0,T*);
				SetCHandle(cobj,self);

				if(GET_ARGS_VALUE(1,false,Boolean)||cobj==0){
					Persistent<Object>* _self = (Persistent<Object>*)getInternelPointer(self,1);
					if(_self){
						setInternelPointer(self,1,0);
						if(!_self->IsEmpty()){
							_self->ClearWeak();
							_self->Dispose();
							_self->Clear();
						}
						delete _self;
					}
				}
				return True();
			}
			return Undefined();
		}
		/*@function integer detach()
		@text ��һ������ľ���������������.
		@return integer ������Դ���, �������һ��ϵͳ��Դ, ��Ҫ�ֶ�������.*/
		static Handle<Value> debind(const Arguments& args){
			HandleScope store;
			while(true){
				Local<Object> self = args.This();
				T* cobj = (T*)getInternelPointer(self,0);
				if(cobj){
					setInternelPointer(self,0,0);
				}

				Persistent<Object>* _self = (Persistent<Object>*)getInternelPointer(self,1);
				if(_self){
					setInternelPointer(self,1,0);
					if(!_self->IsEmpty()){
						_self->ClearWeak();
						_self->Dispose();
						_self->Clear();
					}
					delete _self;
				}
				return store.Close(Uint32::New((UINT_PTR)cobj));
			}
			return Undefined();
		}
		static void _set(Local<String> property, Local<Value> value,const AccessorInfo& info){
			HandleScope store;
			Local<Object> self = info.This();
			T* cobj;
			if(!GetCHandle(cobj,self)) return;
			cs::String name;
			GetString(property,name);
			R::set(name,cobj,value,self);
		}
		static Handle<Value> _get(Local<String> property,const AccessorInfo& info){
			HandleScope store;
			Local<Object> self = info.This();
			T* cobj;
			if(!GetCHandle(cobj,self)) return Undefined();
			cs::String name;
			GetString(property,name);
			return store.Close(R::get(name,cobj,self));
		}
		static inline void set(cs::String& name,T* cobj,Local<Value>& value,Local<Object>& self){}
		static inline Handle<Value> get(cs::String& name,T* cobj,Local<Object>& self){}
		static inline void init(Handle<FunctionTemplate>& ft){}
		static inline void on_create(Handle<Object>& self,T* cobj){}
		static inline void on_dispose(Handle<Object>& self,T* cobj){}
		//��ʼ���������.
		static void Load(Handle<Object>& glb,LPCWSTR name,int tid,Persistent<FunctionTemplate>* inh = 0){
			HandleScope store;
			Handle<FunctionTemplate> ft = FunctionTemplate::New(&R::Create);
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			Handle<ObjectTemplate> inst = ft->InstanceTemplate();  
			//��Ҫ��inst��������ԣ����򣬳�ʼ��һ������new����ʱ���ڲ����ڵ���Create֮ǰ��
			//�����������÷�����
			inst->SetInternalFieldCount(2);

			R::init(ft);

			func->Set(String::New("dispose"),FunctionTemplate::New(&R::dispose),ReadOnly);
			cs::String tss;
			tss.Format(L"new Function(\"{return '%s';}\")",name);
			Handle<Value> ts = LoadJsCode(tss);
			func->Set(String::New("toString"),ts,ReadOnly);
			func->Set(String::New("isCObject"),True(),ReadOnly);
			SET_CLA_FUNC(isDisposed);
			SET_CLA_FUNC(isOwned);
			SET_CLA_FUNC(obj);
			SET_CLA_FUNC(debind);
			SET_CLA_FUNC(bind);

			if(inh)
				ft->Inherit(*inh);
			glb->Set(String::New((uint16_t*)name),ft->GetFunction());
			if(tid!=0)
				GetEnv()->SetTemplate(ft,tid);
		}
	};
	/*@range_end*/
	/*@class CHandleObject
	@text �����̳���CObject, ���󶨵�C++��һ����ͬ�㣬����C++���󶼺���һ���ڲ���Դָ�롣
	������ĸ�������
	@text isNull()���ڲ���Դָ���Ƿ�Ϊ�ա�
	@text handle()���ڲ���Դָ������
	@text attach(handle)����һ����Դָ�롣
	@text detach()��������󶨵���Դָ��ġ�
	@text ���һ��CHandleObject������ڲ�C++����Ϊ�գ���������ȫ������undefined��*/
	/*@range_begin*/
	template<typename T,typename H,typename R> class JsHandleObject : public JsCObject<T,R>{
	public:
		/*@function bool isNull()
		@text ��Դ�����һ��������һ���յ���Դ���һ���� 0 �����ǲ���������ˣ������е�ʱ����-1�ȵȣ�������Ҫ������������жϾ���Ƿ���Ч��
		@return bool ָʾ�ڲ�C++����󶨵���Դ����Ƿ���Ч�����C++������Ϊ�գ����� undefined��*/
		static Handle<Value> isNull(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();
			T* handle;
			if(!GetCHandle(handle,self)){
				return Undefined();
			}
			return Bool(handle->IsNull());
		}
		/*@function integer handle()
		@return integer �ڲ�����ľ��*/
		static Handle<Value> handle(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();
			T* handle;
			if(!GetCHandle(handle,self)){
				return Undefined();
			}
			return store.Close(Uint32::New((UINT_PTR)handle->Handle()));
		}
		/*@function bool attach(handle)
		@text ��һ����Դ����󶨵����������, �Ӷ�����ʹ���������ķ����������в���.
		@param handle -- integer
		@text Ҫ�󶨵ľ��, ÿ�ֶ����Ӧ�ľ��������ȷ.
		@return bool ָʾ�����Ƿ�ɹ�*/
		static Handle<Value> attach(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1||!args[0]->IsUint32()) break;
				Local<Object> self = args.This();
				T* handle;
				if(!GetCHandle(handle,self)) break;
				handle->Attach((H)(UINT_PTR)args[0]->Uint32Value());
				return True();
			}
			return Undefined();
		}
		/*@function integer detach()
		@text ��һ������ľ���������������.
		@return integer ������Դ���, �������һ��ϵͳ��Դ, ��Ҫ�ֶ�������.*/
		static Handle<Value> detach(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1||!args[0]->IsUint32()) break;
				Local<Object> self = args.This();
				T* handle;
				if(!GetCHandle(handle,self)) break;
				UINT_PTR h = (UINT_PTR)handle->Detach();
				return store.Close(Uint32::New(h));
			}
			return Undefined();
		}
		static void Load(Handle<Object>& glb,LPCWSTR name,int tid,Persistent<FunctionTemplate>* inh = 0){
			HandleScope store;
			Handle<FunctionTemplate> ft = FunctionTemplate::New(&R::Create);
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			Handle<ObjectTemplate> inst = ft->InstanceTemplate();  
			R::init(ft);

			func->Set(String::New("dispose"),FunctionTemplate::New(&R::dispose),ReadOnly);
			cs::String tss;
			tss.Format(L"new Function(\"{return '%s';}\")",name);
			Handle<Value> ts = LoadJsCode(tss);
			func->Set(String::New("toString"),ts,ReadOnly);
			func->Set(String::New("isCObject"),True(),ReadOnly);
			SET_CLA_FUNC(isDisposed);
			SET_CLA_FUNC(isOwned);
			SET_CLA_FUNC(obj);
			SET_CLA_FUNC(debind);
			SET_CLA_FUNC(bind);
			SET_CLA_FUNC(handle);
			SET_CLA_FUNC(isNull);
			SET_CLA_FUNC(attach);
			SET_CLA_FUNC(detach);

			inst->SetInternalFieldCount(2);
			if(inh)
				ft->Inherit(*inh);
			glb->Set(String::New((uint16_t*)name),ft->GetFunction());
			GetEnv()->SetTemplate(ft,tid);
		}
	};
	/*@range_end*/
	//JsWrapObject ���ܼ̳У���ֻ�ܰ�װ�������ࡣ
	template<typename T,typename R> class JsWrapObject{
	public:
		class WrapObject : public T{
		public:
			Persistent<Object> self;
			~WrapObject(){
				if(self.IsEmpty()) return;
				setInternelPointer(self,0,0);
				self.ClearWeak();
				self.Dispose();
				self.Clear();
			}
		};
		static void weakCallback (Persistent<Value> value, void *data) {
			delete (WrapObject*)data;
		}
		static v8::Handle<Value> Create(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();
			if(self==GetGlobal()){
				//������ֱ�ӵ��ù��캯����������ʹ�� new ����һ������
				ThrowException(String::New("use new to create object, do not call constructor direct."));
				return Undefined();
			}
			WrapObject* cobj = new WrapObject();
			cobj->self = Persistent<Object>::New(self);
			setInternelPointer(self,0,cobj);
			cobj->self.MakeWeak(cobj,weakCallback);
			cobj->self.MarkIndependent();

			Handle<Function> push = GetEnv()->GetFunc(FUNC_ID_STACK_PUSH);
			Handle<Value> argv[1];
			argv[0] = self;
			CallFunc(GetGlobal(),push,1,argv);
			R::on_create(self,cobj);
			return store.Close(self);
		}
		static Handle<Value> dispose(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();

			WrapObject* cobj = (WrapObject*)getInternelPointer(self,0);
			R::on_dispose(self,cobj);
			delete cobj;
			return True();
		}
		static void Dispose(Handle<Object>& self){
			WrapObject* cobj = (WrapObject*)getInternelPointer(self,0);
			R::on_dispose(self,cobj);
			delete cobj;
		}
		static Handle<Value> isDisposed(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();
			WrapObject* cobj;
			return Bool(!GetCHandle(cobj,self));
		}
		static Handle<Value> obj(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();
			WrapObject* cobj;
			GetCHandle(cobj,self);
			return store.Close(Uint32::New((UINT_PTR)cobj));
		}
		static void _set(Local<String> property, Local<Value> value,const AccessorInfo& info){
			HandleScope store;
			Local<Object> self = info.This();
			WrapObject* cobj;
			if(!GetCHandle(cobj,self)) return;
			cs::String name;
			GetString(property,name);
			R::set(name,cobj,value,self);
		}
		static Handle<Value> _get(Local<String> property,const AccessorInfo& info){
			HandleScope store;
			Local<Object> self = info.This();
			WrapObject* cobj;
			if(!GetCHandle(cobj,self)) return Undefined();
			cs::String name;
			GetString(property,name);
			return store.Close(R::get(name,cobj,self));
		}
		static inline void set(cs::String& name,WrapObject* cobj,Local<Value>& value,Local<Object>& self){}
		static inline Handle<Value> get(cs::String& name,WrapObject* cobj,Local<Object>& self){}
		static inline void init(Handle<FunctionTemplate>& ft){}
		static inline void on_create(Handle<Object>& self,WrapObject* cobj){}
		static inline void on_dispose(Handle<Object>& self,WrapObject* cobj){}
		//��ʼ���������.
		static void Load(Handle<Object>& glb,LPCWSTR name,int tid,Persistent<FunctionTemplate>* inh = 0){
			HandleScope store;
			Handle<FunctionTemplate> ft = FunctionTemplate::New(&R::Create);
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			Handle<ObjectTemplate> inst = ft->InstanceTemplate();  
			//��Ҫ��inst��������ԣ����򣬳�ʼ��һ������new����ʱ���ڲ����ڵ���Create֮ǰ��
			//�����������÷�����
			inst->SetInternalFieldCount(1);

			R::init(ft);

			func->Set(String::New("dispose"),FunctionTemplate::New(&R::dispose),ReadOnly);
			cs::String tss;
			tss.Format(L"new Function(\"{return '%s';}\")",name);
			Handle<Value> ts = LoadJsCode(tss);
			func->Set(String::New("toString"),ts,ReadOnly);
			func->Set(String::New("isCObject"),True(),ReadOnly);
			SET_CLA_FUNC(isDisposed);
			SET_CLA_FUNC(obj);

			if(inh)
				ft->Inherit(*inh);
			glb->Set(String::New((uint16_t*)name),ft->GetFunction());
			if(tid!=0)
				GetEnv()->SetTemplate(ft,tid);
		}
	};
}