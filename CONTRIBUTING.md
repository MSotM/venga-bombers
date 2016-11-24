# Contributing

## Commits

- Work in feature branches. Development should happen on branches split off
  from the `master` branch, documentation on branches split off from the `doc`
  branch.
- Rebase your feature branch onto `master` or `doc` regularly
- Make commits of logical units, keeping separate changes separate. Use `git add -p`
- Review your changes before committing. Use `git add -p`
- Make sure that each individual commit doesn't break anything
- Make sure your commit messages are in the proper format, as described [here][tpope]:

  ```
  Capitalized, short (50 chars or less) summary

  More detailed explanatory text, if necessary.  Wrap it to about 72
  characters or so.  In some contexts, the first line is treated as the
  subject of an email and the rest of the text as the body.  The blank
  line separating the summary from the body is critical (unless you omit
  the body entirely); tools like rebase can get confused if you run the
  two together.

  Write your commit message in the imperative: "Fix bug" and not "Fixed bug"
  or "Fixes bug."  This convention matches up with commit messages generated
  by commands like git merge and git revert.

  Further paragraphs come after blank lines.

  - Bullet points are okay, too

  - Typically a hyphen or asterisk is used for the bullet, followed by a
    single space, with blank lines in between, but conventions vary here

  - Use a hanging indent
  ```

[tpope]: http://tbaggery.com/2008/04/19/a-note-about-git-commit-messages.html

## Code

### General

- Never introduce trailing whitespace
- Make sure all files end with a newline
- Only use LF line endings
- Make sure no line is longer than 79 characters
- Use `kebab-case` for all file names

### C

- Use two spaces for indentation
- Do not indent the `case` statements in a `switch`
- Use a guard of the following form in all header (`.h`) files:
  (in this example, the file is called `file-name.h`)

  ```
  #ifndef FILE_NAME_H
  #define FILE_NAME_H

  ...contents...

  #endif /* FILE_NAME_H */
  ```

- Use `enum`s instead of `#define`s when the things you're defining are "enumerable"
- Use `typedef` to define relevant type names if the actual type is an implementation detail
- Booleans should be of type `bool`, from `stdbool.h`
- The `*` for pointers goes directly before the name: `player_t *player`
- Use `lowercase_names_with_underscores` for everything
- Use the `type_name_t` naming convention for types
- Use `typedef struct {...} struct_name_t` for all struct definitions
- Use `typedef enum {...} enum_name_t` for all enum definitions
- Only use `/* ... */` for comments, not `//`
- Use `/* TODO: thing */` for comments specifying things you have postponed
- Mark sections of a file using `/* Section name --- */` comments, extending to the maximum line length
