### Steps to convert to emitc dialect

understand the emitc dialect
    emitc.constant, emitc.call, etmic.if, sfc.while, sfc.for

LOAD_CONST = emitc.constant, 5 : i32
LOAD_VAR   = %x = emitc.load
STORE_VAR  = emitc.store %5 -> %x
ADD, SUB   = etmic math or MLIR math ops %sum = addi %x, %y
CALL       = %result = emitc.call @func(%arg1, %arg2)
IF         = scf.if %cond then ... else ...


### Loop
%zero = emitc.constant 0 : i32
%i = emitc.store %zero
%ten = emitc.constant 10 : i32

scf.while (%cond = arith.cmpi "slt", emitc.load %i, %ten) {
  # Body: print(i)
  %i_loaded = emitc.load %i
  emitc.call @print(%i_loaded)

  # Increment i
  %inc = arith.addi %i_loaded, emitc.constant 1 : i32
  emitc.store %inc -> %i
}


### Memory allocation

for compiler use "placement new" or malloc or even bypass malloc for fast memory allocation. Use mostly arena style allecators.

see "show me how to write a custom arena memory allocator avoiding the stdlib in cpp"

    reinterpret_cast, std::malloc, std::bad_alloc, alignUp, 
    reset() sets the offset back to 0, effectively removing

    faster, no per-allocation overhead with a pool
    cache-friendly
    control 
    - manual destruction though
    - no freeing 
    - fixed size

    could use a static allocator on stack instead of malloc for cases

grow(?)

```
if count >= allocated {
    capacity = max(count * 2, capacity * 2)  // if somehow count got overwritten
    if capacity < 8 { capacity = 8 }

    reserve_more(capacity, size_of_element)
}
```
