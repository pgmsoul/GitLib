#pragma once
/*�����ǵ�;��εĶ���.*/
namespace cs{
#pragma pack(push,2)
	//���൱�ڵ�
	typedef union CONCISE_API Twin32{
		struct{__int32 x,y;};
		struct{__int64 Value;};

		inline Twin32(){}
		inline Twin32(int x,int y){this->x = x;this->y = y;}
		inline operator POINT(){return *(LPPOINT)this;}
		inline operator LPPOINT(){return (LPPOINT)this;}
		inline operator SIZE(){return *(LPSIZE)this;}
		inline operator LPSIZE(){return (LPSIZE)this;}
		inline SIZE ToSize(){return *(LPSIZE)this;}
		inline POINT ToPoint(){return *(LPPOINT)this;}
		inline LPSIZE ToSizePointer(){return (LPSIZE)this;}
		inline LPPOINT ToPointPointer(){return (LPPOINT)this;}
		inline void operator = (POINT& pt){x = pt.x;y = pt.y;}
		inline void operator = (SIZE& sz){x = sz.cx;y = sz.cy;}
		inline void operator = (LPPOINT pp){x = pp->x;y = pp->y;}
		inline void operator = (LPSIZE ps){x = ps->cx;y = ps->cy;}
		inline bool operator == (POINT& pt){return (x==pt.x)&&(y==pt.y);}
		inline bool operator == (SIZE& sz){return (x==sz.cx)&&(y==sz.cx);}
		inline bool operator == (Twin32& tw){return (x==tw.x)&&(y==tw.y);}
		inline bool operator != (POINT& sz){return (x!=sz.x)||(y!=sz.y);}
		inline bool operator != (SIZE& sz){return (x!=sz.cx)||(y!=sz.cx);}
		inline bool operator != (Twin32& tw){return (x!=tw.x)||(y!=tw.y);}
		inline void SetValue(int x,int y){this->x = x;this->y = y;}
		inline void SetValue(__int64 value){Value = value;}
		inline void Offset(int dx,int dy){x += dx;y += dy;}

		void* operator new(size_t size){return MemoryAlloc((int)size);}
		void operator delete(void* p){MemoryFree(p);}
	}Twin;
	//���൱��16λ��
	typedef union CONCISE_API Twin16{
		struct{__int16 x,y;};
		struct{__int32 Value;};

		inline Twin16(){}
		inline Twin16(INT_PTR value){this->Value = (int)value;}
		inline Twin16(short x,short y){this->x = x;this->y = y;}
		inline void SetValue(int value){this->Value = value;}
		inline void SetValue(short x,short y){this->x = x;this->y = y;}
		inline void operator = (int n){Value = n;}
		inline void operator = (LPARAM lParam){Value = (int)lParam;}
		inline void operator = (WPARAM wParam){Value = (int)wParam;}
		inline void operator = (Twin& tw){x = tw.x;y = tw.y;}
		inline void Offset(int x,int y){this->x += x;this->y += y;}
		inline operator Twin (){return Twin(x,y);}
		inline operator int (){return Value;}

		void* operator new(size_t size){return MemoryAlloc((int)size);}
		void operator delete(void* p){MemoryFree(p);}
	}Twin16;
	//���ζ���.
	typedef struct CONCISE_API Rect32 : public _struct{
	public:
		__int32 left,top,right,bottom;

		Rect32(){}
		Rect32(Rect32& rect);
		Rect32(int left,int top,int right,int bottom);
		Rect32(Twin pt,Twin sz);
		inline operator LPRECT(){return (LPRECT)&left;}
		inline operator RECT(){return *((LPRECT)this);}
		inline void operator = (const RECT* pr){memcpy(this,pr,16);}
		inline Twin Size(){return Twin(right - left,bottom - top);}
		inline int Width(){return right - left;}
		inline int Height(){return bottom - top;}
		inline Twin LeftTop(){return *((Twin*)&left);}
		inline Twin RightBottom(){return *((Twin*)&right);}
		inline void operator = (Rect32& r){::MoveMemory(&left,&r.left,sizeof(Rect32));}
		inline void operator = (RECT& r){::MoveMemory(&left,&r,sizeof(Rect32));}
		inline bool operator != (Rect32& r){return memcmp(&left,&r.left,sizeof(RECT))!=0;}
		inline bool operator != (RECT& r){return memcmp(&left,&r,sizeof(RECT))!=0;}
		inline bool operator == (Rect32& r){return !memcmp(&left,&r.left,sizeof(RECT));}
		inline bool operator == (RECT& r){return !memcmp(&left,&r,sizeof(RECT));}
		inline void OffsetSize(int dx,int dy){right += dx;bottom += dy;}
		inline void SetSize(Twin sz){right = left + sz.x;bottom = top + sz.y;}
		inline bool IsEmpty(){return (left==right)|(top==bottom);}

		void SetValue(int x,int y,int r,int b);
		void SetValue(Twin pt,Twin sz);
		void SetSize(int cx,int cy);
		void SetLocation(int x,int y);
		void SetLocation(Twin pt);
		//ƽ��һ������,4�������Ǹ���ı仯��.
		void Offset(int dleft,int dtop,int dright,int dbottom);
		//������λ��(����).
		void OffsetLocation(int dx,int dy);
		//����(����С)����,sx�����ҷֱ��������,sy���·ֱ��������.��ֵ��Ӧ��С.abs�����Ƿ������ҹ�ϵ,leftʵ���Ͽ�����
		//right�ұ�,abs=1ʱ,�ῼ�����Ч��,��sxΪ��ʱ,������������.
		void Increase(int sx,int sy,bool abs = 0);
		//�����������һ��������ͬ,�����Էֱ����4���ߵĴ�С.
		void Increase(int sl,int st,int sr,int sb,bool abs = 0);
		//�������Һ����µ�λ��,ʹleft<right,top<bottom.
		void Positive();
		//�ж�һ�����Ƿ��ھ����ڲ�.
		bool PtInRect(LPPOINT pt);
		bool PtInRect(int x,int y);
		//��ǰ���κ� r �ཻ������洢�ڵ�ǰ���Σ�����������ڲ�����API����IntersectRect
		bool Intersect(LPRECT r);
		//��ǰ���κ�ָ�������Ƿ��ཻ����ǰ���β���ı䡣
		bool IsIntersect(LPRECT r);
	}Rect;
#pragma pack(pop)
}