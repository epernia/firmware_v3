# minut
Minimal Unit Testing for C

## Features
* Single header implementation
* Simple "API"
* Full abstraction

## Example

```c
#include "minut.h"

// Functionality of the application.
// Intentionally left uncompleted.
int add(int a, int b)
{
    return 0;
}

int main(void)
{
    // Enable MINUT with 1. Disable with 0.
    MINUT(1);

    // This part is not going to be executed.
    int a = add(1, 2);
    printf("a = %d\n", a);
    
    return 0;
}

// Test definitions
TEST(add_1)
{
    ASSERT_EQ(
        5,          // golden
        add(2, 3)); // functionality result
}

TEST(add_2)
{
    ASSERT_EQ(0, add(-3, 3));
}

// Declare tests to be run:
MINUT_BEG
    RUN(add_1());
    RUN(add_2());
MINUT_END

```

Compile and execute:
```sh
$ ./hello
test add_1 ... FAILED: assertion failed: "5 == add(num_a, num_b)"
test add_2 ... ok
test result: FAILED. 1 passed; 1 failed
```


## Usage

1. Include **minut** in your file:
```c
#include "minut.h"
```

2. Define test functions (e.g. `this`) for your functionality with `TEST(test_function_name)` and assert with `ASSERT_EQ(golden, result)`:
*Note: If you need a custom comparator you can use `ASSERT_EQ_CMP(cmp, golden, result)`*
```c
TEST(this)
{
    golden = ...;    // the value you expect
    result = ...;    // your function's return value
    ASSERT_EQ(golden, result);
}

TEST(that)
{
    golden = ...;    // the value you expect
    result = ...;    // your function's return value
    ASSERT_EQ(golden, result);
}
```

3. Declare which tests are going to run:
```c
MINUT_BEG
    RUN(this());
    RUN(that());
MINUT_END
```

4. Run the tests by placing `MINUT(enable)` just before the implementation of your application:
```c
MINUT(true);    // Run the tests and terminate process
MINUT(false);   // Avoid tests and continue application execution
```
