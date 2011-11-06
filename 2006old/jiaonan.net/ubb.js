var bf = 0; //设值为1禁止播放mp、rm、swf，0自动播放


if (typeof(NotBf) != 'undefined' )
    bf=1;
    
function ubb(id)
{
	if (document.getElementById(id))
	{
	document.getElementById(id).innerHTML = ubb2html(document.getElementById(id).innerHTML);
	}
}
function vipubb(id)
{
	if (document.getElementById(id))
	{
	document.getElementById(id).innerHTML = txt2html(document.getElementById(id).innerHTML);
	}
}
function jstubb(id)
{
	if (document.getElementById(id))
	{
	document.getElementById(id).innerHTML = jst2html(document.getElementById(id).innerHTML);
	}
}

function ubb2html(str)
{
	//str = str.replace(/\n/g, "");
	str=str.replace(/(<br>){3,}/gim,"<br><br>");  //清除多余的空行
	str = str.replace(/\[(\/?(u|b|i))\]/gim,"<$1>");//[u][b][i]
	str = str.replace(/\[img\](.+?)\[\/img\]/gi,function($0,$1){return("<a href=\""+$1+"\" target=_blank><img src=\"" + $1 + "\" border=0 onmouseover=\"javascript:if(this.width>screen.width-255)this.width=screen.width-255;\" onload=\"if(this.width>screen.width-255){this.width=screen.width-255;}\" onerror=\"this.src='/image/errimg.gif'\" ><\/a>")});//[img]
	str = str.replace(/\[em(.[^\[]*)\]/gi,function($0,$1){return("<img src=/bbs/em/"+$1+".gif border=0 align=middle>")});
	str = str.replace(/\[url=(.+?)\](.+?)\[\/url\]/gi,function($0,$1,$2){return("<a href=\""+$1+"\" target=_blank>"+$2+"</a>")});//[url=*]
	str = str.replace(/\[url\](.+?)\[\/url\]/gi,function($0,$1){return("<a href=\""+$1+"\" target=_blank>"+$1+"</a>")});//[url]
	//str = str.replace(/(\[color=(.[^\[]*)\])(.[^\[]*)(\[\/color\])/gim,function($0,$1,$2,$3){return("<font color=" + $2 + ">" + $3 + "</font>")});//[font=red][/font]
	str = str.replace(/\[color=([#0-9a-z]{1,7})\]/gim,"<font color=\"$1\">");
	str = str.replace(/\[\/color\]/gim,"</font>");
	str = str.replace(/\[quote\](.+?)\[\/quote\]/,function($0,$1){return("<div style=\"border: 1px dashed #CCCCCC;width: 96%; color: #999999;font-size: 12px;background-color: #f9f9f9;padding:5px;\">" + $1 + "</div>")});//[quote]
	str = str.replace(/\[flash\](.+?)\[\/flash\]/i,function($0,$1){if (bf<1){return("<img src=/edit/ubb/image/swf.gif border=0><a href=\""+$1+"\" target=_blank>"+$1+"</a><br><embed src=\""+$1+"\" width=500 height=400 border=0>")}else{return("<img src=/edit/ubb/image/swf.gif border=0> " + shuziren_bf($1,500,400,1))}});//[flash]
	str = str.replace(/\[mp=([0-9]{1,4}),([0-9]{1,4})\](.+?)\[\/mp\]/i,function($0,$1,$2,$3){if (bf<1){return("<object align=middle classid=CLSID:22d6f312-b0f6-11d0-94ab-0080c74c7e95 class=OBJECT id=md" + bf + " width="+$1+" height="+$2+" ><param name=ShowStatusBar value=-1><param name=Filename value=\""+$3+"\"><embed type=application/x-oleobject codebase=http://activex.microsoft.com/activex/controls/mplayer/en/nsmp2inf.cab#Version=5,1,52,701 flename=mp src=\""+$3+"\" width="+$1+" height="+$2+"></embed><PARAM NAME=AUTOSTART VALUE=1></object>")}else{return("<img src=/edit/ubb/image/wma.gif border=0> " + shuziren_bf($3,$1,$2,2))}});//[mp=*]
	str = str.replace(/\[rm=([0-9]{1,4}),([0-9]{1,4})\](.+?)\[\/rm\]/i,function($0,$1,$2,$3){if (bf<1){return("<OBJECT classid=clsid:CFCDAA03-8BE4-11cf-B84B-0020AFBBCCFA class=OBJECT id=RAOCX width="+$1+" height="+$2+"><PARAM NAME=SRC VALUE=\""+$3+"\"><PARAM NAME=CONSOLE VALUE=\""+$3+"\"><PARAM NAME=CONTROLS VALUE=imagewindow><PARAM NAME=AUTOSTART VALUE=true></OBJECT><br><OBJECT classid=CLSID:CFCDAA03-8BE4-11CF-B84B-0020AFBBCCFA height=32 id=video2 width="+$1+"><PARAM NAME=SRC VALUE=\""+$3+"\"><PARAM NAME=AUTOSTART VALUE=0><PARAM NAME=CONTROLS VALUE=controlpanel><PARAM NAME=CONSOLE VALUE=\""+$3+"\"></OBJECT>")}else{return("<img src=/edit/ubb/image/real.gif border=0> " + shuziren_bf($3,$1,$2,3))}});//[rm=*]
	str = str.replace(/\[csound\](.+?)\[\/csound\]/,function($0,$1){return("<img src=/edit/ubb/image/mid.gif border=0><a href=\""+$1+"\" target=_blank>背景音乐:"+$1+"</a><bgsound src=\""+$1+"\" border=0 loop=-1><br>")});
	str = str.replace(/\[map=(.[^\[]*)\]/i,function($0,$1){return("<p><iframe id='dt' width='400' height='320' scrolling='no' frameborder='0' src='/edit/ubb/image/xs_map.htm?v="+$1+"' style='border:1px solid #CCCCCC'></iframe></p>")});
	str = str.replace(/\[whitepad\](.+?)\[\/whitepad\]/i,function($0,$1){Painter($1)});
	str = str.replace(/\[align=(left|center|right)\]/gim,"<div style=\"text-align:$1\">");
	str = str.replace(/\[\/align\]/gim,"</div>");
        //str = str.replace(/([0-9０-９]{8,11})([^\w|\.|-|\"|<|\/]|$)/gim,function($0,$1,$2){return("***"+$2)}); //屏蔽电话号码
	return str;
}

function txt2html(str) //vip会员签名转化
{
	str=str.replace(/(<br>){2,}/gim,"<br>");  //清除多余的空行
	str = str.replace(/\[img\](.+?)\[\/img\]/i,function($0,$1){return("<img src=\"" + $1 + "\" border=0 onload=\"javascript:if(this.width>480)this.width=480;if(this.height>300)this.style.width=300;\">")});//[img]只匹配第一个图片，g:全部匹配,i:忽略大小写,m:多行查找
	str = str.replace(/\[url=(.+?)\](.+?)\[\/url\]/gi,function($0,$1,$2){return("<a href=\""+$1+"\" target=_blank>"+$2+"</a>")});//[url=*]
	str = str.replace(/\[url\](.+?)\[\/url\]/gi,function($0,$1){return("<a href=\""+$1+"\" target=_blank>"+$1+"</a>")});//[url]
	return str;
}
function jst2html(str) //jst转化
{
	str = str.replace(/\[em(.[^\[]*)\]/gi,function($0,$1){return("<img src=/bbs/em/"+$1+".gif border=0 align=middle>")});
	str = str.replace(/\[img\](.+?)\[\/img\]/gim,function($0,$1){return("<img src=\"" + $1 + "\" border=0 onload=\"javascript:if(this.width>280)this.width=280;if(this.height>280)this.style.width=280;\">")});//[img]
	str = str.replace(/\[url=(.+?)\](.+?)\[\/url\]/gi,function($0,$1,$2){return("<a href=\""+$1+"\" target=_blank>"+$2+"</a>")});//[url=*]
	str = str.replace(/\[url\](.+?)\[\/url\]/gi,function($0,$1){return("<a href=\""+$1+"\" target=_blank>"+$1+"</a>")});//[url]
        //str = str.replace(/([0-9０-９]{8,11})([^\w|\.|-|\"|<|\/]|$)/gim,function($0,$1,$2){return("***"+$2)}); //屏蔽电话号码
	return str;
}


var urlid = 0;

function xsbf()
{
	for(var n=0;n<urlid;n++)
	{
		if(document.getElementById('urlid_'+n).style.display=='inline')document.getElementById('urlid_'+n).style.display='none';
	}
}
function shuziren_bf(u,k,g,lx)
{
document.write('<script src=/jst/image/szr.js></script>');
var t = "<span name=urlid_" + urlid + " id=urlid_" + urlid + " style=\"DISPLAY: none;filter:alpha(opacity=90);padding:6px;font-size:9pt;border: 1px solid #ccc;background: #ffc;POSITION: absolute;text-align:center;\"><font color=gray>" + u + "</font><br><font color=red>访问网址不能确定是否安全，是否继续访问？</font> <br><span style=\"cursor:hand\" onclick='openWindow(\"" + u + "\",\"在线播放器\"," + k + "," + g + "," + lx + ");this.parentNode.style.display=\"none\"'>√继续访问</span>&nbsp;&nbsp;<span style=\"cursor:hand\" onclick='this.parentNode.style.display=\"none\"'>×取消访问</span></span>";
urlid++;
return ("<span>" + t + "</span><a href='" + u + "' target='_blank' onclick='xsbf();urlbfts(this).childNodes[0].style.display=\"inline\";return false'>" +  u + "</a>");
}

function urlbfts(i)
{
var v = i.previousSibling;
while (v.nodeType == 3){v = v.previousSibling;}
return v;
}


var p_id=0;
function Painter(str){
document.write("<script src=\"/edit/ubb/spwhitepad/createShapes.js\"><\/script><p><input type=\"hidden\" id=\"value_spwhitepad_"+p_id+"\" value=\"[whitepad]"+str+"[\/whitepad]\"><iframe src='/edit/ubb/spwhitepad/show.htm' name='spwhitepad_"+p_id+"' frameborder='0' scrolling='no' style='width:400px;height:200px;margin:5px;border:1px dashed #CCCCCC;'></iframe></p>");
p_id++;
}