# 实现说明
- 1.CFormFactory是启动入口,在static实例构造时`CUIController::Instance()->AddUIInitCallback(this)`将自己注册给UISDK。
    SDK初始化完成会调用onUIInit(),UIWork在这个方法开始。
- 2.`CFormFactory::Init`负责解析xml文件和sc图片、加载页面，以及开启子线程监听xml文件变化。xml文件解析是在UI线程调用`CUIController::Instance()->parseXml`方法，解析完成后调用`CFormFactory::LoadForm`加载并显示页面
- 3.新增actor子类CVisualizationActor,新增Stage子类CVisualizationStage，新增form子类CVisualizationForm，事件和描画处理由这些自定义布局实现

# XML解析
- 1.解析入口是`CUIController::Instance()->parseXml`方法，这个方法使用tinyxml开源库解析xml文件，并根据元素和属性new出对应的actor或stage和form，然后赋值属性。
- 最终返回一个堆上的form指针，这个form包含完整的xml内容，用于显示和更新描画。字布局的创建由UIWork在CFormFactory::onGetChildActorFromChildElement中根据xml定义自己完成。

# 布局描画
- 1.可视化项目的布局有三个基类，分别是CVisualizationActor,CVisualizationForm和CVisualizationStage。这三个类为这个项目处理布局的共通部分，例如标注线和自身矩形框的描画，鼠标事件的处理等。

# 标注描画
- 1.描画完actor后，描画标注线；这块现在想在actor draw方法最后，从canvas的currentmutex中获取移动值，这个值是屏幕左上角到actor左上角的。也可以参考GP2023
- 2.然后调用canvas的画线，画string

# 优化
- 1.针对UIWork引用的头文件做封装，封装为baselib.so和frame.so
- 2.新增对其他元器件的支持，依据是元器件一览表
    - 一共15个类型元器件，actor：6个 stage:7个 list和adapter 2个。总工作量大概40人/日

    | 元器件名称 | 工作量 | 其他说明 |
    | :----: | :----: | :----: |
    | TextActor与Button | 2人/日 | 暂不支持点击态
    | ImageActor与ImageButton | 无 | 已实现 |
    | EditText | 无 | 与text actor相同 |
    | ScrollBarTrack | 2人/日 |
    | ScrollBar | 1人/日 | 比ScrollBarTrack多两个翻页按钮，本身是FrameLayout |
    | ListScrollBar | 无 | 与ScrollBar相同 |
    | FrameLayout | 无 | 已实现|
    | AbsoluteLayout | 无 | 已实现 |
    | LinearLayout | 10人/日 | 实现难度较大 |
    | TableLayout | 暂不实现 | 这个项目应该没人用 |
    | TableRow | 暂不实现 | 同TableLayout | 
    | RelativeLayout | 5人/日 | 属性较多 |
    | VerticalScrollLayout | 暂不实现 | 这个项目应该没人用 | 
    | AdapterActor | 4人/日 | 
    | ListActor | 15人/日 | 实现难度非常大 |

# 大众项目期待的其他功能
-   总工作量大概10人/日
    | 功能说明 | 工作量 | 其他说明 |
    | :----: | :----: | :----: |
    | 文言显示 | 无 | textactor实现 |
    | 像gimp工具，可以放大，可调透明度，拉线进行像素级对齐 | 5人/日 | 支持滚轮缩放功能 3人/日；支持透明度调节2人/日，拉线像素对齐暂时不知道咋做 |
    | 可以将xml和resource都加载到工具里 | 5人/日 | 优化xml和resource读取，支持大众项目目录结构。可能会影响加载性能
    | 这个工具有个限制，就像scrollview和superlist元器件，没办法进行第二页的像素级对齐 | 无 | 暂时不知道如何实现 |


## 元器件实现详细设计
- 1.TextActor与Button
    - 先解析font.xml:在UILib.cpp中写一个共通的解析方法xml,将xml内容解析成结构体列表。
    - 拿到font结构列表后保存在UILib中，结构体定义写在UIDef中，解析也在UILib初始化中调用
    - 同理解析string和color
    - 暂时只能单行显示文本，超过部分截断。
    - 需要frame暴露方法，计算文本宽度和高度
        - case 设置为单行文本：直接计算文字应显示宽度即可。
        - case 设置为多行文本：需要给一个矩形框，然后计算在这个矩形框中的显示大小：
            - 先计算单行文字宽度
                - 如果文言宽度没有超过矩形框宽度，宽度为计算值
                  在计算单行高度即可。
                - 如果文言宽度超过矩形框宽度，那么宽度 <= 矩形框宽度
                  然后计算这个宽度放n个字
                  然后计算总字符/n,向上取整y，然后h = y*单字符高度 + (y - 1)*行间距

- 2.ImageActor与ImageButton
    - 完成

- 3.ScrollBarTrack和ScrollBar
    - 自己写一个ScrollBarTrack(参考在LogR的实现吧)//完成，其实和imageActor没区别，drawable改成ThumbDrawable而已(因为暂时无需实现滚动块功能)，与imageActor独立是为了以后实现其具体功能
    - ScrollBar实现（CScrollBarDecTurnPage）
        - 从大众项目使用来看，一般有一个ScrollBar，里面放置一个layout的xml引用。所以ScrollBar应该是一个继承stage的空盒子，里面只有一个actor，即xml引用的layout
        - 需要新增一个xml解析方法，用来根据名字解析stage，解析后的stage返回。
        - 因为继承CVisualizationStage，所以需要先实现framelayout,需要先实现stage
    - 完成

- 4.将Gravity设置给CVisualizationActor，作为所有可视化Actor的基础属性，定义Actor描画内容的居中方式；对于stage来说，即是child actor的整体居中方式.
    需要添加新的插件UIGravityPlug
    - 完成

- 5.AbsoluteLayout实现
    - 完成

- 6.LinearLayout实现 11月中旬完成
    - 增加属性//完成
        - LayoutWeight
        - Orientation
        - WeightSum
        - HorizontalGravity 使用`CUIGravityPlug`
        - VerticalGravity 使用`CUIGravityPlug`
    - 实现measure和layout
        - OnMeasure 原理解析 1
        - OnMeasure 实现    0
        - OnLayout 原理解析 1
        - OnLayout 实现     0
    - 具体实现：
        - 新增actor插件：`CLinearLayoutPlug`；插件只有一个属性：`m_fWeight`，默认为0;并在actor中新增初始化、get和set方法
        - 在`CVisualizationActor`和`CVisualizationStage`中赋值该插件属性
        - 

- 7.RelativeLayout 实现  11月下旬完成
    - 新增actor插件：`CRelativeLayoutPlug`，属性如下，并在actor中新增初始化、get和set方法：
        //用于保存计算过程中的数据
        short m_sPreLeft;
        short m_sPreTop;
        short m_sPreRight;
        short m_sPreBottom;

        bool m_bAlignWithParent;
        uint32 m_aulRule[AlignCount];

        增加public枚举：EnumAlign

    - 由于插件属性中大量使用了id，所以需要新增元器件id，新增个插件吧：`CIDPlug`，id类型是std::string；并在actor中新增初始化、get和set方法

    - 在`CVisualizationActor`和`CVisualizationStage`中赋值`CRelativeLayoutPlug`插件属性和`CIDPlug`

    - 实现onmeasure

    - 实现onlayout 
    



- 8.AdapterActor实现 
- 9.list Actor实现
    list因为依赖adapter在代码中设置，所以无法在只有xml文件的情况下实现list显示。所以只需要显示默认的layout


# 测试阶段bug及修改方案

- 1.启动卡顿、修改xml后卡顿//纳期：cw49周中//完成，已经将加载和刷新时间控制在4秒内
A:尝试在修改xml后不重新加载其他资源，只重新加载form.xml,也就是说只修改属性值，不加载drawable等资源//这样不好，因为无法同步显示修改的资源路径了
  尝试使用配置文件，缩短递归检索文件的路径//没实现
  优化算法，在找到后直接返回，而不是继续寻找//完成

- 2..9图片没有使用九宫格拉伸//纳期：cw49周末//完成
A：添加新方法，在setdrawable时判断，如果资源是.9则使用成员记录；描画时调用canvas的.9资源描画方法;需要参数：纹理的原始宽高
    .9资源描画需要以九宫格对应纹理和模型位置。//使用GL_TRIANGLES，注意每个矩形由2个完整三角形构成
    所有的图片描画都需要判断，如果是.9使用给的可用rect，如果不是.9使用图片自己的rect

3.文言位置需要微调//纳期：cw50周中//完成
A：微调

4.布局位置不对//纳期：cw50周末//完成
A：需要确认问题布局并调整
    