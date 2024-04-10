object Form1: TForm1
  Left = 257
  Top = 129
  Width = 696
  Height = 480
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 104
    Top = 16
    Width = 212
    Height = 13
    Caption = 'This is listener of 10000 port on this machine.'
  end
  object Memo1: TMemo
    Left = 184
    Top = 56
    Width = 185
    Height = 161
    Lines.Strings = (
      'Memo1')
    TabOrder = 0
  end
  object Button1: TButton
    Left = 320
    Top = 320
    Width = 75
    Height = 25
    Caption = 'send'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Memo2: TMemo
    Left = 112
    Top = 264
    Width = 185
    Height = 89
    Lines.Strings = (
      'Memo2')
    TabOrder = 2
  end
  object Button2: TButton
    Left = 488
    Top = 232
    Width = 75
    Height = 25
    Caption = 'close'
    TabOrder = 3
    OnClick = Button2Click
  end
  object ServerSocket1: TServerSocket
    Active = True
    Port = 10000
    ServerType = stNonBlocking
    OnAccept = ServerSocket1Accept
    OnClientRead = ServerSocket1ClientRead
    Left = 96
    Top = 88
  end
end
