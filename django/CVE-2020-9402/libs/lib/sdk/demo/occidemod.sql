/* Copyright (c) 2002, 2010, Oracle and/or its affiliates. 
All rights reserved. */

/*
  NAME
    occidemod - SQL Script to drop OCCI demo objects

  DESCRIPTION
    SQL Script to drop OCCI demo objects created by occidemo.sql
    To be run in the end to drop OCCI demo objects from HR schema

  MODIFIED
    mvasudev    09/27/10 - remove HR schema and user SCOTT
    sudsrini   03/06/03  - sudsrini_occi_10ir1_demos
    sudsrini   02/21/03  - Created

*/

connect scott/tiger

DROP PROCEDURE demo_proc;
DROP FUNCTION demo_fun;

DROP TABLE elements;
DROP TABLE author_tab;
DROP TABLE publisher_tab;
DROP TABLE publ_address_tab;
DROP TABLE journal_tab;
DROP TABLE article_tab;
DROP TABLE librarian_tab;
DROP TABLE book;
DROP TABLE cover;

DROP TYPE journal;
DROP TYPE publ_address;
DROP TYPE librarian;
DROP TYPE people_obj;


DROP TABLE electronic_media;
DROP TYPE elheader_typ;
DROP TYPE elecdoc_tab;
DROP TYPE elecdoc_typ;

DROP TABLE foreign_student_tab;
DROP TABLE parttime_stud_tab;
DROP TABLE student_tab;
DROP TABLE people_tab;
DROP TYPE foreign_student;
DROP TYPE parttime_stud;
DROP TYPE student;
DROP TYPE people_typ;

/* OCCI AQ Object */

connect system/manager 

revoke aq_administrator_role from scott;

connect scott/tiger

BEGIN
  dbms_aqadm.stop_queue(queue_name => 'queue01');
END;
/

BEGIN
  dbms_aqadm.drop_queue('queue01');
END;
/

BEGIN
  dbms_aqadm.drop_queue_table('scott.table01');
END;
/

BEGIN
  dbms_aqadm.stop_queue(queue_name => 'queue02');
END;
/

BEGIN
  dbms_aqadm.drop_queue('queue02');
END;
/

BEGIN
  dbms_aqadm.drop_queue_table('scott.table02');
END;
/

BEGIN
  dbms_aqadm.stop_queue(queue_name => 'queue03');
END;
/

BEGIN
  dbms_aqadm.drop_queue('queue03');
END;
/

BEGIN
  dbms_aqadm.drop_queue_table('scott.table03');
END;
/

BEGIN
  dbms_aqadm.stop_queue(queue_name => 'queue04');
END;
/

BEGIN
  dbms_aqadm.drop_queue('queue04');
END;
/

BEGIN
  dbms_aqadm.drop_queue_table('scott.table04');
END;
/

DROP TYPE scott_obj;

