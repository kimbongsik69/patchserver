//---------------------------------------------------------------------------

#ifndef SecurityThreadH
#define SecurityThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <vector>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <System.Win.ScktComp.hpp>
#include <DateUtils.hpp>
#include "bt_packet.h"
#include "bt_com_packet.h"
//---------------------------------------------------------------------------
#define FF 0xFF
#define CR 0x0D
#define LF 0x0A

#define MAX_BUF_SIZE 200000

enum RECV_STBTE
{
	RS_NORMAL = 0,					///< 일반 패킷 수신 대기 상태
	RS_RECV_FILE_BODY = 1,			///< 패치파일 내용 수신 대기 상태
    RS_RECV_PATCH_LIST_DATA = 2		///< 패치리스트 수신 대기 상태
};



typedef std::vector<byte> vbyte;

using namespace std;

class TSecurityThread : public TServerClientThread
{
private:


    vbyte 		m_RecvBuf;

    RECV_STBTE 	m_RecvState;

    int		m_process_status;
	int 	m_bufPos;;
	AnsiString mUserID;

    int 	m_Site;
    int 	m_WorkType;
    int     m_bSynch;

    int 	process_status;


    PATCH_TRANSFER_HEADER	m_CurrenctHeader;

	AnsiString 	m_ClientPath;
	AnsiString 	m_EXEPath;

	void __fastcall LogFunc(AnsiString msg, AnsiString filename, char *func, int line);
	void __fastcall RunPacket(TWinSocketStream *pStream, byte *buf, int len);
	void __fastcall Send_BT(PATCH_TRANSFER_HEADER *out_hdr, void *out,
							int out_size, void *out_repeat,
							int out_repeat_size, BT_FOOTER *out_ftr);

	int __fastcall SendBuf(void *buf, int len);
	
    void __fastcall ParseRecvBuf();

    bool __fastcall ParseRecvBufLoop();

    void __fastcall Patch_Command(PATCH_CMD *pCmd);

    int __fastcall send_patch_ok();

protected:
	void __fastcall ClientExecute();
public:
	__fastcall TSecurityThread(bool CreateSuspended, TServerClientWinSocket * ASocket);
	__fastcall ~TSecurityThread();



};
//---------------------------------------------------------------------------
#endif
