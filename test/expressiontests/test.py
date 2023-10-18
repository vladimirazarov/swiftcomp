import os
import subprocess
import unittest

def run_parser(code):
    current_dir = os.path.dirname(os.path.abspath(__file__))
    parser_path = os.path.join(current_dir, '../../parser')
    result = subprocess.run([parser_path], input=code, capture_output=True, text=True)
    return result.returncode, result.stdout, result.stderr

class TestParser(unittest.TestCase):

    def test_basic_expression(self):
        code = 'x = 12 + 3'
        self.assertEqual(run_parser(code), (0, '+ 12 3 ', ''))

    def test_precedence(self):
        code = 'x = 12 * 3 + 10'
        self.assertEqual(run_parser(code), (0, '+ * 12 3 10 ', ''))

    def test_parenthesis_precedence(self):
        code = 'x = 57 * (2 + 10)'
        self.assertEqual(run_parser(code), (0, '* 57 + 2 10 ', ''))

    def test_same_precedence(self):
        code = 'x = 10 * 2 / 5'
        self.assertEqual(run_parser(code), (0, '/ * 10 2 5 ', ''))

    def test_same_precedence_with_parenthesis(self):
        code = 'x = 10 * (2 / 5)'
        self.assertEqual(run_parser(code), (0, '* 10 / 2 5 ', ''))
    
    def test_nested_parenthesis(self):
        code = 'x = 10 * (10 + 34 / 5)'
        self.assertEqual(run_parser(code), (0, '* 10 + 10 / 34 5 ', ''))

    def test_multiple_parenthesis(self):
        code = 'x = (10 + 5) * (3 - 1)'
        self.assertEqual(run_parser(code), (0, '* + 10 5 - 3 1 ', ''))

    def test_long_expression(self):
        code = 'x = ((2 + 2) / (3 - 1)) + 3 * (2 - 3)'
        self.assertEqual(run_parser(code), (0, '+ / + 2 2 - 3 1 * 3 - 2 3 ', ''))


if __name__ == '__main__':
    unittest.main()