from typing import Callable


# section 1
print("-" * 80)
print("a closure, with 1 captured variable")


def f() -> Callable:
    data: list[int] = []

    def inner(val: int) -> list[int]:
        data.append(val)
        return data

    return inner


"""
Here, g is a function, and it is also a closure.
As a closure, g treats variable data as a free variable, and stores it in __closure__.
"""


g = f()
print(f"{g = }")
print(f"{g(1) = }")
print(f"{g(2) = }")
print(f"{g.__closure__ = }")
print(hex(id(g(3))))


# section 2
print("-" * 80)
print("a closure, with 3 captured variables")


def f2() -> Callable:
    data1: list[int] = []
    data2: list[str] = []
    data3: list[float] = []

    def inner(val: int) -> None:
        data1.append(val)
        data2.append(str(val))
        data3.append(float(val))
        print(
            f"{data1 = } ({hex(id(data1))}), {data2 = } ({hex(id(data2))}), {data3 = } ({hex(id(data3))})"
        )

    return inner


g2 = f2()
g2(1)
g2(2)
print(f"{g2.__closure__ = }")
