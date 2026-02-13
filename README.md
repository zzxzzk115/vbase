# vbase

**vbase** is the foundational core library for the Vultra ecosystem and other related projects.

It provides essential, standardised logic and architectural primitives shared across multiple modules.

---

## Purpose

vbase defines:

- Core value types
- Architectural patterns
- Lightweight utilities
- Module & service infrastructure
- Event and signalling primitives

It is designed to be:

- Small
- Engine-agnostic
- Dependency-light
- Stable
- Reusable across projects

---

## What vbase Provides

### Core Utilities

- `UUID`
- `StrongID`
- `Result<T, E>`
- `StringView`
- `Span`
- `BinaryReader / BinaryWriter`
- `ScopeExit`
- `Pimpl`

### Architectural Primitives

- `IModule`
- `ModuleRegistry`
- `ServiceRegistry` (Service Locator)
- `Signal`
- `EventBus`
- `Singleton`

These form the base architectural vocabulary for higher-level systems.

---

## Design Philosophy

1. Minimal dependencies
2. No global runtime state (except explicitly designed patterns)
3. Clear ownership semantics
4. Header-first, lightweight abstractions
5. Engine-independent

vbase should remain stable and rarely change.

---

## Example Usage

See `examples/` for usage samples covering:

- UUID generation
- Result handling
- Module & service setup
- EventBus and Signal
- Binary IO
- StrongID usage

and more!

---

## License

This project is under the [MIT](./LICENSE) license.
