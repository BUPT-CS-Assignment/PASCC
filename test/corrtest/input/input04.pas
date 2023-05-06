program TestProcAndFunc;

var
  a, b: integer;

procedure testPro1;
begin
  writeln(2023);
end;

procedure testPro2(n: integer);
begin
  writeln(n + 1);
end;

function testFunc: integer;
begin
  writeln(2023);
end;

function gcd(a, b: integer): integer;
begin
  if b = 0 then gcd := a
  else gcd := gcd(b, a mod b);
end;

function swap(var x, y: integer): integer;
var
  temp: integer;
begin
  temp := x;
  x := y;
  y := temp;
end;

begin
  writeln("procedure-test");
  testPro1;
  testPro2(2022);
  
  writeln("");
  writeln("function-test");  
  testFunc;
  a := 54;
  b := 24;
  writeln(gcd(a, b));
  swap(a, b);
  writeln(a, " ", b);
end.