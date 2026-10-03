---
name: style-review
description: Reviews uncommitted changes against the C++ styleguide and produces a structured markdown report. Use when the user asks to review their changes, check style compliance, or audit diff against master.
---

# Style Review Skill

Reviews uncommitted changes (compared to master) against the project's C++ coding styleguide and produces a structured markdown report in chat.

## Invocation

Run this skill when the user asks to review their changes against the styleguide. The skill does the following:

1. Determine the project root (directory containing `AGENTS.md`)
2. Find the styleguide file
3. Compute `git diff master`
4. Parse the diff and check changed lines against applicable styleguide rules
5. Report findings

## Step 1: Find the Styleguide

Look for the styleguide file in this order:

1. **Explicit reference in AGENTS.md:** Search `AGENTS.md` (in the project root, i.e., the directory containing `AGENTS.md`) for a path reference to a styleguide file (e.g., backticks containing a path to a `.md` file in `docs/`, or an explicit sentence like "read `docs/01-coding-style.md`"). Use the first relevant path found.

2. **Heuristic fallback:** If no explicit reference is found, look in the `docs/` directory for a file whose name suggests it is a styleguide: match files matching patterns like `*coding-style*`, `*style*`, `*conventions*`, `*formatting*`.

3. **Error:** If neither method finds a file, tell the user: "Could not find a styleguide file. Please add an explicit reference in `AGENTS.md` or place a styleguide file in `docs/`."

## Step 2: Compute the Diff

Run:

```bash
git diff master -- '*.cpp' '*.h' '*.hpp' '*.ixx'
```

This captures both staged and unstaged changes on the current branch compared to master. Only C++ source, header, and module interface files are considered.

If there are no changes, report: "No changes compared to master. Nothing to review."

## Step 3: Parse the Styleguide

Read the styleguide file and extract its top-level section headers (lines starting with `## `).

For each section, determine if it is **actionable** by scanning its content for keywords such as:

- Mandatory/forbidden: `never`, `avoid`, `don't`, `must`, `should not`, `do not`
- Recommended: `use`, `always`, `prefer`, `preferably`

If a section contains at least one such keyword, it is actionable. Skip sections that are purely descriptive or informational (e.g., "C++ Standard" explaining availability of C++26).

If no actionable sections are found, report: "The styleguide has no actionable rules to check."

## Step 4: Parse the Diff

Parse the diff output to extract:

- Which files were changed
- For each file, the list of added or modified lines (lines starting with `+` in the diff, excluding the `+++` file header)
- Line numbers in the resulting file (not the diff offset)

Only lines that were actually added or modified count. Deleted lines (`-` prefix) are ignored.

## Step 5: Check Against Rules

For each actionable section in the styleguide, run the corresponding checker against all changed lines. Map styleguide section names to checkers:

| Styleguide Section | Checker |
|---|---|
| `Formatting and Output` | Check for `std::stringstream`, `std::cout`, `std::strstream`, `<sstream>` usage |
| `Ranges` | Check for manual `for` loops with index-based iteration over containers (e.g., `for (size_t i = 0; i < ...`; look for manual index access like `container[i]`) |
| `Layout` | Check for incorrect indentation (not 4 spaces), opening braces on the same line as declarations/statements |
| `Concepts` | Check for `typename T` without concept constraints, `std::enable_if`, unconstrained `auto` in templates |
| `Error Handling` | Check for ignoring `[[nodiscard]]` return values (difficult via diff alone — flag as advisory if context allows) |
| `Include Order` | Check that `#include` directives in changed blocks follow the correct order: corresponding header, local project headers, third-party headers, standard library headers (check blank line separation between sections) |
| `Attributes and Specifiers` | Check for missing `[[nodiscard]]`, `constexpr`, `const`, or `noexcept` on functions that qualify (check function signatures in changed lines and nearby context for advisories) |
| `Naming Conventions` | Check variable names, function names, class names, enum names against the correct case convention (`PascalCase`, `camelCase`, `m_camelCase`, `g_camelCase`, `SCREAMING_SNAKE_CASE`, `snake_case`) |
| `Special Member Functions` | Check if a class declares some special member functions but not all five (requires nearby context check) |
| `Casts` | Check for C-style casts: `(Type)expr` pattern (parenthesized type followed directly by expression), `reinterpret_cast`, `const_cast`; flag `reinterpret_cast` when `std::bit_cast` would work (advisory) |
| `Ownership and Aliasing` | Check for raw pointer function parameters (`T*` or `T* const`) where a reference would express the same contract, and `std::shared_ptr` usage (prefer `std::unique_ptr` or references) |
| `Comments and Documentation` | Check for emoji characters in comments, comments that state the obvious (repeating what the code does), or excessive comments |
| `Function Design` | Check for function names containing "And" or "Or" (indicating multiple responsibilities) |

### How to Check

For each changed line, check if it violates the rule described in the corresponding styleguide section. When a violation is found:

1. Record the **file name**, **line number**, the **full line content**, and the **rule violated**
2. Provide a brief **how to fix** suggestion (one line, clear and actionable)

### Advisory Check (Nearby Context)

After checking all changed lines, do a second pass on the surrounding context:

- For each changed line, look at lines within the same **function** or **immediate code block** (the surrounding `{ ... }` block)
- Check if there are styleguide violations on lines that were *not* changed but are in the same scope
- These are recorded as **advisory** items (not violations)

Advisories should be recorded with the same format as violations (file, line, content, fix suggestion) but are kept separate.

## Step 6: Generate the Report

Produce a markdown report with the following structure:

```markdown
# Style Review Report

## Summary

- Files reviewed: N
- Lines changed: M
- Violations found: X
- Advisories found: Y

## Violations

### <Section Name>

| File | Line | Code | Fix |
|------|------|------|-----|
| `cpu.cpp` | 42 | `std::stringstream ss;` | Use `std::format` instead of `std::stringstream` |
| `cpu.cpp` | 157 | `for (size_t i = 0; i < v.size(); ++i)` | Use `std::ranges::for_each` or range-based `for` |

### <Next Section Name>

...

(Sections with zero violations are omitted)

## Advisories

(Flat list — not grouped by category)

| File | Line | Code | Fix |
|------|------|------|-----|
| `cpu.cpp` | 43 | `m_data = parse(ss.str());` | Use `std::format` for consistency |

(If no advisories, write "No advisories.")

## Notes

(Any unparseable files, skipped sections, etc.)
```

If there are **zero violations and zero advisories**, produce a brief report:

```markdown
# Style Review Report

## Summary

- Files reviewed: N
- Lines changed: M
- ✅ No styleguide violations found.
```

## Implementation Notes

- Parse the diff line-by-line to track which file we're in and what the current line number is
- Track line numbers by counting from the `@@` hunk header (which gives the start line and count)
- For advisory checks, identify function boundaries by matching `{` to `}` nesting
- Be conservative in detection — false positives are better than false negatives, but avoid noise
- When in doubt about a complex pattern (e.g., is this a manual loop or a range-based loop?), err on the side of not flagging it unless the pattern is clear
- The skill should work in any C++ project — the styleguide is the source of truth, not hardcoded rules
