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
