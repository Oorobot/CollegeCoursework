# 选课开发系统（Spring Boot）

## 数据库设计

> Academy（学院）：ano（学院号）、name、address、phone
>
> Admin（管理员）：id，name，password
>
> Course（课程）：cno（学院号），credit，hour，name，ano
>
> CourseSchedule（教师开设的课程）：cno，tno，termId，num，time，classroom，memo，weight
>
> Elective（学生选的课）：cno，tno，termId，sno，usualScore，examScore，totalScore
>
> Student（学生）：sno，ano，name，password，sex，birthday，phone，hometown
>
> Teacher（教师）：tno，ano，name，password，sex，birthday，position，salary
>
> Term（学期）：id，term，status（0：学期开始，1：选课阶段，2：选课结束，3：教师评分，4：学期结束）

## 功能设计（初始密码为123456）

1. 管理员功能：
   1. 学期管理：开设新学期，更新学期状态。
   2. 课程管理：查看/添加/修改课程信息。
   3. 学院管理：查看/添加/修改学院信息。
   4. 学生管理：查看/添加/修改学生信息，初始化学生密码。
   5. 教师管理：查看/添加/修改老师信息，初始化教室密码。
   6. 开课信息：选择学期查看该学期课程信息；查看课程的选课学生信息；学期为“学期开始”时，可以对课程进行备注，可以将学期进入“选课阶段”。
2. 教师功能：（tno）
   1. 查看个人信息（仅可修改姓名，性别，出生日期），修改密码。
   2. 开设课程，仅能够选择处于“学期开始”的学期。
   3. 选择学期查看已开设课程信息；查看课程的选课学生信息；学期状态为“学期开始”时，可以修改课程信息，并且当选课人数为0时，可以删除课程；学期状态为“教师评分”时，可以选择课程进行登记分数。
3. 学生功能：（sno）
   1. 查看个人信息（仅可修改姓名，性别，出生日期，籍贯，联系电话），修改密码。
   2. 选课功能，仅能选择处于“选课阶段”的学期进行课程查询，支持模糊查询。
   3. 选择学期查看课表；当学期状态为“选课阶段”时，可以退课。
   4. 选择学期查询成绩或查看所有学期成绩。

### 触发器和存储过程（MYSQL）

```mysql
CREATE TRIGGER num AFTER INSERT ON elective FOR EACH ROW
BEGIN
	UPDATE course_schedule cs
SET cs.number = (
	SELECT
		COUNT(*)
	FROM
		elective e1
	WHERE
		cs.term_id = e1.term_id
	AND cs.cno = e1.cno
	AND cs.tno = e1.tno
);
END //

DELIMITER //
CREATE PROCEDURE AveragePerTerm (
	IN termId INT,
	IN sno VARCHAR (10),
	OUT ascore INT
)
BEGIN
	SELECT
		AVG(e.total_score) INTO ascore
	FROM
		elective e
	WHERE
		e.sno = sno
	AND e.term_id = termId ;
	END//
DELIMITER ;

DELIMITER //
CREATE PROCEDURE AverageAllTerm (
	IN sno VARCHAR (10),
	OUT ascore INT
)
BEGIN
	SELECT
		AVG(e.total_score) INTO ascore
	FROM
		elective e
	WHERE
		e.sno = sno
	AND e.term_id IN (
		SELECT
			t.id
		FROM
			term t
		WHERE
			t. STATUS = 4
	) ;
	END//
DELIMITER ;
```

### 测试数据

```mysql
insert into academy (ano,name,address,phone) values
('01','计算机学院','上大东区三号楼','65347667'),
('02','通讯学院','上大东区二号楼','65341234'),
('03','材料学院','上大东区四号楼','65347890');

insert into student(sno,name,sex,birthday,hometown,phone,ano，password) values
('1101','李明',0,'1993-03-06','上海','13613005486','02','4QrcOUm6Wau+VuBX8g+IPg=='),
('1102','刘晓明',0,'1992-12-08','安徽','18913457890','01','4QrcOUm6Wau+VuBX8g+IPg=='),
('1103','张颖',1,'1993-01-05','江苏','18826490423','01','4QrcOUm6Wau+VuBX8g+IPg=='),
('1104','刘晶晶',1,'1994-11-06','上海','13331934111','01','4QrcOUm6Wau+VuBX8g+IPg=='),
('1105','刘成刚',0,'1991-06-07','上海','18015872567','01','4QrcOUm6Wau+VuBX8g+IPg=='),
('1106','李二丽',1,'1993-05-04','江苏','18107620945','01','4QrcOUm6Wau+VuBX8g+IPg=='),
('1107','张晓峰',0,'1992-08-16','浙江','13912341078','01','4QrcOUm6Wau+VuBX8g+IPg==');


insert into teacher(tno,name,sex,birthday,position,salary,ano,password) values
('0101','陈迪茂',0,'1973-03-06','副教授',3567.00,'01','4QrcOUm6Wau+VuBX8g+IPg=='),
('0102','马小红',1,'1972-12-08','讲师',2845.00,'01','4QrcOUm6Wau+VuBX8g+IPg=='),
('0201','张心颖',1,'1960-01-05','教授',4200.00,'02','4QrcOUm6Wau+VuBX8g+IPg=='),
('0103','吴宝钢',0,'1980-11-06','讲师',2554.00,'01','4QrcOUm6Wau+VuBX8g+IPg==');

insert into course(cno,name,credit,hour,ano) values
('08305001','离散数学',4,40,'01'),
('08305002','数据库原理',4,50,'01'),
('08305003','数据结构',4,50,'01'),
('08305004','系统结构',6,60,'01'),
('08301001','分子物理学',4,40,'03'),
('08302001','通信学',3,30,'02');

INSERT INTO term(term,status) VALUES
('2012-2013秋季',0),
('2012-2013冬季',0),
('2013-2014秋季',0),
('2013-2014冬季',0);

insert into course_schedule(term_id,cno,tno,time,num) values
(1,'08305001','0103','星期三5-8',50),
(2,'08305002','0101','星期三1-4',50),
(2,'08305002','0102','星期三1-4',40),
(2,'08305002','0103','星期三1-4',40),
(2,'08305003','0102','星期五5-8',50),
(3,'08305004','0101','星期二1-4',40),
(3,'08305001','0102','星期一5-8',50),
(4,'08302001','0201','星期一5-8',50);

insert into elective(sno,term_id,cno,tno,usual_score,exam_score,total_score) values
('1101',1,'08305001','0103',60,60,60),
('1102',1,'08305001','0103',87,87,87),
('1102',2,'08305002','0101',82,82,82),
('1102',3,'08305004','0101',null,null,null),
('1103',1,'08305001','0103',56,56,56),
('1103',2,'08305002','0102',75,75,75),
('1103',2,'08305003','0102',84,84,84),
('1103',3,'08305001','0102',null,null,null),
('1103',3,'08305004','0101',null,null,null),
('1104',1,'08305001','0103',74,74,74),
('1104',4,'08302001','0201',null,null,null),
('1106',1,'08305001','0103',85,85,85),
('1106',2,'08305002','0103',66,66,66),
('1107',1,'08305001','0103',90,90,90),
('1107',2,'08305003','0102',79,79,79),
('1107',3,'08305004','0101',null,null,null);
```

