program TestArray;
var
  arr: array[1..5] of integer;
  i: integer;
begin
  arr[1] := 1;
  arr[2] := 3;
  arr[3] := 5;
  arr[4] := 7;
  arr[5] := 9;
  for i := 1 to 5 do
    writeln(arr[i]);
end.