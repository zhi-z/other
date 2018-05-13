# Ubuntu18.04深度学习GPU环境配置

- Ubuntu 18.04、cuda 9.0、cuDnn v7、TensorFlow/Keras 与anaconda

## 1.背景

为了加速神经网络的训练，使用CPU训练速度很慢，所以使用cuda和cudnn对神经网络进行加速，在配置的过程中遇到了很多坑，大概配置了一两天，感觉是我配置环境以来最繁杂的一次了，各种坑，网上也有很多教程但是实现起来各种ERROR，真心觉得累。经过差不多两天的时间终于把环境配置好。

## 2.硬件与软件版本

显卡：GeForce GTX 1070ti

CPU：AMD Ryzen 7 2700x

系统：Ubuntu 18.04 

cuda：9.0版本

cuDNN： cuDnn v7

anaconda ：3.5版本

TensorFlow：1.8版本

Keras：默认安装最新版

## 3.英伟达显卡驱动安装

我使用的是ppa源安装的方式，因为之前也从网上从官网上下载run文件进行安装，但是但是后面出现崩溃，又得重新安装，所以这里只讲解ppa源的安装方式。

### 3.1 删除旧的驱动

原来Linux默认安装的显卡驱动不是英伟达的驱动，所以先把旧得驱动删除掉。

```
# 删除
sudo apt-get purge nvidia*
```

### 3.2 禁止自带的nouveau nvidia驱动

```
# 打开配置文件
sudo vim /etc/modprobe.d/blacklist-nouveau.conf
```

填写禁止配置的内容：

```
blacklist nouveau
options nouveau modeset=0
```

更新配置文件，

```
sudo update-initramfs -u
```

最后需要进行重启重启之后，因为禁止了显卡的驱动，所以重启后显示的效果很不好，通过这个可以看出，是否完成这一步操作。

（4）添加Graphic Drivers PPA 

```
sudo add-apt-repository ppa:graphics-drivers/ppa
sudo apt-get update
```

查看合适的驱动版本：

```
ubuntu-drivers devices
```

如图所示：

![](https://raw.githubusercontent.com/zhi-z/other/master/%E6%B7%B1%E5%BA%A6%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/001.png)

从图中可以看出可以选择的驱动版本，在这里我选择最高的版本396的。通过如下命令进行安装：安装 完后需要进行重启。

```
# 安装
sudo apt-get install nvidia-driver-396
# 重启
sudo reboot
```

![](https://raw.githubusercontent.com/zhi-z/other/master/%E6%B7%B1%E5%BA%A6%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/006.png)

安装完后，通过如下命令检测是否安装成功，如果成功会显示如下图所示。

```
sudo nvidia-smi
sudo nvidia-settings
```

![](https://raw.githubusercontent.com/zhi-z/other/master/%E6%B7%B1%E5%BA%A6%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/008.png)

![](https://raw.githubusercontent.com/zhi-z/other/master/%E6%B7%B1%E5%BA%A6%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/015.png)

完成显卡驱动的安装。

## 4.cuda 9.0 安装

### 4.1 下载文件

首先在[官网上下载run 文件](https://developer.nvidia.com/cuda-90-download-archive?target_os=Linux&target_arch=x86_64&target_distro=Ubuntu&target_version=1704&target_type=runfilelocal)，如图所示，选择合适run文件。

![](https://raw.githubusercontent.com/zhi-z/other/master/%E6%B7%B1%E5%BA%A6%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/cuda_d.png)

### 4.2 安装依赖库

在下载完成后在运行这个run文件之前先安装依赖库，这个是什么鬼库我也不懂，是看了[这个教程](https://blossomnoodles.github.io/cnBlogs/2018/04/30/Ubuntu18.04-Tensorlow-install.html)来做的。

```
# 依赖库安装
sudo apt-get install freeglut3-dev build-essential libx11-dev libxmu-dev libxi-dev libgl1-mesa-glx libglu1-mesa libglu1-mesa-dev
```

但是不做这个步骤直接运行run文件会出现如图所示。

![](https://raw.githubusercontent.com/zhi-z/other/master/%E6%B7%B1%E5%BA%A6%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/012.png)

所以安装依赖库是为了解决这个问题。

### 4.3 gcc降低版本

CUDA9.0要求GCC版本是5.x或者6.x，其他版本不可以，需要自己进行配置，通过以下命令才对gcc版本进行修改。

```
# 查看版本
g++ --version  
```

![](https://raw.githubusercontent.com/zhi-z/other/master/%E6%B7%B1%E5%BA%A6%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/004.png)

```
# 版本安装：
sudo apt-get install gcc-5
sudo apt-get install g++-5
```

![](https://raw.githubusercontent.com/zhi-z/other/master/%E6%B7%B1%E5%BA%A6%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/003.png)

通过命令替换掉之前的版本：

```
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 50
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-5 50
```

最后记得再次查看版本是否修改成功。

### 4.4 安装cuda

运行run文件。

```
sudo sh cuda_9.0.176_384.81_linux.run
```

注意：在安装过程中会提示是否需要安装显卡驱动，如图所示，在这里要选择n，其他的选择y或者回车键进行安装。

![](https://raw.githubusercontent.com/zhi-z/other/master/%E6%B7%B1%E5%BA%A6%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/014.png)

到最终的结果如果没有错误，得到的结果如图所示：

![](https://raw.githubusercontent.com/zhi-z/other/master/%E6%B7%B1%E5%BA%A6%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/009.png)

其他不正确的形式：

![](https://raw.githubusercontent.com/zhi-z/other/master/%E6%B7%B1%E5%BA%A6%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/013.png)

还有其他形式的，但是没有截屏，对于这种情况可能是某些依赖库没装好还是什么我也不太懂，但是我解决的方法是先安装一下依赖库。然后重启，结果就可以了。

```
sudo apt-get install libprotobuf-dev libleveldb-dev libsnappy-dev libopencv-dev libhdf5-serial-dev protobuf-compiler
sudo apt-get install --no-install-recommends libboost-all-dev
sudo apt-get install libopenblas-dev liblapack-dev libatlas-base-dev
sudo apt-get install libgflags-dev libgoogle-glog-dev liblmdb-dev
```

### 4.5 环境配置

完成以上的步骤以后一定要进行环境的配置。按步骤输入一下命令：

```
sudo gedit ~/.bashrc
```

会推出一个可写的配置文件，在末尾把以下配置写入并保存。

```
export PATH=/usr/local/cuda-9.0/bin${PATH:+:${PATH}}  
export LD_LIBRARY_PATH=/usr/local/cuda-9.0/lib64${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
```

最后执行

```
source ~/.bashrc
```

有时候也需要重启。最好重启一下。

### 4.6 测试

在安装的时候也也相应安装了一些cuda的一些例子，可以进入例子的文件夹然后使用make命令执行。从网上找了两个例子，例一：

```
# 第一步，进入例子文件
cd /usr/local/cuda-8.0/samples/1_Utilities/deviceQuery
# 第二步，执行make命令
sudo make
# 第三步
./deviceQuery
```

如果结果有GPU的信息，说明安装成功。

例二：

```
# 进入例子对应的文件夹
cd NVIDIA_CUDA-9.0_Samples/5_Simulations/fluidsGL
# 执行make
make clean && make
# 运行
./fluidsGL
```

当执行这个例子，我们会看到流动的图，刚开始可能看不到黑洞，需要等待一下时间。

![](https://raw.githubusercontent.com/zhi-z/other/master/%E6%B7%B1%E5%BA%A6%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/002.png)

当运行这个程序的时候我们可以通过如下命令查看GPU是否在使用：

```
nvidia-smi
```

运行以上程序的前后对比如图所示：

![](https://raw.githubusercontent.com/zhi-z/other/master/%E6%B7%B1%E5%BA%A6%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/011.png)

完成CUDA 9.0 的安装。

## 5.cuDnn v7 安装

### 5.1 下载

cuDNN安装的方式非常简单，[官网](http://developer.download.nvidia.com/compute/machine-learning/cudnn/secure/v7.0.5/prod/Doc/cuDNN-Installation-Guide.pdf?inpwliT3tZy3rDJScewprvbzrDtsJakJ6vRL6AjdU6EW1TdPD_dCcXbCVgHdjIenUa5Bpo3Q_gPcVhk3nfjq4efu2nYG4WiOCHb_7-JRutopY2Jy-A7UN7UQMnbvFyf9gZEBpIVAYagNJ-lDTg__LzSd1R-YjlHciQ3zqoPyBigBBRV4D5D9ZcbUXEOguqjtNw)的安装指南有两种方式进行安装。我随便选择了一种方式进行安装。在这个方式中首先需要下载合适的安装包，因为我的CUDA 是9.0的版本，所以选择如下图三个文件安装包：

![](https://raw.githubusercontent.com/zhi-z/other/master/%E6%B7%B1%E5%BA%A6%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/cudnn_1.png)

### 5.2 安装

安装过程如下：

![](https://raw.githubusercontent.com/zhi-z/other/master/%E6%B7%B1%E5%BA%A6%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/cudnn_2.png)

```
# 安装命令
sudo dpkg -i libcudnn7_7.0.3.11-1+cuda9.0_amd64.deb
sudo dpkg -i libcudnn7-dev_7.0.3.11-1+cuda9.0_amd64.deb
sudo dpkg -i libcudnn7-doc_7.0.3.11-1+cuda9.0_amd64.deb
```

### 5.3 测试

安装完以后需要进行测试是否安装成功，测试的过程如下图所示：

![](https://raw.githubusercontent.com/zhi-z/other/master/%E6%B7%B1%E5%BA%A6%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/cudnn_3.png)

```
# 如图中的命令
cp -r /usr/src/cudnn_samples_v7/ $HOME
cd $HOME/cudnn_samples_v7/mnistCUDNN
make clean && make
./mnistCUDNN
```

最终如果有提示信息：“Test passed! ”，则说明安装成功，如图所示：

![](https://raw.githubusercontent.com/zhi-z/other/master/%E6%B7%B1%E5%BA%A6%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/ac.png)

## 6.anaconda安装

对于anaconda3 的安装非常简单，从官网中直接下载3.5版本的sh文件。然后执行如下命令对conda进行安装，我下载的是Anaconda3-5.1.0-Linux-x86_64.sh，过程直接yes、yes安装即可，对于不懂的可以看[这个](https://blog.csdn.net/u012318074/article/details/77074665)更详细的教程。

```
bash Anaconda3-5.1.0-Linux-x86_64.sh
```

安装完成后要重启电脑才能打开jupyter notebook。重启之后在终端输入一下命令进入notebook：

```
jupyter notebook
```

打开notebook界面如下，是生成在浏览器中的.

![](https://raw.githubusercontent.com/zhi-z/other/master/%E6%B7%B1%E5%BA%A6%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/conda.png)

## 7.TensorFlow和Keras 安装

安装完anaconda 以后可以在终端直接用pip 对TensorFlow和Keras进行安装：

```
# 安装 gpu 版本的 tensorflow 和 keras
pip install tensorflow-gpu # 默认安装最新版本
pip install keras 
```

![](https://raw.githubusercontent.com/zhi-z/other/master/%E6%B7%B1%E5%BA%A6%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/tensorflow1.8.png)

![](https://raw.githubusercontent.com/zhi-z/other/master/%E6%B7%B1%E5%BA%A6%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/keras_install.png)

## 8.测试

对于测试的代码可以使用手写数字识别进行测试，在[GitHub这里](https://github.com/zhi-z/DeepLearning/blob/master/Convolutional_Neural_Networks/mnist_cnn.ipynb)可以找到。我用自己的项目跑了一下，是在jupyter notebook上运行的，使用的框架是Keras。对比使用CPU和使用GPU加速对神经网络进行训练的对比，如图所示：

CPU训练：

![](https://raw.githubusercontent.com/zhi-z/other/master/%E6%B7%B1%E5%BA%A6%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/CPU.png)

GPU加速训练：

![](https://raw.githubusercontent.com/zhi-z/other/master/%E6%B7%B1%E5%BA%A6%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/GPU_2.png)

通过以上的对比，明显发现使用GPU加速比单独使用cup对神经网络进行训练快了很多。

使用如下命令查看训练神经网络前和训练神经网络时GPU使用的情况：

```
nvidia-smi
```

![](https://raw.githubusercontent.com/zhi-z/other/master/%E6%B7%B1%E5%BA%A6%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/GPU_1.png)

通过比较前后，GPU的使用情况可以发现训练时调用的GPU，没有训练时GPU的利用率不到百分之十，图上的GPU利用率直接为0，训练时GPU的利用率直接飙升到92%，说明安装的过程没有问题。完成环境的配置。

## 9.总结

感觉经历了九九八十一难，最终终于把环境搭好，虽然搭建的过程遇到了很多坑，但是搭建完成之后，回过来一看感觉也没那么难，各种配置和需要依赖哪些库的问题，还有就是版本的匹配，以为需要安装多个软件，最难的就是版本的匹配问题，又因为Ubuntu18.04算是最新发布的，在安装cuda和cudnn中还没有匹配得，最高也是17.04的版本，但是在安装过程中发现Ubuntu 18.04的兼容性很好。最后要感谢一下网上各位大佬的分享。



## 参考文献：

1.https://blog.csdn.net/u012318074/article/details/77074665

2.https://blog.csdn.net/cyn618/article/details/79997960

3.https://blog.csdn.net/10km/article/details/61191230

4.https://blog.csdn.net/jasonzhangoo/article/details/54866049

5.https://blog.csdn.net/lengconglin/article/details/77506386

6.https://www.cnblogs.com/luruiyuan/p/6660142.html

7.https://blossomnoodles.github.io/cnBlogs/2018/04/30/Ubuntu18.04-Tensorlow-install.html



对于有错的或者问题欢迎留言，共同进步。