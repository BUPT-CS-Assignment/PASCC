program TestDefinition;

const
  c1 = 1;
  c2 = -3.14;
  c3 = 'a';
  c4 = -c1;

type 
  t1 = integer;
  t2 = array[1..3] of real;
  t3 = record x: integer; y: char; end;
  t4 = record m1: t2; m2: t3; end;

var
  v1: t1;
  v2: t2;
  v3: t3;
  v4: t4;

begin
  writeln('const-test');
  writeln(c1);
  writeln(c2);
  writeln(c3);
  writeln(c4);

  writeln;
  writeln('type and var - test');
  v1 := 2023;
  writeln(v1);

  v2[1] := 1.1;
  v2[2] := 2.2;
  v2[3] := 3.3;
  writeln(v2[1], ' ', v2[2], ' ', v2[3]);

  v3.x := 2023;
  v3.y := 't';
  writeln(v3.x, ' ', v3.y);

  v4.m1 := v2;
  v4.m2.x := v3.x;
  v4.m2.y := v3.y;
  writeln(v4.m2.x, ' ', v4.m2.y);
end.