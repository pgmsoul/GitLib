[
	{
		"type": "class",
		"name": "CMemory",
		"text": "内存是一个非常重要的概念，因为任何数据，包括数字和字串，都占用一定的内存空间，也就是说，任何数据都是内存，所以内存的操作非常重要。",
		"member": [
			{
				"type": "function",
				"name": "bind(pointer,size)",
				"text": "把指针绑定到内存对象（从而可以对它进行内存操作）",
				"param": [
					{
						"type": "integer",
						"name": "pointer",
						"text": "一个内存指针（C++任何对象包括数据的指针都是内存指针）"
					},
					{
						"type": "integer",
						"name": "size",
						"text": "这个内存指针的大小，比如整数指针的大小是 4 ，double 指针的大小是 8，具体大小由具体对象来决定。"
					}
				],
				"return": {
					"type": "bool",
					"text": "指示绑定是否成功，如果当前对象的内存对象已经释放，返回 undefined 。"
				},
				"remark": [
					"绑定类型的内存不能调整大小，所有需要调整大小的函数都会失败。解除绑定使用 close 函数"
				]
			},
			{
				"type": "function",
				"name": "close()",
				"text": "释放内存空间或者解除绑定的内存指针。",
				"return": {
					"type": "bool",
					"text": "此函数总是返回 true，除非对象内部的内存对象已经释放（dispose），此时返回 undefined。"
				}
			},
			{
				"type": "function",
				"name": "copyFrom(mem,length,[offset])",
				"text": "把数据从 mem 拷贝到当前内存的 offset 位置",
				"param": [
					{
						"type": "integer",
						"name": "mem",
						"text": "源内存"
					},
					{
						"type": "integer",
						"name": "length",
						"text": "要拷贝的长度，此长度不能大于有效长度（偏移位置到末尾的距离）"
					},
					{
						"type": "integer",
						"name": "offset",
						"text": "当前内存的拷贝位置，缺省是 0 ，开头位置"
					}
				],
				"return": {
					"type": "integer",
					"text": "失败返回 undefined，成功返回实际拷贝的内存长度"
				},
				"remark": [
					"内存操作使用的是MoveMemory，所以源内存和目的内存可以重叠。"
				]
			},
			{
				"type": "function",
				"name": "copyTo(mem,length,[offset])",
				"text": "把数据从当前内存的 offset 位置拷贝到目的内存 mem",
				"param": [
					{
						"type": "integer",
						"name": "mem",
						"text": "目的内存"
					},
					{
						"type": "integer",
						"name": "length",
						"text": "要拷贝的长度，此长度不能大于有效长度（偏移位置到末尾的距离）"
					},
					{
						"type": "integer",
						"name": "offset",
						"text": "当前内存的拷贝位置，缺省是 0 ，开头位置"
					}
				],
				"return": {
					"type": "integer",
					"text": "失败返回 undefined，成功返回实际拷贝的内存长度"
				},
				"remark": [
					"内存操作使用的是MoveMemory，所以源内存和目的内存可以重叠。"
				]
			},
			{
				"type": "function",
				"name": "double([minSize])",
				"text": "把内存容量双倍，但是不管原来的容量是多少，不管minSize设置为多少 ，双倍操作的容量不会小于 0x20",
				"param": [
					{
						"type": "integer",
						"name": "minSize",
						"text": "最少的容量，如果内存容量双倍后，不能达到这个容量，则会被扩充为这个容量。"
					}
				],
				"return": {
					"type": "bool",
					"text": "返回操作是否成功，如果系统内存不够，操作可能会失败，失败返回 undefined，成功返回 true"
				}
			},
			{
				"type": "function",
				"name": "fromHex(hex,[start])",
				"text": "把16进制字串转换为二进制数据，并且填充到指定位置，内存可能被扩充。",
				"param": [
					{
						"type": "string",
						"name": "hex",
						"text": "16进制字串(不能包含0x开头)，如果字串含有非16进制字符，会被剔除，转换仍然会成功。"
					},
					{
						"type": "integer",
						"name": "start",
						"text": "数据被填充的位置，缺省是 0 ，如果源数据比较大，超出内存的现有容量，会自动扩充"
					}
				],
				"return": {
					"type": "number",
					"text": "返回转换的二进制数据长度。"
				}
			},
			{
				"type": "function",
				"name": "getNumber(type,[sign],[start])",
				"text": "C语言常用的数字类型有 char、short、int32、float、double、int64 以及它们对应的无符号类型，但是 JavaScript 只有一种数字类型，所以需要在内存中保存对应的数字类型。这个函数获取一段内存表示对应类型的数字。",
				"param": [
					{
						"type": "integer|string",
						"name": "type",
						"text": "内存数据的类型，有以下 12 种：char（1字节，用 0 表示）、short（2字节，用 1 表示）、int32（4字节，用 2 表示）、float（4字节，用 3 表示）、double（8字节，用 4 表示）、int64（8字节，用 5 表示） 以及它们对应的无符号类型（代码和有符号类型相同）。"
					},
					{
						"type": "bool",
						"name": "sign",
						"text": "把内存看成有符号数字，还是无符号数字，缺省表示有符号数字。比如-127和129（-1和255）这些单字节数字在内存中实际上是相同的。这个参数对 double 和 float 类型无效，因为它们总是有符号的。"
					},
					{
						"type": "integer",
						"name": "type",
						"text": "读取的内存位置偏移"
					}
				],
				"return": {
					"type": "number",
					"text": "返回转换后的数字，如果起始位置到内存的结尾长度不够一个指定类型的大小，返回 undefined 。"
				},
				"remark": [
					"JavaScript 并不能表示64位整数的最大值, JavaScript的有效范围是 正负0x100000000000000之间."
				]
			},
			{
				"type": "function",
				"name": "getString([start],[length],[code])",
				"text": "把内存转换为字串",
				"param": [
					{
						"type": "integer",
						"name": "start",
						"text": "内存数据的起始位置，缺省是 0 。"
					},
					{
						"type": "integer",
						"name": "length",
						"text": "内存数据的长度，缺省是 -1，表示到内存的结尾，但是如果有 NULL 字符，字串会截断。"
					},
					{
						"type": "integer|string",
						"name": "code",
						"text": "字符串编码，可以是数字或者字串名称，如果这个参数和数据的编码不符，会产生乱码。如果这个参数缺省或者为 0 ，表示 Unicode 编码，如果为 3 ，表示 ansi 编码。"
					}
				],
				"return": {
					"type": "string",
					"text": "返回一个字串"
				}
			},
			{
				"type": "property",
				"name": "length",
				"text": "获取设置内存的大小，对于绑定类型的内存，设置无效"
			},
			{
				"type": "function",
				"name": "move(src,dst,length)",
				"text": "在当前内存移动拷贝数据，可能会扩充内存大小。",
				"param": [
					{
						"type": "integer",
						"name": "src",
						"text": "源内存位置"
					},
					{
						"type": "integer",
						"name": "dst",
						"text": "目的内存位置"
					},
					{
						"type": "integer",
						"name": "length",
						"text": "拷贝数据的大小，这个值不能大于当前内存的有效大小（末尾到源位置长度）。"
					}
				],
				"return": {
					"type": "integer",
					"text": "失败返回 undefined，成功返回实际移动内存的长度"
				},
				"remark": [
					"移动内存和拷贝内存不同，拷贝内存不能重叠，如果源内存和目的内存有重叠，可能导致数据错误，这种时候需要使用移动内存方式。"
				]
			},
			{
				"type": "function",
				"name": "setNumber(number,type,[start])",
				"text": "C语言常用的数字类型有 char、short、int32、float、double、int64 以及它们对应的无符号类型，但是 JavaScript 只有一种数字类型，所以需要在内存中保存对应的数字类型。这个函数设置一段内存表示对应类型的数字。",
				"param": [
					{
						"type": "number",
						"name": "number",
						"text": "要设置的数值"
					},
					{
						"type": "integer|string",
						"name": "type",
						"text": "内存数据的类型，有以下 12 种：char（1字节，用 0 表示）、short（2字节，用 1 表示）、int32（4字节，用 2 表示）、float（4字节，用 3 表示）、double（8字节，用 4 表示）、int64（8字节，用 5 表示） 以及它们对应的无符号类型（代码和有符号类型相同）。"
					},
					{
						"type": "integer",
						"name": "start",
						"text": "保存位置"
					}
				],
				"return": {
					"type": "bool",
					"text": "成功返回 true，如果内存分配失败、类型参数错误、对象为空，返回 undefined。"
				},
				"remark": [
					"JavaScript 并不能表示64位整数的最大值, JavaScript的有效范围是 正负0x100000000000000之间.",
					"如果内存不够，会自动调整大小",
					"有符号和无符号数字在内存中的数据是相同的，所以不需要指定数字的符号。"
				]
			},
			{
				"type": "function",
				"name": "setString(str,[start],[code])",
				"text": "把字串按指定编码保存到内存的指定位置，内存可能会被扩充。",
				"param": [
					{
						"type": "string",
						"name": "str",
						"text": "要保存的字串"
					},
					{
						"type": "integer",
						"name": "start",
						"text": "要保存的内存位置，缺省是 0 。"
					},
					{
						"type": "integer|string",
						"name": "code",
						"text": "字符串编码，可以是数字或者字串名称，如果这个参数缺省或者为 0 （或者1200），表示 Unicode 编码，如果为 3 ，表示 ansi 编码。函数使用数字指定编码比使用字串要快。"
					}
				],
				"return": {
					"type": "integer",
					"text": "失败返回 undefined，成功返回数据的字节长度，字串保存到内存总是包含一个 NULL 结尾字符，所以即使空字串，返回值也不会是 0 。"
				},
				"remark": [
					"保存字串总是包含一个结尾的 NULL 字符。"
				]
			},
			{
				"type": "function",
				"name": "toHex([start],[length])",
				"text": "把指定位置和长度的内存用16进制字串表示，返回这个字串。",
				"param": [
					{
						"type": "integer",
						"name": "start",
						"text": "要转换的内存位置，缺省是 0 。"
					},
					{
						"type": "integer",
						"name": "length",
						"text": "要转换的内存大小，缺省是 -1，也就是到内存尾部。"
					}
				],
				"return": {
					"type": "string",
					"text": "返回一个16进制字串，如果内存对象已经disposed，返回 undefined 。"
				}
			},
			{
				"type": "function",
				"name": "zero([offset],[length])",
				"text": "把内存数据置 0 ",
				"param": [
					{
						"type": "integer",
						"name": "offset",
						"text": "要置零的内存位置，缺省是 0 。"
					},
					{
						"type": "integer",
						"name": "length",
						"text": "要置零的内存大小，缺省是 -1，也就是到内存尾部。"
					}
				],
				"return": {
					"type": "integer",
					"text": "返回实际被置零的内存长度，如果内存对象已经disposed，返回 undefined 。"
				}
			}
		]
	},
	{
		"type": "group",
		"name": "Timer",
		"text": "定时器相关函数。",
		"member": [
			{
				"type": "function",
				"name": "killTimer(id)",
				"text": "停止一个计时器。",
				"param": [
					{
						"type": "integer",
						"name": "id",
						"text": "要删除的计时器 id，这个值是 setTimer 函数返回的。"
					}
				],
				"return": {
					"type": "bool",
					"text": "失败返回 undefined，成功返回 true。"
				}
			},
			{
				"type": "function",
				"name": "setTimer(ms,callback)",
				"text": "设置一个周期性调用的计时器，callback 是一个函数，这个函数每 ms 毫秒被呼叫一次。",
				"param": [
					{
						"type": "integer",
						"name": "ms",
						"text": "计时器的周期，单位是毫秒。"
					},
					{
						"type": "function",
						"name": "callback",
						"text": "计时器回调函数。"
					}
				],
				"return": {
					"type": "integer",
					"text": "失败返回 undefined，成功返回一个标识这个计时器的整数id，每个计时器都有自己独立的id，这个id可以用于删除定时器的参数。"
				},
				"remark": [
					"如果需要周期性调用的函数有参数，可以定义一个新函数，设置为 callback，然后在新函数里调用具有参数的函数。"
				]
			}
		]
	}
]