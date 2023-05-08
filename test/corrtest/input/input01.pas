program TestOperator;
var
  a, b: boolean;
  c, d: integer;
  e, f: real;
  g, h: char;
begin
  a := true;
  b := false;
  c := 10;
  d := 3;
  e := 10.5;
  f := 2.5;
  g := 'a';
  h := 'b';

  writeln("bool operand");
  writeln(a and b);
  writeln(a or b);
  writeln(not a);
  writeln(a = b);
  writeln(a <> b);
  writeln(a < b);
  writeln(a > b);
  writeln(a <= b);
  writeln(a >= b);

  writeln("int operand");
  writeln(+c);
  writeln(-c);
  writeln(c + d);
  writeln(c - d);
  writeln(c * d);
  writeln(c / d);
  writeln(c mod d);
  writeln(c div d);
  writeln(c = d);
  writeln(c <> d);
  writeln(c < d);
  writeln(c > d);
  writeln(c <= d);
  writeln(c >= d);

  writeln("real operand");
  writeln(e + f);
  writeln(e - f);
  writeln(e * f);
  writeln(e / f);
  writeln(e = f);
  writeln(e <> f);
  writeln(e < f);
  writeln(e > f);
  writeln(e <= f);
  writeln(e >= f);

  writeln("char operand");
  writeln(g = h);
  writeln(g <> h);
  writeln(g < h);
  writeln(g <= h);
  writeln(g > h);
  writeln(g >= h);

  writeln("real int operand");
  writeln(e + c);
  writeln(e - c);
  writeln(e * c);
  writeln(e / c);
  writeln(e = c);
  writeln(e <> c);
  writeln(e < c);
  writeln(e > c);
  writeln(e <= c);
  writeln(e >= c);

  writeln("int real operand");
  writeln(c + e);
  writeln(c - e);
  writeln(c * e);
  writeln(c / e);
  writeln(c = e);
  writeln(c <> e);
  writeln(c < e);
  writeln(c > e);
end.