# Basic-OS
Basic System in C which calculates md5 hashes of files in a distributed manner.
For this purpose, the OS is split into three processes: application.c, view.c, and slaves

## Programming Considerations
* The OS is written in C
* The OS is written for the x86 architecture
* The code is written in "snakeCase Standard"
* All functions are written in the following format:
``` returnType functionName (argumentType argumentName) {
    // code
} 
```

## Commit Considerations
The commit message should be structured as follows:
```
<type>[optional scope]: <description>

[optional body]

[optional footer(s)]
```
The commit contains the following structural elements, to communicate intent to the consumers of your library:

__fix__: a commit of the type fix patches a bug in your codebase (this correlates with PATCH in Semantic Versioning).
__feat__: a commit of the type feat introduces a new feature to the codebase (this correlates with MINOR in Semantic Versioning).

## Application.c
