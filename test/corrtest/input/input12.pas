program TestRepeat;

var num: integer;

begin
  num := 1;
  repeat
    writeln(num);
    num := num + 1;
  until num = 5
end.