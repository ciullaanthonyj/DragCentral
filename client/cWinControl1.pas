unit cWinControl1;

interface

uses
  SysUtils, Classes, Controls, Messages;

type
  TWinControl1 = class(TWinControl)
  private
    procedure WMPaint(var Message: TWMPaint); message WM_PAINT;
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
  RegisterComponents('noizz', [TWinControl1]);
end;

{ TWinControl1 }

procedure TWinControl1.WMPaint(var Message: TWMPaint);
begin
  ;
end;

end.
