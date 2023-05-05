import re
import pytest
import subprocess

test_case_index = [1]

def remove_ansi_escapes(text):
    ansi_escape = re.compile(r'\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])')
    return ansi_escape.sub('', text)

def run_program(input_file):
    output, error = subprocess.Popen(['../bin/PASCC', '-d', '0', '-i', input_file], stderr=subprocess.PIPE).communicate()
    return remove_ansi_escapes(error.decode('utf-8'))

def read_expected_output(output_file):  
    with open(output_file, 'r') as f:
        expected_output = f.read()
    expected_output = expected_output.replace('\\t', '\t')
    return expected_output

def check_program(input_file, expected_output):
    output = run_program(input_file)
    expected_output = read_expected_output(expected_output)
    
    assert output == expected_output

@pytest.mark.parametrize("input_file, expected_output", [
    (f"errtest/input/input{i:02d}.pas", f"errtest/output/output{i:02d}.txt") for i in test_case_index
])

def test_program_with_multiple_cases(input_file, expected_output):
    check_program(input_file, expected_output)   