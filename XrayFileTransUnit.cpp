//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "XrayFileTransUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
#define FF 0xFF
#define CR 0x0D
#define LF 0x0A

TXrayFTP *XrayFTP;
//---------------------------------------------------------------------------
void __fastcall TXrayFTP::Init()
{
	m_RecvState = RS_NORMAL;
    m_IsReading = false;
    m_bSynch    = 0;
    m_pOwner    = (TForm*)Owner;
    m_Site      = 0;
    m_WorkType  = 1;
}
//---------------------------------------------------------------------------
__fastcall TXrayFTP::TXrayFTP(TComponent* Owner)
    : TDataModule(Owner)
{
    Init();
}
//---------------------------------------------------------------------------
void __fastcall TXrayFTP::SetbSynch( int bSynch )
{
    m_bSynch    = bSynch ;
}
/*
__fastcall TXrayFTP::TXrayFTP(TComponent* Owner, int bSynch)
    : TDataModule(Owner)
{
    Init();
    m_bSynch = bSynch;
}
//---------------------------------------------------------------------------
*/
__fastcall TXrayFTP::~TXrayFTP()
{
}

bool __fastcall TXrayFTP::IsActive()
{
    return LoginSock->Active;
}
//---------------------------------------------------------------------------

void __fastcall TXrayFTP::Close()
{
    LoginSock->Close();
}
//---------------------------------------------------------------------------
void __fastcall TXrayFTP::Open( String Address, int Port )
{
    try
    {
        if( LoginSock->Active )
            LoginSock->Close();

//        AnsiString Str = "Connect To Login Server";
        AnsiString Str = "Connect To Server";
//        PatchForm->ProgressLabel->Font->Color = clBlack;
//        PatchForm->ProgressLabel->Caption = Str;

        m_IP = "127.0.0.1";
//        m_IP = Address;
        m_Port = Port;

        //TH TEST
//        Timer1->Enabled = true;

        //LoginSock->Address = m_IP;
        LoginSock->Host = m_IP;
        LoginSock->Port = m_Port;
        LoginSock->Open();
    }
    catch( ... )
    {
        throw Exception( LoginSock->Host + " : " + LoginSock->Port + " 서버 접속 실패" );
    }
}
//---------------------------------------------------------------------------
bool __fastcall TXrayFTP::ReqPatchList( String  path )
{
    //assert( LoginSock->Active );
	if( !LoginSock->Active )
    {
        throw Exception( "로그인 서버에 접속 안됨" );
    	//return false;
    }

    PATCH_CMD pcmd;
    memset( &pcmd, 0x00, sizeof(PATCH_CMD) );
    pcmd.packet_type = ID_PATCH_CMD;
    pcmd.detail_type = 0;
    pcmd.site = m_Site % 1000;
    pcmd.work_type = m_WorkType;
    pcmd.broadcast   = m_bSynch;   //서버 동기화( 0 - 비동기화, 1 - 동기화)
    pcmd.cmd = PC_LIST;
    strcpy(pcmd.filename, path.c_str());

    pcmd.ff = FF;
    pcmd.cr = CR;
    pcmd.lf = LF;

    SendPatchCmd( &pcmd );

    return true;
}
//---------------------------------------------------------------------------

bool __fastcall TXrayFTP::ReqCmd( int CmdID, String  path )
{
    //assert( LoginSock->Active );
	if( !LoginSock->Active )
    {
        throw Exception( "로그인 서버에 접속 안됨" );
    	//return false;
    }

    PATCH_CMD pcmd;
    memset( &pcmd, 0x00, sizeof(PATCH_CMD) );
    pcmd.packet_type = ID_PATCH_CMD;
    pcmd.detail_type = 0;
    pcmd.site = m_Site % 1000;
    pcmd.work_type = m_WorkType;
    pcmd.broadcast   = m_bSynch;   //서버 동기화

    pcmd.cmd = CmdID;
    strcpy(pcmd.filename, path.c_str());

    pcmd.ff = FF;
    pcmd.cr = CR;
    pcmd.lf = LF;

    SendPatchCmd( &pcmd );

    return true;
}
//---------------------------------------------------------------------------

bool __fastcall TXrayFTP::Get( String SrvPath, String ClientPath )
{
    m_ClientPath = ExtractFilePath( ClientPath );

    //assert( LoginSock->Active );
    if( !LoginSock->Active )
    {
		throw Exception( "로그인 서버에 접속 안됨" );
        //return false;
    }

    if( SrvPath.Pos( "\\" ) || SrvPath.Pos( ":" ) )
    {
        throw Exception( "파일 패스 오류 : " + SrvPath );
    }

    PATCH_CMD pcmd;
    memset( &pcmd, 0x00, sizeof(PATCH_CMD) );
    pcmd.packet_type = ID_PATCH_CMD;
    pcmd.detail_type = 0;
    pcmd.site = m_Site % 1000;
    pcmd.work_type = m_WorkType;
    pcmd.broadcast   = m_bSynch; //서버 동기화

    pcmd.cmd = PC_READ;
    strcpy( pcmd.filename, SrvPath.c_str() );

    pcmd.ff = FF;
    pcmd.cr = CR;
    pcmd.lf = LF;

    SendPatchCmd( &pcmd );

    return true;
}
//----------------------------------------------------------------------------
bool __fastcall TXrayFTP::Put( String ClientPath, String ServerPath )
{
    //assert( LoginSock->Active );
	if( !LoginSock->Active )
    {
        throw Exception( "로그인 서버에 접속 안됨" );
        //return false;
    }

    if( ClientPath.Length() > 255 )
    {
        throw Exception( ClientPath + "Out of File Path" );
    }

    FILE* fp = fopen( ClientPath.c_str(), "rb" );
    //error처리
    if( !fp )
    {
        throw Exception( ClientPath + "패치파일 송신 실패(0)" );
    }
    long int length = 0;
    byte* pBuf = NULL;
    try
    {
        if( fseek( fp, 0L, SEEK_END ) )
        {
            throw Exception( ClientPath + "패치파일 송신 실패(1)" );
        }
        length = ftell( fp );
        if( length == -1 )
        {
            throw Exception( ClientPath + "패치파일 송신 실패(2)" );
        }
        if( fseek( fp, 0L, SEEK_SET ) )
        {
            throw Exception( ClientPath + "패치파일 송신 실패(2)" );
        }

        pBuf = new byte[length];
        memset( pBuf, 0x00, length * sizeof(byte) );
        fread( pBuf, sizeof(byte), length, fp );
    }
    __finally
    {
        fclose(fp);
    }

    PATCH_TRANSFER_HEADER Header;
    memset( &Header, 0x00, sizeof( PATCH_TRANSFER_HEADER ) );
    Header.packet_type  = ID_PATCH_TRANSFER_HEADER;
    Header.detail_type  = 0;
    Header.site = m_Site % 1000;
    Header.work_type = m_WorkType;
    Header.broadcast    = m_bSynch;

    //String ServerPath = StringReplace( ClientPath, "\\", "/", TReplaceFlags() << rfIgnoreCase << rfReplaceAll );
    //ServerPath = ServerPath.SubString( ServerPath.Pos( "/" ), ServerPath.Length() ).Trim();
    strcpy( Header.filename, ServerPath.c_str() );

	Header.filedate = FormatDateTime( "YYYYMMDD", Now().CurrentDate() ).ToInt();
	Header.filetime = FormatDateTime( "HHMMSS", Now().CurrentTime() ).ToInt();
	Header.filesize = length;

	Header.filler = 0;
    Header.ff = FF;
    Header.cr = CR;
    Header.lf = LF;

    PATCH_TRANSFER_FOOTER Footer;
    memset( &Footer, 0x00, sizeof( PATCH_TRANSFER_FOOTER ) );
    Footer.packet_type = ID_PATCH_TRANSFER_FOOTER;
	Footer.detail_type = 0;

    Footer.filler = 0;
    Footer.ff = FF;
    Footer.cr = CR;
    Footer.lf = LF;

    byte* FullBuf = new byte[sizeof(Header) + length * sizeof(byte) + sizeof(Footer)];

    memcpy( FullBuf, &Header, sizeof(Header) );
    memcpy( FullBuf + sizeof(Header), pBuf, length * sizeof(byte) );
    memcpy( FullBuf + sizeof(Header) + length * sizeof(byte), &Footer, sizeof(Footer) );
    LoginSock->Socket->SendBuf( FullBuf, sizeof(Header) + length * sizeof(byte) + sizeof(Footer) );

    delete[] FullBuf;
    //LoginSock->Socket->SendBuf(&Header, sizeof(Header));
    //LoginSock->Socket->SendBuf(pBuf, length * sizeof(byte) );
    //LoginSock->Socket->SendBuf(&Footer, sizeof(Footer));

    delete[] pBuf;

    return true;
}
//---------------------------------------------------------------------------
bool __fastcall TXrayFTP::ParseRecvBufLoop()
{
	if(m_RecvState == RS_NORMAL)
    {
        GUBUN_DATA gubun;

        if(m_RecvBuf.size() < sizeof(gubun))
        {
            // OutputDebugStringA("gubun 보다 작은 데이터 수신");
            return false;
        }

        vbyte::iterator it = m_RecvBuf.begin();
        memcpy(&gubun, &(*it), sizeof(gubun));

        switch(gubun.packet_type)
        {
            case ID_PATCH_LIST_HEADER:
            {
                //FOOTER는 바디에서 처리
                PATCH_LIST_HEADER p;
                if(m_RecvBuf.size() >= sizeof(p))
                {
	                memcpy(&p, &(*it), sizeof(p));
                    if( !(p.ff == FF && p.cr == CR && p.lf == LF) )
                        throw Exception( "패치리스트 해더 파싱실패" );
                    OnRecv(p);
                    m_RecvState = RS_RECV_PATCH_LIST_DATA;

                    vbyte::iterator itE = it;
                    std::advance(itE, sizeof(p));
                    m_RecvBuf.erase(it, itE);
                    return true;
                }
            }
            break;

            case ID_PATCH_TRANSFER_HEADER:
            {
                //FOOTER는 바디에서 처리
                PATCH_TRANSFER_HEADER p;
                if(m_RecvBuf.size() >= sizeof(p))
                {
	                memcpy(&p, &(*it), sizeof(p));
                    if( !(p.ff == FF && p.cr == CR && p.lf == LF) )
                        throw Exception( "패치파일 해더 파싱실패" );
                    OnRecv(p);
                    m_RecvState = RS_RECV_FILE_BODY;

                    vbyte::iterator itE = it;
                    std::advance(itE, sizeof(p));
                    m_RecvBuf.erase(it, itE);
                    return true;
                }
            }
            break;
            /*
            case ID_PATCH_CMD:
            {
				PATCH_CMD p;
                if(RecvBuf.size() >= sizeof(p))
                {
	                memcpy(&p, &(*it), sizeof(p));
                    assert(p.ff == FF && p.cr == CR && p.lf == LF);
                    OnRecv(p);

                    vbyte::iterator itE = it;
                    std::advance(itE, sizeof(p));
                    RecvBuf.erase(it, itE);
                    return true;
                }
            }
            break;
            */
            case ID_PATCH_ERROR:
            {
                //FOOTER는 바디에서 처리
                PATCH_ERROR p;
                if(m_RecvBuf.size() >= sizeof(p))
                {
	                memcpy(&p, &(*it), sizeof(p));
                    if( !(p.ff == FF && p.cr == CR && p.lf == LF) )
                        throw Exception( "패치리스트 해더 파싱실패" );
                    OnRecv(p);
                    m_RecvState = RS_NORMAL;

                    vbyte::iterator itE = it;
                    std::advance(itE, sizeof(p));
                    m_RecvBuf.erase(it, itE);
                    return true;
                }
            }
            case ID_PATCH_OK:
            {
                //FOOTER는 바디에서 처리
                PATCH_OK p;
                if(m_RecvBuf.size() >= sizeof(p))
                {
	                memcpy(&p, &(*it), sizeof(p));
                    if( !(p.ff == FF && p.cr == CR && p.lf == LF) )
                        throw Exception( "패치리스트 해더 파싱실패" );
                    //TH TEST
                    //OnRecv(p);
                    m_RecvState = RS_NORMAL;

                    vbyte::iterator itE = it;
                    std::advance(itE, sizeof(p));
                    m_RecvBuf.erase(it, itE);
                    // TH MODIFY Put 다음에 바로 Get을 하기 위해 SendMessage하는 시점을 뒤로 옮김.
                    OnRecv(p);
                    return true;
                }
            }
//            default: OutputDebugStringA("알 수 없는 데이터 수신");
        }
    }
    else if(m_RecvState == RS_RECV_PATCH_LIST_DATA)
    {
    	if( OnRecvListData() )
        {
	        m_RecvState = RS_NORMAL;
            //TH ADD
            if(m_pOwner)
                ::SendMessage( m_pOwner->Handle, PATCH_MSG_RCV_LIST, 0, 0 );
            return true;
        }
        else
        {
	    	return false;
        }
    }
    else if(m_RecvState == RS_RECV_FILE_BODY)
    {
        if( OnRecvFileData() )
        {
	        m_RecvState = RS_NORMAL;
            if( m_pOwner )
            {
                String FilePath = m_PatchTransHeader.filename;
                FilePath = m_ClientPath + ExtractSrvFileName( FilePath );

                if( m_pOwner )
                    ::SendMessage( m_pOwner->Handle, PATCH_MSG_RCV_FILE, (UINT)FilePath.c_str(), 0 );
            }
            return true;
        }
        else
        {
	    	return false;
        }
    }

    return false;
}
//---------------------------------------------------------------------------
//리스트 수신
bool __fastcall TXrayFTP::OnRecvListData()
{
	unsigned listDataSize = sizeof(PATCH_LIST_DATA)*m_PatchListHeader.list_count;
	unsigned total = listDataSize + sizeof(PATCH_LIST_FOOTER);

    if(m_RecvBuf.size() < total)
    {
        AnsiString str;
        str.sprintf("PATCH_LIST 수신중 %d/%d", m_RecvBuf.size(), total);

        return false;
    }
    else
    {
        AnsiString str = "PATCH_LIST 수신 완료";
        m_IsReading = false;

        vbyte::iterator it = m_RecvBuf.begin();
        vbyte::iterator itE = it;
        std::advance(itE, listDataSize);

        int i;
        for(i = 1; it != itE; ++i, std::advance(it, sizeof(PATCH_LIST_DATA)))
        {
        	PATCH_LIST_DATA data;
            memcpy(&data, &(*it), sizeof(data));

            if( !(data.type == FT_FILE || data.type == FT_DIR) )
                throw Exception( "패치 파일 정보 파싱 실패");

            /*
            Grid->Cells[0][i] = i;

			if(data.type == FT_FILE)
            	Grid->Cells[1][i] = "F";
            else
				Grid->Cells[1][i] = "D";

            Grid->Cells[2][i] = data.filename;

            Grid->Cells[3][i] = data.filesize;
            Grid->Cells[4][i] = data.filedate;
            Grid->Cells[5][i] = data.filetime;
            */
        }
        //Grid->RowCount = i;
        //Grid->AutoSizeColumns(true, 2);

		// (LIST_DATA + FOOTER) 삭제
        it = m_RecvBuf.begin();
        itE = it;
        std::advance(itE, total);
        m_RecvBuf.erase(it, itE);

        return true;
    }
}
//---------------------------------------------------------------------------

bool __fastcall TXrayFTP::OnRecvFileData()
{
    String FilePath = m_PatchTransHeader.filename;
	unsigned listDataSize = m_PatchTransHeader.filesize;
	unsigned total = listDataSize + sizeof(PATCH_TRANSFER_FOOTER);

    if(m_RecvBuf.size() < total)
    {
        AnsiString Str;
        Str.sprintf( " 수신중 %d/%d", m_RecvBuf.size(), total);
        Str = FilePath + Str;

        if( m_pOwner )
            ::SendMessage( m_pOwner->Handle, PATCH_MSG_RCVING_FILE, m_RecvBuf.size(), total );

        return false;
    }
    else
    {
        AnsiString Str = FilePath + " 수신 완료";
        m_IsReading = false;
        FilePath = m_ClientPath + ExtractSrvFileName( FilePath );

        String DirPath = ExtractFilePath( FilePath );
        if( !DirectoryExists( DirPath ) )
        {
            if( !ForceDirectories( DirPath ) )
                return false;
        }

        FILE* fp = fopen( FilePath.c_str(), "wb+" );
        if( !fp )
        {
            throw Exception( FilePath + "패치 파일 수신 실패" );
        }

        fwrite( &m_RecvBuf.front(), sizeof(byte), listDataSize, fp );
        fclose( fp );

        vbyte::iterator it = m_RecvBuf.begin();
        vbyte::iterator itE = it;
        //std::advance(itE, listDataSize);

		// (LIST_DATA + FOOTER) 삭제
        it = m_RecvBuf.begin();
        itE = it;
        std::advance(itE, total);
        m_RecvBuf.erase(it, itE);

        return true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TXrayFTP::OnRecv(const PATCH_ERROR& p)
{
    if( m_pOwner )
        ::SendMessage( m_pOwner->Handle, PATCH_MSG_ERROR, (UINT)(void*)p.msg, 0 );


    throw Exception( p.msg );
}

void __fastcall TXrayFTP::OnRecv(const PATCH_OK& p)
{
    if( m_pOwner )
        ::SendMessage( m_pOwner->Handle, PATCH_MSG_OK, 0, 0 );
}

void __fastcall TXrayFTP::OnRecv(const PATCH_CMD& p)
{
    AnsiString Str = "PATCH_CMD 수신";
	return;
}
//---------------------------------------------------------------------------
void __fastcall TXrayFTP::OnRecv(const PATCH_TRANSFER_HEADER& p)
{
    AnsiString Str = "PATCH_TRANSFER_HEADER 수신";
	m_PatchTransHeader = p;
	return;
}
//---------------------------------------------------------------------------
void __fastcall TXrayFTP::OnRecv(const PATCH_TRANSFER_FOOTER& p)
{
	AnsiString Str = "PATCH_TRANSFER_FOOTER 수신";
	return;
}
//---------------------------------------------------------------------------
void __fastcall TXrayFTP::OnRecv(const PATCH_LIST_HEADER& p)
{
	AnsiString Str = "PATCH_LIST_HEADER 수신";
	m_PatchListHeader = p;
	return;
}
//---------------------------------------------------------------------------
void __fastcall TXrayFTP::OnRecv(const PATCH_LIST_FOOTER& p)
{
	AnsiString Str = "PATCH_LIST_FOOTER 수신";
	return;
}
//---------------------------------------------------------------------------


void __fastcall TXrayFTP::SendPatchCmd( PATCH_CMD* pcmd )
{
    if( LoginSock->Active )
    {
        m_IsReading = true;
        m_RecvState = RS_NORMAL;
        m_RecvBuf.clear();
        LoginSock->Socket->SendBuf(pcmd, sizeof(*pcmd));

// 테스트로 잠시 타이머 꺼놓음
// 2013.1.8
//        Timer1->Enabled = true;
    }
    else
    {
        if( m_pOwner )
        {
            String ErrorMsg = "서버와 연결이 되어있지 않아 작업을 수행할 수 없습니다";
            ::SendMessage( m_pOwner->Handle, PATCH_MSG_ASSERT, (UINT)(void*)ErrorMsg.c_str(), 0 );
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TXrayFTP::LoginSockConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    m_RecvState = RS_NORMAL;
    m_RecvBuf.clear();

    if( m_pOwner )
        ::SendMessage( m_pOwner->Handle, PATCH_MSG_CONNECT, m_Port, (int)m_IP.c_str() );
}
//---------------------------------------------------------------------------
void __fastcall TXrayFTP::LoginSockRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    Timer1->Enabled = false;

    byte buf[4096];
    int rcvSize = Socket->ReceiveBuf(buf, sizeof(buf));

    m_RecvBuf.insert(m_RecvBuf.end(), buf, buf+rcvSize);

    ParseRecvBuf();
}
//---------------------------------------------------------------------------

void __fastcall TXrayFTP::ParseRecvBuf()
{
	while(!m_RecvBuf.empty() && ParseRecvBufLoop());
}
//---------------------------------------------------------------------------

/*
   if(AWorkCountMax > 0)
      ProgressBar1->Max = AWorkCountMax;
   else
      ProgressBar1->Max = BytesToTransfer;
*/
void __fastcall TXrayFTP::LoginSockError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    ErrorCode = 0;

    String ErrorMsg;
    switch( ErrorEvent )
    {
        case eeGeneral:
            ErrorMsg = "데이터 통신 중 요류가 발생하였습니다.";
        break;
        case eeSend:
            ErrorMsg = "데이터를 요청 할 수 없습니다.";
        break;
        case eeReceive :
            ErrorMsg = "데이터를 수신 할 수 없습니다.";
        break;
        case eeConnect :
            ErrorMsg = "서버와 연결 할 수 없습니다.";
        break;
        case eeDisconnect :
            ErrorMsg = "서버와의 연결이 끊겼습니다.";
        break;
        case eeAccept :
            ErrorMsg = "fail to accept.";
        break;
        default :
            ErrorMsg = "알 수 없는 통신 에러 발생.";
    }

    if( m_pOwner )
        ::SendMessage( m_pOwner->Handle, PATCH_MSG_ASSERT, (UINT)(void*)ErrorMsg.c_str(), 0 );

    //throw Exception( ErrorMsg + "\n 재접속 하시기 바랍니다." );
}
//---------------------------------------------------------------------------

void __fastcall TXrayFTP::LoginSockDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    if( m_pOwner )
        ::SendMessage( m_pOwner->Handle, PATCH_MSG_DISCONNECT, m_Port, (int)m_IP.c_str() );
}
//---------------------------------------------------------------------------

String __fastcall TXrayFTP::ExtractSrvFileName( String SrvPath )
{
    SrvPath = "c:\\" + StringReplace( SrvPath, "/", "\\", TReplaceFlags() << rfIgnoreCase << rfReplaceAll );
    return ExtractFileName( SrvPath );
}

void __fastcall TXrayFTP::Timer1Timer(TObject *Sender)
{
    Timer1->Enabled = false;
        if( m_pOwner )
        {
            String ErrorMsg = "서버로 부터 응답이 없습니다.";
            ::SendMessage( m_pOwner->Handle, PATCH_MSG_ASSERT, (UINT)(void*)ErrorMsg.c_str(), 0 );
        }
}
//---------------------------------------------------------------------------

bool __fastcall TXrayFTP::RmFile( String  path )
{
    //assert( LoginSock->Active );
	if( !LoginSock->Active )
    {
        throw Exception( "로그인 서버에 접속 안됨" );
    	//return false;
    }

    PATCH_CMD pcmd;
    memset( &pcmd, 0x00, sizeof(PATCH_CMD) );
    pcmd.packet_type = ID_PATCH_CMD;
    pcmd.detail_type = 0;
    pcmd.site = m_Site % 1000;
    pcmd.work_type = m_WorkType;
    pcmd.broadcast   = m_bSynch;   //서버 동기화( 0 - 비동기화, 1 - 동기화)
    pcmd.cmd = PC_RMFILE;
    strcpy(pcmd.filename, path.c_str());

    pcmd.ff = FF;
    pcmd.cr = CR;
    pcmd.lf = LF;

    SendPatchCmd( &pcmd );

    return true;
}
//---------------------------------------------------------------------------
