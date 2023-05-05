program TestFunction;

var a, b, ret: integer;
c: real;

function max(num1, num2: integer): integer;
var
  result: integer;
begin
  if (num1 > num2) then
    result := num1
  else
    result := num2;
end;
    
begin
   a := 100;
   b := 200;
   c := 300;
   ret := max(a, b);
   write(ret);
end.