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
  testFunc := 54;
end;

function gcd(a, b: integer): integer;
begin
  if b = 0 then gcd := a
  else gcd := gcd(b, a mod b);
end;

procedure swap(var x, y: integer);
var
  temp: integer;
begin
  temp := x;
  x := y;
  y := temp;
end;

begin
  writeln('procedure-test');
  testPro1;
  testPro2(2022);
  
  writeln();
  writeln('function-test');  
  a := testFunc;
  b := 24;
  writeln(gcd(a, b));
  swap(a, b);
  writeln(a, ' ', b);
end.