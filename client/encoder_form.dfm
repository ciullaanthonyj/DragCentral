object Form1: TForm1
  Left = 202
  Top = 135
  Width = 998
  Height = 803
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
  object Button1: TButton
    Left = 560
    Top = 56
    Width = 75
    Height = 25
    Caption = 'Button1'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Edit1: TEdit
    Left = 80
    Top = 56
    Width = 465
    Height = 21
    TabOrder = 1
    Text = 'somestring'
  end
  object Edit2: TEdit
    Left = 80
    Top = 88
    Width = 545
    Height = 21
    TabOrder = 2
    Text = 'somestring'
  end
  object Edit3: TEdit
    Left = 80
    Top = 136
    Width = 545
    Height = 21
    TabOrder = 3
    Text = 'somestring'
  end
  object Button2: TButton
    Left = 712
    Top = 144
    Width = 75
    Height = 25
    Caption = 'Button2'
    TabOrder = 4
    OnClick = Button2Click
  end
  object RichEdit1: TRichEdit
    Left = 0
    Top = 200
    Width = 990
    Height = 569
    Align = alBottom
    Lines.Strings = (
      '#include "stdio.h"'
      ''
      '""'
      ''
      'L"test"'
      ''
      ''
      #9' AnsiString res = s->Query("team_challenge.php",'
      #9'   "act=create&bracket="+booltostr(Bracket)+'
      #9'   "&teamid="+teamid+'
      #9'   "&o_teamid="+o_teamid+'
      #9'   "&headsup="+booltostr(HeadsUp)+'
      #9'   "&forfun="+booltostr(ForFun)+'
      #9'   "&formoney="+booltostr(ForMoney)+'
      #9'   "&wager="+IntToStr(Wager)+'
      #9'   "&challengers="+challengers);')
    TabOrder = 5
    WordWrap = False
  end
  object Button3: TButton
    Left = 816
    Top = 144
    Width = 75
    Height = 25
    Caption = 'Button3'
    TabOrder = 6
    OnClick = Button3Click
  end
  object CheckBox1: TCheckBox
    Left = 856
    Top = 104
    Width = 97
    Height = 17
    Caption = 'CheckBox1'
    TabOrder = 7
    OnClick = CheckBox1Click
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 100
    OnTimer = Timer1Timer
    Left = 760
    Top = 56
  end
  object Timer2: TTimer
    Enabled = False
    Interval = 100
    OnTimer = Timer2Timer
    Left = 856
    Top = 48
  end
end
