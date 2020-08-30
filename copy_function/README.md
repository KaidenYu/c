- Use memcpy to copy a function to heap

- Diffenret part of code for different compiler in target_func()
  - clang: `#if defined(__clang__)`
  - gcc: `#elif defined(__GNUC__)`


- Build & run
  ```
  gcc main.c
  ./a.out
  ```
  or
  ```
  clang main.c
  ./a.out
  ```
