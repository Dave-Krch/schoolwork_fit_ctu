--nejsou ulozene aktivity
delete from suspect_activities;
select * from find_possible_crimes();

COPY suspect_activities FROM '/mnt/c/Users/dejfd/DataGripProjects/BI-SQL/krchdavi_hw04/activities.csv' DELIMITER ',' CSV HEADER;

-- po nacteni aktivit
select * from find_possible_crimes();

COPY (select * from find_possible_crimes()) TO '/mnt/c/Users/dejfd/DataGripProjects/BI-SQL/krchdavi_hw04/posible_crimes.csv' DELIMITER ',' CSV HEADER;

commit;