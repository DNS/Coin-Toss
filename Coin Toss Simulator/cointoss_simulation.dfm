object tossCoinForm: TtossCoinForm
  Left = 195
  Top = 0
  Caption = 'Coin Tossing Simulator'
  ClientHeight = 642
  ClientWidth = 1064
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 775
    Top = 8
    Width = 31
    Height = 13
    Caption = 'Label1'
    OnClick = Label1Click
  end
  object Chart1: TChart
    Left = 8
    Top = 8
    Width = 761
    Height = 273
    Title.Text.Strings = (
      'Result')
    View3D = False
    TabOrder = 0
    ColorPaletteIndex = 13
    object TossChartFastSeries: TFastLineSeries
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.ShapeStyle = fosRoundRectangle
      Marks.Visible = False
      SeriesColor = clBlack
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
      Data = {0000000000}
    end
    object maFastSeries: TFastLineSeries
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.ShapeStyle = fosRoundRectangle
      Marks.Visible = False
      SeriesColor = clRed
      LinePen.Color = clRed
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
      Data = {0000000000}
    end
    object IdealFastSeries: TFastLineSeries
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.ShapeStyle = fosRoundRectangle
      Marks.Visible = False
      SeriesColor = clBlue
      Title = 'IdealFastSeries'
      LinePen.Color = clBlue
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
      Data = {0000000000}
    end
  end
  object tossCoinButton: TButton
    Left = 23
    Top = 560
    Width = 121
    Height = 25
    Caption = 'Toss Coin Now !'
    TabOrder = 1
    OnClick = tossCoinButtonClick
  end
  object ComboBox1: TComboBox
    Left = 15
    Top = 520
    Width = 145
    Height = 21
    Style = csDropDownList
    Ctl3D = True
    DoubleBuffered = True
    ItemIndex = 0
    ParentCtl3D = False
    ParentDoubleBuffered = False
    TabOrder = 2
    Text = '20'
    Items.Strings = (
      '20'
      '100'
      '200'
      '500'
      '1000'
      '10000'
      '100000'
      '1000000')
  end
  object maButton: TButton
    Left = 224
    Top = 560
    Width = 121
    Height = 25
    Caption = 'Calculate MA && '#963
    TabOrder = 3
    OnClick = maButtonClick
  end
  object ComboBox2: TComboBox
    Left = 224
    Top = 520
    Width = 145
    Height = 21
    ItemIndex = 0
    TabOrder = 4
    Text = '2'
    Items.Strings = (
      '2'
      '20'
      '200'
      '2000'
      '20000'
      '200000')
  end
  object ProfitLossButton: TButton
    Left = 416
    Top = 560
    Width = 129
    Height = 25
    Caption = 'Calculate PL'
    TabOrder = 5
    OnClick = ProfitLossButtonClick
  end
  object Chart2: TChart
    Left = 8
    Top = 287
    Width = 761
    Height = 210
    Legend.LegendStyle = lsSeries
    Title.Text.Strings = (
      #963)
    View3D = False
    TabOrder = 6
    ColorPaletteIndex = 13
    object stddevFastSeries: TFastLineSeries
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.ShapeStyle = fosRoundRectangle
      Marks.Visible = False
      SeriesColor = clGreen
      LinePen.Color = clGreen
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
      Data = {0000000000}
    end
    object stddevMAFastSeries: TFastLineSeries
      Active = False
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.ShapeStyle = fosRoundRectangle
      Marks.Visible = False
      SeriesColor = clRed
      Title = 'stddevMAFastSeries'
      LinePen.Color = clRed
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
      Data = {0000000000}
    end
  end
end
