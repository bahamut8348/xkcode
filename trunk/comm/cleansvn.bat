    @REM #########################################################  
    @REM  Name: �ݹ�ɾ��ָ����Ŀ¼����Ѵ��ļ�������ϣ��ִ�е��Ǹ�Ŀ¼  
    @REM  Desciption:   
    @REM  Author: amosryan  
    @REM  Date: 2010-11-01  
    @REM  Version: 1.0  
    @REM  Copyright: Up to you.  
    @REM #########################################################  
      
    @echo on  
    setlocal enabledelayedexpansion  
      
    @REM ��������ɾ����Ŀ¼  
    set WHAT_SHOULD_BE_DELETED=.svn  
      
    for /r . %%a in (.) do (  
      @if exist %%a\!WHAT_SHOULD_BE_DELETED! (  
      echo "ɾ��"%%a\!WHAT_SHOULD_BE_DELETED!   
      rd /s /q %%a\!WHAT_SHOULD_BE_DELETED!   
     )  
    )  
      
    pause  