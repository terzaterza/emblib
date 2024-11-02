## Naming and notation

### Classes
Classes are named lowercase with underscores (`_`) separating words. Member variables are prefixed with `m_` and static variables are prefixed with `s_`.

Methods are listed in the order: public, protected, private.
Methods should start with a verb, for example `int get_count()` instead of `int count()`. Static variables inside methods and functions are prefixed with `s_`.

```cpp
class i2c_master {
    ...
}
```

### Structs
Structs are named same as classes with trailing `_s`. Struct members should not be prefixed and visibility inside a struct must not be modified (all members should be public).

If methods inside a struct are needed, they must be `const`.

```cpp
struct i2c_device_desc_s {
    ...
}
```

### Constants
All uppercase with underscores separating words.

Use `constexpr` whenever possible, `#define` for creating constants should only be used for boolean type constants:

```cpp
#define USE_MUTEX_INSIDE_LOGGER     1

constexpr size_t OUT_PORT_BUFFER_SIZE = 12;
```

### Enums
Enum classes should have the `_e` suffix. Members should follow rules for constants.

If using C-style enums (not enum classes), all members should be prefixed by the enum class name, excluding the `_e` suffix.

### Unions
Suffix `_u`. Don't use classes inside unions, only structs and primitive types.

### Template types
Template typenames should have trailing `_type`. When using template literals, they should follow rules for constants.

### Typedefs and using
Suffix `_t`.

### write/read and get/set
Functions (methods) with write/read names assume that communication will take place outside the main memory (reading from i2c, spi...) where as get/set methods work only with the main memory (ram).