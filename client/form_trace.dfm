object frm_trace: Tfrm_trace
  Left = 396
  Top = 197
  AlphaBlendValue = 100
  Caption = 'Trace Log (Debug Info)'
  ClientHeight = 114
  ClientWidth = 918
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PauseButton: TSpeedButton
    Left = 0
    Top = 0
    Width = 214
    Height = 22
    AllowAllUp = True
    GroupIndex = 1
    Caption = 'Pause'
    Flat = True
  end
  object Memo1: TMemo
    Left = 0
    Top = 0
    Width = 918
    Height = 114
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 0
    ExplicitWidth = 476
    ExplicitHeight = 499
  end
end
