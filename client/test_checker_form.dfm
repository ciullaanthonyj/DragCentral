object Form1: TForm1
  Left = 81
  Top = 100
  Width = 544
  Height = 190
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
    Left = 48
    Top = 128
    Width = 276
    Height = 13
    Caption = 'Parameters: 1 - filename 2 - inifilename 3 - section 4 - value'
  end
  object Open: TButton
    Left = 80
    Top = 24
    Width = 75
    Height = 25
    Caption = 'Open'
    TabOrder = 0
    OnClick = OpenClick
  end
  object Edit1: TEdit
    Left = 88
    Top = 72
    Width = 121
    Height = 21
    TabOrder = 1
    Text = 'Edit1'
  end
  object od: TOpenDialog
    Left = 32
    Top = 24
  end
  object Timer1: TTimer
    Interval = 1
    OnTimer = Timer1Timer
    Left = 464
    Top = 16
  end
end
