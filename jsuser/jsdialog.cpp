#include "stdafx.h"
#include "v8base.h"
#include "resource.h"

namespace v8{
	//*{
	//	"type":"group",
	//	"name":"dialog",
	//	"text":"��/�����ļ��Ի��򣬴��ļ��жԻ���ѡ������Ի���ѡ����ɫ�Ի���",
	//	"remark":[
	//		"��Щ�Ի�����ģ̬�Ի��򣬼���������˸����ڲ�����ֱ���Ի���رգ������޷��л��������ڡ�"
	//	],
	//	"member":[
	//{
	//	"type":"class",
	//	"name":"CFileDialog",
	//	"text":"��/�����ļ��Ի���",
	//	"member":[//*
	class JsFileDialog : public JsWrapObject<cs::FileDialog,JsFileDialog,TEMPLATE_ID_FILEDIALOG>{
		class CBObject : public WrapObject{
		public:
			bool onFileOk(LPOPENFILENAME pfn,cs::FileDialog*){
				HandleScope stack;
				v8::Handle<Value> prop = self->GetHiddenValue(NEW_STR(onFileOk));
				if(!prop->IsFunction()) return false;
				v8::Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(prop);

				int offset = 0;
				LPCWSTR fn = pfn->lpstrFile;
				cs::IntegerList<LPCWSTR> files;
				while(true){
					int len = cs::WcsLength(fn);
					if(len==0) break;
					files.Add(fn);
					fn += len + 1;
				}

				v8::Handle<Value> argv[1];
				int count = files.Count();
				if(count==1){
					argv[0] = NEW_WSTR(files[0]);
				}else{
					v8::Handle<Object> a = Array::New();
					a->Set(NEW_STR(path),NEW_WSTR(files[0]));
					for(uint i=1;i<files.Count();i++){
						a->Set(i-1,NEW_WSTR(files[i]));
					}
					argv[0] = a;
				}
				v8::Handle<Value> rst = CallFunc(self,func,1,argv);

				return rst->BooleanValue();
			}
		};
		//*{
		//	"type":"function",
		//	"name":"showSave(owner)",
		//	"text":"��ʾ�����ļ��Ի����ļ��Ի�����һ��ģ̬�Ի���",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"owner",
		//			"text":"�����ھ����"
		//		}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"�ɹ�����ѡ����ļ�����ʧ��/�����û�ȡ���˶Ի��򣬷��� undefined��"
		//	}
		//}//*
		static Handle<Value> showSave(const Arguments& args){
			HandleScope stack;
			while(true){
				Handle<Object> self = args.This();
				WrapObject* cobj;
				if(!GetCHandle(cobj,self)) break;

				HWND owner = GET_ARGS_INT2(0,0,HWND);
				if(!cobj->ShowSave(owner)) break;

				return stack.Close(NEW_WSTR(cobj->GetFileName()));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"showSave(owner)",
		//	"text":"��ʾ���ļ��Ի����ļ��Ի�����һ��ģ̬�Ի���",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"owner",
		//			"text":"�����ھ����"
		//		}
		//	],
		//	"return":{
		//		"type":"string|array",
		//		"text":"���ֵѡ����һ���ļ�������ѡ����ļ���������Ƕ�ѡģʽ��ѡ����һ�����ϵ��ļ����������ļ��У�����������᷵���ļ��У�������һ�����飬����� path ���Ա�ʾ�ļ����ڵ��ļ��У������Ԫ����ѡ����ļ�����������·������ʧ��/�����û�ȡ���˶Ի��򣬷��� undefined��"
		//	}
		//}//*
		static Handle<Value> showOpen(const Arguments& args){
			HandleScope stack;
			while(true){
				Handle<Object> self = args.This();
				WrapObject* cobj;
				if(!GetCHandle(cobj,self)) break;

				HWND owner = GET_ARGS_INT2(0,0,HWND);
				if(!cobj->ShowOpen(owner)) break;

				uint count = cobj->GetSelectCount();
				if(count==1){
					return stack.Close(NEW_WSTR(cobj->GetFileName()));
				}else{
					Handle<Array> files = Array::New(count);
					files->Set(NEW_STR(path),NEW_WSTR(cobj->GetFileName(0)));
					count++;
					for(uint i=1;i<count;i++){
						files->Set(i-1,NEW_WSTR(cobj->GetFileName(i)));
					}
					return stack.Close(files);
				}
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"addFilter(text,types,[index])",
		//	"text":"���һ���ļ����͹����飬ÿ�����ָ���������͡�",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"text",
		//			"text":"���ļ���������������ʾ���ı���"
		//		},
		//		{
		//			"type":"string",
		//			"name":"types",
		//			"text":"�ö��ŷָ����ļ����ͣ����硰txt,ini������jpg,gif,png�������������á�*����ʾ��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[index]",
		//			"text":"�����λ�ã����� ��ȱʡ���߳��������������ӵ����"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> addFilter(const Arguments& args){
			HandleScope stack;
			while(true){
				if(args.Length()<2) break;
				Handle<Object> self = args.This();
				WrapObject* cobj;
				if(!GetCHandle(cobj,self)) break;

				cs::String text,type;
				GetString(args[0],text);
				GetString(args[1],type);
				int index = GET_ARGS_INT(2,-1);
				cs::StringList sl;
				int count = sl.SplitString(type,L",");
				type = L"";
				for(int i=0;i<count;i++){
					if(type.Length()) type += L";";
					type += L"*.";
					type += sl[i];
				}
				cobj->AddFilter(text,type,index);
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"deleteFilter(index)",
		//	"text":"ɾ��һ���ļ��������͡�",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"Ҫɾ���Ĺ���������"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true������������纯����ʧ�ܣ�ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> deleteFilter(const Arguments& args){
			HandleScope stack;
			while(true){
				if(args.Length()<1) break;
				Handle<Object> self = args.This();
				WrapObject* cobj;
				if(!GetCHandle(cobj,self)) break;

				int index = GET_ARGS_INT(0,0);
				if(!cobj->DeleteFilter(index)) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setDefFilter(index)",
		//	"text":"����ȱʡ���ļ����͹���������",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"ָ����������Ϊȱʡ�ļ����͹��ˡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true������������纯����ʧ�ܣ�ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> setDefFilter(const Arguments& args){
			HandleScope stack;
			while(true){
				if(args.Length()<1) break;
				Handle<Object> self = args.This();
				WrapObject* cobj;
				if(!GetCHandle(cobj,self)) break;

				int index = GET_ARGS_INT(0,0);
				if(!cobj->SetFilterIndex(index)) break;
				return True();
			}
			return Undefined();
		}
	public:
		//*,{
		//	"type":"property",
		//	"name":"title",
		//	"objtype":"string",
		//	"text":"���öԻ���ı��⡣"
		//}
		//,{
		//	"type":"property",
		//	"name":"defExt",
		//	"objtype":"string",
		//	"text":"���ñ���Ի����ȱʡ��׺�����������ı�û�й�������ָ���ĺ�׺������ʹ��������׺�������ļ����Զ����ȱʡ��׺��"
		//}
		//,{
		//	"type":"property",
		//	"name":"initDir",
		//	"objtype":"string",
		//	"text":"�Ի���ĳ�ʼ�ļ��С�"
		//}
		//,{
		//	"type":"property",
		//	"name":"defName",
		//	"objtype":"string",
		//	"text":"ȱʡ�ļ�����"
		//}
		//,{
		//	"type":"property",
		//	"name":"flag",
		//	"objtype":"integer",
		//	"text":"�Ի����񣬾�����ֵ���� MSDN��"
		//}
		//,{
		//	"type":"property",
		//	"name":"flagEx",
		//	"objtype":"boolean",
		//	"text":"�Ƿ���ʾ���ͼ����塣"
		//}
		//,{
		//	"type":"property",
		//	"name":"multiSelect",
		//	"objtype":"boolean",
		//	"text":"�Ƿ������ļ���ѡ���������ڴ򿪶Ի���Ĭ�� false��"
		//}
		//,{
		//	"type":"function",
		//	"name":"onFileOk(filename)",
		//	"text":"�ص����������û�ѡ�����ļ���ʱ�򴥷���",
		//	"param":[
		//		{
		//			"type":"string|array",
		//			"name":"filename",
		//			"text":"��������� showOpen �����ķ���ֵ��ͬ�����ֵѡ����һ���ļ�������ѡ����ļ���������Ƕ�ѡģʽ��ѡ����һ�����ϵ��ļ����������ļ��У�����������᷵���ļ��У�������һ�����飬����� path ���Ա�ʾ�ļ����ڵ��ļ��У������Ԫ����ѡ����ļ�����������·������"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"������� true���Ի��򽫱��ִ򿪡�"
		//	}
		//}//*
		static Handle<Value> get(cs::String& name,WrapObject* cobj,Local<Object>& self){
			if(name==L"title"){
				return NEW_WSTR(cobj->Title.Handle());
			}else if(name==L"defExt"){
				return NEW_WSTR(cobj->DefaultExt.Handle());
			}else if(name==L"defName"){
				return NEW_WSTR(cobj->DefaultFile.Handle());
			}else if(name==L"initDir"){
				return NEW_WSTR(cobj->InitDir.Handle());
			}else if(name==L"flag"){
				return Uint32::New(cobj->Flags);
			}else if(name==L"flagEx"){
				return Boolean::New(cobj->FlagsEx);
			}else if(name==L"onFileOk"){
				return self->GetHiddenValue(NEW_STR(onFileOk));
			}else if(name==L"multiSelect"){
				return Boolean::New((cobj->Flags&OFN_ALLOWMULTISELECT)!=0);
			}
			return Undefined();
		}
		static void set(cs::String& name,WrapObject* cobj,Local<Value>& value,Local<Object>& self){
			if(name==L"title"){
				GetString(value,cobj->Title);
			}else if(name==L"defExt"){
				GetString(value,cobj->DefaultExt);
			}else if(name==L"defName"){
				GetString(value,cobj->DefaultFile);
			}else if(name==L"initDir"){
				GetString(value,cobj->InitDir);
			}else if(name==L"flag"){
				cobj->Flags = value->Uint32Value();
			}else if(name==L"flagEx"){
				cobj->FlagsEx = value->BooleanValue();
			}else if(name==L"onFileOk"){
				if(value->IsFunction()){
					if(cobj->OnFileOk.IsNull()||cobj->OnFileOk.IsEmpty())
						cobj->OnFileOk.Bind((CBObject*)cobj,&CBObject::onFileOk);
				}else{
					cobj->OnFileOk.Empty();
				}
				self->SetHiddenValue(NEW_STR(onFileOk),value);
			}else if(name==L"multiSelect"){
				cobj->Flags |= OFN_ALLOWMULTISELECT;
				if(!value->BooleanValue())
					cobj->Flags -= OFN_ALLOWMULTISELECT;
			}
		}
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			SET_CLA_FUNC(showOpen);
			SET_CLA_FUNC(showSave);
			SET_CLA_FUNC(addFilter);
			SET_CLA_FUNC(deleteFilter);
			SET_CLA_FUNC(setDefFilter);

			SET_CLA_ACCESSOR(title);
			SET_CLA_ACCESSOR(defExt);
			SET_CLA_ACCESSOR(defName);
			SET_CLA_ACCESSOR(initDir);
			SET_CLA_ACCESSOR(flag);
			SET_CLA_ACCESSOR(multiSelect);
			SET_CLA_ACCESSOR(flagEx);
			SET_CLA_ACCESSOR(onFileOk);
		}
	};
	//*]}//*
	//*,{
	//	"type":"class",
	//	"name":"CFontDialog",
	//	"text":"����ѡ��Ի���",
	//	"member":[//*
	class JsFontDialog : public JsWrapObject<cs::FontDialog,JsFontDialog,TEMPLATE_ID_FONTDIALOG>{
		class CBObject : public WrapObject{
		public:
			//*{
			//	"type":"function",
			//	"name":"onApply(logfont)",
			//	"text":"������˶Ի����Ӧ�ð�ť����ȷ����ťʱ������������ص���",
			//	"param":[
			//		{
			//			"type":"integer",
			//			"name":"logfont",
			//			"text":"һ���������������ָ�룬����ͨ�� CFont ����� setLogfont �����������塣"
			//		}
			//	],
			//	"return":{
			//		"type":"void",
			//		"text":"����û�з���ֵ��"
			//	}
			//}//*
			void onApply(LPLOGFONT plf,cs::FontDialog*){
				HandleScope stack;
				v8::Handle<Value> prop = self->GetHiddenValue(NEW_STR(onApply));
				if(!prop->IsFunction()) return;
				v8::Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(prop);

				v8::Handle<Value> argv[1];
				argv[0] = Uint32::New((UINT_PTR)plf);
				CallFunc(self,func,1,argv);
			}
		};
		//*,{
		//	"type":"function",
		//	"name":"show(owner)",
		//	"text":"��ʾ����ѡ��Ի�������ѡ��Ի�����һ��ģ̬�Ի���",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"owner",
		//			"text":"�����ھ����"
		//		}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"�ɹ����� true��ʧ��/�����û�ȡ���˶Ի��򣬷��� undefined��"
		//	}
		//}//*
		static Handle<Value> show(const Arguments& args){
			HandleScope stack;
			while(true){
				Handle<Object> self = args.This();
				WrapObject* cobj;
				if(!GetCHandle(cobj,self)) break;

				HWND owner = GET_ARGS_INT2(0,0,HWND);
				if(!cobj->Show(owner,1,1)) break;

				return True();
			}
			return Undefined();
		}
	public:
		//*,{
		//	"type":"property",
		//	"name":"logfont",
		//	"objtype":"integer",
		//	"text":"���öԻ���ĳ�ʼ״̬�ͻ�ȡ�Ի���ѡ������塣"
		//}//*
		static Handle<Value> get(cs::String& name,WrapObject* cobj,Local<Object>& self){
			if(name==L"logfont"){
				return Uint32::New((UINT_PTR)cobj->LogFont.Handle());
			}else if(name==L"onApply"){
				return self->GetHiddenValue(NEW_STR(onApply));
			}
			return Undefined();
		}
		static void set(cs::String& name,WrapObject* cobj,Local<Value>& value,Local<Object>& self){
			if(name==L"logfont"){
				memcpy(cobj->LogFont.Handle(),(LPLOGFONT)(UINT_PTR)value->Uint32Value(),sizeof(LOGFONT));
			}else if(name==L"onApply"){
				if(value->IsFunction()){
					if(cobj->OnApply.IsNull()||cobj->OnApply.IsEmpty())
						cobj->OnApply.Bind((CBObject*)cobj,&CBObject::onApply);
				}else{
					cobj->OnApply.Empty();
				}
				self->SetHiddenValue(NEW_STR(onApply),value);
			}
		}
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			SET_CLA_FUNC(show);

			SET_CLA_ACCESSOR(logfont);
			SET_CLA_ACCESSOR(onApply);
		}
	};
	//*]}//*

	//*,{
	//	"type":"function",
	//	"name":"showFolderDialog(owner,[init],[title])",
	//	"text":"��ʾһ��ѡ���ļ��жԻ�������һ��ģ̬�Ի���",
	//	"param":[
	//		{
	//			"type":"integer",
	//			"name":"owner",
	//			"text":"�����ھ����"
	//		},
	//		{
	//			"type":"string",
	//			"name":"[init]",
	//			"text":"���öԻ���ĳ�ʼλ�á�"
	//		},
	//		{
	//			"type":"string",
	//			"name":"[title]",
	//			"text":"�Ի���ı��⡣"
	//		}
	//	],
	//	"return":{
	//		"type":"string",
	//		"text":"�ɹ�����ѡ����ļ���·����ʧ��/�����û�ȡ���˶Ի��򣬷��� undefined��"
	//	}
	//}//*
	static Handle<Value> showFolderDialog(const Arguments& args){
		HandleScope stack;
		while(true){
			cs::FolderDialog fd;
			HWND owner = GET_ARGS_INT2(0,0,HWND);
			cs::String title,init;
			if(args.Length()>2) GetString(args[2],title);
			if(args.Length()>1) GetString(args[1],init);
			if(!fd.Show(owner,title,init)) break;
			return stack.Close(NEW_WSTR(fd.GetFolder()));
		}
		return Undefined();
	}
	//*,{
	//	"type":"function",
	//	"name":"showColorDialog(owner,[color])",
	//	"text":"��ʾһ����ɫѡ��Ի�������һ��ģ̬�Ի���",
	//	"param":[
	//		{
	//			"type":"integer",
	//			"name":"owner",
	//			"text":"�����ھ����"
	//		},
	//		{
	//			"type":"integer",
	//			"name":"[color]",
	//			"text":"ָ��һ����ɫ����ΪĬ��ѡ�����ɫ��"
	//		}
	//	],
	//	"return":{
	//		"type":"integer",
	//		"text":"�ɹ�����һ��ѡ�����ɫ��ʧ��/�����û�ȡ���˶Ի��򣬷��� undefined��ע�⣺�������ֵΪ 0 ��ʾ��ɫ�����Բ����� if ֱ���жϷ���ֵ�Ƿ���Ч��"
	//	}
	//}//*
	static Handle<Value> showColorDialog(const Arguments& args){
		HandleScope stack;
		while(true){
			cs::ColorDialog cd;
			HWND owner = GET_ARGS_INT2(0,0,HWND);
			cd.Color = GET_ARGS_INT(1,0);
			if(!cd.Show(owner)) break;

			return stack.Close(Uint32::New(cd.Color));
		}
		return Undefined();
	}
	//*],"source":"D:\\SoftProject\\GitLib\\jsuser\\example\\dialog.jsuser"}//*
	void LoadDialog(Handle<Object>& glb){
		JsFileDialog::Load(glb,L"CFileDialog");
		JsFontDialog::Load(glb,L"CFontDialog");
		SET_OBJ_FUNC_RO(glb,showFolderDialog,showFolderDialog);
		SET_OBJ_FUNC_RO(glb,showColorDialog,showColorDialog);
	}
}