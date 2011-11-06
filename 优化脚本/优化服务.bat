@echo off
echo 服务优化正在处理，请稍等......
@echo
@pause
sc config   ALG start= DEMAND
sc config   AppMgmt start= DEMAND
sc config   AudioEndpointBuilder start= AUTO
sc config   Audiosrv start= AUTO
sc config   BFE start= AUTO
sc config   BITS start= AUTO
sc config   Browser start= AUTO
sc config   CertPropSvc start= DEMAND
sc config   clr_optimization_v2.0.50727_32 start= DEMAND
sc config   COMSysApp start= DEMAND
sc config   CryptSvc start= AUTO
sc config   DcomLaunch start= AUTO
sc config   Dhcp start= AUTO
sc config   Dnscache start= AUTO
sc config   dot3svc start= DEMAND
sc config   EapHost start= DEMAND
sc config   EventSystem start= AUTO
sc config   fdPHost start= DEMAND
sc config   FDResPub start= DEMAND
sc config   FontCache3.0.0.0 start= DEMAND
sc config   gpsvc start= AUTO
sc config   hidserv start= DEMAND
sc config   hkmsvc start= DEMAND
sc config   idsvc start= DEMAND
sc config   IKEEXT start= AUTO
sc config   IPBusEnum start= DEMAND
sc config   KeyIso start= DEMAND
sc config   LanmanServer start= AUTO
sc config   LanmanWorkstation start= AUTO
sc config   lltdsvc start= DEMAND
sc config   lmhosts start= AUTO
sc config   MMCSS start= AUTO
sc config   MpsSvc start= AUTO
sc config   msiserver start= DEMAND
sc config   napagent start= DEMAND
sc config   Netlogon start= DEMAND
sc config   Netman start= DEMAND
sc config   netprofm start= AUTO
sc config   NetTcpPortSharing start= DISABLED
sc config   NlaSvc start= AUTO
sc config   NOD32krn start= AUTO
sc config   nsi start= AUTO
sc config   PlugPlay start= AUTO
sc config   ProfSvc start= AUTO
sc config   RasAuto start= DEMAND
sc config   RasMan start= DEMAND
sc config   RemoteAccess start= DISABLED
sc config   RpcLocator start= DEMAND
sc config   RpcSs start= AUTO
sc config   SamSs start= AUTO
sc config   SCardSvr start= DEMAND
sc config   SCPolicySvc start= DEMAND
sc config   SENS start= AUTO
sc config   SharedAccess start= DISABLED
sc config   ShellHWDetection start= AUTO
sc config   slsvc start= AUTO
sc config   SLUINotify start= DEMAND
sc config   SNMPTRAP start= DEMAND
sc config   SSDPSRV start= DEMAND
sc config   SstpSvc start= DEMAND
sc config   SysMain start= AUTO
sc config   TapiSrv start= DEMAND
sc config   TBS start= AUTO
sc config   Themes start= AUTO
sc config   THREADORDER start= DEMAND
sc config   TrustedInstaller start= DEMAND
sc config   upnphost start= DEMAND
sc config   UxSms start= AUTO
sc config   vds start= DEMAND
sc config   W32Time start= AUTO
sc config   wcncsvc start= DEMAND
sc config   WcsPlugInService start= DEMAND
sc config   WinHttpAutoProxySvc start= DEMAND
sc config   Winmgmt start= AUTO
sc config   Wlansvc start= DEMAND
sc config   wmiApSrv start= DEMAND
sc config   WMPNetworkSvc start= DEMAND
sc config   wscsvc start= AUTO
sc config   wuauserv start= AUTO
sc config   wudfsvc start= DEMAND
@pause
echo.
echo.&echo        优化完成!任意键退出
echo. & pause>nul