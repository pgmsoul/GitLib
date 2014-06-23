#pragma once
#include <Iphlpapi.h>
#include <WinInet.h>
namespace cs{
	class HttpRequest : public _class{
	protected:
		HINTERNET hInternet,hConnect,hRequest;
		DWORD _connectFlag;
	public:
		static LPCWSTR ContentTypeUrlEncoded,//Ҫ����$_POST�Ƚ��յ����ݣ���Ҫ���ͷ
			ContentTypeTextXml;//�ϴ�����ʹ�ö����Ʒ�ʽ���գ�$GLOBALS['HTTP_RAW_POST_DATA']�����ô�ͷ
		DWORD	Flags;
		WORD	Port;
		String Agent,
			Host,
			Page,	//
			Method;	//"POST" OR "GET"
		//���������ı�˿�Ϊhttp��https��Ĭ�϶˿ڣ����ʹ�������˿ڣ���Ҫ�������� Port ��ֵ��
		void SetHttpsMode(bool https);
		bool GetHttpsMode();
		void SetPostMode();
		void SetGetMode();
		bool Create();
		//�����һ������û����ɣ����������ʧ�ܣ�ǿ�ƽ������� Close ������ֻ�� Request �������غ󣬲ſ����������ӣ���������
		bool Connect();
		//���ͷ���µģ�����һ����Ӷ��ͷ���ûس��������ָ������Ҫ�ı�ͷ��һ��ֻ������һ����
		bool SetHeader(LPCWSTR header);
		//�������󣬲��ҷ������ص����ݣ����ʧ�ܷ���ֵ�� -1����Ϊ��ҳ�����������ݳ���Ϊ 0 ���������ʱ������û��ʧ��.
		//��Ҫ������������ݣ���Ϊ������һ��ȡ�صģ���������С�������ơ�
		int Request(const void* data,int len,Memory<char>& html,String* retHead = 0);
		//����һ�����������ӣ��Զ���ȡhttp��ȡhttpsģʽ���Զ���ȡhost���Զ���ȡҳ�棬url������һ����http��https��ͷ�����������ӵ�ַ��
		bool ParseUrl(LPCWSTR url);
		void Close();
		HttpRequest();
		~HttpRequest();
	};
}