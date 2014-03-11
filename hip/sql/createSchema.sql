--creation table bank
CREATE  TABLE  IF NOT EXISTS "main"."hip_bank" (
  "bank_s" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , 
  "bank_id" VARCHAR NOT NULL  UNIQUE , 
  "name" VARCHAR NOT NULL 
);

INSERT INTO hip_bank (bank_id,name) VALUES ('BANK1','Your Bank');

--creation table account
CREATE  TABLE  IF NOT EXISTS "main"."hip_account" (
  "account_s" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , 
  "bank_s" INTEGER NOT NULL , 
  "name" VARCHAR NOT NULL  UNIQUE , 
  "number" INTEGER,
  "type" VARCHAR,
  "initial_amount" FLOAT NOT NULL DEFAULT 0,
  FOREIGN KEY(bank_s) REFERENCES hip_bank(bank_s)
);

--creation table third
CREATE  TABLE  IF NOT EXISTS "main"."hip_third" (
  "third_s" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE ,
  "name" VARCHAR NOT NULL  UNIQUE
);

INSERT INTO hip_third (third_s,name) VALUES (-1,'');

-- type category
CREATE  TABLE  IF NOT EXISTS "main"."hip_categorytype" (
  "categorytype_s" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE ,
  "name" VARCHAR NOT NULL  UNIQUE
);

INSERT INTO hip_categorytype (categorytype_s,name) VALUES (-1,'');
INSERT INTO hip_categorytype (categorytype_s,name) VALUES (1,'Debit');
INSERT INTO hip_categorytype (categorytype_s,name) VALUES (2,'Credit');
INSERT INTO hip_categorytype (categorytype_s,name) VALUES (3,'Debit & Credit');

--creation table category
CREATE  TABLE  IF NOT EXISTS "main"."hip_category" (
  "category_s" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , 
  "name" VARCHAR NOT NULL  UNIQUE,
  "categorytype_s" INTEGER NOT NULL DEFAULT (-1),
  FOREIGN KEY(categorytype_s) REFERENCES hip_categorytype(categorytype_s)
);

INSERT INTO hip_category (category_s,name,categorytype_s) VALUES (-1,'',-1);

--creation table subcategory
CREATE  TABLE  IF NOT EXISTS "main"."hip_subcategory" (
  "subcategory_s" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE ,
  "category_s" INTEGER NOT NULL ,  
  "name" VARCHAR,
  UNIQUE(category_s,name),
  FOREIGN KEY(category_s) REFERENCES hip_category(category_s)
);

INSERT INTO hip_subcategory (subcategory_s,category_s,name) VALUES (-1,-1,'');

CREATE  TRIGGER "main"."hip_category_tgai" 
AFTER INSERT ON "hip_category" 
FOR EACH ROW  
BEGIN 
  INSERT INTO "hip_subcategory" ("category_s","name") VALUES (new.category_s,'');END
;

--creation table transaction
CREATE  TABLE  IF NOT EXISTS "main"."hip_transaction" (
  "transaction_s" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE ,
  "account_s" INTEGER NOT NULL ,
  "ofxfitid" VARCHAR UNIQUE,
  "date" DATETIME NOT NULL ,
  "state" BOOLEAN DEFAULT (0),
  "info" VARCHAR,
  "third_s" INTEGER NOT NULL ,
  "description" VARCHAR,
  "subcategory_s" INTEGER NOT NULL ,
  "labels" VARCHAR,
  "amount" FLOAT NOT NULL,
  "split_flag" boolean DEFAULT (0),
  "split_group" INTEGER,
  "split_total" FLOAT,  
  FOREIGN KEY(account_s) REFERENCES hip_account(account_s),
  FOREIGN KEY(third_s) REFERENCES hip_third(third_s),
  FOREIGN KEY(subcategory_s) REFERENCES hip_subcategory(subcategory_s)
);

-- Table for budget :
CREATE  TABLE  IF NOT EXISTS "main"."hip_budgetyearly" (
  "budgetyearly_s" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE ,
  "name" VARCHAR UNIQUE
);

-- Table for budget :
CREATE  TABLE  IF NOT EXISTS "main"."hip_budget" (
  "budget_s" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE ,
  "budgetyearly_s" INTEGER NOT NULL,
  "subcategory_s" INTEGER NOT NULL ,
  "monthly_flag" BOOLEAN DEFAULT (0),
  "01" FLOAT NOT NULL DEFAULT (0),
  "02" FLOAT NOT NULL DEFAULT (0),
  "03" FLOAT NOT NULL DEFAULT (0),
  "04" FLOAT NOT NULL DEFAULT (0),
  "05" FLOAT NOT NULL DEFAULT (0),
  "06" FLOAT NOT NULL DEFAULT (0),
  "07" FLOAT NOT NULL DEFAULT (0),
  "08" FLOAT NOT NULL DEFAULT (0),
  "09" FLOAT NOT NULL DEFAULT (0),
  "10" FLOAT NOT NULL DEFAULT (0),
  "11" FLOAT NOT NULL DEFAULT (0),
  "12" FLOAT NOT NULL DEFAULT (0),
  "total" FLOAT NOT NULL DEFAULT (0),
  UNIQUE(budgetyearly_s,subcategory_s),
  FOREIGN KEY(budgetyearly_s) REFERENCES hip_budgetyearly(budgetyearly_s),
  FOREIGN KEY(subcategory_s) REFERENCES hip_subcategory(subcategory_s)
);

CREATE TRIGGER "hip_budget_tgai_01" 
AFTER INSERT ON "hip_budget" 
FOR EACH ROW  
WHEN new.monthly_flag = 0 and new.total=0  
BEGIN 
update hip_budget set '01'=new.'01','02'=new.'01','03'=new.'01','04'=new.'01','05'=new.'01','06'=new.'01',
'07'=new.'01','08'=new.'01','09'=new.'01','10'=new.'01','11'=new.'01','12'=new.'01',total=12 * new.'01' 
where budget_s = new.budget_s; END
;

CREATE TRIGGER "hip_budget_tgai_02" 
AFTER INSERT ON "hip_budget" 
FOR EACH ROW  
WHEN new.monthly_flag = 0 and new.total<>0  
BEGIN 
update hip_budget set '01'=new.total/12,'02'=new.total/12,'03'=new.total/12,'04'=new.total/12,'05'=new.total/12,'06'=new.total/12,
'07'=new.total/12,'08'=new.total/12,'09'=new.total/12,'10'=new.total/12,'11'=new.total/12,'12'=new.total/12 
where budget_s = new.budget_s; END
;

CREATE TRIGGER "hip_budget_tgai_03" 
AFTER INSERT ON "hip_budget" 
FOR EACH ROW  
WHEN new.monthly_flag = 1  
BEGIN 
update hip_budget set total = new.'01'+new.'02'+new.'03'+new.'04'+new.'05'+new.'06'+new.
'07'+new.'08'+new.'09'+new.'10'+new.'11'+new.'12'
where budget_s = new.budget_s; END
;

--creation table assignment
CREATE  TABLE  IF NOT EXISTS "main"."hip_field" (
  "field_s" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE ,
  "name" VARCHAR NOT NULL UNIQUE
);

INSERT INTO hip_field (name) VALUES ('description');

CREATE  TABLE  IF NOT EXISTS "main"."hip_condition" (
  "condition_s" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE ,
  "name" VARCHAR NOT NULL UNIQUE
);

INSERT INTO hip_condition (name) VALUES ('like');

CREATE  TABLE  IF NOT EXISTS "main"."hip_assignment" (
  "assignment_s" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE ,
  "name" VARCHAR UNIQUE,
  "field_s" INTEGER NOT NULL,
  "condition_s" INTEGER NOT NULL,
  "criterion" VARCHAR,
  "third_s" INTEGER NOT NULL DEFAULT (-1),
  "third_overwrite_flag" NOT NULL DEFAULT (0),
  "subcategory_s" INTEGER NOT NULL DEFAULT (-1), 
  "subcategory_overwrite_flag" NOT NULL DEFAULT (0), 
  FOREIGN KEY(third_s) REFERENCES hip_third(third_s),
  FOREIGN KEY(subcategory_s) REFERENCES hip_subcategory(subcategory_s),
  FOREIGN KEY(field_s) REFERENCES hip_field(field_s),
  FOREIGN KEY(condition_s) REFERENCES hip_condition(condition_s)
);
  

-- create view
CREATE  VIEW "main"."v_all_data" 
AS 
SELECT 
  'hip_bank'.bank_s                     'bank_s',
  'hip_bank'.bank_id                    'bank_id',
  'hip_bank'.name                       'bank_name',
  'hip_account'.account_s               'account_s',
  'hip_account'.name                    'account_name',
  'hip_account'.number                  'account_number',
  'hip_account'.initial_amount          'account_initial_amount',
  'hip_categorytype'.categorytype_s     'categorytype_s',
  'hip_categorytype'.name               'categorytype_name',
  'hip_category'.category_s             'category_s',
  'hip_category'.name                   'category_name',
  'hip_subcategory'.subcategory_s       'subcategory_s',
  'hip_subcategory'.name                'subcategory_name',
  'hip_third'.third_s                   'third_s',
  'hip_third'.name                      'third_name',
  'hip_transaction'.transaction_s       'transaction_s',
  'hip_transaction'.ofxfitid            'transaction_ofxfitid',
  'hip_transaction'.date                'transaction_date',
  'hip_transaction'.state               'transaction_state',
  'hip_transaction'.info                'transaction_info',
  'hip_transaction'.description         'transaction_description',
  'hip_transaction'.labels              'transaction_labels',
  'hip_transaction'.amount              'transaction_amount',
  'hip_transaction'.split_flag          'transaction_split_flag',
  'hip_transaction'.split_group         'transaction_split_group',
  'hip_transaction'.split_total         'transaction_split_total'
FROM 
  'hip_bank' left outer join 'hip_account' on ('hip_account'.'bank_s'='hip_bank'.'bank_s')
  left outer join 'hip_transaction' on ('hip_transaction'.'account_s'='hip_account'.'account_s')
  left outer join 'hip_subcategory' on ('hip_transaction'.'subcategory_s'='hip_subcategory'.'subcategory_s')
  left outer join 'hip_category' on ('hip_category'.'category_s'='hip_subcategory'.'category_s')
  left outer join 'hip_categorytype' on ('hip_categorytype'.'categorytype_s'='hip_category'.'categorytype_s')
  left outer join 'hip_third' on ('hip_transaction'.'third_s'='hip_third'.'third_s')
;

create view "main"."v_statistical"
as
select 
  bank_s, bank_name,
  account_s, account_name, 
  categorytype_s, categorytype_name, 
  category_s, category_name, 
  subcategory_s, subcategory_name,
  category_name||':'||subcategory_name as catsubcat_name,
  third_s, third_name,
  transaction_date,
  strftime('%Y',transaction_date) as year,
  strftime('%Y',transaction_date) || '-' ||
  CASE 
    WHEN cast(strftime('%m', transaction_date) as integer) BETWEEN 1 AND 3 THEN '02'
    WHEN cast(strftime('%m', transaction_date) as integer) BETWEEN 4 and 6 THEN '05'
    WHEN cast(strftime('%m', transaction_date) as integer) BETWEEN 7 and 9 THEN '08'
    ELSE '11'
  END as 'year_quarter',
  CASE 
    WHEN cast(strftime('%m', transaction_date) as integer) BETWEEN 1 AND 3 THEN 1
    WHEN cast(strftime('%m', transaction_date) as integer) BETWEEN 4 and 6 THEN 2
    WHEN cast(strftime('%m', transaction_date) as integer) BETWEEN 7 and 9 THEN 3
    ELSE 4
  END as quarter,
  strftime('%Y',transaction_date) || '-' || strftime('%m',transaction_date) as year_month,
  strftime('%m',transaction_date) as month,
  strftime('%d',transaction_date) as day,
  case
    when transaction_amount <0 then transaction_amount
    else 0
  end as debit,
  case
    when transaction_amount >=0 then transaction_amount
    else 0
  end as credit,
  transaction_amount total
from v_all_data
;

CREATE VIEW "v_statistical_pivot"
as
select
  bank_s, bank_name,
  account_s, account_name, 
  categorytype_s, categorytype_name, 
  category_s, category_name, 
  subcategory_s, subcategory_name,
  catsubcat_name,
  third_s, third_name,
  year,
  ROUND(SUM("01"),2) as '01',
  ROUND(SUM("02"),2) as '02',
  ROUND(SUM("03"),2) as '03',
  ROUND(SUM("04"),2) as '04',
  ROUND(SUM("05"),2) as '05',
  ROUND(SUM("06"),2) as '06',
  ROUND(SUM("07"),2) as '07',
  ROUND(SUM("08"),2) as '08',
  ROUND(SUM("09"),2) as '09',
  ROUND(SUM("10"),2) as '10',
  ROUND(SUM("11"),2) as '11',
  ROUND(SUM("12"),2) as '12',
  ROUND(SUM(total),2) as total
from (
  select 
    bank_s, bank_name,
    account_s, account_name, 
    categorytype_s, categorytype_name, 
    category_s, category_name, 
    subcategory_s, subcategory_name,
    catsubcat_name,
    third_s, third_name,
    year,
    case
      when month = '01' then total else 0
    end as '01',
    case
      when month = '02' then total else 0
    end as '02',
    case
      when month = '03' then total else 0
    end as '03',
    case
      when month = '04' then total else 0
    end as '04',
    case
      when month = '05' then total else 0
    end as '05',
    case
      when month = '06' then total else 0
    end as '06',
    case
      when month = '07' then total else 0
    end as '07',
    case
      when month = '08' then total else 0
    end as '08',
    case
      when month = '09' then total else 0
    end as '09',
    case
      when month = '10' then total else 0
    end as '10',
    case
      when month = '11' then total else 0
    end as '11',
    case
      when month = '12' then total else 0
    end as '12',
    total
  from v_statistical
)
group by
  bank_s, bank_name,
  account_s, account_name, 
  categorytype_s, categorytype_name, 
  category_s, category_name, 
  subcategory_s, subcategory_name,
  catsubcat_name,
  third_s, third_name,
  year
;

CREATE VIEW "v_all_budget"
AS
SELECT
  --hip_budgetyearly.budgetyearly_s   'budgetyearly_s', 
  hip_budgetyearly.name             'budgetyearly_name',
  hip_category.category_s           'category_s',
  hip_category.name                 'category_name',
  --hip_subcategory.subcategory_s     'subcategory_s',
  hip_subcategory.name              'subcategory_name',
  hip_category.name||':'||hip_subcategory.name as catsubcat_name,
  hip_budget.*
FROM 
  hip_category,
  hip_subcategory left outer join hip_budget on (hip_subcategory.subcategory_s = hip_budget.subcategory_s), 
  hip_budgetyearly
WHERE hip_category.category_s = hip_subcategory.category_s
  AND hip_budgetyearly.budgetyearly_s = hip_budget.budgetyearly_s
;

create view "main"."v_budget_pivot" 
AS 
SELECT budgetyearly_s,subcategory_s,'01' as month,"01" as amount from hip_budget
union
SELECT budgetyearly_s,subcategory_s,'02' as month,"02" as amount from hip_budget
union
SELECT budgetyearly_s,subcategory_s,'03' as month,"03" as amount from hip_budget
union
SELECT budgetyearly_s,subcategory_s,'04' as month,"04" as amount from hip_budget
union
SELECT budgetyearly_s,subcategory_s,'05' as month,"05" as amount from hip_budget
union
SELECT budgetyearly_s,subcategory_s,'06' as month,"06" as amount from hip_budget
union
SELECT budgetyearly_s,subcategory_s,'07' as month,"07" as amount from hip_budget
union
SELECT budgetyearly_s,subcategory_s,'08' as month,"08" as amount from hip_budget
union
SELECT budgetyearly_s,subcategory_s,'09' as month,"09" as amount from hip_budget
union
SELECT budgetyearly_s,subcategory_s,'10' as month,"10" as amount from hip_budget
union
SELECT budgetyearly_s,subcategory_s,'11' as month,"11" as amount from hip_budget
union
SELECT budgetyearly_s,subcategory_s,'12' as month,"12" as amount from hip_budget
;

create view "main"."v_all_budget_pivot"
AS
SELECT
  hip_budgetyearly.budgetyearly_s   'budgetyearly_s', 
  hip_budgetyearly.name             'budgetyearly_name',
  hip_category.category_s           'category_s',
  hip_category.name                 'category_name',
  hip_subcategory.subcategory_s     'subcategory_s',
  hip_subcategory.name              'subcategory_name',
  v_budget_pivot.month,
  v_budget_pivot.amount  
FROM 
  hip_category,
  hip_subcategory left outer join v_budget_pivot on (hip_subcategory.subcategory_s = v_budget_pivot.subcategory_s), 
  hip_budgetyearly
WHERE hip_category.category_s = hip_subcategory.category_s
  AND hip_budgetyearly.budgetyearly_s = v_budget_pivot.budgetyearly_s 
;

-- create view for display category:subcategory in QTableView
create view "main"."v_hip_subcategory"
AS
SELECT 
subcategory_s,  
case 
  when hip_subcategory.name is null  or hip_subcategory.name=""
  then hip_category.name 
  else hip_category.name || ':' || hip_subcategory.name 
end name
FROM 'hip_subcategory'  inner join 'hip_category' on hip_subcategory.category_s = hip_category.category_s
;

-- create views for export hip to homebank files (xhb)
create view "main"."v_xhb_account"
AS
SELECT 
  '<account key="'||
  account_s ||
  ' pos="'||
  account_s ||
  '" type="'||
  case
    when type is null or type =""
    then 1
    else type
  end  ||
  '" name="'||
  hip_account.name||
  '" number="'||
  number||
  '" bankname="'||
  hip_bank.name||
  '" initial="'||
  initial_amount ||
  '" minimum="0" />' data
FROM hip_account INNER JOIN hip_bank ON hip_account.bank_s=hip_bank.bank_s
order by account_s
;

create view "main"."v_xhb_pay"
AS
SELECT 
  '<pay key="'||
  third_s||
  '" name="'||
  name||
  '"/>' data
FROM hip_third order by third_s
;

create view "main"."v_xhb_cat"
AS
SELECT
  '<cat '||
  'key="'|| hip_subcategory.subcategory_s|| '" ' ||
--  ' flags="24"'|| 
  case 
    when hip_subcategory.name is null  or hip_subcategory.name=""
    then ''
    else 'parent="'|| hip_category.subcategory_s ||'" '
  end || 
--  'flags=""'||
  'name="'||
  case 
    when hip_subcategory.name is null  or hip_subcategory.name=""
    then hip_category.name 
    else hip_subcategory.name 
  end || '" ' ||
  case
    when hip_budget.budget_s  is not null
    then 'b0="'||total||
      '" b1="'||"01"||'" b2="'||"02"||'" b3="'||"03"||'" b4="'||"04"||'" b5="'||"05"||'" b6="'||"06"||
      '" b7="'||"07"||'" b8="'||"08"||'" b9="'||"09"||'" b10="'||"10"||'" b11="'||"11"||'" b12="'||"12"||'" '
    else ''
  end ||
  '/>' data
FROM 'hip_subcategory'  INNER JOIN (
  SELECT hip_subcategory.subcategory_s,hip_category.category_s,hip_category.name,hip_category.categorytype_s
  FROM 'hip_subcategory'  INNER JOIN 'hip_category' ON hip_subcategory.category_s = hip_category.category_s
  WHERE hip_subcategory.name =""
) 'hip_category' ON hip_subcategory.category_s = hip_category.category_s
LEFT OUTER JOIN (
  select * from hip_budget where budgetyearly_s = (select max(budgetyearly_s) from hip_budgetyearly)
) hip_budget ON (hip_budget.subcategory_s=hip_subcategory.subcategory_s)
order by hip_subcategory.subcategory_s
;

create view "main"."v_xhb_asg"
AS
SELECT 
  '<asg key="'|| assignment_s  ||'" '||
  'name='|| REPLACE(hip_assignment.criterion,"%","")  ||'" '||
  case
    when hip_condition.name = 'like' AND instr(criterion,"%")	
    then  'exact="0" '
    else  'exact="1" '
  end ||
  'payee='|| third_s  ||'" '||
  'category='|| subcategory_s  ||'" '||
  '/>' data
FROM hip_assignment INNER JOIN hip_condition ON hip_condition.condition_s = hip_assignment.condition_s
order by assignment_s
;

create view "main"."v_xhb_ope"
AS
SELECT 
   '<ope '||
  'date="'|| CAST(CAST(julianday(hip_transaction.date) - julianday('0001-01-01')+1 AS INT)  AS TEXT) ||'" '||
  'amount="'|| amount ||'" '||
  'account="'|| account_s ||'" '||
  --'flags="1" 
  'payee="'|| third_s ||'" '||
  'category="'|| subcategory_s ||'" '||
  'wording="'|| description ||'" '||
  'info="'|| info ||'" '||
  ' />' data
FROM hip_transaction
order by transaction_s
;

create view "main"."v_xhb"
AS
select data
from (
  select 
  '<?xml version="1.0"?>
  <homebank v="0.69999999999999996">
  <properties title="Frank" car_category="139" auto_weekday="1" />' data,1 tri
  union
  select *,2 from v_xhb_account
  union
  select *,3 from v_xhb_pay
  union
  select *,4 from v_xhb_cat
  --union
  --select *,5 from v_xhb_tag
  union
  select *,6 from v_xhb_asg
  union
  select *,7 from v_xhb_ope
  union
  select '</homebank>',8
)
order by tri
;

-- set version of DB
PRAGMA user_version = 1
;

-- get version of DB
-- PRAGMA user_version


 
