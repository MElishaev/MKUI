# Coding Conventions

This file defines the project-specific coding conventions for this repository.

Update these sections over time as the project evolves.

## General Principles
- Prefer clear, readable code over clever shortcuts.
- Preserve existing architecture and style unless there is a strong reason to change it.
- Keep functions and classes focused on one responsibility.
- Avoid unrelated refactors while working on a scoped task.

## Unreal Engine and C++
- Prefer forward declaration where possible and includes where needed
- non-delegate class members should start with the prefix `mMemberName`
- delegate members and reflected delegate properties are the exception to the
  `m` prefix and should start with capital `On<SomethingHappened>`
- local variables should start with lower case and follow camelCase: localVar
- class methods should start with lower case and follow camelCase
- class members should be below class methods in each private/public/protected section
- constexpr constants should use UPPER_CASE_STYLE
- comments should be added describing less trivial members and functions
- Editor- or Blueprint-exposed `UPROPERTY` and `UFUNCTION` declarations require
  a `Category` specifier for the game to package
- internal `UPROPERTY` members used only for serialization or garbage collection
  should not add a `Category` unless they are exposed to the Editor or Blueprints

## Blueprints
- Prefer doing code in c++ and expose tweakable data to blueprints for fast iterations and testing

## Formatting
- Before complicated code, add short description of what it does
- prefer using spaces instead of tabs

## Notes
- If repository conventions and existing code patterns conflict, prefer the explicit guidance in this file unless you intentionally decide to migrate the codebase toward a new standard.

## Game Systems
Here the different game systems' markdown files will be mentioned.
When you need to modify some of the game systems, first read the appropriate .md file to understand what is the purpose and what this system does.
Also, when you update the design of the system significanly, update the relevant .md to reflect these changes.
