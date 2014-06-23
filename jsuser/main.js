"use strict";

require("base");
require("wnd");

Class(function main(){
	testWnd();
	//testImage();
},true);
function testWnd(){
	var frame = new CFrame();
	var param = {"text":"title"};
	frame.setRemPos("main");
	frame.create(param);
	var icon = new CIcon();
	icon.load("app.ico");
	frame.setIcon(icon);
	icon.dispose();
	startMsgLoop();
}
function testImage(){
	Console.show();
	var img = new CImageLoader();
	if(!img.load("girl.gif")){
		Console.log("load gif failed");
		Console.readln();
		return;
	}
	var bmp = new CBitmap();
	Console.log(bmp);
	var count = img.count();
	for(var i=0;i<count;i++){
		var r = img.getBitmap(bmp,i);
		bmp.save(""+i+".jpg");
		Console.log("load frame " + i + ", result: " + r);
	}
	Console.readln();
}
function testFile(){
	var file = new CFile();
	file.create("abc.txt");
	file.length = "";
	return;
	/*var http = new CHttpRequest();
	//http.parseUrl("http://tieba.baidu.com/f?kw=%B3%E8%CE%EF%B6%D4%D5%BD&fr=home");
	http.parseUrl("http://localhost/test.php");
	http.onRequest = function(html,head){
		//alert(html);
		alert(head);
		exitMsgLoop();
	}
	if(http.request())
		startMsgLoop();*/
	var src = "D:\\SoftProject\\Chrome";
	var dst = "D:\\SoftProject\\Chrome2";
	FileSystem.delete(dst,
		function(inf){
			for(var i in inf){
				debug("%s=%s",i,inf[i]);
				if(inf.file=="google.user.js") inf.skip = true;
			}
		},
		function(inf){
			debug("copy progress:%s/%s",inf.offset,inf.length);
		});
}
main();