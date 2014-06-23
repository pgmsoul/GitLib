#pragma once
namespace cs{
	CONCISE_API bool GetImageEncoderClsid(const wchar_t* type,CLSID* pClsid);
	//ImageItem �� Gdiplus �� PropertyItem ��.	
	class ImageItem{
	public:
		PROPID  id;                 // ID of this property
		ULONG   length;             // Length of the property value, in bytes
		WORD    type;               // Type of the value, as one of TAG_TYPE_XXX
		VOID*   value;              // property value
	};
	//��ӦGdiplus::BitmapData���ͣ�Data���������������ݡ�
	class BitmapData{
	public:
		UINT Width;
		UINT Height;
		INT Stride;
		UINT PixelFormat;
		BYTE* Data;
		UINT_PTR Reserved;
	};

	//Image����һ��ͼ����Դ,ĿǰΨһ����;���Ǽ��غͱ��治ͬ���͵�ͼ���ļ�.��Image��Դ��ͼЧ���Ƿǳ��͵�.
	class CONCISE_API Image : public _class{
	protected:
		void* _image;
	public:
		Image();
		//��λͼ��Դ����, ��Ҫ�ֶ�ɾ��bmp��Դ��
		bool Create(HBITMAP bmp);
		//���ļ�����;
		bool Create(const wchar_t* file);
		//���ڴ���أ����������mem������file��ȡ�������.len�����ݳ��ȡ�
		bool Create(void* mem,int len);
		//���õ�ǰ֡����ǰ֡�Ǳ���ͻ�ͼʹ�õ�֡��
		bool SetActiveFrame(int index);
		//����Ϊ�ļ�;quality�Ǳ���������100�����ֵ��0����Сֵ��
		bool Save(const wchar_t* file,long quality = 60);
		//���浽�ڴ�,type�ǰ�����ŵ������������硰.jpg���ȣ�quality�Ǳ���������100�����ֵ��0����Сֵ��
		int Save(Memory<char>& mem,LPCWSTR type,long quality = 60);
		//��ͼ;
		bool Draw(HDC hdc,int x,int y,int cx = 0,int cy = 0);
		//��
		int Width();
		//��
		int Height();
		//��ȡsrImg������ͼ�����srImgΪ0,����ȡ�����Thumbnail.
		bool GetThumbnail(int cx,int cy,Image* srImg = 0);
		//����
		void operator = (Image& img);
		//�رգ���Դ������
		void Close();
		//����ͼ��(gif,jpg)��, ��ά��֡��������, ��ĳάĳ֡ͼ��������ͼ��, ����άһ�㷵��ֵ�� 1, Ŀǰͼ��ֻ����֡������.
		//uint DimensionsCount();
		//���ص�һάͼ���֡��
		uint FrameCount();
		//ͼ������ظ�ʽ����Ӧ���� Gdiplus::ImagePixelFormat ����
		uint PixelFormat();
		//ͼ��֡��Ϣ, ����ֵ��֡��, ��Ϣ����tms����.
		uint GetPropertyItem(Memory<ImageItem>& tms);
		//����������ص�ǰ֡ͼ���һ�� HBITMAP λͼ��Դ,�û������Լ������ͷ�.
		HBITMAP GetBitmap();
		/*//��ȡͼ��ǰָ֡��λ�õ�������ɫ���������������������ʹ��LockPixelData����
		int GetPixel(int x,int y);
		//����ͼ��ǰָ֡��λ�õ�ͼ����ɫ���������������������ʹ��LockPixelData����
		bool SetPixel(int x,int y,int color);
		//��ȡ�������ݣ������ݽ��в����󣬵���UnlockPixelData���������ݻᱻˢ�µ�ͼ���С�
		//rect = 0, ��ʾ��ȡ����ͼ������ݣ�pixelFormat ָ����Ҫ�����ظ�ʽ��0 ��ʾ32λ ARGB ��ʽ��
		BitmapData* LockPixelData(LPRECT rect = 0,int pixelFormat = 0);
		//����LockPixelData�󣬱������UnlockPixelData����������������ͷ���Դ��
		void UnlockPixelData(BitmapData* pbd);*/
		//������
		~Image();
	};
};
