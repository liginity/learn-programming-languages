import inspect
import pprint
# import sys
import types
from typing import Optional


def f() -> None:
    frame = inspect.currentframe()
    # print(frame)
    # pprint.pprint(frame)
    # pprint.pprint(dir(frame))
    for attr in dir(frame):
        if not attr.startswith("f_"):
            continue
        # print(f".{attr} = {getattr(frame, attr)}")

        print(f".{attr} = ", end="")
        if isinstance(getattr(frame, attr), frame.__class__):
            print(frame.__class__)
            continue
        if attr == "f_builtins":
            print("<builtins dict...>")
            continue
        pprint.pprint(getattr(frame, attr), depth=1)


f()
"""
The call stack frames are organized as a linked list.
They are connected with `.f_back`.
"""


print("-" * 80)


def f2() -> None:
    # frame: type(sys._getframe()) = inspect.currentframe()
    # frame: Type[sys._getframe()] = inspect.currentframe()
    frame: Optional[types.FrameType] = inspect.currentframe()
    assert frame is not None
    # NOTE type hint is not working well, mypy complaints.
    print(f"{frame.f_back.f_code.co_name = }")


def g2():
    f2()


g2()
