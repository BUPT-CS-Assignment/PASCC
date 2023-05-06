program TestFlowControl;
var
  n: integer;
begin
  n := 3;

  writeln("if .. then .. else");
  if n > 0 then writeln("n > 0")
  else writeln("n <= 0");

  writeln("");
  writeln("case .. of .. end");
  case n of
    1: writeln('1');
    2: writeln('2');
    3: writeln('3')
  end;

  writeln("");
  writeln("while .. do");
  n := 1;
  while n <= 3 do
  begin
    writeln(n);
    n := n + 1;
  end;

  writeln("");
  writeln("repeat .. until");
  n := 1;
  repeat
    writeln(n);
    n := n + 1;
  until n = 3;

  writeln("");
  writeln("for .. to/downto .. do");
  for n := 3 downto 1 do
    writeln(n);
  for n := 1 to 3 do
    writeln(n);
end.