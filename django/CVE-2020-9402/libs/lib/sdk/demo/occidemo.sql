/* Copyright (c) 2002, 2010, Oracle and/or its affiliates. 
All rights reserved. */

/*
  NAME
    occidemo.sql - Create OCCI demo objects

  DESCRIPTION
    SQL Script to create OCCI demo objects
    Assumes HR schema is setup
    Assumes system's account passwd to be manager
    Execute this before any of the OCCI demos are run
    To drop the objects created by this SQL use occidemod.sql

  MODIFIED
 mvasudev    09/27/10 - remove HR schema and user SCOTT
  sudsrini   03/06/03  - sudsrini_occi_10ir1_demos (include occidemod)
    idcqe      03/13/01  - Created

*/

/* Drop objects before creating them */ 

@occidemod.sql

connect scott/tiger

CREATE TABLE elements (
  element_name VARCHAR2(25),
  molar_volume BINARY_FLOAT,
  atomic_weight BINARY_DOUBLE
);

CREATE TABLE author_tab ( 
  author_id NUMBER, 
  author_name VARCHAR2(25) 
); 

INSERT INTO author_tab (author_id, author_name) VALUES (333, 'JOE');
INSERT INTO author_tab (author_id, author_name) VALUES (444, 'SMITH');

CREATE OR REPLACE TYPE publ_address AS OBJECT ( 
  street_no NUMBER, 
  city VARCHAR2(25) 
) 
/ 

CREATE TABLE publisher_tab ( 
  publisher_id NUMBER, 
  publisher_add publ_address 
); 

INSERT INTO publisher_tab (publisher_id, publisher_add) VALUES 
(11, publ_address (121, 'NEW YORK'));

CREATE TABLE publ_address_tab OF publ_address; 

INSERT INTO publ_address_tab VALUES (22, 'BOSTON');
INSERT INTO publ_address_tab VALUES (33, 'BUFFALO');
INSERT INTO publ_address_tab VALUES (44, 'CALIFORNIA');


CREATE OR REPLACE TYPE journal AS TABLE OF VARCHAR2(50) 
/ 
CREATE TABLE journal_tab (jid NUMBER, jname journal) 
NESTED TABLE jname STORE AS journal_store;

INSERT INTO journal_tab (jid, jname) VALUES (22, journal ('NATION', 'TIMES'));
INSERT INTO journal_tab (jid, jname) VALUES (33, journal ('CRICKET', 'ALIVE'));

CREATE OR REPLACE TYPE people_obj AS OBJECT ( 
  ssn NUMBER, 
  name VARCHAR2(25) 
) NOT FINAL; 
/ 

CREATE OR REPLACE TYPE librarian UNDER people_obj( 
   empno NUMBER, 
   sal   NUMBER(7,2), 
   dob   DATE, 
   photo BLOB 
) 
/ 

CREATE TABLE librarian_tab OF librarian; 

INSERT INTO librarian_tab VALUES 
(101, 'DAVE', 1001, 10000, '12-Jan-1970', empty_blob());
INSERT INTO librarian_tab VALUES 
(102, 'BOB', 1002, 12000, '17-Jan-1970', empty_blob());

CREATE TABLE article_tab ( 
  artid NUMBER, 
  artdesc VARCHAR2(4000), 
  artsummary LONG,
  artfeedbk VARCHAR2(2000)
); 

CREATE OR REPLACE PROCEDURE demo_proc (col1 IN NUMBER, col2 IN OUT VARCHAR2,
col3 OUT CHAR) AS
BEGIN 
  col2 := col1 || ' ' || col2 || ' ' || 'IN-OUT';
  col3 := 'OUT'; 
END;
/

CREATE OR REPLACE FUNCTION demo_fun (col1 IN NUMBER,
col2 IN OUT VARCHAR2, col3 OUT CHAR) RETURN CHAR AS 
BEGIN 
  col2 := col1 || ' ' || col2 || ' ' || 'IN-OUT'; 
  col3 := 'OUT'; 
  RETURN 'abcd';
END;
/

CREATE TABLE book (bookid NUMBER, summary VARCHAR2(4000));

CREATE TABLE cover (c1 NUMBER(5), c2 VARCHAR2(20));

DECLARE 
ch1 VARCHAR2(4000) := 'aa'; 
ch2 VARCHAR2(4000):= '';
nu NUMBER := 0; 
BEGIN 
  FOR nu IN 1..11 LOOP
    ch2 := ch1 || ch2; ch1 := ch2; 
  END LOOP; 
  INSERT INTO book (bookid, summary) VALUES (11, ch1); 
END;
/

CREATE TYPE elecdoc_typ AS OBJECT
    ( document_typ      VARCHAR2(32)
    , formatted_doc     BLOB
    ) ;
/
CREATE TYPE elecdoc_tab AS TABLE OF elecdoc_typ;
/

CREATE TYPE elheader_typ AS OBJECT
    ( header_name        VARCHAR2(256)
    , creation_date      DATE
    , header_text        VARCHAR2(1024)
    , logo               BLOB
    );
/

CREATE TABLE electronic_media
    ( product_id        NUMBER(6)
    , ad_id             NUMBER(6)
    , ad_composite      BLOB
    , ad_sourcetext     CLOB
    , ad_finaltext      CLOB
    , ad_fltextn        NCLOB
    , ad_elecdocs_ntab  elecdoc_tab
    , ad_photo          BLOB
    , ad_graphic        BFILE
    , ad_header         elheader_typ
    , press_release     LONG
    ) NESTED TABLE ad_elecdocs_ntab STORE AS elecdocs_nestedtab;
CREATE UNIQUE INDEX printmedia_pk
    ON electronic_media (product_id, ad_id);

ALTER TABLE electronic_media
ADD ( CONSTRAINT printmedia__pk
      PRIMARY KEY (product_id, ad_id)
    ) ;



CREATE TYPE people_typ AS OBJECT
(
  name VARCHAR2(30),
  ssn NUMBER,
  dob DATE
) not final;
/

CREATE TABLE people_tab OF people_typ;

INSERT INTO people_tab VALUES (people_typ('john', 111, '01-Jan-1970'));
INSERT INTO people_tab VALUES (people_typ('jill', 666, '06-Jan-1976'));

CREATE TYPE student UNDER people_typ
(
  stud_id NUMBER,
  teammate REF people_typ
) NOT FINAL;
/

CREATE TABLE student_tab OF student;
INSERT INTO student_tab VALUES ('jimmy',222,'02-Feb-1976',200,
(SELECT REF(a) FROM people_tab a where name='john'));

CREATE TYPE parttime_stud UNDER student
(
  course_id NUMBER,
  partner REF student
)NOT FINAL;
/
CREATE TABLE parttime_stud_tab OF parttime_stud;

INSERT INTO parttime_stud_tab VALUES ('james',333,'03-Feb-1976',300,
(SELECT REF(a) FROM people_tab a where name='john'),3000,
(SELECT REF(a) FROM student_tab a));


CREATE TYPE foreign_student UNDER parttime_stud
(
  country VARCHAR2(30),
  leader REF parttime_stud
);
/
CREATE TABLE foreign_student_tab OF foreign_student;

COMMIT;


/* OCCI AQ Objects */


connect system/manager 

grant aq_administrator_role, aq_user_role to scott;
grant execute on dbms_aq to scott;
grant execute on dbms_aqadm to scott;

BEGIN
  dbms_aqadm.grant_system_privilege('ENQUEUE_ANY','scott',FALSE);
  dbms_aqadm.grant_system_privilege('DEQUEUE_ANY','scott',FALSE);
END;
/

connect scott/tiger

CREATE OR REPLACE TYPE scott_obj AS OBJECT
(a1 NUMBER, a2 VARCHAR2(25));
/

BEGIN
  dbms_aqadm.create_queue_table (
  queue_table => 'scott.table01',
  queue_payload_type => 'RAW',
  comment => 'single-consumer',
  multiple_consumers => false,
  compatible => '8.1.0'
);
END;
/

BEGIN
  dbms_aqadm.create_queue (
  queue_name => 'queue01', 
  queue_table=> 'scott.table01' 
);
END;
/
BEGIN
  dbms_aqadm.start_queue(queue_name => 'queue01');
END;
/

BEGIN
  dbms_aqadm.create_queue_table (
  queue_table => 'scott.table02',
  queue_payload_type => 'SYS.ANYDATA',
  comment => 'multi-consumer',
  multiple_consumers => true,
  compatible => '8.1.0'
);
END;
/

BEGIN
  dbms_aqadm.create_queue (
  queue_name => 'queue02',
  queue_table=> 'scott.table02'
);
END;
/
BEGIN
  dbms_aqadm.start_queue(queue_name => 'queue02');
END;
/

BEGIN
  dbms_aqadm.create_queue_table (
  queue_table => 'scott.table03',
  queue_payload_type => 'scott_obj',
  comment => 'multi-consumer',
  multiple_consumers => true,
  compatible => '8.1.0'
);
END;
/

BEGIN
  dbms_aqadm.create_queue (
  queue_name => 'queue03',
  queue_table=> 'scott.table03'
);
END;
/
BEGIN
  dbms_aqadm.start_queue(queue_name => 'queue03');
END;
/

BEGIN
  dbms_aqadm.create_queue_table (
  queue_table => 'scott.table04',
  queue_payload_type => 'RAW',
  comment => 'multiple-consumer',
  multiple_consumers => true,
  compatible => '8.1.0'
);
END;
/

BEGIN
  dbms_aqadm.create_queue (
  queue_name => 'queue04',
  queue_table=> 'scott.table04'
);
END;
/
BEGIN
  dbms_aqadm.start_queue(queue_name => 'queue04');
END;
/

Rem Add default local subscribers to the queues

BEGIN
  dbms_aqadm.add_subscriber( queue_name=> 'queue03',
           subscriber=> sys.aq$_agent('AGT1','scott.queue03', 0));
END;
/

BEGIN
  dbms_aqadm.add_subscriber( queue_name=> 'queue04',
           subscriber=> sys.aq$_agent('AGT1','scott.queue04', 0));
END;
/

-- Add Employees table
DROP TABLE employees;
CREATE TABLE employees (
 EMPLOYEE_ID                                NUMBER(6) NOT NULL,
 FIRST_NAME                                         VARCHAR2(20),
 LAST_NAME                                  VARCHAR2(25) NOT NULL,
 EMAIL                                      VARCHAR2(25) NOT NULL,
 PHONE_NUMBER                                       VARCHAR2(20),
 HIRE_DATE                                  DATE NOT NULL,
 JOB_ID                                     VARCHAR2(10) NOT NULL,
 SALARY                                             NUMBER(8,2),
 COMMISSION_PCT                                     NUMBER(2,2),
 MANAGER_ID                                         NUMBER(6),
 DEPARTMENT_ID                                      NUMBER(4));

INSERT INTO employees VALUES(100,'Steven','King','SKING','515.123.4567','17-JUN-03','AD_PRES',24000,NULL,NULL,90);
INSERT INTO employees VALUES(101,'Neena','Kochhar','NKOCHHAR','515.123.4568','21-SEP-05','AD_VP',17000,NULL,100,90);


