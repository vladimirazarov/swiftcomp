# swiftcomp

A compiler for the IFJ23 language, built as a university project.

## Build Instructions

To build the compiler, simply run:

```bash
make
```

This will create the executable named `swiftcomp` in the project root.

## Usage

The compiler takes the source code file as input from standard input.

```bash
./swiftcomp < your_source_file.ifj
```

(Note: Adjust the executable name if changed in the Makefile)

## Example

Assuming you have a file named `example.ifj` with the following content:

```swift
// Example IFJ23 code
func main() {
    let a: Int? = 5
    write(a)
}
```

You can compile and run it (assuming you have an IFJ23 interpreter like `ic23int`) like this:

```bash
./swiftcomp < example.ifj | ic23int
```