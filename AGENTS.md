# AGENTS.md

## Documentation-First Workflow

**Always read the `docs/` folder first** before exploring or modifying any code in this project.

The `docs/` folder contains structured documentation that summarizes the entire codebase:

```
docs/
├── 00-overview.md        # Project structure, build, architecture
├── 01-cpu.md             # CPU, registers, flags, execution model
├── 02-address-bus.md     # AddressBus, Cartridge, memory model
├── 03-instructions.md    # Instruction set, handler patterns, opcode table
├── 04-utilitylib.md      # Byte math, bit masks, suffixes, helpers
├── 05-testing.md         # Test framework, mocks, test file listing
└── 06-build-system.md    # CMake, Conan, code style
```

**Read these docs in order (00 → 01 → 02 → ...)** to get context before:
- Writing new code
- Adding tests
- Modifying existing code
- Debugging or investigating issues
- Exploring unfamiliar parts of the codebase

## Incremental Changes & TDD

**Always make small, incremental changes.** Never rewrite entire files or make large sweeping changes.

**Always use TDD (Test-Driven Development):**
1. Write the failing test first
2. Write just enough code to make it pass
3. Refactor

For new instructions or features:
- Add the test first (or update an existing test)
- Run the test to confirm it fails
- Implement the minimal change to make it pass
- Refactor both test and implementation together
- Update docs to reflect the new feature

Keep each change focused and testable. This makes debugging easier and keeps the codebase stable as it grows.

Only explore source files directly when:
- The docs reference a specific file to read
- A question cannot be answered by the existing docs
- Verifying details not captured in the docs

## Expand Documentation

When you are asked to do something that is **not covered** by the existing `docs/` documentation, **update or create a doc file** before making code changes. This keeps the documentation current and helpful for future AI sessions.

Rules:
1. **Read the relevant docs first** to check if the topic already exists
2. If it doesn't exist or is incomplete, **add a new file or update an existing one**
3. Use the naming convention `NN-name.md` (07, 08, etc.) with a descriptive title
4. Place new docs in `docs/` alongside the existing ones
5. Update `docs/00-overview.md` to include a link to any new documentation files
6. Keep docs concise but thorough — they should be sufficient for an AI agent to navigate the codebase

Examples of when to add docs:
- New modules or libraries added to the project
- New instruction families or hardware components
- Changes to build system, tooling, or CI/CD
- New testing patterns or infrastructure
- Important architectural decisions or conventions not yet documented
