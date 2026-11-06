# minishell

A small shell written in C for the 42 curriculum, by wiwu and dasamuel.

I wrote a printer to vizualise the tokens and Abstract Syntax Tree the shell constructs. Which makes it easier to debug.
Note: Instead of the usual view of a tree with the root in the middle, I use indentation to indicate children (nodes that have the same indentation are siblings).

<img src="docs/ast-branches.png" width="456" alt="OR at the root. Its left child is a pipe of echo and cat. Its right child is a subshell holding AND, with cat and echo below it.">

<img src="docs/ast-two-sides.png" width="477" alt="AND at the root. Its left child is OR and its right child is a pipe, each holding a subshell and a command.">


## Tokens

### Parentheses

<img src="docs/tokens-parentheses.png" width="699" alt="Nested parentheses split into blue tokens, with pipes and && and || around them">

### Pipes, redirections, heredocs

<img src="docs/tokens-pipes-redirects.png" width="560" alt="Pipelines with redirections, and a heredoc piped into cat">

### `&&` and `||`

<img src="docs/tokens-and-or.png" width="560" alt="Commands chained with && and ||">

### Quotes and expansions

An operator inside quotes is just part of the word. `$USER` and `*.txt` are still plain words at this stage, and only get expanded when the command runs.

<img src="docs/tokens-quotes.png" width="560" alt="Quoted strings staying as a single token">

### Trailing operators

<img src="docs/continuation.png" width="560" alt="Continuation prompts for &&, ||, | and (">

### Syntax errors

<img src="docs/syntax-errors.png" width="560" alt="Five rejected commands and their error messages">
