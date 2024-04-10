object Form1: TForm1
  Left = 68
  Top = 163
  Caption = 'Form1'
  ClientHeight = 149
  ClientWidth = 657
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 19
  object Label1: TLabel
    Left = 16
    Top = 8
    Width = 122
    Height = 19
    Caption = 'String to encode:'
  end
  object Label2: TLabel
    Left = 16
    Top = 40
    Width = 49
    Height = 19
    Caption = 'Result:'
  end
  object Label3: TLabel
    Left = 16
    Top = 72
    Width = 95
    Height = 19
    Caption = 'Back Decode:'
  end
  object Label4: TLabel
    Left = 152
    Top = 120
    Width = 210
    Height = 19
    Caption = 'AbhorDJ STRING ENCODING'
  end
  object Edit1: TEdit
    Left = 152
    Top = 8
    Width = 489
    Height = 28
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    Text = 'Edit1'
    OnChange = Edit1Change
  end
  object Edit2: TEdit
    Left = 152
    Top = 40
    Width = 489
    Height = 28
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    Text = 'Edit2'
    OnChange = Edit2Change
  end
  object Edit3: TEdit
    Left = 152
    Top = 72
    Width = 489
    Height = 28
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
    Text = 'Edit2'
  end
end
