program TestArray;
var
  a, b: array [1..10] of array ['a'..'z'] of record x: integer; y:char end;
begin
  a[2]['a'].x := 10;
  b := a;
  b[3] := a[2];
  b[5]['b'].y := 'c';
  writeln(b[2]['a'].x);
  writeln(b[5]['b'].y);
end.