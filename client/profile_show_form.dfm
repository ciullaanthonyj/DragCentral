object Form1: TForm1
  Left = 671
  Top = 210
  Width = 345
  Height = 454
  AlphaBlend = True
  AlphaBlendValue = 245
  Caption = 'Profile Viewer'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object memo: TMemo
    Left = 0
    Top = 41
    Width = 337
    Height = 379
    Align = alClient
    Lines.Strings = (
      'memo')
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 337
    Height = 41
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 1
    object filename: TEdit
      Left = 120
      Top = 8
      Width = 185
      Height = 21
      TabOrder = 0
      Text = 'filename'
    end
    object open: TButton
      Left = 8
      Top = 8
      Width = 49
      Height = 25
      Caption = 'open'
      TabOrder = 1
      OnClick = openClick
    end
    object Button1: TButton
      Left = 56
      Top = 8
      Width = 49
      Height = 25
      Caption = 'save'
      TabOrder = 2
      OnClick = Button1Click
    end
  end
  object od: TOpenDialog
    Filter = 'TXT|*.txt'
    Left = 8
    Top = 64
  end
  object tm: TTimer
    Enabled = False
    Interval = 250
    OnTimer = tmTimer
    Left = 48
    Top = 64
  end
  object autoexec: TTimer
    Interval = 1
    OnTimer = autoexecTimer
    Left = 104
    Top = 64
  end
end
