// ==UserScript==
// @name 			XK Teset For Firefox
// @namespace		xkxkxk
// @author			xuke
// @developer		xuke
// @contributor		
// @description		帮你订票的小助手 :-)
// @match			http://1.1.1.10/
// @match			https://dynamic.12306.cn/otsweb/order/querySingleAction.do?method=init
// @match			https://dynamic.12306.cn/otsweb/main.jsp
// @match			https://dynamic.12306.cn/otsweb/passengerAction.do?method=initUsualPassenger12306
// @match			http://www.12306.cn/mormhweb/logFiles/error.html
// @match			http://dynamic.12306.cn/TrainQuery/TrainInfoByCity.jsp
// @require			http://lib.sinaapp.com/js/jquery/1.8.3/jquery.min.js
// @icon			http://www.12306.cn/mormhweb/images/favicon.ico
// @run-at			document-idle
// @version 		1.0
// @updateURL		
// @supportURL		http://www.fishlee.net/soft/44/
// @homepage		http://www.fishlee.net/soft/44/
// @contributionURL	https://me.alipay.com/imfish
// @contributionAmount	￥1.00
// ==/UserScript==

/*
$.get('/xks', function(d){
	alert(d)
})

.error(function(e){
		alert(e)
	})
*/
var mainurl = 'https://dynamic.12306.cn/otsweb/main.jsp'
var cfg = window.localStorage
var profile

if (location.href.indexOf('error.html')>0){
	var s = 4
	function refresh(){
		$('#err_bot').text(s+'秒后重定向到 '+mainurl)
		setTimeout(function(){
			if(s==0)
				location.href = mainurl
			else refresh()
		}, 1000)
		s -= 1
	}
	refresh()
}
else if(location.href.indexOf('method=init')>0){
	if($('#fromStationText').val()) initQueryPage()
}
else if(location.href.indexOf('main.jsp')>0){
	$('main').attr('height', '1000px')
}
else if(location.href.indexOf('method=initUsualPassenger12306')>0){
	console.log('!!!', 111111111111111)
}

function xTimeout(fun1, fun2, time){
	var t = time
	function refresh(){
		setTimeout(function(){
			if(t<=0){fun2(t); fun1();  }
			else refresh()
		}, 100)
		t = t - 100 ; fun2(t)
	}
	refresh()
}

function saveProfile(){
	saveValue('fromStationText', 'fromStationText', 'fromStation', 'toStationText', 'toStation', 'startdatepicker', 'startTime')
	profile.trainClass = $.map($('input[name=trainClassArr]'), function(e){ return $(e).prop('checked') })
	console.log(profile)
	cfg.setItem('default', JSON.stringify(profile, null, 2))
}
function saveValue(){
	for(var i=0;i<arguments.length;i++){
		profile[arguments[i]] = $('#'+arguments[i]).val()
	}
}

function loadValue(){
	for(var i=0;i<arguments.length;i++){
		 $('#'+arguments[i]).val(profile[arguments[i]])
	}
}

function initQueryPage(){
	$('body').prepend('<div id=top><span id=title>插件加载</span>\
		<button id=saveprofile >保存查询</button> \
	</div>')
	$('<label>间隔:</label><input id=querysleep value=6 size=1 type=text /><input type=checkbox id=autoquery >自动</input>').insertAfter('#submitQuery')
	$('.hdr tr:eq(2) td:gt(2)').each(function(e){
		$(this).attr('otext', $(this).text())
		if($.trim($(this).text()).length>2)
			$(this).text($(this).text().replace('座','').replace('高级软卧', '高软'))
	})
	$('.hdr tr:eq(2) td:gt(2)').append('<input type=checkbox id=sitcheck />')
	$('#submitQuery').css('color', 'yellow').css('font-size', '16px').css('font-weight', 'bold')

	$('#saveprofile').click(function(){
		saveProfile()
	})
	$('#autoquery').click(function(){
	
	})
	$('#submitQuery').bind('click', function(){
		var btn = $(this)
		if(!$('#autoquery').prop('checked')) return;
		xTimeout(function(){
			btn.html('')
			console.log('Done')
			if($('#autoquery').prop('checked')) btn.click()
		}, function(a){
			btn.html(a)
		},$('#querysleep').val()*1000 )
	});
	
	
	console.log(cfg['default'])
	profile = JSON.parse(cfg.default || '{}')
	
	loadValue('fromStationText', 'fromStationText', 'fromStation', 'toStationText', 'toStation', 'startdatepicker', 'startTime')
	$('input[name=trainClassArr]').each(function(i, e){
		$(e).prop('checked', profile.trainClass[i])
	})
	
	console.log('init plugin')

	$('.wc_titlewb').hide()
	$('#gridbox').css('overflow','')
	$('#gridbox').css('height','800px')
}

function hookAjaxDone(req, res){
	console.log('ajax res', req )
	
	function buyTicket(){
		var data={}
		$.post("/otsweb/order/querySingleAction.do?method=submutOrderRequest", data, function(d){
			console.log(d)
		})
/*		$.get("/otsweb/order/confirmPassengerAction.do?method=init", function(d){
			console.log(d)
		}) */
	}
	
	if(req.openURL.indexOf("querySingleAction.do?method=queryLeftTicket")>0){
		var bs = $(".btn130_2, .btn130")
		bs.css("width", "50px")
		bs.css("width", "50px")
		bs.text("预订")
		
		bs.parent().append('<button class=buyticket >购票</button>')
		$(".buyticket").click(buyTicket)
	}
}

function injectMe(){
	var OldXHR = window.XMLHttpRequest;
    var NewXHR = function() {
        var self = this;
        var actualXHR = new OldXHR();
        
        this.requestHeaders = ""
        this.requestBody = ""
        
        // emulate methods from regular XMLHttpRequest object
        this.open = function(a, b, c, d, e) {
                self.openMethod = a.toUpperCase()
                self.openURL = b
                if (self.onopen != null && typeof(self.onopen) == "function") { self.onopen(a,b,c,d,e) } 
                return actualXHR.open(a,b,c,d,e)
            }
        this.send = function(a) {
                if (self.onsend != null && typeof(this.onsend) == "function") { self.onsend(a) } 
                self.requestBody += a
                return actualXHR.send(a)
            }
        this.setRequestHeader = function(a, b) {
                if (self.onsetrequestheader != null && typeof(self.onsetrequestheader) == "function") { self.onsetrequestheader(a, b) } 
                self.requestHeaders += a + ":" + b + "\r\n"
                return actualXHR.setRequestHeader(a, b) 
            }
        this.getRequestHeader = function() {
            return actualXHR.getRequestHeader() 
        }
        this.getResponseHeader = function(a) { return actualXHR.getResponseHeader(a) }
        this.getAllResponseHeaders = function() { return actualXHR.getAllResponseHeaders() }
        this.abort = function() { return actualXHR.abort(); }
        this.addEventListener = function(a, b, c) { return actualXHR.addEventListener(a, b, c) }
        this.dispatchEvent = function(e) { return actualXHR.dispatchEvent(e) }
        this.openRequest = function(a, b, c, d, e) { return actualXHR.openRequest(a, b, c, d, e) }
        this.overrideMimeType = function(e) { return actualXHR.overrideMimeType(e) }
        this.removeEventListener = function(a, b, c) { return actualXHR.removeEventListener(a, b, c) }
        
        function copyState() {
            try {
                self.readyState = actualXHR.readyState
            } catch (e) {}
            try {
                self.status = actualXHR.status
            } catch (e) {}
            try {
                self.responseText = actualXHR.responseText
            } catch (e) {}
            try {
                self.statusText = actualXHR.statusText
            } catch (e) {}
            try {
                self.responseXML = actualXHR.responseXML
            } catch (e) {}
        }
        
        actualXHR.onreadystatechange = function() {
            copyState()
            
            try {
                if (self.onupdate != null && typeof(self.onupdate) == "function") { self.onupdate() } 
            } catch (e) {}
       
            if (self.onreadystatechange != null && typeof(self.onreadystatechange) == "function") { return self.onreadystatechange() } 
        }
        actualXHR.onerror = function(e) {
            copyState()

            try {
                if (self.onupdate != null && typeof(self.onupdate) == "function") { self.onupdate() } 
            } catch (e) {}

            if (self.onerror != null && typeof(self.onerror) == "function") { 
                return self.onerror(e)
            } else if (self.onreadystatechange != null && typeof(self.onreadystatechange) == "function") { 
                return self.onreadystatechange();
            }
        }
        actualXHR.onload = function(e){
            copyState()
			window.hookAjaxDone(self, this)
            try {
                if (self.onupdate != null && typeof(self.onupdate) == "function") { self.onupdate() } 
            } catch (e) {}

            if (self.onload != null && typeof(self.onload) == "function") {
				var r = self.onload(e)
				
                return r
            } else if (self.onreadystatechange != null && typeof(self.onreadystatechange) == "function") { 
                return self.onreadystatechange()
            }
        }
        actualXHR.onprogress = function(e) {
            copyState()

            try {
                if (self.onupdate != null && typeof(self.onupdate) == "function") { self.onupdate() } 
            } catch (e) {}

            if (self.onprogress != null && typeof(self.onprogress) == "function") { 
                return self.onprogress(e)
            } else if (self.onreadystatechange != null && typeof(self.onreadystatechange) == "function") { 
                return self.onreadystatechange()
            }
        }
    }
    window.XMLHttpRequest = NewXHR;
}
var onload = function() {
    unsafeWindow.eval(injectMe.toString());
	unsafeWindow.eval("window.hookAjaxDone = " + hookAjaxDone.toString());
    unsafeWindow.eval("injectMe();");
}

function test(){
	$(document).ajaxComplete(function(e, x, r){
		console.log(r)
	})
}

var onload2 = function(){
	unsafeWindow.eval(test.toString());
	unsafeWindow.eval("test();");
}

window.addEventListener("load", onload2, false);



