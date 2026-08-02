```markdown
# profanity2 Development Patterns

> Auto-generated skill from repository analysis

## Overview

This skill teaches the core development patterns, coding conventions, and workflows used in the `profanity2` repository—a TypeScript project focused on OpenCL kernel development, testing, and cross-platform support. It covers how to add features, fix bugs, update documentation, and manage builds, with practical examples and command suggestions for efficient collaboration.

## Coding Conventions

- **File Naming:**  
  Files use PascalCase (e.g., `Dispatcher.cpp`, `Keccak.cl`).

- **Import Style:**  
  Relative imports are standard.
  ```typescript
  import { SomeUtil } from './SomeUtil';
  ```

- **Export Style:**  
  Named exports are preferred.
  ```typescript
  export function myFunction() { ... }
  export const MY_CONST = 42;
  ```

- **Commit Messages:**  
  Freeform, typically around 61 characters. No strict prefixing.

## Workflows

### Feature Development with Tests and Benchmarks
**Trigger:** When adding a new kernel feature or optimizing an existing one, ensuring correctness and performance.  
**Command:** `/add-kernel-feature-with-tests`

1. Edit or optimize kernel source files (`profanity.cl`, `keccak.cl`).
2. Update or add relevant test files (e.g., `tests/test_correctness.cpp`, `tests/harness.cl`).
3. Update or add benchmark files (e.g., `tests/bench_mod_mul.cpp`).
4. Update `Makefile` or `tests/Makefile` to include new tests/benchmarks.
5. Update `.gitignore` if new generated files are created.
6. Commit all related changes together.

**Example:**
```cpp
// In profanity.cl
__kernel void new_feature(...) { ... }
```
```cpp
// In tests/test_correctness.cpp
TEST_CASE("New feature works as expected") {
    // Test logic
}
```

---

### Documentation Update and Enhancement
**Trigger:** When improving or clarifying documentation for users or developers.  
**Command:** `/update-docs`

1. Edit or add documentation files (`README.md`, `docs/BUILD_UBUNTU.md`, `docs/BUILD_WINDOWS.md`).
2. Add new documentation files if needed.
3. Link documentation changes to related issues or contributions.
4. Commit all documentation changes together.

**Example:**
```markdown
# Building on Ubuntu
Follow these steps to build...
```

---

### Windows Build Support Update
**Trigger:** When adding or fixing Windows build support or instructions.  
**Command:** `/update-windows-build`

1. Edit source files to fix Windows-specific issues (`Dispatcher.cpp`, `profanity.cpp`).
2. Edit `Makefile` to support Windows targets.
3. Update or add Windows build documentation (`docs/BUILD_WINDOWS.md`).
4. Update `README.md` if needed.
5. Commit all related changes together.

**Example:**
```cpp
// Dispatcher.cpp
#ifdef _WIN32
// Windows-specific code
#endif
```

---

### Kernel Bugfix or Compatibility Fix
**Trigger:** When fixing kernel code to work on more platforms or with stricter compilers.  
**Command:** `/fix-kernel-compat`

1. Edit kernel source files (`profanity.cl`, `keccak.cl`).
2. Commit changes with a message referencing the platform or compiler fixed.

**Example:**
```c
// keccak.cl
// Fix for stricter OpenCL compiler
```

---

### Merge Pull Request or Branch
**Trigger:** When integrating changes from a feature or fix branch into the main codebase.  
**Command:** `/merge-pr`

1. Merge the branch or pull request.
2. Resolve any conflicts.
3. Commit the merge with a message referencing the PR or branch.

**Example:**
```sh
git merge feature/new-kernel-optimization
# Resolve conflicts if any
git commit -m "Merge feature/new-kernel-optimization"
```

## Testing Patterns

- **Framework:** Unknown (C++-style tests detected in `.cpp` files).
- **Test File Pattern:** Files named `*.test.ts` for TypeScript, and `test_*.cpp` for C++.
- **Typical Test Example:**
  ```cpp
  // tests/test_correctness.cpp
  TEST_CASE("Kernel returns correct result") {
      // Test logic
  }
  ```
- **Adding Tests:**  
  Place new test files in the `tests/` directory and update the `Makefile` as needed.

## Commands

| Command                        | Purpose                                                        |
|---------------------------------|----------------------------------------------------------------|
| /add-kernel-feature-with-tests  | Add or optimize a kernel feature with tests and benchmarks     |
| /update-docs                   | Update or enhance documentation                                |
| /update-windows-build          | Add or fix Windows build support                               |
| /fix-kernel-compat             | Fix kernel code for compatibility or bugfixes                  |
| /merge-pr                      | Merge a pull request or branch                                 |
```
