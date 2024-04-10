object Form1: TForm1
  Left = 89
  Top = 85
  Width = 874
  Height = 518
  Caption = 'Test Chat Server'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Memo1: TMemo
    Left = 8
    Top = 8
    Width = 393
    Height = 417
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Verdana'
    Font.Style = []
    Lines.Strings = (
      'LOG')
    ParentFont = False
    TabOrder = 0
  end
  object Button1: TButton
    Left = 440
    Top = 16
    Width = 89
    Height = 25
    Caption = 'Room People Update'
    TabOrder = 1
    OnClick = eventRoomPeopleUpdate
  end
  object Memo2: TMemo
    Left = 440
    Top = 48
    Width = 225
    Height = 177
    Lines.Strings = (
      'Memo2')
    TabOrder = 2
  end
  object test1: TButton
    Left = 440
    Top = 400
    Width = 75
    Height = 25
    Caption = 'test1'
    TabOrder = 3
    OnClick = test1Click
  end
  object eventChatMessage: TButton
    Left = 440
    Top = 240
    Width = 153
    Height = 25
    Caption = 'Chat Message'
    TabOrder = 4
    OnClick = eventChatMessageClick
  end
  object Memo3: TMemo
    Left = 440
    Top = 272
    Width = 225
    Height = 121
    Lines.Strings = (
      'Memo3')
    TabOrder = 5
  end
  object changeroom: TButton
    Left = 536
    Top = 16
    Width = 89
    Height = 25
    Caption = 'Change Room'
    TabOrder = 6
    OnClick = changeroomClick
  end
  object test2: TButton
    Left = 520
    Top = 400
    Width = 75
    Height = 25
    Caption = 'time test'
    TabOrder = 7
    OnClick = test2Click
  end
  object reg: TButton
    Left = 600
    Top = 400
    Width = 113
    Height = 25
    Caption = 'register unregister'
    TabOrder = 8
    OnClick = regClick
  end
  object ban: TButton
    Left = 720
    Top = 400
    Width = 113
    Height = 25
    Caption = 'ban unban'
    TabOrder = 9
    OnClick = banClick
  end
  object Button2: TButton
    Left = 704
    Top = 24
    Width = 75
    Height = 25
    Caption = 'Button2'
    TabOrder = 10
  end
  object Button3: TButton
    Left = 720
    Top = 272
    Width = 121
    Height = 25
    Caption = 'register'
    TabOrder = 11
    OnClick = Button3Click
  end
  object Edit1: TEdit
    Left = 720
    Top = 192
    Width = 121
    Height = 21
    TabOrder = 12
    Text = 'Edit1'
  end
  object Edit2: TEdit
    Left = 720
    Top = 224
    Width = 121
    Height = 21
    TabOrder = 13
    Text = 'Edit1'
  end
  object Edit3: TEdit
    Left = 720
    Top = 248
    Width = 121
    Height = 21
    TabOrder = 14
    Text = 'mail'
  end
  object Button4: TButton
    Left = 720
    Top = 312
    Width = 121
    Height = 25
    Caption = 'sendpass'
    TabOrder = 15
    OnClick = Button4Click
  end
  object Button5: TButton
    Left = 440
    Top = 440
    Width = 113
    Height = 25
    Caption = 'connect disconnect'
    TabOrder = 16
    OnClick = Button5Click
  end
  object Button6: TButton
    Left = 584
    Top = 440
    Width = 129
    Height = 25
    Caption = 'quit - enter chat'
    TabOrder = 17
    OnClick = Button6Click
  end
  object Button7: TButton
    Left = 736
    Top = 120
    Width = 75
    Height = 25
    Caption = 'thread'
    TabOrder = 18
    OnClick = Button7Click
  end
  object autoexec: TTimer
    Enabled = False
    Interval = 1
    OnTimer = autoexecTimer
    Left = 720
    Top = 40
  end
  object MainMenu1: TMainMenu
    Left = 776
    Top = 440
    object Test3: TMenuItem
      Caption = 'Tests'
      object Logincreateprivateroomlogout1: TMenuItem
        Caption = 'Login - create private room - logout'
        OnClick = Logincreateprivateroomlogout1Click
      end
      object Enterquit1: TMenuItem
        Caption = 'Enter - quit'
        OnClick = Enterquit1Click
      end
    end
  end
  object enterquit: TTimer
    Enabled = False
    Interval = 5500
    OnTimer = enterquitTimer
    Left = 816
    Top = 40
  end
end
