object Form1: TForm1
  Left = 130
  Top = 165
  Width = 544
  Height = 375
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
    Left = 80
    Top = 32
    Width = 106
    Height = 13
    Caption = 'Random reaction time:'
  end
  object Label2: TLabel
    Left = 80
    Top = 64
    Width = 58
    Height = 13
    Caption = 'Max throttle:'
  end
  object Label3: TLabel
    Left = 72
    Top = 184
    Width = 58
    Height = 13
    Caption = 'Car position:'
  end
  object rt: TEdit
    Left = 192
    Top = 32
    Width = 121
    Height = 21
    TabOrder = 0
    Text = '0.5'
    OnKeyPress = mtKeyPress
  end
  object mt: TEdit
    Left = 192
    Top = 64
    Width = 121
    Height = 21
    TabOrder = 1
    Text = '0.7'
    OnKeyPress = mtKeyPress
  end
  object go: TButton
    Left = 184
    Top = 112
    Width = 75
    Height = 25
    Caption = 'go'
    TabOrder = 2
    OnClick = goClick
  end
  object carpos: TEdit
    Left = 192
    Top = 184
    Width = 121
    Height = 21
    TabOrder = 3
    Text = 'carpos'
  end
  object Memo1: TMemo
    Left = 48
    Top = 216
    Width = 465
    Height = 89
    Lines.Strings = (
      'Memo1')
    TabOrder = 4
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 1
    OnTimer = Timer1Timer
    Left = 424
    Top = 96
  end
end
