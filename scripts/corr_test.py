import pytest
import subprocess

test_case_num = 12

def run_program(input_file):
    input_file_c = input_file.replace('.pas', '.c')
    subprocess.Popen(['../bin/PASCC', '-i', input_file, '-o', input_file_c], stdout=subprocess.PIPE).wait()
    
    input_file_exe = input_file.replace('.pas', '')
    process = subprocess.Popen(['gcc', '-o', input_file_exe, input_file_c], stdout=subprocess.PIPE).wait()
   
    output, error = subprocess.Popen([input_file_exe], stdout=subprocess.PIPE).communicate()
    return output.decode('utf-8').strip()

def read_expected_output(output_file):  
    with open(output_file, 'r') as f:
        expected_output = f.read().strip()
    return expected_output

def check_program(input_file, expected_output):
    output = run_program(input_file)
    expected_output = read_expected_output(expected_output)
    assert output == expected_output

@pytest.mark.parametrize("input_file, expected_output", [
    (f"../test/corrtest/input/input{i:02d}.pas", f"../test/corrtest/output/output{i:02d}.txt") for i in range(1, test_case_num + 1)
])

def test_program_with_multiple_cases(input_file, expected_output):
    check_program(input_file, expected_output)   