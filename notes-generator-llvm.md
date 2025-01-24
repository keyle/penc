# generate to llvm ir

start with our bytecode


`do {b++;} while(a > b);`

`L1:, b, 1, +, b, =, a, b, >, jump_if_true L1`

1. Identify Loop Components

Label L1:: Marks the beginning of the loop.
b, 1, +, b, =: Represents the loop body (b++).
a, b, >, jump_if_true L1: Represents the condition (while (a > b)) and end of block

b, 1, +, b, = → Increment Operation
Translate b = b + 1:

```
%b_new = add i32 %b, 1
store i32 %b_new, i32* %b_ptr
```

a, b, > → Condition Check

```
%a_val = load i32, i32* %a_ptr
%b_val = load i32, i32* %b_ptr
%cond = icmp sgt i32 %a_val, %b_val
```


jump_if_true L1 → Branching

```
br i1 %cond, label %L1, label %exit
```


complete llvm source


```
; Entry block
entry:
  br label %L1            ; Unconditional jump to the loop

; Loop body block
L1:
  %b_val = load i32, i32* %b_ptr
  %b_new = add i32 %b_val, 1
  store i32 %b_new, i32* %b_ptr

  ; Condition block
  %a_val = load i32, i32* %a_ptr
  %b_val_updated = load i32, i32* %b_ptr
  %cond = icmp sgt i32 %a_val, %b_val_updated
  br i1 %cond, label %L1, label %exit

; Exit block
exit:
  ret void
```


# linking without clang

there is a way to not use the system's linker, clang is default on macOS, and use the llvm-lld one but I couldn't get it to link on macOS

https://stackoverflow.com/questions/60434145/are-there-any-c-apis-for-lld

lld::macho is for mac, elf is for linux and mingw for windows. It also needs to be a c++ project, there is no C API. See `cpp-linker` branch in penc2

other refs 
    https://stackoverflow.com/questions/20823371/what-is-the-difference-between-the-functions-of-the-exec-family-of-system-calls
    
    The llvm-ld tool has been removed. The clang driver provides a more reliable solution for turning a set of bitcode files into a binary. To merge bitcode files llvm-link can be used instead.
    https://releases.llvm.org/3.1/docs/ReleaseNotes.html#tools_changes


# linking with clang

1- create llvm IR builder and module
2- build the main func etc. entry
3- initalize targets, find the right targetmachine
4- emit to file .o
5- link via clang or ld (clang is preferred on macOS) via execvp (or system) to produce the binary.
    as noted above there is a possiblity to do it without invoking the system's compiler but llvm-lld but 
                I haven't been able to get that running, it might be removed from recent versions of LLVM 
    also, maybe it can be done with (brew install) lld but I couldn't manage to link lld::macho properly.

