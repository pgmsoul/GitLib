// jscrypto.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "jscrypto.h"

extern "C"{
	//返回数据长度。
	__declspec(dllexport) int __stdcall hexdecode(cs::Memory<char>* dst,LPCWSTR src,int srclen){
		cs::String str;
		str.CopyFrom(src,srclen);
		int len = str.ToBinary(*dst);
		return len;
	}
	//返回包括 null 结尾的字符串长度。
	__declspec(dllexport) int __stdcall hexencode(cs::Memory<char>* dst,const void* src,int srclen){
		cs::String str;
		str.HexBinary(src,srclen);
		uint buflen = str.Length()*2+2;
		if(buflen>dst->Length())
			dst->SetLength(buflen);
		memcpy(dst->Handle(),str.Handle(),buflen);
		return buflen;
	}
	//返回解码数据长度
	__declspec(dllexport) int __stdcall base64decode(cs::Memory<char>* dst,LPCSTR src,int srclen){
		int len = cs::Base64::Decode(*dst,src,srclen);
		return len;
	}
	//返回编码字串长度。
	__declspec(dllexport) int __stdcall base64encode(cs::Memory<char>* dst,const void* src,int srclen){
		int len = cs::Base64::Encode(*dst,src,srclen);
		return len;
	}
	
	__declspec(dllexport) int __stdcall md5_reset(cs::Md5* md5){
		if(md5) md5->Reset();
		return 0;
	}
	__declspec(dllexport) int __stdcall md5_file2(void* digest,LPCWSTR file){
		if(digest==NULL||file==NULL) return 0;
		return cs::Md5::GetFileDigest(digest,file);
	}
	__declspec(dllexport) int __stdcall md5_direct(void* digest,const void* data,int len){
		if(digest==NULL||data==NULL||len<=0) return 0;
		return cs::Md5::GetDigest(digest,data,len);
	}
	__declspec(dllexport) int __stdcall md5_finish2(cs::Md5* md5,void* digest){
		if(md5==NULL||digest==NULL) return 0;
		return md5->Finish(digest);
	}
	__declspec(dllexport) int __stdcall md5_update2(cs::Md5* md5,const void* data,int len){
		if(md5==NULL||data==NULL||len<=0) return 0;
		md5->Update(data,len);
		return 0;
	}
	__declspec(dllexport) cs::Md5* __stdcall md5_create(){
		return new cs::Md5();
	}
	__declspec(dllexport) int __stdcall md5_close(cs::Md5* md5){
		if(md5) delete md5;
		return 0;
	}
	//md5_hmac
	__declspec(dllexport) int __stdcall md5hmac_reset(cs::Md5Hmac* md5,const void* key,int keylen){
		if(md5&&key&&keylen>0) md5->Create(key,keylen);
		return 0;
	}
	__declspec(dllexport) int __stdcall md5hmac_direct(void* digest,const void* data,int len,const void* key,int keylen){
		if(digest==NULL||data==NULL||len<=0) return 0;
		return cs::Md5Hmac::GetDigest(digest,data,len,key,keylen);
	}
	__declspec(dllexport) int __stdcall md5hmac_finish(cs::Md5Hmac* md5,void* digest){
		if(md5==NULL||digest==NULL) return 0;
		return md5->Finish(digest);
	}
	__declspec(dllexport) int __stdcall md5hmac_update(cs::Md5Hmac* md5,const void* data,int len){
		if(md5==NULL||data==NULL||len<=0) return 0;
		md5->Update(data,len);
		return 0;
	}
	__declspec(dllexport) cs::Md5Hmac* __stdcall md5hmac_create(){
		return new cs::Md5Hmac();
	}
	__declspec(dllexport) int __stdcall md5hmac_close(cs::Md5Hmac* md5){
		if(md5) delete md5;
		return 0;
	}

	//sha 返回摘要数据的长度,最大不会超过 64。
	__declspec(dllexport) int __stdcall sha_reset(cs::Sha* sha){
		if(sha) sha->Reset();
		return 0;
	}
	__declspec(dllexport) int __stdcall sha_file(void* digest,LPCWSTR file,int bits){
		if(bits!=20&&bits!=28&&bits!=32&&bits!=48&&bits!=64) return 0;
		if(digest==NULL||file==NULL) return 0;
		return cs::Sha::GetFileDigest(digest,file,(cs::SHA_BITS)bits);
	}
	__declspec(dllexport) int __stdcall sha_direct(void* digest,const void* data,int len,int bits){
		if(bits!=20&&bits!=28&&bits!=32&&bits!=48&&bits!=64) return 0;
		if(digest==NULL||data==NULL||len<=0) return 0;
		return cs::Sha::GetDigest(digest,data,len,(cs::SHA_BITS)bits);
	}
	__declspec(dllexport) int __stdcall sha_finish(cs::Sha* sha,void* digest){
		if(sha==NULL||digest==NULL) return 0;
		return sha->Finish(digest);
	}
	__declspec(dllexport) int __stdcall sha_update(cs::Sha* sha,const void* data,int len){
		if(sha==NULL||data==NULL||len<=0) return 0;
		sha->Update(data,len);
		return 0;
	}
	__declspec(dllexport) cs::Sha* __stdcall sha_create(int bits){
		if(bits!=20&&bits!=28&&bits!=32&&bits!=48&&bits!=64) return 0;
		return new cs::Sha((cs::SHA_BITS)bits);
	}
	__declspec(dllexport) int __stdcall sha_close(cs::Sha* sha){
		if(sha) delete sha;
		return 0;
	}
	//sha_hmac
	__declspec(dllexport) int __stdcall shahmac_reset(cs::ShaHmac* sha,const void* data,int len){
		if(sha&&data&&len>0) sha->Create(data,len);
		return 0;
	}
	__declspec(dllexport) int __stdcall shahmac_file(void* digest,LPCWSTR file,int bits){
		if(bits!=20&&bits!=28&&bits!=32&&bits!=48&&bits!=64) return 0;
		if(digest==NULL||file==NULL) return 0;
		return cs::Sha::GetFileDigest(digest,file,(cs::SHA_BITS)bits);
	}
	__declspec(dllexport) int __stdcall shahmac_direct(void* digest,const void* data,int len,const void* key,int klen,int bits){
		if(bits!=20&&bits!=28&&bits!=32&&bits!=48&&bits!=64) return 0;
		if(digest==NULL||data==NULL||len<=0) return 0;
		return cs::ShaHmac::GetDigest(digest,data,len,key,klen,(cs::SHA_BITS)bits);
	}
	__declspec(dllexport) int __stdcall shahmac_finish(cs::ShaHmac* sha,void* digest){
		if(sha==NULL||digest==NULL) return 0;
		return sha->Finish(digest);
	}
	__declspec(dllexport) int __stdcall shahmac_update(cs::ShaHmac* sha,const void* data,int len){
		if(sha==NULL||data==NULL||len<=0) return 0;
		sha->Update(data,len);
		return 0;
	}
	__declspec(dllexport) cs::ShaHmac* __stdcall shahmac_create(int bits){
		if(bits!=20&&bits!=28&&bits!=32&&bits!=48&&bits!=64) return 0;
		return new cs::ShaHmac((cs::SHA_BITS)bits);
	}
	__declspec(dllexport) int __stdcall shahmac_close(cs::ShaHmac* sha){
		if(sha) delete sha;
		return 0;
	}
	//aes
	__declspec(dllexport) cs::Aes* __stdcall aes_create(){
		return new cs::Aes();
	}
	__declspec(dllexport) int __stdcall aes_close(cs::Aes* aes){
		if(aes) delete aes;
		return 0;
	}
	__declspec(dllexport) int __stdcall aes_set_enc_key(cs::Aes* aes,const void* key,int keylen,cs::AES_BITS keytype){
		if(aes==NULL||key==NULL||keylen==0) return 0;
		if(keytype!=cs::aes_128&&keytype!=cs::aes_192&&keytype!=cs::aes_256) return 0;
		aes->SetEncKey(key,keylen,keytype);
		return 1;
	}
	__declspec(dllexport) int __stdcall aes_set_dec_key(cs::Aes* aes,const void* key,int keylen,cs::AES_BITS keytype){
		if(aes==NULL||key==NULL||keylen==0) return 0;
		if(keytype!=cs::aes_128&&keytype!=cs::aes_192&&keytype!=cs::aes_256) return 0;
		aes->SetDecKey(key,keylen,keytype);
		return 1;
	}
	__declspec(dllexport) int __stdcall aes_set_iv(cs::Aes* aes,const void* iv){
		if(aes==NULL||iv==NULL) return 0;
		aes->SetIV(iv);
		return 1;
	}
	//函数支持流式加密，可以以 16 字节一组，任意组进行连续调用。如果要使用不同的向量，设置 setiv函数。
	//返回加密数据的长度，它总是16的倍数。
	__declspec(dllexport) int __stdcall aes_enc_cbc(cs::Aes* aes,void* output,int outlen,const void* data,int len){
		if(aes==NULL||output==NULL||outlen<=0||data==NULL||len<=0) return 0;
		int reslen = len%16;
		int reqlen;
		if(reslen) reqlen = len - reslen + 16;
		else reqlen = len;
		if(outlen<reqlen) return 0;
		aes->EncryptCbc(output,data,len);
		return reqlen;
	}
	//源数据必须传 16 的倍数，否则函数失败，这个函数支持流式解密，但是需要多传前面一组数据，比如要解密得到第 9,10 两组原文，需要 8,9,10 三组密文。
	//调用这个解密函数之前，必须设置iv。返回解密数据的长度，但是它总是 16 的倍数，也就是可能尾部有补足数据。
	__declspec(dllexport) int __stdcall aes_dec_cbc(cs::Aes* aes,void* output,int outlen,const void* data,int len){
		if(aes==NULL||output==NULL||outlen<=0||data==NULL||len<=0) return 0;
		int reslen = len%16;
		int reqlen;
		if(reslen) reqlen = len - reslen + 16;
		else reqlen = len;
		if(outlen<reqlen) return 0;
		aes->DecryptCbc(output,data,len);
		return reqlen;
	}
	__declspec(dllexport) int __stdcall aes_enc_ecb(cs::Aes* aes,void* output,int outlen,const void* data,int len){
		if(aes==NULL||output==NULL||outlen<=0||data==NULL||len<=0) return 0;
		int res = len%16;
		int req;
		if(res) req = len - res + 16;
		else req = len;
		if(outlen<req) return 0;
		aes->EncryptEcb(output,outlen,data,len);
		return len;
	}
	__declspec(dllexport) int __stdcall aes_dec_ecb(cs::Aes* aes,void* output,int outlen,const void* data,int len){
		if(aes==NULL||output==NULL||outlen<=0||data==NULL||len<=0) return 0;
		if((len%16)||outlen<len) return 0;
		aes->DecryptEcb(output,outlen,data,len);
		return len;
	}
	__declspec(dllexport) LPSTR __stdcall aes_get_iv(cs::Aes* aes){
		if(aes==NULL) return 0;
		return aes->GetIV();
	}
	union _IEcc{
		cs::Ecdsa* dsa;
		cs::Ecies* ies;
	};
	class _Ecc : public cs::_class{
	public:
		_IEcc handle;
		uint	isIes;
		_Ecc(BOOL isIes){
			this->isIes = isIes;
			if(isIes) handle.ies = new cs::Ecies;
			else handle.dsa = new cs::Ecdsa;
		}
		~_Ecc(){
			if(isIes){
				delete handle.ies;
			}else{
				delete handle.dsa;
			}
		}
	};
	__declspec(dllexport) _Ecc* __stdcall ecc_create(BOOL isIes){
		return new _Ecc(isIes);
	}
	__declspec(dllexport) BOOL __stdcall ecc_close(_Ecc* ecc){
		if(ecc==NULL) return 0;
		delete ecc;
		return 1;
	}
	__declspec(dllexport) BOOL __stdcall ecc_createkey(_Ecc* ecc){
		if(ecc==NULL) return 0;
		if(ecc->isIes){
			return ecc->handle.ies->CreateKey();
		}else{
			return ecc->handle.dsa->CreateKey();
		}
	}
	__declspec(dllexport) BOOL __stdcall ecc_makepublickey(_Ecc* ecc){
		if(ecc==NULL) return 0;
		if(ecc->isIes){
			return ecc->handle.ies->MakePublicKey();
		}else{
			return ecc->handle.dsa->MakePublicKey();
		}
	}
	//pubkey 需要 2*ECC_KEY_LEN（68） 字节的大小
	__declspec(dllexport) uint __stdcall ecc_getpublickey(_Ecc* ecc,void * pubkey){
		if(ecc==NULL) return 0;
		if(ecc->isIes){
			return ecc->handle.ies->GetPublicKey(pubkey);
		}else{
			return ecc->handle.dsa->GetPublicKey(pubkey);
		}
	}
	//prikey 需要 ECC_KEY_LEN（68） 字节的大小
	__declspec(dllexport) uint __stdcall ecc_getprivatekey(_Ecc* ecc,void* prikey){
		if(ecc==NULL) return 0;
		if(ecc->isIes){
			return ecc->handle.ies->GetPrivateKey(prikey);
		}else{
			return ecc->handle.dsa->GetPrivateKey(prikey);
		}
	}
	__declspec(dllexport) BOOL __stdcall ecc_setpublickey(_Ecc* ecc,const void* pubkey,uint len){
		if(ecc==NULL) return 0;
		if(ecc->isIes){
			return ecc->handle.ies->SetPublicKey(pubkey,len);
		}else{
			return ecc->handle.dsa->SetPublicKey(pubkey,len);
		}
	}
	__declspec(dllexport) BOOL __stdcall ecc_setprivatekey(_Ecc* ecc,const void* prikey,uint len){
		if(ecc==NULL) return 0;
		if(ecc->isIes){
			return ecc->handle.ies->SetPrivateKey(prikey,len);
		}else{
			return ecc->handle.dsa->SetPrivateKey(prikey,len);
		}
	}
	__declspec(dllexport) uint __stdcall ecc_signlen(){
		return cs::Ecdsa::SignLen();
	}
	__declspec(dllexport) uint __stdcall ecc_keylen(){
		return cs::Ecdsa::KeyLen();
	}
	__declspec(dllexport) BOOL __stdcall ecc_getsign(_Ecc* ecc,void* sign,void* data,uint len){
		if(ecc==NULL) return 0;
		if(ecc->isIes){
			return 0;
		}else{
			return ecc->handle.dsa->GetSign(sign,data,len);
		}
	}
	__declspec(dllexport) BOOL __stdcall ecc_verify(_Ecc* ecc,const void* data,uint len,const void* sign,uint signlen){
		if(ecc==NULL) return 0;
		if(ecc->isIes){
			return 0;
		}else{
			return ecc->handle.dsa->Verify(data,len,sign,signlen);
		}
	}
	__declspec(dllexport) uint __stdcall ecc_encrypt(_Ecc* ecc,cs::Memory<char>* encdata,const void* data,uint len){
		if(ecc==NULL) return 0;
		if(ecc->isIes){
			return ecc->handle.ies->Encrypt(*encdata,data,len);
		}else{
			return 0;
		}
	}
	__declspec(dllexport) uint __stdcall ecc_decrypt(_Ecc* ecc,cs::Memory<char>* decdata,const void* data,uint len){
		if(ecc==NULL) return 0;
		if(ecc->isIes){
			return ecc->handle.ies->Decrypt(*decdata,data,len);
		}else{
			return 0;
		}
	}
};