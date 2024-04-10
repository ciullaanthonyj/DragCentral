object Form1: TForm1
  Left = 241
  Top = 110
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
  object script: TEdit
    Left = 320
    Top = 24
    Width = 129
    Height = 21
    TabOrder = 0
    Text = 'admin.php'
  end
  object Button1: TButton
    Left = 600
    Top = 24
    Width = 75
    Height = 25
    Caption = 'SEND'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Memo1: TMemo
    Left = 48
    Top = 128
    Width = 289
    Height = 313
    Lines.Strings = (
      'Results')
    TabOrder = 2
  end
  object params: TEdit
    Left = 464
    Top = 24
    Width = 129
    Height = 21
    TabOrder = 3
  end
  object dir: TEdit
    Left = 176
    Top = 24
    Width = 129
    Height = 21
    TabOrder = 4
    Text = '/cars/'
  end
  object host: TEdit
    Left = 40
    Top = 24
    Width = 129
    Height = 21
    TabOrder = 5
    Text = 'nazarkuliev.biz'
  end
  object Memo2: TMemo
    Left = 352
    Top = 128
    Width = 329
    Height = 313
    Lines.Strings = (
      'Return')
    TabOrder = 6
  end
  object script2: TEdit
    Left = 320
    Top = 64
    Width = 129
    Height = 21
    TabOrder = 7
    Text = 'admin.php'
  end
  object Button2: TButton
    Left = 600
    Top = 64
    Width = 75
    Height = 25
    Caption = 'SEND'
    TabOrder = 8
    OnClick = Button2Click
  end
  object params2: TEdit
    Left = 464
    Top = 64
    Width = 129
    Height = 21
    TabOrder = 9
  end
  object dir2: TEdit
    Left = 176
    Top = 64
    Width = 129
    Height = 21
    TabOrder = 10
    Text = '/cars/'
  end
  object host2: TEdit
    Left = 40
    Top = 64
    Width = 129
    Height = 21
    TabOrder = 11
    Text = 'nazarkuliev.biz'
  end
end
