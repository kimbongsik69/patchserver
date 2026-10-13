//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <IniFiles.hpp>
#include "SecurityThread.h"
#include "UnitServer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "IdCustomTCPServer"
#pragma resource "*.dfm"
TFormServer *FormServer;

#define Log(p)        LogFunc(p, "Patchserver.log", __FUNC__, __LINE__)
//---------------------------------------------------------------------------
__fastcall TFormServer::TFormServer(TComponent* Owner)
	: TForm(Owner)
{
    m_pLogStrList = new TStringList;
	Log("패치서버 시작");
}
//---------------------------------------------------------------------------
__fastcall TFormServer::~TFormServer()
{
	delete m_pLogStrList;
	ServerSocket->Close();
}

//---------------------------------------------------------------------------
void __fastcall TFormServer::ServerSocketClientError(TObject *Sender,
	  TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
	SetSocketStatus("["+ AnsiString(Socket->SocketHandle) + "] 소켓 에러 ["+AnsiString(Socket->RemoteAddress) +":"+AnsiString(ErrorCode)+"]");
	Socket->Close();
	ErrorCode = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormServer::SetSocketStatus(AnsiString msg)
{
	msg = "- "+msg;
	msg = StringReplace(msg, "\r", "", TReplaceFlags() << rfIgnoreCase << rfReplaceAll);
	msg = StringReplace(msg, "\n", "\r\n", TReplaceFlags() << rfIgnoreCase << rfReplaceAll);

	TTime t = Now();

	Memo1->Lines->Insert(0, 	t.TimeString() + " " + msg);
}
//---------------------------------------------------------------------------
void __fastcall TFormServer::ServerSocketGetThread(TObject *Sender,
	  TServerClientWinSocket *ClientSocket, TServerClientThread *&SocketThread)
{
	Log("쓰레드 생성");
	SocketThread = new TSecurityThread(false, ClientSocket);
}
//---------------------------------------------------------------------------
void __fastcall TFormServer::Button1Click(TObject *Sender)
{
	//Header
	BT_HEADER out_hdr;
	memset(&out_hdr, 0, SZ_BT_HEADER);
	out_hdr.id = 0xf1f2;
	out_hdr.gubun = ID_BT_PACKET;
	out_hdr.tr_no = 40231;
	out_hdr.total_size = SZ_BT_HEADER + SZ_BT_FOOTER + SZ_BT_40231;
	out_hdr.body_size = SZ_BT_40231;
	out_hdr.repeat_size = 0;
	out_hdr.repeat_count = 0;

	//Footer
	BT_FOOTER out_ftr;
	out_ftr.ff = 0xff;
	out_ftr.cr = 0x0d;
	out_ftr.lf = 0x0a;

	//body
	BT_40231 out;
	memset(&out, 0, SZ_BT_40231);

	out.err_no = 0;
	strcpy(out.content, "반갑습니다.");

	int total_size = SZ_BT_HEADER + SZ_BT_40231 + SZ_BT_FOOTER;
	char *send_buf = new char[total_size];

	int pos = 0;

	memcpy(send_buf + pos, &out_hdr, SZ_BT_HEADER);
	pos += SZ_BT_HEADER;

	memcpy(send_buf + pos, &out, SZ_BT_40231);
	pos += SZ_BT_40231;

	memcpy(send_buf + pos, &out_ftr, SZ_BT_FOOTER);

	for(int i = 0; i < ServerSocket->Socket->ActiveConnections; i++)
    {
		ServerSocket->Socket->Connections[i]->SendBuf(send_buf, total_size);
	}

	delete []send_buf;
}
//---------------------------------------------------------------------------
void __fastcall TFormServer::Timer1Timer(TObject *Sender)
{
	Label1->Caption = IntToStr(ServerSocket->Socket->ActiveConnections);
}
//---------------------------------------------------------------------------

void __fastcall TFormServer::ServerSocketThreadEnd(TObject *Sender,
      TServerClientThread *Thread)
{
	Thread->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TFormServer::Timer2Timer(TObject *Sender)
{
	if (m_pLogStrList->Count != 0)
    {
		Memo1->Lines->Add(m_pLogStrList->Strings[0]);
    	m_pLogStrList->Delete(0);
	}
}
//---------------------------------------------------------------------------



void __fastcall TFormServer::FormShow(TObject *Sender)
{
    TIniFile *ini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
	ServerSocket->Port = ini->ReadInteger("Main", "port", 14000);

	Label2->Caption = "서버포트 " + IntToStr(ServerSocket->Port);
    delete ini;

	ServerSocket->Open();
}
//---------------------------------------------------------------------------
void __fastcall TFormServer::LogFunc(AnsiString msg, AnsiString filename, char *func, int line)
{
	AnsiString fn = ExtractFilePath(ParamStr(0))+ filename;
	FILE *fp = fopen(fn.c_str(), "at");
	if (fp) {
		fprintf(fp, "-------------------------------------------------------------------\n");
		fprintf(fp, "%s %-30s %5d %0x\n", Now().DateTimeString().c_str(), func, line, this);
		fprintf(fp, "%s\n", msg.c_str());
		fclose(fp);
	}
}

void __fastcall TFormServer::FormClose(TObject *Sender, TCloseAction &Action)
{
	Log("패치서버 종료");
}
//---------------------------------------------------------------------------


void __fastcall TFormServer::FormCreate(TObject *Sender)
{
//    LogCS = new TCriticalSection();
//	LogFP = NULL;
//	CurrentLogDate = "";
}
//---------------------------------------------------------------------------

void __fastcall TFormServer::FormDestroy(TObject *Sender)
{
//    LogCS->Enter();
//	try
//	{
//		if (LogFP) { fclose(LogFP); LogFP = NULL; }
//	}
//	__finally { LogCS->Leave(); }
//	delete LogCS;
}
//---------------------------------------------------------------------------





