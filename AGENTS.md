# AGENTS.md

## Documentation-First Workflow

**Always read the `docs/` folder first** before exploring or modifying any code in this project.

The `docs/` folder contains structured documentation that summarizes the entire codebase:

```
docs/
├── 00-overview.md        # Project structure, build, architecture
├── 01-coding-style.md    # C++26 coding conventions, naming, attributes, error handling
├── 02-cpu.md             # CPU, registers, flags, execution model
├── 03-address-bus.md     # AddressBus, Cartridge, memory model
├── 04-instructions.md    # Instruction set, handler patterns, opcode table
├── 05-utilitylib.md      # Byte math, bit masks, suffixes, helpers
├── 06-testing.md         # Test framework, mocks, test file listing
└── 07-build-system.md    # CMake, Conan, code style
```

**Read the relevant docs** before tackling your task. Only read the documentation files that relate to
the specific feature, component, or problem you're working on. Use the doc index to quickly find the
right document for your context.

**Before writing any C++ code, read `docs/01-coding-style.md`** — it contains mandatory coding
conventions (naming, attributes, include order, modern C++ patterns) that apply to all code. Do not
skip it. Do not infer conventions from examples. Follow it explicitly.

**The existing codebase may contain deviations from the styleguide.** This is expected — the
styleguide is aspirational and the codebase is a work in progress. **The styleguide is always
leading.** When the existing code conflicts with the styleguide, the styleguide wins. Always
write new code according to the styleguide, and when modifying existing code, correct any style
guide violations you encounter.

## Incremental Changes & TDD

**Always make small, incremental changes.** Never rewrite entire files or make large sweeping changes.

**Always use TDD (Test-Driven Development).** When building a feature test-first, the `tdd-incremental` skill enforces a
strict, one-test-at-a-time red-green-refactor cycle with detailed guidance — use it whenever the user asks to build
something "using TDD," "test-first," wants tests written before code, or asks to add functionality one small test at a
time. See the skill for the full workflow.

**When creating or modifying C++ code, read and follow `docs/01-coding-style.md`** — it specifies
mandatory conventions: `std::format`/`std::println` over `stringstream`/`cout`, `std::ranges`, concepts on
template parameters, naming rules, include order, attribute usage (`[[nodiscard]]`, `constexpr`, `const`,
`noexcept`), and the rule of 5. This is not optional. **If the existing codebase bends these rules,
correct it.** The styleguide always wins over existing code — never accept a style violation because
"that's how it's written."

Keep each change focused and testable. This makes debugging easier and keeps the codebase stable as it grows.

Only explore source files directly when:

- The docs reference a specific file to read
- A question cannot be answered by the existing docs
- Verifying details not captured in the docs

## Expand Documentation

When you are asked to do something that is **not covered** by the existing `docs/` documentation, **update or create a
doc file** before making code changes. This keeps the documentation current and helpful for future AI sessions.

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
