---
name: tdd-incremental
description: Enforces strict, incremental Test-Driven Development (RED-GREEN-REFACTOR-STOP) instead of writing full implementations upfront. Use this skill whenever the user asks to build a feature "using TDD," "test-first," wants tests written before code, or asks Claude to add functionality one small test at a time. Also trigger if the user says things like "don't implement everything at once," "just write one test," or references red/green/refactor. Once this skill is active for a task, keep following its one-test-at-a-time cycle for all subsequent requests in that task unless the user explicitly asks to stop or switch modes.
---

# Incremental TDD

Guides strict, disciplined Test-Driven Development. The core rule: never write more
production code, or more test code, than the smallest next failing test requires.
No upfront full implementations, no batching multiple tests together, no skipping ahead.

## The Cycle

Each turn covers exactly one RED-GREEN-REFACTOR cycle, then stops:

1. **RED** — Write a single, failing unit test for the smallest possible increment of
   behavior. Not a batch of tests. Not the whole feature's test suite. One test that
   captures the next smallest bit of unimplemented behavior.
2. **GREEN** — Write the absolute minimal code required to make that one test pass.
   Resist the urge to generalize, handle unstated edge cases, or implement more than
   the test demands — even if the fuller solution is obvious. Minimal, sometimes
   deliberately "too simple," code is correct here.
3. **REFACTOR** — Clean up the code just written (naming, duplication, structure)
   while keeping all tests green. If nothing meaningfully needs cleanup, say so
   briefly rather than inventing busywork.
4. **STOP** — After REFACTOR, stop and wait for the user's next instruction. Do not
   automatically continue to the next test or next piece of functionality.

## Gathering context before the first cycle

Before writing the first test, make sure you have (ask if genuinely missing, but infer
from the conversation/repo first):

- **Feature/Goal**: the specific behavior being built, stated as a concrete next
  increment if possible (not just "build auth system").
- **Target language/stack**: language + test framework (e.g. Python/pytest,
  TypeScript/Jest, Go/testing).
- **Relevant existing files**: production files this feature touches.
- **Existing tests**: test files this feature's tests belong in.

If files are referenced but not yet read, read them first (use `view` or the relevant
file-reading skill) so the test and code you propose fit the actual codebase's
conventions, imports, and style rather than guessing.

## Output format (every cycle)

Respond with ONLY these three sections, nothing else — no restated plan, no preview of
future tests, no "next I will...":

```
**TEST:**
[The single new unit test, in the relevant test file. Show it as a diff/snippet
clearly scoped to the relevant file, not a full file dump unless the file is new.]

**CODE:**
[The minimal implementation change to make that test pass, in the target file.
Same scoping rule — show what changed, not an unrelated full rewrite.]

**REFACTOR:**
[Concrete cleanup suggestions applied (or "None needed" with a one-line reason).]
```

Then stop. Do not add commentary after REFACTOR about what's coming next.

## Discipline rules

- **One test per cycle.** If the user's request implies several behaviors (e.g. "add
  password hashing" covers hashing, verifying, salting, error handling), pick the
  single smallest one first and name the rest only if asked, not by writing tests for
  them.
- **Minimal means minimal.** In GREEN, prefer the simplest thing that passes — a
  hardcoded return, a stub, a trivial branch — over a "proper" implementation, if the
  test doesn't yet force generality. Generality gets added later, driven by later
  failing tests, not anticipated now.
- **Don't invent requirements.** If the test needs a design decision the user hasn't
  specified (e.g. which hashing algorithm), make the smallest reasonable choice and
  state the assumption in one line inside REFACTOR or alongside CODE — don't ask a
  clarifying question unless truly blocked.
- **Real failing test first.** The RED test must actually fail against the current
  code as it stands (missing function, wrong behavior, etc.) — not a trivially true
  assertion.
- **No test batching even if asked.** If the user asks for "a few tests at once" or
  "just implement the whole thing," gently remind them this skill follows strict
  one-test-at-a-time TDD, and ask if they want to proceed that way anyway or drop the
  skill for this request.
- **Persist across turns.** After STOP, the user's next message (e.g. "looks good",
  "continue", "next") means: start the next RED-GREEN-REFACTOR cycle for the next
  smallest increment. Keep using the three-section output format each time.
