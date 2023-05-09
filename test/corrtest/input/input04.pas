program TestFlowControl(input, output);
var
  n: integer;
  m: boolean;
  ch: char;
begin
  n := 3;
  m := true;
  ch := 'c';

  writeln('if .. then .. else');
  if not m then writeln('n <= 0')
  else 
    if n >= 5 then 
    begin
      n := n - 1;
      writeln('n >= 5')
    end
    else 
    begin
      n := n + 1;
      writeln('n < 5')
    end;

  writeln;
  writeln('case .. of .. end');
  case n of
    1, 2, 3: writeln('n is between 1 and 3');
    4, 5, 6: writeln('n is between 4 and 6');
    7: writeln('n is 7')
  end;
  case m of
    true: writeln('m is true');
    false: writeln('m is false')
  end;
  case ch of
    'a': writeln('ch is a');
    'b': writeln('ch is b');
    'c': writeln('ch is c')
  end;

  writeln();
  writeln('while .. do');
  n := 1;
  while n <= 3 do
  begin
    writeln(n);
    n := n + 1;
  end;

  writeln;
  writeln('repeat .. until');
  n := 1;
  repeat
    writeln(n);
    n := n + 1;
  until n = 3;

  writeln();
  writeln('for .. to/downto .. do');
  for n := 3 downto 1 do
    writeln(n);
  for n := 1 to 3 do
    writeln(n);
end.