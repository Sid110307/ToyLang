# ToyLang

> A simple toy programming language.

## Language

Variables are 32-bit ints, created with `var` and looked up by name.
Anything that starts with a digit is a literal.

| Command        | Effect                                       |
|----------------|----------------------------------------------|
| `var a 2`      | Create variable `a` with value `2`           |
| `let a 5`      | Reassign `a` to `5` (`a` must already exist) |
| `add a b`      | `a = a + b`                                  |
| `sub a b`      | `a = a - b`                                  |
| `mul a b`      | `a = a * b`                                  |
| `div a b`      | `a = a / b`                                  |
| `get a`        | Print the value of `a`                       |
| `print <text>` | Print the rest of the line                   |
| `end`          | Stop execution                               |
| `# ...`        | Comment                                      |

`add`/`sub`/`mul`/`div` always mutate their first argument, which must be an
existing variable. The second argument can be either a variable or a
literal.

### Example

```
var a 2
add a 0
let a 5
add a 1
get a
end
```

Output: `6`.

## Build

```
$ make
```

Requires `gcc` supporting C11 and `make`.

```
$ ./bin/toyLang <file.toy>   # run a program
$ ./bin/toyLang --doc        # print command reference
$ ./bin/toyLang --help       # print usage
```

## Editor support

`plugin/` is a VS Code extension which provides `.toy` syntax highlighting.
