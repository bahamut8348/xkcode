@Echo off 
Echo     A: 宏基  E: 戴尔     I: 神舟   M: LG    Q: TCL     U: 东芝4     Y: 七喜
Echo     B: 华硕  F: 方正     J: 惠普   N: NEC   R: 东芝1   V: 清华同方  Z: 技嘉
Echo     C: 明基  G: 富士通1  K: IBM    O: 三星  S: 东芝2   W: GATEWAY
Echo     D: 康柏  H: 富士通2  L: 联想   P: 索尼  T: 东芝3   X: 微星
Echo    ※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
Echo     1:导入Windows Server 2008 Standard    序列号:223PV-8KCX6-F9KJX-3W2R7-BB2FH
Echo     2:导入Windows Server 2008 Enterprise  序列号:26Y2H-YTJY6-CYD4F-DMB6V-KXFCQ
Echo     3:导入Windows Vista Home Basic        序列号:762HW-QD98X-TQVXJ-8RKRQ-RJC9V
Echo     4:导入Windows Vista Home Premium      序列号:8XPM9-7F9HD-4JJQP-TP64Y-RPFFV
Echo     5:导入Windows Vista Business          序列号:72PFD-BCBK8-R7X4H-6F2XJ-VVMP9
Echo     6:导入Windows Vista Ulimate           序列号:6F2D7-2PCG6-YQQTB-FWK9V-932CC
Echo     7:导入Windows 7 Ulimate               序列号:FJGCP-4DFJD-GJY49-VJBQ7-HYRR2
Echo     8:导入Windows 7 Professional          序列号:YKHFT-KW986-GK4PY-FDWYH-7TP9F
Echo     9:导入Windows 7 Home Premium          序列号:VQB3X-Q3KP8-WJ2H8-R6B6D-7QJB7
Echo    ※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
Echo                      按字母A-Z导入证书 请对应OEM品牌
Echo                       按数字1-9导入序列号 请对应版本
Echo                导入证书（序列号）后自动退出 再次运行导入序列号（证书） 
Echo      东芝1～4分别对应TOSQCI.xrm-ms、TOSCPL.xrm-ms、TOSHIB.xrm-ms、TOSINV.xrm-ms
Echo                       远景 BISO之家-----tengyue制作         
Echo    ※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
@echo off 
set /p x=选择OEM品牌及序列号,回车键确认:{A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,1,2,3,4,5,6,7,8,9} 
goto %x% 
:A 
echo 正在导入宏基OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\Acer.xrm-ms
rd /s /q %windir%\system32\OEM 
exit
:B 
echo 正在导入华硕OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\ASUS.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:C 
echo 正在导入明基认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\BenQ.xrm-ms
rd /s /q %windir%\system32\OEM 
exit
:D 
echo 正在导入康柏OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\COMPAQ.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:E 
echo 正在导入戴尔OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\DELL.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:F 
echo 正在导入方正OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\FOUNDER.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:G 
echo 正在导入富士通1 OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\FUJITSU1.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:H 
echo 正在导入富士通2 OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\FUJITSU2.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:I 
echo 正在导入神舟OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\HASEE.xrm-ms
rd /s /q %windir%\system32\OEM
exit 
:J 
echo 正在导入惠普OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\HP.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:K 
echo 正在导入IBM OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\IBM.xrm-ms
rd /s /q %windir%\system32\OEM
exit 
:L 
echo 正在导入联想OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\lenovo.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:M 
echo 正在导入LG OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\LG.xrm-ms
rd /s /q %windir%\system32\OEM 
exit
:N 
echo 正在导入NEC OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\NEC.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:O 
echo 正在导入三星OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\SAMSUNG.xrm-ms
rd /s /q %windir%\system32\OEM 
exit
:P 
echo 正在导入索尼OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\SONY.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:Q 
echo 正在导入TCL OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\TCL.xrm-ms
rd /s /q %windir%\system32\OEM
exit 
:R 
echo 正在导入东芝1 OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\TOSQCI.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:S 
echo 正在导入东芝2 OEM认证文件
slmgr.vbs -ilc %windir%\system32\OEM\TOSCPL.xrm-ms
rd /s /q %windir%\system32\OEM
exit 
:T 
echo 正在导入东芝3 OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\TOSHIB.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:U 
echo 正在导入东芝4 OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\TOSINV.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:V 
echo 正在导入清华同方OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\TONGFANG.XRM-MS
rd /s /q %windir%\system32\OEM
exit
:W 
echo 正在导入GATEWAY OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\GATEWAY.XRM-MS
rd /s /q %windir%\system32\OEM
exit
:X 
echo 正在导入微星OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\MSI.XRM-MS
rd /s /q %windir%\system32\OEM
exit
:Y 
echo 正在导入七喜OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\HEDY.XRM-MS
rd /s /q %windir%\system32\OEM
exit
:Z 
echo 正在导入技嘉OEM认证文件  
slmgr.vbs -ilc %windir%\system32\OEM\Gigabyte.XRM-MS
rd /s /q %windir%\system32\OEM
exit
:1
echo 正在导入Windows Server 2008标准版序列号
slmgr -ipk 223PV-8KCX6-F9KJX-3W2R7-BB2FH
exit
:2
echo 正在导入Windows Server 2008 企业版序列号
slmgr -ipk 26Y2H-YTJY6-CYD4F-DMB6V-KXFCQ
exit
:3 
echo 正在导入Windows VistaHome Basic版序列号
slmgr -ipk 762HW-QD98X-TQVXJ-8RKRQ-RJC9V
exit 
:4 
echo 正在导入Windows Vista Home Premium版序列号
slmgr -ipk 8XPM9-7F9HD-4JJQP-TP64Y-RPFFV
exit
:5
echo 正在导入Windows Vista Business版序列号
slmgr -ipk 72PFD-BCBK8-R7X4H-6F2XJ-VVMP9
exit 
:6 
echo 正在导入Windiws Vista Ulimate版序列号
slmgr -ipk 6F2D7-2PCG6-YQQTB-FWK9V-932CC
exit 
:7
echo 正在导入Windiws 7 Ulimate版序列号
slmgr -ipk FJGCP-4DFJD-GJY49-VJBQ7-HYRR2
exit
:8
echo 正在导入Windows 7 Professional版序列号
slmgr -ipk YKHFT-KW986-GK4PY-FDWYH-7TP9F
exit
:9
echo 正在导入Windows 7 Home Premium版序列号
slmgr -ipk VQB3X-Q3KP8-WJ2H8-R6B6D-7QJB7
exit
