[
	{
		"type": "class",
		"name": "CMemory",
		"text": "�ڴ���һ���ǳ���Ҫ�ĸ����Ϊ�κ����ݣ��������ֺ��ִ�����ռ��һ�����ڴ�ռ䣬Ҳ����˵���κ����ݶ����ڴ棬�����ڴ�Ĳ����ǳ���Ҫ��",
		"member": [
			{
				"type": "function",
				"name": "bind(pointer,size)",
				"text": "��ָ��󶨵��ڴ���󣨴Ӷ����Զ��������ڴ������",
				"param": [
					{
						"type": "integer",
						"name": "pointer",
						"text": "һ���ڴ�ָ�루C++�κζ���������ݵ�ָ�붼���ڴ�ָ�룩"
					},
					{
						"type": "integer",
						"name": "size",
						"text": "����ڴ�ָ��Ĵ�С����������ָ��Ĵ�С�� 4 ��double ָ��Ĵ�С�� 8�������С�ɾ��������������"
					}
				],
				"return": {
					"type": "bool",
					"text": "ָʾ���Ƿ�ɹ��������ǰ������ڴ�����Ѿ��ͷţ����� undefined ��"
				},
				"remark": [
					"�����͵��ڴ治�ܵ�����С��������Ҫ������С�ĺ�������ʧ�ܡ������ʹ�� close ����"
				]
			},
			{
				"type": "function",
				"name": "close()",
				"text": "�ͷ��ڴ�ռ���߽���󶨵��ڴ�ָ�롣",
				"return": {
					"type": "bool",
					"text": "�˺������Ƿ��� true�����Ƕ����ڲ����ڴ�����Ѿ��ͷţ�dispose������ʱ���� undefined��"
				}
			},
			{
				"type": "function",
				"name": "copyFrom(mem,length,[offset])",
				"text": "�����ݴ� mem ��������ǰ�ڴ�� offset λ��",
				"param": [
					{
						"type": "integer",
						"name": "mem",
						"text": "Դ�ڴ�"
					},
					{
						"type": "integer",
						"name": "length",
						"text": "Ҫ�����ĳ��ȣ��˳��Ȳ��ܴ�����Ч���ȣ�ƫ��λ�õ�ĩβ�ľ��룩"
					},
					{
						"type": "integer",
						"name": "offset",
						"text": "��ǰ�ڴ�Ŀ���λ�ã�ȱʡ�� 0 ����ͷλ��"
					}
				],
				"return": {
					"type": "integer",
					"text": "ʧ�ܷ��� undefined���ɹ�����ʵ�ʿ������ڴ泤��"
				},
				"remark": [
					"�ڴ����ʹ�õ���MoveMemory������Դ�ڴ��Ŀ���ڴ�����ص���"
				]
			},
			{
				"type": "function",
				"name": "copyTo(mem,length,[offset])",
				"text": "�����ݴӵ�ǰ�ڴ�� offset λ�ÿ�����Ŀ���ڴ� mem",
				"param": [
					{
						"type": "integer",
						"name": "mem",
						"text": "Ŀ���ڴ�"
					},
					{
						"type": "integer",
						"name": "length",
						"text": "Ҫ�����ĳ��ȣ��˳��Ȳ��ܴ�����Ч���ȣ�ƫ��λ�õ�ĩβ�ľ��룩"
					},
					{
						"type": "integer",
						"name": "offset",
						"text": "��ǰ�ڴ�Ŀ���λ�ã�ȱʡ�� 0 ����ͷλ��"
					}
				],
				"return": {
					"type": "integer",
					"text": "ʧ�ܷ��� undefined���ɹ�����ʵ�ʿ������ڴ泤��"
				},
				"remark": [
					"�ڴ����ʹ�õ���MoveMemory������Դ�ڴ��Ŀ���ڴ�����ص���"
				]
			},
			{
				"type": "function",
				"name": "double([minSize])",
				"text": "���ڴ�����˫�������ǲ���ԭ���������Ƕ��٣�����minSize����Ϊ���� ��˫����������������С�� 0x20",
				"param": [
					{
						"type": "integer",
						"name": "minSize",
						"text": "���ٵ�����������ڴ�����˫���󣬲��ܴﵽ�����������ᱻ����Ϊ���������"
					}
				],
				"return": {
					"type": "bool",
					"text": "���ز����Ƿ�ɹ������ϵͳ�ڴ治�����������ܻ�ʧ�ܣ�ʧ�ܷ��� undefined���ɹ����� true"
				}
			},
			{
				"type": "function",
				"name": "fromHex(hex,[start])",
				"text": "��16�����ִ�ת��Ϊ���������ݣ�������䵽ָ��λ�ã��ڴ���ܱ����䡣",
				"param": [
					{
						"type": "string",
						"name": "hex",
						"text": "16�����ִ�(���ܰ���0x��ͷ)������ִ����з�16�����ַ����ᱻ�޳���ת����Ȼ��ɹ���"
					},
					{
						"type": "integer",
						"name": "start",
						"text": "���ݱ�����λ�ã�ȱʡ�� 0 �����Դ���ݱȽϴ󣬳����ڴ���������������Զ�����"
					}
				],
				"return": {
					"type": "number",
					"text": "����ת���Ķ��������ݳ��ȡ�"
				}
			},
			{
				"type": "function",
				"name": "getNumber(type,[sign],[start])",
				"text": "C���Գ��õ����������� char��short��int32��float��double��int64 �Լ����Ƕ�Ӧ���޷������ͣ����� JavaScript ֻ��һ���������ͣ�������Ҫ���ڴ��б����Ӧ���������͡����������ȡһ���ڴ��ʾ��Ӧ���͵����֡�",
				"param": [
					{
						"type": "integer|string",
						"name": "type",
						"text": "�ڴ����ݵ����ͣ������� 12 �֣�char��1�ֽڣ��� 0 ��ʾ����short��2�ֽڣ��� 1 ��ʾ����int32��4�ֽڣ��� 2 ��ʾ����float��4�ֽڣ��� 3 ��ʾ����double��8�ֽڣ��� 4 ��ʾ����int64��8�ֽڣ��� 5 ��ʾ�� �Լ����Ƕ�Ӧ���޷������ͣ�������з���������ͬ����"
					},
					{
						"type": "bool",
						"name": "sign",
						"text": "���ڴ濴���з������֣������޷������֣�ȱʡ��ʾ�з������֡�����-127��129��-1��255����Щ���ֽ��������ڴ���ʵ��������ͬ�ġ���������� double �� float ������Ч����Ϊ���������з��ŵġ�"
					},
					{
						"type": "integer",
						"name": "type",
						"text": "��ȡ���ڴ�λ��ƫ��"
					}
				],
				"return": {
					"type": "number",
					"text": "����ת��������֣������ʼλ�õ��ڴ�Ľ�β���Ȳ���һ��ָ�����͵Ĵ�С������ undefined ��"
				},
				"remark": [
					"JavaScript �����ܱ�ʾ64λ���������ֵ, JavaScript����Ч��Χ�� ����0x100000000000000֮��."
				]
			},
			{
				"type": "function",
				"name": "getString([start],[length],[code])",
				"text": "���ڴ�ת��Ϊ�ִ�",
				"param": [
					{
						"type": "integer",
						"name": "start",
						"text": "�ڴ����ݵ���ʼλ�ã�ȱʡ�� 0 ��"
					},
					{
						"type": "integer",
						"name": "length",
						"text": "�ڴ����ݵĳ��ȣ�ȱʡ�� -1����ʾ���ڴ�Ľ�β����������� NULL �ַ����ִ���ضϡ�"
					},
					{
						"type": "integer|string",
						"name": "code",
						"text": "�ַ������룬���������ֻ����ִ����ƣ����������������ݵı��벻������������롣����������ȱʡ����Ϊ 0 ����ʾ Unicode ���룬���Ϊ 3 ����ʾ ansi ���롣"
					}
				],
				"return": {
					"type": "string",
					"text": "����һ���ִ�"
				}
			},
			{
				"type": "property",
				"name": "length",
				"text": "��ȡ�����ڴ�Ĵ�С�����ڰ����͵��ڴ棬������Ч"
			},
			{
				"type": "function",
				"name": "move(src,dst,length)",
				"text": "�ڵ�ǰ�ڴ��ƶ��������ݣ����ܻ������ڴ��С��",
				"param": [
					{
						"type": "integer",
						"name": "src",
						"text": "Դ�ڴ�λ��"
					},
					{
						"type": "integer",
						"name": "dst",
						"text": "Ŀ���ڴ�λ��"
					},
					{
						"type": "integer",
						"name": "length",
						"text": "�������ݵĴ�С�����ֵ���ܴ��ڵ�ǰ�ڴ����Ч��С��ĩβ��Դλ�ó��ȣ���"
					}
				],
				"return": {
					"type": "integer",
					"text": "ʧ�ܷ��� undefined���ɹ�����ʵ���ƶ��ڴ�ĳ���"
				},
				"remark": [
					"�ƶ��ڴ�Ϳ����ڴ治ͬ�������ڴ治���ص������Դ�ڴ��Ŀ���ڴ����ص������ܵ������ݴ�������ʱ����Ҫʹ���ƶ��ڴ淽ʽ��"
				]
			},
			{
				"type": "function",
				"name": "setNumber(number,type,[start])",
				"text": "C���Գ��õ����������� char��short��int32��float��double��int64 �Լ����Ƕ�Ӧ���޷������ͣ����� JavaScript ֻ��һ���������ͣ�������Ҫ���ڴ��б����Ӧ���������͡������������һ���ڴ��ʾ��Ӧ���͵����֡�",
				"param": [
					{
						"type": "number",
						"name": "number",
						"text": "Ҫ���õ���ֵ"
					},
					{
						"type": "integer|string",
						"name": "type",
						"text": "�ڴ����ݵ����ͣ������� 12 �֣�char��1�ֽڣ��� 0 ��ʾ����short��2�ֽڣ��� 1 ��ʾ����int32��4�ֽڣ��� 2 ��ʾ����float��4�ֽڣ��� 3 ��ʾ����double��8�ֽڣ��� 4 ��ʾ����int64��8�ֽڣ��� 5 ��ʾ�� �Լ����Ƕ�Ӧ���޷������ͣ�������з���������ͬ����"
					},
					{
						"type": "integer",
						"name": "start",
						"text": "����λ��"
					}
				],
				"return": {
					"type": "bool",
					"text": "�ɹ����� true������ڴ����ʧ�ܡ����Ͳ������󡢶���Ϊ�գ����� undefined��"
				},
				"remark": [
					"JavaScript �����ܱ�ʾ64λ���������ֵ, JavaScript����Ч��Χ�� ����0x100000000000000֮��.",
					"����ڴ治�������Զ�������С",
					"�з��ź��޷����������ڴ��е���������ͬ�ģ����Բ���Ҫָ�����ֵķ��š�"
				]
			},
			{
				"type": "function",
				"name": "setString(str,[start],[code])",
				"text": "���ִ���ָ�����뱣�浽�ڴ��ָ��λ�ã��ڴ���ܻᱻ���䡣",
				"param": [
					{
						"type": "string",
						"name": "str",
						"text": "Ҫ������ִ�"
					},
					{
						"type": "integer",
						"name": "start",
						"text": "Ҫ������ڴ�λ�ã�ȱʡ�� 0 ��"
					},
					{
						"type": "integer|string",
						"name": "code",
						"text": "�ַ������룬���������ֻ����ִ����ƣ�����������ȱʡ����Ϊ 0 ������1200������ʾ Unicode ���룬���Ϊ 3 ����ʾ ansi ���롣����ʹ������ָ�������ʹ���ִ�Ҫ�졣"
					}
				],
				"return": {
					"type": "integer",
					"text": "ʧ�ܷ��� undefined���ɹ��������ݵ��ֽڳ��ȣ��ִ����浽�ڴ����ǰ���һ�� NULL ��β�ַ������Լ�ʹ���ִ�������ֵҲ������ 0 ��"
				},
				"remark": [
					"�����ִ����ǰ���һ����β�� NULL �ַ���"
				]
			},
			{
				"type": "function",
				"name": "toHex([start],[length])",
				"text": "��ָ��λ�úͳ��ȵ��ڴ���16�����ִ���ʾ����������ִ���",
				"param": [
					{
						"type": "integer",
						"name": "start",
						"text": "Ҫת�����ڴ�λ�ã�ȱʡ�� 0 ��"
					},
					{
						"type": "integer",
						"name": "length",
						"text": "Ҫת�����ڴ��С��ȱʡ�� -1��Ҳ���ǵ��ڴ�β����"
					}
				],
				"return": {
					"type": "string",
					"text": "����һ��16�����ִ�������ڴ�����Ѿ�disposed������ undefined ��"
				}
			},
			{
				"type": "function",
				"name": "zero([offset],[length])",
				"text": "���ڴ������� 0 ",
				"param": [
					{
						"type": "integer",
						"name": "offset",
						"text": "Ҫ������ڴ�λ�ã�ȱʡ�� 0 ��"
					},
					{
						"type": "integer",
						"name": "length",
						"text": "Ҫ������ڴ��С��ȱʡ�� -1��Ҳ���ǵ��ڴ�β����"
					}
				],
				"return": {
					"type": "integer",
					"text": "����ʵ�ʱ�������ڴ泤�ȣ�����ڴ�����Ѿ�disposed������ undefined ��"
				}
			}
		]
	},
	{
		"type": "group",
		"name": "Timer",
		"text": "��ʱ����غ�����",
		"member": [
			{
				"type": "function",
				"name": "killTimer(id)",
				"text": "ֹͣһ����ʱ����",
				"param": [
					{
						"type": "integer",
						"name": "id",
						"text": "Ҫɾ���ļ�ʱ�� id�����ֵ�� setTimer �������صġ�"
					}
				],
				"return": {
					"type": "bool",
					"text": "ʧ�ܷ��� undefined���ɹ����� true��"
				}
			},
			{
				"type": "function",
				"name": "setTimer(ms,callback)",
				"text": "����һ�������Ե��õļ�ʱ����callback ��һ���������������ÿ ms ���뱻����һ�Ρ�",
				"param": [
					{
						"type": "integer",
						"name": "ms",
						"text": "��ʱ�������ڣ���λ�Ǻ��롣"
					},
					{
						"type": "function",
						"name": "callback",
						"text": "��ʱ���ص�������"
					}
				],
				"return": {
					"type": "integer",
					"text": "ʧ�ܷ��� undefined���ɹ�����һ����ʶ�����ʱ��������id��ÿ����ʱ�������Լ�������id�����id��������ɾ����ʱ���Ĳ�����"
				},
				"remark": [
					"�����Ҫ�����Ե��õĺ����в��������Զ���һ���º���������Ϊ callback��Ȼ�����º�������þ��в����ĺ�����"
				]
			}
		]
	}
]