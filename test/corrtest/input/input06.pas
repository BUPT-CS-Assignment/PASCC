program TestStd;

var
  x: integer;
  c: char;
  b: boolean;
  f: real;

begin
  x := -5;
  writeln('abs(', x, ') = ', abs(x));
  
  writeln('sqr(', x, ') = ', sqr(x));

  writeln('odd(', x, ') = ', odd(x));

  x := 65;
  writeln('chr(', x, ') = ', chr(x));

  c := 'B';
  writeln('ord(', c, ') = ', ord(c));

  writeln('succ(', c, ') = ', succ(c));

  writeln('pred(', c, ') = ', pred(c));

  f := 3.8;
  writeln('round(', f, ') = ', round(f));

  f := 3.8;
  writeln('trunc(', f, ') = ', trunc(f));

  f := 90;
  writeln('sin(', f, ') = ', sin(f));

  f := 180;
  writeln('cos(', f, ') = ', cos(f));

  f := 2;
  writeln('exp(', f, ') = ', exp(f));

  f := 10;
  writeln('ln(', f, ') = ', ln(f));

  f := 16;
  writeln('sqrt(', f, ') = ', sqrt(f));

  f := 1;
  writeln('arctan(', f, ') = ', arctan(f));

  writeln('eof = ', eof);

  writeln('eoln = ', eoln);
end.