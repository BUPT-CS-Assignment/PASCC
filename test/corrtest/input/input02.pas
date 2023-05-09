program TestRecord;
var
  rec1: record x: integer; y: char; end;
  rec2: record x: integer; y: char; end; 
  rec3: record x: record x1: integer; x2: real; end; y: char; end;
begin
  // record 赋值
  rec1.x := 2023;
  rec1.y := 't';
  rec2.x := rec1.x;
  rec2.y := rec1.y;
  writeln(rec2.x);
  writeln(rec2.y);

  // 嵌套 record 赋值
  rec3.x.x1 := 2023;
  rec3.x.x2 := 3.14;
  rec3.y := 't';
  writeln(rec3.x.x1);
  writeln(rec3.x.x2);
  writeln(rec3.y);
end.