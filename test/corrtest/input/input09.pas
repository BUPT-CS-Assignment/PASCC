program TestQsort;
  
var 
  n: integer;
  a: array[0..10000] of integer;
  
procedure quickSort(l, r: integer);
var
  i, j, pivot: integer;
begin
  if l < r then
  begin
    i := l;
    j := r;
    pivot := a[i];
    while i < j do
    begin
      while (i < j) and (a[j] >= pivot) do
        j := j - 1;
      a[i] := a[j];
      while (i < j) and (a[i] <= pivot) do
        i := i + 1;
      a[j] := a[i];
    end;
    a[i] := pivot;
    quickSort(l, i - 1);
    quickSort(i + 1, r);
  end;
end;

procedure readData;
var
  i: integer;
begin
  read(n);
  for i := 0 to n - 1 do
    read(a[i]);
end;

procedure printData;
var
  i: integer;
begin
  for i := 0 to n - 1 do
    write(a[i], ' ');
end;

begin
  readData;
  quickSort(0, n - 1);
  printData;
end.