# 选课开发系统

## 数据库设计

> Academy（学院表）：ano（学院号）、name、address、phone
>
> Admin（管理）：id，name，password
>
> Course（课程表）：cno（学院号），credit，hour，name，ano
>
> CourseSchedule（每学期可选择的课）：cno，tno，termId，num，time，classroom
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
   1. 学期管理：开设新学期，修改新学期状态，修改学期名称。
      - Term
   2. 课程管理：添加新课程（Course），删，改。
      - Course，Academy
   3. 学院管理：查看学院信息，可修改，添加
      - Academy
   4. 学生管理：查看所有学生信息，按学号查询，添加学生信息。
      - Student，Academy（**ano**，name），添加Student
   5. 教师管理：查询所有老师信息，按教师号查询，添加老师信息。
      - Teacher，Academy（**ano**，name），添加Teacher，
   6. 开课信息：按学期（termId）查看老师的开课信息（CourseSchedule），此处仅term_status=1时，可以将term_status更新为2.
      - CourseSchedule，Course（**cno**，name），Teacher（**tno**，name），更新termId（**id**，term，status）
2. 教师功能：（tno）
   1. 查看个人信息，仅修改密码。
      - Teacher，Academy（**ano**，name）
   2. 开设课程：仅Term_status=0时，从课程表（Course）中，开设新课程（CourseSchedule），修改，删除；其余只能查看信息。
      - Term，Course，Academy（**ano**，name），添加CourseSchedule
   3. 查看学生：在Term_status >= 1，可从开设课程信息中，从（Elective）中按（cno，tno，termId），查看选课的学生，可踢出学生。
      - Term，CourseSchedule，Elective，Course（cno，name）
   4. 登录分数：Term_status==3时，从（Elective）中按（cno，tno，termId），查看选课的学生，登录分数。
      - Term，CourseSchedule，Elective
3. 学生功能：（sno）
   1. 查看个人信息，可修改密码和电话。
      - Student
   2. 选课功能：只能看到term_id=1的学期，选择学期，根据课号或者课名，在（Course）查询课程，并添加到（Elective）选课表中。可退课。
      - CourseSchedule，Elective，Term
   3. 查看分数：仅可看到term_id=4的学期，选择学期，查看该学期分数。可查看全部课程分数。
      - Elective，Term

