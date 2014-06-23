/* �����XML�ļ������࣬������3��Ҫ�أ��ڵ����ƣ��ڵ����Լ����ڵ��ӽڵ㼯��
�ڵ����Լ���Ԫ����һϵ�еĽڵ����Զ��󣬽ڵ�������һ�����������ƺ�����ֵ��
���ִ���Ա�Ľṹ���ӽڵ㼯��Ԫ����һϵ�е�Xml���󡣽ڵ����ƣ��ڵ���������
���ܺ��ַ����ո�TAB����\t�������س���\r�������С�\n������"������=������<����
��>�����ڵ�����ֵ���ܺ���"���ַ�*/

#pragma once
namespace concise{
	class CONCISE_API  Xml : public TPTree<Xml>{
	protected:
		typedef struct _prop:public _struct{String Name;String Value;}prop;
		String name;
		String value;
		ObjectList<prop>	atrb;

		uint _FromString(LPCWSTR str,uint len);
		void _ToString(String& ns,int Indent = 0);
	public:

		//���� Xml �ڵ�����ơ�
		LPCWSTR GetName(){return name;}
		//���� Xml �ڵ�����ƣ����Ʋ��ܺ����ո�TAB����\t�������س���\r�������С�\n������"������=������<������>����
		bool SetName(LPCWSTR name);
		//���� Xml �ڵ��ֵ��
		LPCWSTR GetValue(){return value;}
		//���� Xml �ڵ��ֵ��ֵ���ܺ�����<������>��������Իس����п�ͷ��ᱻ�Ƴ�����ͷ�Ŀո��tab�ַ�Ҳ�ᱻ�Ƴ���
		bool SetValue(LPCWSTR val);
		//���ýڵ�ֵΪ����
		void SetValueInt(int v,int radix = 16);
		//��ȡ�ڵ������ֵ�������ֵ������10���������ַ�������false������v��Ȼ���ܱ���ֵ�ܹ�ʶ��Ĳ��֡�
		bool GetValueInt(int& v,int radix = 16);
		//���ýڵ�ֵΪdoubleֵ��fwidthָ��С����λ���������������롣
		void SetValueDouble(double v,int fwidth = -1);
		//��ȡ�ڵ��doubleֵ�������ֵ����ת��Ϊdouble������ 0 ������v��Ȼ����ֵ��ʶ��Ĳ��֡�
		bool GetValueDouble(double& v);
		//����ָ����ֵΪ���ݵ�16�����ı���ʾ��
		void SetValueHex(const void* data,int len);
		//��ȡ�ڵ�Ķ�����ֵ�������ֵ����ת��Ϊ��ֵ������ 0 ,����dataΪNULL��������Ҫ���ڴ泤�ȡ�
		int GetValueHex(void* data);

		//���ؽڵ������б���ָ����������������
		LPCWSTR GetPropertyName(int index);
		//���ؽڵ������б���ָ������������ֵ��������Բ����ڣ����� 0.��Ϊ�κ�ֵ�����ִ���ʾ�ģ����������������ȡ���κ����͵�ֵ�����������ִ�����
		LPCWSTR GetProperty(int index);
		//���ؽڵ�ָ���������Ե�ֵ�����Ʋ��ܺ����ո�TAB����\t�������س���\r��������
		//��\n������"������=������<������>�������ָ���������Բ����ڣ����� 0 ��
		LPCWSTR GetProperty(LPCWSTR pName);
		//���ýڵ����ԣ���������Բ����ڣ��Զ��������
		bool SetProperty(LPCWSTR pName,LPCWSTR pValue);
		//����ָ���ڵ����Լ�Ԫ�ظ�����
		uint GetPropertyCount(){return atrb.Count();}
		//���ýڵ�����ֵΪ����
		bool SetPropertyInt(LPCWSTR name,int v,int radix = 16);
		//��ȡ�ڵ����Ե�����ֵ������ָ�����Ƶ����Բ����ڣ����� 0 �������ֵ����ת��Ϊ������������ת�����ִ��Ľ����
		bool GetPropertyInt(LPCWSTR name,int& v,int radix = 16);
		//���ýڵ�����ֵΪdoubleֵ��fwidthָ��С����λ���������������롣
		bool SetPropertyDouble(LPCWSTR name,double v,int fwidth = -1);
		//��ȡ�ڵ����Ե�doubleֵ�������ֵ����ת��Ϊdouble������ָ�����Ƶ����Բ����ڣ����� 0 ��
		bool GetPropertyDouble(LPCWSTR name,double& v);
		//����ָ�����Ե�ֵΪ���ݵ�16�����ı���ʾ��
		bool SetPropertyHex(LPCWSTR name,const void* data,int len);
		//��ȡ�ڵ����ԵĶ�����ֵ�������ֵ����ת��Ϊ��ֵ������ָ�����Ƶ����Բ����ڣ����� 0 ��
		//����dataΪNULL��������Ҫ���ڴ泤�ȡ�
		int GetPropertyHex(LPCWSTR name,void* data);
		//ɾ����������
		void ClearProperty();
		//ɾ��ָ�����Ƶ�����
		bool DelProperty(LPCWSTR name);

		//����ָ�����Ƶ��ӽڵ㣬���always=1�����ӽڵ㲻�����ǣ������������򷵻� 0 ��
		Xml* GetNode(LPCWSTR name,bool always = 0);
		//���һ��ָ�����Ƶ��½ڵ㵽�ӽڵ��б�
		Xml* AddNode(LPCWSTR name,int index = -1);
		//���һ��ָ�����ƺ�ֵ���½ڵ㵽�ӽڵ��б�
		Xml* AddNode(LPCWSTR name,LPCWSTR val,int index = -1);
		//ɾ��ָ�����Ƶ��ӽڵ㡣
		bool DelNode(LPCWSTR name,int start = 0);

		//�����ض������ӽڵ��ֵ���� openalways = 1 ʱ�����������ָ�����Ƶ��ӽڵ��������������򷵻� 0 ��
		//ns��һ��you�ָ���������ע��������Ƶ��ִ��������׶β��������ڵ�����ƣ���Ϊһ��TreeLink����ֻ��һ�����ڵ㣩��
		//Name��Value���Կ��Ժ������ַ������ǣ����Name�ﺬ���ַ�"/"�tns����������ָ�������Բ�Ҫʹ�ú����ַ���/����Name��
		Xml* OpenMultiLevelNode(LPCWSTR ns,bool openalways = 0);
		//ɾ��ָ�����Ƶģ��༶���ӽڵ㡣
		bool DeleteMultiLevelNode(LPCWSTR ns);

		//�˺�������0���ܱ�ʾ�Ѿ���ȷ�����˲����ִ�
		bool FromString(LPCWSTR str);
		//��xmlת��Ϊ�ִ������Զ�����س����з���2���ո��������
		void ToString(String& ns);
		//��xml����Ϊ�ļ������Զ�����س����з���2���ո��������
		bool Save(LPCWSTR file,LPCWSTR cp = L"UTF-8");
		//����һ���ص�����xml�ļ���ʹ��CodePage���ȡָ�����Ƶı���
		bool Load(LPCWSTR file,DWORD cp = CP_UTF8);
		//����Դ���أ�ʹ��CodePage���ȡָ�����Ƶı��롣
		bool Load(ResID id,DWORD cp = CP_UTF8);
	};
	//RegisterTree���ڰ���Ϣ���ı��ķ�ʽ����Ϊxml�ļ�.
	class CONCISE_API RegisterTree : public _class{
	protected:
		void* _innerobj; //it's a register_struct pointer.
	public:
		//��ʱ����ʵ�����ڲ��� Xml ����
		RegisterTree();
		//ʵ����һ�� Xml ���󣬰�file�ļ����������������������RegisterTree�����file��ʹ�á�
		RegisterTree(LPCWSTR file);
		//��ָ���ļ���д, �ڲ��ᴴ��һ������, ���Ҷ�ȡfile�ļ�����������ʼ���������.
		bool Create(LPCWSTR file);
		//д����ǰ, Ӧ�õ���Lock, ��ռ�ڲ�����, ������Lock���ᱻ����, ֱ������Unlock����.
		bool Lock();
		//���Lock, ��ΪΪ�˱����ļ����ݵ�һ��, ͬһʱ��ֻ����һ���򿪲���, Unlock���Զ�����Flush��
		void Unlock();
		//�����ͷ�Create�����򿪵Ķ���, ���ⲿ����ı����ļ�������ʱ.
		//��ҪClose��, ������Create��, ���ݲŻ�ˢ��, ��Ϊ�ڲ�����һ�����ɾͲ����ٴ��ļ���ȡ����.
		void Close();
		//д���ļ�, UnlockҲ��ִ���������, ���Գ����ڻ�û����Unlock����������Ҫ����Ϣд���ļ�ʱ, ����Ҫ�������������
		void Flush();
		//�����ڲ���Xml����, ���ж�д����.
		Xml* GetXml();
		//����
		~RegisterTree();
	};
	//RegisterTree���ڰ���Ϣ���ı��ķ�ʽ����Ϊxml�ļ�.
}