@Echo off 
Echo     A: ���  E: ����     I: ����   M: LG    Q: TCL     U: ��֥4     Y: ��ϲ
Echo     B: ��˶  F: ����     J: ����   N: NEC   R: ��֥1   V: �廪ͬ��  Z: ����
Echo     C: ����  G: ��ʿͨ1  K: IBM    O: ����  S: ��֥2   W: GATEWAY
Echo     D: ����  H: ��ʿͨ2  L: ����   P: ����  T: ��֥3   X: ΢��
Echo    ����������������������������������������������������������������������������
Echo     1:����Windows Server 2008 Standard    ���к�:223PV-8KCX6-F9KJX-3W2R7-BB2FH
Echo     2:����Windows Server 2008 Enterprise  ���к�:26Y2H-YTJY6-CYD4F-DMB6V-KXFCQ
Echo     3:����Windows Vista Home Basic        ���к�:762HW-QD98X-TQVXJ-8RKRQ-RJC9V
Echo     4:����Windows Vista Home Premium      ���к�:8XPM9-7F9HD-4JJQP-TP64Y-RPFFV
Echo     5:����Windows Vista Business          ���к�:72PFD-BCBK8-R7X4H-6F2XJ-VVMP9
Echo     6:����Windows Vista Ulimate           ���к�:6F2D7-2PCG6-YQQTB-FWK9V-932CC
Echo     7:����Windows 7 Ulimate               ���к�:FJGCP-4DFJD-GJY49-VJBQ7-HYRR2
Echo     8:����Windows 7 Professional          ���к�:YKHFT-KW986-GK4PY-FDWYH-7TP9F
Echo     9:����Windows 7 Home Premium          ���к�:VQB3X-Q3KP8-WJ2H8-R6B6D-7QJB7
Echo    ����������������������������������������������������������������������������
Echo                      ����ĸA-Z����֤�� ���ӦOEMƷ��
Echo                       ������1-9�������к� ���Ӧ�汾
Echo                ����֤�飨���кţ����Զ��˳� �ٴ����е������кţ�֤�飩 
Echo      ��֥1��4�ֱ��ӦTOSQCI.xrm-ms��TOSCPL.xrm-ms��TOSHIB.xrm-ms��TOSINV.xrm-ms
Echo                       Զ�� BISO֮��-----tengyue����         
Echo    ����������������������������������������������������������������������������
@echo off 
set /p x=ѡ��OEMƷ�Ƽ����к�,�س���ȷ��:{A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,1,2,3,4,5,6,7,8,9} 
goto %x% 
:A 
echo ���ڵ�����OEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\Acer.xrm-ms
rd /s /q %windir%\system32\OEM 
exit
:B 
echo ���ڵ��뻪˶OEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\ASUS.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:C 
echo ���ڵ���������֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\BenQ.xrm-ms
rd /s /q %windir%\system32\OEM 
exit
:D 
echo ���ڵ��뿵��OEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\COMPAQ.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:E 
echo ���ڵ������OEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\DELL.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:F 
echo ���ڵ��뷽��OEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\FOUNDER.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:G 
echo ���ڵ��븻ʿͨ1 OEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\FUJITSU1.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:H 
echo ���ڵ��븻ʿͨ2 OEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\FUJITSU2.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:I 
echo ���ڵ�������OEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\HASEE.xrm-ms
rd /s /q %windir%\system32\OEM
exit 
:J 
echo ���ڵ������OEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\HP.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:K 
echo ���ڵ���IBM OEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\IBM.xrm-ms
rd /s /q %windir%\system32\OEM
exit 
:L 
echo ���ڵ�������OEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\lenovo.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:M 
echo ���ڵ���LG OEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\LG.xrm-ms
rd /s /q %windir%\system32\OEM 
exit
:N 
echo ���ڵ���NEC OEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\NEC.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:O 
echo ���ڵ�������OEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\SAMSUNG.xrm-ms
rd /s /q %windir%\system32\OEM 
exit
:P 
echo ���ڵ�������OEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\SONY.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:Q 
echo ���ڵ���TCL OEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\TCL.xrm-ms
rd /s /q %windir%\system32\OEM
exit 
:R 
echo ���ڵ��붫֥1 OEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\TOSQCI.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:S 
echo ���ڵ��붫֥2 OEM��֤�ļ�
slmgr.vbs -ilc %windir%\system32\OEM\TOSCPL.xrm-ms
rd /s /q %windir%\system32\OEM
exit 
:T 
echo ���ڵ��붫֥3 OEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\TOSHIB.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:U 
echo ���ڵ��붫֥4 OEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\TOSINV.xrm-ms
rd /s /q %windir%\system32\OEM
exit
:V 
echo ���ڵ����廪ͬ��OEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\TONGFANG.XRM-MS
rd /s /q %windir%\system32\OEM
exit
:W 
echo ���ڵ���GATEWAY OEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\GATEWAY.XRM-MS
rd /s /q %windir%\system32\OEM
exit
:X 
echo ���ڵ���΢��OEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\MSI.XRM-MS
rd /s /q %windir%\system32\OEM
exit
:Y 
echo ���ڵ�����ϲOEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\HEDY.XRM-MS
rd /s /q %windir%\system32\OEM
exit
:Z 
echo ���ڵ��뼼��OEM��֤�ļ�  
slmgr.vbs -ilc %windir%\system32\OEM\Gigabyte.XRM-MS
rd /s /q %windir%\system32\OEM
exit
:1
echo ���ڵ���Windows Server 2008��׼�����к�
slmgr -ipk 223PV-8KCX6-F9KJX-3W2R7-BB2FH
exit
:2
echo ���ڵ���Windows Server 2008 ��ҵ�����к�
slmgr -ipk 26Y2H-YTJY6-CYD4F-DMB6V-KXFCQ
exit
:3 
echo ���ڵ���Windows VistaHome Basic�����к�
slmgr -ipk 762HW-QD98X-TQVXJ-8RKRQ-RJC9V
exit 
:4 
echo ���ڵ���Windows Vista Home Premium�����к�
slmgr -ipk 8XPM9-7F9HD-4JJQP-TP64Y-RPFFV
exit
:5
echo ���ڵ���Windows Vista Business�����к�
slmgr -ipk 72PFD-BCBK8-R7X4H-6F2XJ-VVMP9
exit 
:6 
echo ���ڵ���Windiws Vista Ulimate�����к�
slmgr -ipk 6F2D7-2PCG6-YQQTB-FWK9V-932CC
exit 
:7
echo ���ڵ���Windiws 7 Ulimate�����к�
slmgr -ipk FJGCP-4DFJD-GJY49-VJBQ7-HYRR2
exit
:8
echo ���ڵ���Windows 7 Professional�����к�
slmgr -ipk YKHFT-KW986-GK4PY-FDWYH-7TP9F
exit
:9
echo ���ڵ���Windows 7 Home Premium�����к�
slmgr -ipk VQB3X-Q3KP8-WJ2H8-R6B6D-7QJB7
exit
