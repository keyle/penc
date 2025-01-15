# symbol tables

```

symbol_table = {
    0: {"x": "int", "y": "string"},  # Global scope
    1: {"z": "float"},              # Scope level 1
    2: {"x": "char"}                # Scope level 2 (shadowing)
}

```

we can do it with an array of dictionaries or map rather

NOTE: the order is wrong in the following examples, we want Reverse Polish Notation (RPN), not this

```

["load", "x", "add", "y", "store", "z"]

after

["load", "%x", "add", "%y", "store", "%z"]

```

## delay variable lookups after the bytecode has been created?

probably not. 

that said further semantic analysis will be handled after, such as dead code?


## Hybrid bytecode + analysis:
    check if variable are declared and accessible
    have one or multiple semantic passes afterwards




## Order of Semantic Passes

1    Scope Resolution: Ensure variables and labels are properly defined.
2    Type Checking: Validate types and insert conversions.
3    Control Flow Analysis: Verify labels and blocks.

## Optimization:

1    Constant Folding
2    Dead Code Elimination
3    Inline Expansion

Final Cleanup: Ensure resources are managed correctly.






# type checking / typing

`["load", "x", "add", "y", "store", "z"]`

if x is a float and y is an integer, rewrite the bytecode to add the conversion

`["load", "%x", "convert_int_to_float", "%y", "add", "store", "%z"]`

dissallow ambiguous type conversion

ensure operations are valid for given types

e.g. adding a string to an integer.


# constant folding

`["push", 2, "push", 3, "add", "store", "x"]`

`["push", 5, "store", "x"]`


# deadcode illimination

`["push", 0, "jump_if_false", "L1", "push", 42, "store", "x", "L1:"]`

`["push", 0, "jump_if_false", "L1", "L1:"]`

# control flow analysis

`["L1:", "push", 1, "jump", "L2", "L3:", "push", 2, "L2:"]`

L3 is never used.


# inlining

Reverse Polish Notation (RPN)


`add(2, 3)` -> `2, 3, add, call`


`add(x, y) => x + y` -> `L_add:, x, y, +, return, x, assign, L_add`

yank the L_add label contents until the return, replacing x->2, y->3, add with the operation +;

it's basically a copy paste?


Note, inlining with recursion is a bad idea, it will be really long bytecode.


# variable lifetime and freeing resources

Analyze variable usage to:
    Free resources (e.g., memory or file handles) when they go out of scope.
    Insert cleanup instructions (e.g., finally block handling).






