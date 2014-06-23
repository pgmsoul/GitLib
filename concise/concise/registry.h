#pragma once
namespace cs{
	class CONCISE_API Registry : public HandleType<HKEY>{
	public:
		enum REGRESULT{
			REG_ERROR		=	0,	//����ʧ��
			REG_CREATE		=	1,	//�������µļ�
			REG_OPEN		=	2	//����ԭ�еļ�
		};
		Registry(void);
		~Registry(void);
		//��һ���µ�ע�������,ԭ���ļ��ᱻ�ر�(����Ѿ���һ��ע�����Ļ�).
		bool Attach(HKEY handle);
		//ȡ����.
		HKEY Detach();
		//���ɻ��(ָ���ļ��Ѿ�����)һ��ע����,����ֵ : REG_ERROR����ʧ��;REG_CREATE��������һ���µļ�;REG_OPEN������һ���Ѿ����ڵļ�.
		//subKey��Ҫ�򿪵��Ӽ����ƣ�key���Ѿ��򿪵ļ����߸���create������������ڣ��Ƿ���������access�������Ժ���Ȩ�޴򿪣�option����ѡ��
		REGRESULT Create(LPCWSTR subKey,HKEY key = HKEY_CURRENT_USER,bool create = 0,REGSAM access = KEY_ALL_ACCESS,DWORD option = REG_OPTION_NON_VOLATILE);
		//��Create��Ψһ������, ����������һ��ͨ��б�ܻ�б�ָܷ��ı�ʾ�༶�Ӽ����ִ�, ע�������ǿ��Ժ�б��(��б��)��, ������������޷��򿪺�б��(��б��)�ļ���
		REGRESULT MultiCreate(LPCWSTR multikey,HKEY key = 0,bool create = 0,REGSAM access = KEY_ALL_ACCESS,DWORD option = REG_OPTION_NON_VOLATILE);
		//�򿪵�ǰ����һ���Ӽ�,���Ұ���.
		REGRESULT ToSubKey(LPCWSTR subKey,bool create = 0,REGSAM access = KEY_ALL_ACCESS,DWORD option = REG_OPTION_NON_VOLATILE);
		//�˺�����ToSubKey�������Ǽ�����������б��(��б��)�ָ��ı�ʾ�༶�Ӽ��ִ�.
		REGRESULT ToMultiSubKey(LPCWSTR multiKey,bool create = 0,REGSAM access = KEY_ALL_ACCESS,DWORD option = REG_OPTION_NON_VOLATILE);
		//�ر�ʵ���󶨵�ע����.
		bool Close();
		//ȡ���������,������ʼ��data�Ŀռ��С,���֪�����ݵĳ���.�����֪�����ݵĳ���,Ҳ���Բ���ʼ��data�Ĵ�С.type�������ݵ�����.
		bool GetValue(LPCWSTR valueName,Memory<BYTE>* data,OUT LPDWORD type = NULL);
		//�����������.
		bool SetValue(LPCWSTR valueName,BYTE* data,uint len,IN DWORD type);
		//����Intֵ.
		bool SetIntValue(LPCWSTR valueName,DWORD value);
		//�����������ֵ.
		bool GetIntValue(LPCWSTR valueName,DWORD& val);
		//�����ַ���ֵ.
		bool SetStrValue(LPCWSTR valueName,LPCWSTR wstr);
		//ȡ��һ���ַ���ֵ.
		bool GetStrValue(LPCWSTR valueName,String& wstrValue);
		//ö���Ӽ�������.
		int EnumSubKey(StringMemList* subkeys);
		//ö�ټ����������.
		int EnumValueName(StringMemList* values);
		//ö�ٵ�ǰ�����ַ�������������ƺ�ֵ.
		int EnumStrValue(StringMemList* names,StringMemList* values);
		//ö�ٵ�ǰ������������������ƺ�ֵ.
		int EnumIntValue(StringMemList* names,IntegerList<DWORD>* values);
		//�ѻ���ˢ�µ��ļ�.
		LONG Flush();
		//ɾ��ָ�����Ƶ��Ӽ��Լ���֮�µ��������ݡ�
		bool DelSubKey(LPCWSTR sub);
		//ɾ�����µ�ֵ��
		bool DelValue(LPCWSTR name);
	};
	/*������ע����ȡϵͳ�� CodePage ��Ϣ*/
	class CONCISE_API CodePage : public _class{
	protected:
		Registry reg;
	public:
		bool Create();
		//����ע�����ȫ���� CodePage ���ƣ���Щ������Щ�Ǳ�ƣ���Ӧ�� CodePage ֵ�����ظ���
		int GetAllCodePage(StringMemList* sml);
		//����ָ�����Ƶ����ֵ��0 ��ʾû��ָ�����ƵĴ���ҳ�����Ʋ����ִ�Сд��
		DWORD GetCode(LPCWSTR cpName);
		bool Close();
	};
	//������ȡ�ô���ҳ, ��������ڲ�ά��һ���б�, �������û�е����ƻ�ȥ��ѯע���,
	//����Ѿ���ѯ�ɹ��Ĵ���ҳ����, ��洢���б���, �´�ֱ�ӷ���, �մ����� CP_THREAD_ACP(3), ���������ʹ�� CodePage Ҫ��, �������˼��������Ĵ���ҳ��Ϣ, CodePage �� Create �������ע����ȡȫ���Ĵ���Ҳ��Ϣ.
	DWORD CONCISE_API GetCodePage(LPCWSTR name);
}