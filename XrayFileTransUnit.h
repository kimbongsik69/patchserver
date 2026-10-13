//---------------------------------------------------------------------------
#ifndef XrayFileTransUnitH
#define XrayFileTransUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Forms.hpp>
#include <ScktComp.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
#include <stdio>
#include <vector>
#include "bt_com_packet.h"
//---------------------------------------------------------------------------
const PATCH_MSG_CONNECT     = WM_APP + 2000;
const PATCH_MSG_DISCONNECT  = WM_APP + 2001;
const PATCH_MSG_RCV_LIST    = WM_APP + 2002;
const PATCH_MSG_RCV_FILE    = WM_APP + 2003;
const PATCH_MSG_RCV_CMD     = WM_APP + 2004;
const PATCH_MSG_ERROR       = WM_APP + 2005;
const PATCH_MSG_OK          = WM_APP + 2006;
const PATCH_MSG_ASSERT      = WM_APP + 2007;
const PATCH_MSG_RCVING_FILE = WM_APP + 2008;
//---------------------------------------------------------------------------
typedef unsigned char byte;
typedef std::vector<byte> vbyte;

enum RECV_STBTE
{
	RS_NORMAL = 0,					///< 일반 패킷 수신 대기 상태
	RS_RECV_FILE_BODY = 1,			///< 패치파일 내용 수신 대기 상태
    RS_RECV_PATCH_LIST_DATA = 2		///< 패치리스트 수신 대기 상태
};

class PACKAGE TXrayFTP : public TDataModule
{
__published:	// IDE-managed Components
    TClientSocket *LoginSock;
    TTimer *Timer1;
    void __fastcall LoginSockConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall LoginSockRead(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall LoginSockError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall LoginSockDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall Timer1Timer(TObject *Sender);
private:	// User declarations
    TForm* m_pOwner;

    String      m_IP;
    int         m_Port;

    vbyte m_RecvBuf;
    PATCH_TRANSFER_HEADER m_PatchTransHeader;
    PATCH_LIST_HEADER m_PatchListHeader;
    RECV_STBTE m_RecvState;
    int     m_bSynch;
    bool    m_IsReading;

    void __fastcall ParseRecvBuf();
    bool __fastcall ParseRecvBufLoop();
    void __fastcall OnRecv(const PATCH_CMD& p);
	void __fastcall OnRecv(const PATCH_LIST_HEADER& p);
    void __fastcall OnRecv(const PATCH_LIST_FOOTER& p);
    void __fastcall OnRecv(const PATCH_TRANSFER_HEADER& p);
    void __fastcall OnRecv(const PATCH_TRANSFER_FOOTER& p);
    void __fastcall OnRecv(const PATCH_ERROR& p);
    void __fastcall OnRecv(const PATCH_OK& p);

    bool __fastcall OnRecvListData();
    bool __fastcall OnRecvFileData();


    void __fastcall SendPatchCmd( PATCH_CMD* pcmd );
public:		// User declarations
    __fastcall TXrayFTP(TComponent* Owner);
    void __fastcall SetbSynch( int bSynch );    
//    __fastcall TXrayFTP(TComponent* Owner, int bSynch);
    void __fastcall Init();
    __fastcall ~TXrayFTP();

    bool __fastcall IsActive();

    void __fastcall Open( String  Address, int Port );
    void __fastcall Close();

    bool __fastcall ReqPatchList( String  path );

    // 리턴값이 false 이면 Active = false 인 것이다.
    // 따라서 다시 Open() 을 하고 IsActive() 로 Active 를 확인한 후 함수를 다시 호출한다.
    bool __fastcall Get( String SrvPath, String ClientPath );
    bool __fastcall Put( String ClientPath, String ServerPath );
    bool __fastcall ReqCmd( int CmdID, String  path );

    //TH TEST - RMFILE (서버의 파일 삭제)
    bool __fastcall RmFile( String  path );

    int m_Site;
    int m_WorkType;
    String m_ClientPath;

    String __fastcall ExtractSrvFileName( String SrvPath );
};
//---------------------------------------------------------------------------
extern PACKAGE TXrayFTP *XrayFTP;
//---------------------------------------------------------------------------
#endif
