#include "stdafx.h"

//*{
//	"type":"const",
//	"name":"Stack",
//	"text":"Stack �ṩ��һ�������˳�ʱִ����������Ļ��ơ����ʹ�� Class ��װ�����������ֶ����� Stack ������",
//	"member":[//*
	//*{
	//	"type":"function",
	//	"name":"create()",
	//	"text":"���ɺ�����ǰ�� Stack��",
	//	"param":[
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"����û�з���ֵ��"
	//	}
	//}//*
	//*,{
	//	"type":"function",
	//	"name":"close()",
	//	"text":"���� Stack �ڶ���� dispose �������������� Stack��",
	//	"param":[
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"����û�з���ֵ��"
	//	}
	//}//*
	//*,{
	//	"type":"function",
	//	"name":"push(obj)",
	//	"text":"�Ѷ��� push �� Stack���ں����˳�ʱ������� dispose ���������ã�����Ӷ���� dispose ��������Ӷ���� dispose �����á�",
	//	"param":[
	//	{
	//		"type":"object",
	//		"name":"obj",
	//		"text":"һ�����������"
	//	}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"����û�з���ֵ��"
	//	}
	//}//*
	//*,{
	//	"type":"function",
	//	"name":"pop(obj)",
	//	"text":"�Ѷ��� pop �� Stack����������� push �������溯����Stack ����� push �� pop ˳���޹ؽ�Ҫ���û�������Ҫ�������������",
	//	"param":[
	//	{
	//		"type":"object",
	//		"name":"obj",
	//		"text":"һ�����������"
	//	}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"����û�з���ֵ��"
	//	}
	//}],//*
//*	"example":[
//		"function testStack(){",
//		"	//�ں�����ͷ��ʼ����֮�������� Stack.close()������ Stack ���ƻᱻ�ƻ���",
//		"	Stack.create();",
//		"	//�κ�һ������ֻҪ�� push �� Stack�������˳�ʱ�ͻᱻ���� dispose ���������Զ�����κ��������д�� dispose �����м��ɡ�",
//		"	var obj = {};",
//		"	obj.dispose = function(){",
//		"		alert(\"dispose() called\");",
//		"	}",
//		"	//�� obj push �� Stack��",
//		"	Stack.push(obj);",
//		"	//CFile ������Զ� push ������ʾ���� Stack.push()������ C ��ͷ�Ķ����ڲ����� C++ ���󣬷���ʹ�� C �����ĳ��򣬱����ʼ��һ�� Stack��",
//		"	var file = new CFile();",
//		"	//�ں����˳�ʱ���ã����� push �� Stack �ڵĶ�����������ж�����ڣ�������ÿ�����ڴ����� close��",
//		"	Stack.close();",
//		"}",
//		"testStack();",
//		"//���ʹ�� Class ��װ������������ʾ���� Stack��;",
//		"Class(function testStack(){",
//		"	//�κ�һ������ֻҪ�� push �� Stack�������˳�ʱ�ͻᱻ���� dispose ���������Զ�����κ��������д�� dispose �����м��ɡ�",
//		"	var obj = {};",
//		"	obj.dispose = function(){",
//		"		alert(\"dispose() called\");",
//		"	}",
//		"	//�� obj push �� Stack��",
//		"	Stack.push(obj);",
//		"	//CFile ������Զ� push ������ʾ���� Stack.push()������ C ��ͷ�Ķ����ڲ����� C++ ���󣬷���ʹ�� C �����ĳ��򣬱����ʼ��һ�� Stack��",
//		"	var file = new CFile();",
//		"},true)",
//		"testStack();"
//	]
//}//*