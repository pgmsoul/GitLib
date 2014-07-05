#include "stdafx.h"
//*{
//	"type":"group",
//	"name":"crypt",
//	"text":"提供的如下功能：CEcc，CAes，sha,md5。",
//	"remark":[
//		""
//	],
//	"member":[
//{
//	"type":"function",
//	"name":"parseMemParam(data,len,errFlag)",
//	"text":"分析内存类函数的参数，返回一个包含内存指针和内存长度的对象。",
//	"param":[
//		{
//			"type":"integer|CMemory|string",
//			"name":"data",
//			"text":"数据源，见 remark 部分的说明。"
//		},
//		{
//			"type":"integer",
//			"name":"len",
//			"text":"数据的长度。"
//		},
//		{
//			"type":"string",
//			"name":"errFlag",
//			"text":"一个标识，当抛出异常时用于识别哪里调用的 parseMemParam。"
//		}
//	],
//	"return":{
//		"type":"object",
//		"text":"成功返回一个包含 3 个属性的对象（mem,data,len)，data 是内存的指针，len 是数据的长度，mem 是字串情形时生成的临时内存变量，如果不是用 Class 包装的函数内使用 parseMemParam，这个内存需要手动调用 dispose 来释放。失败返回 false。"
//	},
//	"remark":[
//		"因为 JavaScript 参数无类型，允许传递各种类型的参数，而内存类函数对参数有严格的要求，为了安全，有必要对内存参数做一个统一的标准。",
//		"我们一般会使用 3 种方式传递内存类参数：",
//		"1) data 是一个内存指针（大于 0xffff 的数字），len 是数据的长度，这种情况，data 和 len 都是整数，这种情况 parseMemParam 不做额外处理，直接返回这两个值。",
//		"2) data 是一个 CMemory 对象，len 是数据长度，这种情况，parseMemParam 函数会检测 len 是否大于 data 的长度，如果超过 data 的大小，抛出异常，否则，返回 data 的句柄和 len。",
//		"3) data 既不是一个内存指针也不是一个 CMemory 对象，那么它将会被看成一个字串，任何类型都会被转换为字串；然后把这个字串按 ansi 编码作为数据，len 的传入值被忽略，重新被设置为字串的长度。",
//		"第 1 种情况，如果传递了错误的内存指针和长度，可能引起严重的错误导致程序崩溃，这种错误可能引发的后果是完全不可预知的，是应该极力避免的。第 2 和第 3 种情况，函数总是安全的。",
//		"第 3 种情形，会生成一个临时内存对象，如果 parseMemParam 不是在一个用 Class 包装的函数内调用的，应该手动释放这个临时内存变量。"
//	],
//	"source":"d:\\SoftProject\\GitLib\\jsuser\\jsres\\memory_after.js"
//}
//,{
//	"type":"class",
//	"name":"CEcc([isIes])",
//	"text":"CEcc 封装一个 椭圆加密对象，椭圆加密是一种不对称加密，可以用于加解密也可以用于签名验证系统。椭圆加密对象包含内部资源，不再使用的时候需要 dispose。",
//	"param":[
//		{
//			"type":"boolean",
//			"name":"[isIes]",
//			"text":"椭圆加密有两种对象，一种用于加密，一种用于签名验证，如果 isIes = true 则用于加密，否则用于签名，缺省是签名对象（isIes = false）。"
//		}
//	],
//	"member":[//*
class CEcc{
	//*{
	//	"type":"property",
	//	"name":"keylen",
	//	"objtype":"integer",
	//	"text":"秘钥长度，也是私钥长度，它是一个只读属性，值是 68，公钥长度是私钥长度的 2 倍即 136。"
	//}
	//,{
	//	"type":"property",
	//	"name":"signlen",
	//	"objtype":"integer",
	//	"text":"签名数据的长度，它是一个只读属性，值是 132。"
	//}
	//,{
	//	"type":"function",
	//	"name":"close()",
	//	"text":"释放内部的椭圆加密对象，效果和 dispose 相同，事实上 dispose 就是调用的 close 函数。",
	//	"param":[
	//	],
	//	"return":{
	//		"type":"boolean",
	//		"text":"操作是否成功。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"close()",
	//	"text":"释放内部的椭圆加密对象，dispose 就是直接调用 close 函数。",
	//	"param":[
	//	],
	//	"return":{
	//		"type":"boolean",
	//		"text":"操作是否成功。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"createKey()",
	//	"text":"生成一个新的公钥私钥对，初始对象是没有公钥私钥对的，必须加载或者生成新的秘钥，才可以使用其它功能。",
	//	"param":[
	//	],
	//	"return":{
	//		"type":"boolean",
	//		"text":"操作是否成功。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"makePublicKey()",
	//	"text":"生成新的公钥，一个私钥可以对应多个公钥。",
	//	"param":[
	//	],
	//	"return":{
	//		"type":"boolean",
	//		"text":"操作是否成功。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"getPublicKey()",
	//	"text":"获取当前对象的公钥，保存在一个 CMemory 对象里面，因为公钥是二进制数据。",
	//	"param":[
	//	],
	//	"return":{
	//		"type":"CMemory",
	//		"text":"成功返回一个 68 字节长的内存对象，失败返回 false。返回的内存对象不再使用时需要清理。"
	//	}
	//},
	//{
	//	"type":"function",
	//	"name":"setPublicKey(pubkey,[len])",
	//	"text":"设置公钥，公钥是一段 136 字节的二进制数据，这个函数即可以传递内存，也可以传递内存指针。",
	//	"param":[
	//		{
	//			"type":"integer|CMemory",
	//			"name":"pubkey",
	//			"text":"公钥数据，这个参数可以是一个内存，也可以是一个内存指针。"
	//		},
	//		{
	//			"type":"integer",
	//			"name":"[len]",
	//			"text":"长度，当 pubkey 是一个内存对象时，这个值可以缺省，但是此时 pubk 必须是 136 字节大小，否则函数会失败。如果要传递这个值，就必须传 136，其它值都会失败。"
	//		}
	//	],
	//	"return":{
	//		"type":"boolean",
	//		"text":"操作是否成功。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"getPrivateKey()",
	//	"text":"获取当前对象的私钥，保存在一个 CMemory 对象里面，因为私钥是二进制数据。",
	//	"param":[
	//	],
	//	"return":{
	//		"type":"CMemory",
	//		"text":"成功返回一个 68 字节长的内存对象，失败返回 false。返回的内存对象不再使用时需要清理。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"setPrivateKey(prikey,[len])",
	//	"text":"设置私钥，私钥是一段 68 字节的二进制数据，这个函数即可以传递内存，也可以传递内存指针。",
	//	"param":[
	//		{
	//			"type":"integer|CMemory",
	//			"name":"prikey",
	//			"text":"公钥数据，这个参数可以是一个内存，也可以是一个内存指针。"
	//		},
	//		{
	//			"type":"integer",
	//			"name":"[len]",
	//			"text":"长度，当 prikey 是一个内存对象时，这个值可以缺省，但是此时 prikey 必须是 68 字节大小，否则函数会失败。如果要传递这个值，就必须传 68，其它值都会失败。"
	//		}
	//	],
	//	"return":{
	//		"type":"boolean",
	//		"text":"操作是否成功。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"saveKey(filename,[override],[noPublic],[noPrivate])",
	//	"text":"保存公钥私钥对到文件。",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"filename",
	//			"text":"要保存的文件名。"
	//		},
	//		{
	//			"type":"boolean",
	//			"name":"[Override]",
	//			"text":"如果指定的文件名已经存在，是否覆盖，缺省是不覆盖，函数抛出异常，并返回 false。"
	//		},
	//		{
	//			"type":"string",
	//			"name":"[noPublic]",
	//			"text":"如果设为 true，则不保存公钥，只保存私钥。"
	//		},
	//		{
	//			"type":"string",
	//			"name":"[noPrivate]",
	//			"text":"如果设为 true，则不保存私钥，只保存公钥。"
	//		}
	//	],
	//	"return":{
	//		"type":"boolean",
	//		"text":"操作是否成功。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"savePublicKey(filename,[override])",
	//	"text":"保存公钥到文件。",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"filename",
	//			"text":"要保存的文件名。"
	//		},
	//		{
	//			"type":"boolean",
	//			"name":"[Override]",
	//			"text":"如果指定的文件名已经存在，是否覆盖，缺省是不覆盖，函数抛出异常，并返回 false。"
	//		}
	//	],
	//	"return":{
	//		"type":"boolean",
	//		"text":"操作是否成功。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"savePrivateKey(filename,[override])",
	//	"text":"保存私钥到文件。",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"filename",
	//			"text":"要保存的文件名。"
	//		},
	//		{
	//			"type":"boolean",
	//			"name":"[Override]",
	//			"text":"如果指定的文件名已经存在，是否覆盖，缺省是不覆盖，函数抛出异常，并返回 false。"
	//		}
	//	],
	//	"return":{
	//		"type":"boolean",
	//		"text":"操作是否成功。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"loadKey(filename,[noPublic],[noPrivate])",
	//	"text":"读取一个 saveKey 函数保存的秘钥文件，这个文件依不同情况，可以只包含公钥，只包含私钥，或者都包括。",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"filename",
	//			"text":"要读取的文件名。"
	//		},
	//		{
	//			"type":"string",
	//			"name":"[noPublic]",
	//			"text":"如果设为 true，则只加载私钥。"
	//		},
	//		{
	//			"type":"string",
	//			"name":"[noPrivate]",
	//			"text":"如果设为 true，则只加载公钥。"
	//		}
	//	],
	//	"return":{
	//		"type":"boolean",
	//		"text":"操作是否成功。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"loadPublicKey(filename)",
	//	"text":"从文件加载公钥，这个文件必须包含公钥。",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"filename",
	//			"text":"要读取的文件名。"
	//		}
	//	],
	//	"return":{
	//		"type":"boolean",
	//		"text":"操作是否成功。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"loadPrivateKey(filename)",
	//	"text":"从文件加载私钥，这个文件必须包含私钥。",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"filename",
	//			"text":"要读取的文件名。"
	//		}
	//	],
	//	"return":{
	//		"type":"boolean",
	//		"text":"操作是否成功。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"getSign(data,[len])",
	//	"text":"获取数据的签名，签名只使用私钥。",
	//	"param":[
	//		{
	//			"type":"integer|CMemory|string",
	//			"name":"data",
	//			"text":"要签名的数据，可以是内存指针，内存对象，或者字串，任何其它对象都会被转换外字串。字串数据不会使用 len，而是字串本身的长度。"
	//		},
	//		{
	//			"type":"integer",
	//			"name":"[len]",
	//			"text":"data 是指针或内存时，数据的长度。"
	//		}
	//	],
	//	"return":{
	//		"type":"CMemory",
	//		"text":"成功返回一个 132 字节的内存对象，失败返回 false。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"verify(sign,data,[len])",
	//	"text":"验证签名是否正确，使用此函数必须保证当前对象是一个签名验证对象。当前对象必须设置正确的公钥。",
	//	"param":[
	//		{
	//			"type":"CMemory",
	//			"name":"sign",
	//			"text":"签名数据，它的大小必须是签名长度，132 字节。"
	//		},
	//		{
	//			"type":"integer|CMemory|string",
	//			"name":"data",
	//			"text":"要验证签名的数据，可以是内存指针，内存对象，或者字串，任何其它对象都会被转换外字串。字串数据不会使用 len，而是字串本身的长度。"
	//		},
	//		{
	//			"type":"integer",
	//			"name":"[len]",
	//			"text":"data 是指针或内存时，数据的长度。"
	//		}
	//	],
	//	"return":{
	//		"type":"boolean",
	//		"text":"如果签名验证通过，返回 true，否则返回 false，错误的参数返回 undefined。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"encrypt(data,[len])",
	//	"text":"使用公钥加密数据，使用此函数必须保证当前对象是一个用于加密的对象。",
	//	"param":[
	//		{
	//			"type":"integer|CMemory|string",
	//			"name":"data",
	//			"text":"要加密的数据，可以是内存指针，内存对象，或者字串，任何其它对象都会被转换外字串。字串数据不会使用 len，而是字串本身的长度。"
	//		},
	//		{
	//			"type":"integer",
	//			"name":"[len]",
	//			"text":"data 是指针或内存时，数据的长度。"
	//		}
	//	],
	//	"return":{
	//		"type":"CMemory",
	//		"text":"成功返回一个 CMemory 对象，失败返回 false。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"deccrypt(data,[len])",
	//	"text":"使用私钥解密数据，使用此函数必须保证当前对象是一个用于加密的对象。",
	//	"param":[
	//		{
	//			"type":"integer|CMemory",
	//			"name":"data",
	//			"text":"要解密的数据，可以是内存指针或内存对象。"
	//		},
	//		{
	//			"type":"integer",
	//			"name":"[len]",
	//			"text":"data 是指针或内存时，数据的长度。"
	//		}
	//	],
	//	"return":{
	//		"type":"CMemory",
	//		"text":"成功返回一个 CMemory 对象，失败返回 false。"
	//	}
	//}//*
};
//*],"source":"d:\\SoftProject\\GitLib\\jsuser\\example\\ecc.jsuser"}//*

//*,{
//	"type":"function",
//	"name":"md5(data,[len])",
//	"text":"获取数据的 md5 摘要，md5 摘要是一段 16 字节的数据。",
//	"param":[
//		{
//			"type":"integer|CMemory|string",
//			"name":"data",
//			"text":"源数据，可以是内存指针，内存对象，或者字串，任何其它对象都会被转换外字串。字串数据不会使用 len，而是字串本身的长度。"
//		},
//		{
//			"type":"integer",
//			"name":"[len]",
//			"text":"data 是指针或内存时，数据的长度。"
//		}
//	],
//	"return":{
//		"type":"CMemory",
//		"text":"成功返回一个 CMemory 对象，内存对象的长度是 16 字节，需要手动清理。失败返回 false。"
//	}
//}
//,{
//	"type":"function",
//	"name":"md5_file(filename)",
//	"text":"获取文件的 md5 摘要，md5 摘要是一段 16 字节的数据。",
//	"param":[
//		{
//			"type":"string",
//			"name":"filename",
//			"text":"文件名。"
//		}
//	],
//	"return":{
//		"type":"CMemory",
//		"text":"成功返回一个 CMemory 对象，内存对象的长度是 16 字节，需要手动清理。失败返回 false。"
//	}
//}
//,{
//	"type":"function",
//	"name":"md5hmac(data,[len],key,[keylen])",
//	"text":"获取数据的 md5 算法摘要，md5 摘要是一段 16 字节的数据。为了让相同的数据有不同的 md5 值，设置 key 来加扰，不同的 key 将得到不同的 md5 摘要数据。",
//	"param":[
//		{
//			"type":"integer|CMemory|string",
//			"name":"data",
//			"text":"源数据，可以是内存指针，内存对象，或者字串，任何其它对象都会被转换外字串。字串数据不会使用 len，而是字串本身的长度。"
//		},
//		{
//			"type":"integer",
//			"name":"[len]",
//			"text":"源数据的长度，如果想缺省此参数，传 undefined。"
//		},
//		{
//			"type":"integer|CMemory|string",
//			"name":"key",
//			"text":"加扰数据，可以是内存指针，内存对象，或者字串，任何其它对象都会被转换外字串。字串数据不会使用 len，而是字串本身的长度。"
//		},
//		{
//			"type":"integer",
//			"name":"[keylen]",
//			"text":"加扰数据的长度，如果想缺省此参数，传 undefined。"
//		}
//	],
//	"return":{
//		"type":"CMemory",
//		"text":"成功返回一个 CMemory 对象，内存对象的长度是 16 字节，需要手动清理。失败返回 false。"
//	}
//}
//,{
//	"type":"function",
//	"name":"sha(data,[len],[bits])",
//	"text":"获取数据的 sha 摘要，sha 摘要有 5 种，用 bits 字段指定，分别是 16、28、32、48、64，缺省是 16（sha1）。",
//	"param":[
//		{
//			"type":"integer|CMemory|string",
//			"name":"data",
//			"text":"源数据，可以是内存指针，内存对象，或者字串，任何其它对象都会被转换外字串。字串数据不会使用 len，而是字串本身的长度。"
//		},
//		{
//			"type":"integer",
//			"name":"[len]",
//			"text":"data 是指针或内存时，数据的长度，要缺省此参数，可以传 undefined。"
//		},
//		{
//			"type":"integer",
//			"name":"[bits]",
//			"text":"sha 的类型，同时代表了摘要数据的长度，共有 5 种：16、28、32、48、64，缺省是 16，其它任何错误的值，都按 16 处理。"
//		}
//	],
//	"return":{
//		"type":"CMemory",
//		"text":"成功返回一个 CMemory 对象，内存对象的长度是 bits 字节，需要手动清理。失败返回 false。"
//	},"source":"d:\\SoftProject\\GitLib\\jsuser\\example\\sha.jsuser"
//}
//,{
//	"type":"function",
//	"name":"sha_file(filename,[bits])",
//	"text":"获取文件的 sha 摘要，sha 摘要有 5 种，用 bits 字段指定，分别是 16、28、32、48、64，缺省是 16（sha1）。",
//	"param":[
//		{
//			"type":"string",
//			"name":"filename",
//			"text":"文件名。"
//		},
//		{
//			"type":"integer",
//			"name":"[bits]",
//			"text":"sha 的类型，同时代表了摘要数据的长度，共有 5 种：16、28、32、48、64，缺省是 16，其它任何错误的值，都按 16 处理。"
//		}
//	],
//	"return":{
//		"type":"CMemory",
//		"text":"成功返回一个 CMemory 对象，内存对象的长度是 bits 字节，需要手动清理。失败返回 false。"
//	}
//}
//,{
//	"type":"function",
//	"name":"shahmac(data,[len],key,[keylen],[bits])",
//	"text":"获取数据的 sha 摘要，sha 摘要有 5 种，用 bits 字段指定，分别是 16、28、32、48、64，缺省是 16（sha1）。为了让相同的数据有不同的摘要值，设置 key 来加扰，不同的 key 将得到不同的 sha 摘要数据。",
//	"param":[
//		{
//			"type":"integer|CMemory|string",
//			"name":"data",
//			"text":"源数据，可以是内存指针，内存对象，或者字串，任何其它对象都会被转换外字串。字串数据不会使用 len，而是字串本身的长度。"
//		},
//		{
//			"type":"integer",
//			"name":"[len]",
//			"text":"源数据的长度，如果想缺省此参数，传 undefined。"
//		},
//		{
//			"type":"integer|CMemory|string",
//			"name":"key",
//			"text":"加扰数据，可以是内存指针，内存对象，或者字串，任何其它对象都会被转换外字串。字串数据不会使用 len，而是字串本身的长度。"
//		},
//		{
//			"type":"integer",
//			"name":"[keylen]",
//			"text":"加扰数据的长度，如果想缺省此参数，传 undefined。"
//		}
//	],
//	"return":{
//		"type":"CMemory",
//		"text":"成功返回一个 CMemory 对象，内存对象的长度是 bits 字节，需要手动清理。失败返回 false。"
//	}
//}
//,{
//	"type":"function",
//	"name":"cmpFileSha(file1,file2,[bits])",
//	"text":"比较两个文件的 sha 摘要是否相同，从而确定文件是否相同。",
//	"param":[
//		{
//			"type":"string",
//			"name":"file1",
//			"text":"文件1。"
//		},
//		{
//			"type":"string",
//			"name":"file1",
//			"text":"文件2。"
//		},
//		{
//			"type":"integer",
//			"name":"[bits]",
//			"text":"比较的 sha 类型，缺省 16."
//		}
//	],
//	"return":{
//		"type":"boolean",
//		"text":"返回是否相同，如果出现错误，函数抛出异常。。"
//	}
//}
//,{
//	"type":"class",
//	"name":"CAes([bits])",
//	"text":"AES 加密对象，AES 加密使用 3 种密码位数，分别是 16,24,32，bits 参数指定这个选项。",
//	"param":[
//		{
//			"type":"integer",
//			"name":"[bits]",
//			"text":"使用的密码位数，缺省是 16 字节（128位）。这个位数也可以通过对象的 bits 属性动态设置，错误的值将会被设为默认值 16."
//		}
//	],
//	"member":[//*
class CAes{
	//*{
	//	"type":"function",
	//	"name":"setEncKey(key,[len])",
	//	"text":"设置秘钥，并且设置 AES 对象为加密模式。AES 是对称加密，加解密密码相同。错误的秘钥不会引发错误，因为 AES 本身不区分任何数据。秘钥长度可以是大于 0 的任何长度，但是只有前 bits 字节是有效的，后面的数据会被舍弃。如果不足 bits 字节，自动用 0 补足。",
	//	"param":[
	//		{
	//			"type":"integer|CMemory|string",
	//			"name":"key",
	//			"text":"源数据，可以是内存指针，内存对象，或者字串，任何其它对象都会被转换外字串。字串数据不会使用 len，而是字串本身的长度。"
	//		},
	//		{
	//			"type":"integer",
	//			"name":"[len]",
	//			"text":"源数据的长度，如果想缺省此参数，传 undefined。"
	//		}
	//	],
	//	"return":{
	//		"type":"boolean",
	//		"text":"操作是否成功。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"setDecKey(key,[len])",
	//	"text":"设置秘钥，并且设置 AES 对象为解密模式。AES 是对称加密，加解密密码相同。错误的秘钥不会引发错误，因为 AES 本身不区分任何数据。秘钥长度可以是大于 0 的任何长度，但是只有前 bits 字节是有效的，后面的数据会被舍弃。如果不足 bits 字节，自动用 0 补足。",
	//	"param":[
	//		{
	//			"type":"integer|CMemory|string",
	//			"name":"key",
	//			"text":"源数据，可以是内存指针，内存对象，或者字串，任何其它对象都会被转换外字串。字串数据不会使用 len，而是字串本身的长度。"
	//		},
	//		{
	//			"type":"integer",
	//			"name":"[len]",
	//			"text":"源数据的长度，如果想缺省此参数，传 undefined。"
	//		}
	//	],
	//	"return":{
	//		"type":"boolean",
	//		"text":"操作是否成功。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"setIv(key,[len])",
	//	"text":"设置一个 16 字节的向量，这个向量只用于 cbc 模式。",
	//	"param":[
	//		{
	//			"type":"integer|CMemory|string",
	//			"name":"key",
	//			"text":"源数据，可以是内存指针，内存对象，或者字串，任何其它对象都会被转换外字串。字串数据不会使用 len，而是字串本身的长度。"
	//		},
	//		{
	//			"type":"integer",
	//			"name":"[len]",
	//			"text":"源数据的长度，如果想缺省此参数，传 undefined。最多只有 16 个字节被使用，如果不足 16 字节，用 0 补足。"
	//		}
	//	],
	//	"return":{
	//		"type":"boolean",
	//		"text":"操作是否成功。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"encEcb(data,[len])",
	//	"text":"使用 ecb 模式加密数据。AES 是分组加密，每组长度 16 字节，如果源数据长度不是 16 字节的整数倍，会用 0 来补足到 16 字节对齐。因为 AES 加解密都不改变数据的长度，所以解密后的数据也是 16 的倍数，也就是加密前数据的长度会丢失。某些场合必须保存原始数据长度，才可以重新获得加密前的正确数据。",
	//	"param":[
	//		{
	//			"type":"integer|CMemory|string",
	//			"name":"data",
	//			"text":"源数据，可以是内存指针，内存对象，或者字串，任何其它对象都会被转换外字串。字串数据不会使用 len，而是字串本身的长度。"
	//		},
	//		{
	//			"type":"integer",
	//			"name":"[len]",
	//			"text":"源数据的长度，如果想缺省此参数，传 undefined。最多只有 16 个字节被使用，如果不足 16 字节，用 0 补足。"
	//		}
	//	],
	//	"return":{
	//		"type":"CMemory",
	//		"text":"如果函数成功，返回一个内存对象，如果函数失败返回 false。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"decEcb(data,[len])",
	//	"text":"使用 ecb 模式解密数据。AES 是分组加密，每组长度 16 字节，如果源数据长度不是 16 字节的整数倍，会用 0 来补足到 16 字节对齐。因为 AES 加解密都不改变数据的长度，所以解密后的数据也是 16 的倍数，也就是加密前数据的长度会丢失。某些场合必须保存原始数据长度，才可以重新获得加密前的正确数据。",
	//	"param":[
	//		{
	//			"type":"integer|CMemory|string",
	//			"name":"data",
	//			"text":"源数据，可以是内存指针，内存对象，或者字串，任何其它对象都会被转换外字串。字串数据不会使用 len，而是字串本身的长度。"
	//		},
	//		{
	//			"type":"integer",
	//			"name":"[len]",
	//			"text":"源数据的长度，如果想缺省此参数，传 undefined。最多只有 16 个字节被使用，如果不足 16 字节，用 0 补足。"
	//		}
	//	],
	//	"return":{
	//		"type":"CMemory",
	//		"text":"如果函数成功，返回一个内存对象，如果函数失败返回 false。解密数据长度必须是 16 的整数被，如果不是，函数直接返回 false。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"encCbc(data,[len])",
	//	"text":"使用 cbc 模式加密数据。AES 是分组加密，每组长度 16 字节，如果源数据长度不是 16 字节的整数倍，会用 0 来补足到 16 字节对齐。因为 AES 加解密都不改变数据的长度，所以解密后的数据也是 16 的倍数，也就是加密前数据的长度会丢失。某些场合必须保存原始数据长度，才可以重新获得加密前的正确数据。",
	//	"param":[
	//		{
	//			"type":"integer|CMemory|string",
	//			"name":"data",
	//			"text":"源数据，可以是内存指针，内存对象，或者字串，任何其它对象都会被转换外字串。字串数据不会使用 len，而是字串本身的长度。"
	//		},
	//		{
	//			"type":"integer",
	//			"name":"[len]",
	//			"text":"源数据的长度，如果想缺省此参数，传 undefined。最多只有 16 个字节被使用，如果不足 16 字节，用 0 补足。"
	//		}
	//	],
	//	"return":{
	//		"type":"CMemory",
	//		"text":"如果函数成功，返回一个内存对象，如果函数失败返回 false。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"decCbc(data,[len])",
	//	"text":"使用 cbc 模式解密数据。AES 是分组加密，每组长度 16 字节，如果源数据长度不是 16 字节的整数倍，会用 0 来补足到 16 字节对齐。因为 AES 加解密都不改变数据的长度，所以解密后的数据也是 16 的倍数，也就是加密前数据的长度会丢失。某些场合必须保存原始数据长度，才可以重新获得加密前的正确数据。",
	//	"param":[
	//		{
	//			"type":"integer|CMemory|string",
	//			"name":"data",
	//			"text":"源数据，可以是内存指针，内存对象，或者字串，任何其它对象都会被转换外字串。字串数据不会使用 len，而是字串本身的长度。"
	//		},
	//		{
	//			"type":"integer",
	//			"name":"[len]",
	//			"text":"源数据的长度，如果想缺省此参数，传 undefined。最多只有 16 个字节被使用，如果不足 16 字节，用 0 补足。"
	//		}
	//	],
	//	"return":{
	//		"type":"CMemory",
	//		"text":"如果函数成功，返回一个内存对象，如果函数失败返回 false。解密数据长度必须是 16 的整数被，如果不是，函数直接返回 false。"
	//	}
	//}//*
};
//*],"source":"d:\\SoftProject\\GitLib\\jsuser\\example\\aes.jsuser"
//}],"source":"D:\\SoftProject\\GitLib\\jsuser\\example\\crypt.jsuser"}//*