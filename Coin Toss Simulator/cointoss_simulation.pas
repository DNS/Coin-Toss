
{
Project         : Coin Toss Simulator
Author          : Daniel Sirait <dns@cpan.org>
Copyright       : Copyright (c) 2013 Daniel Sirait
License         : Must be distributed only under the terms of "THE BEER-WARE LICENSE"  (Revision 42):
                 As long as you retain this notice you
                 can do whatever you want with this stuff. If we meet some day, and you think
                 this stuff is worth it, you can buy me a beer in return
Disclaimer      : I CAN UNDER NO CIRCUMSTANCES BE HELD RESPONSIBLE FOR
                  ANY CONSEQUENCES OF YOUR USE/MISUSE OF THIS DOCUMENT,
                  WHATEVER THAT MAY BE (GET BUSTED, WORLD WAR, ETC..).
}

{APPTYPE CONSOLE}
unit cointoss_simulation;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, Math, VCLTee.TeEngine,
  VCLTee.Series, Vcl.ExtCtrls, VCLTee.TeeProcs, VCLTee.Chart, Vcl.Grids;

type
  TtossCoinForm = class(TForm)
    Label1: TLabel;
    Chart1: TChart;
    tossCoinButton: TButton;
    ComboBox1: TComboBox;
    maButton: TButton;
    ComboBox2: TComboBox;
    ProfitLossButton: TButton;
    TossChartFastSeries: TFastLineSeries;
    maFastSeries: TFastLineSeries;
    Chart2: TChart;
    stddevFastSeries: TFastLineSeries;
    IdealFastSeries: TFastLineSeries;
    procedure Label1Click(Sender: TObject);
    procedure tossCoinButtonClick(Sender: TObject);
    procedure maButtonClick(Sender: TObject);
    procedure ProfitLossButtonClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  tossCoinForm: TtossCoinForm;
  init_price: integer;

implementation

{$R *.dfm}


function tossCoin() : integer;
begin
    {tossCoin := RandomRange(0, 2);}
    tossCoin := random(2);
end;

procedure TtossCoinForm.maButtonClick(Sender: TObject);
var i, j, k, period :integer;
var tmp, mean, variance, stddev :double;
begin
    maFastSeries.Clear;
    stddevFastSeries.Clear;

    period := StrtoInt(ComboBox2.Text);
    //label1.Caption := InttoStr(TossChartFastSeries.XValues.Count);

    // Calc MA & StdDev
    for i := 0 to TossChartFastSeries.XValues.Count-1 do
    begin

        if (i >= period) and (i < StrtoInt(ComboBox1.Text)) then
        begin
	        tmp := 0;
        	for j := (1+i-period) to i do
        		tmp := tmp + TossChartFastSeries.YValue[j];

        	mean := tmp / period;
        	maFastSeries.AddXY(i, mean);

        end;
    end;

	// Calc StdDev
    for i := 0 to TossChartFastSeries.XValues.Count-1 do
    begin
        if (i >= period) and  (i < StrtoInt(ComboBox1.Text)) then
        begin
	        tmp := 0;
        	for j := (1+i-period) to i do
        		tmp := tmp + TossChartFastSeries.YValue[j];

        	mean := tmp / period;
            tmp := 0;

            for j := (1+i-period) to i do
                tmp := Power(TossChartFastSeries.YValue[j] - mean, 2);

            variance := tmp / period;
            stddev := Sqrt(variance);
           	stddevFastSeries.AddXY(i, stddev);
        end;
    end;



end;

procedure TtossCoinForm.ProfitLossButtonClick(Sender: TObject);
var i, j, period :integer;
var mean, tmp, sd, sd_max_prev : real;
begin
    period := StrtoInt(ComboBox2.Text);
    label1.Caption := '';

    // Calc StdDev Mean (period)
    for i := 0 to stddevFastSeries.XValues.Count-1 do
    begin
        tmp := 0;
        label1.Caption := label1.Caption + FloattoStr(stddevFastSeries.XValue[i]) + ': ' + FloattoStr(stddevFastSeries.YValue[i]) + sLineBreak;
		//stddevFastSeries

        {if
        	(TossChartFastSeries.YValue[i] > maFastSeries.YValue[i]) and
	    	(stddevFastSeries.YValue[i] > 0.2) and
            (stddevFastSeries.YValue[i-1] < 0.2) and (i > period)
        then
        	label1.Caption := label1.Caption + InttoStr(i) + ': ' + FloattoStr(stddevFastSeries.YValue[i]) + ', ' + FloattoStr(maFastSeries.YValue[i]) + sLineBreak;
         }

    end;




end;

procedure TtossCoinForm.tossCoinButtonClick(Sender: TObject);
var i, t, result :integer;
//var x, y: array of double;
begin
    TossChartFastSeries.Clear;
    IdealFastSeries.Clear;
    maFastSeries.Clear;
    stddevFastSeries.Clear;

    result := init_price;

    //SetLength(x, 5000000);
	//SetLength(y, 5000000);

    for i := 1 to Strtoint(ComboBox1.Text) do
    begin
        t := tossCoin();
        if t = 0 then
            result := result - 1
        else if t = 1 then
            result := result + 1;

        TossChartFastSeries.Add(result);
        IdealFastSeries.Add(init_price);
    end;


end;

procedure TtossCoinForm.FormCreate(Sender: TObject);
begin
	init_price := 0;
end;

procedure TtossCoinForm.Label1Click(Sender: TObject);
begin
    //label1.Caption := 'asdas';
end;

end.


