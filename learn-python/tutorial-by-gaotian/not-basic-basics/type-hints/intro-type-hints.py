from typing import Optional, Sequence, Union


# type hint for params and return values
# simple builtin types
def f(a: int, b: int) -> int:
    return a + b


# "a" + 2
f(1, 2)
# f("a", 2)


# type hint for container types, list, tuple
# NOTE list[int] works since python 3.9
#     older usage: typing.List[int]
def g(nums1: Sequence[int], nums2: list[int]) -> int:
    return sum(nums1) + sum(nums2)


g((1, 2, 3), [1, 2])
# g((1, 2, 3), [1, 'a'])
# g((1, 2, 3), (1, 2))


# the value may be None
def h(x: Union[int, None], y: Optional[int]) -> int:
    if x is None:
        x = 0
    if y is None:
        y = 0
    return x + y


h(1, None)
h(None, 1)
# h("a", 1)
