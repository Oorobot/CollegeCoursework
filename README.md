# 选课开发系统

## 数据库设计

> Academy（学院表）：ano（学院号）、name、address、phone
>
> Admin（管理）：id，name，password
>
> Course（课程表）：cno（学院号），credit，hour，name，ano
>
> CourseSchedule（每学期可选择的课）：cno，tno，termId，num，time，classroom，memo,weight
>
> Elective（学生选课后得到的表）：cno，tno，termId，sno，usualScore，examScore，totalScore
>
> Student（学生表）：sno，ano，name，password，sex，birthday，phone，hometown
>
> Teacher（教师表）：tno，ano，name，password，sex，birthday，position，salary
>
> Term（学期表）：id，term，status（0：教师开设课程，1：学生选课，2：选课结束，3：开始评分，4：学期结束）

## 功能设计（登录，添加的信息初始密码均为123456）

1. 管理员功能：
   1. 学期管理：开设新学期，修改新学期状态。
   2. 课程管理：查看课程信息，添，改。
   3. 学院管理：查看学院信息，添，改。
   4. 学生管理：查看所有学生信息，添，改。
   5. 教师管理：查询所有老师信息，添，改。
   6. 开课信息：按学期（termId）查看老师的开课信息（CourseSchedule），仅开学前一阶段可修改学期状态——开学。
2. 教师功能：（tno）
   1. 查看个人信息，修改密码。
   2. 开设课程：仅Term_status=0时，从课程表（Course）中，开设新课程（CourseSchedule），修改，删除；能查看信息。
   3. 查看学生：在Term_status >= 1，可从开设课程信息中，从（Elective）中按（cno，tno，termId），查看选课的学生。
   4. 登录分数：Term_status==3时，可从开设课程信息中，从（Elective）中按（cno，tno，termId），查看选课的学生，登录分数。
3. 学生功能：（sno）
   1. 查看个人信息，修改密码。
   2. 选课功能：只能看到term_id=1的学期，选择学期，根据课号、课名、教师号、教师名模糊查询，并添加到选课表中，可退课。
   3. 查看分数：仅可看到term_id=4的学期，选择学期，查看该学期分数。

###### 使用了简单的触发器和存储过程（MYSQL）

```sql
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
		e.sno = sno;
	END//
DELIMITER ;
```

