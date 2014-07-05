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
	//	"text":"图形、图像和绘图等相关功能。",
	//	"remark":[
	//		"几乎所有绘图函数都需要一个 hdc 参数，这个参数通过设置 gdi 对象的 hdc 属性来传递。",
	//		"gdi 资源是系统资源，非常有限，要节省使用，不要同时生成大量的 gdi 资源，使用完的系统资源要释放掉。比如 CPen 和 CBrush，你可以生成几个不同颜色的对象备用，也可以调用 fromDCPen 或者 fromDCBrush，使用内置 DC 对象，然后调用 setDCColor 设置不同的颜色。JavaScript 是单线程系统，同时只会有一个绘图线程在运行，实际上很少用到多线程的 ui。"
	//	],
	//	"member":[
	//{
	//	"type":"class",
	//	"name":"Color([red],[green],[blue],[alpha])",
	//	"text":"颜色类，参数为红、绿、蓝、透明通道的值，每个通道的取值范围是 0——255，缺省时为 0。",
	//	"member":[
	//{
	//	"type":"function",
	//	"name":"getIntValue()",
	//	"text":"颜色本质上是一个32位整数，这个函数返回颜色的整数值，红、绿、蓝、透明通道依次从低到高每个通道占 8 位。",
	//	"param":[
	//	],
	//	"return":{
	//		"type":"integer",
	//		"text":"返回当前对象代表的颜色的整数值。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"setIntValue(color)",
	//	"text":"设置当前颜色对象的整数值。",
	//	"param":[
	//		{
	//			"type":"integer",
	//			"name":"color",
	//			"text":"一个代表颜色的整数值。"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"函数没有返回值。"
	//	}
	//}
	//,{
	//	"type":"property",
	//	"name":"red",
	//	"objtype":"integer",
	//	"text":"当前颜色的红色值，范围是 0——255，如果设置了一个错误的数值，getIntValue 函数返回整数的时候会用 &0xFF 来处理。"
	//}
	//,{
	//	"type":"property",
	//	"name":"green",
	//	"objtype":"integer",
	//	"text":"当前颜色的绿色值，范围是 0——255，如果设置了一个错误的数值，getIntValue 函数返回整数的时候会用 &0xFF 来处理。"
	//}
	//,{
	//	"type":"property",
	//	"name":"blue",
	//	"objtype":"integer",
	//	"text":"当前颜色的蓝色值，范围是 0——255，如果设置了一个错误的数值，getIntValue 函数返回整数的时候会用 &0xFF 来处理。"
	//}
	//,{
	//	"type":"property",
	//	"name":"alpah",
	//	"objtype":"integer",
	//	"text":"当前颜色的 alpha 值，范围是 0——255，如果设置了一个错误的数值，getIntValue 函数返回整数的时候会用 &0xFF 来处理。"
	//}
	//]}
	//,{
	//	"type":"class",
	//	"name":"CPen",
	//	"text":"画笔类，它继承自 CHandleObject，用于控制画线时的颜色，宽度，样式等。",
	//	"member":[//*
	class JSPen : public JsHandleObject<cs::Pen,HPEN,JSPen,TEMPLATE_ID_PEN>{
		//*{
		//	"type":"function",
		//	"name":"arc(rect,point1,point2)",
		//	"text":"绘制一段弧线，这个弧线是参数 1 指定的矩形的内接椭圆的一部分，从矩形的中心点向参数 2 和参数 3 分别画两条直线，和椭圆的两个交点（交点1、交点2）分别是弧线的起点和终点，画弧线的方向是交点 1 到交点 2 的逆时针方向。显然，如果参数 2 和参数 3 是同一个点，函数会画一个完整的椭圆。需要设置 hdc 属性。",
		//	"param":[
		//		{
		//			"type":"Rect",
		//			"name":"rect",
		//			"text":"弧线是此矩形的内切椭圆的一部分。"
		//		},
		//		{
		//			"type":"Point",
		//			"name":"[point1]",
		//			"text":"这个点和椭圆的交点是弧线的起始点。如果此参数缺省，函数画一个完整的椭圆。"
		//		},
		//		{
		//			"type":"Point",
		//			"name":"[point2]",
		//			"text":"这个点和椭圆的交点是弧线的结束点。如果此参数缺省，函数画一个完整的椭圆。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"绘制一个矩形。需要设置 hdc 属性。",
		//	"param":[
		//		{
		//			"type":"Rect",
		//			"name":"rect",
		//			"text":"要绘制的矩形。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"连接若干点，形成一条曲线。需要设置 hdc 属性。",
		//	"param":[
		//		{
		//			"type":"array",
		//			"name":"pointArray",
		//			"text":"一个点组成的数组，且长度大于 2 。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"移动当前位置到指定点，这个函数是配合 lineTo 函数使用的，lineTo 函数总是从当前位置到指定位置画一条线。需要设置 hdc 属性。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"x",
		//			"text":"x 坐标"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"y",
		//			"text":"y 坐标"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"从当前位置到指定位置画一条直线。需要设置 hdc 属性。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"x",
		//			"text":"x 坐标"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"y",
		//			"text":"y 坐标"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"画一个矩形，这个函数的参数有两种模式，一种是传 4 个整数，一种是传一个 Rect 对象。需要设置 hdc 属性。",
		//	"param":[
		//		{
		//			"type":"Rect",
		//			"name":"rect",
		//			"text":"一个矩形对象。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
					//可以把left的地址看成一个Rect的地址。
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
		//	"text":"画一个圆角矩形，这个函数有两种模式，一种是传矩形的 4 个属性值，和圆角的两个半径值；一种是传一个矩形和圆角的两个半径值。",
		//	"param":[
		//		{
		//			"type":"Rect",
		//			"name":"rect",
		//			"text":"一个矩形对象。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"rx",
		//			"text":"圆角矩形的角实际上是一个椭圆的 1/4，这个值是椭圆的水平半径。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"ry",
		//			"text":"圆角矩形的角实际上是一个椭圆的 1/4，这个值是椭圆的水平半径。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
					//可以把left的地址看成一个Rect的地址。
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
		//	"text":"获取对象的颜色，宽度，风格 3 个参数。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"object",
		//		"text":"返回值的 width，color，style 3 个属性代表当前对象的对应参数。"
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
		//	"text":"生成系统画笔。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"[color]",
		//			"text":"指定画笔的颜色。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[width]",
		//			"text":"指定画笔的粗细。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[style]",
		//			"text":"指定画笔的样式风格。style 可用的值如下: <br/>CPen.SOLID(实线，这是最常用的样式，也是缺省值）<br/>CPen.DASH(短线)<br/>CPen.DOT(点)<br/>CPen.DASHDOT(短线和点交替)<br/>CPen.DASHDOTDOT(一短线两点)<br/>CPen.NULL(空画笔，没有任何显示)"
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
		//	"text":"设置当前对象为 DC_PEN，DC_PEN 是系统内置的 Pen，不能被销毁，调用 destroy 没有效果。使用 DC_PEN 效率会高一些，而且因为不用生成新的 Pen，节省了系统资源，可以通过 setDCColor 函数设置 DC_PEN 的颜色，但是无法更改宽度和样式，DC_PEN 总是一个像素宽度的 solid 实线 画笔。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"fromStockPen(stock)",
		//	"text":"获取系统内置的画笔对象，查阅微软的文档获取系统内置的画笔参数值，系统内置画笔不会被销毁。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"[stock]",
		//			"text":"内置画笔的代码，缺省获取 DC_PEN。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"设置 DC_PEN 的颜色，调用之前需要设置对象的 hdc 属性。这个函数实际上与当前对象没有关系，它设置的是 hdc 的属性。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"[color]",
		//			"text":"指定画笔的颜色，缺省为黑色。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"释放对象绑定的系统画笔资源。",
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
	//	"text":"区域类，它继承自 CHandleObject，用于表示一个区域，规则的或不规则的。区域（Region）是 Windows 的系统对象，用于很多绘图函数。",
	//	"member":[//*
	class JSRgn : public JsHandleObject<cs::Rgn,HRGN,JSRgn,TEMPLATE_ID_RGN>{
	protected:
		//*{
		//	"type":"function",
		//	"name":"rect(rect)",
		//	"text":"生成一个矩形区域。",
		//	"param":[
		//		{
		//			"type":"Rect",
		//			"name":"rect",
		//			"text":"一个矩形对象。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"由若干点连接的闭合曲线圈起来的区域。",
		//	"param":[
		//		{
		//			"type":"array",
		//			"name":"pointArray",
		//			"text":"一系列 Point 对象形成的数组，数组元素个数必须大于等于 3 。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"生成一个椭圆区域。",
		//	"param":[
		//		{
		//			"type":"Rect",
		//			"name":"[rect]",
		//			"text":"生成的区域是这个矩形的内切椭圆。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"组合当前 Region 和参数 1 指定的 Region，组合方式由 mode 指定，结果保存在当前对象。",
		//	"param":[
		//		{
		//			"type":"CRgn",
		//			"name":"rgn",
		//			"text":"要和当前 Region 组合的 CRgn 对象。"
		//		},
		//		{
		//			"type":"integer|string",
		//			"name":"mode",
		//			"text":"组合方式，可以是以下的值:<br/>“and”或 1，两个区域的交集。<br/>“or”或 2，两个区域并集。<br/>“xor”或 3，两个区域交集之外的部分（对等集）。<br/>“diff”或 4，从当前区域减去和 rgn 相交的部分。<br/>“copy”或 5，复制 rgn 到当前对象。"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"函数成功，返回值是一个 1 到 3 的整数，失败返回 undefined。<br/>1：表示结果是一个空的区域。<br/>2：表示结果是一个简单区域。<br/>3：表示结果是多个简单区域。"
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
		//	"text":"生成一个圆角矩形区域，圆角矩形的 4 个角是一个椭圆的 1/4。",
		//	"param":[
		//		{
		//			"type":"Rect",
		//			"name":"rect",
		//			"text":"一个矩形。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"rx",
		//			"text":"椭圆的水平半径。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"ry",
		//			"text":"椭圆的垂直半径。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
				//可以把left的地址看成一个Rect的地址。
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
		//	"text":"平移一个区域。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"dx",
		//			"text":"水平移动像素。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"dy",
		//			"text":"垂直移动像素。"
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
		//	"text":"判断当前 Region 和 rgn 是否相同。",
		//	"param":[
		//		{
		//			"type":"CRgn",
		//			"name":"rgn",
		//			"text":"要比较的 Region。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"返回比较结果（true 或 false），失败返回 undefined。"
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
		//	"text":"获取当前 Region 的外接矩形。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"Rect",
		//		"text":"成功返回一个 Rect 对象，失败返回 undefined。"
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
		//	"text":"一个点是否在区域内。",
		//	"param":[
		//		{
		//			"type":"Point",
		//			"name":"pt",
		//			"text":"要检测的点。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回一个 boolean 值，失败返回 undefined。"
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
		//	"text":"销毁 Region 资源。",
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
	//	"text":"画刷对象，它继承自 CHandleObject，画刷用于绘制一个区域。",
	//	"member":[//*
	class JSBrush : public JsHandleObject<cs::Brush,HBRUSH,JSBrush,TEMPLATE_ID_BRUSH>{
	protected:
		//*{
		//	"type":"function",
		//	"name":"create(color)",
		//	"text":"生成一个指定颜色的 BS_SOLD 风格的画刷。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"color",
		//			"text":"画刷的颜色。"
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
		//	"text":"获取画刷的颜色。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"成功返回一个代表颜色的整数，失败返回 undefined。"
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
		//	"text":"使用当前画刷填充一个区域。需要设置 hdc 属性。",
		//	"param":[
		//		{
		//			"type":"CRgn",
		//			"name":"rgn",
		//			"text":"要填充的 Region。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"填充一个矩形。需要设置 hdc 属性。",
		//	"param":[
		//		{
		//			"type":"Rect",
		//			"name":"rect",
		//			"text":"要填充的矩形。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"填充一个闭合曲线形成的区域。需要设置 hdc 属性。",
		//	"param":[
		//		{
		//			"type":"array",
		//			"name":"pointArray",
		//			"text":"大于等于 3 个 Point 对象构成的数组，这些点首尾连接形成的区域就是填充的区域。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"填充一个矩形的内切椭圆，需要设置 hdc 属性。",
		//	"param":[
		//		{
		//			"type":"Rect",
		//			"name":"rect",
		//			"text":"一个矩形对象。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"填充一个圆角矩形的区域，需要设置 hdc 属性。",
		//	"param":[
		//		{
		//			"type":"Rect",
		//			"name":"rect",
		//			"text":"一个矩形对象。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"rx",
		//			"text":"圆角的水平半径。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"ry",
		//			"text":"圆角的垂直半径。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
					//可以把left的地址看成一个Rect的地址。
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
		//	"text":"释放系统的画刷资源。",
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
		//	"text":"设置当前对象为 DC_BRUSH，DC_BRUSH 是系统内置的 Brush，不能被销毁，调用 destroy 没有效果。使用 DC_BRUSH 效率会高一些，而且因为不用生成新的 Brush，节省了系统资源，可以通过 setDCColor 函数设置 DC_BRUSH 的颜色。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"fromStockBrush(stock)",
		//	"text":"获取系统内置的画刷对象，查阅微软的文档获取系统内置的画刷参数值，系统内置画刷不会被销毁。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"[stock]",
		//			"text":"内置画刷的代码，缺省获取 DC_BRUSH。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"设置 DC_BRUSH 的颜色，调用之前需要设置对象的 hdc 属性。这个函数与当前对象无关，它设置 hdc 的属性。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"[color]",
		//			"text":"指定画刷的颜色，缺省为黑色。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
	//	"text":"字体，它继承自 CHandleObject，字体是系统用于输出文字的 gdi 对象。",
	//	"member":[//*
	class JSFont : public JsHandleObject<cs::Font,HFONT,JSFont,TEMPLATE_ID_FONT>{
	public:
		//*{
		//	"type":"function",
		//	"name":"fromDefault()",
		//	"text":"绑定默认字体，wnd 控件默认使用这个字体（除非给一个控件设置了独立的字体），对这个字体的任何操作会影响所有使用这个字体的对象。但是缺省字体是绑定到这个对象上的，无法 dispose，而且 destroy 也会返回 undefined。如果要改变缺省字体，必须在任何使用缺省字体的控件创建之前生成字体，否则显示可能会不正常，因为控件在创建时会设置字体，在这之后改变字体的话，控件不知道字体已经改变。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"生成字体资源，设置字体的属性不会立即改变当前字体，需要调用 create 重新生成才字体会生效。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"设置当前字体的参数为系统缺省值，需要调用 create 后才会生效。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"销毁字体资源。",
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
		//	"text":"获取/设置字体的大小，这个参数可以是负数，意义和正数差不多。"
		//}
		//,{
		//	"type":"property",
		//	"name":"name",
		//	"objtype":"string",
		//	"text":"获取/设置字体的名称，空串或无效名称会被当成系统默认字体。"
		//}
		//,{
		//	"type":"property",
		//	"name":"weight",
		//	"objtype":"integer",
		//	"text":"获取/设置字体的粗体程度，400 是默认值（普通字体），900 是粗体字。"
		//}
		//,{
		//	"type":"property",
		//	"name":"underline",
		//	"objtype":"boolean",
		//	"text":"获取/设置字体的下划线。"
		//}
		//,{
		//	"type":"property",
		//	"name":"strikeOut",
		//	"objtype":"boolean",
		//	"text":"获取/设置字体的删除线。"
		//}
		//,{
		//	"type":"property",
		//	"name":"logfont",
		//	"objtype":"integer",
		//	"text":"获取/设置字体的 logfont，logfont 是字体内部的一个对象指针。当设置这个属性的时候，没有类型检查，所以应该确保这是一个有效的 logfont 指针。"
		//}
		//,{
		//	"type":"property",
		//	"name":"italic",
		//	"objtype":"boolean",
		//	"text":"获取/设置字体是否斜体。"
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
	//	"text":"CBitmap 管理系统位图资源，位图是 Windows 图像系统使用的资源。",
	//	"remark":[
	//		"大尺寸的位图非常耗费系统资源，不要在内存中保留大量的大尺寸位图对象。绘制大尺寸位图也非常耗时间。"
	//	],
	//	"member":[//*
	class JSBitmap : public JsHandleObject<cs::Bitmap,HBITMAP,JSBitmap,TEMPLATE_ID_BITMAP>{
	protected:
		//*{
		//	"type":"function",
		//	"name":"save(filename,[quality])",
		//	"text":"保存当前位图对象到一个文件，文件格式取决于文件名的后缀，支持的格式取决于系统安装的解码器，通常支持 jpg、png、gif（单帧）、bmp 等等。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"filename",
		//			"text":"文件名，后缀决定保存的格式。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[quality]",
		//			"text":"图像质量，质量越高，文件越大，图像失真越小。这个值的范围是 0 到 100 ,缺省值是 100。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"读取一个图像文件，支持 jpg、png、gif（只能读取第一帧）、bmp 等格式。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"filename",
		//			"text":"图像文件的路径，图像文件读取的时候具体格式识别是文件内容决定的，与文件名后缀无关。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"从内存加载一个位图，这个内存必须是一个有效的图像文件数据。",
		//	"param":[
		//		{
		//			"type":"CMemory",
		//			"name":"mem",
		//			"text":"包含图像文件数据的内存对象。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[len]",
		//			"text":"数据长度，如果这个值缺省或者是一个错误的数据，长度取 mem 的整个大小。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"把图像保存到内存数据，而不是文件，这些数据和保存到文件的数据是相同的。",
		//	"param":[
		//		{
		//			"type":"CMemory",
		//			"name":"mem",
		//			"text":"用于保存图像数据的内存对象，mem 如果不够大，会被自动扩充，这时会被强制转换为 system 内存。如果 mem 足够大，则不会调整长度，所以，数据长度应该看返回值，而不是 mem 的长度。"
		//		},
		//		{
		//			"type":"string",
		//			"name":"type",
		//			"text":"保存图像的类型，如“jpg”、“gif” 等。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[quality]",
		//			"text":"图像质量，质量越高，文件越大，图像失真越小。这个值的范围是 0 到 100 ,缺省值是 100。"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"成功返回图像数据的长度，失败返回 undefined。"
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
		//	"text":"获取当前位图的尺寸。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"Size",
		//		"text":"成功返回一个表示图像尺寸的 Size 对象，失败返回 undefined。"
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
		//	"text":"调整图像的尺寸。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"cx",
		//			"text":"设置图像的宽度，0 或负数会导致函数失败。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"cy",
		//			"text":"设置图像的高度，0 或负数会导致函数失败。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"生成一个指定宽度的位图。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"cx",
		//			"text":"设置图像的宽度，0 或负数会导致函数失败。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"cy",
		//			"text":"设置图像的高度，0 或负数会导致函数失败。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。<b>位图比较占用系统资源，特别是大位图，所以内存不够可能导致函数失败</b>。"
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
		//	"text":"复制一个位图的一部分。",
		//	"param":[
		//		{
		//			"type":"CBitmap",
		//			"name":"bitmap",
		//			"text":"要复制的位图资源。这个值可以是它自己。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[cx]",
		//			"text":"要复制的位图资源的宽度，不能超过源位图允许的范围。缺省、0 或其它值 表示最大宽度。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[cy]",
		//			"text":"要复制的位图资源的高度，不能超过源位图允许的范围。缺省、0 或其他值 表示最大高度。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[x]",
		//			"text":"要复制的位图资源的左上角位置 x 坐标，不能超过源位图允许的范围。缺省或其他值 表示 0 。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[y]",
		//			"text":"要复制的位图资源的左上角位置 y 坐标，不能超过源位图允许的范围。缺省或其他值 表示 0 。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"复制图标的彩色图像，图标包含两幅图像。",
		//	"param":[
		//		{
		//			"type":"CIcon",
		//			"name":"icon",
		//			"text":"要复制的图标对象。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"获取位图的像素数组。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"array",
		//		"text":"成功返回一个数组，数组的元素是一个代表颜色的整数，每个元素代表一个像素，第一个元素表示左下角的像素颜色，依次向右，然后向上，最后一个元素是右上角的像素，位图的像素数是长乘以宽。失败返回 undefined。"
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
		//	"text":"设置位图的像素。",
		//	"param":[
		//		{
		//			"type":"array",
		//			"name":"pixelArray",
		//			"text":"包含像素数据的一个数组，这个数组的长度必须是位图的长乘以宽，即和位图的像素数相同。每个元素代表像素的颜色。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"销毁位图资源。",
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
		//	"text":"。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"hdc",
		//			"text":"图像绘制的 hdc 。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[x]",
		//			"text":"位图绘制水平位置，缺省是 0."
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[y]",
		//			"text":"位图绘制垂直位置，缺省是 0 。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[cx]",
		//			"text":"位图绘制宽度，缺省是 0 ，表示原始宽度，没有拉伸。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[cy]",
		//			"text":"位图绘制高度，缺省是 0 ，表示原始高度，没有拉伸。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[sx]",
		//			"text":"位图源起始位置（左上角） x 坐标，缺省是 0。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[sy]",
		//			"text":"位图源起始位置（左上角） y 坐标，缺省是 0。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[scx]",
		//			"text":"位图源宽度，缺省是 0，表示最大的可能宽度。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[scy]",
		//			"text":"位图源高度，缺省是 0，表示最大的可能高度。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"alphaDraw 增加一个 alpha 参数，这个参数指定绘制图像时的透明度。透明是指，绘制完成后背景图像一定程度的被“透过”，而不是完全被覆盖。alphaDraw 比 draw 会慢很多。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"hdc",
		//			"text":"图像绘制的 hdc 。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[alpha]",
		//			"text":"透明度，取值范围是（0——255），0 表示完全透明，当前图像完全不覆盖背景，255 表示完全不透明，效果和 draw 相同。这个值缺省是 255。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[isAlpha]",
		//			"text":"图像是否考虑透明通道。图像的每个像素，除了红绿蓝三个像素值之外，还有一个 alpha 值，它的取值范围也是 0——255，如果设为考虑透明通道（isAlpha = true），alphaDraw 在绘图时，每个像素会按照它的 alpha 值来决定透明度，0 完全透明，255 完全不透明。图标和 png 图像有透明通道，普通图像没有透明通道，也就是普通图像每个像素的 alpha 值都是 0。所以按绘制透明图的方式绘制普通图像，相当于整个图像是透明的，什么都看不到。反之，如果按照绘制普通图的方式绘制透明图像，alpah 通道的值完全不考虑，透明图会失去透明效果（会有一个黑色背景）。isAlpha = false 相当于把任意像素的 alpha 值设为 255，所以要正确的绘制普通图像，必须设置 isAlpha = false。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[x]",
		//			"text":"位图绘制水平位置，缺省是 0."
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[x]",
		//			"text":"位图绘制水平位置，缺省是 0."
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[y]",
		//			"text":"位图绘制垂直位置，缺省是 0 。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[cx]",
		//			"text":"位图绘制宽度，缺省是 0 ，表示原始宽度，没有拉伸。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[cy]",
		//			"text":"位图绘制高度，缺省是 0 ，表示原始高度，没有拉伸。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[sx]",
		//			"text":"位图源起始位置（左上角） x 坐标，缺省是 0。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[sy]",
		//			"text":"位图源起始位置（左上角） y 坐标，缺省是 0。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[scx]",
		//			"text":"位图源宽度，缺省是 0，表示最大的可能宽度。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[scy]",
		//			"text":"位图源高度，缺省是 0，表示最大的可能高度。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
	//	"text":"此对象提供绘制文本的功能。",
	//	"member":[//*
	class JSTextDraw{
		//*{
		//	"type":"function",
		//	"name":"draw(str,left,top,[width],[height])",
		//	"text":"在指定的矩形内绘制文本，需要设置对象的 hdc 属性。",
		//	"param":[
		//		{
		//			"类型":"string",
		//			"name":"str",
		//			"text":"要绘制的文本。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"left",
		//			"text":"矩形的 left 位置。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"top",
		//			"text":"矩形的 top 位置。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[width]",
		//			"text":"矩形的宽度，缺省值是 0xffff，一个足够大的值。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[height]",
		//			"text":"矩形的高度，缺省值是 0xffff，一个足够大的值。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回实际输出的字符数，失败返回 undefined。"
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
		//	"text":"设置绘图的单行属性，当对象有单行属性时，无视回车换行符，所有文本都在第一行输出。对象默认没有此属性，回车（\\r）、换行（\\n）、回车换行（\\r\\n）输出时都会换行。",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true 设置此属性，false 取消此属性。"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"函数没有返回值。"
		//	}
		//}
		//,{
		//	"type":"function",
		//	"name":"setWordBreak(yes)",
		//	"text":"设置自动换行属性，如果矩形的宽度无法输出一个整行文本，会自动换行，如果没有此属性，文本只会遇到回车换行符时才会换行。此属性不会切断单词，如果一个单词在单行无法显示，会被剪切（但是返回值仍然计算没有显示出来的字符）。此属性是默认属性。",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true 设置此属性，false 取消此属性。"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"函数没有返回值。"
		//	}
		//}
		//,{
		//	"type":"function",
		//	"name":"setEditControl(yes)",
		//	"text":"设置类编辑控件属性，这个属性不会输出半行文本（如果不具有此属性，底部文本可能被剪切，只显示上边的部分），单词不会被剪切，而是强制换行到下一行显示（如果设置了 wordbread 属性）。这个属性是默认属性。",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true 设置此属性，false 取消此属性。"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"函数没有返回值。"
		//	}
		//}
		//,{
		//	"type":"function",
		//	"name":"setNoClip(yes)",
		//	"text":"设置不剪切属性，这个属性会顺序输出所有文本，无视高度参数，如果设置了不换行属性，也会无视宽度参数。此属性不是默认属性。",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true 设置此属性，false 取消此属性。"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"函数没有返回值。"
		//	}
		//}
		//,{
		//	"type":"function",
		//	"name":"setEndEllipsis(yes)",
		//	"text":"设置省略号属性，如果矩形无法显示全部文本，此属性会在尾部显示一个省略号。此属性不是默认属性",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true 设置此属性，false 取消此属性。"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"函数没有返回值。"
		//	}
		//}
		//,{
		//	"type":"function",
		//	"name":"setNoPrefix(yes)",
		//	"text":"设置 noprefix 属性，如果没有此属性，字串中的“&”字符后面的一个字符会显示下划线（要显示“&”字符用两个连续的“&&”），设置此属性关闭这个特性。此属性不是默认属性。",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true 设置此属性，false 取消此属性。"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"函数没有返回值。"
		//	}
		//}
		//,{
		//	"type":"function",
		//	"name":"setAlignLeft(yes)",
		//	"text":"设置文本对齐方式为左对齐，此属性是默认属性。",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true 设置此属性，false 取消此属性。"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"函数没有返回值。"
		//	}
		//}
		//,{
		//	"type":"function",
		//	"name":"setAlignCenter(yes)",
		//	"text":"设置文本对齐方式为中间对齐，此属性不是默认属性。",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true 设置此属性，false 取消此属性。"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"函数没有返回值。"
		//	}
		//}
		//,{
		//	"type":"function",
		//	"name":"setAlignRight(yes)",
		//	"text":"设置文本对齐方式为右对齐，此属性不是默认属性。",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true 设置此属性，false 取消此属性。"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"函数没有返回值。"
		//	}
		//}
		//,{
		//	"type":"function",
		//	"name":"setAlignTop(yes)",
		//	"text":"设置文本垂直对齐方式为上对齐，此属性是默认属性。",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true 设置此属性，false 取消此属性。"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"函数没有返回值。"
		//	}
		//}
		//,{
		//	"type":"function",
		//	"name":"setAlignVCenter(yes)",
		//	"text":"设置垂直文本对齐方式为中间对齐，此属性不是默认属性。这个属性仅仅设置了单行属性的时候才会起作用。",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true 设置此属性，false 取消此属性。"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"函数没有返回值。"
		//	}
		//}
		//,{
		//	"type":"function",
		//	"name":"setAlignBottom(yes)",
		//	"text":"设置垂直文本对齐方式为下对齐，此属性不是默认属性。",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"yes",
		//			"text":"true 设置此属性，false 取消此属性。"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"函数没有返回值。"
		//	}
		//}
		//,{
		//	"type":"property",
		//	"name":"name",
		//	"objtype":"string",
		//	"text":"获取/设置字体的名称。"
		//}
		//,{
		//	"type":"property",
		//	"name":"hdc",
		//	"objtype":"integer",
		//	"text":"获取/设置对象的 hdc 属性，draw 函数需要这个属性。"
		//}
		//,{
		//	"type":"property",
		//	"name":"tabstop",
		//	"objtype":"integer",
		//	"text":"获取/设置 tab 字符的显示，默认值是 8，表示 tab 字符的宽度是 8 个字符（4个汉字）。如果设置此值小于等于 0，tab 字符不会显示。"
		//}
		//,{
		//	"type":"property",
		//	"name":"font",
		//	"objtype":"CFont",
		//	"text":"获取/设置绘制文本使用的字体，此属性的默认值是 null，使用默认字体。"
		//}
		//,{
		//	"type":"property",
		//	"name":"color",
		//	"objtype":"integer",
		//	"text":"获取/设置文本的颜色，默认值是 0，黑色。"
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
	//	"text":"一个图像文件不一定只有一帧图像，比如 gif 可以有多帧图像，这个类可以加载第一帧以外的图像。",
	//	"member":[//*
	class JSImageLoader : public JsCObject<cs::Image,JSImageLoader,TEMPLATE_ID_IMAGELOADER>{
		//*{
		//	"type":"function",
		//	"name":"load(file|mem,[len])",
		//	"text":"从图像文件或者内存加载一个图像.",
		//	"param":[
		//		{
		//		"type":"string|CMemory",
		//		"name":"file|mem",
		//		"text":"当参数是一个字串类型时, 被看成一个图像文件。当参数是一个 CMemory 类型的对象时, 使用它存储的数据生成图像, 如果没有指定 len 参数，数据的长度就是 CMemory 对象的大小。"
		//		},
		//		{
		//		"type":"integer",
		//		"name":"[len]",
		//		"text":"当第一个参数是一个内存对象时，指定数据的长度，如果此参数缺省，数据长度被认为是内存对象的大小。"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"成功返回图像的帧数，失败返回 undefined"
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
		//	"text":"获取一帧图像, 一个图像文件可能含有多帧图像. 通常的单帧图像实际上是第一帧图像.",
		//	"param":[
		//		{
		//		"type":"CBitmap",
		//		"name":"bitmap",
		//		"text":"用于存储图像的 CBitmap 对像。"
		//		},
		//		{
		//		"type":"integer",
		//		"name":"[index]",
		//		"text":"要获取的图像帧的索引，缺省是 0，如果索引超过允许的范围，函数失败。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
	//	"text":"图标类。",
	//	"member":[//*
	class JSIcon : public JsHandleObject<cs::Icon,HICON,JSIcon,TEMPLATE_ID_ICON>{
		//*{
		//	"type":"function",
		//	"name":"load(finename,[cx],[cy])",
		//	"text":"加载一个图标文件，一个图标文件实际上是一个图标组，可能含有多个图标。cx，cy 指定要加载什么尺寸的图标，如果指定尺寸的图标不存在，函数会自动寻找一个尺寸最接近的图标。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"finename",
		//			"text":"图标文件的名称，这个函数也可以用于加载 cur 鼠标指针文件。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[cx]",
		//			"text":"要加载的图标的宽度，如果缺省使用系统的缺省值，比如 32 像素。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[cy]",
		//			"text":"要加载的图标的高度，如果缺省使用系统的缺省值，比如 32 像素。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回true，失败返回 undefined。"
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
		//	"text":"。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"hdc",
		//			"text":"要绘制图标的 hdc 。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"x",
		//			"text":"绘制图标位置 x 坐标。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"y",
		//			"text":"绘制图标位置的 y 坐标。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[cx]",
		//			"text":"绘制宽度，缺省是原始宽度。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[cy]",
		//			"text":"绘制高度，缺省是原始高度。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回true，失败返回undefined。"
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
		//	"text":"获取图标的尺寸。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"Size",
		//		"text":"成功返回一个 Size 对象，失败返回undefined。"
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
		//	"text":"把当前图标转换为黑白图像，注意，图标的位深是不变的。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"从一个图标对象复制图标。",
		//	"param":[
		//		{
		//			"type":"CIcon",
		//			"name":"icon",
		//			"text":"源图标对象。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"从一个 CBitmap 图像生成图标。",
		//	"param":[
		//		{
		//			"type":"CBitmap",
		//			"name":"bitmap",
		//			"text":"源 bitmap。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"销毁图标资源，如果不在使用图标，尽快销毁以是否资源。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回true，失败返回undefined。"
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