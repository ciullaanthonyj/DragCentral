unit cGrButton3;

interface

uses
  SysUtils, Classes, Controls, ExtCtrls, StdCtrls;

type
  TGrButton3 = class(TImage)
  private
    Caption:TLabel;
    Button:TButton;
    { Private declarations }
  protected
    { Protected declarations }
  public
    constructor Create(AOwner: TComponent); override;
    procedure SetBounds(ALeft, ATop, AWidth, AHeight: Integer);override;
    procedure ButtonEnter(Sender: TObject);
    procedure ButtonExit(Sender: TObject);
    procedure ButtonClick(Sender: TObject);
    destructor Destroy; override;
  published
    { Published declarations }
  end;

procedure Register;

implementation

procedure Register;
begin
  RegisterComponents('noizz', [TGrButton3]);
end;

{ TGrButton3 }

procedure TGrButton3.ButtonClick(Sender: TObject);
begin
  Caption.Caption := 'Click';
end;

procedure TGrButton3.ButtonEnter(Sender: TObject);
begin
  Caption.Caption := 'Enter';
end;

procedure TGrButton3.ButtonExit(Sender: TObject);
begin
  Caption.Caption := 'Exit';
end;

constructor TGrButton3.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  Button := TButton.Create(self);
  Button.Parent := TWinControl(Owner);
  Button.Width := 0;
  Button.Height := 0;
  Button.OnEnter := ButtonEnter;
  Button.OnExit := ButtonExit;
  Button.OnClick := ButtonClick;
  Caption := TLabel.Create(self);
  Caption.Parent := TWinControl(AOwner);
  Caption.Caption := 'shit';
  Caption.Transparent := true;
  Caption.AutoSize := true;
  Caption.Alignment := taCenter;
end;

destructor TGrButton3.Destroy;
begin
  inherited;
end;

procedure TGrButton3.SetBounds(ALeft, ATop, AWidth, AHeight: Integer);
begin
  inherited SetBounds(ALeft,ATop,AWidth,AHeight);
  if(Caption<>nil) then
  begin
    Caption.Top := ATop + (Caption.Height+AHeight) div 2;
    Caption.Left := ALeft + (Caption.Width+AWidth) div 2;
    Caption.BringToFront;
  end;

end;

end.
