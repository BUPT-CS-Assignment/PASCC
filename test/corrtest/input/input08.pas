program testString;
var
  str: array[1..10] of array[1..20] of char;
  
begin
  read(str[1]);
  writeln(str[1]);
  str[2] := 'aaa' + 'b' + 'b' + 'b' + 'ccc';
  writeln(str[2]);
end.