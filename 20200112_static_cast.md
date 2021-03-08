# static_cast

static_cast是一个c++运算符，功能是把一个表达式转换为某种类型，但没有运行时类型检查来保证转换的安全性。

## 用法

static_cast <type-id>( expression )

该运算符把expression转换为type-id类型，但没有运行时类型检查来保证转换的安全性。它主要有如下几种用途：

①用于[类层次结构](https://baike.baidu.com/item/类层次结构)中基类（父类）和[派生类](https://baike.baidu.com/item/派生类)（子类）之间[指针](https://baike.baidu.com/item/指针)或[引用](https://baike.baidu.com/item/引用)的转换。

进行上行转换（把派生类的指针或引用转换成基类表示）是安全的；

进行下行转换（把基类指针或引用转换成派生类表示）时，由于没有动态类型检查，所以是不安全的。

②用于基本数据类型之间的转换，如把int转换成char，把int转换成enum。这种转换的安全性也要开发人员来保证。

③把空指针转换成目标类型的空指针。

④把任何类型的表达式转换成void类型。

注意：static_cast不能转换掉expression的const、volatile、或者__unaligned属性。

C++中static_cast和reinterpret_cast的区别

[C++primer](https://baike.baidu.com/item/C%2B%2Bprimer)第四版第五章里写了[编译器](https://baike.baidu.com/item/编译器)隐式执行任何[类型转换](https://baike.baidu.com/item/类型转换)都可由static_cast显示完成;reinterpret_cast通常为操作数的位模式提供较低层的重新解释

## 示例

1、C++中的static_cast执行非[多态](https://baike.baidu.com/item/多态)的转换，用于代替C中通常的转换操作。因此，被做为显式类型转换使用。比如：

```
int i;
float f = 166.71;
i = static_cast<int>(f);
```

此时结果，i的值为166。

2、C++中的reinterpret_cast主要是将数据从一种类型的转换为另一种类型。所谓“通常为操作数的位模式提供较低层的重新解释”也就是说将数据以[二进制](https://baike.baidu.com/item/二进制)存在形式的重新解释。比如：

```
int i;
char *p = "This is an example.";
i = reinterpret_cast<int>(p);
```

此时结果，i与p的值是完全相同的。reinterpret_cast的作用是说将指针p的值以[二进制](https://baike.baidu.com/item/二进制)（位模式）的方式被解释为整型，并赋给i，//i 为整型；一个明显的现象是在转换前后没有数位损失。