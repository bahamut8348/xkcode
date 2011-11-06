object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 288
  ClientWidth = 398
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  DesignSize = (
    398
    288)
  PixelsPerInch = 96
  TextHeight = 13
  object ListView1: TListView
    Left = 8
    Top = 8
    Width = 382
    Height = 241
    Anchors = [akLeft, akTop, akRight, akBottom]
    Columns = <
      item
        AutoSize = True
        Caption = 'PrcessName'
      end
      item
        AutoSize = True
        Caption = 'Path'
      end
      item
        Caption = 'PID'
      end>
    GridLines = True
    HideSelection = False
    IconOptions.AutoArrange = True
    Items.ItemData = {
      01740000000400000000000000FFFFFFFFFFFFFFFF0000000000000000000000
      0000FFFFFFFFFFFFFFFF0000000000000000046C0069007300740000000000FF
      FFFFFFFFFFFFFF0000000000000000056C00690073007400320000000000FFFF
      FFFFFFFFFFFF0000000000000000056C006900730074003300}
    MultiSelect = True
    RowSelect = True
    SortType = stText
    TabOrder = 0
    ViewStyle = vsReport
    OnColumnClick = ListView1ColumnClick
  end
  object Button1: TButton
    Left = 315
    Top = 255
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = '&Close'
    TabOrder = 1
  end
  object ImageList1: TImageList
    Left = 216
    Top = 56
  end
end
