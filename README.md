# IceCrystal简介

 IceCrystal是一个大场景（数字地球）解决方案，它包含了渲染器Soul和基于Soul构建的开放场景渲染库LargescaleScene。

 Soul是一个OpenGL渲染库。不要将它与Ogre库弄混了，尽管Soul和Ogre有一些共同的特征，但Soul更小一点。Soul库提供了用于
 三维渲染的核心功能：
- 用于矢量和矩阵运算的数学类
- 面向对象的OpenGL接口，从磁盘加载网格、纹理或着色器的资源框架。
- 提供了一个非常小但通用的场景图以描述三维场景，这个场景图基于任务调度框架。

 LargescaleScene底层使用Soul，是一个开放场景渲染库。
它被设计用来进行实时渲染大范围的场景，最大可以达到整个星球的规模。
在行星规模情况下不可能在GPU的显存中存储所有的地景数据，而是要在运
行过程中根据视点的位置产生相应的数据。这可以通过从硬盘中加载预先计
算好的数据，或者通过算法生成数据来实现。LargescaleScene的另一个目
标是实时编辑地景，它是通过在运行过程中重新生成地景数据实现的，同时
在视点移动时也重新生成地景数据。

LargescaleScene由内核库和几个扩展插件组成。内核库提供了生成器（producer）框架、地形
（terrain）框架和一个基本的用户接口框架。
- 生成器框架定义了所有数据生成器的通用接口，例如 CPU 或 GPU 生成器、栅格数
据或矢量数据生成器等。生成器可以使用其它生成器产生的数据来生成数据，然后
进行复杂的组合（例如地形法线生成器可以使用高程生成器产生的高程数据来产生
地形的法线）。生成器框架提供了一个通用的缓存组件来存储产生的数据，它利用
了时间相干性：通过缓存，在一帧中产生的数据能够在以后的帧中重复使用；
- 地形框架使用了基于当前视点动态分割的地形四叉树。它还提供了个 GLSL 一致变
量类型，以访问 GPU 上缓存的栅格数据，以及（通过使用数据生成器）更新这些
缓存和绘制地形的方法。它还提供了用来将平面地形映射到其它形状，如球形（渲
染星球）的变形处理；
- 用户接口框架基于事件处理器。它为 LargescaleScene中显示的大范围场景提供了基本的导
航。
LargescaleScene的插件提供了几个基于以上框架实现的生成器：有的生成器用来产生高程数据、
有的用来产生栅格数据（这些数据可以表示任何你想要的数据，例如光照反射数据、地表物
的类型、法线、等高线、环境遮挡等）、有的用来产生矢量数据（通过将矢量数据栅格化为
纹理，可以将矢量数据转化为栅格数据）。

LargescaleScene在以下方面基于Soul库：
- 生成器框架使用了Soul的任务来产生数据。因此，地景数据可以并行的产生，或
者基于Soul调度框架的预取能力在使用前预先加载数据。这个框架还支持生成器
任务间的依赖关系，因此当一个生成器产生的数据被编辑后，其它直接或间接从这
个数据产生的数据都会自动重新计算；
- 地形框架使用了Soul用于着色器和网格等的渲染框架，并且在场景图中增加了用
于更新 GPU 缓存和绘制地形的方法；
- 最后，LargescaleScene扩展了Soul的资源框架，增加了生成器和地形组件类型资源。

# IceCrystal构建

1. 进入IceCrystal目录

2. 新建一个build目录

3. 进入build目录

4. cmake ../

# 效果演示

![Earth](https://github.com/clojur/IceCrystal/blob/master/Images/Earth1.png)
![im1](https://github.com/clojur/IceCrystal/blob/master/Images/Terrain.png)
![Terrain2](https://github.com/clojur/IceCrystal/blob/master/Images/Terrain2.png)
![Terrain3](https://github.com/clojur/IceCrystal/blob/master/Images/hh.png)
![Terrain4](https://github.com/clojur/IceCrystal/blob/master/Images/hh1.png)
![Terrain5](https://github.com/clojur/IceCrystal/blob/master/Images/sun.png)
