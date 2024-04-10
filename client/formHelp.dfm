object Help: THelp
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Help'
  ClientHeight = 104
  ClientWidth = 349
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesktopCenter
  OnKeyPress = FormKeyPress
  PixelsPerInch = 96
  TextHeight = 13
  object Memo1: TMemo
    Left = 0
    Top = 0
    Width = 349
    Height = 104
    Align = alClient
    Color = 15395562
    Lines.Strings = (
      'Racing controls:'
      ''
      'Arrows <Up>, <Down> - select speed'
      'Adjust gas with mouse'
      'Brakes with mouse'
      'Arrow <Right> - use nitrous')
    ReadOnly = True
    TabOrder = 0
  end
end
