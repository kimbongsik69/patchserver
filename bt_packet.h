#ifndef _BT_PACKET_H
#define _BT_PACKET_H

//---------------------------------------------------------------------
// TR 번호 규칙 (5자리)
//---------------------------------------------------------------------
// 끝자리 0 : 요청 or IN
// 끝자리 1 : OUT or 응답
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// 패킷 구성
//---------------------------------------------------------------------
//
// Header
// Body
// Repeat 1
// Repeat 2
//        ...
// Repeat n
// Footer
//
//---------------------------------------------------------------------
//로긴 116.126.87.156 14100
//업무 116.126.87.156 14110
//id abcdefgh
//pw abcdefgh

//---------------------------------------------------------------------
// TR번호와 SK TR 번호 규칙
// 해당 TR번호 : BT TR 중 2로 시작 (예: 23010)
//---------------------------------------------------------------------
// SK TR번호에서 끝에 두번째 자리 생략 + In,Out구분 (0,1)
//
// SK 1번째 + SK 2번째 + SK 3번째 + SK 5번째 + Int, out 구분 (0,1)
//
//
// 예 ) BT 23010	SK_TR 23001 Input
//
// 예 ) BT 23011	SK_TR 23001 Ouput
//
//
// (로그인과 계좌리스트 조회 제외)
//---------------------------------------------------------------------


#ifndef _UCHAR
#define _UCHAR
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned short ushort;
#endif

#define ID_SK_PACKET		1
#define ID_BT_PACKET		2
#define ID_ANCHOR_PACKET	3
#define CNT_MAX_ACOUNT		32

//#pragma pack( push, 1 )

typedef	struct _at_header
{
	ushort  id;     			// 0xf1f2
	ushort  gubun;     			// 1 SK, 2 AT, 3 ANCHOR
	ushort  tr_no;
	ushort 	filler;	
	int 	body_size;
    int	    total_size;         // header + body + repeat*count + footer
	int 	repeat_size;
	int 	repeat_count;
} BT_HEADER;

typedef	struct _at_footer
{
	uchar filler;
	uchar ff;
	uchar cr;
	uchar lf;
} BT_FOOTER;

#define	SZ_BT_HEADER	sizeof(BT_HEADER)
#define	SZ_BT_FOOTER	sizeof(BT_FOOTER)

//---------------------------------------------------------------------
// 로그인 IN	10010 <- 10100
//---------------------------------------------------------------------
typedef	struct
{
	uchar id[20];
	uchar passwd[12];
    char  LocalIP[20];  //
} BT_10010;

#define SZ_BT_10010	sizeof(BT_10010)
//---------------------------------------------------------------------
// 로그인 OUT	10011 <- 10101
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 로그인 오류
	uchar msg[80];		// 로그인 오류시 메시지
	uchar id[20];
    int   UserGubun;        // 1 : 일반사용자, 99 : 관리자
} BT_10011;

#define SZ_BT_10011	sizeof(BT_10011)



//---------------------------------------------------------------------
// 계좌정보 IN	10020 <- 10200	SK_TR 09004
//---------------------------------------------------------------------
typedef	struct
{
	uchar id[20];
} BT_10020;

#define SZ_BT_10020	sizeof(BT_10020)
//---------------------------------------------------------------------
// 계좌정보 OUT	10021 <- 10201
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지
	uchar id[20];

} BT_10021;

#define SZ_BT_10021	sizeof(BT_10021)
typedef	struct
{

	char  no[20];      	//계좌번호
	char  name[40];    	//계좌명
	char  passwd[20];   //계좌비밀번호
	int  type;     		//0: 파생 1: 모의투자

} BT_10021_REPEAT;

#define SZ_BT_10021_REPEAT	sizeof(BT_10021_REPEAT)
//---------------------------------------------------------------------
// 우편번호 IN 10030 <- 10300
//---------------------------------------------------------------------
typedef struct
{
 uchar dong[28];  // 읍, 면, 동 (최소 2글자 이상 (4byte))
} BT_10030;

#define SZ_BT_10030 sizeof(BT_10030)
//---------------------------------------------------------------------
// 우편번호 OUT 10031 <- 10301
//---------------------------------------------------------------------
typedef struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 로그인 오류시 메시지
    uchar dong[28];  // 읍, 면, 동 (최소 2글자 이상 (4byte))
} BT_10031;

#define SZ_BT_10031 sizeof(BT_10031)

typedef struct
{
 char zipcode[8];      // 우편번호
 char sido[5];   // 특별시, 광역시, 도
 char gugun[15];         // 시,군,구
 char dong[27];          // 읍,면,동
 char ri[45];            // 리, 건물명
 char bunji[20];         // 번지, 아파트동, 호수
} BT_10031_REPEAT;

#define SZ_BT_10031_REPEAT sizeof(BT_10031_REPEAT)

//---------------------------------------------------------------------
// 선물옵션 주문체결 Master(실시간) IN	10040 <- 99010	SK_TR XF1 - IN이 있어야 하는지지? (
//---------------------------------------------------------------------
typedef	struct
{
	uchar id[20];	//userid
}BT_10040;
#define SZ_BT_10040	sizeof(BT_10040)

//---------------------------------------------------------------------
// 선물옵션 주문체결 Master(실시간) OUT 10041 <- 99011 SK_TR XF1 - IN이 있어야 하는지?
//(1번 통보된 내용은 전송하지 않는다..)
//---------------------------------------------------------------------
typedef struct
{
    int   	err_no;   // 0 : 정상 1 ~: 오류번호
    uchar 	msg[80];  // 오류시 메시지

	int		nClientKey;			//2011.02.22추가

    char    acc_no[12];        	//계좌번호
    int  	jumun_no;          	//주문번호
    int  	gubun;    			// 01 접수 02 체결 03 정정 04 취소 05 거부확인 06 접수거부
    int  	org_jumun_no;      	//원주문번호
    int     acode;             	//종목코드 - szJomokcd
    char  	name[40];        	//종목명
    double 	j_amount;     		//주문단가
    int  	j_qty;             	//주문수량 - szJsu
    double 	c_amount;           //체결단가
    int  	c_qty;            	//체결수량
    int  	m_qty;            	//미체결수량
    char    j_type;            	//주문유형("L" 지정가 "M" 시장가 "C" 조건부지정가 "B" 최유리지정가) - szJtype
    char    filler[4];
    char 	c_time[16];   //체결시간 YYYYMMDDHHNNSS
    int     j_gubun;            //주문구분(1:매도, 2:매수, 3:정정, 4:취소) - szJgubun
    int  	m_gubun;          //매매구분 0. 일반, 1. 커버드콜 - szMgubun
    double 	cur_price;          //현재가
    int     sign;    //등락부호
    int  vol_qty;   //체결량
}BT_10041;
#define SZ_BT_10041 sizeof(BT_10041)

//---------------------------------------------------------------------
// 이사님 요청 추가부분
//---------------------------------------------------------------------
typedef struct 
{
  char 		acc_no[12];						// '계좌번호',
  char 		yyyymmdd[12];  					// '거래일',
  char 		acc_passwd[12];  				// '계좌비밀번호',
  char 		user_id[20];  					// '회원아이디',
  char 		acc_name[32];  					// '계좌명',
  char 		manager[20];	 				// '관리자',
  //int 		gubun;							// '계좌구분 (0:일반, 1: 모투];',
  char  	rq_dongsi;      				// 동시호가 진입 신청 (0:미신청 1: 신청)
  char  	rq_over;      					// 오버 신청 (0:미신청 1: 신청)
  short   	acc_status;						// 계좌 상태 (0:정상 1: 탈퇴)

  int 		status;							// '계좌상태 (0:정상, 1: 해지];',
  int 		dambo_kijun_unit;  				// '담보금기준(1구좌];',
  int 		dambo_min_unit;  				// '최소담보금(1구좌];',
  int 		acc_kind;  						// '가입상품 (0:선물, 1:옵션, 2:선옵];',
  int 		acc_qty;  						// '구매수량',
  double 	f_fee_rate;	 					// '선물수수료',
  double	op_fee_rate;	 				// '옵션수수료',
  double 		coupon;  						// 사용 가능일 (선구매);',
  double 		today_use;  					// '당일사용여부(0:사용안함 1: 사용];',
  double 		fu_sonik; 						// '선물실현손익						 ',
  double 		op_sonik; 						// '옵션실현손익						 ',
  double 		sonik; 							// '총실현손익               ',
  double  		daily_charge;					// '일이용료',
  double  		weekly_charge;					// '주이용료',
  double  		monthly_charge;					// '월이용료 ',
  double 		fu_fee; 						// '선물수수료               ',
  double 		op_fee; 						// '옵션수수료               ',
  double 		fee; 							// '총수수료                 ',
  double 		fu_calc_sonik; 					// '선물평가손익             ',
  double 		op_calc_sonik; 					// '옵션평가손익             ',
  double 		calc_sonik; 					// '총잔고평가손익           ',
  double 		fu_jumun_deposit; 				// '선물주문증거금           ',
  double 		op_mesu_jumun_deposit; 			// '옵션매수주문증거금       ',
  double 		op_mudo_jumun_deposit; 			// '옵션매도주문증거금       ',
  double 		op_jumun_deposit; 				// '옵션주문증거금       ',
  double 		jumun_deposit; 					// '주문증거금               ',
  double 		fu_witak_deposit; 				// '선물위탁증거금           ',
  double 		op_mesu_witak_deposit; 			// '옵션매수위탁증거금       ',
  double 		op_medo_witak_deposit; 			// '옵션매도위탁증거금       ',
  double 		op_witak_deposit; 				// '옵션위탁증거금       ',
  double 		witak_deposit; 					// '위탁증거금               ',
  double 		in_rq_money; 					// '입금신청금액',
  double 		in_bank; 						// '은행 입금액              ',
  double 		in_etc; 						// '기타입금액                  ',
  double 		in_money; 						// '총입금액    (은행입금 + 기타입금)                    ',
  double 		coupon_charge; 					// '코폰 이용료선결제금액         ',
  double 		charge; 						// '이용료                   ',
  double 		etc_cost; 						// '기타비용                 ',
  double 		out_rq_money; 					// '출금신청금액',
  double 		out_bank; 						// '은행출금액                   ',
  double 		out_cost; 						// '출금비용                 ',
  double 		out_money; 						// '총출금액 (선결제+이용료+기타비용+은행출금액+출금수수료)                   ',
  double 		loan; 							// '대출금                   ',
  double 		dambo; 							// '담보금                   ',
  double 		yetak; 							// '예탁금                   ',
  double 		calc_yetak; 					// '평가예탁총액             ',
  double 		net_calc_yetak; 				// '순평가예탁총액           ',
  double 		calc_dambo; 						// '평가담보금               ',
  double 		jumun_possible; 				// '주문가능금액						 ',
  double 		out_possible; 					// '출금가능금액             ',
  double 		dambo_min; 						// '최소유지담보금           ',
  char		start_date[12];					// 계좌 개설일
  char		mo_acc_id[12]; 					// '모계좌',
  bool 		update_flag;					// 갱신여부 플래그
  char		autoclear;						// 현재 로스컷 상태 0: 정상 1: 로스컷
  char		today_losscut;					// 금일 로스컷 여부 0: 정상 1:로스컷 나간적 있음
  char		limit_qty;						// 최대신청가능구좌수
  int		dummy;
} BT_ACC_INFO;


typedef struct 
{
  char 		acc_no[12]; 		// 계좌번호',
  int		acode; 				// 종목코드',
  char		name[40]; 			// 종목명',
  char		mm_gubun[12]; 		// 매매구분 (매도, 매수)',
  int		qty; 				// 잔고수량',
  double   	ave_amount;			// 평균단가',
  double	cur_price;			// 현재가',
  unsigned int		money; 				// 매입금액',
  int		calc_money; 		// 평가금액',
  int		calc_sonik; 		// 평가손익',
  int		sonik; 				// 누적손익',
  int		witak_deposit; 		// 위탁증거금',
  int		over_qty;			// 오버 허용 수량
  bool 		update_flag;					// 갱신여부 플래그
  
  char		autoclear;			//자동청산 대상여부
  short		filler[2];
  int		dummy;
} BT_JANGO;


typedef struct 
{
  int		jumun_no;			// '주문번호',
  char		acc_no[12];			// '계좌번호',
  char		mo_acc_id[12]; 		// '모계좌',
  int		mo_jumun_no;		// '모계좌 주문번호',
  int		mo_org_jumun_no;	// '모계좌 원주문 번호',
  int		acode;				// '종목코드',
  char		code[12];			// '종목코드',
  char 		name[20];			// '종목명',
  char		jumun_gubun[12];	// '주문구분 (매도, 매수, 정정, 취소)',
  char		jumun_type[32];		// '주문유형 (지정가 시장가 조건부지정가 최유리지정가)',
  int 		qty;				// '주문수량',
  double 	amount;				// '주문단가',
  int 		che_qty;			// '체결수량',
  double	che_amount;			// '체결단가',
  int 		miche_qty;			// '미체결수량',
  int 		org_jumun_no;		// '원주문번호',
  char		status[20];			// '상태',
  short		new_qty;            // 신규주문수량
  short		nc_qty;             // 신규체결수량
  short		cc_qty;				// 청산체결수량
  int		client_key;			// 클라이언트에서 서버로 보내는 주문키.HHNNSSZZZ - 0813추가.
  int 		jumun_money;		// '총주문비용',
  int 		che_money;			// '총체결비용',
  int 		jumun_deposit;		// '주문증거금',
  int 		cur_price;			// '현재가',
  int		fee; 				// 수수료               
  int		sonik; 				// 매매손익          
  char 		jumun_time[16];	 	// '체결시간
  char 		che_time[16];	 	// '체결시간
  bool 		update_flag;		// 갱신여부 플래그
  char		who;				// 주문 주체 0: 고객 1: 운영자 2: 반대매매
  char		autoclear;			// 자동 취소 대상
  char		filler[1];

  int		dummy;
} BT_JUMUN;

//정현민 차장 it센타 02-3151-6401

//---------------------------------------------------------------------
// BT 실시간 예탁금 (내부 정보)  IN  10050 <- 99120
//---------------------------------------------------------------------
typedef	struct
{
	char    szGejano[12];       //계좌번호
} BT_10050;

#define SZ_BT_10050	sizeof(BT_10050)
//---------------------------------------------------------------------
// BT 실시간 예탁금 (내부 정보)  OUT  10051 <- 99121
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	char msg[80];		// 오류시 메시지

	BT_ACC_INFO info;

} BT_10051;

#define SZ_BT_10051	sizeof(BT_10051)

//---------------------------------------------------------------------
// BT 실시간 잔고 (내부 정보)    IN 10060 <- 99130
//---------------------------------------------------------------------
typedef	struct
{
	char    szGejano[12];       //계좌번호 - szGejano
} BT_10060;

#define SZ_BT_10060	sizeof(BT_10060)
//---------------------------------------------------------------------
// BT 실시간 잔고 (내부 정보)    OUT 10061 <- 99131
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	char msg[80];		// 오류시 메시지
	char    szGejano[12];       //계좌번호 - szGejano
} BT_10061;

#define SZ_BT_10061	sizeof(BT_10061)

typedef	BT_JANGO BT_10061_REPEAT;

#define SZ_BT_10061_REPEAT	sizeof(BT_10061_REPEAT)

//---------------------------------------------------------------------
// BT 실시간 미체결 (내부 정보) In 10070 <- 99140
//---------------------------------------------------------------------
typedef	struct
{
	char    szGejano[12];       //계좌번호 - szGejano
} BT_10070;

#define SZ_BT_10070	sizeof(BT_10070)

//---------------------------------------------------------------------
// BT 실시간 미체결 (내부 정보)    OUT 10071 <- 99141
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	char msg[80];		// 오류시 메시지
	char    szGejano[12];       //계좌번호 - szGejano
} BT_10071;

#define SZ_BT_10071	sizeof(BT_10071)

typedef	BT_JUMUN BT_10071_REPEAT;
#define SZ_BT_10071_REPEAT	sizeof(BT_10071_REPEAT)

//---------------------------------------------------------------------
//BT 재접속시 ID통보.. IN 10080
//---------------------------------------------------------------------
typedef struct
{
	uchar id[20];
	uchar passwd[12];
    char  LocalIP[20];  //
} BT_10080;
#define SZ_BT_10080	sizeof(BT_10080)

//---------------------------------------------------------------------
//BT 재접속시 ID통보.. OUT 10081 - 필요 없을것 같은데...
//---------------------------------------------------------------------
typedef struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	char msg[80];		// 오류시 메시지
} BT_10081;
#define SZ_BT_10081	sizeof(BT_10081)

//---------------------------------------------------------------------
// 선옵주문 IN  30000 <- 20000   SK_TR 20000
//---------------------------------------------------------------------
typedef	struct
{
	char    szGejano[12];       //계좌번호 - szGejano
	char    szGejapassword[8];  //비밀번호 - szGejapassword
	int     nAcode;             //종목코드 - szJomokcd
	int 	nJsu;           	//주문수량 - szJsu
	double 	dJamt;           	//주문가격 - szJamt
	int     nJgubun;            //주문구분(1:매도, 2:매수, 3:정정, 4:취소) - szJgubun
	int     nFOgubun;           //선물옵션구분(1:선물, 2:옵션) - szSOgubun
	int 	nSOgubun;        	//선물옵션구분 1. 선물, 2. 옵션
	int 	nMgubun;         	//매매구분 0. 일반, 1. 커버드콜 - szMgubun
	char    szJtype;           	//주문유형("L" 지정가 "M" 시장가 "C" 조건부지정가 "B" 최유리지정가) - szJtype
	char 	szJterm;          	//주문조건("I" IOC, "F" FOK, "SPACE" 일반) - szJterm
	char 	filler[2];
	int     nOjno;              //원주문 번호 - szOjno
	int		nClientKey;			//사용자가 사용하는 주문번호. 0813 추가.
	int		Extended[5];		//추후사용시 이름을 바꿔서 사용. 패킷 전체 사이즈가 변경되는것 방지용.
} BT_30000;

#define SZ_BT_30000	sizeof(BT_30000)

//---------------------------------------------------------------------
// 선옵주문 OUT	30001 <- 20001
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지
	int		nJno;            	//주문번호
	char    szGejano[12];       //계좌번호
	int     nAcode;             //종목코드
	char 	szJomokName[40];    //종목명
  	int		nClientKey;			//사용자가 사용하는 주문번호. 0813 추가.
	int		Extended[5];			//추후사용시 이름을 바꿔서 사용. 패킷 전체 사이즈가 변경되는것 방지용.
} BT_30001;

#define SZ_BT_30001	sizeof(BT_30001)


//---------------------------------------------------------------------
// 선옵청산주문 IN  30010 <- 21000
//---------------------------------------------------------------------
typedef	struct
{
	char    szGejano[12];       //계좌번호 - szGejano
	char    szGejapassword[8];  //비밀번호 - szGejapassword
	int		nJno;            	//주문번호
	int     nAcode;             //종목코드 - szJomokcd
	int 	nJsu;           	//주문수량 - szJsu

	int     nJgubunOrg;         //주문구분(1:매도, 2:매수) - 원래 주문형태

	int     nFOgubun;           //선물옵션구분(1:선물, 2:옵션) - szSOgubun
	char    szJtype;           	//주문유형("L" 지정가 "M" 시장가 "C" 조건부지정가 "B" 최유리지정가) - szJtype
	char 	filler[3];
	double 	dJamt;           	//주문가격 - szJamt
} BT_30010;

#define SZ_BT_30010	sizeof(BT_30010)

//---------------------------------------------------------------------
// 선옵청산주문 OUT	30011 <- 21001
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지
	int		nJno;            	//주문번호
	char    szGejano[12];       //계좌번호
	int     nAcode;             //종목코드
	char 	szJomokName[40];    //종목명
} BT_30011;

#define SZ_BT_30011	sizeof(BT_30011)

//---------------------------------------------------------------------
// 주문가능 수량  IN  30020 <- 20110    SK_TR 20011
//---------------------------------------------------------------------
typedef	struct
{
    char    szGejano[12];       //계좌번호
    char    szGejapassword[8];  //비밀번호
    int     nAcode;             //종목코드
    int  	nMgubun;   //매매구분(1:매도, 2:매수)
    char  	szJtype;         //주문유형 L. 지정가, M. 시장가
    char    filler[3];
    double dJamt;    //주문단가
} BT_30020;
#define SZ_BT_30020	sizeof(BT_30020)

//---------------------------------------------------------------------
// 주문가능 수량  OUT  30021 <- 20111
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지

	char    szGejano[12];       //계좌번호
	int     nAcode;             //종목코드
	char 	szJomokName[40];    //종목명
	int     nJsu;          		//주문가능수량
	int     nCsu;          		//청산가능수량

} BT_30021;
#define SZ_BT_30021	sizeof(BT_30021)



//---------------------------------------------------------------------
// 선옵잔고조회 IN	30030 <- 23000	SK_TR 20005
//---------------------------------------------------------------------
typedef	struct
{
	char    szGejano[12];       //계좌번호
	char    szGejapassword[8];  //비밀번호
} BT_30030;

#define SZ_BT_30030	sizeof(BT_30030)
//---------------------------------------------------------------------
// 선옵잔고조회 OUT	30031 <- 23001
// 평균단가, 수익율 - 처리?
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지
	char    szGejano[12];       //계좌번호
} BT_30031;

#define SZ_BT_30031	sizeof(BT_30031)

typedef	struct
{
	int     nAcode;             //종목코드
	char 	szJomokName[40];    //종목명
	char 	szMgubun[12];      	//매매구분 (매도, 매수)

	int		nJsu;           	//잔고수량
	double	dMamt;           	//매입가격
	double	dHamt;           	//현재가
	double	dPi;            	//평가손익
	double	dIamt;          	//매입금액
	double	dPamt;          	//평균단가
//	double	dIamt;          	//수익율

} BT_30031_REPEAT;

#define SZ_BT_30031_REPEAT	sizeof(BT_30031_REPEAT)

//------------------------------------------------------------------------------------------------
// 체결 내역 조회 IN	30040 <- 23010	SK_TR
//------------------------------------------------------------------------------------------------
typedef	struct
{
	char    szGejano[12];       //계좌번호
	char    szGejapassword[8];  //비밀번호
} BT_30040;

#define SZ_BT_30040	sizeof(BT_30040)
//---------------------------------------------------------------------
// 체결내역 조회 OUT	30041 <- 23011
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지
	char    szGejano[12];       //계좌번호
} BT_30041;

#define SZ_BT_30041	sizeof(BT_30041)

typedef	struct
{
	char    szDateTime[16];		//체결날짜 + 시간 (YYYYMMDDHHmmSS)
	int		nJno;            	//주문번호
	int     nAcode;             //종목코드
	char 	szJomokName[40];    //종목명
	char 	szJgubun[12];      	//주문구분 (매도, 매수, 정정, 취소)
	char 	szJtype[16];    	//주문유형 (지정가 시장가 조건부지정가 최유리지정가)
	int		nJsu;               //주문수량
	double	dJamt;           	//주문단가
	int		nCsu;           	//체결수량
	double	dCamt;           	//체결단가
	int		nMsu;           	//미체결수량
	int 	nOjno;              //원주문번호
	char 	szStatus[20];       //상태

} BT_30041_REPEAT;

#define SZ_BT_30041_REPEAT	sizeof(BT_30041_REPEAT)

//---------------------------------------------------------------------
// 미체결 내역 조회 IN	30050 <- 23020	SK_TR 23002
//---------------------------------------------------------------------
typedef	struct
{
	char    szGejano[12];       //계좌번호
	char    szGejapassword[8];  //비밀번호
} BT_30050;

#define SZ_BT_30050	sizeof(BT_30050)
//---------------------------------------------------------------------
// 미체결 내역 조회 OUT	30051 <- 23021
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지
	char    szGejano[12];       //계좌번호
} BT_30051;

#define SZ_BT_30051	sizeof(BT_30051)

typedef struct
{
 char    szDateTime[16];  //주문날짜 + 시간 (YYYYMMDDHHmmSS)
 int  nJno;             //주문번호
 int     nAcode;             //종목코드
 char  szJomokName[40];    //종목명
 char  szJgubun[12];       //주문구분 (매도, 매수, 정정, 취소)
 char  szJtype[16];     //주문유형 (지정가 시장가 조건부지정가 최유리지정가)
 int  nMsu;            //미체결수량
 double dJamt;            //주문단가
 double dHamt;            //현재가
 int  nOjno;              //원주문번호 - 201004-5 박준호 추가

} BT_30051_REPEAT;
#define SZ_BT_30051_REPEAT	sizeof(BT_30051_REPEAT)

//---------------------------------------------------------------------
//2.	출금처리 수신  (실시간 개별 공지 수신)
//2.	입금처리 수신  (실시간 개별 공지 수신)
// 		주문체결 수신 (실시간 수신)
//---------------------------------------------------------------------
// 계좌별 체결내역 조회  IN	30060 <- 31000
//---------------------------------------------------------------------
typedef	struct
{
	char    szGejano[12];       //계좌번호 - szGejano
	char    szGejapassword[8];  //비밀번호 - szGejapassword
	char    szDateBegin[8];     //조회시작일
	char    szDateEnd[8];       //조회종료일
	int 	nMedoflag;			//매도주문 (1 - 조회, 0-조회안함)
	int 	nMesuflag;			//매수주문 (1 - 조회, 0-조회안함)
	int 	nModflag;			//정정주문 (1 - 조회, 0-조회안함)
	int 	nCancleflag;		//취소주문 (1 - 조회, 0-조회안함)
} BT_30060;
#define SZ_BT_30060	sizeof(BT_30060)
//---------------------------------------------------------------------
// 계좌별 체결내역 조회  OUT  30061 <-	31001
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지
	char    szGejano[12];       //계좌번호
} BT_30061;
#define SZ_BT_30061	sizeof(BT_30061)

typedef struct
{
	char szJumunDate[16];  //주문일시
	int  nJno;            //주문번호
	int     nAcode;    //종목코드
	char  szJomokName[40];    //종목명
	char  szJgubun[12];  //주문구분 (매도, 매수, 정정, 취소)
	char  szJtype[16];  //주문유형 (지정가,시장가,조건부지정가,최유리지정가)
	int  nJsu;    //주문수량
	double dJamt;            //주문단가
	int  nCsu;            //체결수량
	double dCamt;            //체결단가
	int  nOjno;    //원주문번호
	char  szStatus[20];       //상태
	double  dSilSonik;   //실현손익
	int  nMgubun;          //매매구분 0. 일반, 1. 커버드콜 - szMgubun
	char    szDate[16];   //체결일시 YYYYMMDDHHmmSS

} BT_30061_REPEAT;


#define SZ_BT_30061_REPEAT	sizeof(BT_30061_REPEAT)

//---------------------------------------------------------------------
// 당일 정산손인분석현황  IN	30070 <- 31100
//---------------------------------------------------------------------
typedef	struct
{
	char    szGejano[12];       //계좌번호 - szGejano
	char    szGejapassword[8];  //비밀번호 - szGejapassword
} BT_30070;
#define SZ_BT_30070	sizeof(BT_30070)

//---------------------------------------------------------------------
// 당일 정산속인분석현황  OUT  30071 <- 31101
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지

	char    szGejano[12];       //계좌번호 - szGejano
    double  calc_yetak;        //예탁총액 = 평가예탁총액 - dYetakHap
    double  fu_sonik;           //선물실현손익 - dFuSilSonik
    double  op_sonik;           //옵션실현손익 - dOtSilSonik
    double  fu_calc_sonik;      //선물 평가손익 - dFuPi
    double  op_calc_sonik;      //옵션 평가손익 - dOtPi
    double  fu_fee;             //선물수수료 - dFuCmi
    double  op_fee;             //옵션수수료 - dOtCmi
    int     in_money;           // '총입금액    (은행입금 + 기타입금)                    ',
    int     out_money;          // '총출금액 (선결제+이용료+기타비용+은행출금액+출금수수료)                   ',
    double calc_dambo;          //평가담보금
} BT_30071;
#define SZ_BT_30071	sizeof(BT_30071)
typedef	struct
{
	int     nAcode;             //종목코드
	char 	szJomokName[40];    //종목명
	char 	szMgubun[12];      	//매매구분 (매도, 매수)
	int		nJsu;           	//잔고수량
	double	dMamt;           	//매입가격
	double	dHamt;           	//현재가
	double	dPi;            	//평가손익
	double	dIamt;          	//매입금액
	double	dPamt;          	//평균단가

} BT_30071_REPEAT;
#define SZ_BT_30071_REPEAT	sizeof(BT_30071_REPEAT)
//---------------------------------------------------------------------
// 기간별정산손인분석현황  IN  30080 <-	31110
//---------------------------------------------------------------------
typedef	struct
{
	char    szGejano[12];       //계좌번호 - szGejano
	char    szGejapassword[8];  //비밀번호 - szGejapassword
	char    szDateBegin[8];     //조회시작일
	char    szDateEnd[8];       //조회종료일
} BT_30080;
#define SZ_BT_30080	sizeof(BT_30080)

//---------------------------------------------------------------------
// 기간별정산속인분석현황  OUT	30081 <- 31111
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지

	char    szGejano[12];       //계좌번호 - szGejano
    double fu_sonikHap;
    double op_sonikHap;
    double fu_feeHap;
    double op_feeHap;
    int   in_moneyHap;     // '총입금액    (은행입금 + 기타입금)                    ',
    int   out_moneyHap;    // '총출금액 (선결제+이용료+기타비용+은행출금액+출금수수료)                   ',
	double	sun_sonikHap;			//순손익 = 총실현손익 - 수수료
} BT_30081;
#define SZ_BT_30081	sizeof(BT_30081)
typedef	struct
{
    char    szDate[8];  //YYYYMMDD
    double  fu_sonik;   //선물실현손익 - dFuSilSonik
    double  op_sonik;   //옵션실현손익 - dOtSilSonik
    double  fu_fee;     //선물수수료 - dFuCmi
    double  op_fee;     //옵션수수료 - dOtCmi
    int   in_money;     // '총입금액    (은행입금 + 기타입금)                    ',
    int   out_money;    // '총출금액 (선결제+이용료+기타비용+은행출금액+출금수수료)                   ',
    double dambo;       //정산후 담보금
	double	sun_sonik;			//순손익 = 총실현손익 - 수수료

} BT_30081_REPEAT;
#define SZ_BT_30081_REPEAT	sizeof(BT_30081_REPEAT)



//---------------------------------------------------------------------
// 예탁금조회  IN 30090 <- 31200
//---------------------------------------------------------------------
typedef struct
{
 char    szGejano[12];       //계좌번호 - szGejano
 char    szGejapassword[8];  //비밀번호 - szGejapassword
} BT_30090;
#define SZ_BT_30090 sizeof(BT_30090)
//---------------------------------------------------------------------
// 예탁금조회  OUT 30091 <- 31201
//---------------------------------------------------------------------
typedef struct
{
	int   err_no;			        // 0 : 정상 1 ~: 오류번호
	uchar msg[80];		            // 오류시 메시지

    char    szGejano[12];           //계좌번호 - szGejano
    char szGeraSijakIl[8];          //거래개시일 - 추가(YYYYMMDD)
    char acc_kind[12];              // '가입상품 (0:선물, 1:옵션, 3:선옵];',
	int  acc_qty;                   //거래구좌수

    double dambo;                   //개장시담보금
    double calc_dambo;              //평가담보금
    double dambo_min;               //로스컷 설정금액
	double remain_losscut;          //로스컷 여유금액 (calc_dambo - dambo-min)

    double  loan;                   //대출금          - a
    double  yetak;                  //개장시 예탁총액
    double  calc_yetak;             //예탁총액 = 평가예탁총액 - dYetakHap
    double  net_calc_yetak;         //순평가예탁총액   - a

    double fu_calc_sonik;           //선물 평가손익 - dFuPi
    double op_calc_sonik;           //옵션 평가손익 - dOtPi
    double  calc_sonik;             //총잔고 평가손익
    double  fu_sonik;               //선물실현손익 - dFuSilSonik
    double  op_sonik;               //옵션실현손익 - dOtSilSonik
    double  dGegaujiYesugm;         //계좌유지예수금
    double  fee;                    //수수료 합계 - dCmiHap
    double  fu_fee;                 //선물수수료 - dFuCmi
    double  op_fee;                 //옵션수수료 - dOtCmi

    double  fu_jumun_deposit;       //선물주문증거금
    double  op_mesu_jumun_deposit;  //옵션매수주문증거금
    double  op_mudo_jumun_deposit;  //옵션매도주문증거금
    double  jumun_deposit;          //주문증거금 - dJumunGanungDeposit

    double  fu_witak_deposit;       //선물위탁증거금
    double  op_mesu_witak_deposit;  //옵션매수위탁증거금
    double  op_medo_witak_deposit;  //옵션매도위탁증거금
    double  witak_deposit;          //위탁증거금 - dTrustDeposit

    double  net_dambo;          //평가담보금 - a

    double  out_possible;       //출금가능금액 - dChulgmGanungAtm
    double  order_possible;     //주문가능금액 - dJumunGanungAtm
    char    status[12];         //계좌상태
    char    today_use[12];      //당일계좌신청상태  - "신청", "미신청"
    char szGeraJehangubun[12];  //거래제한구분

    int   in_rq_money;          // '입금신청금액',
    int   in_bank;              // '은행 입금액              ',
    int   in_etc;               // '기타입금액                  ',
    int   in_money;             // '총입금액    (은행입금 + 기타입금)                    ',
    int   coupon_charge;        // '이용료선결제금액         ',
    int   charge;               // '이용료                   ',
    int   etc_cost;             // '기타비용                 ',
    int   out_rq_money;         // '출금신청금액',
    int   out_bank;             // '은행출금액                   ',
    int   out_cost;             // '출금비용                 ',
    int   out_money;            // '총출금액 (선결제+이용료+기타비용+은행출금액+출금수수료)                   ',

    int  coupon;   //쿠폰
	
	char	rq_status[12];	// 오버나잇 상태.
} BT_30091;
#define SZ_BT_30091 sizeof(BT_30091)

//---------------------------------------------------------------------
// 로그인 비밀번호 변경  IN	30100 <- 32100
//---------------------------------------------------------------------
typedef	struct
{
	uchar id[20];			//로그인 아이디
	uchar passwd[12];		//기존 비밀번호
	uchar Newpasswd[12];	//신규비밀번호

} BT_30100;
#define SZ_BT_30100	sizeof(BT_30100)
//---------------------------------------------------------------------
// 로그인 비밀번호 변경  OUT  30101 <- 32101
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지

	uchar id[20];
	uchar passwd[12];		//변경된 비밀번호
} BT_30101;

#define SZ_BT_30101	sizeof(BT_30101)

//---------------------------------------------------------------------
// 주문계좌비밀번호 변경  IN  30110 <- 32200
//---------------------------------------------------------------------
typedef	struct
{
	uchar id[20];					//로그인 아이디
	char    szGejano[12];			//계좌번호 - szGejano
	char    szGejapassword[8];		//기존비밀번호 - szGejapassword (계좌비밀번호)
	char    szNewGejapassword[8];	//변경비밀번호 - szGejapassword (계좌비밀번호)

} BT_30110;
#define SZ_BT_30110	sizeof(BT_30110)

//---------------------------------------------------------------------
// 주문계좌비밀번호 변경  OUT 30111 <- 32201
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지

	char    szGejano[12];			//계좌번호 - szGejano
	char    szGejapassword[8];			//변경된 계좌번호 - szGejano
} BT_30111;

#define SZ_BT_30111	sizeof(BT_30111)
//---------------------------------------------------------------------
// 고객정보조회  IN	30120 <- 32300
//---------------------------------------------------------------------
typedef	struct
{
	uchar id[20];			//로그인 아이디
	uchar passwd[12];		//비밀번호
} BT_30120;
#define SZ_BT_30120	sizeof(BT_30120)

//---------------------------------------------------------------------
// 고객정보조회  OUT   30121 <-	32301
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지

	uchar id[20];			//로그인 아이디

	char  szName[40];		//이름
	char  szJuminNo[16];	//주민번호 "-"는 제외한다.
	char  szEmail[80];		//이메일주소
	char  szMobile[16];		//전호번호국번이 3자리 또는 4자리여서 "-"를 붙여서 구분한다.
	char  szTel[20];		//082-043-1234-1234
	char  szZip[8];			//우편번호
	char  szAddress1[120];	//주소 - 크기조정..?
	char  szAddress2[60];	//주소 - 크기조정..?
	char  szBankName[40];		//은행명
	char  szGejano[20];		//계좌번호 - szGejano
    char  szYegmju[40];     //예금주
} BT_30121;
#define SZ_BT_30121	sizeof(BT_30121)

//---------------------------------------------------------------------
// 고객정보변경  IN	30130 <- 32400
//---------------------------------------------------------------------
typedef	struct
{
	uchar id[20];			//로그인 아이디
	uchar passwd[12];		//비밀번호  - 비밀번호가 틀리면 변경하면 안됨..
	char  szEmail[80];		//이메일주소
	char  szMobile[16];		//전호번호국번이 3자리 또는 4자리여서 "-"를 붙여서 구분한다.
	char  szTel[20];		//082-043-1234-1234
	char  szZip[8];			//우편번호
	char  szAddress1[120];	//주소 - 크기조정..?
	char  szAddress2[60];	//주소 - 크기조정..?
	char  szBankName[40];	//은행명
	char  szGejano[20];		//계좌번호 - szGejano
    char  szYegmju[40];     //예금주
} BT_30130;
#define SZ_BT_30130	sizeof(BT_30130)
//---------------------------------------------------------------------
// 고객정보변경  OUT  30131 <-	32401
//---------------------------------------------------------------------
typedef	struct
{

	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar  msg[80];		// 오류시 메시지

	uchar  id[20];			//로그인 아이디
} BT_30131;

#define SZ_BT_30131	sizeof(BT_30131)
//---------------------------------------------------------------------
// 입금신청  IN	30140 <- 33100
//---------------------------------------------------------------------
typedef	struct
{
	uchar	id[20];					//아이디 - 필요한가...
	char	szGejano[12];			//계좌번호 - szGejano
	char	szGejapassword[8];		//비밀번호 - szGejapassword
	char    szSendBank[20];	        //입금되는 은행계좌번호 - 회사
	char	szName[40];				//입금자명
	double	dAmt;					//입금액
} BT_30140;
#define SZ_BT_30140	sizeof(BT_30140)
//---------------------------------------------------------------------
// 입금신청  OUT	30141 <- 33101
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지

	char    szGejano[12];	//계좌번호 - szGejano
	double	dAmt;					//입금액
} BT_30141;

#define SZ_BT_30141	sizeof(BT_30141)
//---------------------------------------------------------------------
// 입금은행 조회  IN  30150 <-	33200
//---------------------------------------------------------------------
typedef	struct
{
	uchar	id[20];					//아이디
} BT_30150;
#define SZ_BT_30150	sizeof(BT_30150)
//---------------------------------------------------------------------
// 입금은행 조회  OUT	30151 <- 33201
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지

	uchar	id[20];					//아이디

} BT_30151;
#define SZ_BT_30151	sizeof(BT_30151)

typedef	struct
{
	char	szBank[40];				//은행명
	char	szGejano[20];			//계좌번호
	char	szYegmju[40];			//예금주
	char    phone[20];      //문의전화번호
} BT_30151_REPEAT;
#define SZ_BT_30151_REPEAT	sizeof(BT_30151_REPEAT)
//---------------------------------------------------------------------
// 시중은행리스트 조회  IN	30160 <- 33210
//---------------------------------------------------------------------
typedef	struct
{
	uchar	id[20];					//아이디
} BT_30160;
#define SZ_BT_30160	sizeof(BT_30160)
//---------------------------------------------------------------------
// 시중은행리스트 조회  OUT	30161 <- 33211
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지

	uchar	id[20];					//아이디

} BT_30161;
#define SZ_BT_30161	sizeof(BT_30161)
typedef	struct
{
	char	szBank[40];				//은행명
} BT_30161_REPEAT;
#define SZ_BT_30161_REPEAT	sizeof(BT_30161_REPEAT)


//---------------------------------------------------------------------
// 출금신청   IN   30170 <- 33300
//---------------------------------------------------------------------
typedef	struct
{
	uchar	id[20];					//아이디
	char	szGejano[12];			//계좌번호 - szGejano
	char	szGejapassword[8];		//비밀번호 - szGejapassword
	char	szBank[40];				//받을은행
	char	szYegmju[40];			//예금주
	char	szBankGejano[20];		//계좌번호 - szGejano
	double	dAmt;					//출금신청금액
} BT_30170;
#define SZ_BT_30170	sizeof(BT_30170)
//---------------------------------------------------------------------
// 출금신청  OUT	30171 <- 33301
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지

	char    szGejano[12];	//계좌번호 - szGejano
} BT_30171;

#define SZ_BT_30171	sizeof(BT_30171)
//---------------------------------------------------------------------
//입출금내역 조회  IN	30180 <- 33600
//---------------------------------------------------------------------
typedef	struct
{
	char    szGejano[12];       //계좌번호 - szGejano
	char    szGejapassword[8];  //비밀번호 - szGejapassword
	char	szInoutFlag;		//입출금구분 ("1" - 입금내역, "2" - 출금내역, "3" - 입출금내역)
	char    filler[3];
	char    szDateBegin[8];     //조회시작일
	char    szDateEnd[8];       //조회종료일
} BT_30180;
#define SZ_BT_30180	sizeof(BT_30180)

//---------------------------------------------------------------------
// 입출금내역 조회  OUT	30181 <- 33601
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지

	char    szGejano[12];	//계좌번호 - szGejano
} BT_30181;

#define SZ_BT_30181	sizeof(BT_30181)


typedef	struct
{
	char	szInoutFlag;		//입출금구분 ("1" - 입금내역, "2" - 출금내역)
	char    filler[3];
	char	szSinchungDate[16];	//신청일시 YYYYMMDDHHnnSS
//	int		nNo;				//일련번호
	char	msg[80];			//내용
	double  inout_moneySinchung;//입출금 신청액
	double  inout_money;        //입출금 처리금액
//	double	dAmt;				//거래금액(실체 입출금액)
	double	dAmt;				//거래금액(실체 입출금액)
	char	szBank[40];			//은행명
	char	szGeraGejano[20];	//거래은행계좌번호
	char	szIpgmju[40];		//입금자
	char	szYegmju[40];		//예금주명
	char	szBankIcheDate[14];	//처리일시
	char	szIcheSt[16];		//처리상태
	char	msgMemo[80];		//기타내용(특이사항)
} BT_30181_REPEAT;

#define SZ_BT_30181_REPEAT	sizeof(BT_30181_REPEAT)

//---------------------------------------------------------------------
//상품구좌수 변경 IN   30190 <- 33700
//---------------------------------------------------------------------
typedef	struct
{
	char    szGejano[12];       //계좌번호 - szGejano
	char    szGejapassword[8];  //비밀번호 - szGejapassword
	char	acc_kind[12];		//가입상품 - 선물, 옵션, 선옵
	int		acc_qty;				//거래구좌수
} BT_30190;
#define SZ_BT_30190	sizeof(BT_30190)
//---------------------------------------------------------------------
//상품구좌수 변경 OUT	30191 <- 33701
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지

	char    szGejano[12];	//계좌번호 - szGejano
} BT_30191;
#define SZ_BT_30191	sizeof(BT_30191)


//---------------------------------------------------------------------
//상품구좌수 예상조회 IN 30200 <- 33710
//---------------------------------------------------------------------
typedef	struct
{
	char    szGejano[12];       //계좌번호 - szGejano
	char	acc_kind[12];		//가입상품 - 선물, 옵션, 선옵
	int		acc_qty;				//거래구좌수
} BT_30200;
#define SZ_BT_30200	sizeof(BT_30200)
//---------------------------------------------------------------------
//상품구좌수 예상조회 OUT 30201 <- 33711
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지

	char    szGejano[12];       //계좌번호 - szGejano
	char	acc_kind[12];		//가입상품 - 선물, 옵션, 선옵
	int		acc_qty;				//거래구좌수
    int     coupon;            //쿠폰수량
	double	fee;		        //이용료
    double  dambo_after;        //신청후담보금
	double	dambo_need;			//필요담보금
	double	dambo_min;			//로스컷 설정금액
	double  out_possible; 	    //출금가능금액 - dChulgmGanungAtm
} BT_30201;
#define SZ_BT_30201	sizeof(BT_30201)
//---------------------------------------------------------------------
//최대가용구좌수 조회 IN 30210 <- 33720
//---------------------------------------------------------------------
typedef	struct
{
	char    szGejano[12];       //계좌번호 - szGejano
	char    szGejapassword[8];  //비밀번호 - szGejapassword
	char	acc_kind[12];		//가입상품 - 선물, 옵션, 선옵
} BT_30210;
#define SZ_BT_30210	sizeof(BT_30210)
//---------------------------------------------------------------------
//최대가용구좌수 조회 OUT 30211 <- 33721
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지

	char    szGejano[12];           //계좌번호 - szGejano
	char	acc_kind[12];		    //가입상품 - 선물, 옵션, 선옵
    int     acc_qty;              //가용구좌수
} BT_30211;
#define SZ_BT_30211	sizeof(BT_30211)

//---------------------------------------------------------------------
//당일계좌사용신청 IN 30220 <- 33800
//---------------------------------------------------------------------
typedef	struct
{
	char    szGejano[12];       //계좌번호 - szGejano
	char    szGejapassword[8];  //비밀번호 - szGejapassword
	char	acc_kind[12];		//가입상품 - 선물, 옵션, 선옵
	int		acc_qty;				//거래구좌수
	int		nUseFlag;			//1:사용신청. 2:신청취소
} BT_30220;
#define SZ_BT_30220	sizeof(BT_30220)
//---------------------------------------------------------------------
//당일계좌사용신청 OUT 30221 <- 33801
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지

	char    szGejano[12];       //계좌번호 - szGejano
	int		nUseFlag;			//1:사용신청. 2:신청취소
} BT_30221;
#define SZ_BT_30221	sizeof(BT_30221)

//---------------------------------------------------------------------
//기본이용료금액조회  IN  30230 <-	33810
//---------------------------------------------------------------------
typedef	struct
{
	char    szGejano[12];       //계좌번호 - szGejano
	char    szGejapassword[8];  //비밀번호 - szGejapassword
} BT_30230;
#define SZ_BT_30230	sizeof(BT_30230)
//---------------------------------------------------------------------
//기본이용료금액조회  OUT	30231 <- 33811
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지

	char    szGejano[12];       //계좌번호 - szGejano
    double  dDayAmt;    //일일이용료
    double  dWeekAmt;    //일일이용료
    double  dMonthAmt;    //일일이용료
} BT_30231;
#define SZ_BT_30231	sizeof(BT_30231)
//---------------------------------------------------------------------
//이용료선납  IN	30240 <- 33820
//---------------------------------------------------------------------
typedef	struct
{
	char    szGejano[12];       //계좌번호 - szGejano
	char    szGejapassword[8];  //비밀번호 - szGejapassword
	int		nGiganGubun;		//1-일납, 2-주납, 3-월납
	double	dAmt;				//이용료 금액
} BT_30240;
#define SZ_BT_30240	sizeof(BT_30240)
//---------------------------------------------------------------------
//이용료선납  OUT	30241 <- 33821
//---------------------------------------------------------------------
typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지

	char    szGejano[12];       //계좌번호 - szGejano
} BT_30241;
#define SZ_BT_30241	sizeof(BT_30241)

//---------------------------------------------------------------------
//오버나잇수량조회 IN 30250 <- 33900
//---------------------------------------------------------------------
typedef	struct
{
	char    szGejano[12];       //계좌번호
	char    szGejapassword[8];  //비밀번호
} BT_30250;

#define SZ_BT_30250	sizeof(BT_30250)
//---------------------------------------------------------------------
//오버나잇수량조회 OUT 30251 <- 33901 - 여러개의 계좌를 가질수 있으니..
//---------------------------------------------------------------------

typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지

	char    szGejano[12];       //계좌번호
} BT_30251;

#define SZ_BT_30251	sizeof(BT_30251)

typedef	struct
{
	int     nAcode;             //종목코드
	char 	szJomokName[40];    //종목명
	char 	szMgubun[12];      	//매매구분 (매도, 매수)

	int		nJsu;           	//잔고수량
	double	dMamt;           	//매입가격
	double	dHamt;           	//현재가
	double	dPi;            	//평가손익
	double	dIamt;          	//매입금액
	double	dPamt;          	//평균단가
	int		nOver;		        //선물 오버나잇 수량
    int     nNotOver;           //청산예정수량
} BT_30251_REPEAT;

#define SZ_BT_30251_REPEAT	sizeof(BT_30251_REPEAT)

//---------------------------------------------------------------------
//로그정보 전송  IN	30260 <- 41000
//---------------------------------------------------------------------
typedef	struct
{
	uchar	id[20];		//사용자 ID
	char    szGejano[12];       //계좌번호
    char  LocalIP[20];  //
	char	szDate[8];	//YYYYMMDD
	char	szTime[6];	//HHMMSS
	char 	filler[2];
	char	msg[204];	//사용정보를 담는다..
//	char	msg[1024];	//사용정보를 담는다..

} BT_30260;

#define SZ_BT_30260	sizeof(BT_30260)
//---------------------------------------------------------------------
//공지목록 조회  IN 30270 <- 42000
//---------------------------------------------------------------------
typedef struct
{
    uchar id[20];       //사용자 아이디
    int   nGubun;       // 1- 공지사항 + 긴급공지, 2-공지사항, 3-긴급공지
    int   nDataNum;     //0이면 처음부터
    char  szDate[8];   //YYYYMMDD - 공지조회기간
} BT_30270;
#define SZ_BT_30270 sizeof(BT_30270)
//---------------------------------------------------------------------
//공지목록 조회  OUT 30271 <- 42001
//---------------------------------------------------------------------
typedef struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지

//    uchar   id[20];       //사용자 아이디

} BT_30271;
#define SZ_BT_30271 sizeof(BT_30271)

typedef struct
{
	uchar   id[20];       //사용자 아이디 - 전체공지이면 ""
	int     nGubun;         // 2-공지사항, 3-긴급공지
     int   nDataNum;     //0이면 처음부터
	char    szDate[8];      //YYYYMMDD
    char    szTime[6];      //HHmmSS
	char    szTitle[92];    //공지제목

} BT_30271_REPEAT;
#define SZ_BT_30271_REPEAT sizeof(BT_30271_REPEAT)
//---------------------------------------------------------------------
//공지내용 조회  IN 30280 <- 42100
//---------------------------------------------------------------------
typedef struct
{
    uchar id[20];   //사용자 아이디
    int   nDataNum; //0이면 처음부터
} BT_30280;
#define SZ_BT_30280 sizeof(BT_30280)
//---------------------------------------------------------------------
//공지내용 조회  OUT 30281 <- 42101
//---------------------------------------------------------------------
typedef struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지

    uchar id[20];   //사용자 아이디
    int   nDataNum; //0이면 처음부터
	char  szMsg[2048];    //공지내용
} BT_30281;
#define SZ_BT_30281 sizeof(BT_30281)
//---------------------------------------------------------------------
//실시간개인 공지 조회  30291 <- OUT 42201
//---------------------------------------------------------------------
typedef struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];		// 오류시 메시지

    uchar id[20];   //사용자 아이디
    char    szDate[8];      //YYYYMMDD
    char    szTime[6];      //HHmmSS
    char    filler[2];
    int   nDataNum; //0이면 처음부터
	char    szTitle[96];    //공지제목
    char    szMsg[2048];    //공지내용
} BT_30291;
#define SZ_BT_30291 sizeof(BT_30291)

//---------------------------------------------------------------------
//오버나잇, 동시호가 신청. IN 30300
//---------------------------------------------------------------------
typedef	struct
{
	int		type;			// 0: 오버나잇, 1: 동시호가	
	char    acc_no[12];     //계좌번호
	char    pass[12];  		//비밀번호
	int		use;			// 0: 취소, 1:신청
} BT_30300;

#define SZ_BT_30300	sizeof(BT_30300)
//---------------------------------------------------------------------
//오버나잇, 동시호가 신청 응답 OUT 30301
//---------------------------------------------------------------------

typedef	struct
{
	int   err_no;			// 0 : 정상 1 ~: 오류번호
	uchar msg[80];			// 오류시 메시지
} BT_30301;

#define SZ_BT_30301	sizeof(BT_30301)

//---------------------------------------------------------------------
// 주문가능 수량(매수, 매도 같이)  IN 30310
//---------------------------------------------------------------------
typedef	struct
{
    char    szGejano[12];       //계좌번호
    char    szGejapassword[8];  //비밀번호
    int     nAcode;             //종목코드
    char  	szJtype;         	//주문유형 L. 지정가, M. 시장가
    char    filler[3];
    double 	dJamt;    			//주문단가
} BT_30310;
#define SZ_BT_30310	sizeof(BT_30310)

//---------------------------------------------------------------------
// 주문가능 수량(매수, 매도 같이)  OUT 30311
//---------------------------------------------------------------------
typedef	struct
{
	int   	err_no;		// 0 : 정상 1 ~: 오류번호
	uchar 	msg[80];	// 오류시 메시지

	char    szGejano[12];       //계좌번호
	int     nAcode;     //종목코드
	int     buy_pos;     		//주문가능수량(매수)
	int     buy_pos_new;    	//주문가능수량(신규매수)
	int     buy_pos_payoff; 	//주문가능수량(매수주문할매도잔고)

	int  	sell_pos;  			//주문가능수량(매도)
	int  	sell_pos_new; 		//주문가능수량(신규매도)
	int  	sell_pos_payoff;	//주문가능수량(매도주문할매수잔고)
} BT_30311;
#define SZ_BT_30311	sizeof(BT_30311)

//---------------------------------------------------------------------
// 회원리스트 조회 IN 40100
//---------------------------------------------------------------------
typedef struct
{
    int     level;      //관리자 레벨
    char    mng_id[20];  //관리자 ID
    char    id[40];     //검색할 ID 또는 이름
} BT_40100;
#define SZ_BT_40100 sizeof(BT_40100)

//---------------------------------------------------------------------
// 회원리스트 응답 OUT 40101.
//---------------------------------------------------------------------
typedef	struct
{
	int		err_no;			// 0 : 정상 1 ~: 오류
	char	msg[80];
} BT_40101;
#define SZ_BT_40101	sizeof(BT_40101)

typedef struct
{
    char    id[20];         //사용자 아이디
	char    name[40];	    //이름
	char    tel[20];	    //연락처
    int     is_login;       //접속 여부 1:접속 0:미접속
    int     level;          //회원등급 0: 최고운영자 1:운영자 10:중간관리자 20:일반
    int     is_use;         //0: 사용, 1: 미사용
} BT_40101_REPEAT;
#define SZ_BT_40101_REPEAT sizeof(BT_40101_REPEAT)

//---------------------------------------------------------------------
// 회원 상세정보 및 계좌정보 조회 IN 40110. 
//---------------------------------------------------------------------
typedef struct
{
    int     level;      //관리자 레벨
    char    mng_id[20];  //관리자 ID
    char    id[20];     //상세정보 및 계좌정보 요청 ID
} BT_40110;
#define SZ_BT_40110 sizeof(BT_40110)

//---------------------------------------------------------------------
// 회원 상세정보 및 계좌정보 응답 OUT 40111.
//---------------------------------------------------------------------
typedef	struct
{
	int		err_no;			// 0 : 정상 1 ~: 오류
	char	msg[80];

	char    id[20];          // User ID - 수정시에는 키로 사용.
	char    name[40];	    // 이름	
	
    char    pass[12];       //비밀번호
    char    email[80];	    //이메일주소
	char    zip[8];		    //우편번호
	char    address1[120];  //주소 - 크기조정
	char    address2[60];	//주소 - 크기조정
	char    bank[40];	    //은행명
    char    owner[40];      //예금주
	char    mem_acc_no[20];	//출금 계좌번호

	double  fu_fee;         //선물수수료
	double  op_fee;         //옵션수수료
	double  fu_dambo_unit;  //선물담보금기준
	double  op_dambo_unit;  //옵션담보금기준
	double  fo_dambo_unit;  //선물/옵션담보금기준
	double  fu_dambo_min;   //선물최소담보금
	double  op_dambo_min;   //옵션최소담보금
	double  fo_dambo_min;   //선물/옵션최소담보금
	double  fu_loan;    	//선물기본대출금
	double  op_loan;    	//옵션기본대출금
	double  fo_loan;    	//선물/옵션기본대출금

	int     daily_charge;   //일이용료
	int     weekly_charge;  //주이용료
	int     monthly_charge; //월이용료

	char    oper_id[20];     //상담담당자 id
	char    oper_name[40];   //상담담당자 이름
	char    mo_acc[12];      //모계좌id
	int		limit_qty;		 //최대신청가능 구좌수
	int     site;			 //가입한 site
	char    site_fullname[40];		
	char	jumin1[8];		//주민번호 앞자리 6
	char	jumin2[8];		//주민번호 뒷자리 7
} BT_40111;
#define SZ_BT_40111	sizeof(BT_40111)

typedef struct
{
    int     acc_type;       //계좌유형 0: 실전투자 99:모의투자
	char    acc_no[12];     //계좌번호
    char    acc_pass[8];    //계좌비밀번호
} BT_40111_REPEAT;
#define SZ_BT_40111_REPEAT sizeof(BT_40111_REPEAT)

//---------------------------------------------------------------------
// 회원리스트 삭제 IN 40120.
//---------------------------------------------------------------------
typedef struct
{
    int     level;      //관리자 레벨
    char    mng_id[20]; //관리자 ID
} BT_40120;
#define SZ_BT_40120 sizeof(BT_40120)

typedef struct
{
    char    id[20];     //삭제요청 ID
} BT_40120_REPEAT;
#define SZ_BT_40120_REPEAT sizeof(BT_40120_REPEAT)
//---------------------------------------------------------------------
// 회원리스트 삭제 요청후 응답 OUT 40121.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		// 0 : 정상 1 ~: 오류
	char    msg[80];    // 오류시 메시지
} BT_40121;
#define SZ_BT_40121 sizeof(BT_40121)

//---------------------------------------------------------------------
// 계좌삭제 IN 40130. 
//---------------------------------------------------------------------
typedef struct
{
    int     level;      //관리자 레벨
    char    mng_id[20]; //관리자 ID
    char    id[20];     //삭제할 계좌를 사용중인 ID
} BT_40130;
#define SZ_BT_40130 sizeof(BT_40130)

typedef struct
{    
    char    acc_no[12]; //계좌번호    
} BT_40130_REPEAT;
#define SZ_BT_40130_REPEAT sizeof(BT_40130_REPEAT)
//---------------------------------------------------------------------
// 계좌삭제 응답 OUT 40131. TYPE 1
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;     // 0:정상 1~:오류
	char    msg[80];	// 오류시 메시지
} BT_40131;
#define SZ_BT_40131 sizeof(BT_40131)

//---------------------------------------------------------------------
// 수수료 및 담보금 설정 값 조회. IN 40140. 
//---------------------------------------------------------------------
typedef struct
{
    int     level;      //관리자 레벨
    char    mng_id[20]; //관리자 ID
} BT_40140;
#define SZ_BT_40140 sizeof(BT_40140)

//---------------------------------------------------------------------
// 수수료 및 담보금 설정 값 조회응답 OUT 40141.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;         //0:정상 1~:오류
	char    msg[80];        //오류시 메시지

    double  fu_fee;         //선물수수료 * 10000
    double  op_fee;         //옵션수수료 * 10000
    double  fu_dambo_unit;  //선물담보금기준
    double  op_dambo_unit;  //옵션담보금기준
    double  fo_dambo_unit;  //선물/옵션담보금기준
    double  fu_dambo_min;   //선물최소담보금
    double  op_dambo_min;   //옵션최소담보금
    double  fo_dambo_min;   //선물/옵션최소담보금
    double  fu_loan;    	//선물기본대출금
    double  op_loan;    	//옵션기본대출금
    double  fo_loan;    	//선물/옵션기본대출금

    int     daily_charge;   //일이용료
    int     weekly_charge;  //주이용료
    int     monthly_charge; //월이용료
	
	int		limit_qty;		 //최대신청가능 구좌수
} BT_40141;
#define SZ_BT_40141 sizeof(BT_40141)

//---------------------------------------------------------------------
// 회원 추가,수정 요청. IN 40150.
//---------------------------------------------------------------------
typedef struct
{
    int    level;           //관리자 레벨
    char   mng_id[20];      //관리자 ID
    
    char   oper;            // "I":추가   "U":수정
	char   id[20];          // User ID - 수정시에는 키로 사용.
	char   pass[20];        // Password
	char   name[40];	    // 이름
	char   tel[20];	        // 082-043-1234-1234
	char   email[80];		// 이메일주소
	char   zip[8];			// 우편번호
	char   address1[120];	// 주소 Main                            
	char   address2[60];	    // 주소 Sub
    int    is_use;          // 0: 사용, 1: 미사용
    int    mem_level;       // 회원등급 0: 최고운영자 1:운영자 10:중간관리자 20:일반
    double fu_fee;          //선물수수료 * 10000
    double op_fee;          //옵션수수료 * 10000

	char   bank[40];		// 은행명
	char   owner[40];       // 예금주
	char   mem_acc_no[20];	// 출금 계좌번호

    double fu_dambo_unit;   //선물담보금기준
    double op_dambo_unit;   //옵션담보금기준
    double fo_dambo_unit;   //선물/옵션담보금기준
    double fu_dambo_min;    //선물최소담보금
    double op_dambo_min;    //옵션최소담보금
    double fo_dambo_min;    //선물/옵션최소담보금
    double fu_loan;    	    //선물기본대출금
    double op_loan;    	    //옵션기본대출금
    double fo_loan;    	    //선물/옵션기본대출금

    int    daily_charge;    //일이용료
    int    weekly_charge;   //주이용료
    int    monthly_charge;  //월이용료

    char   oper_id[20];     //상담담당자 id
	char   mo_acc[12];      //모계좌id
	int	   limit_qty;		 //최대신청가능 구좌수
	int    site;				 //site 2011.01.13추가
	char	jumin1[8];		//주민번호 앞자리 6
	char	jumin2[8];		//주민번호 뒷자리 7
} BT_40150;
#define SZ_BT_40150 sizeof(BT_40150)

//---------------------------------------------------------------------
// 회원 추가,수정 요청 응답. OUT 40151.
//---------------------------------------------------------------------
typedef struct
{
	int   err_no;           //0:정상 1~:오류
	char  msg[80];	        //오류시 메시지
} BT_40151;
#define SZ_BT_40151 sizeof(BT_40151)


//---------------------------------------------------------------------
// 계좌 추가,수정 요청. IN 40160.
//---------------------------------------------------------------------
typedef struct
{
    int   level;            //관리자 레벨
    char  mng_id[20];       //관리자 ID
    
    char  id[20];           //계좌소유 회원 ID
    char  oper;             //'I':추가   'U':수정
    int   acc_type;         //계좌유형 0: 실전투자 99:모의투자 *수정시 불가항목
    char  acc_no[12];       //계좌번호                         *수정시 불가항목
    char  acc_pass[8];      //계좌비밀번호
} BT_40160;
#define SZ_BT_40160 sizeof(BT_40160)

//---------------------------------------------------------------------
// 계좌 추가,수정 요청 응답. OUT 40161.
//---------------------------------------------------------------------
typedef struct
{
	int   err_no;           //0:정상 1~:오류
	char  msg[80];	        //오류시 메시지
} BT_40161;
#define SZ_BT_40161 sizeof(BT_40161)

//---------------------------------------------------------------------
// 계좌 생성 요청. IN 40170.
//---------------------------------------------------------------------
typedef struct
{
    int   level;            //관리자 레벨
    char  mng_id[20];       //관리자 ID    
    int   acc_type;         //계좌유형 0: 실전투자 99:모의투자 *수정시 불가항목
} BT_40170;
#define SZ_BT_40170 sizeof(BT_40170)

//---------------------------------------------------------------------
// 계좌 생성 응답. OUT 40171.
//---------------------------------------------------------------------
typedef struct
{
	int   err_no;           //0:정상 1~:오류
	char  msg[80];	        //오류시 메시지
    char  acc_no[12];       //계좌번호
} BT_40171;
#define SZ_BT_40171 sizeof(BT_40171)

//---------------------------------------------------------------------
// ID 중복확인. IN 40180.
//---------------------------------------------------------------------
typedef struct
{
    int   level;            //관리자 레벨
    char  mng_id[20];       //관리자 ID
    char  id[20];           //User ID
} BT_40180;
#define SZ_BT_40180 sizeof(BT_40180)

//---------------------------------------------------------------------
// ID 중복확인 응답. OUT 40181.
//---------------------------------------------------------------------
typedef struct
{
	int   err_no;           //0:정상 1~:오류
	char  msg[80];	        //오류시 메시지    
} BT_40181;
#define SZ_BT_40181 sizeof(BT_40181)

//---------------------------------------------------------------------
// 상담 담당자 목록 요청. 40190
//---------------------------------------------------------------------
typedef struct
{
    int     level;          //관리자 레벨
    char    mng_id[20];     //관리자 ID

    int     oper_level;      //요청할 담당자 목록의 레벨.
} BT_40190;
#define SZ_BT_40190 sizeof(BT_40190)

//---------------------------------------------------------------------
// 상담 담당자 목록 응답 OUT 40191. 
//---------------------------------------------------------------------
typedef	struct
{
	int		err_no;			// 0 : 정상 1 ~: 오류
	char	msg[80];
} BT_40191;
#define SZ_BT_40191	sizeof(BT_40191)

typedef struct
{
    char    id[20];     //ID
	char    name[40];   //이름
} BT_40191_REPEAT;
#define SZ_BT_40191_REPEAT sizeof(BT_40191_REPEAT)



//---------------------------------------------------------------------
// 공지사항 관리 전체리스트 IN 40200
//---------------------------------------------------------------------
typedef struct
{
    int     level;            //관리자 레벨
    char    mng_id[20];       //관리자 ID
} BT_40200;
#define SZ_BT_40200 sizeof(BT_40200)

//---------------------------------------------------------------------
// 공지사항 관리 전체리스트 조회 응답 OUT 40201.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;     // 0 : 정상 1 ~: 오류
	char    msg[80];    //오류시 메시지
} BT_40201;
#define SZ_BT_40201 sizeof(BT_40201)

typedef struct
{
    char    date[8];      //YYYYMMDD
    char    time[6];      //HHmmSS
    int     datanum;        //공지사항일련번호
    int     gubun;          //2-공지사항, 3-긴급공지
    uchar   id[20];         //사용자 아이디 - 전체공지이면 ""
	char    title[92];      //공지제목
} BT_40201_REPEAT;
#define SZ_BT_40201_REPEAT sizeof(BT_40201_REPEAT)

//---------------------------------------------------------------------
// 공지사항 삭제 IN 40210.
//---------------------------------------------------------------------
typedef struct
{
    int     level;            //관리자 레벨
    char    mng_id[20];       //관리자 ID
} BT_40210;
#define SZ_BT_40210 sizeof(BT_40210)

typedef struct
{
	int     no;				    //삭제할 공지사항 일련번호
} BT_40210_REPEAT;
#define SZ_BT_40210_REPEAT sizeof(BT_40210_REPEAT)

//---------------------------------------------------------------------
// 공지사항 삭제 응답. OUT 40211.
//---------------------------------------------------------------------
typedef struct
{
	int   err_no;		// 0 : 정상 1 ~: 오류
	char  msg[80];      // 오류시 메시지
} BT_40211;
#define SZ_BT_40211 sizeof(BT_40211)

//---------------------------------------------------------------------
// 공지사항 전송요청 IN 40220.
//---------------------------------------------------------------------
typedef struct
{
    int     level;          //관리자 레벨
    char    mng_id[20];     //관리자 ID

    char    id[20];		    //수신자ID "" = 전체
    int     nGubun;         //2-공지사항, 3-긴급공지
	char    title[92];    //공지제목
	char    content[2048];	//공지내용
} BT_40220;
#define SZ_BT_40220 sizeof(BT_40220)

//---------------------------------------------------------------------
// 공지사항 전송요청 응답. OUT 40221.
//---------------------------------------------------------------------
typedef struct
{
	int   err_no;		    // 0 : 정상 1 ~: 오류
	char  msg[80];          // 오류시 메시지
} BT_40221;
#define SZ_BT_40221 sizeof(BT_40221)

//---------------------------------------------------------------------
// 공지사항 상세내용 요청 IN 40230.
//---------------------------------------------------------------------
typedef struct
{
    int     level;          //관리자 레벨
    char    mng_id[20];     //관리자 ID
	int     nDataNum;	    //공지사항 일련번호 : 수정시에는 Key
} BT_40230;
#define SZ_BT_40230 sizeof(BT_40230)

//---------------------------------------------------------------------
// 공지사항 상세내용 응답. OUT 40231.
//---------------------------------------------------------------------
typedef struct
{
	int   err_no;		    //0 : 정상 1 ~: 오류
	char  msg[80];          //오류시 메시지

    char  content[2048];     //공지내용
} BT_40231;
#define SZ_BT_40231 sizeof(BT_40231)

//---------------------------------------------------------------------
// 공지전송 대상 리스트 조회 IN 40240.
//---------------------------------------------------------------------
typedef struct
{
    int     level;          //관리자 레벨
    char    mng_id[20];     //관리자 ID
    char    name[40];       //조회할 이름 또는 ID
} BT_40240;
#define SZ_BT_40240 sizeof(BT_40240)

//---------------------------------------------------------------------
// 공지사항 상세내용 응답. OUT 40241.
//---------------------------------------------------------------------
typedef struct
{
	int   err_no;		    //0 : 정상 1 ~: 오류
	char  msg[80];          //오류시 메시지
} BT_40241;
#define SZ_BT_40241 sizeof(BT_40241)

typedef struct
{
    char    id[20];         //사용자 아이디
	char    name[40];	    //이름
	char    tel[20];	    //연락처
} BT_40241_REPEAT;
#define SZ_BT_40241_REPEAT sizeof(BT_40241_REPEAT)

//---------------------------------------------------------------------
// 실시간 요청사항 수신. OUT 40301
//---------------------------------------------------------------------
typedef struct
{
	char    hhnnss[6];      //요청시간.
	int     reqno;			//요청사항 일련번호
	char    acc_no[12];		//계좌번호
	char    id[20];			//회원아이디
	char    name[40];		//회원명
	int     type;			//0:입금	1:출금	2:기타
	int     value;          //거래금액
	char    owner[40];      //입금자
	int		site;
} BT_40301;
#define SZ_BT_40301 sizeof(BT_40301)

//---------------------------------------------------------------------
// 요청수신 조회 IN 40310.
//---------------------------------------------------------------------
typedef struct
{
    int     level;          //관리자 레벨
    char    mng_id[20];     //관리자 ID
} BT_40310;
#define SZ_BT_40310 sizeof(BT_40310)

//---------------------------------------------------------------------
// 요청수신 조회 응답. OUT 40311.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    // 0 : 정상 1 ~: 오류
	char    msg[80];        // 오류시 메시지
} BT_40311;
#define SZ_BT_40311 sizeof(BT_40311)

typedef struct
{
    char    hhnnss[6];      //요청시간.
	int     reqno;			//요청사항 일련번호
	char    acc_no[12];		//계좌번호
    char    id[20];			//회원아이디
	char    name[40];		//회원명
	int     type;			//0:입금	1:출금	2:기타
    int     value;          //거래금액
	char    owner[40];      //입금자
	int     site;
} BT_40311_REPEAT;
#define SZ_BT_40311_REPEAT sizeof(BT_40311_REPEAT)

//---------------------------------------------------------------------
// 요청수신 조회 IN 40400.
//---------------------------------------------------------------------
typedef struct
{
	int     level;          //관리자 레벨
    char    mng_id[20];     //관리자 ID
    char    yyyymmdd[8];    //기준일
	char    type[2];        //"00":입출금제외 "10":입금만 "01":출금만 "11":입금+출금
} BT_40400;
#define SZ_BT_40400 sizeof(BT_40400)

//---------------------------------------------------------------------
// 요청수신 조회 응답. OUT 40401.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    // 0 : 정상 1 ~: 오류
	char    msg[80];        // 오류시 메시지
} BT_40401;
#define SZ_BT_40401 sizeof(BT_40401)

typedef struct
{
	//시분초. 일시. 표시는 시분까지만.
	char    hhnnss[6];      //요청시간.
	int     reqno;			//요청사항 일련번호
	char    acc_no[12];		//계좌번호
	char    id[20];			//회원아이디
	char    name[40];		//회원명
	int     type;			//0:입금	1:출금	2:기타
	int     value;          //거래금액
	char    owner[40];      //예금주
	int     site;			//site
} BT_40401_REPEAT;
#define SZ_BT_40401_REPEAT sizeof(BT_40401_REPEAT)

//---------------------------------------------------------------------
// 처리내용조회 IN 40410.
//---------------------------------------------------------------------
typedef struct
{
	int     level;          //관리자 레벨
	char    mng_id[20];     //관리자 ID
	char    yyyymmdd[8];    //기준일
    char    type[2];        //"00":입출금제외 "10":입금만 "01":출금만 "11":입금+출금
} BT_40410;
#define SZ_BT_40410 sizeof(BT_40410)

//---------------------------------------------------------------------
// 처리내용조회 응답. OUT 40411.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    // 0 : 정상 1 ~: 오류
	char    msg[80];        // 오류시 메시지
} BT_40411;
#define SZ_BT_40411 sizeof(BT_40411)

typedef struct
{
    //처리 시분.
    char    hhnnss[6];      //요청시간.
	int     reqno;			//요청사항 일련번호
	char    acc_no[12];		//계좌번호
    char    id[20];			//회원아이디
	char    name[40];		//회원명
	int     type;			//0:입금	1:출금	2:기타
    double  value;          //거래금액
    double  fee;            //수수료
	char    bank[40];		//은행명
	char    mem_acc_no[20];	//출금계좌번호
    char    in_name[40];    //입금인 또는 예금주
    char    result[12];     //이체상태 0:처리중 1:처리완료
	char    yyyymmdd[8];    //이체일
	char	oper_id[20];	//입출금처리한 운영자ID
	int     site;
} BT_40411_REPEAT;
#define SZ_BT_40411_REPEAT sizeof(BT_40411_REPEAT)

//---------------------------------------------------------------------
// 고객정보 조회요청 IN 40420.
//---------------------------------------------------------------------
typedef struct
{
    int     level;          //관리자 레벨
    char    mng_id[20];     //관리자 ID
    int     reqno;          //요청일련번호
} BT_40420;
#define SZ_BT_40420 sizeof(BT_40420)

//---------------------------------------------------------------------
// 고객정보 조회응답. OUT 40421.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    // 0 : 정상 1 ~: 오류
	char    msg[80];        // 오류시 메시지

	char    acc_no[12];     //계좌번호
	char	name[40];		//회원명
    char	id[20];			//회원ID
	char    tel[20];	    //연락처
	double	dambo;			//담보금
	double  out_possible;   //출금:출금가능금액
    char    bank[40];	    //출금:은행명
    char    owner[40];      //출금:예금주
    char    mem_acc_no[20];	//출금:출금계좌번호
    char    in_name[40];    //입금:입금인
	int		limit_qty;		 //최대신청가능 구좌수
} BT_40421;
#define SZ_BT_40421 sizeof(BT_40421)

//---------------------------------------------------------------------
// 입출금 요청 IN 40430.
//---------------------------------------------------------------------
typedef struct
{
    int     level;      //관리자 레벨
    char    mng_id[20]; //관리자 ID
    int     type;       //1:입금   2:출금
	int     no;		    //요청사항 일련번호 :없으면 -1
    char    acc_no[12];	//계좌번호    
    double  money;      //입금액 또는 출금액
    char    memo[100];  //메모
    int     process;    // 처리유형 1:요청 , 0:요청취소
} BT_40430;
#define SZ_BT_40430 sizeof(BT_40430)

//---------------------------------------------------------------------
// 입출금 요청 응답. OUT 40431.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		        //0 : 정상 1 ~: 오류
	char    msg[80];            //오류시 메시지
    char    result[12];         //처리결과 0:-  1:완료 2:취소  3:기타
    int     no;                 //처리된 원 요청번호 : 없으면 -1
} BT_40431;
#define SZ_BT_40431 sizeof(BT_40431)

//---------------------------------------------------------------------
// 기타 입출금 계좌 리스트 요청 IN 40440.
//---------------------------------------------------------------------
typedef struct
{
    int     level;      //관리자 레벨
    char    mng_id[20]; //관리자 ID
    char    name[40];       //조회할 이름 또는 ID
} BT_40440;
#define SZ_BT_40440 sizeof(BT_40440)

//---------------------------------------------------------------------
// 기타 입출금 계좌 리스트 응답. OUT 40441.
//---------------------------------------------------------------------
typedef struct
{
	int   err_no;		    //0 : 정상 1 ~: 오류
	char  msg[80];          //오류시 메시지
} BT_40441;
#define SZ_BT_40441 sizeof(BT_40441)

typedef struct
{
    char    acc_no[12];	    //계좌번호    
    char    id[20];         //사용자 아이디
	char    name[40];	    //이름
} BT_40441_REPEAT;
#define SZ_BT_40441_REPEAT sizeof(BT_40441_REPEAT)


//---------------------------------------------------------------------
// 고객정보 조회요청 IN 40450.
//---------------------------------------------------------------------
typedef struct
{
    int     level;          //관리자 레벨
    char    mng_id[20];     //관리자 
    char    acc_no[12];	    //계좌번호    
} BT_40450;
#define SZ_BT_40450 sizeof(BT_40450)

//---------------------------------------------------------------------
// 고객정보 조회응답. OUT 40451.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    // 0 : 정상 1 ~: 오류
	char    msg[80];        // 오류시 메시지

    char    acc_no[12];     //계좌번호
	char	name[40];		//회원명
    char	id[20];			//회원ID
	char    tel[20];	    //연락처
	double	dambo;			//담보금
    double  out_possible;   //출금:출금가능금액
    char    bank[40];	    //출금:은행명
    char    owner[40];      //출금:예금주
    char    mem_acc_no[20];	//출금:출금계좌번호
	int		limit_qty;		 //최대신청가능 구좌수
} BT_40451;
#define SZ_BT_40451 sizeof(BT_40451)

//---------------------------------------------------------------------
// 회원 사용정보 조회 IN 40460.
//---------------------------------------------------------------------
typedef struct
{
    int     level;          //관리자 레벨
    char    mng_id[20];     //관리자 
	char    id[20];     	//회원 ID
} BT_40460;
#define SZ_BT_40460 sizeof(BT_40460)

//---------------------------------------------------------------------
// 회원 사용정보 조회 응답. OUT 40461.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    // 0 : 정상 1 ~: 오류
	char    msg[80];        // 오류시 메시지

	char    bank[40];	    //은행명
    char    owner[40];      //예금주
	char    mem_acc_no[20];	//출금 계좌번호
	
    double  fu_fee;         //선물수수료
    double  op_fee;         //옵션수수료
    double  fu_dambo_unit;  //선물담보금기준
    double  op_dambo_unit;  //옵션담보금기준
    double  fo_dambo_unit;  //선물/옵션담보금기준
    double  fu_dambo_min;   //선물최소담보금
    double  op_dambo_min;   //옵션최소담보금
    double  fo_dambo_min;   //선물/옵션최소담보금
    double  fu_loan;    	//선물기본대출금
    double  op_loan;    	//옵션기본대출금
    double  fo_loan;    	//선물/옵션기본대출금

    int     daily_charge;   //일이용료
    int     weekly_charge;  //주이용료
    int     monthly_charge;	//월이용료
	int		limit_qty;		//최대신청가능 구좌수
} BT_40461;
#define SZ_BT_40461 sizeof(BT_40461)

//---------------------------------------------------------------------
// 회원 사용정보 수정 요청 IN 40470.
//---------------------------------------------------------------------
typedef struct
{
    int     level;          //관리자 레벨
    char    mng_id[20];     //관리자 	
	char    id[20];     	//회원 ID. 수정요청 대상.
	
	char    bank[40];	    //은행명
    char    owner[40];      //예금주
	char    mem_acc_no[20];	//출금 계좌번호
	
    double  fu_fee;         //선물수수료
    double  op_fee;         //옵션수수료
    double  fu_dambo_unit;  //선물담보금기준
    double  op_dambo_unit;  //옵션담보금기준
    double  fo_dambo_unit;  //선물/옵션담보금기준
    double  fu_dambo_min;   //선물최소담보금
    double  op_dambo_min;   //옵션최소담보금
    double  fo_dambo_min;   //선물/옵션최소담보금
    double  fu_loan;    	//선물기본대출금
    double  op_loan;    	//옵션기본대출금
    double  fo_loan;    	//선물/옵션기본대출금

    int     daily_charge;   //일이용료
    int     weekly_charge;  //주이용료
    int     monthly_charge;	//월이용료
	int		limit_qty;		//최대신청가능 구좌수
} BT_40470;
#define SZ_BT_40470 sizeof(BT_40470)

//---------------------------------------------------------------------
// 회원 사용정보 수정 요청 응답. OUT 40471.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    // 0 : 정상 1 ~: 오류
	char    msg[80];        // 오류시 메시지
} BT_40471;
#define SZ_BT_40471 sizeof(BT_40471)

//---------------------------------------------------------------------
// 회원리스트 조회 IN 40500.
//---------------------------------------------------------------------
typedef struct
{
    int     level;      //관리자 레벨
    char    mng_id[20]; //관리자 ID
    int     type;       //0:전체 1:실접속자만
} BT_40500;
#define SZ_BT_40500 sizeof(BT_40500)

//---------------------------------------------------------------------
// 회원리스트 조회 응답. OUT 40501.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		        // 0 : 정상 1 ~: 오류
	char    msg[80];            // 오류시 메시지
} BT_40501;
#define SZ_BT_40501 sizeof(BT_40501)

typedef struct
{
    char    acc_no[12];	    //계좌번호    
    char    id[20];			//회원 아이디
	char    name[40];       //회원명
    int     is_login;       //접속 여부 1:접속 0:미접속
    int     jango;          //잔고 1:있음  0:없음
	int     losscut;        //반대매매 1:있음  0:없음
    int     remain;         //미체결 1:있음 0:없음
} BT_40501_REPEAT;
#define SZ_BT_40501_REPEAT sizeof(BT_40501_REPEAT)

//---------------------------------------------------------------------
// 계좌잔고 요청 IN 40510.
//---------------------------------------------------------------------
typedef struct
{
    int     level;          //관리자 레벨
    char    mng_id[20];     //관리자 ID
    char    acc_no[12];	    //계좌번호    
} BT_40510;
#define SZ_BT_40510 sizeof(BT_40510)

//---------------------------------------------------------------------
// 계좌잔고 요청 응답. OUT 40511.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		        // 0 : 정상 1 ~: 오류
	char    msg[80];            // 오류시 메시지
} BT_40511;
#define SZ_BT_40511 sizeof(BT_40511)

typedef struct
{
	int     acode;             //종목코드
	char 	jomok_name[40];    //종목명
	char 	gubun[12];      	//매매구분 (매도, 매수)

	int		quantity;           //잔고수량
	double	in_price;           //매입가격
	double	cur_price;          //현재가
	double	calc_sonik;         //평가손익
	double	buy_value;          //매입금액
	double	avg_price;          //평균단가
} BT_40511_REPEAT;
#define SZ_BT_40511_REPEAT sizeof(BT_40511_REPEAT)

//---------------------------------------------------------------------
// 계좌미체결 요청 IN 40520.
//---------------------------------------------------------------------
typedef struct
{
    int     level;          //관리자 레벨
    char    mng_id[20];     //관리자 ID
    char    acc_no[12];	    //계좌번호    
} BT_40520;
#define SZ_BT_40520 sizeof(BT_40520)

//---------------------------------------------------------------------
// 계좌미체결 요청 응답. OUT 40521.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		        // 0 : 정상 1 ~: 오류
	char    msg[80];            // 오류시 메시지
} BT_40521;
#define SZ_BT_40521 sizeof(BT_40521)

typedef struct
{
    char    datetime[14];   //주문날짜시간(YYYYMMDDHHmmSS)
    int     no;             //주문번호
    int     acode;          //종목코드
    char    jongmok[40];    //종목명
    char    gubun[12];      //주문구분 (매도, 매수, 정정, 취소)
    char    type[16];       //주문유형 (지정가 시장가 조건부지정가 최유리지정가)
    int     quantity;       //미체결수량
    double  order_price;    //주문단가
    double  cur_price;      //현재가
    int     order_no;       //원주문번호
} BT_40521_REPEAT;
#define SZ_BT_40521_REPEAT sizeof(BT_40521_REPEAT)

//---------------------------------------------------------------------
// 계좌예탁금 조회 요청 IN 40530.
//---------------------------------------------------------------------
typedef struct
{
    int     level;          //관리자 레벨
    char    mng_id[20];     //관리자 ID
    char    acc_no[12];	    //계좌번호    
} BT_40530;
#define SZ_BT_40530 sizeof(BT_40530)

//---------------------------------------------------------------------
// 계좌예탁금 조회 응답. OUT 40531.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		        // 0 : 정상 1 ~: 오류
	char    msg[80];            // 오류시 메시지

    char    acc_kind[12];           //'가입상품 (0:선물, 1:옵션, 3:선옵];',
    int     acc_qty;                //거래구좌수
    char    status[12];             //계좌상태
    char    today_use[12];          //당일계좌신청상태  - "신청", "미신청"
    int     coupon;                 //쿠폰
    double  out_possible;           //출금가능금액 - dChulgmGanungAtm
    double  order_possible;         //주문가능금액 - dJumunGanungAtm

	double  calc_yetak;		        //평가예탁총액
    double  yetak;                  //개장시 예탁총액
    double  calc_dambo;             //평가담보금
    double  dambo;                  //개장시담보금
    double  dambo_min;              //로스컷설정금액
	double  remain_losscut;         //로스컷 여유금액

    double  jumun_deposit;          //주문증거금
    double  fu_jumun_deposit;       //선물주문증거금
    double  op_mesu_jumun_deposit;  //옵션매수주문증거금
    double  op_mudo_jumun_deposit;  //옵션매도주문증거금
    double  calc_sonik;             //총잔고 평가손익
    double  fu_calc_sonik;          //선물 평가손익 
    double  op_calc_sonik;          //옵션 평가손익 

    double  witak_deposit;          //위탁증거금 
    double  fu_witak_deposit;       //선물위탁증거금
    double  op_mesu_witak_deposit;  //옵션매수위탁증거금
    double  op_medo_witak_deposit;  //옵션매도위탁증거금
    double  sonik;                  //총실현손익
    double  fu_sonik;               //선물실현손익 
    double  op_sonik;               //옵션실현손익 

    int     in_rq_money;            //입금신청금액
    int     in_money;               //총입금액
    int     in_bank;                //일반입금액
    int     in_etc;                 //기타입금액
    double  fee;                    //수수료
    double  fu_fee;                 //선물수수료
    double  op_fee;                 //옵션수수료

    int     out_rq_money;           //출금신청금액
    int     out_money;              //총출금액(선결제+이용료+기타비용+은행출금액+출금수수료)
    int     out_bank;               //일반출금액
    int     out_cost;               //출금비용
    int     coupon_charge;          //쿠폰구매출금
    int     charge;                 //이용료
    int     etc_cost;               //기타비용
	
	char	rq_status[12];	// 오버나잇 상태.
} BT_40531;
#define SZ_BT_40531 sizeof(BT_40531)

//---------------------------------------------------------------------
// 선옵주문 IN  40540
//---------------------------------------------------------------------
typedef	struct
{
    char    acc_no[12];	            //계좌번호    
	int     code;                   //종목코드

	int 	quantity;               //주문수량
	double 	price;                  //주문가격
	int     order;                  //주문구분(1:매도, 2:매수, 3:정정, 4:취소)
	char    priceType;              //주문유형("L" 지정가 "M" 시장가)
	char 	filler[3];
	int     no;                     //원주문 번호 - szOjno
} BT_40540;

#define SZ_BT_40540	sizeof(BT_40540)                  

//---------------------------------------------------------------------
// 선옵주문 OUT	40541
//---------------------------------------------------------------------
typedef	struct
{
	int     err_no;		        // 0 : 정상 1 ~: 오류
	char    msg[80];            // 오류시 메시지
} BT_40541;

#define SZ_BT_40541	sizeof(BT_40541)

//---------------------------------------------------------------------
// 선옵주문 체결 OUT	40551
//---------------------------------------------------------------------
typedef	struct
{
	int     err_no;		        // 0 : 정상 1 ~: 오류
	char    msg[80];            // 오류시 메시지

    int     no;                 //주문번호
    int     code;               //종목코드
	int 	quantity;           //주문수량
	double 	price;              //주문가격
	int     order;              //주문구분(1:매도, 2:매수, 3:정정, 4:취소)
} BT_40551;

#define SZ_BT_40551	sizeof(BT_40551)

//---------------------------------------------------------------------
// 회원리스트 조회 IN 40600.
//---------------------------------------------------------------------
typedef struct
{
    int     level;          //관리자 레벨
    char    mng_id[20];     //관리자 ID
} BT_40600;
#define SZ_BT_40600 sizeof(BT_40600)

//---------------------------------------------------------------------
// 회원리스트 조회 응답. OUT 40601.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		        // 0 : 정상 1 ~: 오류
	char    msg[80];            // 오류시 메시지
} BT_40601;
#define SZ_BT_40601 sizeof(BT_40601)

typedef struct
{
	char    acc_no[12];		//계좌번호
    char    id[20];			//사용자 아이디
	char    name[40];       //회원명
    int     is_login;       //접속 여부 1:접속 0:미접속
} BT_40601_REPEAT;
#define SZ_BT_40601_REPEAT sizeof(BT_40601_REPEAT)

//---------------------------------------------------------------------
// 회원이용내역 조회 IN 40610.
//---------------------------------------------------------------------
typedef struct
{
    int     level;          // 관리자 레벨
    char    mng_id[20];     // 관리자 ID
    char    from[8];        // 조회 시작일(YYYYMMDD)
    char    to[8];          // 조회 종료일(YYYYMMDD)
	char    acc_no[12];		// 계좌번호
} BT_40610;
#define SZ_BT_40610 sizeof(BT_40610)

//---------------------------------------------------------------------
// 회원이용내역 조회 응답. OUT 40611.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    // 0 : 정상 1 ~: 오류
	char    msg[80];        // 오류시 메시지
} BT_40611;
#define SZ_BT_40611 sizeof(BT_40611)

typedef struct
{
    char    datetime[14];   //이용일시
    char    connectIP[40];  //접속IP 111.222.333.444(00.11.22.33)
    char    contents[204]; //내용
} BT_40611_REPEAT;
#define SZ_BT_40611_REPEAT sizeof(BT_40611_REPEAT)


//---------------------------------------------------------------------
// 입출금내역 조회 요청 IN 40620.
//---------------------------------------------------------------------
typedef struct
{
    int     level;          // 관리자 레벨
    char    mng_id[20];     // 관리자 ID
    char    from[8];        // 조회 시작일(YYYYMMDD)
    char    to[8];          // 조회 종료일(YYYYMMDD)
	char    acc_no[12];		// 계좌번호
} BT_40620;
#define SZ_BT_40620 sizeof(BT_40620)

//---------------------------------------------------------------------
// 입출금내역 조회 응답. OUT 40621.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    // 0 : 정상 1 ~: 오류
	char    msg[80];        // 오류시 메시지
} BT_40621;
#define SZ_BT_40621 sizeof(BT_40621)

typedef struct
{
    char    datetime[14];   //거래일시
	int     type;			//0:입금	1:출금	2:기타
    double  value;          //거래금액
    double  fee;            //수수료
	char    bank[40];		//은행명
	char    mem_acc_no[20];	//출금계좌번호
    char    in_name[40];    //입금자
    char    owner[40];      //예금주
    char    result[12];     //이체상태 0:처리중 1:처리완료
} BT_40621_REPEAT;
#define SZ_BT_40621_REPEAT sizeof(BT_40621_REPEAT)

//---------------------------------------------------------------------
// 거래내역 조회 요청 IN 40630.
//---------------------------------------------------------------------
typedef struct
{
    int     level;          // 관리자 레벨
    char    mng_id[20];     // 관리자 ID
    char    from[8];        // 조회 시작일(YYYYMMDD)
    char    to[8];          // 조회 종료일(YYYYMMDD)
	char    acc_no[12];		// 계좌번호

    int     all;            // 0: 전체 데이터, 1: 체결데이터만.
} BT_40630;
#define SZ_BT_40630 sizeof(BT_40630)

//---------------------------------------------------------------------
// 거래내역 조회 응답. OUT 40631.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    // 0 : 정상 1 ~: 오류
	char    msg[80];        // 오류시 메시지
} BT_40631;
#define SZ_BT_40631 sizeof(BT_40631)

typedef struct
{
    char    datetime[14];    //거래일
    int     no;             //주문번호
	char 	name[40];       //종목명
    char    type[12];       //지정가,시장가 등.
    char    gubun[12];      //매수,매도,정정,취소
    double  order_price;    //주문가
    int     order_quantity; //주문량
    double  trade_price;    //체결가
    int     trade_quantity; //체결량
    int     real_no;        //원주문 번호
    char    status[20];     //주문상태.
    char    chegyultime[14];    //거래일
} BT_40631_REPEAT;
#define SZ_BT_40631_REPEAT sizeof(BT_40631_REPEAT)

//---------------------------------------------------------------------
// 손익내역 조회 요청 IN 40640.
//---------------------------------------------------------------------
typedef struct
{
    int     level;          // 관리자 레벨
    char    mng_id[20];     // 관리자 ID
    char    from[8];        // 조회 시작일(YYYYMMDD)
    char    to[8];          // 조회 종료일(YYYYMMDD)
	char    acc_no[12];		// 계좌번호
} BT_40640;
#define SZ_BT_40640 sizeof(BT_40640)

//---------------------------------------------------------------------
// 손익내역 조회 응답. OUT 40641.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    // 0 : 정상 1 ~: 오류
	char    msg[80];        // 오류시 메시지
} BT_40641;
#define SZ_BT_40641 sizeof(BT_40641)

typedef struct
{
    char    datetime[14];    // 날짜
    double  sonik;          //실현손익
    double  calc_sonik;     //평가손익
    double  fee;            //수수료 - dFuCmi
    double	sun_sonik;		//순손익
    double  dambo;          //평가담보금
    int     in_money;       //입금
    int     out_money;      //출금
} BT_40641_REPEAT;
#define SZ_BT_40641_REPEAT sizeof(BT_40641_REPEAT)

//---------------------------------------------------------------------
// 모계좌 조회 요청 IN 40700.
//---------------------------------------------------------------------
typedef struct
{
    int     level;      //관리자 레벨
    char    mng_id[20]; //관리자 id
    char    id[20];     //모계좌 id
} BT_40700;
#define SZ_BT_40700 sizeof(BT_40700)

//---------------------------------------------------------------------
// 모계좌 조회 응답. OUT 40701.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    // 0 : 정상 1 ~: 오류
	char    msg[80];        // 오류시 메시지

	char    id[20];     //모계좌 id
	    
    double  calc_yetak;     //평가예탁총액
    double  yetak;          //예탁총액
    double  out_possible;   //인출가능금액
    double  order_possible; //주문가능금액

    double  eval_tot;       //실현손익
    double  sonik_tot;      //평가손익
    double  fee;            //수수료
    
    int     mem_account;    //회원계좌
    int     today_use;      //금일사용신청
    int     today_account;  //금일사용구좌    
} BT_40701;
#define SZ_BT_40701 sizeof(BT_40701)

//---------------------------------------------------------------------
// 모계좌 잔고 조회 IN 40710.
//---------------------------------------------------------------------
typedef struct
{
    int     level;      //관리자 레벨
    char    mng_id[20]; //관리자 id
    char    id[20];     //모계좌 id
} BT_40710;
#define SZ_BT_40710 sizeof(BT_40710)

//---------------------------------------------------------------------
// 모계좌 잔고 응답. OUT 40711.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    // 0 : 정상 1 ~: 오류
	char    msg[80];        // 오류시 메시지
	char    id[20];     //모계좌 id
} BT_40711;
#define SZ_BT_40711 sizeof(BT_40711)

typedef struct
{
	char 	name[40];       //종목명
    char    type[12];       //1:매수   2:매도  3:정정  4:취소
    int     quantity;       //수량
} BT_40711_REPEAT;
#define SZ_BT_40711_REPEAT sizeof(BT_40711_REPEAT)

//---------------------------------------------------------------------
// 회원계좌 잔고 조회 IN 40720.
//---------------------------------------------------------------------
typedef struct
{
    int     level;      //관리자 레벨
    char    mng_id[20]; //관리자 id
    char    id[20];     //모계좌 id
} BT_40720;
#define SZ_BT_40720 sizeof(BT_40720)

//---------------------------------------------------------------------
// 회원계좌 잔고 응답. OUT 40721.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    // 0 : 정상 1 ~: 오류
	char    msg[80];        // 오류시 메시지
	char    id[20];     //모계좌 id
} BT_40721;
#define SZ_BT_40721 sizeof(BT_40721)

typedef struct
{
	char 	name[40];       //종목명
    char    type[12];       //1:매수   2:매도  3:정정  4:취소
    int     quantity;       //수량
} BT_40721_REPEAT;
#define SZ_BT_40721_REPEAT sizeof(BT_40721_REPEAT)

//---------------------------------------------------------------------
// 모계좌 미체결 조회 IN 40730.
//---------------------------------------------------------------------
typedef struct
{
    int     level;      //관리자 레벨
    char    mng_id[20]; //관리자 id
    char    id[20];     //모계좌 id
} BT_40730;
#define SZ_BT_40730 sizeof(BT_40730)

//---------------------------------------------------------------------
// 모계좌 미체결조회 응답. OUT 40731.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    // 0 : 정상 1 ~: 오류
	char    msg[80];        // 오류시 메시지
	char    id[20];     //모계좌 id
} BT_40731;
#define SZ_BT_40731 sizeof(BT_40731)

typedef struct
{
    int     no;             //주문번호
	char 	name[40];       //종목명
    char    type[12];       //1:매수   2:매도  3:정정  4:취소
    int     quantity;       //수량
} BT_40731_REPEAT;
#define SZ_BT_40731_REPEAT sizeof(BT_40731_REPEAT)

//---------------------------------------------------------------------
// 회원계좌 미체결 조회 IN 40740.
//---------------------------------------------------------------------
typedef struct
{
    int     level;      //관리자 레벨
    char    mng_id[20]; //관리자 id
    char    id[20];     //모계좌 id
} BT_40740;
#define SZ_BT_40740 sizeof(BT_40740)

//---------------------------------------------------------------------
// 회원계좌 미체결조회 응답. OUT 40741.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    // 0 : 정상 1 ~: 오류
	char    msg[80];        // 오류시 메시지
	char    id[20];     //모계좌 id
} BT_40741;
#define SZ_BT_40741 sizeof(BT_40741)

typedef struct
{
    int     real_no;        //모계좌주문번호
    int     no;             //주문번호
	char 	name[40];       //종목명
    char    type[12];       //1:매수   2:매도  3:정정  4:취소
    int     quantity;       //수량
	char	user_id[20];	//보유한 회원ID
	char    accno[20];      //보유한 회원 계좌번호
} BT_40741_REPEAT;
#define SZ_BT_40741_REPEAT sizeof(BT_40741_REPEAT)

//---------------------------------------------------------------------
// 모계좌 리스트 IN 40750.
//---------------------------------------------------------------------
typedef struct
{
	int     level;      //관리자 레벨
	char    mng_id[20]; //관리자 id
} BT_40750;
#define SZ_BT_40750 sizeof(BT_40750)

//---------------------------------------------------------------------
// 모계좌 리스트 응답. OUT 40751.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    // 0 : 정상 1 ~: 오류
	char    msg[80];        // 오류시 메시지
} BT_40751;
#define SZ_BT_40751 sizeof(BT_40751)

typedef struct
{
	char 	mo_acc[12];     //모계좌ID
} BT_40751_REPEAT;
#define SZ_BT_40751_REPEAT sizeof(BT_40751_REPEAT)

//---------------------------------------------------------------------
// 모계좌,회원계좌 잔고 및 미체결 - 모계좌 선옵주문 IN  40760
//---------------------------------------------------------------------
typedef	struct
{
	char    mo_acc[20];       	//모계좌ID
	int     nAcode;             //종목코드 - szJomokcd
	int 	nJsu;           	//주문수량 - szJsu
	double 	dJamt;           	//주문가격 - szJamt
	int     nJgubun;            //주문구분(1:매도, 2:매수, 3:정정, 4:취소) - 3: 모계좌 잔고조정시 정정은 하지않음.
	int     nFOgubun;           //선물옵션구분(1:선물, 2:옵션) - szSOgubun
	char    szJtype;           	//주문유형("L" 지정가 "M" 시장가 "C" 조건부지정가 "B" 최유리지정가) - L,M 만 사용.
	char 	filler[3];
	int     nOjno;              //원주문 번호 - szOjno : 취소일때만 사용.
} BT_40760;

#define SZ_BT_40760	sizeof(BT_40760)

//---------------------------------------------------------------------
// 모계좌,회원계좌 잔고 및 미체결 - 모계좌 선옵주문 OUT	40761
//---------------------------------------------------------------------
typedef	struct
{
	int   	err_no;				//0 : 정상 1 ~: 오류번호
	uchar 	msg[80];			//오류시 메시지
	char    mo_acc[20];       	//모계좌ID
	int     nAcode;         	//종목코드
} BT_40761;

#define SZ_BT_40761	sizeof(BT_40761)

//---------------------------------------------------------------------
// 모계좌,회원계좌 잔고 및 미체결 - 회원계좌 미체결 취소 IN 40770
//---------------------------------------------------------------------
typedef	struct
{
	int     nOjno;              //회원계좌 미체결 주문번호
} BT_40770;

#define SZ_BT_40770	sizeof(BT_40770)

//---------------------------------------------------------------------
// 모계좌,회원계좌 잔고 및 미체결 - 회원계좌 미체결 취소 OUT	40771
//---------------------------------------------------------------------
typedef	struct
{
	int   	err_no;		//0 : 정상 1 ~: 오류번호
	uchar 	msg[80];	//오류시 메시지
	int		nJno;       //주문번호
} BT_40771;

#define SZ_BT_40771	sizeof(BT_40771)

//---------------------------------------------------------------------
// 회원/모계좌 잔고/미체결 real조회 - 관리자전용 40780
//---------------------------------------------------------------------
typedef	struct
{
    int     level;      //관리자 레벨
	char    mng_id[20]; //관리자 id
} BT_40780;

#define SZ_BT_40780	sizeof(BT_40780)

//---------------------------------------------------------------------
// 종합리포트 조회 요청 IN 40800.
//---------------------------------------------------------------------
typedef struct
{
    int     level;      //관리자 레벨
    char    mng_id[20]; //관리자 ID
} BT_40800;
#define SZ_BT_40800 sizeof(BT_40800)

//---------------------------------------------------------------------
// 종합리포트 조회 응답. OUT 40801.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    //0 : 정상 1 ~: 오류
	char    msg[80];        //오류시 메시지
    //운영일반
    int     user_con;       //접속중인 사용자
    int     user_tot;       //전체접속자
    int     today_use;      //당일사용신청
    int     acc_cnt;        //구좌수
    double  suik_sum;       //총수익
    double  fee;            //수수료
    double  daily_fee;      //이용료
    double  sell_coupon;    //쿠폰판매
    //계좌종합
    double  dambo;          //개장시담보금
    double  calc_dambo;     //평가담보금
    double  order_possible; //주문가능금액
    double  out_possible;   //출금가능금액
    double  sum_fee;        //수수료 합계
    double  fu_fee;         //선물수수료
    double  op_fee;         //옵션수수료
    double  calc_sonik;     //평가손익
    double  fu_calc_sonik;  //선물 평가손익
    double  op_calc_sonik;  //옵션 평가손익
    double  sonik;          //실현손익
    double  fu_sonik;       //선물실현손익
    double  op_sonik;       //옵션실현손익
    //입출금종합
    double  in_rq_money;    //입금신청금액
    double  in_money;       //총입금액
    double  in_bank;        //일반입금
    double  in_etc;         //기타입금

    double  out_rq_money;   //출금신청금액
    double  out_money;      //총출금액
    double  out_bank;       //일반출금
    double  coupon_charge;  //쿠폰구매출금
    double  charge;         //이용료
    double  out_cost;       //출금비용
    double  etc_cost;       //기타비용
} BT_40801;
#define SZ_BT_40801 sizeof(BT_40801)

//---------------------------------------------------------------------
// 모계좌조회 요청 IN 40810.
//---------------------------------------------------------------------
typedef struct
{
    int     level;          //관리자 레벨
    char    mng_id[20];     //관리자 ID
    char 	mo_acc[12];     //모계좌ID
} BT_40810;
#define SZ_BT_40810 sizeof(BT_40810)

//---------------------------------------------------------------------
// 모계좌조회 응답. OUT 40811.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    // 0 : 정상 1 ~: 오류
	char    msg[80];        // 오류시 메시지

    char    id[12];			//모계좌id
    double  calc_yetak;     //평가예탁총액
    double  yetak;          //예탁총액
    double  out_possible;   //인출가능금액
    double  order_possible; //주문가능금액
    double  eval_tot;       //실현손익
    double  sonik_tot;      //평가손익
    double  fee;            //수수료
    int     jango;          //잔고 체크 0:' '  1:확인요망
    int     remain;         //미체결 체크 0:' '  1:확인요망
} BT_40811;
#define SZ_BT_40811 sizeof(BT_40811)

//---------------------------------------------------------------------
// 모계좌 체결 확인 요청 IN 40820.
//---------------------------------------------------------------------
typedef struct
{
    int     level;          //관리자 레벨
    char    mng_id[20];     //관리자 ID
    char 	mo_acc[12];     //모계좌ID
} BT_40820;
#define SZ_BT_40820 sizeof(BT_40820)

//---------------------------------------------------------------------
// 모계좌 체결 확인 응답. OUT 40821.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    // 0 : 정상 1 ~: 오류
	char    msg[80];        // 오류시 메시지
    char 	mo_acc[12];     //모계좌ID
    int     jango;          //잔고 체크 0:' '  1:확인요망
} BT_40821;
#define SZ_BT_40821 sizeof(BT_40821)

//---------------------------------------------------------------------
// 모계좌 미체결 확인 요청 IN 40830.
//---------------------------------------------------------------------
typedef struct
{
    int     level;          //관리자 레벨
    char    mng_id[20];     //관리자 ID
    char 	mo_acc[12];     //모계좌ID
} BT_40830;
#define SZ_BT_40830 sizeof(BT_40830)

//---------------------------------------------------------------------
// 모계좌 미체결 확인 응답. OUT 40831.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    // 0 : 정상 1 ~: 오류
	char    msg[80];        // 오류시 메시지
    char 	mo_acc[12];     //모계좌ID
    int     remain;         //미체결 체크 0:' '  1:확인요망
} BT_40831;
#define SZ_BT_40831 sizeof(BT_40831)

//---------------------------------------------------------------------
// 설정관리 조회 요청 IN 40900.
//---------------------------------------------------------------------
typedef struct
{
    int     level;      //관리자 레벨
    char    mng_id[20]; //관리자 ID
} BT_40900;
#define SZ_BT_40900 sizeof(BT_40900)

//---------------------------------------------------------------------
// 설정관리 조회 응답. OUT 40901.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    //0 : 정상 1 ~: 오류
	char    msg[80];        //오류시 메시지
} BT_40901;
#define SZ_BT_40901 sizeof(BT_40901)

typedef struct
{
	int     fee_day;        //일간사용료
	int     fee_week;       //주간사용료
	int     fee_month;      //월간사용료
	double  fu_fee;         //선물수수료
	double  op_fee;         //옵션수수료

	int     fu_dambo_unit;  //선물담보금기준
	int     op_dambo_unit;  //옵션담보금기준
	int     fo_dambo_unit;  //선물/옵션담보금기준
	int     fu_dambo_min;   //선물최소담보금
	int     op_dambo_min;   //옵션최소담보금
	int     fo_dambo_min;   //선물/옵션최소담보금
	int     fu_loan;    	//선물기본대출금
	int     op_loan;    	//옵션기본대출금
	int     fo_loan;    	//선물/옵션기본대출금

	char    email[40];      //문의 이메일
	char    phone[20];      //문의전화번호
	char    bank[40];       //입금은행명
	char    bank_acc[20];   //입금은행계좌번호
	char    bank_name[30];  //은행예금주명
	int		site;
	char    site_fullname[40];
	char    site_nick[40];
} BT_40901_REPEAT;
#define SZ_BT_40901_REPEAT sizeof(BT_40901_REPEAT)

//---------------------------------------------------------------------
// 설정관리 저장 요청 IN 40910.
//---------------------------------------------------------------------
typedef struct
{
    int     level;          //관리자 레벨
    char    mng_id[20];     //관리자 ID

    int     fee_day;        //일간사용료
    int     fee_week;       //주간사용료
    int     fee_month;      //월간사용료
    double  fu_fee;         //선물수수료 * 10000
    double  op_fee;         //옵션수수료 * 10000

    int     fu_dambo_unit;  //선물담보금기준
    int     op_dambo_unit;  //옵션담보금기준
    int     fo_dambo_unit;  //선물/옵션담보금기준
    int     fu_dambo_min;   //선물최소담보금
    int     op_dambo_min;   //옵션최소담보금
    int     fo_dambo_min;   //선물/옵션최소담보금
    int     fu_loan;    	//선물기본대출금
    int     op_loan;    	//옵션기본대출금
    int     fo_loan;    	//선물/옵션기본대출금

    char    email[40];      //문의 이메일
    char    phone[20];      //문의전화번호
    char    bank[40];       //입금은행명
    char    bank_acc[20];   //입금은행계좌번호
	char    bank_name[30];  //은행예금주명
	int     site;
	char    site_fullname[40];
	char    site_nick[40];
} BT_40910;
#define SZ_BT_40910 sizeof(BT_40910)

//---------------------------------------------------------------------
// 설정관리 저장 응답. OUT 40911.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    //0 : 정상 1 ~: 오류
	char    msg[80];        //오류시 메시지
} BT_40911;
#define SZ_BT_40911 sizeof(BT_40911)

//---------------------------------------------------------------------
// SITE 조회 IN 40920.
//---------------------------------------------------------------------
typedef struct
{
	int     level;          //관리자 레벨
	char    mng_id[20];     //관리자 ID
} BT_40920;
#define SZ_BT_40920 sizeof(BT_40920)

//---------------------------------------------------------------------
// 요청수신 조회 응답. OUT 40921.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    // 0 : 정상 1 ~: 오류
	char    msg[80];        // 오류시 메시지
} BT_40921;
#define SZ_BT_40921 sizeof(BT_40921)

typedef struct
{
	int     site;			//site
	char    site_fullname[40];
} BT_40921_REPEAT;
#define SZ_BT_40921_REPEAT sizeof(BT_40921_REPEAT)

//---------------------------------------------------------------------
// 일별정산조회 IN 41000.
//---------------------------------------------------------------------
typedef struct
{
    int     level;      //관리자 레벨
    char    mng_id[20]; //관리자 ID
    char    from[8];    //시작일
    char    to[8];      //종료일
} BT_41000;
#define SZ_BT_41000 sizeof(BT_41000)

//---------------------------------------------------------------------
// 일별정산조회 응답. OUT 41001.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    //0 : 정상 1 ~: 오류
	char    msg[80];        //오류시 메시지

    double  suik_sum;       //총수익
    double  fee;            //수수료
    double  daily_fee;      //이용료
    double  sell_coupon;    //쿠폰판매

    int     in_money;       //총입금액
    int     in_bank;        //일반입금
    int     in_etc;         //기타입금

    int     out_money;      //총출금액
    int     out_bank;       //일반출금
    int     coupon_charge;  //쿠폰구매출금
    int     charge;         //이용료
    int     out_cost;       //출금비용
    int     etc_cost;       //기타비용

    double  sum_fee;        //수수료 합계
    double  fu_fee;         //선물수수료
    double  op_fee;         //옵션수수료
} BT_41001;
#define SZ_BT_41001 sizeof(BT_41001)

typedef struct
{
    char    yyyymmdd[8];    //날짜
    int     mem_cnt;        //회원수
    int     mem_use;        //사용회원
    int     tot_acc;        //총구좌수
    double  dambo;          //담보금
    double  suik;           //수익
    double  in_money;       //입금
    double  out_money;      //출금
    double  fee;            //수수료
    double  charge1;        //이용료
    double  sell_coupon;    //쿠폰판매
    double  calc_sonik;     //평가손익
    double  sonik;          //실현손익
    double  fu_fee;         //선물수수료
    double  op_fee;         //옵션수수료
    double  in_bank;        //일반입금
    double  in_etc;         //기타입금
    double  out_bank;       //일반출금
    double  coupon_charge;  //쿠폰구매출금
    double  charge2;         //이용료
    double  out_cost;       //출금비용
    double  etc_cost;       //기타비용
} BT_41001_REPEAT;
#define SZ_BT_41001_REPEAT sizeof(BT_41001_REPEAT)

//---------------------------------------------------------------------
// 계좌손익모니터링 조회 IN 41100.
//---------------------------------------------------------------------
typedef struct
{
    int     level;      //관리자 레벨
    char    mng_id[20]; //관리자 ID
} BT_41100;
#define SZ_BT_41100 sizeof(BT_41100)

//---------------------------------------------------------------------
// 계좌손익모니터링 응답. OUT 41101.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    //0 : 정상 1 ~: 오류
	char    msg[80];        //오류시 메시지

    double  loan;           //대여금 합계
    double  sonik;          //실현손익 합계
    double  calc_sonik;     //평가손익 합계
    double  fee;            //수수료 합계
    double	sun_sonik;		//순손익 합계
    double  dambo;          //평가담보금 합계
    int     in_money;       //입금 합계
    int     out_money;      //출금 합계
} BT_41101;
#define SZ_BT_41101 sizeof(BT_41101)

typedef struct
{
	char    name[40];       //회원명
    char    id[20];			//회원아이디
	char    acc_no[12];		//계좌번호
    double  loan;           //대여금
    double  sonik;          //실현손익
    double  calc_sonik;     //평가손익
    double  fee;            //수수료 - dFuCmi
    double	sun_sonik;		//순손익
    double  dambo;          //평가담보금
    int     in_money;       //입금
    int     out_money;      //출금
    char    status[20];     //상태
	char	use_today;		//당일사용신청여부 1:신청 0:미신청
	char	filler[3];		
} BT_41101_REPEAT;
#define SZ_BT_41101_REPEAT sizeof(BT_41101_REPEAT)

//---------------------------------------------------------------------
// 계좌번호 비밀번호 확인 IN 49000.
//---------------------------------------------------------------------
typedef struct
{
    int     level;          //관리자 레벨
    char    mng_id[20];     //관리자 ID
    char    acc_no[12];     //계좌번호
    char 	acc_pass[12];   //'계좌비밀번호',
} BT_49000;
#define SZ_BT_49000 sizeof(BT_49000)

//---------------------------------------------------------------------
// 계좌번호 비밀번호 확인 응답. OUT 49001.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		    //0 : 정상 1 ~: 오류
	char    msg[80];        //오류시 메시지
    char 	acc_name[32];  	// '계좌명',
} BT_49001;
#define SZ_BT_49001 sizeof(BT_49001)

//---------------------------------------------------------------------
// 계좌리스트 조회 IN 41200.
//---------------------------------------------------------------------
typedef struct
{
    int     level;      //관리자 레벨
    char    mng_id[20]; //관리자 ID
} BT_41200;
#define SZ_BT_41200 sizeof(BT_41200)

//---------------------------------------------------------------------
// 계좌리스트 조회 응답. OUT 41201.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		        // 0 : 정상 1 ~: 오류
	char    msg[80];            // 오류시 메시지
} BT_41201;
#define SZ_BT_41201 sizeof(BT_41201)

typedef struct
{
    char    acc_no[12];	        //계좌번호    
    char    id[20];			    //회원 아이디
	char    name[40];           //회원명
	double	calc_sonik;         //총손익
    double  calc_dambo;         //평가담보금
    char    status[20];         //상태
} BT_41201_REPEAT;
#define SZ_BT_41201_REPEAT sizeof(BT_41201_REPEAT)

//---------------------------------------------------------------------
// 보유포지션 조회 IN 41210.
//---------------------------------------------------------------------
typedef struct
{
    int     level;      //관리자 레벨
    char    mng_id[20]; //관리자 ID
} BT_41210;
#define SZ_BT_41210 sizeof(BT_41210)

//---------------------------------------------------------------------
// 보유포지션 조회 응답. OUT 41211.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		        // 0 : 정상 1 ~: 오류
	char    msg[80];            // 오류시 메시지
} BT_41211;
#define SZ_BT_41211 sizeof(BT_41211)

typedef struct
{
    char    acc_no[12];	        //계좌번호    
    char    id[20];			    //회원 아이디
    int     acode;              //종목코드
    char    name[40];           //종목명
    char	jumun_gubun[12];	//주문구분
    int     quantity;           //수량    
    double  ave_amount;			//평균단가
    double	cur_price;			//현재가
    double	calc_sonik;         //평가손익
	char    status[20];         //상태
} BT_41211_REPEAT;
#define SZ_BT_41211_REPEAT sizeof(BT_41211_REPEAT)

//---------------------------------------------------------------------
// 미체결포지션 조회 IN 41220.
//---------------------------------------------------------------------
typedef struct
{
    int     level;      //관리자 레벨
    char    mng_id[20]; //관리자 ID
} BT_41220;
#define SZ_BT_41220 sizeof(BT_41220)

//---------------------------------------------------------------------
// 미체결포지션 조회 응답. OUT 41221.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		        // 0 : 정상 1 ~: 오류
	char    msg[80];            // 오류시 메시지
} BT_41221;
#define SZ_BT_41221 sizeof(BT_41221)

typedef struct
{
    char    acc_no[12];	        //계좌번호    
    char    id[20];			    //회원 아이디
    int     order_num;          //주문번호
    int     acode;              //종목코드
    char    name[40];           //종목명
    char	jumun_gubun[12];	//주문구분
    int     ord_qty;            //주문수량    
    int     mi_qty;             //미체결량  
	double 	ord_price;          //주문가격    
	char    status[20];         //상태
} BT_41221_REPEAT;
#define SZ_BT_41221_REPEAT sizeof(BT_41221_REPEAT)

//---------------------------------------------------------------------
// 계좌별 보유포지션 조회 IN 41230.
//---------------------------------------------------------------------
typedef struct
{
    int     level;      //관리자 레벨
    char    mng_id[20]; //관리자 ID
    char    acc_no[12];	//계좌번호
} BT_41230;
#define SZ_BT_41230 sizeof(BT_41230)

//---------------------------------------------------------------------
// 계좌별 보유포지션 조회 응답. OUT 41231.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		        // 0 : 정상 1 ~: 오류
	char    msg[80];            // 오류시 메시지
} BT_41231;
#define SZ_BT_41231 sizeof(BT_41231)

typedef struct
{
    int     acode;              //종목코드
    char    name[40];           //종목명
    char	jumun_gubun[12];	//주문구분
    int     quantity;           //수량    
} BT_41231_REPEAT;
#define SZ_BT_41231_REPEAT sizeof(BT_41231_REPEAT)

//---------------------------------------------------------------------
// 계좌별 미체결포지션 조회 IN 41240.
//---------------------------------------------------------------------
typedef struct
{
    int     level;      //관리자 레벨
    char    mng_id[20]; //관리자 ID
    char    acc_no[12];	//계좌번호
} BT_41240;
#define SZ_BT_41240 sizeof(BT_41240)

//---------------------------------------------------------------------
// 계좌별 미체결포지션 조회 응답. OUT 41241.
//---------------------------------------------------------------------
typedef struct
{
	int     err_no;		        // 0 : 정상 1 ~: 오류
	char    msg[80];            // 오류시 메시지
} BT_41241;
#define SZ_BT_41241 sizeof(BT_41241)

typedef struct
{
    int     acode;              //종목코드
    char    name[40];           //종목명
    char	jumun_gubun[12];	//주문구분
    int     m_qty;            //주문수량    
    int     order_num;          //주문번호
} BT_41241_REPEAT;
#define SZ_BT_41241_REPEAT sizeof(BT_41241_REPEAT)

//---------------------------------------------------------------------
//Keep Alive 10090
//---------------------------------------------------------------------
typedef struct
{
	int   dummy;
} BT_10090;
#define SZ_BT_10090 sizeof(BT_10090)
//---------------------------------------------------------------------
//Keep Alive .. OUT 10091
//---------------------------------------------------------------------
typedef struct
{
	int   dummy;
} BT_10091;
#define SZ_BT_10091 sizeof(BT_10091)

//---------------------------------------------------------------------
// 서버 설정 정보 요청 (AccClass 설정용)     10090
//---------------------------------------------------------------------
typedef struct
{
	int   dummy;
} BT_10100;
#define SZ_BT_10100 sizeof(BT_10100)
//---------------------------------------------------------------------
// 서버 설정 정보 수신 (AccClass 설정용)     10091
//---------------------------------------------------------------------
typedef struct
{
	int losscut_type;

	int f_losscut_per_qty;
	int f_losscut_min;
	int op_losscut_min;
	int fo_losscut_min;

	int op_medo_deposit;

	int op_mesu_qty_limit;
	int op_medo_qty_limit;
	int f_qty_limit;

	int acc_qty_limit;

	int filler[5];

} BT_10101;
#define SZ_BT_10101 sizeof(BT_10101)

//---------------------------------------------------------------------
// 웹 멤버 추가. IN 44100.
//---------------------------------------------------------------------
typedef struct
{
	char   id[20];          // User ID - 수정시에는 키로 사용.
	char   pass[12];        // Password
	char   name[40];	    // 이름
	char   acc_pass[12];    // 계좌 Password
	char   tel[20];	        // 082-043-1234-1234
    char   email[80];		// 이메일주소
	char   zip[8];			// 우편번호
	char   address1[120];	// 주소 Main                            
	char   address2[60];	    // 주소 Sub

	char   bank[40];		// 은행명
    char   owner[40];       // 예금주
	char   mem_acc_no[20];	// 출금 계좌번호

	char   jumin1[8];		//주민번호1
	char   jumin2[8];		//주민번호2
} BT_44100;
#define SZ_BT_44100 sizeof(BT_44100)

//---------------------------------------------------------------------
// 웹 멤버 추가 요청 응답. OUT 44101.
//---------------------------------------------------------------------
typedef struct
{
	char  err_no[4];           //0:정상 1~:오류
	char  msg[80];	        //오류시 메시지
} BT_44101;
#define SZ_BT_44101 sizeof(BT_44101)

//---------------------------------------------------------------------
// 웹 멤버 추가. IN 44110.-----------------sub 사이트가 있을경우
//---------------------------------------------------------------------
typedef struct
{
	char   id[20];          // User ID - 수정시에는 키로 사용.
	char   pass[12];        // Password
	char   name[40];	    // 이름
	char   acc_pass[12];    // 계좌 Password
	char   tel[20];	        // 082-043-1234-1234
	char   email[80];		// 이메일주소
	char   zip[8];			// 우편번호
	char   address1[120];	// 주소 Main
	char   address2[60];	    // 주소 Sub

	char   bank[40];		// 은행명
	char   owner[40];       // 예금주
	char   mem_acc_no[20];	// 출금 계좌번호

	char   jumin1[8];		//주민번호1
	char   jumin2[8];		//주민번호2

	char   site[4];			//사이트값
} BT_44110;
#define SZ_BT_44110 sizeof(BT_44110)

//---------------------------------------------------------------------
// 웹 멤버 추가 요청 응답. OUT 44111.-----------------sub 사이트가 있을경우
//---------------------------------------------------------------------
typedef struct
{
	char  err_no[4];           //0:정상 1~:오류
	char  msg[80];	        //오류시 메시지
} BT_44111;
#define SZ_BT_44111 sizeof(BT_44111)

//---------------------------------------------------------------------
// ID 중복확인. IN 44200.
//---------------------------------------------------------------------
typedef struct
{
	char  id[20];           //User ID
} BT_44200;
#define SZ_BT_44200 sizeof(BT_44200)

//---------------------------------------------------------------------
// ID 중복확인 응답. OUT 44201.
//---------------------------------------------------------------------
typedef struct
{
	char  err_no[4];           //0:정상 1~:오류
	char  msg[80];	        //오류시 메시지
} BT_44201;
#define SZ_BT_44201 sizeof(BT_44201)

//---------------------------------------------------------------------
// ID 중복확인. IN 44210.-----------------sub 사이트가 있을경우
//---------------------------------------------------------------------
typedef struct
{
	char  id[20];           //User ID
	char   site[4];			//사이트값
} BT_44210;
#define SZ_BT_44210 sizeof(BT_44210)

//---------------------------------------------------------------------
// ID 중복확인 응답. OUT 44211.-----------------sub 사이트가 있을경우
//---------------------------------------------------------------------
typedef struct
{
	char  err_no[4];           //0:정상 1~:오류
	char  msg[80];	        //오류시 메시지
} BT_44211;
#define SZ_BT_44211 sizeof(BT_44211)

//---------------------------------------------------------------------
// ID 삭제확인. IN 44300.
//---------------------------------------------------------------------
typedef struct
{
	char  id[20];           //User ID
	char  check;         //잔고체크 여부 '0' 체크 안하고 '1' 체크
	char  filler[3];         //filler
} BT_44300;
#define SZ_BT_44300 sizeof(BT_44300)

//---------------------------------------------------------------------
// ID 삭제확인 응답. OUT 44301.
//---------------------------------------------------------------------
typedef struct
{
	char  err_no[4];           //0:정상 1~:오류
	char  msg[80];          //오류시 메시지
} BT_44301;
#define SZ_BT_44301 sizeof(BT_44301)

//---------------------------------------------------------------------
// ID 삭제확인. IN 44310.-----------------sub 사이트가 있을경우
//---------------------------------------------------------------------
typedef struct
{
	char  id[20];           //User ID
	char  check[4];         //잔고체크 여부 '0' 체크 안하고 '1' 체크
	char  site[4];			//사이트값
} BT_44310;
#define SZ_BT_44310 sizeof(BT_44310)

//---------------------------------------------------------------------
// ID 삭제확인 응답. OUT 44311.-----------------sub 사이트가 있을경우
//---------------------------------------------------------------------
typedef struct
{
	char  err_no[4];           //0:정상 1~:오류
	char  msg[80];          //오류시 메시지
} BT_44311;
#define SZ_BT_44311 sizeof(BT_44311)

//---------------------------------------------------------------------
// 웹 회원정보 전송. IN 44400.
//---------------------------------------------------------------------
typedef struct
{
	char  id[20];           //User ID
	char   site[4];			//사이트값
} BT_44400;
#define SZ_BT_44400 sizeof(BT_44400)

//---------------------------------------------------------------------
// 웹 회원정보 요청 응답. OUT 44401.
//---------------------------------------------------------------------
typedef struct
{
	char  err_no[4];           //0:정상 1~:오류
	char  msg[80];          //오류시 메시지
	
	char   id[20];          // User ID - 수정시에는 키로 사용.

	char  fu_fee[8];         //선물수수료
	char  op_fee[8];         //옵션수수료

	char    oper_id[20];     //상담담당자 id
	char    oper_name[40];   //상담담당자 이름
	char    mo_acc[12];      //모계좌id
} BT_44401;
#define SZ_BT_44401 sizeof(BT_44401)

//---------------------------------------------------------------------
//#pragma pack( pop, 1 )
#endif




