<?php  
  
  include "../functions.php";
  include "../connect.php";
  session_start();
  if(!isset($_SESSION['authorized']) || !$_SESSION['authorized']) {
    echo "Not authorized.";
    return;
  }
?>

<html>

<head>
<meta http-equiv="Content-Type"
content="text/html; charset=iso-8859-1">
<title> Web Posting Information </title>
<style type="text/css">
<!--
.style1 {font-weight: bold}
-->
</style>
</head>

<body>
<!-- modterms.html
<!-- 
	This file allows users to post files to their web with the Web Publishing Wizard or FrontPad, using the same username and password they would
	use if they were authoring with the FrontPage Explorer and Editor.

	The values below are automatically set by FrontPage at installation
	time.  Normally, you do not need to modify these values, but in case
	you do, the parameters are as follows:

	'BaseURL' is the URL for your web server.
	
	'DefaultPage' is the name of the default (home) page name 
	for your web.

	'XferType' specifies that the FrontPage server extensions have been
	installed on this web.  This value should not be changed.

	'FPShtmlScriptUrl', 'FPAuthorScriptUrl', and 'FPAdminScriptUrl' specify
	the relative urls for the scripts that FrontPage uses for remote
	authoring.  These values should not be changed.

	'version' identifies the version of the format of this file, and
	should not be changed.
--><!-- WebPost 
    version="0.100"
    BaseUrl="http://www.drag-central.com"
    XferType="FrontPage"
    DefaultPage="index.htm"
    FPShtmlScriptUrl="_vti_bin/shtml.exe/_vti_rpc"
    FPAuthorScriptUrl="_vti_bin/_vti_aut/author.exe"
    FPAdminScriptUrl="_vti_bin/_vti_adm/admin.exe"
-->
<p align="center" class="style1">    Moderator Terms/Guidelines</p>
<p align="center"><strong>Welcome to your moderator control panel. Before proceeding review<br>
  this page in full at all times as changes can be made at any given time&hellip;</strong></p>
<p align="center"><strong>---Before banning actions---</strong></p>
<p align="center"><strong>*Always warn before banning.</strong></p>
<p align="center"><strong> *Never threaten the user so that they get defensive.</strong></p>
<p align="center"><strong>*Try to reason with the person.</strong></p>
<p align="center"><strong>*Banning should be last resort.</strong></p>
<p align="center"><strong>*Show respect to everyone.</strong></p>
<p align="center"><strong>*Never degrade the person who gets banned.</strong></p>
<p align="center"><strong>*Don&rsquo;t argue with the person.</strong></p>
<p align="center"><strong>*Be professional about the matter no mater the issue.</strong></p>
<p align="center"><strong>Note: You may put different reasons BUT they must be professional, and to the point&hellip;</strong></p>
<p align="center"><strong>---Warning Procedures---</strong></p>
<p align="center"><strong>First Offense: Give a warning to the user for his/her actions.</strong></p>
<p align="center"><strong>Second Offense: User is kicked/banned from the chat room. <br>
</strong></p>
<p align="center"><strong>Please Copy and Paste these into your Reason for which Rule they broke. If there is no rule for what the have done please use your best judgement possible to decide which is the best to handle the situation.<br>
</strong></p>
<p align="center"><strong>(Advertising) You may not reproduce, transmit, sell, or use Drag Central for commercial purposes, including the promotion, advertising or solicitation of funds or goods, or the solicitation of anyone to join or become a user of any commercial online web site or other organization. This Rule is also intended use with (Flooding) Rule 4. --- Warn them one time, Ban for 2 days.</strong></p>
<p align="center"><strong>(Inappropriate Username) You may not create a user name or screen name through Drag-Central that is indecent, libelous, defamatory, obscene, threatening, invasive of privacy or publicity rights, abusive, illegal, harassing, contain expressions of hatred, bigotry, racism, or pornography, are otherwise objectionable, that would constitute or encourage a criminal offense, violate the rights of any party, or violate any Terms Of Use or Privacy Policy. --- Account ban 7 days, ask them to change there username.</strong></p>
<p align="center"><strong>(Hacking) You are prohibited from uploading to, distributing, or publishing through Drag-Central any Materials that contain viruses or any other computer code, corrupt files, or programs designed to interrupt, destroy, or limit the functionality or disrupt any software, hardware, telecommunications, networks, servers or other equipment used by Drag-Central. --- 7 Day ban no warning needed, please contact a Admin for Ip ban or to do further steps.</strong></p>
<p align="center"><strong>(Flooding) You may not intentionally disrupt other Users or discussions by repeatedly posting the same message or excessive post within chat rooms. We may set procedures on the use of the chat such as setting the maximum message submited in a certain amount of time. We may change these procedures at any time without notice to you. --- Warn once, then if countines 12 hour Ban. If happen with the same person (Ip Address) Double the Ban time.</strong></p>
<p align="center"><strong>(Impersonation) You may not impersonate another person or User; attempt to get a password, other account information, or other private information from a User, or harvest email addresses or other information. --- 3 day Ban. No warning needed.</strong></p>
<p align="center"><strong>(Multiple Login) You may not login from more then one account at a time. Intentionally having two accounts logged into Drag-Central for your personal gain is against the rules. Intent to make people belive your someone your not is Impersonation and also is against Rule 5. ---Warn 1 time if they do not log from one of the accounts ban both for 12 hours. Happens more then once with the same person (Ip address) Double the ban time.</strong></p>
<p align="center"><strong>---Automatic Demod---</strong></p>
<p align="center"><strong>1. Flame wars.</strong></p>
<p align="center"><strong>2. Use foul, vulgar, or obscene language.</strong></p>
<p align="center"><strong>3. Show favorites to other players.</strong></p>
<p align="center"><strong>4. Promoting other games or websites.</strong></p>
<p align="center"><strong>5. Obscene content on your Drag Central related website.</strong></p>
<p align="center"><strong>6. Giving out any information that moderators/administrators can only see.</strong></p>
<p align="center"><strong>7. Never let anyone on your account. </strong></p>
<p align="center"><strong>---Moderator Break---<br>
  We do understand that sometimes things come up and you will need to take a break, we reqire at least a two week notice with the following information: dates you will be absent, the date you will return,<br>
  and the reason for this break request. This information should be emailed to Modbreak@drag-central.com.</strong><br>
</p>
</body>
</html>
