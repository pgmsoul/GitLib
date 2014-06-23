#include "stdafx.h"
#include "v8base.h"
#include "resource.h"

namespace v8{
	class JsLayout{
	public:
		static LPCWSTR CreateCtrlObject(LPCWSTR className){
			if(cs::WcsLength(className)==0){
				CONASSERT(L"className is null");
				return 0;
			}else if(cs::WcsEqualNoCase(className,L"Static")){
				return L"CLabel";
				//return new Label;
			}else if(cs::WcsEqualNoCase(className,L"Button")){
				return L"CButton";
				//return new Button;
			}else if(cs::WcsEqualNoCase(className,L"Edit")){
				return L"CEdit";
				//return new Edit;
			}else if(cs::WcsEqualNoCase(className,L"SysDateTimePick32")){
				return L"CDateTimePicker";
				//return new DateTimePicker;
			}else if(cs::WcsEqualNoCase(className,L"SysIPAddress32")){
				return L"CIpAddress";
				//return new IpAddress;
			}else if(cs::WcsEqualNoCase(className,L"SysLink")){
				return L"CLink";
				//return new Link;
			}else if(cs::WcsEqualNoCase(className,L"SysHeader32")){
				return L"CHeader";
				//return new Header;
			}else if(cs::WcsEqualNoCase(className,L"msctls_updown32")){
				return L"CUpDown";
				//return new UpDown;
			}else if(cs::WcsEqualNoCase(className,L"msctls_trackbar32")){
				return L"CTrackBar";
				//return new TrackBar;
			}else if(cs::WcsEqualNoCase(className,L"msctls_statusbar32")){
				return L"CStatusBar";
				//return new StatusBar;
			}else if(cs::WcsEqualNoCase(className,L"SysListView32")){
				return L"CListView";
				//return new ListView;
			}else if(cs::WcsEqualNoCase(className,L"msctls_hotkey32")){
				return L"CHotKey";
				//return new HotKey;
			}else if(cs::WcsEqualNoCase(className,L"SysTabControl32")){
				return L"CTabPage";
				//return new TabPage;
			}else if(cs::WcsEqualNoCase(className,L"msctls_progress32")){
				return L"CProgressBar";
				//return new ProgressBar;
			}else if(cs::WcsEqualNoCase(className,L"ToolbarWindow32")){
				return L"CToolBar";
				//return new ToolBar;
			}else if(cs::WcsEqualNoCase(className,L"SysTreeView32")){
				return L"CTreeView";
				//return new TreeView;
			}else if(cs::WcsEqualNoCase(className,L"ComboBoxEx32")){
				return L"CComboBoxEx";
				//return new ComboBoxEx;
			}else if(cs::WcsEqualNoCase(className,L"ComboBox")){
				return L"CComboBox";
				//return new ComboBox;
			}else if(cs::WcsEqualNoCase(className,L"ListBox")){
				return L"CMonthCalendar";
				//return new ListBox;
			}else if(cs::WcsEqualNoCase(className,L"SysMonthCal32")){
				return L"CMonthCalendar";
				//return new MonthCalendar;
			}
			_ASSERT_EXPR(0,L"ICtrl.CreateCtrlObject not find class name");
			return 0;
		}

		static Handle<Value> _createCtrlObject(const Arguments& args){
			HandleScope store;
			while(true){
				Handle<Object> self = args.This();
				cs::String className;
				GetString(args[0],className);
				if(className.IsNull()) break;
				LPCWSTR jsName = CreateCtrlObject(className);
				cs::String js;
				if(jsName==0){
					js.Format(L"new CView()");
				}else{
					js.Format(L"new %s();",jsName);
				}
				Handle<Object> control = LoadJsCode(js,L"_createCtrlObject")->ToObject();
				return store.Close(control);
			}
			return Undefined();
		}
		static Handle<Value> _createControl(const Arguments& args){
			HandleScope store;
			while(true){
				Handle<Object> self = args.This();
				HWND parent = GET_ARGS_INT2(0,0,HWND);
				if(!parent) break;
				Handle<Value> ctrl = self->Get(String::New("control"));
				if(ctrl.IsEmpty()) break;
				if(IsTemplate(ctrl,TEMPLATE_ID_WND)){
					Handle<Object> control = ctrl->ToObject();
					Handle<Value> prop = control->Get(String::New("create"));
					Handle<Function> create = Handle<Function>::Cast(prop);
					Handle<Value> argv[1];
					argv[0] = Uint32::New((UINT_PTR)parent);
					CallFunc(control,create,1,argv);
				}else break;
				return True();
			}
			return Undefined();
		}
	};
	void LoadLayout(Handle<Object>& glb){
		HandleScope store;
		LoadJsRes(IDR_JS_LAYOUT,L"Layout.js");
		Handle<Function> layoutparam = GetObjProperty<Function>(glb,L"LayoutParam");
		GetEnv()->SetFunc(layoutparam,FUNC_ID_LAYOUTPARAM);
		Handle<Function> layout = GetObjProperty<Function>(glb,L"Layout");
		GetEnv()->SetFunc(layout,FUNC_ID_LAYOUT);
		Handle<Object> proto = layout->Get(String::New("prototype"))->ToObject();
		SET_OBJ_FUNC_RO(proto,_createCtrlObject,JsLayout::_createCtrlObject);
		SET_OBJ_FUNC_RO(proto,_createControl,JsLayout::_createControl);
	}

}