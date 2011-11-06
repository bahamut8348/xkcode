regsvr32 /u /s igfxpph.dll
reg delete HKEY_CLASSES_ROOT\Directory\Background\shellex\ContextMenuHandlers /f
reg add HKEY_CLASSES_ROOT\Directory\Background\shellex\ContextMenuHandlers\new /ve /d {D969A300-E7FF-11d0-A93B-00A0C90F2719}
; 说明：Intel集成显卡（如810，815E，845G，865G等），会
; 在桌面右键生成多余的菜单，没有作用不要紧，关键是这些
; 菜单会使右键弹出变得缓慢。给人一种电脑速度慢的感觉。
; 这是清除右键多余菜单的办法。
; 南昌市上门电脑维修与导购
; 网站：http://www.80791.com
; 联系电话：0791-2900570
; 公司电话：0791-5208226
; 传真 0791-5208226
; 公司地址：南昌市建设路223号
; 联 系 人： 范先生
; 邮箱：fan1chun@qq.com
; Q  Q：99468739 
; 一切收费30-50元(可开发票）
; 免费解答电脑技术问题  冰锋电脑科技

