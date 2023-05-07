program testString;
var
  str: array[1..10] of char;
  
begin
  read(str);
  writeln(str);
  
  str := "aaa" + "bbb" + "ccc";
  writeln(str);
end.