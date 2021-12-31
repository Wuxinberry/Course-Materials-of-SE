# UML notes 02 Use case

collected by wxb



## 1 Use Case

* 定义

    A description of a potential series of interactions between a role (known as an actor) and a system, to achieve a goal.

    描述 角色和系统之间为实现目标而进行的一系列交互

    本质上是功能分解的思想，不是OOT面向对象

* 用途
    * 指定主体（系统或系统的一部分）的行为，描述主体执行的动作序列，包括变体，以产生可观察到的价值结果给 actor。
    * 需求的一部分，用来描述功能性需求 functional requirements

* use case 的实例就是 scenario

### 1.1 use case 的表示

* 用**椭圆 + 文字标签**进行描述
* 文字要求用**动宾结构**

![image-20211228164543128](https://s2.loli.net/2021/12/28/Vtk6uc9JvmoLa3d.png)

### 1.2 scenario 情境

* 表示贯穿用例的一条路径，用来表示用例的一种特定情况，是用例的实例

* 每个用例都有一系列的 scenario 情境

    其中一个是primary scenario主要场景，其他是secondary scenario次要场景

    次要场景常常用于描述异常或替代路径

### 1.3 characteristics of use case 用例的特征

用例是 需求requirement、分析analysis、设计design、开发development、测试test、部署deployment的单元

* 用例必须相对独立，具有功能完整性 functionally complete

    > 正确用例：取出钱
    >
    > 错误用例：填取钱表

* 用例执行的结果必须可被观察且有意义 observable

    > 正确用例：登录系统
    >
    > 错误用例：输入密码

* 用例必须由 actor 触发，不能自动开始，也不能由另一个用例触发

    > 正确用例：操作员 -> 取出钱
    >
    > 错误用例：ATM -> 吐钱

* 用例名字必须为动宾结构

    > 正确用例：人 -> 吃饭
    >
    > 错误用例：人 -> 吃

### 1.4 relationship

关系可以用来更好地表达系统的 common part 和 variant part

* 用例和 actor 之间的关系：association
* 用例之间的关系
    * <\<include>>: 从 dependency 依赖关系衍生而来
    * <\<extend>>: 从 dependency 依赖关系衍生而来
    * <\<generalization>>: 继承

#### include: has-a

* 表示一个用例 (base use case) 的行为包含incorporate了另一个用例 (included use case) 的行为，是一种特殊的依赖关系

* 用于避免重复，并且被包含的用例不会单独存在

* 表示方式：箭头从 base use case 指向被包含的用例

    ![image-20211228184334651](https://s2.loli.net/2021/12/28/HySMQer2bJER8a3.png)

#### extend: is-a

* 表示一个用例extension拓展了另一个用例base use case的行为，与继承关系类似，也是依赖关系的一种特例，但是extension points需要被指定，并且只能在这几个点拓展

* base use case 可以单独存在，且有意义，必须被完善定义

    extension use case本身没有意义

* extension use case 可以访问、修改base use case中的属性

    base use case 则无法感知 extension use case 的存在，因此也不能访问或修改extension use case的属性和操作

* 表示方式：箭头从 extension use case 指向 base use case

    ![image-20211228190122676](https://s2.loli.net/2021/12/28/s4f1Wezi76hClB2.png)

#### generalization: is-a

* 和类的泛化相同，表示child use case 从 parent use case 继承行为和意义，child use case 可以重写和增加 parent use case 的行为

* 很少使用

* 表示方法：箭头从child use case 指向 parent use case，使用实线空心箭头

    ![](https://s2.loli.net/2021/12/28/XJOzw5hfnpP2tdK.png)

### 1.5 寻找use case 的原则

* Heuristic principles 启发式原则
    * 与用户沟通
    * 把自己当成actor
    * actor 和 use case 的定义不能分离



## 2 Use Case Diagram

* 属于动态建模

* represent a group of use cases, actors and the relationships between them

    表示一组用例与参与者的关系

* a use case model is made up of several use case diagrams

    用例模型由几个用例图构成

* 描述每个用例中 executor 的交互，描述外部用户的外部视角

### 2.1 functions

* Capture the system user requirement

    捕获系统的用户需求

* Describe the boundary of the system

    描述系统边界

* Designate the system’s external behavior

    设计系统的外部行为

* Give guidance to the developer for the functional development

    给开发者进行功能开发的指导

* Is the start point of the system. Guide the design of all class diagrams and interaction diagrams

    系统的起始点；引导所有的类图设计和交互图设计

* Produce test cases and user documents

    创建测试用例和用户手册

* Estimate the person-months and the process of the project

    估算项目的人月（开销）和进程

### 2.2 actor definition 参与者的定义

* 使用当前系统的人、物、另一个系统
* 不能是当前系统的一部分
* 每个用例都有actor，
* actor与use case 之间只有association的关联：actor 和 use case 之间互相通信

#### actor 表示方式

* 火柴人：actor是人类
* label: actor是外部系统
* decoration: 人或系统都行

![image-20211228163224481](https://s2.loli.net/2021/12/28/wpZgAlUY1bm2skN.png)

#### relationship between actors 参与者之间的关系

**generalization 泛化**

- 与面向对象中的继承关系相似
- child actor, parent actor
- 子参与者除了继承父参与者的行为和语义以外，还可以自己增加行为和语义
- 凡是父参与者使用的地方，子参与者都可以使用



## 3 Use Case Description

* user: developer, user, project manager, tester

* 内容：没有统一形式，尽可能充分

    * use case identification：识别 use case

        描述, id, name, 创建者，创建日期，最后修改人，最后修改日期

    * use case definition：定义 use case

        描述，actors, trigger, priority, state, preconditions, postconditions

        normal flow, alternative flow, exceptions

    ![image-20211228193514865](https://s2.loli.net/2021/12/28/x7FofWKNtIOArUq.png)

    ![image-20211228193819615](https://s2.loli.net/2021/12/28/UVTgKLvkaypJ7cq.png)

    

