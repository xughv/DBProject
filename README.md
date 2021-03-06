# DBProject

### 编译
我们已经在根目录下编写好了`Makefile`文件，所以你可以在根目录下简单地使用`make`指令编译此程序。
**注意：** 程序默认使用的编译环境是`g++ -std=c++11`，可在Linux，OS X系统上编译并运行。

### 使用方法
| 参数 | 类型 | 描述 |必须|
| ------ | :----: | ------ | ----- |
| -n | integer | 数据的数量 | 是 |
| -d | integer | 数据的维度 | 是 |
| -qn | integer | 查询的数量 | 是 |
| -ds | string | 数据集文件的路径 | 是 |
| -qs | string | 查询集文件的路径 | 是 |
| -of | string | 输出结果文件夹路径 | 否 |


**Option 0:** 
结果输出到默认目录（当前目录下的results文件夹）

`./medrank -n 60000 -d 784 -qn 100 -ds ./data/Mnist.ds -qs ./data/Mnist.q`

**Option 1:** 
结果输出到自定义目录（当前目录下的output文件夹）

`./medrank -n 60000 -d 784 -qn 100 -ds ./data/Mnist.ds -qs ./data/Mnist.q -of output/`
### 数据的格式
数据集 与 查询集

| [偏移] | [类型] | [值] | [描述]|
| ------ | :----: | :----: | ------ |
| 0000 | 32 bit integer | 0x00000803(2051) | magic number |
| 0004 | 32 bit integer | 60000 | number of images |
| 0008 | 32 bit integer | 28 | number of rows |
| 0012 | 32 bit integer | 28 | number of columns |
| 0016 | unsigned byte | ?? | pixel |
| 0017 | unsigned byte  | ?? | pixel |
| .... |               |    |       |
| xxxx | unsigned byte | ?? | pixel |

### 输出结果
如果有输入输出文件夹-of的参数，则文件夹`index`与一个输出文件`result.out`在指定文件夹下。
如果无输入-of参数，则文件夹`index`与文件`result.out`在根目录的results文件夹下
- `index`目录下每一个文件就是一棵树（二进制形式保存）
- `result.out`用字符形式保存，里前四行分别是
  - Indexing time：建立索引所需时间
  - Runing time：一个查询所需时间
  - IO cost：IO操作的次数
  - Overall Ratio：误差（越接近1越准确）
- `result.out`后面的行数表示对应查询用MEDRANK算法得到的数据集里的id
