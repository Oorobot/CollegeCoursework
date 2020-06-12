# 选课开发系统

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

###### 触发器和存储过程（MYSQL）

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

