
## Overview

I implemented a command line interpreter (CLI) or, a shell. When you type in a command (in response to its prompt), the shell
creates a child process that executes the command you entered and then prompts for more user input when it has finished.


## Program Specifications

### Basic Shell: `wish`

The shell can be invoked with either no arguments or a single argument;
anything else is an error. Here is the no-argument way:

```
prompt> ./wish
wish> 
```

The mode above is called interactive mode, and allows the user to type
commands directly. The shell also supports a *batch mode*, which instead reads
input from a batch file and executes commands from therein. Here is how you
run the shell with a batch file named `batch.txt`:

```
prompt> ./wish batch.txt
```

### Basic Shell

The shell runs in a while loop, repeatedly asking for input to tell it what command to execute. 
It then executes that command. The loop continues indefinitely, until the user types the built-in
command `exit`, at which point it exits. That's it!

### Built-in Commands

Whenever the shell accepts a command, it checks whether the command is
a built-in command or not. Build in commands: `exit`, `cd`, and `path` 

* `exit`: When the user types `exit`, your shell should simply call the `exit`
  system call with 0 as a parameter. It is an error to pass any arguments to
  `exit`. 

* `cd`: `cd` always take one argument (0 or >1 args should be signaled as an
error). To change directories, use the `chdir()` system call with the argument
supplied by the user; if `chdir` fails, that is also an error.

* `path`: The `path` command takes 0 or more arguments, with each argument
  separated by whitespace from the others. A typical usage would be like this:
  `wish> path /bin /usr/bin`, which would set `/bin` and `/usr/bin` as the
  search path of the shell. If the user sets path to be empty, then the shell
  should not be able to run any programs (except built-in commands). The
  `path` command always overwrites the old path with the newly specified
  path. 

### Parallel Commands

The shell also allows the user to launch parallel commands.

```
wish> cmd1 & cmd2 args1 args2 & cmd3 args1
```


Finally, keep versions of your code. More advanced programmers will use a
source control system such as git. Minimally, when you get a piece of
functionality working, make a copy of your .c file (perhaps a subdirectory
with a version number, such as v1, v2, etc.). By keeping older, working
versions around, you can comfortably work on adding new functionality, safe in
the knowledge you can always go back to an older, working version if need be.

