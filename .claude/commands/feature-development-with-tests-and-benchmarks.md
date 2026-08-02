---
name: feature-development-with-tests-and-benchmarks
description: Workflow command scaffold for feature-development-with-tests-and-benchmarks in profanity2.
allowed_tools: ["Bash", "Read", "Write", "Grep", "Glob"]
---

# /feature-development-with-tests-and-benchmarks

Use this workflow when working on **feature-development-with-tests-and-benchmarks** in `profanity2`.

## Goal

Implements a new feature or optimization in the OpenCL kernel, accompanied by new or updated tests and benchmarks to verify correctness and performance.

## Common Files

- `profanity.cl`
- `keccak.cl`
- `tests/bench_mod_mul.cpp`
- `tests/bench_mod_mul_pr49.cpp`
- `tests/harness.cl`
- `tests/test_correctness.cpp`

## Suggested Sequence

1. Understand the current state and failure mode before editing.
2. Make the smallest coherent change that satisfies the workflow goal.
3. Run the most relevant verification for touched files.
4. Summarize what changed and what still needs review.

## Typical Commit Signals

- Edit or optimize the kernel source file (profanity.cl, keccak.cl).
- Update or add new test source files (tests/test_correctness.cpp, tests/bench_mod_mul.cpp, tests/harness.cl, tests/testutil.hpp).
- Update or add benchmark files (tests/bench_mod_mul.cpp, tests/bench_mod_mul_pr49.cpp).
- Update Makefile or tests/Makefile to include new tests/benchmarks.
- Update .gitignore if new generated files are created.

## Notes

- Treat this as a scaffold, not a hard-coded script.
- Update the command if the workflow evolves materially.