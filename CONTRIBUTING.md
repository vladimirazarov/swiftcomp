# Code Conventions:

## 1. Indentation and Spacing:
* Use 4 spaces for indentation, no tabs.
* Place spaces around operators, e.g., x = y + z;.
* Use a space after commas in function arguments, e.g., func(x, y, z);.

## 2. Naming Conventions:
* Variables and functions: snake_case.
* Constants and macros: UPPER_SNAKE_CASE.
* Typedefs and structs: CamelCase.
## 3. Braces:
``` 
if (condition) {
    // code
} else {
    // code
}
```
## 4. Comments:
* Use English for all comments.
* Use /* ... */ for multi-line comments and // for single-line comments.
* Comment above the code, not beside it.
* Functions should have comments describing their purpose, parameters, and return values.
## 5. Git Conventions:
1. Branch Naming:
* Feature branches: feature/feature-name.
* Bugfix branches: bugfix/bug-name.
2. Commit Messages:
* Use the imperative mood ("Add feature" not "Added feature").
3. Pull Requests:
* Provide a concise description of the changes.
* Ensure that the code has been tested and passes all tests before submitting.
## 6. Code Reviews:
* At least one team member must review and approve the pull request.
* Address all comments and suggestions before merging.
## 7. Testing:
* Write unit tests for all core functionality.
* Aim for a minimum of 80% code coverage.

# Tools:
1. Formatter and other stuff
VS Marketplace Link: https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools