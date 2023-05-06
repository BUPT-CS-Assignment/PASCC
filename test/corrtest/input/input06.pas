program TestDefinition;

const
  c1 = 1;
  c2 = -3.14;
  c3 = 'a';
  c4 = -c1;

type
  t1 = integer;
  t2 = array[1..3] of real;

var
  v1: t1;
  v2: t2;

begin
  writeln("const-test");
  writeln(c1);
  writeln(c2);
  writeln(c3);
  writeln(c4);

  writeln(" ");
  writeln("type and var - test");
  v1 := 5;
  v2[1] := 1.1;
  v2[2] := 2.2;
  v2[3] := 3.3;
  writeln(v1);
  writeln(v2[1], " ", v2[2], " ", v2[3]);
end.