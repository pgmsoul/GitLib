#pragma once
namespace cs{
	class CONCISE_API Md5 : public _class{
	protected:
		void* _md5_ctx;
	public:
		Md5();
		~Md5();
		void Reset();
		void Update(const void* data,uint len);
		uint Finish(void* digest16);
		static uint GetDigest(void* digest16,const void* data,uint len);
		static uint GetFileDigest(void* digest16,LPCWSTR fn);
	};
	class CONCISE_API Md5Hmac : public _class{
	protected:
		void* _md5_ctx;
	public:
		Md5Hmac();
		~Md5Hmac();
		void Create(const void* key,uint keylen);
		void Update(const void* data,uint len);
		uint Finish(void* digest16);
		static uint GetDigest(void* digest16,const void* data,uint len,const void* key,uint klen);
	};
	enum SHA_BITS{
		sha_160 = 20,
		sha_224 = 28,
		sha_256 = 32,
		sha_384 = 48,
		sha_512 = 64
	};
	//Reset ���Ǳ�����õģ���Ϊ���캯�����Ѿ�������һ�Ρ�
	class CONCISE_API Sha : public _class{
	protected:
		void*	_sha_ctx;
		uint	_bits;
	public:
		Sha(SHA_BITS bits = sha_160);
		~Sha();
		void Reset();
		void Update(const void* data,uint len);
		uint Finish(void* digest);
		static uint GetDigest(void* digest,const void* data,uint len,SHA_BITS bits = sha_160);
		static uint GetFileDigest(void* digest,LPCWSTR fn,SHA_BITS bits = sha_160);
	};
	class CONCISE_API ShaHmac : public _class{
	protected:
		void*	_sha_ctx;
		uint	_bits;
	public:
		ShaHmac(SHA_BITS bits = sha_160);
		~ShaHmac();
		void Create(const void* key,uint klen);
		void Update(const void* data,uint len);
		uint Finish(void* digest);
		static uint GetDigest(void* digest,const void* data,uint len,const void* key,uint klen,SHA_BITS bits = sha_160);
	};
	/*@class Aes
	@text ���������Aes�ӽ���*/
	//text �������롣Aesʹ�õ�������3�֣�128λ��192λ��256λ����Ӧ��Key���ȷֱ�Ϊ16�ֽڡ�24�ֽڡ�32�ֽڣ�����û������ĳ��Ȳ���ָ�������ͣ�����0���㡣
	enum AES_BITS{
		aes_128 = 16,
		aes_192 = 24,
		aes_256 = 32
	};
	class CONCISE_API Aes : public _class{
	protected:
		void* _aes_ctx;
		char _iv[16];
	public:
		Aes();
		~Aes();
		//����������iv16 ��һ��16�ֽڵ����ݣ���� iv16 Ϊ 0 ��������16�ֽڶ�����Ϊ 0 ��
		void SetIV(const void* iv16 = 0);
		inline char* GetIV(){return _iv;}
		/*@function bool SetKey(const void* userkey,int len,const void* disturb = 0,int len2 = 0)
		@param userkey -- const void*
		@text �ӽ������룬Aes�ǶԳƼ��ܣ����ܽ�����ͬһ�����롣
		@param len -- int
		@text ���볤�ȣ�������ֵΪ 0 ��������ᱻ����һ�� 0 ��β���ַ�����
		@param keytype -- KEY_TYPE
		@text �������ͣ�Aes֧�� 3 �ֳ��ȵ����룺128λ��192λ��256λ�����userkey�ĳ��Ȳ����Ӧ��������ĳ��ȣ����� 0 ��ĩβ���㣬�������ָ���ĳ��ȣ�������ֽڻᱻ������
		@return void
		@text ����û�з���ֵ��*/
		void SetEncKey(const void* userkey,int keylen = 0,AES_BITS keytype = aes_128);
		void SetDecKey(const void* userkey,int keylen = 0,AES_BITS keytype = aes_128);
		/*@function Encrypt(const void* in16,void* out16)
		@text �������ݣ���������16�ֽ�Ϊ��λ�ģ������������涼����С��16�ֽڣ�
		���������ֽ�������ͬ�ġ�
		@param in16 -- const void*
		@text �������ݣ�����С��16�ֽ�
		@param out16 -- void*
		@text ������ݣ�����С��16�ֽ�
		@return void
		@text û�з���ֵ��*/
		void EncryptEcb(const void* in16,void* out16);
		/*@function void Decrypt(const void* in16,void* out16)
		@text �������ݣ���������16�ֽ�Ϊ��λ�ģ������������涼����С��16�ֽڣ�
		���������ֽ�������ͬ�ġ�
		@param in16 -- const void*
		@text �������ݣ�����С��16�ֽ�
		@param out16 -- void*
		@text ������ݣ�����С��16�ֽ�
		@return void
		@text û�з���ֵ��*/
		void DecryptEcb(const void* in16,void* out16);
		//iv16��һ��16�ֽ����ݣ����ܽ���ʱ������ͬ��������ݺ�Դ���ݽ���λ���㣬ÿ�ζ��Ǳ仯�ġ�
		//cbc ģʽ֧��16�ֽ�һ�����ʽ����ʽ������������ iv �����ʼ��Ϊ�ͼ���ʱ��ͬ��
		//iv �����Ϊ 0 ��ʹ��һ��ȫΪ 0 �� 16 �ֽ�������
		void EncryptCbc(void* output,const void* input,int len);
		//cbc ģʽ֧��16�ֽ�һ�����ʽ����ʽ������������ iv �����ʼ��Ϊ�ͼ���ʱ��ͬ��
		//cbc ���ܿ������ѡ2�����ڵ����Ŀ飬����֮�󣬺�һ��������ȷ��ԭ�ġ�
		void DecryptCbc(void* output,const void* input,int len);
		void EncryptCfb(void* output,const void* input,int len,void* iv16,int* ivoffset);
		void DecryptCfb(void* output,const void* input,int len,void* iv16,int* ivoffset);
		/*@function void Encrypt(const void* input,int len,void* output)
		@text ����ָ�����ȵ����ݣ�������������ⳤ�ȣ������������16�ֽ���������
		������ܳ�����17�ֽڣ��������Ҫ32�ֽڵĿռ䡣
		@param input -- const void*
		@text Ҫ���ܵ�����
		@param len -- int
		@text �������ݴ�С
		@param output -- void*
		@text ������ݻ��棬��Ҫ�Ĵ�С����16�ı�����
		@return int
		@text ��������ĳ��ȣ�����һ��16�ı�����*/
		int EncryptEcb(void* output,int buflen,const void* input,int len);
		/*@function Decrypt(const void* input,int len,void* output)
		@text �������ݣ��������ݵ�ʵ�ʳ��ȱ�����16�ı��������ǿ���ָ�� len
		Ϊ���ܵ����ݳ��ȡ���ΪAes�������Ǵ���16�ֽڣ���16�ֽڵ���������β����������
		������ݲ���ģ����ܵ�ʱ����Ҫ�ص��ⲿ�����ݡ����ǣ�Aes�ļ������ݱ�����¼
		���ݳ��ȣ�������һ��16�ֽڣ��������û��Լ���¼ԭʼ���ݵ�ʵ�ʳ��ȡ�
		@text ����17�ֽڵ�ԭʼ���ݣ����ܺ�õ�32�ֽڵļ������ݣ���32�ֽ��ǲ��ܽضϵģ�
		�����޷����ܡ�ֻ������32�ֽ����ݽ��ܺ��32�ֽ����ݣ����ܽص������������ݡ�
		��ʱ������ָ�����볤�� len Ϊ 17������ʵ�ʵĳ�����32�����ܺ�output�Զ�ֻ����
		17�ֽڵ���Ч���ݣ�����output�Ĵ�С������17�ֽڣ��������Խ�硣
		@param input -- const void*
		@text Ҫ���ܵ����ݣ�ʵ����Ч���ȱ�����16����������
		@param len -- int
		@text �������ݴ�С����ֵ������ԭʼ���ݵĳ���
		@param output -- void*
		@text ������ݻ��棬��Ҫ�Ĵ�СΪlen��������ָ���ĳ��ȡ�
		@return int
		@text ��������ĳ��ȣ�������� len ���������ʧ�ܣ����� 0 .*/
		int DecryptEcb(void* output,int buflen,const void* input,int len);
		/*@function int Encrypt(const void* input,int len,Memory<uchar>& outbuf)
		@text �������ݣ����������һ��Memory�������档
		@param input -- cosnt void*
		@text �������ݡ�
		@param len -- int
		@text �������ݵĳ��ȡ�
		@param outbuf -- Memory<uchar>&
		@text ���ڴ洢������ݡ�
		@return int
		@text ����������ݵĳ��ȣ�ʧ�ܷ��� 0 .*/
		//����ֵ�Ǽ������ݵĳ��ȣ�outbuf���Զ����ô�С�����ǲ�һ�������ݳ��ȣ����ܴ�������
		int EncryptEcb(Memory<char>& outbuf,const void* input,int len);
		/*@function int Decrypt(const void* input,int len,Memory<uchar>& outbuf)
		@text �������ݣ��������ݵ�ʵ�ʳ��ȱ�����16�ı��������ǿ���ָ�� len
		Ϊ���ܵ����ݳ��ȡ���ΪAes�������Ǵ���16�ֽڣ���16�ֽڵ���������β����������
		������ݲ���ģ����ܵ�ʱ����Ҫ�ص��ⲿ�����ݡ����ǣ�Aes�ļ������ݱ�����¼
		���ݳ��ȣ�������һ��16�ֽڣ��������û��Լ���¼ԭʼ���ݵ�ʵ�ʳ��ȡ�
		@text ����17�ֽڵ�ԭʼ���ݣ����ܺ�õ�32�ֽڵļ������ݣ���32�ֽ��ǲ��ܽضϵģ�
		�����޷����ܡ�ֻ������32�ֽ����ݽ��ܺ��32�ֽ����ݣ����ܽص������������ݡ�
		��ʱ������ָ�����볤�� len Ϊ 17������ʵ�ʵĳ�����32�����ܺ�output�Զ�ֻ����
		17�ֽڵ���Ч���ݡ�
		@param input -- const void*
		@text Ҫ���ܵ����ݣ�ʵ����Ч���ȱ�����16����������
		@param len -- int
		@text �������ݴ�С����ֵ������ԭʼ���ݵĳ���
		@param output -- void*
		@text ������ݻ��棬��Ҫ�Ĵ�СΪlen��������ָ���ĳ��ȡ�
		@return int
		@text ��������ĳ��ȣ�������� len ���������ʧ�ܣ����� 0 .*/
		int DecryptEcb(Memory<char>& outbuf,const void* input,int len);
	};
	class CONCISE_API Base64{
	public:
		static int Base64::Encode(Memory<char>& output,const void* src,int len);
		//Դ���ݵĳ��� len ������ -1�������� src ���ַ����ȡ�
		static int Base64::Decode(Memory<char>& output,const char* src,int len);
		static int Base64::Encode(char *dst, int *dlen,const void* src, int slen);
		//Դ���ݵĳ��� len ������ -1�������� src ���ַ����ȡ�
		static int Base64::Decode(void *dst, int *dlen,const char *src, int slen);
	};
	//void sha512(const void* key,int keylen,const void* data,int datalen,void* buf64);
}