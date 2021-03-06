//file: crypt.jsuser

"use strict";

//jscrypt.dll 必须在脚本所在的目录，否则应该使用绝对路径或者相对路径指出 jscrypt.dll 位置。
var jscrypt;
if(!jscrypt) jscrypt = Api.loadModule("../bin/jscrypt.dll");
if(!jscrypt) throw "load jscrypto.dll error";
//如果要处理大数据的哈希值，需要使用 CMd5、CSha 等的实例对象方式，多次调用 update 函数来实现，当前文档没有包装这些函数。
function initApi(name){
	var api	= new Api();
	if(!api.create(jscrypt,name)) throw "get api "+name+" failed";
	return api;
}
var cryp = {};
//base64编码和16进制编码，使用 CMemory 对象的 fromHex，fromBase64，toHex 和 toBase64 更方便。
//cryp.hexdecode	= initApi("hexdecode");
//cryp.hexencode	= initApi("hexencode");
//cryp.base64decode	= initApi("base64decode");
//cryp.base64encode	= initApi("base64encode");

//加载 jscrypt.dll 内的 C 函数。
cryp.md5_reset		= initApi("md5_reset");
cryp.md5_file		= initApi("md5_file2");
cryp.md5_direct		= initApi("md5_direct");
cryp.md5_finish		= initApi("md5_finish2");
cryp.md5_update		= initApi("md5_update2");
cryp.md5_create		= initApi("md5_create");
cryp.md5_close		= initApi("md5_close");

cryp.md5hmac_reset	= initApi("md5hmac_reset");
cryp.md5hmac_direct	= initApi("md5hmac_direct");
cryp.md5hmac_finish	= initApi("md5hmac_finish");
cryp.md5hmac_update	= initApi("md5hmac_update");
cryp.md5hmac_create	= initApi("md5hmac_create");
cryp.md5hmac_close	= initApi("md5hmac_close");

cryp.sha_reset		= initApi("sha_reset");
cryp.sha_file		= initApi("sha_file");
cryp.sha_direct		= initApi("sha_direct");
cryp.sha_finish		= initApi("sha_finish");
cryp.sha_update		= initApi("sha_update");
cryp.sha_create		= initApi("sha_create");
cryp.sha_close		= initApi("sha_close");

cryp.shahmac_reset	= initApi("shahmac_reset");
cryp.shahmac_direct	= initApi("shahmac_direct");
cryp.shahmac_finish	= initApi("shahmac_finish");
cryp.shahmac_update	= initApi("shahmac_update");
cryp.shahmac_create	= initApi("shahmac_create");
cryp.shahmac_close	= initApi("shahmac_close");

cryp.aes_create		= initApi("aes_create");
cryp.aes_close		= initApi("aes_close");
cryp.aes_set_enc_key	= initApi("aes_set_enc_key");
cryp.aes_set_dec_key	= initApi("aes_set_dec_key");
cryp.aes_set_iv		= initApi("aes_set_iv");
cryp.aes_get_iv		= initApi("aes_get_iv");
cryp.aes_enc_cbc		= initApi("aes_enc_cbc");
cryp.aes_dec_cbc		= initApi("aes_dec_cbc");
cryp.aes_enc_ecb		= initApi("aes_enc_ecb");
cryp.aes_dec_ecb		= initApi("aes_dec_ecb");

cryp.ecc_create		= initApi("ecc_create");
cryp.ecc_close		= initApi("ecc_close");
cryp.ecc_createkey	= initApi("ecc_createkey");
cryp.ecc_getpublickey	= initApi("ecc_getpublickey");
cryp.ecc_getprivatekey	= initApi("ecc_getprivatekey");
cryp.ecc_setpublickey	= initApi("ecc_setpublickey");
cryp.ecc_setprivatekey	= initApi("ecc_setprivatekey");
cryp.ecc_getsign		= initApi("ecc_getsign");
cryp.ecc_verify		= initApi("ecc_verify");
cryp.ecc_encrypt		= initApi("ecc_encrypt");
cryp.ecc_decrypt		= initApi("ecc_decrypt");
cryp.ecc_makepublickey	= initApi("ecc_makepublickey");
cryp.ecc_keylen		= initApi("ecc_keylen");
cryp.ecc_signlen		= initApi("ecc_signlen");

//CEcc 有两种，isIes = true 用于加密，isIes = false 用于签名，两者不能混用。
function CEcc(isIes){
	if(this===undefined){
		throw "can't call this function direct,must use new CEcc(true|false);";
		return undefined;
	}
	//启用自动销毁机制
	Stack.push(this);
	this.handle = cryp.ecc_create(isIes);
}
Object.defineProperty(CEcc, "keylen", {
	value: cryp.ecc_keylen(),
	writable: false,
	enumerable: true,
	configurable: true
});
Object.defineProperty(CEcc, "signlen", {
	value: cryp.ecc_signlen(),
	writable: false,
	enumerable: true,
	configurable: true
});
//释放一个 ecdsa 对象
CEcc.prototype.close = function(){
	var r = 0!=cryp.ecc_close(this.handle);
	this.handle = 0;
	return r;
}
CEcc.prototype.dispose = function(){
	this.close();
}
//生成一个新的公钥私钥对，初始对象是没有公钥私钥对的，必须加载或者生成新的秘钥，才可以使用其它功能。
CEcc.prototype.createKey = function(){
	return 0!=cryp.ecc_createkey(this.handle);
}
//生成新的公钥，一个私钥可以对应多个公钥。
CEcc.prototype.makePublicKey = function(){
	return 0!=cryp.ecc_makepublickey(this.handle);
}
//获取公钥，成功返回一个 CMemory 对象。
CEcc.prototype.getPublicKey = function(){
	var pubkey = new CMemory();
	pubkey.length = CEcc.keylen*2;
	var r = cryp.ecc_getpublickey(this.handle,pubkey.handle());
	if(!r){
		pubkey.dispose();
		return undefined;
	}
	return pubkey;
}
//设置公钥，参数是一个 CMemory 对象，长度必须是 2*68。
CEcc.prototype.setPublicKey = function(pubkey,len){
	var p = parseMemParamAsBin(pubkey,len,"setPublicKey(data,len)");
	if(!p) return false;
	if(p.len!=136){
		throw "CEcc.setPublicKey(pubkey,len), len!==136,len = "+len;
		return false;
	}
	var r = 0!=cryp.ecc_setpublickey(this.handle,p.data,p.len);
	if(!r){
		throw "CEcc.setPublicKey(pubkey) return false";
	}
	return r;
}
CEcc.prototype.getPrivateKey = function(){
	var prikey = new CMemory();
	prikey.length = CEcc.keylen;
	var r = cryp.ecc_getprivatekey(this.handle,prikey.handle());
	if(!r){
		prikey.dispose();
		return undefined;
	}
	return prikey;
}
CEcc.prototype.setPrivateKey = function(prikey,len){
	var p = parseMemParamAsBin(prikey,len,"setPrivateKey(data,len)");
	if(!p) return false;
	if(p.len!=CEcc.keylen){
		throw "CEcc.setPrivateKey(prikey,len), len!==68,len = "+len;
		return false;
	}
	var r = 0!=cryp.ecc_setprivatekey(this.handle,p.data,p.len);
	if(!r){
		throw "CEcc.setPrivateKey(prikey) return false";
	}
	return r;
}
CEcc.prototype.savePrivateKey = function(filename,override){
	return this.saveKey(filename,override,true);
}
CEcc.prototype.savePublicKey = function(filename,override){
	return this.saveKey(filename,override,false,true);
}
CEcc.prototype.saveKey = Class(function(filename,override,noPublic,noPrivate){
	if(FileSystem.isExist(filename)&&!override){
		throw "can't override a exist file: "+filename;
		return false;
	}
	if(!FileSystem.createFolder(filename,true)){
		throw "can't create parent folder of: "+filename;
		return false;
	}
	var prikey,pubkey;
	if(!noPublic){
		pubkey = this.getPublicKey();
		if(!pubkey) return false;
	}
	if(!noPrivate){
		prikey = this.getPrivateKey();
		if(!prikey) return false;
	}
	var file = new CFile();
	if(!file.create(filename)){
		throw "can't create file: "+filename;
		return false;
	}
	file.length = 0;
	if(pubkey){
		if(CEcc.keylen*2!=file.write(pubkey)){
			throw "write public data failed";
			return false;
		}
	}
	if(prikey){
		if(CEcc.keylen!=file.write(prikey)){
			throw "write private data failed";
			return false;
		}
	}
	return true;
});
CEcc.prototype.loadPrivateKey = function(filename){
	return this.loadKey(filename,true);
}
CEcc.prototype.loadPublicKey = function(filename){
	return this.loadKey(filename,false,true);
}
CEcc.prototype.loadKey = Class(function(filename,noPublic,noPrivate){
	var file = new CFile();
	if(!file.create(filename,true,true)){
		throw "loadKey() can't open file to read: "+filename;
		return false;
	}
	var key = new CMemory();
	var len = CEcc.keylen*3;
	key.length = len;
	var readlen = file.read(key,len);
	if(readlen!=CEcc.keylen&&readlen!=CEcc.keylen*2&&readlen!=CEcc.keylen*3){
		throw "loadKey() can't read key data";
		return false;
	}
	if(!noPublic){
		if(readlen<CEcc.keylen*2){
			throw "loadKey() file length error: "+readlen;
			return false;
		}
		var r = 0!=cryp.ecc_setpublickey(this.handle,key.handle(),CEcc.keylen*2);
		if(!r){
			throw "loadKey() public key data is error";
			return false;
		}
	}
	if(!noPrivate){
		var pridata;
		if(readlen==CEcc.keylen) pridata = key.handle();
		else{
			if(readlen!=CEcc.keylen*3){
				throw "loadKey() file maybe a public key file.";
				return false;
			}
			pridata = key.handle() + CEcc.keylen*2;
		}
		var r = 0!=cryp.ecc_setprivatekey(this.handle,pridata,CEcc.keylen);
		if(!r){
			throw "loadKey() private key data is error";
			return false;
		}
	}
	return true;
});
CEcc.prototype.getSign = function(data,len){
	var p = parseMemParam(data,len,"CEcc.verify(sign,data,len)");
	if(!p) return undefined;
	var sign = new CMemory();
	sign.length = CEcc.signlen;
	var r = cryp.ecc_getsign(this.handle,sign.handle(),p.data,p.len);
	if(p.mem) p.mem.dispose();
	if(!r){
		sign.dispose();
		return undefined;
	}
	return sign;
}
CEcc.prototype.verify = Class(function(sign,data,len){
	var p = parseMemParam(data,len,"CEcc.verify(sign,data,len)");
	if(!p) return undefined;
	if(!(sign instanceof CMemory)){
		throw "CEcc.verify(sign,data,len), sign 不是一个 CMemory 对象";
		return undefined;
	}
	if(sign.length!=CEcc.signlen){
		throw "CEcc.verify(sign,data,len), sign.length must be: "+CEcc.signlen+", current is: "+sign.length;
		return undefined;
	}
	var rst = 0!=cryp.ecc_verify(this.handle,p.data,p.len,sign.handle(),CEcc.signlen);
	if(p.mem) p.mem.dispose();
	return rst;
});

CEcc.prototype.encrypt = function(data,len){
	var p = parseMemParam(data,len,"CEcc.encrypt(data,len)");
	if(!p) return undefined;
	var output = new CMemory();
	var enclen = cryp.ecc_encrypt(this.handle,output.obj(),p.data,p.len);
	if(p.mem) p.mem.dispose();
	if(!enclen){
		output.dispose();
		return undefined;
	}
	return output;
};
CEcc.prototype.decrypt = function(data,len){
	var p = parseMemParamAsBin(data,len,"CEcc.decrypt(data,len)");
	if(!p) return undefined;
	var output = new CMemory();
	var r = cryp.ecc_decrypt(this.handle,output.obj(),p.data,p.len);
	if(!r){
		output.dispose();
		return undefined;
	}
	return output;
}
function md5(data,len){
	var p = parseMemParam(data,len,"CMd5(data,len)");
	if(!p) return undefined;
	var rst = new CMemory();
	rst.length = 16;
	var r = 0!=cryp.md5_direct(rst.handle(),p.data,p.len);
	if(p.mem) p.mem.dispose();
	if(!r){
		rst.dispose();
		return undefined;
	}
	return rst;
}
function md5_file(fn){
	var rst = new CMemory();
	rst.length = 16;
	var r = cryp.md5_file(rst,fn);
	if(!r){
		rst.dispose();
		return undefined;
	}
	return rst;
}

function md5hmac(data,len,key,klen){
	var pd = parseMemParam(data,len,"md5hmac(data,len,key,klen)");
	if(!pd) return undefined;
	var pk = parseMemParam(key,klen,"CMd5Hmac(key,klen,data,len)");
	if(!pk) return undefined;

	var rst = new CMemory();
	rst.length = 16;
	var r = 0!=cryp.md5hmac_direct(rst.handle(),pd.data,pd.len,pk.data,pk.len);
	if(pd.mem) pd.mem.dispose();
	if(pk.mem) pk.mem.dispose();
	if(!r){
		rst.dispose();
		return undefined;
	}
	return rst;
}
function sha(data,len,bits){
	if(bits!==20&&bits!==28&&bits!=32&&bits!=48&&bits!=64){
		bits = 20;
	}
	var p = parseMemParam(data,len,"sha(data,len,bits)");
	if(!p) return undefined;
	var rst = new CMemory();
	rst.length = bits;
	var r = cryp.sha_direct(rst.handle(),p.data,p.len,bits);
	if(p.mem) p.mem.dispose();
	if(!r){
		rst.dispose();
		return undefined;
	}
	return rst;
}
function sha_file(fn,bits){
	if(bits!==20&&bits!==28&&bits!==32&&bits!==48&&bits!==64) bits = 20;
	var rst = new CMemory();
	rst.length = bits;
	var r = cryp.sha_file(rst.handle(),fn,bits);
	if(!r){
		rst.dispose();
		return undefined;
	}
	return rst;
}
function shahmac(data,len,key,klen,bits){
	var pd = parseMemParam(data,len,"shahmac(data,ken,key,klen,bits)");
	if(!pd) return undefined;
	var pk = parseMemParam(key,klen,"shahmac(key,klen,data,ken,bits)");
	if(!pk) return undefined;
	var digest = new CMemory();
	digest.length = bits;
	var r = 0!=cryp.shahmac_direct(digest.handle(),pd.data,pd.len,pk.data,pk.len,bits);
	if(pd.mem) pd.mem.dispose();
	if(pk.mem) pk.mem.dispose();
	if(!r){
		digest.dispose();
		return undefined;
	}
	return digest;
}
function cmpFileSha(file1,file2,bits){
	if(bits!==20&&bits!==28&&bits!==32&&bits!==48&&bits!==64) bits = 20;
	var digest1 = sha_file(file1,bits);
	if(!digest1){
		throw "can't read file: " + file1;
		return undefined;
	}
	var digest2 = sha_file(file2,bits);
	if(!digest2){
		throw "can't read file: " + file2;
		return undefined;
	}
	return digest1.equal(digest2.handle(),bits);
}
function CAes(bits){
	this.handle = cryp.aes_create();
	this.bits = bits;
	Stack.push(this);
}
CAes.prototype = {
	get bits(){
		if(this._bits!==16&&this._bits!==24&&this._bits!==32) this._bits = 16;
		return this._bits;
	},
	set bits(val){
		this._bits = val;
	}
}
CAes.prototype.dispose = function(){
	cryp.aes_close(this.handle);
}
CAes.prototype.setKey = function(key,len,dec){//bits = 16,24,32
	var p = parseMemParam(key,len,"CAes.setKey(data,len,dec)");
	if(!p) return undefined;
	if(dec)
		var r = cryp.aes_set_dec_key(this.handle,p.data,p.len,this.bits);
	else
		var r = cryp.aes_set_enc_key(this.handle,p.data,p.len,this.bits);
	if(p.mem) p.mem.dispose();
	return r!=0;
}
CAes.prototype.setEncKey = function(key,len){
	this.setKey(key,len,false);
}
CAes.prototype.setDecKey = function(key,len){
	this.setKey(key,len,true);
}
//iv 是一个大于16字节的内存对象，或者一个字串，字串不足16字节自动用 0 补足。
CAes.prototype.setIv = function(iv,len){
	var p = parseMemParam(iv,len,"CAes.setIv(data,len)");
	if(!p) return undefined;
	var r = cryp.aes_set_iv(this.handle,p.data,p.len);
	if(p.mem) p.mem.dispose();
	return r!=0;
}
CAes.prototype.getIv = function(){
	var iv = new CMemory();
	iv.length = 16;
	cryp.aes_get_iv(this.handle,iv.handle());
}
CAes.prototype.enc = function(data,len,mode){
	var p = parseMemParam(data,len,"CAes.encypt/decrypt(data,len)");
	if(!p) return undefined;
	var outlen;
	var reslen = p.len%16;
	if(reslen) outlen = p.len - reslen + 16;
	else outlen = p.len;
	var rst = new CMemory();
	rst.length = outlen;
	if(mode==="ecb")
		var enclen = cryp.aes_enc_ecb(this.handle,rst.handle(),outlen,p.data,p.len);
	else if(mode==="cbc")
		var enclen = cryp.aes_enc_cbc(this.handle,rst.handle(),outlen,p.data,p.len);
	else{
		rst.dispose();
		if(p.mem) p.mem.dispose();
		throw 'mode not "ecb" or "cbc"';
		return undefined;
	}
	if(p.mem) p.mem.dispose();
	if(!enclen){
		rst.dispose();
		return undefined;
	}
	return rst;
}
CAes.prototype.dec = function(data,len,mode){
	var p = parseMemParamAsBin(data,len,"CAes.decypt/decrypt(data,len)");
	if(!p) return undefined;
	if(p.len==0||p.len%16){
		throw "data.length is not 16x:"+len;
		return undefined;
	}
	var rst = new CMemory();
	rst.length = p.len;
	if(mode==="ecb")
		var declen = cryp.aes_dec_ecb(this.handle,rst.handle(),p.len,p.data,p.len);
	else if(mode==="cbc")
		var declen = cryp.aes_dec_cbc(this.handle,rst.handle(),p.len,p.data,p.len);
	else{
		rst.dispose();
		throw 'mode not "ecb" or "cbc"';
		return undefined;
	}
	if(!declen){
		rst.dispose();
		return undefined;
	}
	return rst;
}
CAes.prototype.encEcb = function(data,len){
	return this.enc(data,len,"ecb");
}
CAes.prototype.decEcb = function(data,len){
	return this.dec(data,len,"ecb");
}
CAes.prototype.encCbc = function(data,len){
	return this.enc(data,len,"cbc");
}
CAes.prototype.decCbc = function(data,len){
	return this.dec(data,len,"cbc");
}
