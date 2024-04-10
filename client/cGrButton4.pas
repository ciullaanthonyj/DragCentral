unit cGrButton4;

interface

uses
  SysUtils, Classes, Controls;

type
  TGrButton4 = class(TGraphicControl)
  private
    { Private declarations }
  protected
    { Protected declarations }
  public
    { Public declarations }
  published
    { Published declarations }
  end;

procedure Register;

implementation

procedure Register;
begin
  RegisterComponents('noizz', [TGrButton4]);
end;

end.
