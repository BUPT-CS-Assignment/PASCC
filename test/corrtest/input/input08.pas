program testReadAndWrite;
var
  v_i: integer;
  v_r: real;
  v_c: char;
  v_b: boolean;
  
  rec: record x: integer; y: char; end;
  arr: array[1..3] of integer;
  str: array[1..5] of array[1..5] of char;
  
begin
  read(v_i);
  read(v_r, v_c, v_b);
  write(v_i);
  write(' ');
  write(v_r, ' ', v_c, ' ', v_b);
  writeln;
  
  read(rec.x, rec.y);
  writeln(rec.x, ' ', rec.y);

  read(str[1]);
  writeln(str[1]);

  readln(arr[1], arr[2]);
  read(arr[3]);
  writeln(arr[1], ' ', arr[2], ' ', arr[3]);
end.