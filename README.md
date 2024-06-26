# FBI

**FBI** is something halfway to a package manager. It allows the user to install programs from different sources and update them.

## But why?

I like to try out a lot of software. Sometimes, this software is so novel or obscure that it can not be found even on the [AUR](https://aur.archlinux.org/). In these cases, I usually clone the source code and compile the program myself.

As a consequence, the number of git repositories (and sometimes code from other places) begin to stockpile on my disk, making maintenance somewhat difficult. This can be somewhat mitigates by a simple shell script like [this one](https://github.com/nmke-de/utils-nm/blob/master/git_update) I wrote myself, but without storing information on how to build/compile these programs, this only goes so far.

**FBI** tries to solve this problem by not only fetching the source code, but also by calling `make` (and in the future, other build programs). This should make maintenance easier.

## What about the name?

**F**etch, **B**uild and **I**nstall!

## Security considerations

If you use this, **be aware of what you install, from which sources!!!**

## Dependencies

Hard dependencies:

- Libc
- Linux (untested for other OSs)

Dependencies which are, in theory, optional, but recommended:

- curl
- git
- make

## Build Dependencies

- A C Compiler

## Build

Make sure to fetch the git submodules. Do this with `git submodule update --init --remote`.

Type `make`. To install, type `sudo make install`. It is recommended to set the `FBI_WORKPLACE` environment variable.

## Usage

```bash
fbi [OPTIONS] <url>
# where OPTIONS includes the following:
#
# FETCH OPTIONS:
[|-git|-hg|-f 'custom command']
#
# BUILD OPTIONS:
[|-go|-haredo|-make|-vlang|-b 'custom command']
#
# INSTALL OPTIONS:
[|-cd-install|-go-install|-haredo-install|-make-install|-i 'custom command']

# alternatively, to update, this:
fbi update
```
