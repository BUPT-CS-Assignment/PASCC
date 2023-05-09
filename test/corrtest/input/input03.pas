program TestArray;
var  
  arr1: array[1..3] of integer;  
  arr2, arr3: array [1..10] of array ['a'..'z'] of record x: integer; y: char; end; 
begin
  { 一维数组赋值 }
  arr1[1] := 1;
  arr1[2] := 2;
  arr1[3] := 3;
  writeln(arr1[3]);
  
  { 二维record数组赋值 }
  arr2[1]['a'].x := 1;
  arr2[1]['a'].y := 'a';
  arr2[2]['b'].x := 2;
  arr3[1]['a'].x := arr2[1]['a'].x;
  writeln(arr3[1]['a'].x);
  
  { 二维数组行拷贝 }
  arr3[2] := arr2[2];
  writeln(arr3[2]['b'].x);
  
  { 二维数组拷贝 }
  arr3 := arr2;
  writeln(arr3[1]['a'].y);
end.