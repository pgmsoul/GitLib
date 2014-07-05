#include "stdafx.h"
#include "v8base.h"
#include "resource.h"

namespace v8{
	//*{
	//	"type":"group",
	//	"name":"dialog",
	//	"text":"打开/保存文件对话框，打开文件夹对话框，选择字体对话框，选择颜色对话框。",
	//	"remark":[
	//		"这些对话框都是模态对话框，即如果传递了父窗口参数，直到对话框关闭，焦点无法切换到父窗口。"
	//	],
	//	"member":[
	//{
	//	"type":"class",
	//	"name":"CFileDialog",
	//	"text":"打开/保存文件对话框。",
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
		//	"text":"显示保存文件对话框，文件对话框是一个模态对话框。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"owner",
		//			"text":"父窗口句柄。"
		//		}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"成功返回选择的文件名，失败/或者用户取消了对话框，返回 undefined。"
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
		//	"text":"显示打开文件对话框，文件对话框是一个模态对话框。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"owner",
		//			"text":"父窗口句柄。"
		//		}
		//	],
		//	"return":{
		//		"type":"string|array",
		//		"text":"如果值选择了一个文件，返回选择的文件名；如果是多选模式且选择了一个以上的文件（不包括文件夹，这个函数不会返回文件夹），返回一个数组，数组的 path 属性表示文件所在的文件夹，数组的元素是选择的文件名（不包括路径）。失败/或者用户取消了对话框，返回 undefined。"
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
		//	"text":"添加一个文件类型过滤组，每组可以指定多种类型。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"text",
		//			"text":"在文件类型下拉框里显示的文本。"
		//		},
		//		{
		//			"type":"string",
		//			"name":"types",
		//			"text":"用逗号分隔的文件类型，比如“txt,ini”，“jpg,gif,png”，所有类型用“*”表示。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[index]",
		//			"text":"插入的位置，负数 、缺省或者超出最大索引，添加到最后。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"删除一组文件过滤类型。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"要删除的过滤索引。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，如果索引超界函数会失败，失败返回 undefined。"
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
		//	"text":"设置缺省的文件类型过滤索引。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"指定的索引作为缺省文件类型过滤。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，如果索引超界函数会失败，失败返回 undefined。"
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
		//	"text":"设置对话框的标题。"
		//}
		//,{
		//	"type":"property",
		//	"name":"defExt",
		//	"objtype":"string",
		//	"text":"设置保存对话框的缺省后缀，如果输入框文本没有过滤类型指定的后缀名（即使有其它后缀），则文件名自动添加缺省后缀。"
		//}
		//,{
		//	"type":"property",
		//	"name":"initDir",
		//	"objtype":"string",
		//	"text":"对话框的初始文件夹。"
		//}
		//,{
		//	"type":"property",
		//	"name":"defName",
		//	"objtype":"string",
		//	"text":"缺省文件名。"
		//}
		//,{
		//	"type":"property",
		//	"name":"flag",
		//	"objtype":"integer",
		//	"text":"对话框风格，具体数值查阅 MSDN。"
		//}
		//,{
		//	"type":"property",
		//	"name":"flagEx",
		//	"objtype":"boolean",
		//	"text":"是否显示左侧图标面板。"
		//}
		//,{
		//	"type":"property",
		//	"name":"multiSelect",
		//	"objtype":"boolean",
		//	"text":"是否允许文件多选，仅适用于打开对话框，默认 false。"
		//}
		//,{
		//	"type":"function",
		//	"name":"onFileOk(filename)",
		//	"text":"回调函数，当用户选择了文件的时候触发。",
		//	"param":[
		//		{
		//			"type":"string|array",
		//			"name":"filename",
		//			"text":"这个参数和 showOpen 函数的返回值相同，如果值选择了一个文件，返回选择的文件名；如果是多选模式且选择了一个以上的文件（不包括文件夹，这个函数不会返回文件夹），返回一个数组，数组的 path 属性表示文件所在的文件夹，数组的元素是选择的文件名（不包括路径）。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"如果返回 true，对话框将保持打开。"
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
	//	"text":"字体选择对话框。",
	//	"member":[//*
	class JsFontDialog : public JsWrapObject<cs::FontDialog,JsFontDialog,TEMPLATE_ID_FONTDIALOG>{
		class CBObject : public WrapObject{
		public:
			//*{
			//	"type":"function",
			//	"name":"onApply(logfont)",
			//	"text":"当点击了对话框的应用按钮或者确定按钮时，这个函数被回调。",
			//	"param":[
			//		{
			//			"type":"integer",
			//			"name":"logfont",
			//			"text":"一个包含字体参数的指针，可以通过 CFont 对象的 setLogfont 函数设置字体。"
			//		}
			//	],
			//	"return":{
			//		"type":"void",
			//		"text":"函数没有返回值。"
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
		//	"text":"显示字体选择对话框，字体选择对话框是一个模态对话框。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"owner",
		//			"text":"父窗口句柄。"
		//		}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"成功返回 true，失败/或者用户取消了对话框，返回 undefined。"
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
		//	"text":"设置对话框的初始状态和获取对话框选择的字体。"
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
	//	"text":"显示一个选则文件夹对话框，它是一个模态对话框。",
	//	"param":[
	//		{
	//			"type":"integer",
	//			"name":"owner",
	//			"text":"父窗口句柄。"
	//		},
	//		{
	//			"type":"string",
	//			"name":"[init]",
	//			"text":"设置对话框的初始位置。"
	//		},
	//		{
	//			"type":"string",
	//			"name":"[title]",
	//			"text":"对话框的标题。"
	//		}
	//	],
	//	"return":{
	//		"type":"string",
	//		"text":"成功返回选择的文件夹路径，失败/或者用户取消了对话框，返回 undefined。"
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
	//	"text":"显示一个颜色选则对话框，它是一个模态对话框。",
	//	"param":[
	//		{
	//			"type":"integer",
	//			"name":"owner",
	//			"text":"父窗口句柄。"
	//		},
	//		{
	//			"type":"integer",
	//			"name":"[color]",
	//			"text":"指定一个颜色，作为默认选择的颜色。"
	//		}
	//	],
	//	"return":{
	//		"type":"integer",
	//		"text":"成功返回一个选择的颜色，失败/或者用户取消了对话框，返回 undefined。注意：如果返回值为 0 表示黑色，所以不能用 if 直接判断返回值是否有效。"
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