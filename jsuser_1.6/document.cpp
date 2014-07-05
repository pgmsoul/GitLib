#include "stdafx.h"

//*{
//	"type":"const",
//	"name":"Stack",
//	"text":"Stack 提供了一个函数退出时执行清理操作的机制。如果使用 Class 封装函数，无需手动处理 Stack 操作。",
//	"member":[//*
	//*{
	//	"type":"function",
	//	"name":"create()",
	//	"text":"生成函数当前的 Stack。",
	//	"param":[
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"函数没有返回值。"
	//	}
	//}//*
	//*,{
	//	"type":"function",
	//	"name":"close()",
	//	"text":"调用 Stack 内对象的 dispose 函数，并且清理 Stack。",
	//	"param":[
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"函数没有返回值。"
	//	}
	//}//*
	//*,{
	//	"type":"function",
	//	"name":"push(obj)",
	//	"text":"把对象 push 进 Stack，在函数退出时，对象的 dispose 函数被调用，后添加对象的 dispose 先于先添加对象的 dispose 被调用。",
	//	"param":[
	//	{
	//		"type":"object",
	//		"name":"obj",
	//		"text":"一个对象变量。"
	//	}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"函数没有返回值。"
	//	}
	//}//*
	//*,{
	//	"type":"function",
	//	"name":"pop(obj)",
	//	"text":"把对象 pop 出 Stack。这个函数是 push 函数的逆函数。Stack 对象的 push 和 pop 顺序无关紧要，用户很少需要调用这个函数。",
	//	"param":[
	//	{
	//		"type":"object",
	//		"name":"obj",
	//		"text":"一个对象变量。"
	//	}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"函数没有返回值。"
	//	}
	//}],//*
//*	"example":[
//		"function testStack(){",
//		"	//在函数开头初始化，之后必须调用 Stack.close()，否则 Stack 机制会被破坏。",
//		"	Stack.create();",
//		"	//任何一个对象，只要被 push 进 Stack，函数退出时就会被调用 dispose 函数，所以对象的任何清理操作写在 dispose 函数中即可。",
//		"	var obj = {};",
//		"	obj.dispose = function(){",
//		"		alert(\"dispose() called\");",
//		"	}",
//		"	//把 obj push 进 Stack。",
//		"	Stack.push(obj);",
//		"	//CFile 类对象自动 push 无需显示调用 Stack.push()，而且 C 开头的对象内部绑定了 C++ 对象，凡是使用 C 类对象的程序，必须初始化一个 Stack。",
//		"	var file = new CFile();",
//		"	//在函数退出时调用，清理 push 进 Stack 内的对象，如果函数有多个出口，必须在每个出口处调用 close。",
//		"	Stack.close();",
//		"}",
//		"testStack();",
//		"//如果使用 Class 封装函数，无需显示调用 Stack。;",
//		"Class(function testStack(){",
//		"	//任何一个对象，只要被 push 进 Stack，函数退出时就会被调用 dispose 函数，所以对象的任何清理操作写在 dispose 函数中即可。",
//		"	var obj = {};",
//		"	obj.dispose = function(){",
//		"		alert(\"dispose() called\");",
//		"	}",
//		"	//把 obj push 进 Stack。",
//		"	Stack.push(obj);",
//		"	//CFile 类对象自动 push 无需显示调用 Stack.push()，而且 C 开头的对象内部绑定了 C++ 对象，凡是使用 C 类对象的程序，必须初始化一个 Stack。",
//		"	var file = new CFile();",
//		"},true)",
//		"testStack();"
//	]
//}//*