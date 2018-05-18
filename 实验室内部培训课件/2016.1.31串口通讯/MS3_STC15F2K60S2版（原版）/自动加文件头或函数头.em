/*
********************************************************************************
* 文件名     :  main.c
* 描述       : 
 
* 版本     作者            日期            说明
* V1.0    农华艺        2014/04/21       初始版本
********************************************************************************
*/


/*******************************************************************************
* 函数名	: msOSInsertFileHeader
* 描述	    : 插入文件注释
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
macro msOSInsertFileHeader()
{
    // get aurthor name
    szMyName = getenv(MYNAME)
    if (strlen(szMyName) <= 0)
    {
        szMyName = "农华艺"
    }

    // get company name
    szCompanyName = getenv(MYCOMPANY)
    if (strlen(szCompanyName) <= 0)
    {
        szCompanyName = szMyName
    }

    // get time
    szTime = GetSysTime(True)
    Day = szTime.Day
    Month = szTime.Month
    Year = szTime.Year
    if (Day < 10)
    {
        szDay = "0@Day@"
    }
    else
    {
        szDay = Day
    }
    if (Month < 10)
    {
        szMonth = "0@Month@"
    }
    else
    {
        szMonth = Month
    }

    // get file name
    hbuf = GetCurrentBuf()
    szpathName = GetBufName(hbuf)
    szfileName = GetFileName(szpathName)
    nlength = StrLen(szfileName)

    // assemble the string
    hbuf = GetCurrentBuf()
    
    InsBufLine(hbuf, 0, "")
    InsBufLine(hbuf, 1, "/*******************************************************************************")
    InsBufLine(hbuf, 2, "* 文件名 : @szfileName@")
    InsBufLine(hbuf, 3, "")
    InsBufLine(hbuf, 4, "* 版本     作者            日期            说明")
    InsBufLine(hbuf, 5, "* V1.0    @szMyName@        @Year@/@szMonth@/@szDay@       初始版本")
    InsBufLine(hbuf, 6, "")
    InsBufLine(hbuf, 7, "* 描述   : MCU:      晶振:    MHz")
    InsBufLine(hbuf, 8, "           ")
    InsBufLine(hbuf, 9, "*******************************************************************************/")
    InsBufLine(hbuf,10, "")


    // put the insertion point
    SetBufIns(hbuf, 7, 16) //设置光标停留位置
}

/*******************************************************************************
* 函数名	: msOSInsertFunctionHeader
* 描述	    : 插入函数注释
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
macro msOSInsertFunctionHeader()
{
    AuthorName = "fast_sky\@sina.com"
    hwnd = GetCurrentWnd()
    lnFirst = GetWndSelLnFirst(hwnd)
    hbuf = GetCurrentBuf()//申请变量空间，Get a handle to the current file buffer and the name

    InsBufLine(hbuf, lnFirst+0, "")
    InsBufLine(hbuf, lnFirst+1, "/*******************************************************************************")
    InsBufLine(hbuf, lnFirst+2, "* 功能描述 : ")
    InsBufLine(hbuf, lnFirst+3, "* 函数属性 : ")
    InsBufLine(hbuf, lnFirst+4, "* 输入参数 : ")
    InsBufLine(hbuf, lnFirst+5, "* 返回参数 : ")
    InsBufLine(hbuf, lnFirst+6, "* 函数详解 : ")
    InsBufLine(hbuf, lnFirst+7, "      ")
    InsBufLine(hbuf, lnFirst+8, "*******************************************************************************/")
    InsBufLine(hbuf, lnFirst+9, "")

    SetBufIns(hbuf, lnFirst+10, 0) //设置光标停留位置

}


/*******************************************************************************
* 函数名	: msOSInsertHFileBanner
* 描述	    : 插入*.h头文件防重加载条件宏定义
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
macro msOSInsertHFileBanner()
{
    // get file name
    hbuf = GetCurrentBuf()
    szpathName = GetBufName(hbuf)
    szfileName = GetFileName(szpathName)
    szfileName = toupper(szfileName)

    // create banner
    banner = "__"
    nlength = strlen(szfileName)
	
    i=0
    while (i < nlength)
    {
        if (szfileName[i] == ".")
        {
            banner = cat(banner, "_")
        }
        else
        {
            banner = cat(banner, szfileName[i])
        }

        i = i+1
    }

    banner = cat(banner, "__")

    // print banner
    hwnd = GetCurrentWnd()
    lnFirst = GetWndSelLnFirst(hwnd)
    lnLast = GetWndSelLnLast(hwnd)
	 
    hbuf = GetCurrentBuf()
    InsBufLine(hbuf, lnFirst, "")
    InsBufLine(hbuf, lnFirst+1, "")
    InsBufLine(hbuf, lnFirst+2, "#ifndef @banner@")
    InsBufLine(hbuf, lnFirst+3, "#define @banner@")
    InsBufLine(hbuf, lnFirst+4, "")
    InsBufLine(hbuf, lnFirst+5, "")
    InsBufLine(hbuf, lnFirst+6, "")
    InsBufLine(hbuf, lnFirst+7, "")
    InsBufLine(hbuf, lnFirst+8, "")
    InsBufLine(hbuf, lnFirst+9, "#endif /*@banner@*/")
    InsBufLine(hbuf, lnFirst+10, "")

    SetBufIns(hbuf, lnFirst+11, 0)
}

/*==================================================================
* Function	: GetFileName
* Description	: get file name from path
*			  
* Input Para	: pathName	: path string
			  
* Output Para	: None
			  
* Return Value: name		: file name
==================================================================*/

macro GetFileName(pathName)
{
    nlength = strlen(pathName)
    i = nlength - 1
    name = ""
    while (i + 1)
    {
        ch = pathName[i]
        if ("\\" == "@ch@")
            break
        i = i - 1
    }
    i = i + 1
    while (i < nlength)
    {
        name = cat(name, pathName[i])
        i = i + 1
    }

    return name
}
/*******************************************************************************
* 函数名	: MultiLineComment
* 描述	    : 快捷键进行注释的宏定义，名为:MultiLineComment
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
macro msOSMultiLineComment()
{
    hwnd = GetCurrentWnd()
    selection = GetWndSel(hwnd)
    LnFirst = GetWndSelLnFirst(hwnd)      //取首行行号
    LnLast = GetWndSelLnLast(hwnd)      //取末行行号
    hbuf = GetCurrentBuf()
  
    if(GetBufLine(hbuf, 0) == "//magic-number:tph85666031"){
        stop
    }
  
    Ln = Lnfirst
    buf = GetBufLine(hbuf, Ln)
    len = strlen(buf)
  
    while(Ln <= Lnlast) {
        buf = GetBufLine(hbuf, Ln)  //取Ln对应的行
        if(buf == ""){                    //跳过空行
            Ln = Ln + 1
            continue
        }
  
        if(StrMid(buf, 0, 1) == "/") {       //需要取消注释,防止只有单字符的行
            if(StrMid(buf, 1, 2) == "/"){
                PutBufLine(hbuf, Ln, StrMid(buf, 2, Strlen(buf)))
            }
        }
  
        if(StrMid(buf,0,1) != "/"){          //需要添加注释
            PutBufLine(hbuf, Ln, Cat("//", buf))
        }
        Ln = Ln + 1
    }
  
    SetWndSel(hwnd, selection)
}
