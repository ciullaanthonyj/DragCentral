<?php


include "connect.php";

if(  !($_SERVER['REMOTE_ADDR']=="213.86.83.116" || $_SERVER['QUERY_STRING']=="doitplease" ))
  die ("Git outta here");


echo "============== RECREATING DC DATABASE ===============<br>";
//query("DROP TABLE users;");

query("CREATE TABLE users (
  id bigint(12) NOT NULL auto_increment,
  nick varchar(32) NOT NULL default '' UNIQUE,
  pass varchar(32) NOT NULL default '',
  PRIMARY KEY (id),
  KEY (nick)
) TYPE=MyISAM;");

  query("ALTER TABLE users ADD money bigint(12) NOT NULL default 0");
  query("ALTER TABLE users ADD email varchar(32) NOT NULL default ''");
  query("ALTER TABLE users ADD inroom bigint(12)");
  query("ALTER TABLE users ADD moder int(1) default 0");
  query("ALTER TABLE users ADD admin int(1) default 0");
  query("ALTER TABLE users ADD member int(1) default 0");
  query("ALTER TABLE users ADD male int(1) default 0");
  query("ALTER TABLE users ADD female int(1) default 0");
  query("ALTER TABLE users ADD lastlogintime int(10) default 0");
  query("ALTER TABLE users ADD lastcheckmessagetime int(10) default 0");
  query("ALTER TABLE users ADD invisible int(1) default 0");
  query("ALTER TABLE users ADD lastip varchar(32) NOT NULL default''");
  query("ALTER TABLE users ADD wins int(10) NOT NULL default 0");
  query("ALTER TABLE users ADD loses int(10) NOT NULL default 0");
  query("ALTER TABLE users ADD expires int(10) NOT NULL default 0");
  query("ALTER TABLE users ADD noprivates int(1) NOT NULL default 0");
  query("ALTER TABLE users ADD rank int(10) NOT NULL default 100");
  
  query("insert into users(nick,pass) values('%SYSTEM%','-reserved nick-'");

//query("DROP TABLE messages;");

query("CREATE TABLE messages (
  id bigint(12) NOT NULL auto_increment,
  nick varchar(32) NOT NULL default '',
  message varchar(255) NOT NULL,
  PRIMARY KEY  (id)
) TYPE=MyISAM;");

  query("ALTER TABLE messages ADD room bigint(12)");
  query("ALTER TABLE messages ADD tonick varchar(32)");
  query("ALTER TABLE messages ADD time int(10) default 0");


//query("DROP TABLE rooms;");

query("CREATE TABLE rooms (
  id bigint(12) NOT NULL auto_increment,
  name varchar(32) NOT NULL default '' UNIQUE,
  PRIMARY KEY (id),
  KEY (name)
) TYPE=MyISAM;");

query("ALTER TABLE rooms ADD pass varchar(32)");
query("ALTER TABLE rooms ADD lastvisittime int(10)");
query("ALTER TABLE rooms ADD permanent int(1) default 0");
query("ALTER TABLE rooms ADD creator bigint(12) default NULL");
query("ALTER TABLE rooms ADD maxpeople bigint(12) default 0");
query("ALTER TABLE rooms ADD maxpeopletime int(10) default 0");


query("INSERT INTO rooms (name,permanent) values('Lobby',1);");
//query("INSERT INTO rooms (name,permanent) values('Moderator Help',1);");
//query("INSERT INTO rooms (name,permanent) values('Race Room',1);");

//query("DROP TABLE ban;");

query("CREATE TABLE bans (
  id bigint(12) NOT NULL auto_increment,
  timestamp int(10) NOT NULL default '0',
  PRIMARY KEY (id),
  KEY timestamp (timestamp)
) TYPE=MyISAM;");

// baning from chat or game
query("ALTER TABLE bans ADD chat int(1) default 0");
query("ALTER TABLE bans ADD game int(1) default 0");
// ban length in seconds
query("ALTER TABLE bans ADD length int(10) default 0");
query("ALTER TABLE bans ADD userid int(12) default 0");
query("ALTER TABLE bans ADD moderid int(12) default 0");
query("ALTER TABLE bans ADD reason text NOT NULL");

echo "<br>CARS<br>";
query("DROP TABLE cars");
query("CREATE TABLE cars (
  id bigint(12) NOT NULL auto_increment,
  name varchar(64) NOT NULL default '',
  price bigint(12) default 0,
  membercar int(1) default 0,
  weight int(10) default 0,
  horses int(10) default 0,
  speednum int(1) default 5,
  PRIMARY KEY (id),
  KEY (name)
) TYPE=MyISAM;");

	
query("INSERT INTO cars (id,name,price,membercar,weight,horses,speednum) 
  values(1,'Audi',10000,0,1140,100,5);");
query("INSERT INTO cars (id,name,price,membercar,weight,horses,speednum) 
  values(2,'Skyline GTR',10000,0,1400,130,5);");
query("INSERT INTO cars (id,name,price,membercar,weight,horses,speednum) 
  values(3,'S 2000',10000,0,1300,120,5);");
query("INSERT INTO cars (id,name,price,membercar,weight,horses,speednum) 
  values(4,'Viper',10000,0,1400,150,6);");
query("INSERT INTO cars (id,name,price,membercar,weight,horses,speednum) 
  values(5,'Carerra GT',20000,1,1500,545,6);");

echo "Creating teams<Br>";
query("CREATE TABLE teams (
  id bigint(12) NOT NULL auto_increment,
  name varchar(64) NOT NULL default '',
  creationtime int(10) default 0,
  PRIMARY KEY (id),
  key (name)
) TYPE=MyISAM;");

  query("ALTER TABLE teams ADD wins int(10) NOT NULL default 0");
  query("ALTER TABLE teams ADD loses int(10) NOT NULL default 0");


query("CREATE TABLE usercars (
  id bigint(12) NOT NULL auto_increment,
  userid bigint(12) NOT NULL,
  carid bigint(12) NOT NULL,
  PRIMARY KEY (id)
) TYPE=MyISAM;");

query("ALTER TABLE usercars ADD selected int(1) NOT NULL default 0");
// LEFT NITROS FROM 0 to 100
query("ALTER TABLE usercars ADD nitrousleft float NOT NULL default 0");
query("ALTER TABLE usercars ADD locked int(1) NOT NULL default 0");
query("ALTER TABLE usercars ADD enginedamage float NOT NULL default 0");
 

echo "Creating challenges<Br>";
query("CREATE TABLE challenges (
  id bigint(12) NOT NULL auto_increment,
  userid bigint(12) NOT NULL,
  touserid bigint(12) NOT NULL,
  forfun int default 0,
  formoney int default 0,
  bracket int default 0,
  headsup int default 0,
  brackettime float default 0,
  racetime float default 0,
  wager float default 0,
  PRIMARY KEY(id)
) TYPE=MyISAM;");

query("ALTER TABLE challenges ADD answerwin int NOT NULL default 0");
query("ALTER TABLE challenges ADD answered int NOT NULL default 0");
query("ALTER TABLE challenges ADD answerbrackettime float NOT NULL default 0");
query("ALTER TABLE challenges ADD answerracetime float NOT NULL default 0");
query("ALTER TABLE challenges ADD falsestart int(1) NOT NULL default 0");
query("ALTER TABLE challenges ADD answerfalsestart int(1) NOT NULL default 0");
query("ALTER TABLE challenges ADD reactiontime float NOT NULL default 0");
query("ALTER TABLE challenges ADD answerreactiontime float NOT NULL default 0");
query("ALTER TABLE challenges ADD creationtime int(10) NOT NULL default 0");
query("ALTER TABLE challenges ADD draw int(1) NOT NULL default 0");
query("ALTER TABLE challenges ADD createstart int(1) NOT NULL default 0");
query("ALTER TABLE challenges ADD createend int(1) NOT NULL default 0");
query("ALTER TABLE challenges ADD answerstart int(1) NOT NULL default 0");
query("ALTER TABLE challenges ADD answerend int(1) NOT NULL default 0");
query("ALTER TABLE challenges ADD answerstarttime int(10) NOT NULL default 0");
query("ALTER TABLE challenges ADD hideincoming int(1) NOT NULL default 0");
query("ALTER TABLE challenges ADD hideoutgoing int(1) NOT NULL default 0");
query("ALTER TABLE challenges ADD usercarid bigint(12) NOT NULL default 0");
query("ALTER TABLE challenges ADD answerusercarid bigint(12) NOT NULL default 0");
query("ALTER TABLE challenges ADD forcar int(1) default 0");


// used in whothis.php
//query("DROP TABLE bruteips");

query("CREATE TABLE bruteips (
  id bigint(10) NOT NULL auto_increment,
  ip varchar(32),
  lasttime int(10) NOT NULL default 0,
  count int(10),
  primary key(id) ) 
  TYPE=MyISAM;");

query("CREATE TABLE bruteiphistory (
  id bigint(10) NOT NULL auto_increment,
  bruteipid bigint(10),
  time int(10) NOT NULL default 0,
  primary key(id) )
  TYPE=MyISAM;");

query("ALTER TABLE bruteiphistory ADD query text NOT NULL");

// HELP TABLE FOR PSEUDO UNION (damned MySQL 4.0.25)
query("CREATE TABLE variants (
  id int(2) NOT NULL auto_increment,
  caseone int(1) NOT NULL default 0,
  casetwo int(1) NOT NULL default 0,
  PRIMARY KEY (id)
) TYPE=MyISAM;");

query("INSERT INTO variants (id,caseone,casetwo) values(1,1,0);");
query("INSERT INTO variants (id,caseone,casetwo) values(2,0,1);");

query("CREATE TABLE roomstats (
  id bigint(12) NOT NULL auto_increment,
  roomname varchar(32),
  count int(10) NOT NULL default 0,
  timestamp int(10),
  primary KEY(id)) TYPE=MyISAM;");

query("CREATE TABLE compchallenges (
  id bigint(12) NOT NULL auto_increment,
  userid bigint(12) NOT NULL default 0,
  wager int(10) NOT NULL default 0,
  timestamp int(10) NOT NULL default 0,
  primary KEY(id)) TYPE=MyISAM;");

query("CREATE TABLE money (
  id bigint(12) NOT NULL auto_increment,
  userid bigint(12) NOT NULL default 0,  
  value int(10) NOT NULL default 0,
  time int(10) NOT NULL default 0,
  comment varchar(64),
  primary KEY(id)) TYPE=MyISAM;" );

query("ALTER TABLE money ADD startmoney int(10) NOT NULL default 0");
query("ALTER TABLE money ADD endmoney int(10) NOT NULL default 0");
query("ALTER TABLE money ADD opponentid bigint(12) NOT NULL default 0");

echo"<br>Teams<br>";

//query("DROP TABLE teams");
query("CREATE TABLE teams (
  id bigint(12) NOT NULL auto_increment,
  name varchar(64) NOT NULL default '',  
  money int(10) NOT NULL default 0,
  creationtime int(10) NOT NULL default 0,
  comment text not null default '',
  primary KEY(id)) TYPE=MyISAM;" );

query("CREATE TABLE team_members (
  id bigint(12) NOT NULL auto_increment,
  userid bigint(12) NOT NULL default 0,  
  owner int(1) NOT NULL default 0,
  primary KEY(id)) TYPE=MyISAM;" );
  
query("ALTER TABLE team_members ADD teamid bigint(12) NOT NULL default 0");
query("ALTER TABLE team_members ADD jointime int(10) NOT NULL default 0");
query("ALTER TABLE team_members ADD usercarid bigint(12) NOT NULL default 0");

query("CREATE TABLE team_money (
  id bigint(12) NOT NULL auto_increment,
  teamid bigint(12) NOT NULL default 0,
  userid bigint(12) NOT NULL default 0,  
  value int(10) NOT NULL default 0,
  comment text not null default '',
  primary KEY(id)) TYPE=MyISAM;" );
query("ALTER TABLE team_money ADD o_teamid bigint(12) NOT NULL default 0");


query("CREATE TABLE team_requests (
  id bigint(12) NOT NULL auto_increment,
  teamid bigint(12) NOT NULL default 0,
  userid bigint(12) NOT NULL default 0,  
  accepted int(1) NOT NULL default 0,
  comment text not null default '',
  primary KEY(id)) TYPE=MyISAM;" );
query("ALTER TABLE team_requests ADD declined int(1) NOT NULL default 0");

echo "<br>Team Challenges<br>";
//query("DROP TABLE team_challenges");
query("CREATE TABLE team_challenges (
  id bigint(12) NOT NULL auto_increment,
  teamid bigint(12) NOT NULL default 0,
  o_teamid bigint(12) NOT NULL default 0,  
  bracket int(1) NOT NULL default 0,
  headsup int(1) not null default 0,
  forfun int(1) NOT NULL default 0,
  formoney int(1) NOT NULL default 0,
  wager int(10) NOT NULL default 0,
  creationtime int(10) NOT NULL default 0,
  ended int(1) NOT NULL default 0,
  win int(1) NOT NULL default 0,
  o_win int(1) NOT NULL default 0,
  draw int(1) NOT NULL default 0,
  created int(1) NOT NULL default 0,
  accepted int(1) NOT NULL default 0,
  declined int(1) NOT NULL default 0,  
  primary KEY(id)) TYPE=MyISAM;" );
  

query("CREATE TABLE team_challengers (
  id bigint(12) NOT NULL auto_increment,
  challengeid bigint(12) NOT NULL default 0,
  userid bigint(12) NOT NULL default 0,  
  usercarid bigint(12) NOT NULL default 0,
  reactiontime float NOT NULL default 0,
  racetime float not null default 0,
  brackettime float not null default 0,
  foul int(1) not null default 0,
  started int(1) not null default 0,
  startedtime int(10) not null default 0,
  ended int(1) not null default 0,
  win int(1) NOT null default 0,
  o_userid bigint(12) NOT NULL default 0,
  o_usercarid bigint(12) NOT NULL default 0,
  o_reactiontime float NOT NULL default 0,
  o_racetime float NOT NULL default 0,
  o_brackettime float not null default 0,
  o_win int(1) not null default 0,
  o_foul int(1) not null default 0,
  o_started int(1) not null default 0,
  o_startedtime int(10) not null default 0,
  o_ended int(1) not null default 0, 
  draw int(1) not null default 0,
  primary KEY(id)) TYPE=MyISAM;" );


echo "<br>PARTS<br>";
//query("DROP TABLE partgroups");
query("CREATE TABLE partgroups (
  id bigint(12) NOT NULL auto_increment,
  name varchar(64) NOT NULL default '',
  PRIMARY KEY (id),
  key (name)
) TYPE=MyISAM;");

query("INSERT INTO partgroups(id,name) VALUES(1,'Appearance')");
query("INSERT INTO partgroups(id,name) VALUES(2,'Intake')");
query("INSERT INTO partgroups(id,name) VALUES(3,'NOS')");
query("INSERT INTO partgroups(id,name) VALUES(4,'Suspension/Tires')");
query("INSERT INTO partgroups(id,name) VALUES(5,'Chargers')");
query("INSERT INTO partgroups(id,name) VALUES(6,'Gear Box')");
query("INSERT INTO partgroups(id,name) VALUES(7,'Engines')");
query("INSERT INTO partgroups(id,name) VALUES(8,'Accessories')");
query("INSERT INTO partgroups(id,name) VALUES(9,'LE Cooling')");
query("INSERT INTO partgroups(id,name) VALUES(10,'LE Fluids')");
query("INSERT INTO partgroups(id,name) VALUES(11,'Cooling')");
query("INSERT INTO partgroups(id,name) VALUES(12,'Fluids')");


//query("DROP TABLE parts");
query("CREATE TABLE parts (
  id bigint(12) NOT NULL auto_increment,
  name varchar(64) NOT NULL default '',
  groupid bigint(12) NOT NULL default 0,
  price bigint(12),
  hp int(10) NOT NULL default 0,
  slip int(10) NOT NULL default 0,
  grp int(4) NOT NULL default 0,
  require1 int(10) NOT NULL default 0,
  require2 int(10) NOT NULL default 0,
  require3 int(10) NOT NULL default 0,
  PRIMARY KEY (id),
  key (name)
) TYPE=MyISAM;");

// APPEARANCE
query("INSERT INTO parts (id,groupid,name,price,hp,slip)
  VALUES(1,1,'Auto Meter',350,0,0)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip)
  VALUES(2,1,'Boost Controller',500,0,0)");
// INTAKE
query("INSERT INTO parts (id,groupid,name,price,hp,slip) 
  VALUES(2,2,'Warm Air Intake',150,8,0)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip) 
  VALUES(3,2,'Cold Air Intake',300,24,0)");
// NOS
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp) 
  VALUES(4,3,'Nitrous Oxide 50 Shot',1200,40,0,1)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp) 
  VALUES(5,3,'Nitrous Oxide 100 Shot',2200,80,0,1)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp) 
  VALUES(6,3,'Nitrous Oxide 200 Shot',3500,170,0,1)");
// SUSPENSION 
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)
  VALUES(7,4,'Street Suspension',1000,0,9,3)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)          
  VALUES(7,4,'Sport Suspension',2000,0,12,3)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)
  VALUES(8,4,'Race Suspension',2900,0,35,3)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)
  VALUES(9,4,'Street Tires',250,0,17,4)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)          
  VALUES(8,4,'Drag Radials',500,0,25,4)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)
  VALUES(11,4,'Drag Slicks',1200,0,60,4)");
// CHARGERS
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp) 
  VALUES(9,5,'Turbo Charger',2200,65,0,2)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp) 
  VALUES(10,5,'Super Charger',3200,54,0,2)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip)
  VALUES(11,6,'Magic Gear Box',2700,0,0)");
// ENGINES
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)
  VALUES(12,7,'K24A2 Engine Swap',7000,225,0,5)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)
  VALUES(13,7,'MRZ Engine Swap',60000,855,0,5)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)
  VALUES(14,7,'LS2 Engine Swap',45000,715,0,5)");
// LE COOLING
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)
  VALUES(15,9,'Dcomic's LE Radiator',450,20,0,6)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)
  VALUES(16,9,'Dcomic's LE Thermostat',95,10,0,7)");
// LE FLUIDS
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)
  VALUES(17,10,'Dcomic's LE Coolant Fluid',50,18,0,8)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)
  VALUES(18,10,'Dcomic's LE Lube',200,32,0,9)");
// COOLING
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)
  VALUES(19,11,'Street Radiator',50,3,0,6)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)
  VALUES(20,11,'Sport Radiator',120,6,0,6)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)
  VALUES(21,11,'Drag Radiator',500,14,0,6)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)
  VALUES(22,11,'Sport Thermostat',55,3,0,7)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)
  VALUES(23,11,'Drag Thermostat',115,7,0,7)");
// FLUIDS
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)
  VALUES(24,12,'Street Coolant Fluids',25,3,0,8)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)
  VALUES(25,12,'Sport Coolant Fluids',50,7,0,8)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)
  VALUES(26,12,'Drag Coolant Fluids',65,14,0,8)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)
  VALUES(27,12,'Street Lube',100,9,0,9)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)
  VALUES(28,12,'Sport Lube',195,20,0,9)");
query("INSERT INTO parts (id,groupid,name,price,hp,slip,grp)
  VALUES(29,12,'Drag Lube',245,25,0,9)");

query("CREATE TABLE carparts (
  id bigint(12) NOT NULL auto_increment,
  usercarid bigint(12) NOT NULL,
  partid bigint(12) NOT NULL,
  installed int(1) NOT NULL default 0,
  PRIMARY KEY (id)
) TYPE=MyISAM;");


query("ALTER TABLE carparts ADD gears text default ''");

query("CREATE TABLE bm (
  id bigint(12) NOT NULL auto_increment,
  scriptname varchar(64) NOT NULL default '',
  timestamp int(10) default 0,
  msecs int(10) default 0,
  PRIMARY KEY (id)
) TYPE=MyISAM;");

/*
query("CREATE TABLE bmscripts (
  id bigint(12) NOT NULL auto_increment,
  scriptname varchar(64) NOT NULL default '',
  count int(10) default 0,
  wholetime int(10) default 0,
  PRIMARY KEY (id)
) TYPE=MyISAM;");*/

query("CREATE TABLE ignores (
  id bigint(12) NOT NULL auto_increment,
  userid bigint(12) NOT NULL default 0,
  ignoreid bigint(12) NOT NULL default 0,
  PRIMARY KEY (id)
) TYPE=MyISAM;");

query("CREATE TABLE activation (
  id bigint(12) NOT NULL auto_increment,
  userid bigint(12) NOT NULL default 0,
  emailrandom varchar(32) NOT NULL default '',
  activatenumber varchar(32) NOT NULL default '',
  trycount bigint(10) NOT NULL default 0,
  activated int(1) NOT NULL default 0,
  PRIMARY KEY (id)
) TYPE=MyISAM;");

query("ALTER TABLE activation ADD time int(10) NOT NULL default 0");

query("CREATE TABLE wordsfilter (
  id bigint(12) NOT NULL auto_increment,
  word varchar(32) NOT NULL default '',
  repl varchar(32) NOT NULL default '',
  PRIMARY KEY (id)
) TYPE=MyISAM;");

query("CREATE TABLE online (
  id bigint(12) NOT NULL auto_increment,
  userid bigint(12) NOT NULL default 0,
  lastactivity int(10) NOT NULL default 0,
  PRIMARY KEY (id)
) TYPE=MyISAM;");

query("CREATE TABLE banhistory (
  id bigint(12) NOT NULL auto_increment,
  userid bigint(12) NOT NULL default 0,
  moderid bigint(12) NOT NULL default 0,
  timestamp int(10) NOT NULL default 0,
  length int(10) NOT NULL default 0,
  reason text NOT null default '',
  chatban int(1) NOT NULL default 0,
  gameban int(1) NOT NULL default 0,
  chatkick int(1) NOT NULL default 0,
  PRIMARY KEY (id)
) TYPE=MyISAM;");

query("CREATE TABLE bugs (
  id bigint(12) NOT NULL auto_increment,
  timestamp int(10) NOT NULL default 0,
  bug text NOT null default '',
  author varchar(32) NOT NULL default '',
  fixed int(1) NOT NULL default 0,
  PRIMARY KEY (id)
) TYPE=MyISAM;");


query("CREATE TABLE sysmess (
  id bigint(12) NOT NULL auto_increment,
  lasttimesend int(10) NOT NULL default 0,
  message VARCHAR(200) NOT NULL default '',
  periodsec int(8) NOT NULL default 60,
  PRIMARY KEY (id)
) TYPE=MyISAM;");

query("ALTER TABLE sysmess ADD offsetsec int(8) NOT NULL default 0");

query("CREATE TABLE iptrack (
  id bigint(12) NOT NULL auto_increment,    
  userid bigint(12) NOT NULL default 0,
  ip varchar(32) NOT NULL default'',
  time int(10) NOT NULL default 0,
  PRIMARY KEY(id)
) TYPE=MyISAM;");


query("CREATE TABLE onlinestats (
    id bigint(12) NOT NULL auto_increment,
    count int(10) NOT NULL default 0,
    timestamp int(10),
    primary KEY(id)) TYPE=MyISAM;");

query("CREATE TABLE team_messages (
  id bigint(12) NOT NULL auto_increment,    
  teamid bigint(12) NOT NULL default 0,
  nick varchar(32) NOT NULL default '',
  message text NOT NULL default '',
  time int(10) NOT NULL default 0,
  PRIMARY KEY(id)
) TYPE=MyISAM;");

query("ALTER TABLE team_messages ADD userid bigint(12) NOT NULL default 0");

query("CREATE INDEX team_messages_time ON team_messages(time)");

query("CREATE INDEX iptrack_userid ON iptrack(userid)");

query("CREATE INDEX activation_id ON activation(id)");

query("CREATE INDEX activation_userid ON activation(userid)");

query("CREATE INDEX onlinestats_time ON onlinestats(timestamp)");


query("ALTER TABLE challenges ADD message varchar(254) after touserid");
//also set all permissions on mysql for tables parts,team_challenges,usercars
//to make linked update queries work (some mistic)
  
?>