/*
类型 ID, 可以通过 GetEnv()返回的全局Env指针调用 
GetTemplate或GetFunc函数, 获取指定ID的对象或函数模版
*/
#pragma once
namespace v8{
	//内部函数
#define FUNC_ID_POINT		0
#define FUNC_ID_SIZE		1
#define FUNC_ID_RECT		2
#define FUNC_ID_COLOR		3
#define FUNC_ID_STACK_PUSH	4
#define FUNC_ID_VIEWPARAM	5
#define FUNC_ID_LAYOUTPARAM	6
#define FUNC_ID_STACK_POP	7
#define FUNC_ID_LAYOUT		8
#define FUNC_ID_TEXTDRAW	9
#define FUNC_ID_MOUSE		10
#define FUNC_ID_MESSAGE		11
#define FUNC_ID_STACK_DISPOSE	12

#define FUNC_ID_MAX			16
	//GDI
#define TEMPLATE_ID_PEN		0
#define TEMPLATE_ID_BRUSH	1
#define TEMPLATE_ID_RGN		2
#define TEMPLATE_ID_FONT	3
#define TEMPLATE_ID_API_CALLBACK	4
#define TEMPLATE_ID_BITMAP	5
#define TEMPLATE_ID_IMAGE	6
#define TEMPLATE_ID_ICON	7
	//窗口
#define TEMPLATE_ID_WND		8
#define TEMPLATE_ID_VIEW	9		
#define TEMPLATE_ID_FRAME	10
#define TEMPLATE_ID_CTRL	11
	//内核
#define TEMPLATE_ID_MEMORY	13
	//API
#define TEMPLATE_ID_API		14
	//文件
#define TEMPLATE_ID_FILE	15
#define TEMPLATE_ID_FILELIST	16
	//控件
#define TEMPLATE_ID_IMAGELIST	17
#define TEMPLATE_ID_LABEL		18
#define TEMPLATE_ID_BUTTON		19
#define TEMPLATE_ID_GROUPBOX	20
#define TEMPLATE_ID_CHECKBOX	21
#define TEMPLATE_ID_RADIO		22
#define TEMPLATE_ID_EDIT		23
#define TEMPLATE_ID_DATETIMEPICKER	24
#define TEMPLATE_ID_MONTHCALENDAR	25
#define TEMPLATE_ID_LISTBOX			26
#define TEMPLATE_ID_COMBOBOX		27
#define TEMPLATE_ID_COMBOBOXEX		28
#define TEMPLATE_ID_TREEVIEW		29
#define TEMPLATE_ID_TOOLBAR			30
#define TEMPLATE_ID_PROGRESSBAR		31
#define TEMPLATE_ID_TOOLTIP			32
#define TEMPLATE_ID_TABPAGE			33
#define TEMPLATE_ID_HOTKEY			34
#define TEMPLATE_ID_LISTVIEW		35
#define TEMPLATE_ID_STATUSBAR		36
#define TEMPLATE_ID_TRACKBAR		37
#define TEMPLATE_ID_UPDOWN			38
#define TEMPLATE_ID_HEADER			39
#define TEMPLATE_ID_LINK			40
#define TEMPLATE_ID_IPADDRESS		41
#define TEMPLATE_ID_TOPLAYOUT		43
#define TEMPLATE_ID_DRAGLAYOUT		44
#define TEMPLATE_ID_BINDER			46
#define TEMPLATE_ID_MENU			47
#define TEMPLATE_ID_CONFIG			48
#define TEMPLATE_ID_REGISTRY		49
#define TEMPLATE_ID_IMAGELOADER		50
#define TEMPLATE_ID_THREADMSG		51
#define TEMPLATE_ID_TCPLISTENER		52
#define TEMPLATE_ID_TCPTRANSFER		53
#define TEMPLATE_ID_TIMER			54
#define TEMPLATE_ID_UDPSENDER		55
#define TEMPLATE_ID_UDPRECEIVER		56
#define TEMPLATE_ID_IP				57
#define TEMPLATE_ID_HOST			58
#define TEMPLATE_ID_HTTPREQUEST		59
#define TEMPLATE_ID_HTTPHEADER		60
#define TEMPLATE_ID_FONTDIALOG		61
#define TEMPLATE_ID_COLORDIALOG		62
#define TEMPLATE_ID_FILEDIALOG		63
#define TEMPLATE_ID_MYSQL			64

#define TEMPLATE_ID_HTTP			65
#define TEMPLATE_ID_SOCKET			66
#define TEMPLATE_ID_TCPSERVER		67
#define TEMPLATE_ID_UPNP			68
#define TEMPLATE_ID_TEST			69
#define TEMPLATE_ID_

#define TEMPLATE_ID_MAX		128
	/*
	Label
	Button
	GroupBox
	CheckBox
	Radio
	Edit
	DateTimePicker
	MonthCalendar
	ListBox
	ComboBox
	ComboBoxEx
	TreeView
	ToolBar
	ProgressBar
	ToolTip
	TabPage
	HotKey
	ListView
	StatusBar
	TrackBar
	UpDown
	Header
	Link
	IpAddress
	*/
}