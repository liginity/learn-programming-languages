import sys


"""Learn how to define and use decorators inside class definition.

Since python 3.10, use staticmethod (see version 2 below) to create a decorator that works in 3 places:
- in the class
- through the class name
- through object instance of the class


Video:
- title 【python】如何在class内部定义一个装饰器？这里的坑你要么不知道，要么不会填！
- link https://www.bilibili.com/video/BV1FV4y1g7hJ/
"""


def show_class_name(cls):
    """Decorate a class definition, print information."""
    print("-" * 80)
    print(cls.__name__)
    return cls


def log_function(func):
    def wrapper(*args, **kwargs):
        print("function start!")
        print(f"name: {func.__name__}")
        print(f"args: {args}")
        ret = func(*args, **kwargs)
        print("function end!")
        return ret

    return wrapper


"""
Version1: decorator is defined as a normal mothod.

The decorator could be used through an object instance of the class.
"""


@show_class_name
class DecoratorsVersion1:
    def log_function(self, func):
        def wrapper(*args, **kwargs):
            print("function start!")
            print(f"name: {func.__name__}")
            print(f"args: {args}")
            ret = func(*args, **kwargs)
            print("function end!")
            return ret

        return wrapper

    # NOTE not working
    # @log_function
    # def fib(self, n):
    #     if n <= 1:
    #         return 1
    #     return self.fib(n - 1) + self.fib(n - 2)


dv1_obj = DecoratorsVersion1()


@dv1_obj.log_function
def fib(n):
    if n <= 1:
        return 1
    return fib(n - 1) + fib(n - 2)


fib(2)


"""
Version2: the decorator is created with a staticmethod.

The decorator could be used through the class and the object instance of the class.
And it can be used inside the class since python 3.10.
"""


@show_class_name
class DecoratorsVersion2:
    @staticmethod
    def log_function(func):
        def wrapper(*args, **kwargs):
            print("function start!")
            print(f"name: {func.__name__}")
            print(f"args: {args}")
            ret = func(*args, **kwargs)
            print("function end!")
            return ret

        return wrapper

    print(f"(in DecoratorsVersion2 definition) {log_function = }")

    if sys.version_info >= (3, 10):

        @log_function
        def fib(self, n):
            if n <= 1:
                return 1
            return self.fib(n - 1) + self.fib(n - 2)


print(f"(outside DecoratorsVersion2 definition) {DecoratorsVersion2.log_function = }")

dv2_obj = DecoratorsVersion2()
if sys.version_info >= (3, 10):
    dv2_obj.fib(1)


@DecoratorsVersion2.log_function
def f2():
    pass


f2()


@dv2_obj.log_function
def g2():
    pass


g2()


"""
Version3: the decorator is a normal function inside the class.

The decorator could be used inside the class, and used through the class.
"""


@show_class_name
class DecoratorsVersion3:
    # NOTE copy definition of log_function outside.
    #     just do not write it manually.
    log_function2 = log_function

    @log_function2
    def f(self):
        pass


dv3_obj = DecoratorsVersion3()
dv3_obj.f()


@DecoratorsVersion3.log_function2
def f3():
    pass


f3()
