program cGrButton_Test;

uses
  Forms,
  cGrButton_Test_Form in 'cGrButton_Test_Form.pas' {Form1};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
