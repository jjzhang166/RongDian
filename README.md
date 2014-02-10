##熔点工具箱简介
> 主要为web程序员提供开发过程中所需要的小工具。

##项目基本结构：

RongDian RDTools项目二进制、资源、日志、语言、截屏、编码生成等文件存放目录
bug 问题记录的文件夹
common    自定义功能类的文件夹
docs	工程的文档文件夹
DuiMsg	基于duilib界面库的消息框静态链接库，用户代替系统MessgeBox的调用
inc	RDTools项目公共定义，定义公共数据、接口，用于个子模块的交互
lib	第三方库文件(只有二进制文件，无工程编译生成)、其它而相关文件，此文件夹内文件会在编译时拷贝到RongDian\Bin目录下
	
PCREText	PCRE正则匹配第三方库测试模块
RDCoder(预留工程)	熔点常见编码模块(提供MD5、URL、BASE64等编码接口)
RDFinder	文件查找模块(静态链接库，对特定目录提供文件名、文件内容、时间、大小等查找)
RDManager(预留工程)	插件管理模块
RDTools	熔点工具箱，RDTools项目主项目
SnapShot	截屏取色模块
RongDian.sln	RDTools项目管理文件，加载RDTools各相关子项目
	
thirdparty	第三方类库
RongDian	工程生成文件的目标文件夹
RongDian\bin	个子模块编译生成二进制文件存放目录，重新编译时，可以将此文件夹删除
RongDian\coder	编码模块转换后生成的编码文件的存放目录，机备份文件存放目录
RongDian\img	截屏模块图片生成目录
RongDian\lang	语言文件存放目录
RongDian\log	日志文件存放目录
RongDian\res	资源文件存放目录(界面布局的xml文件、界面图片等)
test	测试工程的文件夹

##目前主要功能
> 1、取色器
> 2、文件编码转换
> 3、代码格式化
> 4、ip地址设置
> 5、hosts文件设置

#版权
> GPL