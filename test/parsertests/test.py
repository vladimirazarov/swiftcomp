import os
import subprocess
import unittest

def run_parser(code):
    current_dir = os.path.dirname(os.path.abspath(__file__))
    parser_path = os.path.join(current_dir, '../../parser')
    result = subprocess.run([parser_path], input=code, capture_output=True, text=True)
    return result.returncode, result.stdout, result.stderr

class TestParser(unittest.TestCase):

    def test_declaration_with_type(self):
        code = 'var x : Int'
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_declaration_without_type(self):
        code = 'var x'
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_initialization_with_type(self):
        code = 'let x : Int = 5'
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_initialization_without_type(self):
        code = 'let x = 5'
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_function_call_assignment(self):
        code = 'let x : Int = id()'
        self.assertEqual(run_parser(code), (0, '', ''))
    
    def test_expression_assignment(self):
        code = 'let x : Int = 4+5+"slkfda"'
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_simple_function_definition(self):
        code = '''func decrement(of n: Int, by m: Int) -> Int {
                        return n - m
                    }'''
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_void_function_definition(self):
        code = ' func vvoid(of n: Int, by m: Int) {}'
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_function_early_return_statement(self):
        code = ''' func decrement(of n: Int, by m: Int) -> Int {var x : Int = "lalallaalla" var x = functionLol() return n - m let k : String = "adflsfjlk" let m = 3 var k1 = 20.0}'''
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_function_call(self):
        code = 'print("Hello, world!")'
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_function_call_one_arg(self):
        code = 'print(x)'
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_function_call_more_args(self):
        code = 'add(x, y, z)'
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_assignment_func(self):
        code = 'x = add(a, b)'
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_func_call_different_args1(self):
        code = 'x = add(a, 4)'
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_func_call_different_args2(self):
        code = 'x = add(first: 3,second: b)'
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_func_call_different_args3(self):
        code = 'x = add(first:a, 50.0)'
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_func_call_different_args4(self):
        code = 'x = add(a, second:"string")'
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_func_call_different_args5(self):
        code = 'x = add(a, second:"string", "string", 3, 50.0, hello : "world")'
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_assignment_expression(self):
        code = 'x = a + b - c'
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_if(self):
        code = 'if x < 10 {} else {}'
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_while(self):
        code = 'while x < 10 {}'
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_block(self):
        code = '{ let x : Int = 5 }'
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_nested_blocks(self):
        code = '{ { let x : Int = 5 } }'
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_multiple_function_definitions(self):
        code = '''func foo() {}
                  func bar() {}'''
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_altering_function_definitions_and_statements(self):
        code = '''func foo() {}
                  let x : Int = 5
                  func bar() {}'''
        self.assertEqual(run_parser(code), (0, '', ''))

    def test_expressions_and_declarations(self):
        code = '''func foo() {}
                  let x : Int = 5 * 5 + 5 - x + aaaa - dslafjlk
                  x = 324 * 3241514 - 3241
                  var x = 228
                  func bar() {}'''
        self.assertEqual(run_parser(code), (0, '', ''))






if __name__ == '__main__':
    unittest.main()