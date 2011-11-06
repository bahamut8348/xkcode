#! coding: utf-8

import os

body_t = """
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<html>
<head>
<title> {title} </title>
</head>
<style>
	body{ font-size:12px;}
	h3{font-family:Arial}
	.logo{background-image:url(images/logo_fill.jpg); background-position:0 -1px;}
	.testcase{ margin-left:30px; width:600px; float:left; border-bottom:1px solid {bordercolor}; border-right:1px solid {bordercolor}; border-left:1px solid {bordercolor};}
	.testcase .index{float:left; width:60px; text-align:right; border-left:0px solid {bordercolor}; height:18px; line-height:18px;}
	.testcase .casename{text-indent:10px; float:left; width: 300px; border-left:1px solid {bordercolor};height:18px; line-height:18px;}
	.testcase .caseimg{text-indent:10px; float:left; border-left:1px solid {bordercolor}; height:18px; line-height:18px;}
	.allcase{ float:left; width:599px; }
	.testitem{font-family:Arial; margin-top:4px; background-color: {titlebk};float:left;width:600px; border: 1px solid {bordercolor}; height:25px; font-size:14px; text-indent:20px; font-weight:bold; margin-left:30px; line-height:25px;}
	.reth{
		color:#555555; font-weight:bold; background-color:{titlebk2};height:18px; line-height:18px; border-top: 1px solid {bordercolor}; 
	}
	.itemtime{float:right;}
	.itemname{float:left;}
	.ret0{color:red;}
	.detailitem{float:right; width:539px; border-left:1px solid {bordercolor};border-top:1px solid {bordercolor};}
	.detailitem .itemname{ float:left; width:80px; text-indent: 20px; font-family:Arial}
	.detailitem .itemvalue{ float:right; width:430px;border-left:1px solid {bordercolor}; line-height:16px; text-indent:4px;}
	.detailitem .listname{ text-indent:4px; float:left; width:538px; background-color:{titlebk2}; height:20px; line-height:20px;}
	.detailitem .index{float:left; width:61px; border-left:0px; padding:0; border-right: 1px solid {bordercolor};}
	.detailinfo {width:599px; float:right; border-left: 0px solid {bordercolor}; }
	.casedetail {float:left; line-height:18px; height:18px; padding-top:3px; padding-bottom:3px; margin-left:10px; cursor:hand; }
	.caseimg{}
	.foot{ float:left; width:900px; ;}
	.testitemwrap{float:left; width:900px;}
</style>
<body>
<div class="logo"><img src="images/logo.jpg" /></div>
<h3>Server Test Report</h3>
<hr />
{body}
<div class="foot" ><img src="images/ps.jpg" /></div>
</body>
</html>
"""

testitem_t =""" <div class="testitem" > <a class="itemname">%s</a><span class="itemtime">%s</span> </div> 
"""

testcase_t = """ <div class="testcase ret%s" ><div class="index"> <span class="casedetail" onclick="this.parentNode.nextSibling.nextSibling.nextSibling.style.display=this.parentNode.nextSibling.nextSibling.nextSibling.style.display=this.parentNode.nextSibling.nextSibling.nextSibling.style.display=='none'?'inline':'none';this.firstChild.firstChild.src=this.firstChild.firstChild.src.search('images/_minus1.gif')>1?'images/_plus1.gif':'images/_minus1.gif';">%s</span> <span>%s</span> </div><div class="casename">%s</div><div class="caseimg">%s</div><div style="display:none;" class="detailinfo">
	<div class="detailitem" >
		<div class="listname">Expect</div>
	</div>
	%s
	
	<div class="detailitem" >
		<div class="listname">Return</div>
	</div>
	%s
	
	<div class="detailitem" >
		<div class="listname">Parameters</div>
	</div>
	%s
	
	
	
	</div>
</div>"""


def makelistitem(name, value):
	return '<div class="detailitem" ><div class="itemname">%s</div> <div class="itemvalue">%s</div> </div>' % (name, value)

import xml.etree.ElementTree as xml

def main():
	global body_t
	caseimg={}
	caseimg['0'] = '¡Á'
	caseimg['1'] = '¡Ì'
	F = open("report/report.html", "w")
	wr = F.write
	html=""
	tree = xml.ElementTree()
	tree.parse("report.xml")
	items = tree.getroot().findall("testitem")
	for e in items:
		html += "<div class='testitemwrap' >"
		testitem = testitem_t % (e.attrib['name'] , e.attrib['createtime'])
		html += testitem
		html += "<div class='allcase'>"
		html += "<div class='testcase reth' ><div class='index'>No.</div><div class='casename'>Item Name</div><div class='caseimg'>Result</div></div>"
		for case in e.findall('testcase'):
			html_expect=""
			html_expect = "".join( map(lambda x: makelistitem(x.text, x.attrib['desc']), case.findall("expect/code") ) )
			html_return = "".join( map(lambda x: makelistitem(x.text, x.attrib['desc']), case.findall("return/code") ) )
			html_params = "".join( map(lambda x: makelistitem(x.attrib['name'][0].upper()+x.attrib['name'][1:], x.text), case.findall("params/param") ) )
			testcase = testcase_t % (case.attrib['result'],  "<a><img src=images/_plus1.gif /></a>",  case.attrib['index'],case.attrib['name'][0].upper()+case.attrib['name'][1:], caseimg[case.attrib['result']], html_expect, html_return, html_params)
			html+= testcase
		html+="</div></div>"
			
	body_t = body_t.replace("{body}", html)
	body_t = body_t.replace("{title}", "Server Test Report")
	body_t=body_t.replace("{resultcolor}", "#dddddd")
	body_t=body_t.replace("{bordercolor}", "#a1a1a1")
	body_t=body_t.replace("{titlebk}", "#A2B5CD")
	body_t=body_t.replace("{titlebk2}", "#DCDCDC")
	wr(body_t)
	F.close()		
	
	

if __name__ == '__main__':
	main()


