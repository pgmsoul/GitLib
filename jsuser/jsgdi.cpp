#include "stdafx.h"
#include "v8base.h"
#include "resource.h"

namespace v8{
	void ColorJSFromC(Handle<Value> val,cs::Color* col){
		HandleScope store;
		Local<Object> obj = val->ToObject();
		obj->Set(String::New("red"),Uint32::New(col->red));
		obj->Set(String::New("green"),Uint32::New(col->green));
		obj->Set(String::New("blue"),Uint32::New(col->blue));
		obj->Set(String::New("alpha"),Uint32::New(col->alpha));
	}
	void ColorJSToC(Handle<Value> val,cs::Color* col){
		HandleScope store;
		Local<Object> obj = val->ToObject();
		col->red = obj->Get(String::New("red"))->Uint32Value();
		col->green = obj->Get(String::New("green"))->Uint32Value();
		col->blue = obj->Get(String::New("blue"))->Uint32Value();
		col->alpha = obj->Get(String::New("alpha"))->Uint32Value();
	}
	//*{
	//	"type":"group",
	//	"name":"gdi",
	//	"text":"ͼ�Ρ�ͼ��ͻ�ͼ����ع��ܡ�",
	//	"remark":[
	//		"�������л�ͼ��������Ҫһ�� hdc �������������ͨ������ gdi ����� hdc ���������ݡ�",
	//		"gdi ��Դ��ϵͳ��Դ���ǳ����ޣ�Ҫ��ʡʹ�ã���Ҫͬʱ���ɴ����� gdi ��Դ��ʹ�����ϵͳ��ԴҪ�ͷŵ������� CPen �� CBrush����������ɼ�����ͬ��ɫ�Ķ����ã�Ҳ���Ե��� fromDCPen ���� fromDCBrush��ʹ������ DC ����Ȼ����� setDCColor ���ò�ͬ����ɫ��JavaScript �ǵ��߳�ϵͳ��ͬʱֻ����һ����ͼ�߳������У�ʵ���Ϻ����õ����̵߳� ui��"
	//	],
	//	"member":[
	//{
	//	"type":"class",
	//	"name":"Color([red],[green],[blue],[alpha])",
	//	"text":"��ɫ�࣬����Ϊ�졢�̡�����͸��ͨ����ֵ��ÿ��ͨ����ȡֵ��Χ�� 0����255��ȱʡʱΪ 0��",
	//	"member":[
	//{
	//	"type":"function",
	//	"name":"getIntValue()",
	//	"text":"��ɫ��������һ��32λ�������������������ɫ������ֵ���졢�̡�����͸��ͨ�����δӵ͵���ÿ��ͨ��ռ 8 λ��",
	//	"param":[
	//	],
	//	"return":{
	//		"type":"integer",
	//		"text":"���ص�ǰ����������ɫ������ֵ��"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"setIntValue(color)",
	//	"text":"���õ�ǰ��ɫ���������ֵ��",
	//	"param":[
	//		{
	//			"type":"integer",
	//			"name":"color",
	//			"text":"һ��������ɫ������ֵ��"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"����û�з���ֵ��"
	//	}
	//}
	//,{
	//	"type":"property",
	//	"name":"red",
	//	"objtype":"integer",
	//	"text":"��ǰ��ɫ�ĺ�ɫֵ����Χ�� 0����255�����������һ���������ֵ��getIntValue ��������������ʱ����� &0xFF ������"
	//}
	//,{
	//	"type":"property",
	//	"name":"green",
	//	"objtype":"integer",
	//	"text":"��ǰ��ɫ����ɫֵ����Χ�� 0����255�����������һ���������ֵ��getIntValue ��������������ʱ����� &0xFF ������"
	//}
	//,{
	//	"type":"property",
	//	"name":"blue",
	//	"objtype":"integer",
	//	"text":"��ǰ��ɫ����ɫֵ����Χ�� 0����255�����������һ���������ֵ��getIntValue ��������������ʱ����� &0xFF ������"
	//}
	//,{
	//	"type":"property",
	//	"name":"alpah",
	//	"objtype":"integer",
	//	"text":"��ǰ��ɫ�� alpha ֵ����Χ�� 0����255�����������һ���������ֵ��getIntValue ��������������ʱ����� &0xFF ������"
	//}
	//]}
	//,{
	//	"type":"class",
	//	"name":"CPen",
	//	"text":"�����࣬���̳��� CHandleObject�����ڿ��ƻ���ʱ����ɫ����ȣ���ʽ�ȡ�",
	//	"member":[//*
	class JSPen : public JsHandleObject<cs::Pen,HPEN,JSPen,TEMPLATE_ID_PEN>{
		//*{
		//	"type":"function",
		//	"name":"arc(rect,point1,point2)",
		//	"text":"����һ�λ��ߣ���������ǲ��� 1 ָ���ľ��ε��ڽ���Բ��һ���֣��Ӿ��ε����ĵ������ 2 �Ͳ��� 3 �ֱ�����ֱ�ߣ�����Բ���������㣨����1������2���ֱ��ǻ��ߵ������յ㣬�����ߵķ����ǽ��� 1 ������ 2 ����ʱ�뷽����Ȼ��������� 2 �Ͳ��� 3 ��ͬһ���㣬�����ửһ����������Բ����Ҫ���� hdc ���ԡ�",
		//	"param":[
		//		{
		//			"type":"Rect",
		//			"name":"rect",
		//			"text":"�����Ǵ˾��ε�������Բ��һ���֡�"
		//		},
		//		{
		//			"type":"Point",
		//			"name":"[point1]",
		//			"text":"��������Բ�Ľ����ǻ��ߵ���ʼ�㡣����˲���ȱʡ��������һ����������Բ��"
		//		},
		//		{
		//			"type":"Point",
		//			"name":"[point2]",
		//			"text":"��������Բ�Ľ����ǻ��ߵĽ����㡣����˲���ȱʡ��������һ����������Բ��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> arc(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				Local<Object> self = args.This();
				cs::Pen* handle;
				if(!GetCHandle(handle,self)) break;
				HDC hdc = (HDC)(INT_PTR)self->Get(String::New("hdc"))->Int32Value();
				if(hdc==0) break;
				cs::Rect pr;
				cs::Twin p1,p2;
				if(!RectJsToC(args[0],&pr)) break;
				if(args.Length()>1){
					if(!PointJsToC(args[1],&p1)) break;
					if(args.Length()>2){
						if(!PointJsToC(args[2],&p2)) break;
					}else{
						p2 = p1;
					}
				}else{
					p1.x = pr.left;
					p1.y = pr.top;
					p2 = p1;
				}
				handle->Select(hdc);
				bool r = 0!=::Arc(hdc,pr.left,pr.top,pr.right,pr.bottom,p1.x,p1.y,p2.x,p2.y);
				handle->Select(hdc);
				if(!r) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"arc(rect)",
		//	"text":"����һ�����Ρ���Ҫ���� hdc ���ԡ�",
		//	"param":[
		//		{
		//			"type":"Rect",
		//			"name":"rect",
		//			"text":"Ҫ���Ƶľ��Ρ�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> rect(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				Handle<Object> self = args.This();
				HDC hdc = (HDC)(INT_PTR)self->Get(String::New("hdc"))->Int32Value();
				if(hdc==0) break;
				cs::Rect pr;
				if(!RectJsToC(args[0],&pr)) break;
				cs::Pen* handle;
				if(!GetCHandle(handle,self)) break;
				handle->Select(hdc);
				cs::DC dc;
				dc.Attach(hdc);
				dc.MoveTo(pr.left,pr.top);
				dc.LineTo(pr.right,pr.top);
				dc.LineTo(pr.right,pr.bottom);
				dc.LineTo(pr.left,pr.bottom);
				dc.LineTo(pr.left,pr.top);
				dc.Detach();
				handle->Select(hdc);
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"polyTo(pointArray)",
		//	"text":"�������ɵ㣬�γ�һ�����ߡ���Ҫ���� hdc ���ԡ�",
		//	"param":[
		//		{
		//			"type":"array",
		//			"name":"pointArray",
		//			"text":"һ������ɵ����飬�ҳ��ȴ��� 2 ��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> polyTo(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1||!args[0]->IsArray()) break;

				Handle<Object> self = args.This();
				HDC hdc = (HDC)(INT_PTR)self->Get(String::New("hdc"))->Int32Value();
				if(hdc==0) break;

				cs::Pen* handle;
				if(!GetCHandle(handle,self)) break;

				Local<Array> point = Local<Array>::Cast(args[1]);
				cs::List<cs::Twin> pl;
				uint len = point->Length();
				pl.Expand(len);
				for(uint i=0;i<len;i++){
					Local<Object> vPt = point->Get(i)->ToObject();
					pl[i].x = vPt->Get(String::New("x"))->Int32Value();
					pl[i].y = vPt->Get(String::New("y"))->Int32Value();
				}
				handle->Select(hdc);
				bool r = 0!=PolylineTo(hdc,(LPPOINT)pl.GetHandle(),len);
				handle->Select(hdc);
				if(!r) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"moveTo(x,y)",
		//	"text":"�ƶ���ǰλ�õ�ָ���㣬������������ lineTo ����ʹ�õģ�lineTo �������Ǵӵ�ǰλ�õ�ָ��λ�û�һ���ߡ���Ҫ���� hdc ���ԡ�",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"x",
		//			"text":"x ����"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"y",
		//			"text":"y ����"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> moveTo(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<2) break;
				Handle<Object> self = args.This();
				HDC hdc = (HDC)(INT_PTR)self->Get(String::New("hdc"))->Int32Value();
				if(hdc==0) break;

				int x = args[0]->Int32Value();
				int y = args[1]->Int32Value();
				bool r = ::MoveToEx(hdc,x,y,0)!=0;
				if(!r) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"lineTo(x,y)",
		//	"text":"�ӵ�ǰλ�õ�ָ��λ�û�һ��ֱ�ߡ���Ҫ���� hdc ���ԡ�",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"x",
		//			"text":"x ����"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"y",
		//			"text":"y ����"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> lineTo(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<2) break;
				Handle<Object> self = args.This();
				HDC hdc = (HDC)(INT_PTR)self->Get(String::New("hdc"))->Int32Value();
				if(hdc==0) break;
				int x = args[0]->Int32Value();
				int y = args[1]->Int32Value();
				cs::Pen* handle;
				if(!GetCHandle(handle,self)) break;
				handle->Select(hdc);
				bool r = ::LineTo(hdc,x,y)!=0;
				handle->Select(hdc);
				if(!r) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"drawRect(rect)",
		//	"text":"��һ�����Σ���������Ĳ���������ģʽ��һ���Ǵ� 4 ��������һ���Ǵ�һ�� Rect ������Ҫ���� hdc ���ԡ�",
		//	"param":[
		//		{
		//			"type":"Rect",
		//			"name":"rect",
		//			"text":"һ�����ζ���"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> drawRect(const Arguments& args){
			HandleScope store;
			while(true){
				int left,top,right,bottom;
				if(args.Length()>=4){
					left = args[0]->Int32Value();
					top = args[1]->Int32Value();
					right = args[2]->Int32Value();
					bottom = args[3]->Int32Value();
				}else if(args.Length()>=1){
					//���԰�left�ĵ�ַ����һ��Rect�ĵ�ַ��
					if(!RectJsToC(args[0],(cs::Rect*)&left)) break;
				}else break;
				Handle<Object> self = args.This();
				HDC hdc = (HDC)(INT_PTR)self->Get(String::New("hdc"))->Int32Value();
				if(hdc==0) break;
				cs::Pen* handle;
				if(!GetCHandle(handle,self)) break;
				handle->Select(hdc);
				MoveToEx(hdc,left,top,0);
				LineTo(hdc,right,top);
				LineTo(hdc,right,bottom);
				LineTo(hdc,left,bottom);
				LineTo(hdc,left,top);
				handle->Select(hdc);
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"roundRect(rect,rx,ry)",
		//	"text":"��һ��Բ�Ǿ��Σ��������������ģʽ��һ���Ǵ����ε� 4 ������ֵ����Բ�ǵ������뾶ֵ��һ���Ǵ�һ�����κ�Բ�ǵ������뾶ֵ��",
		//	"param":[
		//		{
		//			"type":"Rect",
		//			"name":"rect",
		//			"text":"һ�����ζ���"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"rx",
		//			"text":"Բ�Ǿ��εĽ�ʵ������һ����Բ�� 1/4�����ֵ����Բ��ˮƽ�뾶��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"ry",
		//			"text":"Բ�Ǿ��εĽ�ʵ������һ����Բ�� 1/4�����ֵ����Բ��ˮƽ�뾶��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> roundRect(const Arguments& args){
			HandleScope store;
			while(true){
				int left,top,right,bottom,width,height;
				if(args.Length()>=6){
					left = args[0]->Int32Value();
					top = args[1]->Int32Value();
					right = args[2]->Int32Value();
					bottom = args[3]->Int32Value();
					width = args[4]->Int32Value();
					height = args[5]->Int32Value();
				}else if(args.Length()>=3){
					//���԰�left�ĵ�ַ����һ��Rect�ĵ�ַ��
					if(!RectJsToC(args[0],(cs::Rect*)&left)) break;
					width = args[1]->Int32Value();
					height = args[2]->Int32Value();
				}else break;
				Handle<Object> self = args.This();
				HDC hdc = (HDC)(INT_PTR)self->Get(String::New("hdc"))->Int32Value();
				if(hdc==0) break;
				cs::Pen* handle;
				if(!GetCHandle(handle,self)) break;
				handle->Select(hdc);
				HBRUSH hb = (HBRUSH)::SelectObject(hdc,(HBRUSH)::GetStockObject(NULL_BRUSH));
				bool r = RoundRect(hdc,left,top,right,bottom,width,height)!=0;
				::SelectObject(hdc,hb);
				handle->Select(hdc);
				if(!r) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getInfo()",
		//	"text":"��ȡ�������ɫ����ȣ���� 3 ��������",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"object",
		//		"text":"����ֵ�� width��color��style 3 �����Դ���ǰ����Ķ�Ӧ������"
		//	}
		//}//*
		static Handle<Value> getInfo(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();
			Handle<Object> info = Object::New();
			cs::Pen* handle;
			if(!GetCHandle(handle,self)) return Undefined();
			LOGPEN lp;
			if(!handle->GetLogPen(&lp)) return Undefined();
			info->Set(String::New("color"),Int32::New(lp.lopnColor));
			info->Set(String::New("style"),Int32::New(lp.lopnStyle));
			info->Set(String::New("width"),Int32::New(lp.lopnWidth.x));
			return store.Close(info);
		}
		//*,{
		//	"type":"function",
		//	"name":"create([color],[width],[style])",
		//	"text":"����ϵͳ���ʡ�",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"[color]",
		//			"text":"ָ�����ʵ���ɫ��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[width]",
		//			"text":"ָ�����ʵĴ�ϸ��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[style]",
		//			"text":"ָ�����ʵ���ʽ���style ���õ�ֵ����: <br/>CPen.SOLID(ʵ�ߣ�������õ���ʽ��Ҳ��ȱʡֵ��<br/>CPen.DASH(����)<br/>CPen.DOT(��)<br/>CPen.DASHDOT(���ߺ͵㽻��)<br/>CPen.DASHDOTDOT(һ��������)<br/>CPen.NULL(�ջ��ʣ�û���κ���ʾ)"
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
				Local<Object> self = args.This();
				cs::Pen* handle;
				if(!GetCHandle(handle,self)) break;

				int len = args.Length();
				int color = 0;
				if(len>0) color = args[0]->Int32Value();
				int width = 1;
				if(len>1) width = args[1]->Int32Value();
				int style = 0;
				if(len>3) style = args[2]->Int32Value();
				bool r = handle->Create(color,style,width);
				if(!r) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"fromDCPen()",
		//	"text":"���õ�ǰ����Ϊ DC_PEN��DC_PEN ��ϵͳ���õ� Pen�����ܱ����٣����� destroy û��Ч����ʹ�� DC_PEN Ч�ʻ��һЩ��������Ϊ���������µ� Pen����ʡ��ϵͳ��Դ������ͨ�� setDCColor �������� DC_PEN ����ɫ�������޷����Ŀ�Ⱥ���ʽ��DC_PEN ����һ�����ؿ�ȵ� solid ʵ�� ���ʡ�",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"fromStockPen(stock)",
		//	"text":"��ȡϵͳ���õĻ��ʶ��󣬲���΢����ĵ���ȡϵͳ���õĻ��ʲ���ֵ��ϵͳ���û��ʲ��ᱻ���١�",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"[stock]",
		//			"text":"���û��ʵĴ��룬ȱʡ��ȡ DC_PEN��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> fromStockPen(const Arguments& args){
			HandleScope store;
			while(true){
				Local<Object> self = args.This();
				cs::Pen* handle;
				if(!GetCHandle(handle,self)) break;
				int sp = GET_ARGS_INT(0,DC_PEN);
				bool r = handle->Attach((HPEN)::GetStockObject(sp));
				if(!r) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setDCColor([color])",
		//	"text":"���� DC_PEN ����ɫ������֮ǰ��Ҫ���ö���� hdc ���ԡ��������ʵ�����뵱ǰ����û�й�ϵ�������õ��� hdc �����ԡ�",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"[color]",
		//			"text":"ָ�����ʵ���ɫ��ȱʡΪ��ɫ��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> setDCColor(const Arguments& args){
			HandleScope store;
			while(true){
				Local<Object> self = args.This();
				HDC hdc = (HDC)(INT_PTR)self->Get(String::New("hdc"))->Int32Value();
				if(hdc==0) break;

				uint color = GET_ARGS_INT(0,0);
				::SetDCPenColor(hdc,color);
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"destroy()",
		//	"text":"�ͷŶ���󶨵�ϵͳ������Դ��",
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
				Local<Object> self = args.This();
				cs::Pen* handle;
				if(!GetCHandle(handle,self)) break;
				if(!handle->Delete()) break;
				return True();
			}
			return Undefined();
		}
	public:
		static void init(Handle<FunctionTemplate> &ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			SET_CLA_FUNC(getInfo);
			SET_CLA_FUNC(rect);
			SET_CLA_FUNC(create);
			SET_CLA_FUNC(destroy);
			SET_CLA_FUNC(moveTo);
			SET_CLA_FUNC(lineTo);
			SET_CLA_FUNC(drawRect);
			SET_CLA_FUNC(roundRect);
			SET_CLA_FUNC(polyTo);
			SET_CLA_FUNC(arc);
			SET_CLA_FUNC(setDCColor);
			SET_CLA_FUNC(fromStockPen);
			
			SET_CLA_CONST(DASH,PS);
			SET_CLA_CONST(DOT,PS);
			SET_CLA_CONST(DASHDOT,PS);
			SET_CLA_CONST(DASHDOTDOT,PS);
			SET_CLA_CONST(NULL,PS);
			SET_CLA_CONST(SOLID,PS);
		}
	};
	//*]}//*
	//*,{
	//	"type":"class",
	//	"name":"CRgn",
	//	"text":"�����࣬���̳��� CHandleObject�����ڱ�ʾһ�����򣬹���Ļ򲻹���ġ�����Region���� Windows ��ϵͳ�������ںܶ��ͼ������",
	//	"member":[//*
	class JSRgn : public JsHandleObject<cs::Rgn,HRGN,JSRgn,TEMPLATE_ID_RGN>{
	protected:
		//*{
		//	"type":"function",
		//	"name":"rect(rect)",
		//	"text":"����һ����������",
		//	"param":[
		//		{
		//			"type":"Rect",
		//			"name":"rect",
		//			"text":"һ�����ζ���"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> rect(const Arguments& args){
			HandleScope store;
			if(args.Length()<=0) return Undefined();
			int left,top,right,bottom;
			if(args.Length()>=4){
				left = args[0]->Int32Value();
				top = args[1]->Int32Value();
				right = args[2]->Int32Value();
				bottom = args[3]->Int32Value();
			}else if(args.Length()>=1){
				if(!RectJsToC(args[0],(cs::Rect*)&left)) return Undefined();
			}
			cs::Rgn* handle;
			if(!GetCHandle(handle,args.This())) return Undefined();
			bool r = handle->CreateRect(left,top,right,bottom);
			return Bool(r);
		}
		//*,{
		//	"type":"function",
		//	"name":"poly(pointArray)",
		//	"text":"�����ɵ����ӵıպ�����Ȧ����������",
		//	"param":[
		//		{
		//			"type":"array",
		//			"name":"pointArray",
		//			"text":"һϵ�� Point �����γɵ����飬����Ԫ�ظ���������ڵ��� 3 ��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> poly(const Arguments& args){
			HandleScope store;
			if(args.Length()<=0) return Undefined();
			if(!args[0]->IsArray()) return Undefined();
			Local<Array> point = Local<Array>::Cast(args[0]);
			cs::List<cs::Twin> pl;
			uint len = point->Length();
			pl.Expand(len);
			for(uint i=0;i<len;i++){
				Local<Object> vPt = point->Get(i)->ToObject();
				pl[i].x = vPt->Get(String::New("x"))->Int32Value();
				pl[i].y = vPt->Get(String::New("y"))->Int32Value();
			}
			int mode;
			if(args.Length()>1){
				mode = args[1]->Uint32Value();
			}else mode = 1;
			cs::Rgn* handle;
			if(!GetCHandle(handle,args.This())) return Undefined();
			bool r = handle->CreatePolygon((LPPOINT)pl.GetHandle(),len,mode);
			return Bool(r);
		}
		//*,{
		//	"type":"function",
		//	"name":"ellipse(rect)",
		//	"text":"����һ����Բ����",
		//	"param":[
		//		{
		//			"type":"Rect",
		//			"name":"[rect]",
		//			"text":"���ɵ�������������ε�������Բ��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> ellipse(const Arguments& args){
			HandleScope store;
			if(args.Length()<=0) return Undefined();
			int left,top,right,bottom;
			if(args.Length()>=4){
				left = args[0]->Int32Value();
				top = args[1]->Int32Value();
				right = args[2]->Int32Value();
				bottom = args[3]->Int32Value();
			}else if(args.Length()>=1){
				if(!RectJsToC(args[0],(cs::Rect*)&left)) return Undefined();
			}
			cs::Rgn* handle;
			if(!GetCHandle(handle,args.This())) return Undefined();
			bool r = handle->CreateEllipse(left,top,right,bottom);
			return Bool(r);
		}
		//*,{
		//	"type":"function",
		//	"name":"combine(rgn,mode)",
		//	"text":"��ϵ�ǰ Region �Ͳ��� 1 ָ���� Region����Ϸ�ʽ�� mode ָ������������ڵ�ǰ����",
		//	"param":[
		//		{
		//			"type":"CRgn",
		//			"name":"rgn",
		//			"text":"Ҫ�͵�ǰ Region ��ϵ� CRgn ����"
		//		},
		//		{
		//			"type":"integer|string",
		//			"name":"mode",
		//			"text":"��Ϸ�ʽ�����������µ�ֵ:<br/>��and���� 1����������Ľ�����<br/>��or���� 2���������򲢼���<br/>��xor���� 3���������򽻼�֮��Ĳ��֣��Եȼ�����<br/>��diff���� 4���ӵ�ǰ�����ȥ�� rgn �ཻ�Ĳ��֡�<br/>��copy���� 5������ rgn ����ǰ����"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"�����ɹ�������ֵ��һ�� 1 �� 3 ��������ʧ�ܷ��� undefined��<br/>1����ʾ�����һ���յ�����<br/>2����ʾ�����һ��������<br/>3����ʾ����Ƕ��������"
		//	}
		//}//*
		static Handle<Value> combine(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<2) break;
				Local<Object> self = args.This();

				cs::Rgn* handle,*handle2;
				if(!GetCHandle(handle,self)) break;
				if(!IsTemplate(args[0],TEMPLATE_ID_RGN)) break;
				if(!GetCHandle(handle2,args[0]->ToObject())) break; 
				DWORD cbMode;
				if(args[1]->IsUint32()){
					cbMode = GET_ARGS_INT(1,cs::Rgn::Or);
					if(cbMode<1||cbMode>5) break;
				}else if(args[1]->IsString()){
					cs::String sMode;
					GetString(args[1],sMode);
					sMode.Lower();
					if(sMode==L"and") cbMode = cs::Rgn::And;
					else if(sMode==L"or") cbMode = cs::Rgn::Or;
					else if(sMode==L"xor") cbMode = cs::Rgn::Xor;
					else if(sMode==L"diff") cbMode = cs::Rgn::Diff;
					else if(sMode==L"copy") cbMode = cs::Rgn::Copy;
					else if(sMode==L"alternate") cbMode = ALTERNATE;
					else if(sMode==L"winding") cbMode = WINDING;
					else break;
				}else break;
				int r = handle->Combine(handle2->Handle(),cbMode);
				if(r==ERROR) break;
				return store.Close(Int32::New(r));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"roundRect(rect,rx,ry)",
		//	"text":"����һ��Բ�Ǿ�������Բ�Ǿ��ε� 4 ������һ����Բ�� 1/4��",
		//	"param":[
		//		{
		//			"type":"Rect",
		//			"name":"rect",
		//			"text":"һ�����Ρ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"rx",
		//			"text":"��Բ��ˮƽ�뾶��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"ry",
		//			"text":"��Բ�Ĵ�ֱ�뾶��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> roundRect(const Arguments& args){
			HandleScope store;
			int left,top,right,bottom,width,height;
			if(args.Length()>=6){
				left = args[0]->Int32Value();
				top = args[1]->Int32Value();
				right = args[2]->Int32Value();
				bottom = args[3]->Int32Value();
				width = args[4]->Int32Value();
				height = args[5]->Int32Value();
			}else if(args.Length()>=3){
				//���԰�left�ĵ�ַ����һ��Rect�ĵ�ַ��
				if(!RectJsToC(args[0],(cs::Rect*)&left)) return Undefined();
				width = args[1]->Int32Value();
				height = args[2]->Int32Value();
			}else
				return Undefined();
			Local<Object> self = args.This();
			cs::Rgn* handle;
			if(!GetCHandle(handle,self)) return Undefined();
			bool r = handle->CreateRoundRect(left,top,right,bottom,width,height);
			if(!r) return Undefined();
			return True();
		}
		//*,{
		//	"type":"function",
		//	"name":"offset(dx,dy)",
		//	"text":"ƽ��һ������",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"dx",
		//			"text":"ˮƽ�ƶ����ء�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"dy",
		//			"text":"��ֱ�ƶ����ء�"
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
				if(args.Length()<2) break;
				Local<Object> self = args.This();
				cs::Rgn* handle;
				if(!GetCHandle(handle,self)) break;
				int dx = GET_ARGS_INT(0,0);
				int dy = GET_ARGS_INT(1,0);
				if(!handle->Offset(dx,dy)) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"equal(rgn)",
		//	"text":"�жϵ�ǰ Region �� rgn �Ƿ���ͬ��",
		//	"param":[
		//		{
		//			"type":"CRgn",
		//			"name":"rgn",
		//			"text":"Ҫ�Ƚϵ� Region��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"���رȽϽ����true �� false����ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> equal(const Arguments& args){
			HandleScope store;
			if(args.Length()<=0) return Undefined();
			Local<Object> self = args.This();
			cs::Rgn* handle;
			if(!GetCHandle(handle,self)) 
				return Undefined();
			cs::Rgn* handle2;
			if(!GetCHandle(handle2,args[0]->ToObject())) 
				return Undefined();
			bool r = *handle==*handle2;
			return Bool(r);
		}
		//*,{
		//	"type":"function",
		//	"name":"rectBox()",
		//	"text":"��ȡ��ǰ Region ����Ӿ��Ρ�",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"Rect",
		//		"text":"�ɹ�����һ�� Rect ����ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> rectBox(const Arguments& args){
			HandleScope store;
			while(true){
				Local<Object> self = args.This();
				cs::Rgn* handle;
				if(!GetCHandle(handle,self)) break;
				cs::Rect pr;
				if(!handle->GetRectBox(pr)) break;
				Handle<Value> rect = NewRect(0,0,0,0);
				RectJsFromC(rect,&pr);
				return store.Close(rect);
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"ptInRgn(pt)",
		//	"text":"һ�����Ƿ��������ڡ�",
		//	"param":[
		//		{
		//			"type":"Point",
		//			"name":"pt",
		//			"text":"Ҫ���ĵ㡣"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ�����һ�� boolean ֵ��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> ptInRgn(const Arguments& args){
			HandleScope store;
			if(args.Length()<2) return Undefined();
			cs::Twin tw;
			tw.x = args[0]->Int32Value();
			tw.y = args[1]->Int32Value();
			Local<Object> self = args.This();
			cs::Rgn* handle;
			if(!GetCHandle(handle,self)) 
				return Undefined();
			bool r = handle->PtInRgn(tw);
			return Bool(r);
		}
		//*,{
		//	"type":"function",
		//	"name":"destroy()",
		//	"text":"���� Region ��Դ��",
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
				Local<Object> self = args.This();
				cs::Rgn* handle;
				if(!GetCHandle(handle,self)) break;
				if(!handle->Delete()) break;
				return True();
			}
			return Undefined();
		}
	public:
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			SET_CLA_FUNC(rect);
			SET_CLA_FUNC(poly);
			SET_CLA_FUNC(ellipse);
			SET_CLA_FUNC(roundRect);
			SET_CLA_FUNC(combine);
			SET_CLA_FUNC(offset);
			SET_CLA_FUNC(equal);
			SET_CLA_FUNC(ptInRgn);
			SET_CLA_FUNC(rectBox);
			SET_CLA_FUNC(destroy);
		}
	};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CBrush",
	//	"text":"��ˢ�������̳��� CHandleObject����ˢ���ڻ���һ������",
	//	"member":[//*
	class JSBrush : public JsHandleObject<cs::Brush,HBRUSH,JSBrush,TEMPLATE_ID_BRUSH>{
	protected:
		//*{
		//	"type":"function",
		//	"name":"create(color)",
		//	"text":"����һ��ָ����ɫ�� BS_SOLD ���Ļ�ˢ��",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"color",
		//			"text":"��ˢ����ɫ��"
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
				cs::Brush* handle;
				if(!GetCHandle(handle,args.This())) break;
				int color = GET_ARGS_INT(0,0);
				int style = GET_ARGS_INT(1,BS_SOLID);
				int hatch = GET_ARGS_INT(2,0);

				bool r = handle->Create(color,style,hatch);
				if(!r) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getColor()",
		//	"text":"��ȡ��ˢ����ɫ��",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"�ɹ�����һ��������ɫ��������ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> getColor(const Arguments& args){
			HandleScope store;
			cs::Brush* handle;
			if(!GetCHandle(handle,args.This())) return Undefined();
			LOGBRUSH lb;
			if(!handle->GetLogBrush(&lb)) return Undefined();
			return store.Close(Uint32::New(lb.lbColor));
		}
		//*,{
		//	"type":"function",
		//	"name":"fillRgn(rgn)",
		//	"text":"ʹ�õ�ǰ��ˢ���һ��������Ҫ���� hdc ���ԡ�",
		//	"param":[
		//		{
		//			"type":"CRgn",
		//			"name":"rgn",
		//			"text":"Ҫ���� Region��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> fillRgn(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				Handle<Object> self = args.This();
				HDC hdc = (HDC)(INT_PTR)self->Get(String::New("hdc"))->Int32Value();
				if(hdc==0) break;
				if(!IsTemplate(args[0],TEMPLATE_ID_RGN)) break;
				cs::Rgn* rgn;
				if(!GetCHandle(rgn,args[0]->ToObject())) break;
				cs::Brush* handle;
				if(!GetCHandle(handle,args.This())) break;
				bool r = ::FillRgn(hdc,*rgn,*handle)!=0;
				if(!r) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"fillRect(rect)",
		//	"text":"���һ�����Ρ���Ҫ���� hdc ���ԡ�",
		//	"param":[
		//		{
		//			"type":"Rect",
		//			"name":"rect",
		//			"text":"Ҫ���ľ��Ρ�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> fillRect(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				Handle<Object> self = args.This();
				HDC hdc = (HDC)(INT_PTR)self->Get(String::New("hdc"))->Int32Value();
				if(hdc==0) break;
				cs::Rect rect;
				if(!RectJsToC(args[0],&rect)) break;
				cs::Brush* brush;
				if(!GetCHandle(brush,args.This())) break;
				bool r = ::FillRect(hdc,rect,brush->Handle())!=0;
				if(!r) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"poly(pointArray)",
		//	"text":"���һ���պ������γɵ�������Ҫ���� hdc ���ԡ�",
		//	"param":[
		//		{
		//			"type":"array",
		//			"name":"pointArray",
		//			"text":"���ڵ��� 3 �� Point ���󹹳ɵ����飬��Щ����β�����γɵ����������������"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> poly(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				Handle<Object> self = args.This();
				HDC hdc = (HDC)(INT_PTR)self->Get(String::New("hdc"))->Int32Value();
				if(hdc==0) break;
				if(!args[0]->IsArray()) break;
				Local<Array> point = Local<Array>::Cast(args[0]);
				cs::List<cs::Twin> pl;
				uint len = point->Length();
				pl.Expand(len);
				for(uint i=0;i<len;i++){
					Local<Object> vPt = point->Get(i)->ToObject();
					pl[i].x = vPt->Get(String::New("x"))->Int32Value();
					pl[i].y = vPt->Get(String::New("y"))->Int32Value();
				}
				cs::Brush* brush;
				if(!GetCHandle(brush,args.This())) break;
				brush->Select(hdc);
				HPEN hb = (HPEN)::SelectObject(hdc,(HPEN)::GetStockObject(NULL_PEN));
				bool r = ::Polygon(hdc,(LPPOINT)pl.GetHandle(),len)!=0;
				SelectObject(hdc,hb);
				brush->Select(hdc);
				if(!r) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"ellipse(rect)",
		//	"text":"���һ�����ε�������Բ����Ҫ���� hdc ���ԡ�",
		//	"param":[
		//		{
		//			"type":"Rect",
		//			"name":"rect",
		//			"text":"һ�����ζ���"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> ellipse(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				Handle<Object> self = args.This();
				HDC hdc = (HDC)(INT_PTR)self->Get(String::New("hdc"))->Int32Value();
				if(hdc==0) break;
				int left,top,right,bottom;
				if(args.Length()>=4){
					left = args[0]->Int32Value();
					top = args[1]->Int32Value();
					right = args[2]->Int32Value();
					bottom = args[3]->Int32Value();
				}else if(args.Length()>=1){
					if(!RectJsToC(args[0],(cs::Rect*)&left)) break;
				}else break;
				cs::Brush* brush;
				if(!GetCHandle(brush,args.This())) break;
				brush->Select(hdc);
				HPEN hb = (HPEN)::SelectObject(hdc,(HPEN)::GetStockObject(NULL_PEN));
				bool r = ::Ellipse(hdc,left,top,right,bottom)!=0;
				SelectObject(hdc,hb);
				brush->Select(hdc);
				if(!r) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"roundRect(rect,rx,ry)",
		//	"text":"���һ��Բ�Ǿ��ε�������Ҫ���� hdc ���ԡ�",
		//	"param":[
		//		{
		//			"type":"Rect",
		//			"name":"rect",
		//			"text":"һ�����ζ���"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"rx",
		//			"text":"Բ�ǵ�ˮƽ�뾶��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"ry",
		//			"text":"Բ�ǵĴ�ֱ�뾶��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> roundRect(const Arguments& args){
			HandleScope store;
			while(true){
				int left,top,right,bottom,width,height;
				if(args.Length()>=6){
					left = args[0]->Int32Value();
					top = args[1]->Int32Value();
					right = args[2]->Int32Value();
					bottom = args[3]->Int32Value();
					width = args[4]->Int32Value();
					height = args[5]->Int32Value();
				}else if(args.Length()>=3){
					//���԰�left�ĵ�ַ����һ��Rect�ĵ�ַ��
					if(!RectJsToC(args[0],(cs::Rect*)&left)) break;
					width = args[1]->Int32Value();
					height = args[2]->Int32Value();
				}else break;

				Handle<Object> self = args.This();
				HDC hdc = (HDC)(INT_PTR)self->Get(String::New("hdc"))->Int32Value();
				if(hdc==0) break;
				cs::Brush* brush;
				if(!GetCHandle(brush,args.This())) break;
				brush->Select(hdc);
				HPEN hp = (HPEN)::SelectObject(hdc,(HPEN)::GetStockObject(NULL_PEN));
				bool r = RoundRect(hdc,left,top,right,bottom,width,height)!=0;
				::SelectObject(hdc,hp);
				brush->Select(hdc);
				if(!r) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"destroy()",
		//	"text":"�ͷ�ϵͳ�Ļ�ˢ��Դ��",
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
				Local<Object> self = args.This();
				cs::Brush* handle;
				if(!GetCHandle(handle,self)) break;
				if(!handle->Delete()) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"fromDCBrush()",
		//	"text":"���õ�ǰ����Ϊ DC_BRUSH��DC_BRUSH ��ϵͳ���õ� Brush�����ܱ����٣����� destroy û��Ч����ʹ�� DC_BRUSH Ч�ʻ��һЩ��������Ϊ���������µ� Brush����ʡ��ϵͳ��Դ������ͨ�� setDCColor �������� DC_BRUSH ����ɫ��",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"fromStockBrush(stock)",
		//	"text":"��ȡϵͳ���õĻ�ˢ���󣬲���΢����ĵ���ȡϵͳ���õĻ�ˢ����ֵ��ϵͳ���û�ˢ���ᱻ���١�",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"[stock]",
		//			"text":"���û�ˢ�Ĵ��룬ȱʡ��ȡ DC_BRUSH��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> fromStockBrush(const Arguments& args){
			HandleScope store;
			while(true){
				Local<Object> self = args.This();
				cs::Brush* handle;
				if(!GetCHandle(handle,self)) break;
				int sp = GET_ARGS_INT(0,DC_BRUSH);
				bool r = handle->Attach((HBRUSH)::GetStockObject(sp));
				if(!r) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setDCColor([color])",
		//	"text":"���� DC_BRUSH ����ɫ������֮ǰ��Ҫ���ö���� hdc ���ԡ���������뵱ǰ�����޹أ������� hdc �����ԡ�",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"[color]",
		//			"text":"ָ����ˢ����ɫ��ȱʡΪ��ɫ��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> setDCColor(const Arguments& args){
			HandleScope store;
			while(true){
				Local<Object> self = args.This();
				HDC hdc = (HDC)(INT_PTR)self->Get(String::New("hdc"))->Int32Value();
				if(hdc==0) break;

				uint color = GET_ARGS_INT(0,0);
				SetDCBrushColor(hdc,color);
				return True();
			}
			return Undefined();
		}
	public:
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			SET_CLA_FUNC(create);
			SET_CLA_FUNC(destroy);
			SET_CLA_FUNC(getColor);
			SET_CLA_FUNC(fillRgn);
			SET_CLA_FUNC(fillRect);
			SET_CLA_FUNC(poly);
			SET_CLA_FUNC(ellipse);
			SET_CLA_FUNC(roundRect);
			SET_CLA_FUNC(fromStockBrush);
			SET_CLA_FUNC(setDCColor);
		}
	};

	//*]}//*
	void LogFontToC(Handle<Object>& lf,LPLOGFONT clf){
		HandleScope store;
		Handle<Value> val;
		SET_VAR_TO_PROP(val,clf->lfCharSet,lf,charSet,Uint32);
		SET_VAR_TO_PROP(val,clf->lfClipPrecision,lf,clipPrecision,Uint32);
		SET_VAR_TO_PROP(val,clf->lfEscapement,lf,escapment,Uint32);
		SET_VAR_TO_PROP(val,clf->lfOrientation,lf,orientation,Uint32);
		SET_VAR_TO_PROP(val,clf->lfQuality,lf,quality,Uint32);
		SET_VAR_TO_PROP(val,clf->lfStrikeOut,lf,strikeOut,Boolean);
		SET_VAR_TO_PROP(val,clf->lfUnderline,lf,underline,Boolean);
		SET_VAR_TO_PROP(val,clf->lfWeight,lf,weight,Uint32);;
		SET_VAR_TO_PROP(val,clf->lfHeight,lf,size,Uint32);
		SET_VAR_TO_PROP(val,clf->lfItalic,lf,italic,Boolean);
		SET_VAR_TO_PROP(val,clf->lfOutPrecision,lf,outPrecision,Uint32);;
		SET_VAR_TO_PROP(val,clf->lfPitchAndFamily,lf,pitchAndFamily,Uint32);;
		SET_VAR_TO_PROP(val,clf->lfWidth,lf,width,Uint32);;
		val = GET_PROP(lf,name);
		if(!val->IsUndefined()){
			cs::String name;
			GetString(val,name);
			cs::WcsCopy(clf->lfFaceName,name,31);
		}
	}
	void LogFontFromC(Handle<Object>& lf,LPLOGFONT clf){
		lf->Set(String::New("charSet"),Int32::New(clf->lfCharSet));
		lf->Set(String::New("clipPrecision"),Int32::New(clf->lfClipPrecision));
		lf->Set(String::New("escapment"),Int32::New(clf->lfEscapement));
		lf->Set(String::New("orientation"),Int32::New(clf->lfOrientation));
		lf->Set(String::New("quality"),Int32::New(clf->lfQuality));
		lf->Set(String::New("strikeOut"),Boolean::New(clf->lfStrikeOut!=0));
		lf->Set(String::New("underline"),Boolean::New(clf->lfUnderline!=0));
		lf->Set(String::New("weight"),Int32::New(clf->lfWeight));
		lf->Set(String::New("name"),String::New((uint16_t*)clf->lfFaceName));
		lf->Set(String::New("size"),Int32::New(clf->lfHeight));
		lf->Set(String::New("italic"),Boolean::New(0!=clf->lfItalic));
		lf->Set(String::New("outPrecision"),Int32::New(clf->lfOutPrecision));
		lf->Set(String::New("pitchAndFamily"),Int32::New(clf->lfPitchAndFamily));
		lf->Set(String::New("width"),Int32::New(clf->lfWidth));
	}
	//*,{
	//	"type":"class",
	//	"name":"CFont",
	//	"text":"���壬���̳��� CHandleObject��������ϵͳ����������ֵ� gdi ����",
	//	"member":[//*
	class JSFont : public JsHandleObject<cs::Font,HFONT,JSFont,TEMPLATE_ID_FONT>{
	public:
		//*{
		//	"type":"function",
		//	"name":"fromDefault()",
		//	"text":"��Ĭ�����壬wnd �ؼ�Ĭ��ʹ��������壨���Ǹ�һ���ؼ������˶��������壩�������������κβ�����Ӱ������ʹ���������Ķ��󡣵���ȱʡ�����ǰ󶨵���������ϵģ��޷� dispose������ destroy Ҳ�᷵�� undefined�����Ҫ�ı�ȱʡ���壬�������κ�ʹ��ȱʡ����Ŀؼ�����֮ǰ�������壬������ʾ���ܻ᲻��������Ϊ�ؼ��ڴ���ʱ���������壬����֮��ı�����Ļ����ؼ���֪�������Ѿ��ı䡣",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> fromDefault(const Arguments& args){
			HandleScope store;
			while(true){
				Handle<Object> self = args.This();
				Handle<Value> prop = self->Get(String::New("bind"));
				if(!prop->IsFunction()) break;
				Handle<Function> bind = Handle<Function>::Cast(prop);
				Handle<Value> argv[2];
				argv[0] = Uint32::New((UINT_PTR)cs::GetDefaultFont());
				argv[1] = True();
				return CallFunc(self,bind,2,argv);
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"create()",
		//	"text":"����������Դ��������������Բ��������ı䵱ǰ���壬��Ҫ���� create �������ɲ��������Ч��",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> create(const Arguments& args){
			HandleScope store;
			while(true){
				cs::Font* font;
				if(!GetCHandle(font,args.This())) break;
				if(!font->Create()) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"reset()",
		//	"text":"���õ�ǰ����Ĳ���Ϊϵͳȱʡֵ����Ҫ���� create ��Ż���Ч��",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> reset(const Arguments& args){
			HandleScope store;
			cs::Font* font;
			if(GetCHandle(font,args.This())){
				font->Reset();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"destroy()",
		//	"text":"����������Դ��",
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
				Local<Object> self = args.This();
				cs::Font* handle;
				if(!GetCHandle(handle,self)) break;
				if(handle==cs::GetDefaultFont()) break;
				if(!handle->Delete()) break;
				return True();
			}
			return Undefined();
		}
	public:
		//*,{
		//	"type":"property",
		//	"name":"size",
		//	"objtype":"integer",
		//	"text":"��ȡ/��������Ĵ�С��������������Ǹ����������������ࡣ"
		//}
		//,{
		//	"type":"property",
		//	"name":"name",
		//	"objtype":"string",
		//	"text":"��ȡ/������������ƣ��մ�����Ч���ƻᱻ����ϵͳĬ�����塣"
		//}
		//,{
		//	"type":"property",
		//	"name":"weight",
		//	"objtype":"integer",
		//	"text":"��ȡ/��������Ĵ���̶ȣ�400 ��Ĭ��ֵ����ͨ���壩��900 �Ǵ����֡�"
		//}
		//,{
		//	"type":"property",
		//	"name":"underline",
		//	"objtype":"boolean",
		//	"text":"��ȡ/����������»��ߡ�"
		//}
		//,{
		//	"type":"property",
		//	"name":"strikeOut",
		//	"objtype":"boolean",
		//	"text":"��ȡ/���������ɾ���ߡ�"
		//}
		//,{
		//	"type":"property",
		//	"name":"logfont",
		//	"objtype":"integer",
		//	"text":"��ȡ/��������� logfont��logfont �������ڲ���һ������ָ�롣������������Ե�ʱ��û�����ͼ�飬����Ӧ��ȷ������һ����Ч�� logfont ָ�롣"
		//}
		//,{
		//	"type":"property",
		//	"name":"italic",
		//	"objtype":"boolean",
		//	"text":"��ȡ/���������Ƿ�б�塣"
		//}//*
		static Handle<Value> get(cs::String& name,cs::Font* cobj,Local<Object>& self){
			LPLOGFONT plf = cobj->LogFont();
			if(name==L"name"){
				return String::New((uint16_t*)plf->lfFaceName);
			}else if(name==L"italic"){
				return Boolean::New(plf->lfItalic!=0);
			}else if(name==L"underline"){
				return Boolean::New(plf->lfUnderline!=0);
			}else if(name==L"strikeOut"){
				return Boolean::New(plf->lfStrikeOut!=0);
			}else if(name==L"weight"){
				return Uint32::New(plf->lfWeight);
			}else if(name==L"size"){
				return Uint32::New(plf->lfHeight);
			}else if(name==L"charSet"){
				return Uint32::New(plf->lfCharSet);
			}else if(name==L"clipPrecision"){
				return Uint32::New(plf->lfClipPrecision);
			}else if(name==L"orientation"){
				return Uint32::New(plf->lfOrientation);
			}else if(name==L"escapment"){
				return Uint32::New(plf->lfEscapement);
			}else if(name==L"outPrecision"){
				return Uint32::New(plf->lfOutPrecision);
			}else if(name==L"quality"){
				return Uint32::New(plf->lfQuality);
			}else if(name==L"pitchAndFamily"){
				return Uint32::New(plf->lfPitchAndFamily);
			}else if(name==L"width"){
				return Uint32::New(plf->lfWidth);
			}else if(name==L"logfont"){
				return Uint32::New((UINT_PTR)plf);
			}
			return Undefined();
		}
		static void set(cs::String& name,cs::Font* cobj,Local<Value>& value,Local<Object>& self){
			LPLOGFONT plf = cobj->LogFont();
			if(name==L"name"){
				if(!value->IsString()) return;
				cs::String name;
				GetString(value,name);
				cs::WcsCopy(plf->lfFaceName,name,31);
			}else if(name==L"italic"){
				if(!value->IsBoolean()) return;
				plf->lfItalic = value->BooleanValue();
			}else if(name==L"underline"){
				if(!value->IsBoolean()) return;
				plf->lfUnderline = value->BooleanValue();
			}else if(name==L"strikeOut"){
				if(!value->IsBoolean()) return;
				plf->lfStrikeOut = value->BooleanValue();
			}else if(name==L"weight"){
				if(!value->IsUint32()) return;
				plf->lfWeight = value->Uint32Value();
			}else if(name==L"size"){
				if(!value->IsUint32()) return;
				plf->lfHeight = value->Int32Value();
			}else if(name==L"charSet"){
				if(!value->IsUint32()) return;
				plf->lfCharSet = value->Uint32Value();
			}else if(name==L"clipPrecision"){
				if(!value->IsUint32()) return;
				plf->lfClipPrecision = value->Uint32Value();
			}else if(name==L"orientation"){
				if(!value->IsUint32()) return;
				plf->lfOrientation = value->Uint32Value();
			}else if(name==L"escapment"){
				if(!value->IsUint32()) return;
				plf->lfEscapement = value->Uint32Value();
			}else if(name==L"outPrecision"){
				if(!value->IsUint32()) return;
				plf->lfOutPrecision = value->Uint32Value();
			}else if(name==L"quality"){
				if(!value->IsUint32()) return;
				plf->lfQuality = value->Uint32Value();
			}else if(name==L"pitchAndFamily"){
				if(!value->IsUint32()) return;
				plf->lfPitchAndFamily = value->Uint32Value();
			}else if(name==L"width"){
				if(!value->IsUint32()) return;
				plf->lfWidth = value->Uint32Value();
			}else if(name==L"logfont"){
				cobj->SetLogFont((LPLOGFONT)value->Uint32Value());
			}
		}
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			SET_CLA_FUNC(create);
			SET_CLA_FUNC(destroy);
			SET_CLA_FUNC(reset);
			SET_CLA_FUNC(fromDefault);

			SET_CLA_ACCESSOR(name);
			SET_CLA_ACCESSOR(italic);
			SET_CLA_ACCESSOR(underline);
			SET_CLA_ACCESSOR(strikeOut);
			SET_CLA_ACCESSOR(weight);
			SET_CLA_ACCESSOR(size);
			SET_CLA_ACCESSOR(charSet);
			SET_CLA_ACCESSOR(clipPrecision);
			SET_CLA_ACCESSOR(orientation);
			SET_CLA_ACCESSOR(escapment);
			SET_CLA_ACCESSOR(outPrecision);
			SET_CLA_ACCESSOR(quality);
			SET_CLA_ACCESSOR(pitchAndFamily);
			SET_CLA_ACCESSOR(width);
			SET_CLA_ACCESSOR(logfont);
		}
	};
	class JSIcon;
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CBitmap",
	//	"text":"CBitmap ����ϵͳλͼ��Դ��λͼ�� Windows ͼ��ϵͳʹ�õ���Դ��",
	//	"remark":[
	//		"��ߴ��λͼ�ǳ��ķ�ϵͳ��Դ����Ҫ���ڴ��б��������Ĵ�ߴ�λͼ���󡣻��ƴ�ߴ�λͼҲ�ǳ���ʱ�䡣"
	//	],
	//	"member":[//*
	class JSBitmap : public JsHandleObject<cs::Bitmap,HBITMAP,JSBitmap,TEMPLATE_ID_BITMAP>{
	protected:
		//*{
		//	"type":"function",
		//	"name":"save(filename,[quality])",
		//	"text":"���浱ǰλͼ����һ���ļ����ļ���ʽȡ�����ļ����ĺ�׺��֧�ֵĸ�ʽȡ����ϵͳ��װ�Ľ�������ͨ��֧�� jpg��png��gif����֡����bmp �ȵȡ�",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"filename",
		//			"text":"�ļ�������׺��������ĸ�ʽ��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[quality]",
		//			"text":"ͼ������������Խ�ߣ��ļ�Խ��ͼ��ʧ��ԽС�����ֵ�ķ�Χ�� 0 �� 100 ,ȱʡֵ�� 100��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> save(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				cs::Bitmap* pbmp;
				if(!GetCHandle(pbmp,args.This())) break;
				cs::String file;
				GetString(args[0],file);
				int quality;
				if(args.Length()>1) quality = args[1]->Int32Value();
				else quality = 100;
				cs::Image img;
				if(!img.Create(pbmp->GetClip())) break;
				if(!img.Save(file,quality)) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"load(filename)",
		//	"text":"��ȡһ��ͼ���ļ���֧�� jpg��png��gif��ֻ�ܶ�ȡ��һ֡����bmp �ȸ�ʽ��",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"filename",
		//			"text":"ͼ���ļ���·����ͼ���ļ���ȡ��ʱ������ʽʶ�����ļ����ݾ����ģ����ļ�����׺�޹ء�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> load(const Arguments& args){
			HandleScope store;
			if(args.Length()<1) return Undefined();
			cs::Bitmap* pbmp;
			if(!GetCHandle(pbmp,args.This())) return Undefined();
			cs::String file;
			GetString(args[0],file);
			cs::Image img;
			if(!img.Create(file)) return Undefined();
			bool r = pbmp->Attach(img.GetBitmap());
			return Bool(r);
		}
		//*,{
		//	"type":"function",
		//	"name":"fromMemory(mem,[len])",
		//	"text":"���ڴ����һ��λͼ������ڴ������һ����Ч��ͼ���ļ����ݡ�",
		//	"param":[
		//		{
		//			"type":"CMemory",
		//			"name":"mem",
		//			"text":"����ͼ���ļ����ݵ��ڴ����"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[len]",
		//			"text":"���ݳ��ȣ�������ֵȱʡ������һ����������ݣ�����ȡ mem ��������С��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> fromMemory(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				if(!IsTemplate(args[0],TEMPLATE_ID_MEMORY)) break;
				Handle<Object> self = args.This();
				cs::Bitmap* pbmp;
				if(!GetCHandle(pbmp,self)) break;
				cs::Memory<char>* mem;
				if(!GetCHandle(mem,args[0]->ToObject())) break;
				int len = GET_ARGS_INT(1,0);
				if(len<=0) len = mem->Length();

				cs::Image img;
				if(!img.Create(mem->Handle(),len)) break;
				if(!pbmp->Attach(img.GetBitmap())) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"toMemory(mem,type,[quality])",
		//	"text":"��ͼ�񱣴浽�ڴ����ݣ��������ļ�����Щ���ݺͱ��浽�ļ�����������ͬ�ġ�",
		//	"param":[
		//		{
		//			"type":"CMemory",
		//			"name":"mem",
		//			"text":"���ڱ���ͼ�����ݵ��ڴ����mem ��������󣬻ᱻ�Զ����䣬��ʱ�ᱻǿ��ת��Ϊ system �ڴ档��� mem �㹻���򲻻�������ȣ����ԣ����ݳ���Ӧ�ÿ�����ֵ�������� mem �ĳ��ȡ�"
		//		},
		//		{
		//			"type":"string",
		//			"name":"type",
		//			"text":"����ͼ������ͣ��硰jpg������gif�� �ȡ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[quality]",
		//			"text":"ͼ������������Խ�ߣ��ļ�Խ��ͼ��ʧ��ԽС�����ֵ�ķ�Χ�� 0 �� 100 ,ȱʡֵ�� 100��"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"�ɹ�����ͼ�����ݵĳ��ȣ�ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> toMemory(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<2) break;
				if(!IsTemplate(args[0],TEMPLATE_ID_MEMORY)) break;
				Handle<Object> self = args.This();
				cs::Bitmap* cobj;
				if(!GetCHandle(cobj,self)) break;
				cs::Memory<char>* mem;
				if(!GetCHandle(mem,args[0]->ToObject())) break;

				cs::String type;
				GetString(args[1],type);
				int pos = type.Find(L'.');
				if(pos==-1) type.Insert('.',0);
				else if(pos>0){
					type.Tail(pos);
				}

				int quality = GET_ARGS_INT(2,100);

				cs::Image img;
				if(!img.Create(cobj->GetClip())) break;
				int len = img.Save(*mem,type,quality);
				if(len==0) break;
				return store.Close(Uint32::New(len));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getSize()",
		//	"text":"��ȡ��ǰλͼ�ĳߴ硣",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"Size",
		//		"text":"�ɹ�����һ����ʾͼ��ߴ�� Size ����ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> getSize(const Arguments& args){
			HandleScope store;
			cs::Bitmap* pbmp;
			if(!GetCHandle(pbmp,args.This())) return Undefined();
			cs::Twin sz = pbmp->Size();
			return store.Close(NewSize(sz.x,sz.y));
		}
		//*,{
		//	"type":"function",
		//	"name":"resize(cx,cy)",
		//	"text":"����ͼ��ĳߴ硣",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"cx",
		//			"text":"����ͼ��Ŀ�ȣ�0 �����ᵼ�º���ʧ�ܡ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"cy",
		//			"text":"����ͼ��ĸ߶ȣ�0 �����ᵼ�º���ʧ�ܡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> resize(const Arguments& args){
			HandleScope store;
			if(args.Length()<2) return Undefined();
			int cx,cy;
			cx = args[0]->Int32Value();
			cy = args[1]->Int32Value();
			if(cx<=0||cy<=0) return Undefined();
			cs::Bitmap* pbmp;
			if(!GetCHandle(pbmp,args.This())) return Undefined();
			bool r = pbmp->Resize(cx,cy);
			return Bool(r);
		}
		//*,{
		//	"type":"function",
		//	"name":"create(cx,cy)",
		//	"text":"����һ��ָ����ȵ�λͼ��",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"cx",
		//			"text":"����ͼ��Ŀ�ȣ�0 �����ᵼ�º���ʧ�ܡ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"cy",
		//			"text":"����ͼ��ĸ߶ȣ�0 �����ᵼ�º���ʧ�ܡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��<b>λͼ�Ƚ�ռ��ϵͳ��Դ���ر��Ǵ�λͼ�������ڴ治�����ܵ��º���ʧ��</b>��"
		//	}
		//}//*
		static Handle<Value> create(const Arguments& args){
			HandleScope store;
			if(args.Length()<2) return Undefined();
			int cx,cy;
			cx = args[0]->Int32Value();
			cy = args[1]->Int32Value();
			if(cx<=0||cy<=0) return Undefined();
			cs::Bitmap* pbmp;
			if(!GetCHandle(pbmp,args.This())) return Undefined();
			bool r = pbmp->CreateCompatibleBitmap(cx,cy);
			return Bool(r);
		}
		//*,{
		//	"type":"function",
		//	"name":"copyFrom(bitmap,[cx],[cy],[x],[y])",
		//	"text":"����һ��λͼ��һ���֡�",
		//	"param":[
		//		{
		//			"type":"CBitmap",
		//			"name":"bitmap",
		//			"text":"Ҫ���Ƶ�λͼ��Դ�����ֵ���������Լ���"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[cx]",
		//			"text":"Ҫ���Ƶ�λͼ��Դ�Ŀ�ȣ����ܳ���Դλͼ����ķ�Χ��ȱʡ��0 ������ֵ ��ʾ����ȡ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[cy]",
		//			"text":"Ҫ���Ƶ�λͼ��Դ�ĸ߶ȣ����ܳ���Դλͼ����ķ�Χ��ȱʡ��0 ������ֵ ��ʾ���߶ȡ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[x]",
		//			"text":"Ҫ���Ƶ�λͼ��Դ�����Ͻ�λ�� x ���꣬���ܳ���Դλͼ����ķ�Χ��ȱʡ������ֵ ��ʾ 0 ��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[y]",
		//			"text":"Ҫ���Ƶ�λͼ��Դ�����Ͻ�λ�� y ���꣬���ܳ���Դλͼ����ķ�Χ��ȱʡ������ֵ ��ʾ 0 ��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> copyFrom(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				if(!IsTemplate(args[0],TEMPLATE_ID_BITMAP)) break;
				cs::Bitmap* idb;
				if(!GetCHandle(idb,args[0]->ToObject())) break;
				cs::Bitmap* pbmp;
				if(!GetCHandle(pbmp,args.This())) break;
				int x,y,cx,cy;
				cx = GET_ARGS_INT(1,0);
				cy = GET_ARGS_INT(2,0);
				x = GET_ARGS_INT(3,0);
				y = GET_ARGS_INT(4,0);
				bool r = pbmp->Attach(idb->GetClip(cx,cy,x,y));
				if(!r) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"fromIcon(icon)",
		//	"text":"����ͼ��Ĳ�ɫͼ��ͼ���������ͼ��",
		//	"param":[
		//		{
		//			"type":"CIcon",
		//			"name":"icon",
		//			"text":"Ҫ���Ƶ�ͼ�����"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> fromIcon(const Arguments& args){
			HandleScope store;
			if(args.Length()<1) return Undefined();
			if(!IsTemplate(args[0],TEMPLATE_ID_ICON)) return Undefined();
			cs::Icon* idb;
			if(!GetCHandle(idb,args[0]->ToObject())) return Undefined();
			cs::Bitmap* pbmp;
			if(!GetCHandle(pbmp,args.This())) return Undefined();
			bool r = pbmp->FromIcon(*idb);
			return Bool(r);
		}
		//*,{
		//	"type":"function",
		//	"name":"getPixelArray()",
		//	"text":"��ȡλͼ���������顣",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"array",
		//		"text":"�ɹ�����һ�����飬�����Ԫ����һ��������ɫ��������ÿ��Ԫ�ش���һ�����أ���һ��Ԫ�ر�ʾ���½ǵ�������ɫ���������ң�Ȼ�����ϣ����һ��Ԫ�������Ͻǵ����أ�λͼ���������ǳ����Կ�ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> getPixelArray(const Arguments& args){
			HandleScope store;
			cs::Bitmap* pbmp;
			if(!GetCHandle(pbmp,args.This())) return Undefined();
			cs::Memory<char> p;
			p.SetUseSysMemory(1);
			pbmp->GetPixelData((cs::Memory<byte>*)&p,32);
			uint len = p.Length()/4;
			Local<Array> a = Array::New(len);
			for(uint i=0,j=0;j<len;i+=4,j++){
				uint v = p[i]+(p[i+1]<<8)+(p[i+2]<<16)+(p[i+3]<<24);
				a->Set((uint32_t)j,Uint32::New(v));
			}
			return a;
		}
		//*,{
		//	"type":"function",
		//	"name":"setPixelArray(pixelArray)",
		//	"text":"����λͼ�����ء�",
		//	"param":[
		//		{
		//			"type":"array",
		//			"name":"pixelArray",
		//			"text":"�����������ݵ�һ�����飬�������ĳ��ȱ�����λͼ�ĳ����Կ�����λͼ����������ͬ��ÿ��Ԫ�ش������ص���ɫ��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> setPixelArray(const Arguments& args){
			HandleScope store;
			if(args.Length()<1||!args[0]->IsArray()) return Undefined();
			Local<Array> a = Local<Array>::Cast(args[0]);
			cs::Memory<char> p;
			p.SetUseSysMemory(1);
			uint len = a->Length();
			p.SetLength(len*4);
			for(uint i=0,pi=0;i<len;i++,pi+=4){
				uint c = a->Get(i)->Uint32Value();
				p[pi+3] = (c>>24);
				p[pi+2] = (c>>16)&0xFF;
				p[pi+1] = (c>>8)&0xFF;
				p[pi] = c&0xFF;
			}
			cs::Bitmap* pbmp;
			if(!GetCHandle(pbmp,args.This())) return Undefined();
			bool r = 0!=pbmp->SetPixelData((cs::Memory<byte>*)&p,32);
			return Bool(r);
		}
		//*,{
		//	"type":"function",
		//	"name":"destroy()",
		//	"text":"����λͼ��Դ��",
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
				Local<Object> self = args.This();
				cs::Bitmap* handle;
				if(!GetCHandle(handle,self)) break;
				if(!handle->Delete()) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"draw(hdc,[x],[y],[cx],[cy],[sx],[sy],[scx],[scy])",
		//	"text":"��",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"hdc",
		//			"text":"ͼ����Ƶ� hdc ��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[x]",
		//			"text":"λͼ����ˮƽλ�ã�ȱʡ�� 0."
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[y]",
		//			"text":"λͼ���ƴ�ֱλ�ã�ȱʡ�� 0 ��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[cx]",
		//			"text":"λͼ���ƿ�ȣ�ȱʡ�� 0 ����ʾԭʼ��ȣ�û�����졣"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[cy]",
		//			"text":"λͼ���Ƹ߶ȣ�ȱʡ�� 0 ����ʾԭʼ�߶ȣ�û�����졣"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[sx]",
		//			"text":"λͼԴ��ʼλ�ã����Ͻǣ� x ���꣬ȱʡ�� 0��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[sy]",
		//			"text":"λͼԴ��ʼλ�ã����Ͻǣ� y ���꣬ȱʡ�� 0��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[scx]",
		//			"text":"λͼԴ��ȣ�ȱʡ�� 0����ʾ���Ŀ��ܿ�ȡ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[scy]",
		//			"text":"λͼԴ�߶ȣ�ȱʡ�� 0����ʾ���Ŀ��ܸ߶ȡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> draw(const Arguments& args){
			HandleScope store;
			HDC hdc;
			int x,y,cx,cy,sx,sy,scx,scy;
			if(args.Length()<1) return Undefined();
			cs::Bitmap* db;
			if(!GetCHandle(db,args.This())) return Undefined();
			hdc = (HDC)(INT_PTR)args[0]->Uint32Value();
			if(args.Length()>1) x = args[1]->Int32Value();
			else x = 0;
			if(args.Length()>2) y = args[2]->Int32Value();
			else y = 0;
			if(args.Length()>3) cx = args[3]->Int32Value();
			else cx = 0;
			if(args.Length()>4) cy = args[4]->Int32Value();
			else cy = 0;
			if(args.Length()>5) sx = args[5]->Int32Value();
			else sx = 0;
			if(args.Length()>6) sy = args[6]->Int32Value();
			else sy = 0;
			if(args.Length()>7) scx = args[7]->Int32Value();
			else scx = 0;
			if(args.Length()>8) scy = args[8]->Int32Value();
			else scy = 0;
			bool r = db->Draw(hdc,x,y,cx,cy,sx,sy,scx,scy);
			if(!r) return Undefined();
			return True();
		}
		//*,{
		//	"type":"function",
		//	"name":"alphaDraw(hdc,[alpha],[x],[y],[cx],[cy],[sx],[sy],[scx],[scy])",
		//	"text":"alphaDraw ����һ�� alpha �������������ָ������ͼ��ʱ��͸���ȡ�͸����ָ��������ɺ󱳾�ͼ��һ���̶ȵı���͸��������������ȫ�����ǡ�alphaDraw �� draw �����ܶࡣ",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"hdc",
		//			"text":"ͼ����Ƶ� hdc ��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[alpha]",
		//			"text":"͸���ȣ�ȡֵ��Χ�ǣ�0����255����0 ��ʾ��ȫ͸������ǰͼ����ȫ�����Ǳ�����255 ��ʾ��ȫ��͸����Ч���� draw ��ͬ�����ֵȱʡ�� 255��"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[isAlpha]",
		//			"text":"ͼ���Ƿ���͸��ͨ����ͼ���ÿ�����أ����˺�������������ֵ֮�⣬����һ�� alpha ֵ������ȡֵ��ΧҲ�� 0����255�������Ϊ����͸��ͨ����isAlpha = true����alphaDraw �ڻ�ͼʱ��ÿ�����ػᰴ������ alpha ֵ������͸���ȣ�0 ��ȫ͸����255 ��ȫ��͸����ͼ��� png ͼ����͸��ͨ������ͨͼ��û��͸��ͨ����Ҳ������ͨͼ��ÿ�����ص� alpha ֵ���� 0�����԰�����͸��ͼ�ķ�ʽ������ͨͼ���൱������ͼ����͸���ģ�ʲô������������֮��������ջ�����ͨͼ�ķ�ʽ����͸��ͼ��alpah ͨ����ֵ��ȫ�����ǣ�͸��ͼ��ʧȥ͸��Ч��������һ����ɫ��������isAlpha = false �൱�ڰ��������ص� alpha ֵ��Ϊ 255������Ҫ��ȷ�Ļ�����ͨͼ�񣬱������� isAlpha = false��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[x]",
		//			"text":"λͼ����ˮƽλ�ã�ȱʡ�� 0."
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[x]",
		//			"text":"λͼ����ˮƽλ�ã�ȱʡ�� 0."
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[y]",
		//			"text":"λͼ���ƴ�ֱλ�ã�ȱʡ�� 0 ��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[cx]",
		//			"text":"λͼ���ƿ�ȣ�ȱʡ�� 0 ����ʾԭʼ��ȣ�û�����졣"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[cy]",
		//			"text":"λͼ���Ƹ߶ȣ�ȱʡ�� 0 ����ʾԭʼ�߶ȣ�û�����졣"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[sx]",
		//			"text":"λͼԴ��ʼλ�ã����Ͻǣ� x ���꣬ȱʡ�� 0��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[sy]",
		//			"text":"λͼԴ��ʼλ�ã����Ͻǣ� y ���꣬ȱʡ�� 0��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[scx]",
		//			"text":"λͼԴ��ȣ�ȱʡ�� 0����ʾ���Ŀ��ܿ�ȡ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[scy]",
		//			"text":"λͼԴ�߶ȣ�ȱʡ�� 0����ʾ���Ŀ��ܸ߶ȡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> alphaDraw(const Arguments& args){
			HandleScope store;
			HDC hdc;
			BYTE alpha;
			bool aData;
			int x,y,cx,cy,sx,sy,scx,scy;
			if(args.Length()<1) return Undefined();
			hdc = (HDC)(INT_PTR)args[0]->Uint32Value();
			cs::Bitmap* db;
			if(!GetCHandle(db,args.This())) return Undefined();

			if(args.Length()>1) alpha = (BYTE)args[1]->Int32Value();
			else alpha = 255;
			if(args.Length()>2) aData = args[2]->BooleanValue();
			else aData = false;
			if(args.Length()>3) x = args[3]->Int32Value();
			else x = 0;
			if(args.Length()>4) y = args[4]->Int32Value();
			else y = 0;
			if(args.Length()>5) cx = args[5]->Int32Value();
			else cx = 0;
			if(args.Length()>6) cy = args[6]->Int32Value();
			else cy = 0;
			if(args.Length()>7) sx = args[7]->Int32Value();
			else sx = 0;
			if(args.Length()>8) sy = args[8]->Int32Value();
			else sy = 0;
			if(args.Length()>9) scx = args[9]->Int32Value();
			else scx = 0;
			if(args.Length()>10) scy = args[10]->Int32Value();
			else scy = 0;
			bool r = db->DrawAlpha(hdc,alpha,aData,x,y,cx,cy,sx,sy,scx,scy);
			return Bool(r);
		}
	public:
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			SET_CLA_FUNC(save);
			SET_CLA_FUNC(load);
			SET_CLA_FUNC(getSize);
			SET_CLA_FUNC(resize);
			SET_CLA_FUNC(create);
			SET_CLA_FUNC(destroy);
			SET_CLA_FUNC(copyFrom);
			SET_CLA_FUNC(fromIcon);
			SET_CLA_FUNC(fromMemory);
			SET_CLA_FUNC(toMemory);
			SET_CLA_FUNC(getPixelArray);
			SET_CLA_FUNC(setPixelArray);
			SET_CLA_FUNC(draw);
			SET_CLA_FUNC(alphaDraw);
		}
	};
	//*],"source":"d:\\SoftProject\\GitLib\\jsuser\\example\\bitmap.jsuser"}//*

	//*,{
	//	"type":"class",
	//	"name":"TextDraw",
	//	"text":"�˶����ṩ�����ı��Ĺ��ܡ�",
	//	"member":[//*
	class JSTextDraw{
		//*{
		//	"type":"function",
		//	"name":"draw(str,left,top,[width],[height])",
		//	"text":"��ָ���ľ����ڻ����ı�����Ҫ���ö���� hdc ���ԡ�",
		//	"param":[
		//		{
		//			"����":"string",
		//			"name":"str",
		//			"text":"Ҫ���Ƶ��ı���"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"left",
		//			"text":"���ε� left λ�á�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"top",
		//			"text":"���ε� top λ�á�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[width]",
		//			"text":"���εĿ�ȣ�ȱʡֵ�� 0xffff��һ���㹻���ֵ��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[height]",
		//			"text":"���εĸ߶ȣ�ȱʡֵ�� 0xffff��һ���㹻���ֵ��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ�����ʵ��������ַ�����ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> draw(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<3) break;

				Local<Object> self = args.This();
				Handle<Value> prop;
				DRAWTEXTPARAMS dtp;
				::ZeroMemory(&dtp,sizeof(dtp));
				dtp.cbSize = sizeof(dtp);

				prop = GET_PROP(self,hdc);
				HDC hdc = (HDC)(INT_PTR)prop->Uint32Value();
				if(hdc==0) break;

				cs::Font* font = 0;
				prop = GET_PROP(self,font);
				if(IsTemplate(prop,TEMPLATE_ID_FONT)){
					GetCHandle(font,prop->ToObject());
				}
				if(font==0){
					font = cs::GetDefaultFont();
				}

				prop = GET_PROP(self,color);
				int color = prop->Uint32Value();

				prop = GET_PROP(self,_dt);
				DWORD dt = prop->Uint32Value();

				prop = GET_PROP(self,tabstop);
				int tabstop = prop->Int32Value();
				dt |= DT_EXPANDTABS;
				if(tabstop<=0) dt -= DT_EXPANDTABS;
				else dtp.iTabLength = tabstop;

				cs::String str;
				GetString(args[0],str);

				cs::Rect cr;
				cr.left = GET_ARGS_INT(1,0);
				cr.top = GET_ARGS_INT(2,0);
				cr.right = GET_ARGS_INT(3,0xffff) + cr.left;
				cr.bottom = GET_ARGS_INT(4,0xffff) + cr.top;

				SetBkMode(hdc,TRANSPARENT);
				SetTextColor(hdc,color);
				font->Select(hdc);
				if(!DrawTextEx(hdc,str,str.Length(),cr,dt,&dtp)) break;
				font->Select(hdc);
				return store.Close(Uint32::New(dtp.uiLengthDrawn));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setSingleLine(yes)",
		//	"text":"���û�ͼ�ĵ������ԣ��������е�������ʱ�����ӻس����з��������ı����ڵ�һ�����������Ĭ��û�д����ԣ��س���\\r�������У�\\n�����س����У�\\r\\n�����ʱ���ỻ�С�",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true ���ô����ԣ�false ȡ�������ԡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"����û�з���ֵ��"
		//	}
		//}
		//,{
		//	"type":"function",
		//	"name":"setWordBreak(yes)",
		//	"text":"�����Զ��������ԣ�������εĿ���޷����һ�������ı������Զ����У����û�д����ԣ��ı�ֻ�������س����з�ʱ�Żỻ�С������Բ����жϵ��ʣ����һ�������ڵ����޷���ʾ���ᱻ���У����Ƿ���ֵ��Ȼ����û����ʾ�������ַ�������������Ĭ�����ԡ�",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true ���ô����ԣ�false ȡ�������ԡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"����û�з���ֵ��"
		//	}
		//}
		//,{
		//	"type":"function",
		//	"name":"setEditControl(yes)",
		//	"text":"������༭�ؼ����ԣ�������Բ�����������ı�����������д����ԣ��ײ��ı����ܱ����У�ֻ��ʾ�ϱߵĲ��֣������ʲ��ᱻ���У�����ǿ�ƻ��е���һ����ʾ����������� wordbread ���ԣ������������Ĭ�����ԡ�",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true ���ô����ԣ�false ȡ�������ԡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"����û�з���ֵ��"
		//	}
		//}
		//,{
		//	"type":"function",
		//	"name":"setNoClip(yes)",
		//	"text":"���ò��������ԣ�������Ի�˳����������ı������Ӹ߶Ȳ�������������˲��������ԣ�Ҳ�����ӿ�Ȳ����������Բ���Ĭ�����ԡ�",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true ���ô����ԣ�false ȡ�������ԡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"����û�з���ֵ��"
		//	}
		//}
		//,{
		//	"type":"function",
		//	"name":"setEndEllipsis(yes)",
		//	"text":"����ʡ�Ժ����ԣ���������޷���ʾȫ���ı��������Ի���β����ʾһ��ʡ�Ժš������Բ���Ĭ������",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true ���ô����ԣ�false ȡ�������ԡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"����û�з���ֵ��"
		//	}
		//}
		//,{
		//	"type":"function",
		//	"name":"setNoPrefix(yes)",
		//	"text":"���� noprefix ���ԣ����û�д����ԣ��ִ��еġ�&���ַ������һ���ַ�����ʾ�»��ߣ�Ҫ��ʾ��&���ַ������������ġ�&&���������ô����Թر�������ԡ������Բ���Ĭ�����ԡ�",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true ���ô����ԣ�false ȡ�������ԡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"����û�з���ֵ��"
		//	}
		//}
		//,{
		//	"type":"function",
		//	"name":"setAlignLeft(yes)",
		//	"text":"�����ı����뷽ʽΪ����룬��������Ĭ�����ԡ�",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true ���ô����ԣ�false ȡ�������ԡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"����û�з���ֵ��"
		//	}
		//}
		//,{
		//	"type":"function",
		//	"name":"setAlignCenter(yes)",
		//	"text":"�����ı����뷽ʽΪ�м���룬�����Բ���Ĭ�����ԡ�",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true ���ô����ԣ�false ȡ�������ԡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"����û�з���ֵ��"
		//	}
		//}
		//,{
		//	"type":"function",
		//	"name":"setAlignRight(yes)",
		//	"text":"�����ı����뷽ʽΪ�Ҷ��룬�����Բ���Ĭ�����ԡ�",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true ���ô����ԣ�false ȡ�������ԡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"����û�з���ֵ��"
		//	}
		//}
		//,{
		//	"type":"function",
		//	"name":"setAlignTop(yes)",
		//	"text":"�����ı���ֱ���뷽ʽΪ�϶��룬��������Ĭ�����ԡ�",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true ���ô����ԣ�false ȡ�������ԡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"����û�з���ֵ��"
		//	}
		//}
		//,{
		//	"type":"function",
		//	"name":"setAlignVCenter(yes)",
		//	"text":"���ô�ֱ�ı����뷽ʽΪ�м���룬�����Բ���Ĭ�����ԡ�������Խ��������˵������Ե�ʱ��Ż������á�",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true ���ô����ԣ�false ȡ�������ԡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"����û�з���ֵ��"
		//	}
		//}
		//,{
		//	"type":"function",
		//	"name":"setAlignBottom(yes)",
		//	"text":"���ô�ֱ�ı����뷽ʽΪ�¶��룬�����Բ���Ĭ�����ԡ�",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true ���ô����ԣ�false ȡ�������ԡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"����û�з���ֵ��"
		//	}
		//}
		//,{
		//	"type":"property",
		//	"name":"name",
		//	"objtype":"string",
		//	"text":"��ȡ/������������ơ�"
		//}
		//,{
		//	"type":"property",
		//	"name":"hdc",
		//	"objtype":"integer",
		//	"text":"��ȡ/���ö���� hdc ���ԣ�draw ������Ҫ������ԡ�"
		//}
		//,{
		//	"type":"property",
		//	"name":"tabstop",
		//	"objtype":"integer",
		//	"text":"��ȡ/���� tab �ַ�����ʾ��Ĭ��ֵ�� 8����ʾ tab �ַ��Ŀ���� 8 ���ַ���4�����֣���������ô�ֵС�ڵ��� 0��tab �ַ�������ʾ��"
		//}
		//,{
		//	"type":"property",
		//	"name":"font",
		//	"objtype":"CFont",
		//	"text":"��ȡ/���û����ı�ʹ�õ����壬�����Ե�Ĭ��ֵ�� null��ʹ��Ĭ�����塣"
		//}
		//,{
		//	"type":"property",
		//	"name":"color",
		//	"objtype":"integer",
		//	"text":"��ȡ/�����ı�����ɫ��Ĭ��ֵ�� 0����ɫ��"
		//}//*
	public:
		static void Load(Handle<Object>& glb){
			Handle<Function> td = GetObjProperty<Function>(glb,L"TextDraw");
			GetEnv()->SetFunc(td,FUNC_ID_TEXTDRAW);
			Handle<Object> proto = td->Get(String::New("prototype"))->ToObject();
			SET_OBJ_FUNC_RO(proto,draw,JSTextDraw::draw);
		}
	};
	//*],"source":"D:\\SoftProject\\GitLib\\jsuser\\example\\textdraw.jsuser"}//*

	//*,{
	//	"type":"class",
	//	"name":"CImageLoader",
	//	"text":"һ��ͼ���ļ���һ��ֻ��һ֡ͼ�񣬱��� gif �����ж�֡ͼ���������Լ��ص�һ֡�����ͼ��",
	//	"member":[//*
	class JSImageLoader : public JsCObject<cs::Image,JSImageLoader,TEMPLATE_ID_IMAGELOADER>{
		//*{
		//	"type":"function",
		//	"name":"load(file|mem,[len])",
		//	"text":"��ͼ���ļ������ڴ����һ��ͼ��.",
		//	"param":[
		//		{
		//		"type":"string|CMemory",
		//		"name":"file|mem",
		//		"text":"��������һ���ִ�����ʱ, ������һ��ͼ���ļ�����������һ�� CMemory ���͵Ķ���ʱ, ʹ�����洢����������ͼ��, ���û��ָ�� len ���������ݵĳ��Ⱦ��� CMemory ����Ĵ�С��"
		//		},
		//		{
		//		"type":"integer",
		//		"name":"[len]",
		//		"text":"����һ��������һ���ڴ����ʱ��ָ�����ݵĳ��ȣ�����˲���ȱʡ�����ݳ��ȱ���Ϊ���ڴ����Ĵ�С��"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"�ɹ�����ͼ���֡����ʧ�ܷ��� undefined"
		//	}
		//}//*
		static Handle<Value> load(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				cs::Image* img;
				if(!GetCHandle(img,args.This())) break;
				if(IsTemplate(args[0],TEMPLATE_ID_MEMORY)){
					cs::Memory<char>* mem;
					if(!GetCHandle(mem,args[0]->ToObject())) break;
					if(!img->Create(mem->Handle(),mem->Length())) break;
				}else{
					cs::String file;
					GetString(args[0],file);
					if(!img->Create(file)) break;
				}
				return store.Close(Uint32::New(img->FrameCount()));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getBitmap(bitmap,[index])",
		//	"text":"��ȡһ֡ͼ��, һ��ͼ���ļ����ܺ��ж�֡ͼ��. ͨ���ĵ�֡ͼ��ʵ�����ǵ�һ֡ͼ��.",
		//	"param":[
		//		{
		//		"type":"CBitmap",
		//		"name":"bitmap",
		//		"text":"���ڴ洢ͼ��� CBitmap ����"
		//		},
		//		{
		//		"type":"integer",
		//		"name":"[index]",
		//		"text":"Ҫ��ȡ��ͼ��֡��������ȱʡ�� 0�����������������ķ�Χ������ʧ�ܡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> getBitmap(const Arguments& args){
			HandleScope store;
			HBITMAP bmp = 0;
			while(true){
				if(args.Length()<1) break;
				cs::Image* img;
				if(!GetCHandle(img,args.This())) break;
				int index = GET_ARGS_INT(1,0);
				img->SetActiveFrame(index);
				bmp = img->GetBitmap();
				if(bmp==0) break;
				if(IsTemplate(args[0],TEMPLATE_ID_BITMAP)){
					cs::Bitmap* bitmap;
					if(!GetCHandle(bitmap,args[0]->ToObject())) break;
					if(!bitmap->Attach(bmp)) break;
				}else break;
				return True();
			}
			if(bmp) ::DeleteObject(bmp);
			return Undefined();
		}
	public:
		static inline void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			SET_CLA_FUNC(load);
			SET_CLA_FUNC(getBitmap);
		}
	};
	//*],"source":"D:\\SoftProject\\GitLib\\jsuser\\example\\gif.jsuser"}//*

	//*,{
	//	"type":"class",
	//	"name":"CIcon",
	//	"text":"ͼ���ࡣ",
	//	"member":[//*
	class JSIcon : public JsHandleObject<cs::Icon,HICON,JSIcon,TEMPLATE_ID_ICON>{
		//*{
		//	"type":"function",
		//	"name":"load(finename,[cx],[cy])",
		//	"text":"����һ��ͼ���ļ���һ��ͼ���ļ�ʵ������һ��ͼ���飬���ܺ��ж��ͼ�ꡣcx��cy ָ��Ҫ����ʲô�ߴ��ͼ�꣬���ָ���ߴ��ͼ�겻���ڣ��������Զ�Ѱ��һ���ߴ���ӽ���ͼ�ꡣ",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"finename",
		//			"text":"ͼ���ļ������ƣ��������Ҳ�������ڼ��� cur ���ָ���ļ���"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[cx]",
		//			"text":"Ҫ���ص�ͼ��Ŀ�ȣ����ȱʡʹ��ϵͳ��ȱʡֵ������ 32 ���ء�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[cy]",
		//			"text":"Ҫ���ص�ͼ��ĸ߶ȣ����ȱʡʹ��ϵͳ��ȱʡֵ������ 32 ���ء�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ�����true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> load(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				cs::Icon* b;
				if(!GetCHandle(b,args.This())) break;
				cs::String file;
				GetString(args[0],file);
				int cx,cy;
				cx = GET_ARGS_INT(1,0);
				cy = GET_ARGS_INT(2,0);
				if(!b->LoadFile(file,cx,cy)) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"draw(hdc,x,y,[cx],[cy])",
		//	"text":"��",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"hdc",
		//			"text":"Ҫ����ͼ��� hdc ��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"x",
		//			"text":"����ͼ��λ�� x ���ꡣ"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"y",
		//			"text":"����ͼ��λ�õ� y ���ꡣ"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[cx]",
		//			"text":"���ƿ�ȣ�ȱʡ��ԭʼ��ȡ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[cy]",
		//			"text":"���Ƹ߶ȣ�ȱʡ��ԭʼ�߶ȡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ�����true��ʧ�ܷ���undefined��"
		//	}
		//}//*
		static Handle<Value> draw(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<3) break;
				cs::Icon* b;
				if(!GetCHandle(b,args.This())) break;
				HDC hdc = (HDC)(INT_PTR)args[0]->Uint32Value();
				int x,y,cx,cy;
				x = GET_ARGS_INT(1,0);
				y = GET_ARGS_INT(2,0);
				cx = GET_ARGS_INT(3,0);
				cy = GET_ARGS_INT(4,0);
				if(!b->Draw(hdc,x,y,cx,cy)) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"size()",
		//	"text":"��ȡͼ��ĳߴ硣",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"Size",
		//		"text":"�ɹ�����һ�� Size ����ʧ�ܷ���undefined��"
		//	}
		//}//*
		static Handle<Value> size(const Arguments& args){
			HandleScope store;
			while(true){
				cs::Icon* b;
				if(!GetCHandle(b,args.This())) break;
				cs::Twin sz = b->Size();
				if(sz.x!=0&&sz.y!=0)
					return store.Close(NewPoint(sz.x,sz.y));
				else break;
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"toBlackWhite()",
		//	"text":"�ѵ�ǰͼ��ת��Ϊ�ڰ�ͼ��ע�⣬ͼ���λ���ǲ���ġ�",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> toBlackWhite(const Arguments& args){
			HandleScope store;
			while(true){
				cs::Icon* b;
				if(!GetCHandle(b,args.This())) break;
				if(!cs::IconToBlackWhite(b)) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"copyFrom(icon)",
		//	"text":"��һ��ͼ�������ͼ�ꡣ",
		//	"param":[
		//		{
		//			"type":"CIcon",
		//			"name":"icon",
		//			"text":"Դͼ�����"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> copyFrom(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				cs::Icon* b;
				if(!GetCHandle(b,args.This())) break;
				if(!IsTemplate(args[0],TEMPLATE_ID_ICON)) break;
				cs::Icon* b2;
				if(!GetCHandle(b2,args[0]->ToObject())) break;
				b->Attach(b2->GetCopy());
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"fromBitmap(bitmap)",
		//	"text":"��һ�� CBitmap ͼ������ͼ�ꡣ",
		//	"param":[
		//		{
		//			"type":"CBitmap",
		//			"name":"bitmap",
		//			"text":"Դ bitmap��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> fromBitmap(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				cs::Icon* b;
				if(!GetCHandle(b,args.This())) break;
				if(!IsTemplate(args[0],TEMPLATE_ID_BITMAP)) break;
				cs::Bitmap* bmp;
				if(!GetCHandle(bmp,args[0]->ToObject())) break;
				int alpha,cx,cy,x,y,hotX,hotY;
				alpha = GET_ARGS_INT(1,0);
				cx = GET_ARGS_INT(2,0);
				cy = GET_ARGS_INT(3,0);
				x = GET_ARGS_INT(4,0);
				y = GET_ARGS_INT(5,0);
				hotX = GET_ARGS_INT(6,0);
				hotY = GET_ARGS_INT(7,0);
				if(!b->FromBitmap(*bmp,alpha,cx,cy,x,y,hotX,hotY)) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"destroy()",
		//	"text":"����ͼ����Դ���������ʹ��ͼ�꣬�����������Ƿ���Դ��",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ�����true��ʧ�ܷ���undefined��"
		//	}
		//}//*
		static Handle<Value> destroy(const Arguments& args){
			HandleScope store;
			while(true){
				Local<Object> self = args.This();
				cs::Icon* handle;
				if(!GetCHandle(handle,self)) break;
				if(!handle->Delete()) break;
				return True();
			}
			return Undefined();
		}
	public:
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			SET_CLA_FUNC(load);
			SET_CLA_FUNC(destroy);
			SET_CLA_FUNC(copyFrom);
			SET_CLA_FUNC(fromBitmap);
			SET_CLA_FUNC(draw);
			SET_CLA_FUNC(size);
			SET_CLA_FUNC(toBlackWhite);
		}
	};
	//*],"source":"D:\\SoftProject\\GitLib\\jsuser\\example\\icon.jsuser"}//*

	//*]}//*
	void LoadGDI(Handle<Object>& glb){
		LoadJsRes(IDR_JS_GDI_BEFORE,L"gdi_before.js");
		JSPen::Load(glb,L"CPen");
		JSBrush::Load(glb,L"CBrush");
		JSRgn::Load(glb,L"CRgn");
		JSBitmap::Load(glb,L"CBitmap");
		JSImageLoader::Load(glb,L"CImageLoader");
		JSIcon::Load(glb,L"CIcon");
		JSFont::Load(glb,L"CFont");
		JSTextDraw::Load(glb);
		LoadJsRes(IDR_JS_GDI_AFTER,L"gdi_after.js");
	}
}