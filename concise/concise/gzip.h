#pragma once
namespace cs{
	class CONCISE_API Ungzip : public _class{
	protected:
		void* strm;//z_stream 
	public:
		Ungzip();
		~Ungzip();
		inline bool IsNull(){return strm==NULL;}
		inline bool NotNull(){return strm!=NULL;}
		//��ʼ����������������ظ����ã����Output��������-1����ʾ���ִ��󣬴�ʱ�˶����ѷ�������Ҫ���µ��ô˺�����
		void Create();
		//�Ѵ˶�������Ϊ NULL ��
		void Close();
		//����Ҫ��ѹ�����ݡ��ٴε��ô˴˺�����Ҫ����һ��Output������
		bool Input(char* data,int len);
		//�����ѹ���ݣ����� 0 ��������ɣ�
		//���� 1 ���������ݣ������ٴε��ô˺���������ݣ��������ɴ���
		//���� -1 �����ִ����ڷ��� -1 �Ժ����Ҫ�ٴ�ʹ�����ʵ�����������µ��� Create() ������
		int Output(char* buf,int& len);
		//�����ѹ���ݣ�������������ݳ��ȡ����ִ��󷵻�ֵΪ 0 ��
		int Output(Memory<char>& buf);
	};
	//Gzip����һ��ѹ���������Բ��ϵĵ���Input�������ݣ�Output������ݡ�
	class CONCISE_API Gzip : public _class{
	protected:
		void* strm;//z_stream 
	public:
		Gzip();
		~Gzip();
		inline bool IsNull(){return strm==NULL;}
		inline bool NotNull(){return strm!=NULL;}
		//��ʼ����level��ѹ���ʵļ���������������ظ����ã����Output��������-1����ʾ���ִ��󣬴�ʱ�˶����ѷ�������Ҫ���µ��ô˺�����
		void Create(int level = 6);
		//�Ѵ˶�������Ϊ NULL ��
		void Close();
		//����Ҫѹ�������ݣ�buf��lenΪ0 ����false���˺��������������ã��������һ��Output�������ܼ����������ݡ�
		bool Input(char* buf,int len);
		//���ѹ�����ݣ����� 0 �����������Լ����������ݡ�
		//���� 1 ����������û�д�����ɣ���Ҫ�������ô˺������ѹ�����ݣ���������ٴε�����Input��������Щδ��������ݻᶪʧ��
		//���� -1 �����ִ������Ҫ�ٴ�ʹ�����ʵ�����������µ��� Create() ������
		int Output(char* buf,int &len);
		//���ȫ�����ݣ��������ݳ��ȡ�
		int Output(Memory<char>& buf);
	};
	//����ѹ�����ݿ�����Ҫ����󻺳�������
	CONCISE_API uint GetMaxCompressLen(uint len);
	//ѹ�����ݣ�����ѹ�����ݵĳ��ȣ�data��Ҫѹ��������Դ��length��Դ���ݵĳ��ȣ�zData��ѹ�����ݴ�ŵĻ�������bufLen���������ĳ��ȡ�
	CONCISE_API uint Compress(const char* data,uint length,char* zData,uint bufLen);
	//��ѹ���ݣ����ؽ�ѹ���ݵĳ��ȣ�zData��Ҫ��ѹ����Դ���ݣ�length��Դ���ݵĳ��ȣ�data����ѹ���ݵĴ�Ż�������bufLen���������Ĵ�С��
	CONCISE_API uint DeCompress(const char* zData,uint length,char* data,uint bufLen);
	//ѹ�����ݣ�����ѹ�����ݵĳ��ȣ�data��Ҫѹ��������Դ��zData�����ѹ�����ݵĻ��壬�������ô�С���������Զ�������
	CONCISE_API uint Compress(const char* data,uint len,Memory<char>* zData);
	//��ѹ���ݣ����ؽ�ѹ���ݵĳ��ȣ�zData��Ҫ��ѹ������Դ��data����Ž�ѹ���ݵĻ��������������ô�С���������Զ�������
	CONCISE_API uint DeCompress(const char* zData,uint len,Memory<char>* data);
}