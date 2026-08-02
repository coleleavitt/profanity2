---
name: documentation-update-and-enhancement
description: Workflow command scaffold for documentation-update-and-enhancement in profanity2.
allowed_tools: ["Bash", "Read", "Write", "Grep", "Glob"]
---

# /documentation-update-and-enhancement

Use this workflow when working on **documentation-update-and-enhancement** in `profanity2`.

## Goal

Updates or enhances documentation, including usage examples, build instructions, and troubleshooting guides.

## Common Files

- `README.md`
- `docs/BUILD_UBUNTU.md`
- `docs/BUILD_WINDOWS.md`

## Suggested Sequence

1. Understand the current state and failure mode before editing.
2. Make the smallest coherent change that satisfies the workflow goal.
3. Run the most relevant verification for touched files.
4. Summarize what changed and what still needs review.

## Typical Commit Signals

- Edit or add documentation files (README.md, docs/BUILD_UBUNTU.md, docs/BUILD_WINDOWS.md).
- Add new documentation files if needed.
- Link documentation changes to related issues or contributions.
- Commit all documentation changes together.

## Notes

- Treat this as a scaffold, not a hard-coded script.
- Update the command if the workflow evolves materially.