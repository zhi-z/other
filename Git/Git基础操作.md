# Git基本使用

Git也用一段时间了，之前主要是把代码上传到GitHub，方便随时获取代码。现在学了msOS之后，发现代码越来越多，觉得各种版本不好管理，在王总示范使用Git进行代码管理之后，感觉使用这个进行代码管理，比格提高了。所以打算使用Git管理代码。于是在之前总结的基础上，做个简单的教程，以分享学习的过程。本教程是在window上实现的，但是在Linux同样适用，Linux上直接在Terminal操作，更加方便。主要是对Git小白，大神可忽略。

### 1 创建版本库

当安装好Git以后，找到在Git文件文件夹下会有一个git-bash.exe程序，先打开git-bash.exe。

![img](https://raw.githubusercontent.com/zhi-z/other/master/Git/image/git_cre_1.png)

打开后git-bash.exe

![img](https://raw.githubusercontent.com/zhi-z/other/master/Git/image/git_cre_2.png)

在电脑上创建一个一个文件夹，这个文件夹用于git进行文件管理，比如我在电脑桌面创建一个文件夹（msOS），然后在git-bash输入cd ~/desktop/msOS进入要管理的文件夹。

![1532686059741](https://raw.githubusercontent.com/zhi-z/other/master/Git/image/assets/1532686059741.png)

为了方便进行管理，我们在Git中创建用户名(user.name)和用户email（user.email），在米宁行中输入如下命令进行创建：

```
git config --global user.name "姓名"
git config--global user.email "邮箱号码"
```

管理员身份创建好以后，可以用git config user.name 查看是否创建好。如下图所示。

![1532686738310](https://raw.githubusercontent.com/zhi-z/other/master/Git/image/assets/1532686738310.png) 

然后在文件中使用git init命令创建管理文件。创建完成后可以使用命令:ls –a查看生成的文件。

![1532686902530](https://raw.githubusercontent.com/zhi-z/other/master/Git/image/assets/1532686902530.png)

到这里就完成了版本库的创建，这时候可以把需要管理的项目代码放到该文件中进行管理。

### 2 本地提交 

接着上面的部分，在这里我把msOS代码放到文件中。需要对msOS代码进行管理。把文件放文件夹中以后，通过git status查看当前状态。状态的转换关系后面细讲。

![1532689903590](/home/datah/Desktop/GitHub/other/Git/image/1532689903590.png)

出现以上的结果，表明当前处于Untracked状态，没有被加入到版本库。

所以还需要通过如下两步把需要管理的代码进行提交。

- 添加到暂存区（Stage）

```
# 把要管理的修改的代码提交到暂存区（Stage）
git add .  
```

![1532690854145](/home/datah/Desktop/GitHub/other/Git/image/1532690854145.png)

这时候的转态为Stage状态。接着进行提交。

- 提交

```
git connit -m "第一次提交" # 提交到版本管理库
```

git commit命令，-m后面输入的是本次提交的说明，可以输入任意内容，当然最好是有意义的，这样你就能从历史记录里方便地找到改动记录。 

提交完成之后，通过git status查看状态，可得到如下结果，说明已经完成提交到本地的管理库。

![1532691853981](/home/datah/Desktop/GitHub/other/Git/image/1532691853981.png)

### 3 三个状态

在使用git的过程中重点还是要了解文件所在的状态，可以通过转态确认修改是否提交成功。大致分为三个状态，如图所示。

![](https://raw.githubusercontent.com/zhi-z/other/master/Git/image/git_status.png)



#### 3.1 状态说明

 (1)  Untracked状态：是git不知道有什么文件，需要add才可以，add之后变成Staged状态这时候git就知道有这个文件，可以对这个文件进行版本管理；

(2) staged状态：任何一个文件修改后到这个状态都能被提交（commit），提交之后就变成unstage；

(3) unstaged状态：unmodified和modified都是unstaged状态。Unmodified表示所有的修改都被提交了，此时不用再提交；当修改我们创建的内容，文件就从unmodified状态变成modified状态。

#### 3.2  状态转换

1）	三个状态转换流程：Untracked -> unstage -> stage  -> unstage....
2）	更细致流程：unstracked ->unstage(add) -> stage(commit) -> unmodified(修改文件) -> Modefied(add) ->stage(commit) -> unstage .........

### 4 版本穿梭

当多次修改并提交以后，有后时候需要回到以前的版本。例如，在这里我修改了三次，提交了三次。

第二次提交在代码中进行了修改，如图所示，第一次在这个函数中是空的。

![1532693171153](/home/datah/Desktop/GitHub/other/Git/image/1532693171153.png)

第三次提交在该函数中也添加了一句，如图所示。

![1532693469802](/home/datah/Desktop/GitHub/other/Git/image/1532693469802.png)

之后通过git log --oneline命令查看有几次被提交了，如图所示

![1532693451599](/home/datah/Desktop/GitHub/other/Git/image/1532693451599.png)

现在代码中是处于第三次提交的状态，当想回到第一次提交时的版本的时候，如下命令。

```
git reset --hard f121198 # 第二次提交的地址
```

这时候，在代码中又回到了第二次提交时候的版本了，对于其他版本的转换也可以先通过git reflog:查看每一层指针指向的过程，可以查看过去commit版本的id，然后通过以上的命令回到指定的版本。完成版本的转换。

版本转换常用的命令:

1. git log --oneline:查看有几次被提交了
2. 对于复制了一个文件以后：git commit --amend --no--edit (amend 表示在原来文件commit的基础上进行提交，no--edit在原来commit上不做修改）"--no-edit": 不编辑, 直接合并到上一个 commit
3. 从staged状态返回到unstaged(modified)转态：git reset 1.py
4. git reset--hard HEAD：把指针移动到最后commit的版本
5. git reset--hard HEAD^:有到少个^指针就往后移动多少个
6. git reset--hard HEAD~10:回到前10个
7. git reset--hard 指针（这里可以使用地址号如：49028cc，或者：HEAD@{8}地址的位置）：回到该地址上
8. git reflog:查看每一层指针指向的过程，可以查看过去commit版本的id；

### 5 上传GitHub

有时候在本地会容易丢失，这时候可以提交到GitHub上。提交到GitHub上的过程也非常简单。

#### 5.1 创建SSH KEY

首先在在用户目录下的.ssh文件下是否有id_rsa和id_rsa.pub，如果有就跳过这步。如果没有的话可以通过如下命令生成。

```
ssh-keygen -t rsa -C "123456@qq.com"
```

接着登录GitHub，找到右下角的Settings，然后选中SSH and GPG keys，接着在右上角选中New SSH key的按钮。

![1532695456986](/home/datah/Desktop/GitHub/other/Git/image/assets/1532695456986.png)

接着把id_rsa.pub中的内容复制到可以框中，如图所示。

![1532695691006](/home/datah/Desktop/GitHub/other/Git/image/assets/1532695691006.png)

#### 5.2 上传

上传之前再GitHub上创建一个仓库，新创建的仓库都会有上传的命令，如图所示，但是只需要输入最后两行命令就可以上传。因为之前我们已经在本地commit过了。

![1532695928023](/home/datah/Desktop/GitHub/other/Git/image/assets/1532695928023.png)

```
git remote add origin git@github.com:BryanJahangir/msOS.git
git push -u origin master #对于-u只需要第一次才需要用，以后上传直接使用git push origin master就可以实现
```

结果

![1532696519891](/home/datah/Desktop/GitHub/other/Git/image/assets/1532696519891.png)

当出现以上的结果，表明上传成功，这时候在GitHub就可以找到了。在GitHub上可以看到我们上传的代码，如图所示。

![1532696652060](/home/datah/Desktop/GitHub/other/Git/image/assets/1532696652060.png)



# 命令小抄：

1. cd:进入目录
2. git configuser.name（user.email）:查看名称
3. touch 文件名：创建文件
4. git status:查看文件状态
5. git add 文件名：对修改的文件添加到staged状态
6. git commit -m 名字（随便取）：告诉Git，把文件提交到仓库
7. 一般步骤：先add修改，然co后commit提交

## 查看修改：

1. git log:查看修改记录
2. git status -s：查看状态
3. 在Modefied状态下使用命令 git diff :查看当前修改与之前完成commit时有什么不同
4. git add . :add所有的文件
5. 在stage状态：git diff --cached 查看当前状态与上一次commit时有什么不同
6. 当有两个状态例如：MM 2.py（前一个是绿色的M，后一个是红色的M）:git diff HEAD 查看他们两个状态与上一次commit有什么不同 

## 回到从前版本（在各种commit版本中往返）：

1. git log --oneline:查看有几次被提交了
2. 对于复制了一个文件以后：git commit --amend --no--edit (amend 表示在原来文件commit的基础上进行提交，no--edit在原来commit上不做修改）"--no-edit": 不编辑, 直接合并到上一个 commit
3. 从staged状态返回到unstaged(modified)转态：git reset 1.py
4. git reset--hard HEAD：把指针移动到最后commit的版本
5. git reset--hard HEAD^:有到少个^指针就往后移动多少个
6. git reset--hard HEAD~10:回到前10个
7. git reset--hard 指针（这里可以使用地址号如：49028cc，或者：HEAD@{8}地址的位置）：回到该地址上
8. git reflog:查看每一层指针指向的过程，可以查看过去commit版本的id；

## 针对单个文件之间的往返

1. git checkoutc6772a2(指针) -- 1.py(文件名);把哪个文件回到哪个commit中。再进行add，然后再进行commit；

## 分支

1. git log--oneline --graph：查看分支
2. git branchdev    # 建立 dev 分支
3. git branch        # 查看当前分支
4. git checkoutdev  # 这时 HEAD 已经被切换至 dev 分支
5. git branch -ddev:删除掉dev分支 
6. git checkout-b  dev：建立分支dev，并把指针移动到dev这个分支上
7. git commit -am"change 3 in dev"  #"-am": add 所有改变 并直接 commit
8. 创建分支并commit后切换到主分支：git merge --no-ff -m "keep mergeinfo" dev         # 保留 merge 信息，把dev分支上修改的同步到当前分支

## 上传代码到GitHub

1. git与GitHub连接：git remote add origin https://github.com/guyibang/TEST2.git

2. 第一次把代码提交送到GitHub上要有-u : git push -u origin master

3. 后面提交代码的时候只需要： git push origin master

4. 当要重新上传到别的库，先执行这个命令，把上次上传的先弹出：git pull --rebase origin master

5. 错误提示：提示出错信息：fatal: remote origin already exists.

   则执行命令：git remote rm origin

   然后再重新连接要上传的库：git remote add origin git@github.com:zhi-z/AI.git

