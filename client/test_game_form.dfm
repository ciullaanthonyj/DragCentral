object Form1: TForm1
  Left = 103
  Top = 58
  Width = 862
  Height = 564
  Caption = 'Race'
  Color = clGray
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  Menu = MainMenu1
  OldCreateOrder = False
  PopupMenu = PopupMenu1
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 854
    Height = 510
    ActivePage = TabSheet1
    Align = alClient
    TabIndex = 0
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'game'
      object Start: TButton
        Left = 736
        Top = 448
        Width = 75
        Height = 25
        Caption = 'Restart'
        TabOrder = 0
        OnClick = StartClick
      end
      object Button2: TButton
        Left = 216
        Top = 456
        Width = 107
        Height = 25
        Caption = 'No staging'
        TabOrder = 1
        OnClick = Button2Click
      end
      object Button3: TButton
        Left = 136
        Top = 456
        Width = 75
        Height = 25
        Caption = 'Finish'
        TabOrder = 2
        OnClick = Button3Click
      end
      object Memo1: TMemo
        Left = 688
        Top = 88
        Width = 153
        Height = 241
        Lines.Strings = (
          ''
          ''
          ''
          ''
          ''
          ''
          ''
          ''
          ''
          ''
          ''
          ''
          ''
          ''
          ''
          ''
          '')
        TabOrder = 3
      end
      object flupview: TShockwaveFlash
        Left = 8
        Top = 9
        Width = 24
        Height = 393
        TabOrder = 4
        ControlData = {
          67556655100700007B0200009E28000008000200000000000800040000002000
          00000800040000002000000008000E000000570069006E0064006F0077000000
          0B00FFFF0B00FFFF08000A000000480069006700680000000800020000000000
          0B00000008000000000008000200000000000800100000004E006F0053006300
          61006C00650000000B0000000B00000008000200000000000800000000000800
          0200000000000D00000000000000000000000000000000000B0001000B000000
          080000000000030000000000}
      end
      object flstaging: TShockwaveFlash
        Left = 16
        Top = 9
        Width = 24
        Height = 393
        TabOrder = 5
        ControlData = {
          67556655100700007B0200009E28000008000200000000000800040000002000
          00000800040000002000000008000E000000570069006E0064006F0077000000
          0B00FFFF0B00FFFF08000A000000480069006700680000000800020000000000
          0B00000008000000000008000200000000000800100000004E006F0053006300
          61006C00650000000B0000000B00000008000200000000000800000000000800
          0200000000000D00000000000000000000000000000000000B0001000B000000
          080000000000030000000000}
      end
      object flroad: TShockwaveFlash
        Left = 40
        Top = 9
        Width = 617
        Height = 211
        TabOrder = 6
        ControlData = {
          6755665510070000C53F0000CF15000008000200000000000800040000002000
          00000800040000002000000008000E000000570069006E0064006F0077000000
          0B00FFFF0B00FFFF08000A000000480069006700680000000800020000000000
          0B00000008000000000008000200000000000800100000004E006F0053006300
          61006C00650000000B0000000B00000008000200000000000800000000000800
          0200000000000D00000000000000000000000000000000000B0001000B000000
          080000000000030000000000}
      end
      object flcarpanel: TShockwaveFlash
        Left = 40
        Top = 220
        Width = 618
        Height = 182
        TabOrder = 7
        ControlData = {
          6755665510070000DF3F0000CF12000008000200000000000800040000002000
          00000800040000002000000008000E000000570069006E0064006F0077000000
          0B00FFFF0B00FFFF08000A000000480069006700680000000800020000000000
          0B00000008000000000008000200000000000800100000004E006F0053006300
          61006C00650000000B0000000B00000008000200000000000800000000000800
          0200000000000D00000000000000000000000000000000000B0001000B000000
          080000000000030000000000}
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'engine'
      ImageIndex = 1
      object pb: TPaintBox
        Left = 56
        Top = 16
        Width = 353
        Height = 185
      end
      object Button1: TButton
        Left = 56
        Top = 216
        Width = 75
        Height = 25
        Caption = 'test'
        TabOrder = 0
        OnClick = Button1Click
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'testwinlosedraw'
      ImageIndex = 2
      object Memo2: TMemo
        Left = 16
        Top = 40
        Width = 809
        Height = 345
        Lines.Strings = (
          'Memo2')
        TabOrder = 0
      end
      object Button4: TButton
        Left = 16
        Top = 8
        Width = 75
        Height = 25
        Caption = 'RUN'
        TabOrder = 1
        OnClick = Button4Click
      end
    end
  end
  object Timer1: TTimer
    Interval = 30
    OnTimer = Timer1Timer
    Left = 576
    Top = 456
  end
  object PopupMenu1: TPopupMenu
    Left = 776
    Top = 456
    object SpeedUp1: TMenuItem
      Caption = 'Speed Up'
      ShortCut = 38
      Visible = False
      OnClick = SpeedUp1Click
    end
    object Down: TMenuItem
      Caption = 'Speed Down'
      ShortCut = 40
      Visible = False
      OnClick = DownClick
    end
  end
  object ApplicationEvents1: TApplicationEvents
    OnMessage = ApplicationEvents1Message
    Left = 672
    Top = 456
  end
  object MainMenu1: TMainMenu
    Left = 384
    Top = 456
    object Test1: TMenuItem
      Caption = 'Tests'
      ShortCut = 120
      object Bracketrace1: TMenuItem
        Caption = 'Bracket Race Game'
        OnClick = Bracketrace1Click
      end
      object Youlose1: TMenuItem
        Caption = 'Computer - Break out (simulation)'
        OnClick = BreakOutCLick
      end
      object Youwinsimulation1: TMenuItem
        Caption = 'Computer - You win (simulation)'
        OnClick = Youwinsimulation1Click
      end
      object Youlosesimulation1: TMenuItem
        Caption = 'Computer - You lose (simulation)'
        OnClick = Youlosesimulation1Click
      end
      object WithracerYouwinsimulation1: TMenuItem
        Caption = 'With racer - First drive (simulation)'
        OnClick = WithracerYouwinsimulation1Click
      end
    end
  end
end
