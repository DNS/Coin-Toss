program CoinToss;

uses
  Vcl.Forms,
  cointoss_simulation in 'cointoss_simulation.pas' {tossCoinForm},
  Vcl.Themes,
  Vcl.Styles;

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TtossCoinForm, tossCoinForm);
  Application.Run;
end.
