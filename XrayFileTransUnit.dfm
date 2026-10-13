object XrayFTP: TXrayFTP
  OldCreateOrder = False
  Height = 114
  Width = 215
  object LoginSock: TClientSocket
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = LoginSockConnect
    OnDisconnect = LoginSockDisconnect
    OnRead = LoginSockRead
    OnError = LoginSockError
    Left = 16
    Top = 8
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 3000
    OnTimer = Timer1Timer
    Left = 64
    Top = 8
  end
end
