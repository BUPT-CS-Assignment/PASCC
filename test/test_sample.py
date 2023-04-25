import pytest
import subprocess

def run_program(input_file):
    input_file_c = input_file.replace('.txt', '.c')
    subprocess.Popen(['../bin/PASCC_TEST', '-i', input_file, '-o', input_file_c], stdout=subprocess.PIPE).wait()
    
    input_file_exe = input_file.replace('.txt', '')
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
    ("input/input01.txt", "output/output01.txt"),
    ("input/input02.txt", "output/output02.txt")
])

def test_program_with_multiple_cases(input_file, expected_output):
    check_program(input_file, expected_output) 