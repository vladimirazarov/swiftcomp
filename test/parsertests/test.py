import os
import subprocess

def run_parser(code):
    current_dir = os.path.dirname(os.path.abspath(__file__))
    parser_path = os.path.join(current_dir, '../../parser')
    result = subprocess.run([parser_path], input=code, capture_output=True, text=True)
    return result.returncode, result.stdout, result.stderr

def test_declaration():
    code = 'var x : Int'
    assert run_parser(code) == (0, '', '')

def test_initialization():
    code = 'let x : Int = 5'
    assert run_parser(code) == (0, '', '')

def test_assignment():
    code = 'x = 5'
    assert run_parser(code) == (0, '', '')

def test_function_call():
    code = 'print("Hello, world!")'
    assert run_parser(code) == (0, '', '')

def test_return_statement():
    code = 'return 5'
    assert run_parser(code) == (0, '', '')

def test_while_loop():
    code = 'while x < 10 {}'
    assert run_parser(code) == (0, '', '')

def test_if_cond():
    code = 'if x < 10 {} else {}'
    assert run_parser(code) == (0, '', '')

def test_function_definition():
    code = 'func add(x : Int, y: Int) -> int {}'
    assert run_parser(code) == (0, '', '')

def test_block():
    code = '{ let x : Int = 5 }'
    assert run_parser(code) == (0, '', '')

def test_empty_program():
    code = ''
    assert run_parser(code) == (0, '', '')

if __name__ == '__main__':
    test_declaration()
    test_initialization()
    test_assignment()
    test_function_call()
    test_return_statement()
    test_while_loop()
    test_if_cond()
    test_function_definition()
    test_block()
    test_empty_program()
    print("All tests passed!")